#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <pwd.h>
#define LOG_TAG "LIBRIL-P"
#define LOG_NDDEBUG 0
#include <utils/Log.h>
#include <cutils/sockets.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "librildp.h"

static pthread_mutex_t s_startupMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_startupCond = PTHREAD_COND_INITIALIZER;
static int s_started = 0;

static pthread_mutex_t s_connectMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_connectCond = PTHREAD_COND_INITIALIZER;
static int s_connected = 0;


static int s_fdWakeupRead;
static int s_fdWakeupWrite;

static struct socket_event s_wakeupfd_event;

static fd_set readFds;
static int nfds = 0;

static pthread_mutex_t list_mutex;
#define _MUTEX_ACQUIRE() pthread_mutex_lock(&list_mutex)
#define _MUTEX_RELEASE() pthread_mutex_unlock(&list_mutex)
#define _MUTEX_INIT() pthread_mutex_init(&list_mutex, NULL)
#define _MUTEX_DESTROY() pthread_mutex_destroy(&list_mutex)

static pthread_t s_tid_thread;


static struct socket_event *watch_table[MAX_FD_EVENTS];
static struct socket_event pending_list;

#define RESPONSE_SOLICITED 0
#define RIL_SET_UP_DATA_CALL_REQUEST	27
#define RIL_REQUEST_SET_DATA_ON_TO_MD	2111
#define RIL_REQUEST_MODEM_POWEROFF	2010
#define SET_UP_DATA_CALL_RESPONSE_MAX_LEN	1024
#define MOBILE_NET_STATE_DEACTIVE	0
#define MOBILE_NET_STATE_ACTIVE	1
static unsigned int data_call_seq[2];
static unsigned char data_call_state = MOBILE_NET_STATE_DEACTIVE;
static unsigned int data_call_response_len = 0;
static int data_call_sys = 0;
static char data_call_response[SET_UP_DATA_CALL_RESPONSE_MAX_LEN];

static pthread_mutex_t data_call_mutex;
#define DATA_CALL_MUTEX_ACQUIRE() pthread_mutex_lock(&data_call_mutex)
#define DATA_CALL_MUTEX_RELEASE() pthread_mutex_unlock(&data_call_mutex)
#define DATA_CALL_MUTEX_INIT() pthread_mutex_init(&data_call_mutex, NULL)
#define DATA_CALL_MUTEX_DESTROY() pthread_mutex_destroy(&data_call_mutex)
#define READ_INT_FROM_POINTER(p) (*(unsigned int*)(p))
#define WRITE_INT_TO_POINTER(p, v) do { \
    (*(unsigned int*)(p) = (v));	\
}while(0)

static int is_setup_data_call_response(unsigned char *buf)
{
    int i;

    if (READ_INT_FROM_POINTER(buf) != RESPONSE_SOLICITED) {
        return 0;
    }


    for (i = 0; i < 2; i++) {
        if (READ_INT_FROM_POINTER(buf+4) == data_call_seq[i]) {
            return i;
        }
    }

    if (i >= 2) {
        return 0;
    }

    return 0;
}


static void update_mobile_net_state(struct server_socket *s_sock, unsigned int len, unsigned char *buf)
{
    int i;
    int sys;

    if (s_sock->fission_mode != 1) {
        return;
    }

    DATA_CALL_MUTEX_ACQUIRE();
    /* is a setup_data_call response */
    sys = is_setup_data_call_response(buf);
    if ((sys != 1) && (sys != 0)) {
        DATA_CALL_MUTEX_RELEASE();
        return;
    }

    data_call_response_len = len;
    memcpy(data_call_response, buf, len);
    data_call_state = MOBILE_NET_STATE_ACTIVE;
    data_call_sys = sys;
    DATA_CALL_MUTEX_RELEASE();
}

static int process_mobile_net_request(struct client_socket *c_sock, unsigned int len, unsigned char *buf)
{
    int token;
    int result;
    char header[4];
    int ret = 1;

    if (c_sock->fission_mode != 1) {
        return 0;
    }

    DATA_CALL_MUTEX_ACQUIRE();

    if ((READ_INT_FROM_POINTER(buf) == RIL_REQUEST_SET_DATA_ON_TO_MD) ||
        (READ_INT_FROM_POINTER(buf) == RIL_REQUEST_MODEM_POWEROFF)){
        data_call_state = MOBILE_NET_STATE_DEACTIVE;
        ret = 0;
        goto out;
    }

    if (READ_INT_FROM_POINTER(buf) != RIL_SET_UP_DATA_CALL_REQUEST) {
        ret = 0;
        goto out;
    }

    len = len;
    data_call_seq[c_sock->sys_id] = READ_INT_FROM_POINTER(buf + 4);

    if (data_call_sys == c_sock->sys_id) {
        data_call_state = MOBILE_NET_STATE_DEACTIVE;
        ret = 0;
        goto out;
    }

    if (data_call_state != MOBILE_NET_STATE_ACTIVE) {
        ret = 0;
        goto out;
    }

    token = READ_INT_FROM_POINTER(buf + 4);
    WRITE_INT_TO_POINTER(data_call_response+4, token);

    header[0] = header[1] = 0;
    header[2] = (char)((data_call_response_len >> 8) & 0xff);
    header[3] = (char)((data_call_response_len) & 0xff);
    result = send_buf_to_socket(c_sock->s_lp->fd_command,
                header, sizeof(header));
    if (result == 0)
    send_buf_to_socket(c_sock->s_lp->fd_command,
                data_call_response, data_call_response_len);
    if (c_sock->s_lp->cdi) {
    c_sock->s_lp->cdi->client_tx += (data_call_response_len + sizeof(header));
    }

out:
    DATA_CALL_MUTEX_RELEASE();
	return ret;
}

static int dir_exists(const char *path)
{
	struct stat buf;
	if (stat(path, &buf) < 0)
		return 0;
	return S_ISDIR(buf.st_mode);
}

