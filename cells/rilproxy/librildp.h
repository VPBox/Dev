
#ifndef SOCKETEVENT_H
#define SOCKETEVENT_H

#include<stdbool.h>


#define MAX_FD_EVENTS 16
#define HEADER_SIZE 4
#define MAX_COMMAND_BYTES (10*1024)

struct client_socket;
typedef struct record_stream record_stream;

enum socket_type {
    RILD = 0,
    RILD2,
    RILD_OEM,
    RILD_IMS,
    RILD_MTK_MODEM,
    RILD_MTK_UT,
    RILD_MTK_UT2,
    RILD_SOCKET_TYPE_NUM,
};

enum {
    RIL_CARD1 = 0,
    RIL_CARD2,
    RIL_DONGLE,
    RIL_CARD_NUM
};

typedef void (*socket_event_cb)(int fd, short events, void *userdata);
typedef struct socket_listen_param slp;
typedef struct socket_connect_param scp;
typedef struct socket_debug_param sdp;

struct socket_event {
    struct socket_event *next;
    struct socket_event *prev;
    
    int fd;
    int index;
    bool persist;
    socket_event_cb func;
    void *param;
};

struct cache_buf {
	int count;
	int offset;
	char *buf;
};


struct socket_listen_param {
    int fd_listen;
    int fd_command;
	bool accept;
	bool cache_send;
    const char* processname;
    struct socket_event* commands_event;
    struct socket_event* listen_event;
    record_stream *p_rs;
    void (*process_commands_callback)(int fd, short flags, void *param);
    void (*listen_client_callback)(int fd, short flags, void *param);
	void (*client_disconnect)(slp *s_lp);
	struct cache_buf *cache;
	struct client_debug_info *cdi;
};

struct socket_connect_param {
    int fd_connect;
    const char* processname;
    struct socket_event* connect_event;
    record_stream *p_rs;
    void (*process_server_data_callback)(int fd, short flags, void *param);
	void (*server_disconnect)(scp *s_cp);
	struct cache_buf *cache;
	struct server_debug_info *sdi;
};

struct socket_debug_param {
	int fd_listen;
	int fd_command;
	struct socket_event* d_commands_event;
	struct socket_event* d_listen_event;
	record_stream *p_rs;
	void (*process_commands_callback)(int fd, short flags, void *param);
	void (*listen_client_callback)(int fd, short flags, void *param);
	void (*disconnect)(sdp *s_dp);
	struct server_debug_info *sdi;
	struct client_debug_info *cdi;
};


struct server_socket {
    int type;
    int fission_mode;
    int index;
	int wait; /* whether wait client connect */
    struct client_socket *c_sock;
    const char *sub_dir;
    const char **sock_name;
    scp *s_cp;
    char *buf;
};

struct client_socket {
    int fission_mode;
    int index;
    int sys_id;
    struct server_socket *s_sock;
    const char *sub_dir;
    const char **sock_name;
    slp *s_lp;
    char *buf;
};

struct record_stream {
    int fd;
    size_t maxRecordLen;

    unsigned char *buffer;

    unsigned char *unconsumed;
    unsigned char *read_end;
    unsigned char *buffer_end;
};

struct server_debug_info {
	unsigned int server_fd;
	unsigned int server_pkg;
	unsigned int server_rx;
	unsigned int server_tx;
	unsigned int server_status;
	unsigned int server_disc_count;
	pid_t pid;
	time_t last_time;
};

struct client_debug_info {
	unsigned int client_fd;
	unsigned int client_pkg;
	unsigned int client_rx;
	unsigned int client_tx;
	unsigned int client_status;
	unsigned int client_disc_count;
	time_t last_time;
};

struct debug_info {
	struct server_debug_info sdi;
	struct client_debug_info cdi[2];
};


extern void connect_server_socket
(
struct server_socket *s_sock, 
struct client_socket *c_sock, 
int fission_mode
);

extern int start_listen_client
(
struct client_socket *c_sock
);

extern int create_client_socket_by_path
(
struct client_socket *c_sock, 
struct server_socket *s_sock, 
mode_t perm,
uid_t uid, 
gid_t gid, 
int fission_mode
);

extern void socket_event_set
(struct socket_event * se, 
int fd, 
bool persist, 
socket_event_cb func, 
void * param
);

extern void socket_event_add_wakeup(struct socket_event *se);

extern void socket_event_del(struct socket_event * se);

extern void send_buf_packet(int fd, uint32_t header, char *buf, int buf_len);

extern int recv_len_data(int fd, char *buf, int length);

extern void create_pthread(void);

extern int send_buf_to_socket(int fd, char *buf, int buf_len);

extern void default_process_commands_callback
(
int fd, 
short flags, 
void *param
);

extern record_stream *new_record_stream(int fd, size_t maxRecordLen);

extern void free_record_stream(record_stream *rs);

extern int get_next_record_stream (record_stream *p_rs, void ** p_outRecord,
                                    size_t *p_outRecordLen);

extern void signal_to_connect_server(void);

extern void server_disconnect(struct server_socket *ss);

extern void store_server_data(struct server_socket *s_sock, void *p_record, size_t recordlen);

extern void send_server_data(struct server_socket *s_sock, slp *s_lp);

extern void store_client_data(struct client_socket *c_sock, void *p_record, size_t recordlen);

extern void send_client_data(struct client_socket *c_sock, int fd_connect);

extern int create_socket_for_debug(const char *path, const char *name, int index, mode_t perm, uid_t uid, gid_t gid);

extern int socket_exists(const char *path);

extern int connect_to_socket(char *path);

extern	int connect_debug_server(const char *path, const char *name, int index);

extern int start_listen_debug(sdp *s_dp);
#endif