#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <linux/socket.h>
#include "cellsocket.h"

#define LOG_TAG "CellSocket"

#include <cutils/log.h>
#include <cutils/memory.h>
#include <cutils/misc.h>

#define MAX_MSG_LEN 1024
#define MAX_HANDLE_MSG_INDEX 10

static int daemon_fd = -1;
static handle_message handle_message_vector[MAX_HANDLE_MSG_INDEX] = {0};

void register_cmd_handle(handle_message h,int flag)
{
	if(flag > -1 && flag < MAX_HANDLE_MSG_INDEX){
		handle_message_vector[flag] = h;
	}
}

void unregister_cmd_handle(int flag)
{
	if(flag > -1 && flag < MAX_HANDLE_MSG_INDEX){
		handle_message_vector[flag] = NULL;
	}
}

static void *handle_client_message(void *opaque)
{
	int ret;
	int handle_flag;
	int cmd_flag;
	char* send_buf;
	int send_count;
	int rec_msg_cmd;
	struct client_message *client_msg = (struct client_message *)opaque;

	rec_msg_cmd = *((int*)client_msg->_rec_msg);
	handle_flag = (rec_msg_cmd >> 16) & 0xff;
	cmd_flag = rec_msg_cmd & 0xff;

	ALOGD("handle_client_message begin handle_flag[%d] cmd_flag[%d]", handle_flag, cmd_flag);
	if(handle_flag > -1 && handle_flag < MAX_HANDLE_MSG_INDEX){
		if(handle_message_vector[handle_flag])
			(*handle_message_vector[handle_flag])(client_msg,cmd_flag);
	}
	ALOGD("handle_client_message end send_msg_len[%d]", client_msg->_send_msg_len);

	/* Send the now prepared reply */
	send_buf = (char*)client_msg->_send_msg;
	send_count = client_msg->_send_msg_len;
	while (send_count > 0) {
		ret = send(client_msg->_socket, send_buf, send_count, 0);
		if (ret <= 0) {
			ALOGE(" handle_client_message send reply: "
				  "ERROR(%d) %s", errno, strerror(errno));
			break;
		}
		send_count -= ret;
		send_buf += ret;
	}


	if(client_msg->_send_msg_len)
		free(client_msg->_send_msg);
	if(client_msg->_rec_msg_len)
		free(client_msg->_rec_msg);
	close(client_msg->_socket);
	free(client_msg);
	return (void *)0; /* Unused */
}

/*
 * Listen for incoming connections and handle them one by one
 *
 * It's a bit rough, but we really expect all data to come through more or less
 * at once.
 */
static void listen_server_daemon(int daemon_fd)
{
	int ret;
	int cmd_socket;
	int rec_msg_cmd;
	int rec_msg_len;
	void *message, *ptr;
	pthread_t handle_thread;
	struct client_message *c_message;
	int rec_count;

	/* OK, we're ready to accept some incoming connections */
	if (listen(daemon_fd, 5) == -1) {
		ALOGE("listen client_data->socket: %s", strerror(errno));
		goto out_err;
	}

	/* Main accept loop */
	for (;;) {

		/* Block and wait for one */
		cmd_socket = accept(daemon_fd,NULL, NULL);
		if (cmd_socket < 0) {
			ALOGE("accept err: %s", strerror(errno));
			goto out_err;
		}

		/* Caught a fish - now fetch and validate the protocol fields */
		ret = recv(cmd_socket, (void *)&rec_msg_len, 4, 0);
		if (ret != 4) {
			ALOGE("recv rec_msg_len err: %s", strerror(errno));
			close(cmd_socket);
			continue;
		}

		if (rec_msg_len > MAX_MSG_LEN) {
			ALOGE("Unexpected rec_msg_len value: %u", rec_msg_len);
			close(cmd_socket);
			continue;
		}

		/* Let's get a buffer for the data */
		message = malloc(rec_msg_len);
		if (!message) {
			ALOGE("malloc message buffer err: %s", strerror(errno));
			close(cmd_socket);
			goto out_err;
		}

		/* This may not come all at once, so receive in chunks */
		rec_count = rec_msg_len;
		while (rec_count > 0) {
			ptr = (char *)message + (rec_msg_len - rec_count);
			ret = recv(cmd_socket, ptr, rec_count, 0);
			if (ret <= 0) {
				ALOGE("recv message err: %s", strerror(errno));
				free(message);
				close(cmd_socket);
				break;
			}

			rec_count -= ret;
		}

		/*
		 * If we received the entire message then go ahead and process
		 * it in its own thread and let the thread close the socket
		 * (and free the message memory) when it's done with it.
		 */
		if (rec_count == 0) {
			c_message = (struct client_message *)malloc(sizeof(*c_message));
			if (!c_message) {
				ALOGE("malloc client_message: "
					  "err(%d) %s", errno, strerror(errno));
				free(message);
				close(cmd_socket);
				goto out_err;
			}

			c_message->_socket = cmd_socket;
			c_message->_rec_msg_len = rec_msg_len;
			c_message->_rec_msg = message;
			c_message->_send_msg_len = 0;
			c_message->_send_msg = 0;

			/* TODO: keep track of these threads */
			ret = pthread_create(&handle_thread, NULL,
						 handle_client_message, (void *)c_message);
			if (ret) {
				ALOGE("pthread_create handle_thread: "
					  "ERROR(%d) %s", errno, strerror(errno));
				free(message);
				free(c_message);
				close(cmd_socket);
				goto out_err;
			}
		} else {
			ALOGE("Could not receive entire message "
				  "(missing %u bytes)", rec_count);
			free(message);
			close(cmd_socket);
		}
	}

out_err:
	return ;
}