int socket_exists(const char *path)
{
	struct stat buf;
	if (stat(path, &buf) < 0)
		return 0;
	return S_ISSOCK(buf.st_mode);
}

static int send_buf(int fd, void *data, size_t len)
{
	int ret, remain = len;
	char *buf = data;
	while (remain > 0) {
		if ((ret = write(fd, buf, remain)) <= 0) {
			return -1;
		}
		buf += ret;
		remain -= ret;
	}
	return 0;
}

int send_buf_to_socket(int fd, char *buf, int buf_len)
{
	int ret = 0;
	
	if (fd <= 0) {
		RLOGE("%s fd error, wait fd connected", __func__);
        return -1;
	}

resend:
	ret = send_buf(fd, (void *)buf, buf_len);

	if ((ret < 0) && (errno == EAGAIN || errno == EINTR)) {
		RLOGD("send_buf_to_socket resend errno:%d strerr:%s\n", errno, strerror(errno));
		sleep(1);
		goto resend;
	} else if (ret < 0) {
		RLOGE("send_buf_to_socket error errno:%d strerr:%s\n", errno, strerror(errno));
		close(fd);
		return -1;
	}

    return 0;
}

int recv_len_data(int fd, char *buf, int length)
{
	int remaining;
	int offset;
	int count_read;

    offset = 0;
    remaining = length;
    do {
        count_read = read(fd, buf+offset, remaining);

        if (count_read <= 0 ) {
            RLOGE("%s Hit EOS reading message length", __func__);
            return -1;
        }

        offset += count_read;
        remaining -= count_read;
    } while (remaining > 0);

	return 0;
}


void send_buf_packet(int fd, uint32_t header, char *buf, int buf_len)
{    
    send_buf_to_socket(fd, (char *)&header, sizeof(header));
    send_buf_to_socket(fd, buf, buf_len);
}

static void send_buf_packet_to_client
(
struct client_socket *c_sock, 
char *buf, 
int buf_len
)
{
	if (c_sock[0].fission_mode == 1) {
		send_buf_to_socket(c_sock[0].s_lp->fd_command, buf, buf_len);
	}
    send_buf_to_socket(c_sock[1].s_lp->fd_command, buf, buf_len);
}

void store_server_data(struct server_socket *s_sock, void *p_record, size_t recordlen)
{
    char header[4];

	header[0] = header[1] = 0;
    header[2] = (char)((recordlen >> 8) & 0xff);
    header[3] = (char)((recordlen) & 0xff);

	if ((s_sock->s_cp->cache->offset + recordlen) > MAX_COMMAND_BYTES) {
		RLOGE("out of s_sock->s_cp->cache!");
		return;
	}
	
	memcpy(s_sock->s_cp->cache->buf + s_sock->s_cp->cache->offset, header, sizeof(header));
	s_sock->s_cp->cache->offset += sizeof(header);
	memcpy(s_sock->s_cp->cache->buf + s_sock->s_cp->cache->offset, (char *)p_record, recordlen);
	s_sock->s_cp->cache->offset += recordlen;
	s_sock->s_cp->cache->count++;
	RLOGD("%s %s:%s _header:%x recordlen:%zu count:%d", __func__, s_sock->sub_dir, *(s_sock->sock_name + s_sock->index), *((uint32_t *)header), recordlen, s_sock->s_cp->cache->count);
}

void store_client_data(struct client_socket *c_sock, void *p_record, size_t recordlen)
{
    char header[4];

	header[0] = header[1] = 0;
    header[2] = (char)((recordlen >> 8) & 0xff);
    header[3] = (char)((recordlen) & 0xff);

	if ((c_sock->s_lp->cache->offset + recordlen) > MAX_COMMAND_BYTES) {
		RLOGE("out of c_sock->s_lp->cache!");
		return;
	}
	
	memcpy(c_sock->s_lp->cache->buf + c_sock->s_lp->cache->offset, header, sizeof(header));
	c_sock->s_lp->cache->offset += sizeof(header);
	memcpy(c_sock->s_lp->cache->buf + c_sock->s_lp->cache->offset, (char *)p_record, recordlen);
	c_sock->s_lp->cache->offset += recordlen;
	c_sock->s_lp->cache->count++;
	RLOGD("%s %s:%s _header:%x recordlen:%zu count:%d", __func__, c_sock->sub_dir, *(c_sock->sock_name + c_sock->index), *((uint32_t *)header), recordlen, c_sock->s_lp->cache->count);
}

void send_server_data(struct server_socket *s_sock, slp *s_lp)
{
	int result;
	int i = 0;
	int _offset = 0;
	uint32_t _dataSize;
	uint32_t _header;
	char *_buf;

	if (!s_sock) {
		RLOGE("s_sock is NULL!!!!");
		return;
	}

	if (!s_sock->s_cp->cache) {
		return;
	}

	RLOGD("%s:%d %s:%s count:%d", __func__, __LINE__, s_sock->sub_dir, *(s_sock->sock_name + s_sock->index), s_sock->s_cp->cache->count);
	for (i=0; i<s_sock->s_cp->cache->count; i++) {
		_header = *((uint32_t *)(s_sock->s_cp->cache->buf + _offset));
		_offset += sizeof(_header);
		_dataSize = ntohl(_header);
		_buf = s_sock->s_cp->cache->buf + _offset;
		_offset += _dataSize;
		RLOGV("%s:%d %s:%s _header:%x _dataSize:%d", __func__, __LINE__, s_sock->sub_dir, *(s_sock->sock_name + s_sock->index), _header, _dataSize);
		result = send_buf_to_socket(s_lp->fd_command, (char *)&_header, sizeof(_header));
        if (result == 0)
            result = send_buf_to_socket(s_lp->fd_command, _buf, _dataSize);
		else
			break;

		if (s_lp->cdi) {
			s_lp->cdi->client_tx += (_dataSize + sizeof(_header));
		}
	}
}

