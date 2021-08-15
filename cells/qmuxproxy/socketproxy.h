#ifndef __SOCKET_PROXY_H__
#define __SOCKET_PROXY_H__

#define LINUX_SOCKET_INVALID_FD (-1)
#define LINUX_SOCKET_INVALID_ID (-1)

#define LINUX_SOCKET_PAIR_MAX_CLIENTS (100)
#define LINUX_SOCKET_PAIR_PATH_LEN (256)

#define LINUX_SOCKET_MAX_BUF_SIZE (5*1024)

#define LINUX_SOCKET_PAIR_CONNECT_TRIES (60)

#ifndef MAX
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#endif

#ifndef MIN
#define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif

//mutex for the whole socketpair list
#define INIT_LIST_MUTEX(x) pthread_mutex_init(&x->common_info->list_lock, NULL)
#define LOCK_LIST_MUTEX(x) pthread_mutex_lock(&x->common_info->list_lock)
#define UNLOCK_LIST_MUTEX(x) pthread_mutex_unlock(&x->common_info->list_lock)

//mutex for the single socketpair
#define INIT_ITEM_MUTEX(x) pthread_mutex_init(&x->item_lock, NULL)
#define LOCK_ITEM_MUTEX(x) pthread_mutex_lock(&x->item_lock)
#define UNLOCK_ITEM_MUTEX(x) pthread_mutex_unlock(&x->item_lock)

//mutex for the single socketpair and the whole socketpair list
#define LOCK_ALL_MUTEX(x) pthread_mutex_lock(&x->common_info->list_lock);  \
                     pthread_mutex_lock(&x->item_lock)
#define UNLOCK_ALL_MUTEX(x) pthread_mutex_unlock(&x->item_lock); \
                     pthread_mutex_unlock(&x->common_info->list_lock)

typedef int (*listener_client_addr_convert_connect_bind_addr)(
    void *param,
    const char *client_addr,
    char *convert_client_addr,
    char *convert_bind_addr);

typedef int (*pre_user_rx_listener_msg)(
    void *param,
    int listener_fd);

typedef int (*pre_user_rx_connect_msg)(
    void *param,
    int connect_fd);

typedef int (*pre_user_process_listener_msg)(
    void *param,
    int listener_fd);
typedef int (*pre_user_process_connect_msg)(
    void *param,
    int connect_fd);

typedef struct _socket_pair_common_
{
    fd_set listener_master_fd_set;
    fd_set listener_read_fd_set;
    fd_set connect_master_fd_set;
    fd_set connect_read_fd_set;
    int listener_pipe_fds[2]; //trigger the listerner rx task to update the connnect fd and max fd
    int connect_pipe_fds[2]; //trigger the connnect rx task to update the connnect fd and max fd

    pthread_mutex_t list_lock;

    int listener_max_fd;
    int connect_max_fd;

    // rx buf
    unsigned char listener_rx_buf[LINUX_SOCKET_MAX_BUF_SIZE];
    int listener_rx_buf_index;
    int listener_rx_buf_remaining;
    unsigned char connect_rx_buf[LINUX_SOCKET_MAX_BUF_SIZE];
    int connect_rx_buf_index;
    int connect_rx_buf_remaining;
} socket_pair_common;

typedef struct _socket_pair_
{
    socket_pair_common *common_info;

    char listener_path[LINUX_SOCKET_PAIR_PATH_LEN];
    char connect_path[LINUX_SOCKET_PAIR_PATH_LEN];

    int listener_fd;
    int listener_client_fds[LINUX_SOCKET_PAIR_MAX_CLIENTS];
    int connect_fds[LINUX_SOCKET_PAIR_MAX_CLIENTS];

    pthread_mutex_t item_lock;

    listener_client_addr_convert_connect_bind_addr lst_cli_addr_convert_conn_bind_addr;

    pre_user_rx_listener_msg hook_user_rx_listener_msg;
    pre_user_rx_connect_msg hook_user_rx_connect_msg;

    pre_user_process_listener_msg hook_user_process_listener_msg;
    pre_user_process_connect_msg hook_user_process_connect_msg;

    void *user_data;

    struct _socket_pair_ *pre;
    struct _socket_pair_ *next;
} socket_pair;

int socketpair_list_init(
    socket_pair **pp_socket_pair);

int
socketpair_list_add(
    socket_pair *head_socket_pair,
    const char *listener_path,
    const char *connect_path,
    listener_client_addr_convert_connect_bind_addr lst_cli_addr_convert_conn_bind_addr,
    pre_user_rx_listener_msg hook_user_rx_listener_msg,
    pre_user_rx_connect_msg hook_user_rx_connect_msg,
    pre_user_process_listener_msg hook_user_process_listener_msg,
    pre_user_process_connect_msg hook_user_process_connect_msg,
    void *user_data,
    socket_pair **pp_new_socket_pair);

int socketpair_list_del(
    socket_pair **pp_head_socket_pair,
    socket_pair *p_socket_pair);

void* socketpair_list_listener_new_task(
    void *param);

void* socketpair_list_connect_new_task(
    void *param);

int trigger_listener(
    socket_pair *p_socket_pair);

int trigger_connect(
    socket_pair *p_socket_pair);

#endif
