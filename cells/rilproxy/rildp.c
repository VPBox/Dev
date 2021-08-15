#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pwd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <cutils/sockets.h>
#define LOG_TAG "RIL-P"
#define LOG_NDDEBUG 0
#include <utils/Log.h>
#include <sys/wait.h>
#include <private/android_filesystem_config.h>
#include "rildp.h"
#include "librildp.h"

#define PHONE_PROCESS "radio"

static struct client_socket rild_sock[2];
static struct server_socket rilp_sock;
static struct cache_buf cb;
static struct cache_buf cb1;
static struct cache_buf cb2;

const char *rild_sock_sub_dir1 = "/data/cells/cell1/dev/socket";
const char *rild_sock_sub_dir2 = "/dev/socket";
const char *rilp_sock_sub_dir = "/dev/socket";
const char *rild_debug_sock_name = "mtkrild_dbg";

static struct socket_event s_commands_event1;
static struct socket_event s_listen_event1;

static struct socket_event s_commands_event2;
static struct socket_event s_listen_event2;

static struct socket_event s_connect_event;

static struct socket_event d_commands_event;
static struct socket_event d_listen_event;
static struct socket_event d_commands_event_cell1;
static struct socket_event d_listen_event_cell1;
static struct socket_event d_commands_event_cell2;
static struct socket_event d_listen_event_cell2;

static struct server_debug_info sdi;

static struct client_debug_info cdi[2];

static void listen_client_callback(int fd, short flags, void *param);

static void rild_server_disconnect(scp *s_cp);

static void rild_client_disconnect(slp *s_lp);

static void debug_disconnect(sdp *s_dp);

static slp s_param_listen_socket_ril1 = {
    -1,                                  /* fdListen */
    -1,                                  /* fdCommand */
	0,									 /* accept */
	0,							         /* cache_send */
    PHONE_PROCESS,                       /* processName */
    &s_commands_event1,                  /* commands_event */
    &s_listen_event1,                    /* listen_event */
    NULL,                                /* record_stream */
    NULL,                                /* process_commands_callback */
    listen_client_callback,              /* listen_client_callback */
	rild_client_disconnect,	             /* client_disconnect */
	&cb1,
	&cdi[0],
};

static slp s_param_listen_socket_ril2 = {
    -1,                                  /* fdListen */
    -1,                                  /* fdCommand */
    0,                                   /* accept */
	0,									 /* cache_send */
    PHONE_PROCESS,                       /* processName */
    &s_commands_event2,                  /* commands_event */
    &s_listen_event2,                    /* listen_event */
    NULL,                                /* record_stream */
    NULL,                                /* process_commands_callback */
    listen_client_callback,              /* listen_client_callback */
	rild_client_disconnect,	             /* client_disconnect */
	&cb2,
	&cdi[1],
};


static scp s_param_connect_socket_ril = {
    -1,                                  /* fdConnect */
    PHONE_PROCESS,                       /* processName */
    &s_connect_event,                    /* connect_event */
    NULL,                                /* record_stream */
    NULL,                                /* process_server_data_callback */
	rild_server_disconnect,	             /* server_disconnect */
	&cb,								 /* cache_buf */
	&sdi,
};

static sdp s_param_debug_socket_ril = {
    -1,                                  /* fdListen */
    -1,                                  /* fdCommand */
    &d_commands_event,                   /* d_commands_event */
    &d_listen_event,                     /* d_listen_event */
    NULL,                                /* record_stream */
    NULL,                                /* process_commands_callback */
    NULL,                                /* listen_client_callback */
	debug_disconnect,	                 /* disconnect */
	&sdi,
	cdi,
};

static sdp s_param_debug_socket_ril_cell1 = {
    -1,                                  /* fdListen */
    -1,                                  /* fdCommand */
    &d_commands_event_cell1,             /* d_commands_event */
    &d_listen_event_cell1,               /* d_listen_event */
    NULL,                                /* record_stream */
    NULL,                                /* process_commands_callback */
    NULL,                                /* listen_client_callback */
	debug_disconnect,	                 /* disconnect */
	&sdi,
	cdi,
};

static sdp s_param_debug_socket_ril_cell2 = {
    -1,                                  /* fdListen */
    -1,                                  /* fdCommand */
    &d_commands_event_cell2,             /* d_commands_event */
    &d_listen_event_cell2,               /* d_listen_event */
    NULL,                                /* record_stream */
    NULL,                                /* process_commands_callback */
    NULL,                                /* listen_client_callback */
	debug_disconnect,	                 /* disconnect */
	&sdi,
	cdi,
};

const char *ril_socket_name[RILD_SOCKET_TYPE_NUM] = {
    "rild",
    "rild2",
    "rild-oem",
    "rild-ims",
    "rild-mtk-modem",
    "rild-mtk-ut",
    "rild-mtk-ut-2",
    "rild-dongle"
};