void send_client_data(struct client_socket *c_sock, int fd_connect)
{
	int result;
	int i = 0;
	int _offset = 0;
	uint32_t _dataSize;
	uint32_t _header;
	char *_buf;

	if (!c_sock) {
		RLOGE("c_sock is NULL!!!!");
		return;
	}

	if (!c_sock->s_lp->cache) {
		return;
	}

	RLOGD("%s:%d %s:%s count:%d", __func__, __LINE__, c_sock->sub_dir, *(c_sock->sock_name + c_sock->index), c_sock->s_lp->cache->count);
	for (i=0; i<c_sock->s_lp->cache->count; i++) {
		_header = *((uint32_t *)(c_sock->s_lp->cache->buf + _offset));
		_offset += sizeof(_header);
		_dataSize = ntohl(_header);
		_buf = c_sock->s_lp->cache->buf + _offset;
		_offset += _dataSize;
		RLOGV("%s:%d %s:%s _header:%x _dataSize:%d", __func__, __LINE__, c_sock->sub_dir, *(c_sock->sock_name + c_sock->index), _header, _dataSize);
		result = send_buf_to_socket(fd_connect, (char *)&_header, sizeof(_header));
        if (result == 0)
            result = send_buf_to_socket(fd_connect, _buf, _dataSize);
		else
			break;
		if (c_sock->s_sock->s_cp->sdi) {
			c_sock->s_sock->s_cp->sdi->server_tx += (_dataSize + sizeof(_header));
		}
	}
}



static void dump_socket_event(struct socket_event * se)
{
    RLOGV("~~~~ Event %p ~~~~", se);
    RLOGV("     fd      = %d", se->fd);
    RLOGV("     pers    = %d", se->persist);
    RLOGV("     func    = %p", se->func);
    RLOGV("     param   = %p", se->param);
    RLOGV("~~~~~~~~~~~~~~~~~~");
}

static void remove_watch(struct socket_event * se, int index)
{
    RLOGV("~~~~ +remove_watch ~~~~");
    watch_table[index] = NULL;
    se->index = -1;

    FD_CLR(se->fd, &readFds);

    if (se->fd+1 == nfds) {
        int n = 0, i;

        for (i = 0; i < MAX_FD_EVENTS; i++) {
            struct socket_event *rev = watch_table[i];

            if ((rev != NULL) && (rev->fd > n)) {
                n = rev->fd;
            }
        }
        nfds = n + 1;
        RLOGV("~~~~ nfds = %d ~~~~", nfds);
    }
    RLOGV("~~~~ -remove_watch ~~~~");
}

static void add_to_list(struct socket_event *se, struct socket_event *list)
{
    se->next = list;
    se->prev = list->prev;
    se->prev->next = se;
    list->prev = se;
}


static void process_read_readies(fd_set * rfds, int n)
{
	int i;
    RLOGV("~~~~ +process_read_readies (%d) ~~~~", n);
    _MUTEX_ACQUIRE();

    for (i = 0; (i < MAX_FD_EVENTS) && (n > 0); i++) {
        struct socket_event *rev = watch_table[i];
        if (rev != NULL && FD_ISSET(rev->fd, rfds)) {
            add_to_list(rev, &pending_list);
            if (rev->persist == false) {
                remove_watch(rev, i);
            }
            n--;
        }
    }

    _MUTEX_RELEASE();
    RLOGV("~~~~ -process_read_readies (%d) ~~~~", n);
}

static void init_list(struct socket_event * list)
{
    memset(list, 0, sizeof(struct socket_event));
    list->next = list;
    list->prev = list;
    list->fd = -1;
}

void socket_event_init()
{
    _MUTEX_INIT();
	DATA_CALL_MUTEX_INIT();

    FD_ZERO(&readFds);
    init_list(&pending_list);
    memset(watch_table, 0, sizeof(watch_table));
}