static int create_socket(const char *path)
{
	struct sockaddr_un local;
	size_t len;
	int sock;

	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	fcntl(sock, F_SETFD, FD_CLOEXEC);

	local.sun_family = AF_UNIX;
	strncpy(local.sun_path, path, sizeof(local.sun_path));
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(sock, (struct sockaddr *)&local, len) == -1) {
		return -1;
	}

	return sock;
}

/*
 * Creates the socket and registers it with the internal bookkeeping. Each call
 * will create a new thread that accepts new connections and handles incoming
 * data. Remember to close destroy the wifi host before exiting the program.
 */
void create_server_daemon(int bhost)
{
	int ret;
	int fd;
	pthread_t daemon_thread;
	char socket_path[PATH_MAX]={0};

	if(bhost){
		snprintf(socket_path, PATH_MAX, "%s%s","/data/cells/cell1",VM_SOCKET_PATH_C);
	}else{
		snprintf(socket_path, PATH_MAX, "%s",VM_SOCKET_PATH_H);
	}

	ALOGD("creating daemon socket at: %s", socket_path);

	fd = create_socket(socket_path);
	if (fd == -1){
		ALOGD("creating daemon socket at: %s failed.", socket_path);
		return;
	}

	/* Start listening for connections in a main thread */
	listen_server_daemon(fd);

	close(fd);
}

/*
 * Open connection with wifi_proxy and return file descriptor.
 */
static int open_conn(const char* server_socket)
{
	int fd;
	struct sockaddr_un addr;
	size_t addr_len;

	/* Establish connection */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) {
		ALOGE("Failed to connect to celld");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(&addr.sun_path[0], server_socket);
	addr_len = strlen(server_socket) + 1 + offsetof(struct sockaddr_un, sun_path);
	if (connect(fd, (struct sockaddr *) &addr, addr_len) != 0) {
		if (errno == ENOENT)
			ALOGE("server does not appear to be running");
		else
			ALOGE("Failed to connect to server");
		return -1;
	}
	return fd;
}

static int send_buf(int fd, void *data, size_t len)
{
	int ret, remain = len;
	char *buf = (char*)data;
	while (remain > 0) {
		if ((ret = write(fd, buf, remain)) <= 0)
			return -1;
		buf += ret;
		remain -= ret;
	}
	return 0;
}

static int recv_reply(int fd, size_t *len, char *msg)
{
	int ret = 0;
	int ret_val;
	int reply = 0;
	int left = 0;
	char *ptr = NULL;

	ret = recv(fd, (void *)&reply, sizeof(int), 0);
	if (ret != sizeof(int)) {
		ALOGE("get reply :%d\n",ret);
		return 0;
	}

	*len = reply;

	msg = (char*)malloc(reply);
	if (NULL == msg) {
		ALOGE("malloc reply buf fail!\n");
		return -1;
	}
	memset(msg, 0, reply);

	/* This may not come all at once, so receive in chunks */
	left = reply;
	while (left > 0) {
		ptr = msg + (reply - left);
		ret = recv(fd, (void *)ptr, left, 0);
		if (ret <= 0) {
			ALOGE("get reply msg fail!\n");
			free(msg);
			return -1;
		}
		left -= ret;
	}
	ALOGI("get reply msg %s, retval %d\n", msg, reply);

	return ret_val;
}

int send_msg(int bhost,int send_len, const char *send_msg, size_t *reply_len, char *reply_msg)
{
	int fd = 0;
	int ret = 0;
	int pkt_len = sizeof(int) + send_len; /* send_len + send_msg */
	char *pbuf = NULL;
	char socket_path[PATH_MAX]={0};

	if(bhost){
		snprintf(socket_path, PATH_MAX, "%s%s","/data/cells/cell1",VM_SOCKET_PATH_H);
	}else{
		snprintf(socket_path, PATH_MAX, "%s",VM_SOCKET_PATH_C);
	}

	fd = open_conn(socket_path);
	if (fd < 0) {
		ALOGE("open_con fail! socket_path=%s \n",socket_path);
		return -1;
	}

	pbuf = (char*)malloc(pkt_len);
	if (NULL == pbuf) {
		ALOGE("malloc fail!\n");
		ret = -1;
		goto END;
	}
	memset(pbuf, 0, pkt_len);

	memcpy(pbuf, &send_len, sizeof(int));
	memcpy(pbuf+sizeof(int), send_msg, send_len);

	if (send_buf(fd, (void *)pbuf, pkt_len)) {
		ALOGE("sending msg num fail!\n");
		ret = -1;
		goto END;
	}

	recv_reply(fd, reply_len, reply_msg);

END:
	close(fd);
	if (NULL != pbuf) {
		free(pbuf);
		pbuf = NULL;
	}
	return ret;
}