const char *ril_H_socket_name[RILD_SOCKET_TYPE_NUM] = {
    "rild_",
    "rild2_",
    "rild-oem_",
    "rild-ims_",
    "rild-mtk-modem_",
    "rild-mtk-ut_",
    "rild-mtk-ut-2_",
    "rild-dongle_"
};

static void send_buf_packet_to_all_client(uint32_t header, char *buf, int buf_len)
{
    if (rild_sock[0].fission_mode == 1) {
        send_buf_packet(rild_sock[0].s_lp->fd_command, header, buf, buf_len);
    }
    send_buf_packet(rild_sock[1].s_lp->fd_command, header, buf, buf_len);
}

static void listen_client_callback
(
int fd, 
short flags, 
void *param
)
{
    int ret;
    int err;
    int is_phone_socket;
    int fdCommand = -1;
    const char* processName;

	flags = flags;

    struct client_socket *c_sock = (struct client_socket *)param;

    struct sockaddr_un peeraddr;
    socklen_t socklen = sizeof(peeraddr);

    struct ucred creds;
    socklen_t szCreds = sizeof(creds);

    struct passwd *pwd = NULL;

    processName = PHONE_PROCESS;


    fdCommand = accept(fd, (struct sockaddr *)&peeraddr, &socklen);

    if (fdCommand < 0 ) {
        RLOGE("Error on accept() errno:%d", errno);
        socket_event_add_wakeup(c_sock->s_lp->listen_event);
        /* start listening for new connections again */
        return;
    }

    /* check the credential of the other side and only accept socket from
    * phone process
    */
    errno = 0;
    is_phone_socket = 0;

    err = getsockopt(fdCommand, SOL_SOCKET, SO_PEERCRED, &creds, &szCreds);

    if (err == 0 && szCreds > 0) {
        errno = 0;
        pwd = getpwuid(creds.uid);
        if (pwd != NULL) {
            if (strcmp(pwd->pw_name, processName) == 0) {
                is_phone_socket = 1;
            } else {
                RLOGE("RILD can't accept socket from process %s", pwd->pw_name);
            }
        } else {
            RLOGE("Error on getpwuid() errno: %d", errno);
        }
    } else {
        RLOGD("Error on getsockopt() errno: %d", errno);
    }

    if (!is_phone_socket) {
        RLOGE("RILD must accept socket from %s", processName);

        close(fdCommand);
        fdCommand = -1;

        /* start listening for new connections again */
        socket_event_add_wakeup(c_sock->s_lp->listen_event);
        return;
    }

    ret = fcntl(fdCommand, F_SETFL, O_NONBLOCK);

    if (ret < 0) {
        RLOGE ("%s Error setting O_NONBLOCK errno:%d", __func__, errno);
    }

    c_sock->s_lp->fd_command = fdCommand;
	if (c_sock->s_lp->cdi) {
		c_sock->s_lp->cdi->client_fd = fdCommand;
		c_sock->s_lp->cdi->client_status = 1;
	}

    RLOGD("accept %s %s:%s fdCommand:%d", __func__, c_sock->sub_dir, 
            *(c_sock->sock_name + c_sock->index), c_sock->s_lp->fd_command);

    c_sock->s_lp->p_rs = new_record_stream(c_sock->s_lp->fd_command, 
                                            MAX_COMMAND_BYTES);

	if (c_sock->s_lp->cache) {
		c_sock->s_lp->cache->buf = (char *)calloc(1, MAX_COMMAND_BYTES);
		c_sock->s_lp->cache->count = 0;
		c_sock->s_lp->cache->offset = 0;
	}
	
	c_sock->s_lp->accept = 1;

	send_server_data(c_sock->s_sock, c_sock->s_lp);

    if (c_sock->s_lp->process_commands_callback == NULL) {
        c_sock->s_lp->process_commands_callback = 
                        default_process_commands_callback;
    }

    socket_event_set(c_sock->s_lp->commands_event, fdCommand, 1,
                        c_sock->s_lp->process_commands_callback, c_sock);
    socket_event_add_wakeup(c_sock->s_lp->commands_event);

	if (1 == c_sock->s_sock->wait) {
		signal_to_connect_server();
	}
}

static void init_ril(int ril_index)
{
    rilp_sock.type = ril_index;
    rilp_sock.index = ril_index;
	rilp_sock.wait = 0;
    rilp_sock.sub_dir = rilp_sock_sub_dir;
    rilp_sock.sock_name = ril_H_socket_name;
    rilp_sock.s_cp = &s_param_connect_socket_ril;
	rilp_sock.s_cp->sdi->pid = getpid();

    rild_sock[0].index = ril_index;
    rild_sock[0].sock_name = ril_socket_name;
    rild_sock[0].sub_dir = rild_sock_sub_dir1;
    rild_sock[0].s_lp = &s_param_listen_socket_ril1;
	rild_sock[0].sys_id = 1;
	if ((ril_index != RILD) && (ril_index != RILD2)) {
		rild_sock[0].s_lp->listen_client_callback = NULL;
	}

    rild_sock[1].index = ril_index;
    rild_sock[1].sock_name = ril_socket_name;
    rild_sock[1].sub_dir = rild_sock_sub_dir2;
    rild_sock[1].s_lp = &s_param_listen_socket_ril2;
	rild_sock[1].sys_id = 0;;
	if ((ril_index != RILD) && (ril_index != RILD2)) {
		rild_sock[1].s_lp->listen_client_callback = NULL;
	}
}