void socket_event_set
(struct socket_event * se, 
int fd, 
bool persist, 
socket_event_cb func, 
void * param
)
{
    RLOGV("~~~~ socket_event_set %p ~~~~", se);
    memset(se, 0, sizeof(struct socket_event));
    se->fd = fd;
    se->index = -1;
    se->persist = persist;
    se->func = func;
    se->param = param;
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

void socket_event_add(struct socket_event * se)
{
    int i;
    
    _MUTEX_ACQUIRE();
    RLOGV("%s enter", __func__);
    for (i = 0; i < MAX_FD_EVENTS; i++) {
        if (watch_table[i] == NULL) {
            watch_table[i] = se;
            se->index = i;
            //dump_socket_event(se);
            FD_SET(se->fd, &readFds);
            if (se->fd >= nfds) nfds = se->fd+1;
            break;
        }
    }
    RLOGV("%s event counts:%d", __func__, i);
    _MUTEX_RELEASE();
}

void socket_event_del(struct socket_event * se)
{
    _MUTEX_ACQUIRE();

    if (se->index < 0 || se->index >= MAX_FD_EVENTS) {
        _MUTEX_RELEASE();
        return;
    }

    remove_watch(se, se->index);

    _MUTEX_RELEASE();
}

static void process_wakeup_callback(int fd, short flags, void *param) {
    char buff[16];
    int ret;

	fd = fd;
	flags = flags;
	param = param;

    RLOGV("process_wakeup_callback");

    /* empty our wakeup socket out */
    do {
        ret = read(s_fdWakeupRead, &buff, sizeof(buff));
    } while (ret > 0 || (ret < 0 && errno == EINTR));
}

static void trigger_ev_loop() {
    int ret;
    if (!pthread_equal(pthread_self(), s_tid_thread)) {
        /* trigger event loop to wakeup. No reason to do this,
         * if we're in the event loop thread */
         do {
            ret = write (s_fdWakeupWrite, " ", 1);
         } while (ret < 0 && errno == EINTR);
    }
}

void socket_event_add_wakeup(struct socket_event *se)
{
    socket_event_add(se);
    trigger_ev_loop();
}

void server_disconnect(struct server_socket *ss)
{
    struct client_socket *c_sock = ss->c_sock;

	sleep(3);
    if (c_sock[0].s_lp->client_disconnect) {
		c_sock[0].s_lp->client_disconnect(c_sock[0].s_lp);
		c_sock[0].s_lp->cdi->client_pkg = 0;
		c_sock[0].s_lp->cdi->client_rx  = 0;
		c_sock[0].s_lp->cdi->client_tx  = 0;
	}

	if (c_sock[1].s_lp->client_disconnect) {
		c_sock[1].s_lp->client_disconnect(c_sock[1].s_lp);
		c_sock[1].s_lp->cdi->client_pkg = 0;
		c_sock[1].s_lp->cdi->client_rx  = 0;
		c_sock[1].s_lp->cdi->client_tx  = 0;
	}

	if (ss->s_cp->server_disconnect) {
		ss->s_cp->server_disconnect(ss->s_cp);
		ss->s_cp->sdi->server_pkg = 0;
		ss->s_cp->sdi->server_rx = 0;
		ss->s_cp->sdi->server_tx = 0;
	}
}

void client_disconnect(struct client_socket *cs)
{
    struct client_socket *c_sock = cs->s_sock->c_sock;
    struct server_socket *s_sock = cs->s_sock;

	if (c_sock[0].s_lp->client_disconnect) {
		c_sock[0].s_lp->client_disconnect(c_sock[0].s_lp);
		c_sock[0].s_lp->cdi->client_pkg = 0;
		c_sock[0].s_lp->cdi->client_rx  = 0;
		c_sock[0].s_lp->cdi->client_tx  = 0;
	}

	if (c_sock[1].s_lp->client_disconnect) {
		c_sock[1].s_lp->client_disconnect(c_sock[1].s_lp);
		c_sock[1].s_lp->cdi->client_pkg = 0;
		c_sock[1].s_lp->cdi->client_rx  = 0;
		c_sock[1].s_lp->cdi->client_tx  = 0;
	}

	if (s_sock->s_cp->server_disconnect) {
		s_sock->s_cp->server_disconnect(s_sock->s_cp);
		s_sock->s_cp->sdi->server_pkg = 0;
		s_sock->s_cp->sdi->server_rx = 0;
		s_sock->s_cp->sdi->server_tx = 0;
	}
}

void default_process_commands_callback
(
int fd, 
short flags, 
void *param
)
{
	struct client_socket *c_sock = (struct client_socket *)param;;
    int ret = 0;
    void *p_record;
    size_t recordlen;
    record_stream *p_rs = c_sock->s_lp->p_rs;
    const char *sock = *(c_sock->sock_name + c_sock->index);
    char header[4];

	flags = flags;

    RLOGD("%s %s:%s fd:%d fd:%d", __func__, 
            c_sock->sub_dir, sock, fd, c_sock->s_lp->fd_command);

    for (;;) {
        ret = get_next_record_stream(p_rs, &p_record, &recordlen);

        if (ret == 0 && p_record == NULL) {
            /* end-of-stream */
            RLOGD("%s %s:%s end-of-stream", __func__, c_sock->sub_dir, sock);
            break;
        } else if (ret < 0) {
            break;
        } else if (ret == 0) { /* && p_record != NULL */
            int result;

			if (c_sock->s_lp->cdi) {
				c_sock->s_lp->cdi->client_pkg++;
				c_sock->s_lp->cdi->client_rx += (recordlen + sizeof(header));
				time(&c_sock->s_lp->cdi->last_time);
			}
#if 0
			if (process_mobile_net_request(c_sock, recordlen, (unsigned char*)p_record)) {
				continue;
			}
#endif
			if (c_sock->s_sock->s_cp->fd_connect <= 0) {
				/* store */
				if (c_sock->s_sock->s_cp->cache) {
					store_client_data(c_sock, p_record, recordlen);
				}
				continue;
			}
            header[0] = header[1] = 0;
            header[2] = (char)((recordlen >> 8) & 0xff);
            header[3] = (char)((recordlen) & 0xff);
            result = send_buf_to_socket(c_sock->s_sock->s_cp->fd_connect, 
                                        header, sizeof(header));
            if (result == 0)
                send_buf_to_socket(c_sock->s_sock->s_cp->fd_connect, 
                                        p_record, recordlen);
            RLOGV("%s %p:%zu, %s:%s %d", __func__, 
                    p_record, recordlen, c_sock->sub_dir, sock, result);
			if (c_sock->s_sock->s_cp->sdi) {
				c_sock->s_sock->s_cp->sdi->server_tx += (recordlen + sizeof(header));
			}
        }
    }
    
    if (ret == 0 || !(errno == EAGAIN || errno == EINTR)) {
        /* fatal error or end-of-stream */
        if (ret != 0) {
            RLOGE("error on reading command socket errno:%d errstr:%s\n", errno, strerror(errno));
        } else {
            RLOGW("EOS.  Closing command socket errno:%d errstr:%s\n", errno, strerror(errno));
        }

		if (c_sock->s_lp->cdi) {
			c_sock->s_lp->cdi->client_disc_count++;
			c_sock->s_lp->cdi->client_status = 0;
		}

        client_disconnect(c_sock);
    }
}

static void default_process_server_data_callback
(
int fd, 
short flags, 
void *param
) 
{
	struct server_socket *s_sock = (struct server_socket *)param;
	int ret = 0;
	void *p_record;
	size_t recordlen;
	record_stream *p_rs = s_sock->s_cp->p_rs;
	const char *sock = *(s_sock->sock_name + s_sock->index);
	char header[4];

	flags = flags;

	RLOGD("%s %s:%s fd:%d fd:%d", __func__, 
			s_sock->sub_dir, sock, fd, s_sock->s_cp->fd_connect);
	for (;;) {
		ret = get_next_record_stream(p_rs, &p_record, &recordlen);

		if (ret == 0 && p_record == NULL) {
			/* end-of-stream */
			RLOGD("%s %s:%s end-of-stream", __func__, s_sock->sub_dir, sock);
			break;
		} else if (ret < 0) {
			break;
		} else if (ret == 0) { /* && p_record != NULL */
			int result = 0;

			if (s_sock->s_cp->sdi) {
				s_sock->s_cp->sdi->server_pkg++;
				s_sock->s_cp->sdi->server_rx += (recordlen + sizeof(header));
				time(&s_sock->s_cp->sdi->last_time);
			}

			//update_mobile_net_state(s_sock, recordlen, p_record);

			header[0] = header[1] = 0;
			header[2] = (char)((recordlen >> 8) & 0xff);
			header[3] = (char)((recordlen) & 0xff);

			RLOGV("%s %s:%s _header:%x recordlen:%zu", __func__, s_sock->sub_dir, sock, *((uint32_t *)header), recordlen);
			if (!s_sock->wait && s_sock->s_cp->cache) {
				if (s_sock->fission_mode == 1) {
					if (!s_sock->c_sock[0].s_lp->accept || !s_sock->c_sock[1].s_lp->accept) {
						/* cache */
						RLOGV("func:%s, line:%d", __func__, __LINE__);
						store_server_data(s_sock, p_record, recordlen);
					}
				} else {
					if (!s_sock->c_sock[1].s_lp->accept) {
						/* cache */
						RLOGV("func:%s, line:%d", __func__, __LINE__);
						store_server_data(s_sock, p_record, recordlen);
					}
				}
			}

			if (s_sock->fission_mode == 1) {
				if (s_sock->c_sock[0].s_lp->accept) {
					result = send_buf_to_socket(s_sock->c_sock[0].s_lp->fd_command,
									header, sizeof(header));
					if (result == 0)
						result = send_buf_to_socket(s_sock->c_sock[0].s_lp->fd_command,
												p_record, recordlen);
					RLOGV("%s:%d result:%d", __func__, __LINE__, result);
					if (s_sock->c_sock[0].s_lp->cdi) {
						s_sock->c_sock[0].s_lp->cdi->client_tx += (recordlen + sizeof(header));
					}
				}
			}

			if ((s_sock->c_sock[1].s_lp->accept)) {
				result = send_buf_to_socket(s_sock->c_sock[1].s_lp->fd_command,
									header, sizeof(header));
				if (result == 0)
					result = send_buf_to_socket(s_sock->c_sock[1].s_lp->fd_command,
											p_record, recordlen);
				RLOGV("%s:%d result:%d", __func__, __LINE__, result);
				if (s_sock->c_sock[1].s_lp->cdi) {
					s_sock->c_sock[1].s_lp->cdi->client_tx += (recordlen + sizeof(header));
				}
			}

			RLOGV("%s %p:%zu, %s:%s %d", __func__,
					p_record, recordlen, s_sock->sub_dir, sock, result);
		}
	}

	if (ret == 0 || !(errno == EAGAIN || errno == EINTR)) {
		/* fatal error or end-of-stream */
		if (ret != 0) {
			RLOGE("error on reading command socket errno:%d\n", errno);
		} else {
			RLOGW("EOS.  Closing command socket errno:%d.", errno);
		}

		if (s_sock->s_cp->sdi) {
			s_sock->s_cp->sdi->server_disc_count++;
			s_sock->s_cp->sdi->server_status = 0;
		}

		server_disconnect(s_sock);
	}
}



static void default_listen_client_callback
(
int fd, 
short flags, 
void *param
)
{
    struct client_socket *c_sock = (struct client_socket *)param;
    struct sockaddr_un remote;
	socklen_t len;
    int ret;

	flags = flags;

    len = sizeof(remote);

    fd = accept(fd, (struct sockaddr *)&remote, &len);
    if (fd < 0) {
		RLOGE("listen_rild_qmux_radio_socket accept: %s", strerror(errno));
		return;
	}


    RLOGD("accept %s %s:%s fdCommand:%d", __func__, c_sock->sub_dir, *(c_sock->sock_name + c_sock->index), fd);

    ret = fcntl(fd, F_SETFL, O_NONBLOCK);

    if (ret < 0) {
        RLOGE ("%s Error setting O_NONBLOCK errno:%d", __func__, errno);
    }

    c_sock->s_lp->fd_command = fd;
	if (c_sock->s_lp->cdi) {
		c_sock->s_lp->cdi->client_fd = fd;
		c_sock->s_lp->cdi->client_status = 1;
	}
	
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

    socket_event_set(c_sock->s_lp->commands_event, fd, true,
                        c_sock->s_lp->process_commands_callback, c_sock);
    socket_event_add_wakeup(c_sock->s_lp->commands_event);

	if (1 == c_sock->s_sock->wait) {
		signal_to_connect_server();
	}
}

static void print_readies(fd_set * rfds)
{
	int i;
	
    for (i = 0; (i < MAX_FD_EVENTS); i++) {
        struct socket_event *rev = watch_table[i];
        if (rev != NULL && FD_ISSET(rev->fd, rfds)) {
            RLOGV("DONE: fd=%d is ready", rev->fd);
        }
    }
}

static void remove_from_list(struct socket_event *se)
{
    RLOGV("~~~~ +remove_from_list ~~~~");
    se->next->prev = se->prev;
    se->prev->next = se->next;
    se->next = NULL;
    se->prev = NULL;
    RLOGV("~~~~ -remove_from_list ~~~~");
}


static void fire_pending(void)
{
    RLOGV("~~~~ +fire_pending ~~~~");
    struct socket_event *se = pending_list.next;
    while (se != &pending_list) {
        struct socket_event * next = se->next;
        remove_from_list(se);
        se->func(se->fd, 0, se->param);
        se = next;
    }
    RLOGV("~~~~ -fire_pending ~~~~");
}


void socket_event_loop(void)
{
    int n;
    fd_set rfds;

    for (;;) {
        // make local copy of read fd_set
        memcpy(&rfds, &readFds, sizeof(fd_set));
        //print_readies(&rfds);
        n = select(nfds, &rfds, NULL, NULL, NULL);
        //print_readies(&rfds);
        RLOGV("~~~~ %d events fired ~~~~", n);
        if (n < 0) {
            if (errno == EINTR) continue;

            RLOGE("socket_event: select error (%d)", errno);
            return;
        }
        process_read_readies(&rfds, n);

        fire_pending();
    }
}


static void *event_loop(void *param)
{
    int ret;
    int filedes[2];

	param = param;

    socket_event_init();

    pthread_mutex_lock(&s_startupMutex);

    s_started = 1;
    pthread_cond_broadcast(&s_startupCond);

    pthread_mutex_unlock(&s_startupMutex);

    ret = pipe(filedes);

    if (ret < 0) {
        RLOGE("Error in pipe() errno:%d", errno);
        return NULL;
    }

    s_fdWakeupRead = filedes[0];
    s_fdWakeupWrite = filedes[1];

    fcntl(s_fdWakeupRead, F_SETFL, O_NONBLOCK);

    socket_event_set(&s_wakeupfd_event, s_fdWakeupRead, true,
                process_wakeup_callback, NULL);

    socket_event_add_wakeup(&s_wakeupfd_event);

    // Only returns on error
    socket_event_loop();
    RLOGE ("error in event_loop_base errno:%d", errno);
    // kill self to restart on error
    kill(0, SIGKILL);

    return NULL;
}

static void _create_pthread(void *(*start_routine)(void *), void *arg)
{
    /* spin up event_loop thread and wait for it to get started */
    s_started = 0;
    pthread_mutex_lock(&s_startupMutex);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int result = pthread_create(&s_tid_thread, &attr, start_routine, arg);
    if (result != 0) {
        RLOGE("Failed to create dispatch thread: %s", strerror(result));
        goto done;
    }

    while (s_started == 0) {
        pthread_cond_wait(&s_startupCond, &s_startupMutex);
    }

done:
    pthread_mutex_unlock(&s_startupMutex);
}

void create_pthread(void)
{
    _create_pthread(event_loop, NULL);
}

static void pthread_started(void)
{
    pthread_mutex_lock(&s_startupMutex);

    s_started = 1;
    pthread_cond_broadcast(&s_startupCond);

    pthread_mutex_unlock(&s_startupMutex);
}

int create_client_socket_by_path
(
struct client_socket *c_sock, 
struct server_socket *s_sock, 
mode_t perm,
uid_t uid, 
gid_t gid, 
int fission_mode
)
{
	struct sockaddr_un addr;
	int fd, ret;
    const char *sock = *(c_sock->sock_name + c_sock->index);

    {
		while (!dir_exists(c_sock->sub_dir)) {
			sleep(1);
		}
	}
    
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		RLOGE("Failed to open socket '%s': %s\n", 
                c_sock->sub_dir, strerror(errno));
		return -1;
	}

	memset(&addr, 0 , sizeof(addr));
	addr.sun_family = AF_UNIX;
    
	snprintf(addr.sun_path, sizeof(addr.sun_path), 
	            "%s/%s", c_sock->sub_dir, sock);

	ret = unlink(addr.sun_path);
	if (ret != 0 && errno != ENOENT) {
		RLOGE("Failed to unlink old socket '%s:%s': %s\n", 
                c_sock->sub_dir, sock, strerror(errno));
		goto out_close;
	}

	ret = bind(fd, (struct sockaddr *) &addr, sizeof (addr));
	if (ret) {
		RLOGE("Failed to bind socket '%s:%s': %s\n", 
                c_sock->sub_dir, sock, strerror(errno));
		goto out_unlink;
	}

	chown(addr.sun_path, uid, gid);
    if (perm)
        chmod(addr.sun_path, perm);

	RLOGI("Created socket '%s:%s' fd:%d", c_sock->sub_dir, sock, fd);

	c_sock->s_lp->fd_listen = fd;
    c_sock->s_sock = s_sock;
    c_sock->fission_mode = fission_mode;
	
	return fd;
	
out_unlink:
	unlink(addr.sun_path);
out_close:
	close(fd);
	return -1;
}

int create_socket_for_debug(const char *path, const char *name, int index, mode_t perm, uid_t uid, gid_t gid)
{
	struct sockaddr_un addr;
	int fd, ret;
    
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		RLOGE("Failed to open debug socket '%s:%s': %s\n", path, name, strerror(errno));
		return -1;
	}

	memset(&addr, 0 , sizeof(addr));
	addr.sun_family = AF_UNIX;
    
	snprintf(addr.sun_path, sizeof(addr.sun_path), "%s/%s%d", path, name, index);

	ret = unlink(addr.sun_path);
	if (ret != 0 && errno != ENOENT) {
		RLOGE("Failed to unlink old debug socket : %s\n", strerror(errno));
		goto out_close;
	}

	ret = bind(fd, (struct sockaddr *) &addr, sizeof (addr));
	if (ret) {
		RLOGE("Failed to bind debug socket : %s\n", strerror(errno));
		goto out_unlink;
	}

	chown(addr.sun_path, uid, gid);
    if (perm)
        chmod(addr.sun_path, perm);

	RLOGI("Created debug socket '%s' fd:%d", path, fd);
	
	return fd;
	
out_unlink:
	unlink(addr.sun_path);
out_close:
	close(fd);
	return -1;
}


int start_listen_client
(
struct client_socket *c_sock
)
{
    int ret;
    const char *sock = *(c_sock->sock_name + c_sock->index);
    slp *param = c_sock->s_lp;
    
    ret = listen(param->fd_listen, 4);
    if (ret < 0) {
        RLOGE("Failed to listen on control socket '%d' %s:%s : %s",
             param->fd_listen, c_sock->sub_dir, sock, strerror(errno));
        exit(-1);
    }

    RLOGD("%s %s:%s", __func__, c_sock->sub_dir, sock);

    if (param->listen_client_callback == NULL) {
        param->listen_client_callback = default_listen_client_callback;
    }

    socket_event_set(param->listen_event, param->fd_listen, false,
                param->listen_client_callback, c_sock);

    socket_event_add_wakeup(param->listen_event);

    return 0;
}