static void rild_server_disconnect(scp *s_cp)
{
	if (s_cp->fd_connect > 0) {
        close(s_cp->fd_connect);
        s_cp->fd_connect = -1;
        socket_event_del(s_cp->connect_event);
        free_record_stream(s_cp->p_rs);
		if (s_cp->cache) {
			free(s_cp->cache->buf);
			s_cp->cache->buf = NULL;
			s_cp->cache->count = 0;
			s_cp->cache->offset = 0;
		}
		if (!rilp_sock.wait)
			signal_to_connect_server();
    }
}

static void rild_client_disconnect(slp *s_lp)
{
	if (s_lp->fd_command > 0) {
		close(s_lp->fd_command);
        s_lp->fd_command = -1;
		s_lp->accept = 0;
		s_lp->cache_send = 0;
        socket_event_del(s_lp->commands_event);
        free_record_stream(s_lp->p_rs);
        socket_event_add_wakeup(s_lp->listen_event);
		if (s_lp->cache) {
			free(s_lp->cache->buf);
			s_lp->cache->buf = NULL;
			s_lp->cache->count = 0;
			s_lp->cache->offset = 0;
		}
	}
}

static void debug_disconnect(sdp *s_dp)
{
	if (s_dp->fd_command > 0) {
		close(s_dp->fd_command);
        s_dp->fd_command = -1;
        socket_event_del(s_dp->d_commands_event);
        free_record_stream(s_dp->p_rs);
		RLOGV("%s", __func__);
	}
}

static void create_socket_for_ril(int fission_mode)
{
    if (fission_mode == 1) {
        create_client_socket_by_path(&rild_sock[0], &rilp_sock, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP, 
                                                AID_ROOT, AID_RADIO, fission_mode);
    }
    create_client_socket_by_path(&rild_sock[1], &rilp_sock, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP, 
                                            AID_ROOT, AID_RADIO, fission_mode);
}

static void connect_listen_rilp_socket(int fission_mode)
{
    connect_server_socket(&rilp_sock, rild_sock, fission_mode);
}

static void start_listen_rild_client(int fission_mode)
{
    if (fission_mode == 1) {
        start_listen_client(&rild_sock[0]);
    }
    start_listen_client(&rild_sock[1]);
}

static int is_TSystem(const char *path)
{
	struct stat buf;
	if (stat(path, &buf) < 0)
		return 0;
	return S_ISREG(buf.st_mode);
}

int main(int argc, char **argv)
{
    int fission_mode = 1;
    unsigned int ril_index = 0;

    if (argc < 2) {
        RLOGE("input error!\n");
        return -1;
    }

    ril_index = atoi(argv[1]);
    if (ril_index >= RILD_SOCKET_TYPE_NUM) {
        RLOGE("input error! ril_index = %d\n", ril_index);
        return -1;
    }

    RLOGD("ril_index = %d\n", ril_index);

    init_ril(ril_index);
    create_socket_for_ril(fission_mode);
#if 0
	s_param_debug_socket_ril.fd_listen = create_socket_for_debug(rilp_sock_sub_dir, rild_debug_sock_name, ril_index, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH, AID_ROOT, AID_RADIO);
	if (fission_mode == 1)
		s_param_debug_socket_ril_cell1.fd_listen = create_socket_for_debug(rild_sock_sub_dir1, rild_debug_sock_name, ril_index, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH, AID_ROOT, AID_RADIO);
	s_param_debug_socket_ril_cell2.fd_listen = create_socket_for_debug(rild_sock_sub_dir2, rild_debug_sock_name, ril_index, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH, AID_ROOT, AID_RADIO);
#endif
	setuid(AID_RADIO);

    create_pthread();
	start_listen_rild_client(fission_mode);
#if 0
	start_listen_debug(&s_param_debug_socket_ril);
	if (fission_mode == 1)
		start_listen_debug(&s_param_debug_socket_ril_cell1);
	start_listen_debug(&s_param_debug_socket_ril_cell2);
#endif
    connect_listen_rilp_socket(fission_mode);

    if (!rilp_sock.wait)
        signal_to_connect_server();

    RLOGE("starting sleep loop");
    while (1) {
        sleep(UINT32_MAX);
    }

    return 0;
}