void default_process_debug_commands_callback
(
int fd, 
short flags, 
void *param
)
{
	sdp *s_dp = (sdp *)param;;
    int ret = 0;
    void *p_record;
    size_t recordlen;
    record_stream *p_rs = s_dp->p_rs;
	char header[4];

	flags = flags;

    RLOGV("%s fd:%d fd:%d", __func__, fd, s_dp->fd_command);

    for (;;) {
        ret = get_next_record_stream(p_rs, &p_record, &recordlen);

        if (ret == 0 && p_record == NULL) {
            /* end-of-stream */
            RLOGV("%s end-of-stream", __func__);
            break;
        } else if (ret < 0) {
            break;
        } else if (ret == 0) { /* && p_record != NULL */
        	int len = sizeof(struct debug_info);
        	header[0] = header[1] = 0;
			header[2] = (char)((len >> 8) & 0xff);
			header[3] = (char)((len) & 0xff);

			RLOGV("%s %zu", __func__, recordlen);

			if (!memcmp(p_record, "debug", 5)) {
				send_buf_to_socket(s_dp->fd_command, header, sizeof(header));
				send_buf_to_socket(s_dp->fd_command, (char *)s_dp->sdi, sizeof(struct server_debug_info));
				send_buf_to_socket(s_dp->fd_command, (char *)s_dp->cdi, sizeof(struct client_debug_info)*2);
			}
        }
    }
    
    if (ret == 0 || !(errno == EAGAIN || errno == EINTR)) {
        /* fatal error or end-of-stream */
        if (ret != 0) {
            RLOGE("error on reading command socket errno:%d errstr:%s\n", errno, strerror(errno));
        } else {
            RLOGW("EOS.  Closing command socket errno:%d errstr:%s\n", errno, strerror(errno));
        }
		if (s_dp->disconnect)
			s_dp->disconnect(s_dp);
    }
}

static void default_listen_debug_callback
(
int fd, 
short flags, 
void *param
)
{
    sdp *s_dp = (sdp *)param;
    struct sockaddr_un remote;
	socklen_t len;
    int ret;

	flags = flags;

    len = sizeof(remote);

    fd = accept(fd, (struct sockaddr *)&remote, &len);
    if (fd < 0) {
		RLOGE("default_listen_debug_callback accept: %s", strerror(errno));
		return;
	}


    RLOGD("accept %s fdCommand:%d", __func__, fd);

    ret = fcntl(fd, F_SETFL, O_NONBLOCK);

    if (ret < 0) {
        RLOGE ("%s Error setting O_NONBLOCK errno:%d", __func__, errno);
    }

    s_dp->fd_command = fd;
    s_dp->p_rs = new_record_stream(s_dp->fd_command, MAX_COMMAND_BYTES);
	
    if (s_dp->process_commands_callback == NULL) {
        s_dp->process_commands_callback = 
                        default_process_debug_commands_callback;
    }

    socket_event_set(s_dp->d_commands_event, fd, true, s_dp->process_commands_callback, s_dp);
    socket_event_add_wakeup(s_dp->d_commands_event);
}


int start_listen_debug(sdp *s_dp)
{
    int ret;
    
    ret = listen(s_dp->fd_listen, 4);
    if (ret < 0) {
        RLOGE("Failed to listen on control socket '%d' %s", s_dp->fd_listen, strerror(errno));
        exit(-1);
    }

    RLOGD("%s", __func__);

    if (s_dp->listen_client_callback == NULL) {
        s_dp->listen_client_callback = default_listen_debug_callback;
    }

    socket_event_set(s_dp->d_listen_event, s_dp->fd_listen, true, s_dp->listen_client_callback, s_dp);

    socket_event_add_wakeup(s_dp->d_listen_event);

    return 0;
}

int connect_debug_server(const char *path, const char *name, int index)
{
    char _path[256];
    int fd;
	int ret;

    RLOGI("%s", __func__);

    snprintf(_path, sizeof(_path), "%s/%s%d", path, name, index);
        
    if (!socket_exists(_path)) {
		return -1;
	}

    fd = connect_to_socket(_path);
	if (fd < 0) {
		RLOGE("%s connect error, errno:%d, strerror:%s", __func__, errno, strerror(errno));
		return -1;
	}

	return fd;
}



int connect_to_socket(char *path)
{
	int sd;
	struct sockaddr_un addr;
	size_t addr_len;

	/* Establish connection */
	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sd == -1) {
		RLOGE("Failed to connect to rilp, socket fail, erro=%d", errno);
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    RLOGD("connect_to_socket path:%s", path);
	addr_len = sizeof(addr);
	if (connect(sd, (struct sockaddr *) &addr, addr_len) != 0) {
		if (errno == ENOENT)
			RLOGE("Failed to connect to rilp: does not appear to be running\n");
		else
			RLOGE("Failed to connect to rilp, path=%s, errno=%d", addr.sun_path, errno);
             close(sd);
		return -1;
	}
	return sd;
}

static void set_connected_val(int val)
{
    pthread_mutex_lock(&s_connectMutex);
    s_connected = val;
    pthread_mutex_unlock(&s_connectMutex);
}

static void _connect_server(struct server_socket *s_sock, int wait)
{
    char path[256];
    char *sock = *(s_sock->sock_name + s_sock->index);
    int fd;
	int ret;

    snprintf(path, sizeof(path), "%s/%s", s_sock->sub_dir, sock);
    for (;;) {
        pthread_mutex_lock(&s_connectMutex);
        while (s_connected < wait) {
            RLOGI("%s path:%s s_connected:%d, wait:%d",
                    __func__, path, s_connected, wait);
            pthread_cond_wait(&s_connectCond, &s_connectMutex);
        }
        pthread_mutex_unlock(&s_connectMutex);

        RLOGI("%s path:%s s_connected:%d, wait:%d wait succ",
                            __func__, path, s_connected, wait);

        set_connected_val(0);
        
        while (!socket_exists(path)) {
			sleep(1);
		}

        while((fd = connect_to_socket(path)) < 0) {
    		sleep(2);
    	}

		ret = fcntl(fd, F_SETFL, O_NONBLOCK);

	    if (ret < 0) {
	        RLOGE ("%s Error setting O_NONBLOCK errno:%d", __func__, errno);
	    }

		/* send */
		send_client_data(&s_sock->c_sock[0], fd);
		send_client_data(&s_sock->c_sock[1], fd);
		
        s_sock->s_cp->fd_connect = fd;
		s_sock->s_cp->sdi->server_fd = fd;
		s_sock->s_cp->sdi->server_status = 1;

        s_sock->s_cp->p_rs = new_record_stream(s_sock->s_cp->fd_connect,
                                                MAX_COMMAND_BYTES);

		if (s_sock->s_cp->cache) {
			s_sock->s_cp->cache->buf = (char *)calloc(1, MAX_COMMAND_BYTES);
			s_sock->s_cp->cache->count = 0;
			s_sock->s_cp->cache->offset = 0;
		}

        if (s_sock->s_cp->process_server_data_callback == NULL) {
            s_sock->s_cp->process_server_data_callback = default_process_server_data_callback;
        }

        RLOGI("%s %s:%s s_connectCond:%d exit", __func__, s_sock->sub_dir,
                *(s_sock->sock_name + s_sock->index), s_connected);
        
        socket_event_set(s_sock->s_cp->connect_event, fd, true,
                    s_sock->s_cp->process_server_data_callback, s_sock);

        socket_event_add_wakeup(s_sock->s_cp->connect_event);
    }
}

void signal_to_connect_server(void)
{
    pthread_mutex_lock(&s_connectMutex);
    s_connected++;
    pthread_cond_broadcast(&s_connectCond);
    pthread_mutex_unlock(&s_connectMutex);
}

static void *connect_server(void *param)
{
    struct server_socket *s_sock = (struct server_socket *)param;
    char path[256];
    const char *sock = *(s_sock->sock_name + s_sock->index);
    int fd;

    pthread_mutex_lock(&s_startupMutex);

    s_started = 1;
    pthread_cond_broadcast(&s_startupCond);

    pthread_mutex_unlock(&s_startupMutex);

	_connect_server(s_sock, 1);

    return (void *)0;
}

void connect_server_socket
(
struct server_socket *s_sock, 
struct client_socket *c_sock, 
int fission_mode
)
{
    const char *sock = *(s_sock->sock_name + s_sock->index);

    s_sock->fission_mode = fission_mode;
    s_sock->c_sock = c_sock;

    RLOGI("connect_server_socket '%s:%s'", s_sock->sub_dir, sock);

    _create_pthread(connect_server, (void *)s_sock);
}

record_stream *new_record_stream(int fd, size_t maxRecordLen)
{
    record_stream *ret;

    ret = (record_stream *)calloc(1, sizeof(record_stream));

    ret->fd = fd;
    ret->maxRecordLen = maxRecordLen;
    ret->buffer = (unsigned char *)malloc (maxRecordLen + HEADER_SIZE);

    ret->unconsumed = ret->buffer;
    ret->read_end = ret->buffer;
    ret->buffer_end = ret->buffer + maxRecordLen + HEADER_SIZE;

    return ret;
}

void free_record_stream(record_stream *rs)
{
    free(rs->buffer);
    free(rs);
}


static unsigned char * get_end_of_record (unsigned char *p_begin,
                                            unsigned char *p_end)
{
    size_t len;
    unsigned char * p_ret;

    if (p_end < p_begin + HEADER_SIZE) {
        return NULL;
    }

    //First four bytes are length
    len = ntohl(*((uint32_t *)p_begin));

    p_ret = p_begin + HEADER_SIZE + len;

    if (p_end < p_ret) {
        return NULL;
    }

    return p_ret;
}


static void *get_next_record (record_stream *p_rs, size_t *p_outRecordLen)
{
    unsigned char *record_start, *record_end;

    record_end = get_end_of_record(p_rs->unconsumed, p_rs->read_end);

    if (record_end != NULL) {
        /* one full line in the buffer */
        record_start = p_rs->unconsumed + HEADER_SIZE;
        p_rs->unconsumed = record_end;

        *p_outRecordLen = record_end - record_start;

        return record_start;
    }

    return NULL;
}

int get_next_record_stream (record_stream *p_rs, void ** p_outRecord,
                                    size_t *p_outRecordLen)
{
    void *ret;

    ssize_t countRead;

    /* is there one record already in the buffer? */
    ret = get_next_record (p_rs, p_outRecordLen);

    if (ret != NULL) {
        *p_outRecord = ret;
        return 0;
    }

    // if the buffer is full and we don't have a full record
    if (p_rs->unconsumed == p_rs->buffer
        && p_rs->read_end == p_rs->buffer_end
    ) {
        // this should never happen
        //ALOGE("max record length exceeded\n");
        assert (0);
        errno = EFBIG;
        return -1;
    }

    if (p_rs->unconsumed != p_rs->buffer) {
        // move remainder to the beginning of the buffer
        size_t toMove;

        toMove = p_rs->read_end - p_rs->unconsumed;
        if (toMove) {
            memmove(p_rs->buffer, p_rs->unconsumed, toMove);
        }

        p_rs->read_end = p_rs->buffer + toMove;
        p_rs->unconsumed = p_rs->buffer;
    }

    countRead = read(p_rs->fd, p_rs->read_end, p_rs->buffer_end - p_rs->read_end);

    if (countRead <= 0) {
        /* note: end-of-stream drops through here too */
        *p_outRecord = NULL;
        return countRead;
    }

    p_rs->read_end += countRead;

    ret = get_next_record (p_rs, p_outRecordLen);

    if (ret == NULL) {
        /* not enough of a buffer to for a whole command */
        errno = EAGAIN;
        return -1;
    }

    *p_outRecord = ret;
    return 0;
}
