/******************************************************************************
 @file    socketproxy.c
 @brief   The socket proxy to connect the cells and host by socket

 DESCRIPTION
 The QMUX proxy to connect the cells and host by socket, in case the cells can not see the
 SOCKET file.
 So create the file on cells and the file on host as following:
 cells: /data/cells/cell-name/host-socket-file-path
 host: host-socket-file-path

 INITIALIZATION AND SEQUENCING REQUIREMENTS
 None
 ******************************************************************************/
#define _GNU_SOURCE

#include <linux/capability.h>
#include <unistd.h>
#include <stddef.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <cutils/log.h>

#include "socketproxy.h"

#define LOG_TAG "SOCKET-PROXY"

#define FREE(x) do{ \
                  if((x) != NULL) { \
                    free(x); \
                  } \
                } while(0)

/* Local function declarations */

/*===========================================================================
 LOCAL FUNCTION DEFINITIONS
 ===========================================================================*/
static int socketpair_list_init_common(
    socket_pair *p_socket_pair);

static int socketpair_del(
    socket_pair *head_socket_pair,
    socket_pair *p_socket_pair);

static int socketpair_connect_server_is_ready(
    socket_pair *p_socket_pair);

static int socketpair_start_listener_socket(
    socket_pair *p_socket_pair);

static int
socketpair_get_listener_socket(
    const char *listen_sock_path);

static int
socketpair_get_connect_socket(
    const char *bind_sock_path,
    const char *conn_sock_path);

static int
socketpair_client_add(
    socket_pair *p_socket_pair);

static int
socketpair_client_del(
    socket_pair *p_socket_pair,
    int client_fd);

static int socketpair_disconnect(
    socket_pair *p_socket_pair,
    int connect_fd);

static int
socketpair_get_client_id(
    socket_pair *p_socket_pair,
    int client_fd);

static int
socketpair_get_connect_id(
    socket_pair *p_socket_pair,
    int client_fd);

static int
socketpair_find_client_id(
    socket_pair *p_socket_pair,
    int client_fd);

static int
socketpair_find_connect_id(
    socket_pair *p_socket_pair,
    int connect_fd);

static int
socketpair_find_connect_by_client(
    socket_pair *p_socket_pair,
    int client_fd);

static int
socketpair_find_client_by_connect(
    socket_pair *p_socket_pair,
    int connect_fd);

static int
socketpair_listener_rx_msg(
    socket_pair *p_socket_pair,
    int fd);

static int
socketpair_listener_tx_msg(
    socket_pair *p_socket_pair,
    int client_fd);

static int
socketpair_connect_rx_msg(
    socket_pair *p_socket_pair,
    int fd);

static int
socketpair_connect_tx_msg(
    socket_pair *p_socket_pair,
    int connect_fd);

static int
socketpair_handle_listener_msg(
    socket_pair *p_socket_pair,
    int client_fd);

static int
socketpair_handle_connect_msg(
    socket_pair *p_socket_pair,
    int connect_fd);

static int
socketpair_listener_update(
    socket_pair *p_socket_pair);

static int
socketpair_connect_update(
    socket_pair *p_socket_pair);

/*===========================================================================
 FUNCTION  socketpair_list_init
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
int socketpair_list_init(
    socket_pair **pp_socket_pair)
{
    int ret = 0;
    int i;
    socket_pair *head_socket_pair = NULL;
    socket_pair_common *common_info = NULL;

    if (NULL == pp_socket_pair)
    {
        ret = -1;
        goto bail;
    }

    //create head socketpair
    head_socket_pair = (socket_pair*) malloc(sizeof(socket_pair));
    if (NULL == head_socket_pair)
    {
        ALOGE("socketpair_list_init fail, malloc socket_pair is null");
        ret = -1;
        goto bail;
    }
    memset(head_socket_pair, 0x0, sizeof(socket_pair));

    //create common info
    common_info = (socket_pair_common*) malloc(sizeof(socket_pair_common));
    if (NULL == common_info)
    {
        ALOGE("socketpair_list_init fail, malloc common_info(socket_pair_common) is null");
        ret = -1;
        goto bail;
    }

    head_socket_pair->common_info = common_info;

    if (socketpair_list_init_common(head_socket_pair) < 0)
    {
        ALOGE("socketpair_list_init fail, socketpair_list_common_init failed");
        ret = -1;
        goto bail;
    }

    head_socket_pair->pre = head_socket_pair;
    head_socket_pair->next = head_socket_pair;

    *pp_socket_pair = head_socket_pair;

    ALOGI("socketpair_list_init done!!!\r\n");
    bail: if (ret < 0)
    {
        FREE(common_info);
        FREE(head_socket_pair);
    }
    return ret;
}

/*===========================================================================
 FUNCTION  socketpair_list_common_init
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_list_init_common(
    socket_pair *p_socket_pair)
{
    int ret = 0;
    socket_pair_common *common_info = NULL;

    if (NULL == p_socket_pair)
    {
        ALOGE("socketpair_list_common_init fail, p_socket_pair is null");
        ret = -1;
        goto bail;
    }

    common_info = p_socket_pair->common_info;

    if (NULL == common_info)
    {
        ALOGE("socketpair_list_common_init fail, common_info(socket_pair_common) is null");
        ret = -1;
        goto bail;
    }

    common_info->listener_max_fd = LINUX_SOCKET_INVALID_FD;
    common_info->connect_max_fd = LINUX_SOCKET_INVALID_FD;
    common_info->listener_max_fd = LINUX_SOCKET_INVALID_FD;
    common_info->connect_max_fd = LINUX_SOCKET_INVALID_FD;
    FD_ZERO(&common_info->listener_master_fd_set);
    FD_ZERO(&common_info->listener_read_fd_set);
    FD_ZERO(&common_info->connect_master_fd_set);
    FD_ZERO(&common_info->connect_read_fd_set);

    INIT_LIST_MUTEX(p_socket_pair);

    //create the pipe for connect, to trigger the conncect rx task
    if (pipe2(common_info->connect_pipe_fds, O_NONBLOCK) < 0)
    {
        ALOGE("socketpair_list_init, connect pipe() system call  returns errno[%d:%s]\n",
                errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    else
    {
        ALOGI("socketpair_list_init, connect pipe() system call, fd[0]=%d, fd[1]=%d\n", common_info->connect_pipe_fds[0],
                common_info->connect_pipe_fds[1]);

        FD_SET(common_info->connect_pipe_fds[0],
                &common_info->connect_master_fd_set);
        if (common_info->connect_max_fd < common_info->connect_pipe_fds[0])
        {
            common_info->connect_max_fd = common_info->connect_pipe_fds[0];
        }
    }

    //create the pipe for listener, to trigger the listener rx task
    if (pipe2(common_info->listener_pipe_fds, O_NONBLOCK) < 0)
    {
        ALOGE("socketpair_list_init, listener pipe() system call  returns errno[%d:%s]\n",
                errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    else
    {
        ALOGI("socketpair_list_init, listener pipe() system call, fd[0]=%d, fd[1]=%d\n", common_info->listener_pipe_fds[0],
                common_info->listener_pipe_fds[1]);

        FD_SET(common_info->listener_pipe_fds[0],
                &common_info->listener_master_fd_set);
        if (common_info->listener_max_fd < common_info->listener_pipe_fds[0])
        {
            common_info->listener_max_fd = common_info->listener_pipe_fds[0];
        }
    }

    ALOGI("socketpair_list_common_init done!!!\r\n");
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_list_add
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
int socketpair_list_add(
    socket_pair *head_socket_pair,
    const char *listener_path,
    const char *connect_path,
    listener_client_addr_convert_connect_bind_addr lst_cli_addr_convert_conn_bind_addr,
    pre_user_rx_listener_msg hook_user_rx_listener_msg,
    pre_user_rx_connect_msg hook_user_rx_connect_msg,
    pre_user_process_listener_msg hook_user_process_listener_msg,
    pre_user_process_connect_msg hook_user_process_connect_msg,
    void *user_data,
    socket_pair **pp_new_socket_pair)
{
    int ret = 0;
    int i;
    int fd;

    socket_pair *new_socket_pair = (socket_pair*) malloc(sizeof(socket_pair));
    if (NULL == new_socket_pair)
    {
        ALOGE("socketpair_list_add fail, malloc socket_pair is null");
        ret = -1;
        goto bail;
    }

    //common info
    new_socket_pair->common_info = head_socket_pair->common_info;

    new_socket_pair->lst_cli_addr_convert_conn_bind_addr =
            lst_cli_addr_convert_conn_bind_addr;
    new_socket_pair->hook_user_rx_listener_msg = hook_user_rx_listener_msg;
    new_socket_pair->hook_user_rx_connect_msg = hook_user_rx_connect_msg;
    new_socket_pair->hook_user_process_listener_msg =
            hook_user_process_listener_msg;
    new_socket_pair->hook_user_process_connect_msg =
            hook_user_process_connect_msg;
    new_socket_pair->user_data = user_data;

    if (listener_path != NULL)
    {
        strcpy(new_socket_pair->listener_path, listener_path);
    }
    if (connect_path != NULL)
    {
        strcpy(new_socket_pair->connect_path, connect_path);
    }

    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        new_socket_pair->listener_client_fds[i] = LINUX_SOCKET_INVALID_FD;
        new_socket_pair->connect_fds[i] = LINUX_SOCKET_INVALID_FD;
    }

    INIT_ITEM_MUTEX(new_socket_pair);

    /* add the point to list */
    new_socket_pair->pre = head_socket_pair->pre;
    new_socket_pair->next = head_socket_pair;
    head_socket_pair->pre->next = new_socket_pair;
    head_socket_pair->pre = new_socket_pair;

    if (pp_new_socket_pair != NULL)
    {
        *pp_new_socket_pair = new_socket_pair;
    }

    bail: if (ret < 0)
    {
        FREE(new_socket_pair);
    }
    return ret;
}

/*===========================================================================
 FUNCTION  socketpair_list_del
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
int socketpair_list_del(
    socket_pair **pp_head_socket_pair,
    socket_pair *p_socket_pair)
{
    int ret = 0;
    socket_pair *item, *head_socket_pair;
    int i;

    if (pp_head_socket_pair == NULL || (*pp_head_socket_pair) == NULL)
    {
        ALOGE("socketpair_list_del failed, head is null\n");
        ret = -1;
        goto bail;
    }

    head_socket_pair = *pp_head_socket_pair;
    if (p_socket_pair == head_socket_pair)
    {
        ALOGI("socketpair_list_del all\n");
        item = head_socket_pair;
        while (item != NULL && item->next != head_socket_pair)
        {
            item = item->next;
            socketpair_del(head_socket_pair, item);
        }
        FREE(head_socket_pair->common_info);
        FREE(head_socket_pair);
        *pp_head_socket_pair = NULL;
    }
    else
    {
        socketpair_del(head_socket_pair, p_socket_pair);
    }
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_list_del
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_del(
    socket_pair *head_socket_pair,
    socket_pair *p_socket_pair)
{
    int ret = 0;
    socket_pair *item;
    int i;

    if (p_socket_pair == head_socket_pair)
    {
        ALOGE("socketpair_list_del failed, cause of del the head socketpair\n");
        ret = -1;
        goto bail;
    }
    else
    {
        p_socket_pair->pre->next = p_socket_pair->next;
        p_socket_pair->next->pre = p_socket_pair->pre;

        for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
        {
            LOCK_ALL_MUTEX(p_socket_pair);
            if (p_socket_pair->listener_client_fds[i] != LINUX_SOCKET_INVALID_FD)
            {
                FD_CLR(p_socket_pair->listener_client_fds[i],
                        &p_socket_pair->common_info->listener_master_fd_set);
                close(p_socket_pair->listener_client_fds[i]);
            }
            if (p_socket_pair->connect_fds[i] != LINUX_SOCKET_INVALID_FD)
            {
                FD_CLR(p_socket_pair->connect_fds[i],
                        &p_socket_pair->common_info->connect_master_fd_set);
                close(p_socket_pair->connect_fds[i]);
            }
            UNLOCK_ALL_MUTEX(p_socket_pair);
        }
        FREE(p_socket_pair);

        ALOGI("socketpair_del success\n");
    }
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_connect_server_is_ready
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_connect_server_is_ready(
    socket_pair *p_socket_pair)
{
    int rc, ret = 0;
    struct stat stats;

    if (strlen(p_socket_pair->connect_path) == 0)
    {
        ALOGE("socketpair_connect_server_is_ready path = null, no need to wait\n");
        ret = 1;
        goto bail;
    }
    //just in case the file is exsit
    //unlink(p_socket_pair->connect_path);
    //get the client addr
    if ((rc = stat(p_socket_pair->connect_path, &stats)) < 0)
    {
        ALOGE("socketpair_connect_server_is_ready, unable to stat client socket file \"%s\", rc = %d\n", p_socket_pair->connect_path, rc);
        goto bail;
    }
    if (S_ISSOCK (stats.st_mode) == 0)
    {
        ALOGE("socketpair_connect_server_is_ready, client socket file not a socket file, rc = %d\n", rc);
        goto bail;
    }
    ret = 1;
    ALOGI("socketpair_connect_server_is_ready is ready\n");
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_start_listener_socket
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_start_listener_socket(
    socket_pair *p_socket_pair)
{
    int fd;
    int ret = 0;
    if (strlen(p_socket_pair->listener_path) == 0)
    {
        ALOGE("socketpair_start_listener_socket, path is NULL, not need to listener\n");
        goto bail;
    }
    fd = socketpair_get_listener_socket(p_socket_pair->listener_path);
    if (fd < 0)
    {
        ret = -1;
        ALOGE("socketpair_start_listener_socket failed, socketpair_get_listener_socket failed\n");
        goto bail;
    }
    LOCK_ALL_MUTEX(p_socket_pair);
    p_socket_pair->listener_fd = fd;
    FD_SET(fd, &p_socket_pair->common_info->listener_master_fd_set);
    if (p_socket_pair->common_info->listener_max_fd < fd)
    {
        p_socket_pair->common_info->listener_max_fd = fd;
    }
    UNLOCK_ALL_MUTEX(p_socket_pair);
    ALOGI("socketpair_start_listener_socket done\n");
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_get_listener_socket
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_get_listener_socket(
    const char *listen_sock_path)
{
    struct sockaddr_un addr;
    int listen_fd = LINUX_SOCKET_INVALID_FD;
    int fd = LINUX_SOCKET_INVALID_FD;
    int rc;
    size_t len, path_len;

    if (NULL == listen_sock_path)
    {
        ALOGE("bad param to socketpair_get_listener_socket");
        goto bail;
    }

    /* Get the connection listener socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        ALOGE("unable to open listener socket, rc=%d, errno=[%d:%s]\n", fd, errno, strerror(errno));
        goto bail;
    }

    /* Unlink socket path name just in case.... */
    unlink(listen_sock_path);

    /* setup for bind */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    path_len = strlen(listen_sock_path);
    path_len = MIN(path_len, (sizeof(addr.sun_path) - 1));
    addr.sun_family = AF_UNIX;
    memcpy(&addr.sun_path[0], listen_sock_path, path_len);
    addr.sun_path[path_len] = '\0';

    len = offsetof (struct sockaddr_un, sun_path) + path_len;

    ALOGI("addr path=%s, len=%d\n", addr.sun_path, (int) len);

    /* Bind socket to address */
    if ((rc = bind(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un)))
            < 0)
    {
        ALOGE("unable to bind to listener socket, rc=%d, errno=[%d:%s]\n", rc, errno, strerror(errno));
        goto bail;
    }

    /* Allow RW permissions only for user and group */
    if (-1 == chmod(listen_sock_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))
    {
        ALOGE("unable to chmod listen_sock_path=%s, errno=[%d:%s]\n", listen_sock_path,
        errno, strerror(errno));
    }

    /* Make socket a listener */
    if ((rc = listen(fd, 5)) < 0)
    {
        ALOGE("unable to listen with listener socket, rc=%d, errno=[%d:%s]\n", rc,
                errno, strerror(errno));
        goto bail;
    }

    listen_fd = fd;

    bail: if (LINUX_SOCKET_INVALID_FD == listen_fd)
    {
        if (LINUX_SOCKET_INVALID_FD != fd)
        {
            close(fd);
        }
    }

    return listen_fd;
}

/*===========================================================================
 FUNCTION  socketpair_get_connect_socket
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_get_connect_socket(
    const char *bind_sock_path,
    const char *conn_sock_path)
{
    struct sockaddr_un server_addr, client_addr;
    int client_fd = LINUX_SOCKET_INVALID_FD;
    int ret_fd = LINUX_SOCKET_INVALID_FD;
    int rc, i;
    socklen_t len;

    if (NULL == conn_sock_path)
    {
        ALOGE("bad param to socketpair_get_connect_socket");
        goto bail;
    }

    /* Initialize the addr variables */
    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    memset(&server_addr, 0, sizeof(struct sockaddr_un));

    /* Get the connection listener socket */
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        ALOGE("unable to open client socket, rc = %d\n", client_fd);
        goto bail;
    }

    //bind the socket to path
    if (bind_sock_path != NULL && strlen(bind_sock_path) != 0)
    {
        client_addr.sun_family = AF_UNIX;
        snprintf(client_addr.sun_path, sizeof(client_addr.sun_path), "%s",
                bind_sock_path);

        len = (socklen_t) (offsetof(struct sockaddr_un, sun_path)
                + strlen(client_addr.sun_path));

        /* Delete file in case it exists */
        unlink(client_addr.sun_path);

        /* Bind socket to address */
        if ((rc = bind(client_fd, (struct sockaddr*) &client_addr,
                sizeof(struct sockaddr_un))) < 0)
        {
            ALOGE("socketpair_get_connect_socket, unable to bind to client socket, rc = %d errno[%d:%s]\n", rc,
                    errno, strerror(errno));
            goto bail;
        }

        /* Allow RW permissions only for user and group */
        if (-1 == chmod(client_addr.sun_path,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))
        {
            ALOGE("socketpair_get_connect_socket, unable to chmod bind_sock_path=%s, errno=[%d:%s]\n", client_addr.sun_path,
                    errno, strerror(errno));
            goto bail;
        }

        ALOGI("<socketpair_get_connect_socket> bind socket paths =%s\n",
                bind_sock_path);
    }

    ALOGI("<socketpair_get_connect_socket> using socket paths connect=%s\n",
            conn_sock_path);

    server_addr.sun_family = AF_UNIX;

    snprintf(server_addr.sun_path, sizeof(server_addr.sun_path), "%s",
            conn_sock_path);

    len = (socklen_t) (offsetof(struct sockaddr_un, sun_path)
            + strlen(server_addr.sun_path));

    /* Connect to the server's connection socket */
    for (i = 0; i < LINUX_SOCKET_PAIR_CONNECT_TRIES; i++)
    {
        if ((rc = connect(client_fd, (struct sockaddr*) &server_addr, len)) < 0)
        {
            ALOGE("unable to connect to server, errno=[%d:%s], attempt=%d\n",
                    errno, strerror(errno), i + 1);
            sleep(1);
        }
        else
        {
            ALOGI("successfully connected to server, attempt=%d\n", i + 1);
            break;
        }
    }

    if (rc < 0)
    {
        ALOGE("unable to connect to server after %d tries... giving up\n", i);
        goto bail;
    }

    ret_fd = client_fd;

    ALOGI("<socketpair_get_connect_socket> connect done connect fd=%d\n",
            ret_fd);

    bail: if (LINUX_SOCKET_INVALID_FD == ret_fd
            && LINUX_SOCKET_INVALID_FD != client_fd)
    {
        /* Delete file in case it exists */
        close(client_fd);
        unlink(client_addr.sun_path);
    }

    return ret_fd;
}

/*===========================================================================
 FUNCTION  socketpair_client_add
 ===========================================================================*/
/*!
 @brief


 @return
 None

 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_client_add(
    socket_pair *p_socket_pair)
{
    int ret = 0;
    int listener_fd;
    int client_fd, rc;
    socklen_t len;
    struct stat stats;
    struct sockaddr_un addr;
    int need_bind_file = 1;
    int i;
    char convert_client_addr[LINUX_SOCKET_PAIR_PATH_LEN] = "";
    char connect_bind_addr[LINUX_SOCKET_PAIR_PATH_LEN] = "";

    memset(&addr, 0, sizeof(struct sockaddr_un));
    len = sizeof(struct sockaddr_un);

    listener_fd = p_socket_pair->listener_fd;

    if (LINUX_SOCKET_INVALID_FD == listener_fd)
    {
        ALOGE("socketpair_client_add, unable to accept on invalid listener socket\n");
        ret = -1;
        goto bail;
    }

    if ((client_fd = accept(listener_fd, (struct sockaddr*) &addr, &len)) < 0)
    {
        ALOGE("socketpair_client_add, unable to accept on listener socket, rc=%d, errno=[%d:%s]\n", client_fd,
                errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    if (p_socket_pair->lst_cli_addr_convert_conn_bind_addr != NULL)
    {
        p_socket_pair->lst_cli_addr_convert_conn_bind_addr(p_socket_pair,
                addr.sun_path, convert_client_addr, connect_bind_addr);
    }
    //get the client addr
    if ((rc = stat(convert_client_addr, &stats)) < 0)
    {
        ALOGE("socketpair_client_add, unable to stat client socket file \"%s\", rc = %d\n", convert_client_addr, rc);
        need_bind_file = 0;
    }
    if (S_ISSOCK (stats.st_mode) == 0)
    {
        ALOGE("socketpair_client_add, client socket file not a socket file, rc = %d\n", rc);
        need_bind_file = 0;
    }
    else
    {
        // No longer need the temp file
        unlink(convert_client_addr);
        ALOGI("socketpair_client_add, client socket file path = %s\n",
                convert_client_addr);
    }

    i = socketpair_get_client_id(p_socket_pair, client_fd);
    if (i == LINUX_SOCKET_INVALID_ID)
    {
        ALOGE("client FD out of range = %d\n", client_fd);
        close(client_fd);
        ret = -1;
        goto bail;
    }

    ALOGI("<socketpair_client_add> client(%d) id(%d) connected\n", client_fd,
            i);

    LOCK_LIST_MUTEX(p_socket_pair);
    /* Add the new fd to the master fd set */
    FD_SET(client_fd, &p_socket_pair->common_info->listener_master_fd_set);
    if (p_socket_pair->common_info->listener_max_fd < client_fd)
    {
        p_socket_pair->common_info->listener_max_fd = client_fd;
    }
    UNLOCK_LIST_MUTEX(p_socket_pair);

    /* connect to server */
    if (strlen(p_socket_pair->connect_path) != 0)
    {
        ALOGI("<socketpair_client_add> socketpair_get_connect_socket path = %s\n",
                p_socket_pair->connect_path);
        //if bind the path
        if (need_bind_file)
        {
            p_socket_pair->connect_fds[i] = socketpair_get_connect_socket(
                    connect_bind_addr, p_socket_pair->connect_path);
        }
        else
        {
            p_socket_pair->connect_fds[i] = socketpair_get_connect_socket(
            NULL, p_socket_pair->connect_path);
        }

        //if connect failed, then close client socket
        if (p_socket_pair->connect_fds[i] == LINUX_SOCKET_INVALID_FD)
        {
            ret = -1;
            close(p_socket_pair->listener_client_fds[i]);
            goto bail;
        }
        else
        {
            LOCK_LIST_MUTEX(p_socket_pair);
            /* Add the new fd to the master fd set */
            FD_SET(p_socket_pair->connect_fds[i],
                    &p_socket_pair->common_info->connect_master_fd_set);
            if (p_socket_pair->common_info->connect_max_fd
                    < p_socket_pair->connect_fds[i])
            {
                p_socket_pair->common_info->connect_max_fd =
                        p_socket_pair->connect_fds[i];
            }
            // trigger the connect task to update the fds.
            if (write(p_socket_pair->common_info->connect_pipe_fds[1], "x", 1)
                    < 0)
            {
                ALOGE("write connect_pipe_fds fails, returns errno[%d:%s]\n", errno, strerror(errno));
                ret = -1;
                goto bail;
            }
            UNLOCK_LIST_MUTEX(p_socket_pair);
        }
    }
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_client_del
 ===========================================================================*/
/*!
 @brief


 @return
 None

 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_client_del(
    socket_pair *p_socket_pair,
    int client_fd)
{
    int i;
    int ret = 0;

    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        if (p_socket_pair->listener_client_fds[i] == client_fd)
        {
            p_socket_pair->listener_client_fds[i] = LINUX_SOCKET_INVALID_FD;
            break;
        }
    }

    if (i >= LINUX_SOCKET_PAIR_MAX_CLIENTS)
    {
        ALOGE("client FD out of range = %d\n", client_fd);
        close(client_fd);
        ret = -1;
        goto bail;
    }

    LOCK_ALL_MUTEX(p_socket_pair);
    /* clear the fd from the master fd set */
    FD_CLR(client_fd, &p_socket_pair->common_info->listener_master_fd_set);
    close(client_fd);

    ALOGI("del the client fd=%d\n", client_fd);

    /* disconnect to server */
    close(p_socket_pair->connect_fds[i]);
    /* clear the fd from the master fd set */
    FD_CLR(p_socket_pair->connect_fds[i],
            &p_socket_pair->common_info->connect_master_fd_set);
    p_socket_pair->connect_fds[i] = LINUX_SOCKET_INVALID_FD;

    // trigger the listener task to update the fds.
    if (write(p_socket_pair->common_info->listener_pipe_fds[1], "x", 1) < 0)
    {
        ALOGE("write listener_pipe_fds fails, returns errno[%d:%s]\n", errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    // trigger the connect task to update the fds.
    if (write(p_socket_pair->common_info->connect_pipe_fds[1], "x", 1) < 0)
    {
        ALOGE("write connect_pipe_fds fails, returns errno[%d:%s]\n", errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    UNLOCK_ALL_MUTEX(p_socket_pair);
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_disconnect
 ===========================================================================*/
/*!
 @brief


 @return
 None

 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_disconnect(
    socket_pair *p_socket_pair,
    int connect_fd)
{
    int i;
    int ret = 0;

    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        if (p_socket_pair->connect_fds[i] == connect_fd)
        {
            p_socket_pair->connect_fds[i] = LINUX_SOCKET_INVALID_FD;
            break;
        }
    }

    if (i >= LINUX_SOCKET_PAIR_MAX_CLIENTS)
    {
        ALOGE("connect FD out of range = %d\n", connect_fd);
        close(connect_fd);
        ret = -1;
        goto bail;
    }

    LOCK_ALL_MUTEX(p_socket_pair);
    /* clear the fd from the master fd set */
    FD_CLR(connect_fd, &p_socket_pair->common_info->connect_master_fd_set);
    close(connect_fd);

    ALOGI("socketpair_disconnect, connect fd=%d\n", connect_fd);

    /* clear the fd from the master fd set */
    FD_CLR(p_socket_pair->listener_client_fds[i],
            &p_socket_pair->common_info->listener_master_fd_set);
    /* disconnect to server */
    close(p_socket_pair->listener_client_fds[i]);
    p_socket_pair->listener_client_fds[i] = LINUX_SOCKET_INVALID_FD;

    // trigger the listener task to update the fds.
    if (write(p_socket_pair->common_info->listener_pipe_fds[1], "x", 1) < 0)
    {
        ALOGE("write connect_pipe_fds fails, returns errno[%d:%s]\n", errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    // trigger the connect task to update the fds.
    if (write(p_socket_pair->common_info->connect_pipe_fds[1], "x", 1) < 0)
    {
        ALOGE("write connect_pipe_fds fails, returns errno[%d:%s]\n", errno, strerror(errno));
        ret = -1;
        goto bail;
    }
    UNLOCK_ALL_MUTEX(p_socket_pair);
    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_get_client_id
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_get_client_id(
    socket_pair *p_socket_pair,
    int client_fd)
{
    int i;
    LOCK_ITEM_MUTEX(p_socket_pair);
    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        if (p_socket_pair->listener_client_fds[i] == LINUX_SOCKET_INVALID_ID)
        {
            p_socket_pair->listener_client_fds[i] = client_fd;
            break;
        }
    }
    UNLOCK_ITEM_MUTEX(p_socket_pair);

    if (i == LINUX_SOCKET_PAIR_MAX_CLIENTS)
    {
        return LINUX_SOCKET_INVALID_ID;
    }

    return i;
}

/*===========================================================================
 FUNCTION  socketpair_get_connect_id
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_get_connect_id(
    socket_pair *p_socket_pair,
    int connect_fd)
{
    int i;
    LOCK_ITEM_MUTEX(p_socket_pair);
    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        if (p_socket_pair->connect_fds[i] == LINUX_SOCKET_INVALID_ID)
        {
            p_socket_pair->connect_fds[i] = connect_fd;
            break;
        }
    }
    UNLOCK_ITEM_MUTEX(p_socket_pair);

    if (i == LINUX_SOCKET_PAIR_MAX_CLIENTS)
    {
        return LINUX_SOCKET_INVALID_ID;
    }

    return i;
}

/*===========================================================================
 FUNCTION  socketpair_find_client_id
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_find_client_id(
    socket_pair *p_socket_pair,
    int client_fd)
{
    int i;
    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        if (p_socket_pair->listener_client_fds[i] == client_fd)
        {
            return i;
        }
    }

    return LINUX_SOCKET_INVALID_ID;
}

/*===========================================================================
 FUNCTION  socketpair_find_connect_id
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_find_connect_id(
    socket_pair *p_socket_pair,
    int connect_fd)
{
    int i;
    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        if (p_socket_pair->connect_fds[i] == connect_fd)
        {
            return i;
        }
    }

    return LINUX_SOCKET_INVALID_ID;
}

/*===========================================================================
 FUNCTION  socketpair_find_connect_by_client
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_find_connect_by_client(
    socket_pair *p_socket_pair,
    int client_fd)
{
    int i;
    i = socketpair_find_client_id(p_socket_pair, client_fd);

    if (i == LINUX_SOCKET_INVALID_ID)
    {
        return LINUX_SOCKET_INVALID_FD;
    }
    return p_socket_pair->connect_fds[i];
}

/*===========================================================================
 FUNCTION  socketpair_find_client_by_connect
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_find_client_by_connect(
    socket_pair *p_socket_pair,
    int connect_fd)
{
    int i;
    i = socketpair_find_connect_id(p_socket_pair, connect_fd);

    if (i == LINUX_SOCKET_INVALID_ID)
    {
        return LINUX_SOCKET_INVALID_FD;
    }
    return p_socket_pair->listener_client_fds[i];
}

/*===========================================================================
 FUNCTION  socketpair_listener_rx_msg
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
//TODO: rev
static int socketpair_listener_rx_msg(
    socket_pair *p_socket_pair,
    int fd)
{
    int buf_size;
    int rc = 1;

    if (p_socket_pair->common_info->listener_rx_buf_remaining <= 0)
    {
        return rc;
    }

    if ((buf_size =
            recv(fd,
                    (void *) &p_socket_pair->common_info->listener_rx_buf[p_socket_pair->common_info->listener_rx_buf_index],
                    p_socket_pair->common_info->listener_rx_buf_remaining, 0))
            <= 0)
    {
        ALOGE("listener RX on fd=%d returned error=%d errno[%d:%s]\n", fd, (int) buf_size, errno, strerror(errno));
        if (buf_size == 0) /* Client has gone away */
        {
            rc = 0;
        }
    }
    else
    {
        p_socket_pair->common_info->listener_rx_buf_index += buf_size;
        p_socket_pair->common_info->listener_rx_buf_remaining = 0;
    }
    return rc;
}

/*===========================================================================
 FUNCTION  socketpair_listener_tx_msg
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_listener_tx_msg(
    socket_pair *p_socket_pair,
    int client_fd)
{
    int ret = 0;
    int rc;

    if ((rc = send(client_fd,
            (void *) p_socket_pair->common_info->connect_rx_buf,
            (size_t) p_socket_pair->common_info->connect_rx_buf_index,
            MSG_DONTWAIT | MSG_NOSIGNAL)) < 0)
    {
        ALOGE("socketpair_listener_tx_msg:  send error = %d\n", rc);
        goto bail;
    }

    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_connect_rx_msg
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
//TODO:
static int socketpair_connect_rx_msg(
    socket_pair *p_socket_pair,
    int fd)
{
    int buf_size;
    int rc = 1;

    if ((buf_size =
            recv(fd,
                    (void *) &p_socket_pair->common_info->connect_rx_buf[p_socket_pair->common_info->connect_rx_buf_index],
                    p_socket_pair->common_info->connect_rx_buf_remaining, 0))
            <= 0)

    {
        ALOGE("connect RX on fd=%d returned error=%d errno[%d:%s]\n", fd, (int) buf_size, errno, strerror(errno));
        if (buf_size == 0) /* Client has gone away */
        {
            rc = 0;
        }
    }
    else
    {
        p_socket_pair->common_info->connect_rx_buf_index += buf_size;
        p_socket_pair->common_info->connect_rx_buf_remaining = 0;
    }
    return rc;
}

/*===========================================================================
 FUNCTION  socketpair_connect_tx_msg
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_connect_tx_msg(
    socket_pair *p_socket_pair,
    int connect_fd)
{
    int ret = 0;
    int rc;

    if ((rc = send(connect_fd,
            (void *) p_socket_pair->common_info->listener_rx_buf,
            (size_t) p_socket_pair->common_info->listener_rx_buf_index,
            MSG_DONTWAIT | MSG_NOSIGNAL)) < 0)
    {
        ALOGE("socketpair_connect_tx_msg:  send error = %d\n", rc);
        goto bail;
    }

    bail: return ret;
}

/*===========================================================================
 FUNCTION  socketpair_process_client_msg
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_handle_listener_msg(
    socket_pair *p_socket_pair,
    int client_fd)
{
    int rc;
    int ret = 0;
    int i;
    int connect_fd;

    p_socket_pair->common_info->listener_rx_buf_index = 0;
    p_socket_pair->common_info->listener_rx_buf_remaining =
            LINUX_SOCKET_MAX_BUF_SIZE;

    if (p_socket_pair->hook_user_rx_listener_msg != NULL)
    {
        rc = p_socket_pair->hook_user_rx_listener_msg(p_socket_pair, client_fd);
    }

    rc = socketpair_listener_rx_msg(p_socket_pair, client_fd);

    if (!rc)
    {
        ALOGE("socketpair_handle_listener_msg, client del, client fd=%d\n", client_fd);
        socketpair_client_del(p_socket_pair, client_fd);
    }
    else
    {
        //ALOGD("<socketpair_handle_listener_msg>: process client msg, user handle = %d\n", (int)p_socket_pair->hook_user_process_listener_msg);
        if (p_socket_pair->hook_user_process_listener_msg != NULL)
        {
            rc = p_socket_pair->hook_user_process_listener_msg(p_socket_pair,
                    client_fd);
        }

        connect_fd = socketpair_find_connect_by_client(p_socket_pair,
                client_fd);
        if (connect_fd != LINUX_SOCKET_INVALID_FD)
        {
            socketpair_connect_tx_msg(p_socket_pair, connect_fd);
        }
    }
    return rc;
}

/*===========================================================================
 FUNCTION  socketpair_process_connect_msg
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_handle_connect_msg(
    socket_pair *p_socket_pair,
    int connect_fd)
{
    int rc = 0;
    int i;
    int client_fd;

    p_socket_pair->common_info->connect_rx_buf_index = 0;
    p_socket_pair->common_info->connect_rx_buf_remaining =
            LINUX_SOCKET_MAX_BUF_SIZE;

    if (p_socket_pair->hook_user_rx_connect_msg != NULL)
    {
        p_socket_pair->hook_user_rx_connect_msg(p_socket_pair, connect_fd);
    }
    rc = socketpair_connect_rx_msg(p_socket_pair, connect_fd);

    if (!rc)
    {
        ALOGE("socketpair_handle_connect_msg, disconncect, connect fd=%d\n", connect_fd);
        socketpair_disconnect(p_socket_pair, connect_fd);
    }
    else
    {
        if (p_socket_pair->hook_user_process_connect_msg != NULL)
        {
            p_socket_pair->hook_user_process_connect_msg(p_socket_pair,
                    connect_fd);
        }

        client_fd = socketpair_find_client_by_connect(p_socket_pair,
                connect_fd);
        if (client_fd != LINUX_SOCKET_INVALID_FD)
        {
            socketpair_listener_tx_msg(p_socket_pair, client_fd);
        }
    }

    return rc;
}

/*===========================================================================
 FUNCTION  socketpair_listener_update
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_listener_update(
    socket_pair *p_socket_pair)
{
    int ret = 0;
    int i;
    int fd;
    int nread;
    char tmp[1];

    if (NULL == p_socket_pair)
    {
        ALOGE("socketpair_listener_update fail, p_socket_pair is null\n");
        ret = -1;
    }

    // if is the pipes, ignore
    if (FD_ISSET(p_socket_pair->common_info->listener_pipe_fds[0],
            &p_socket_pair->common_info->listener_read_fd_set))
    {
        //ALOGI("socketpair_listener_update, listener pipe not empty\n");
        nread = read(p_socket_pair->common_info->listener_pipe_fds[0], &tmp, 1);

        while (nread > 1)
        {
            //ALOGI("socketpair_listener_update cleanup pipes\n");
            nread = read(p_socket_pair->common_info->listener_pipe_fds[0], &tmp,
                    1);
        }
        //FD_CLR(p_socket_pair->listener_pipe_fds[0], &p_socket_pair->common_info->listener_read_fd_set);
    }

    /* Loop through all client FD's and process any with messages */
    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        fd = p_socket_pair->listener_client_fds[i];
        if ((fd != LINUX_SOCKET_INVALID_FD)
                && (FD_ISSET(fd,
                        &p_socket_pair->common_info->listener_read_fd_set)))
        {
            //ALOGV("<socketpair_listener_update> processing socket_pair's client(%d, %d) msg\n", i, fd);
            socketpair_handle_listener_msg(p_socket_pair, fd);
            FD_CLR(fd, &p_socket_pair->common_info->listener_read_fd_set);
        } /* if */
    } /* for */

    /* Process any requests on the listener fds */
    if (FD_ISSET(p_socket_pair->listener_fd,
            &p_socket_pair->common_info->listener_read_fd_set))
    {
        ALOGI("<socketpair_listener_update> socket_pair adding client\n");
        socketpair_client_add(p_socket_pair);
    }

    return ret;
}

/*===========================================================================
 FUNCTION  socketpair_connect_update
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
static int socketpair_connect_update(
    socket_pair *p_socket_pair)
{
    int ret = 0;
    int i;
    int fd;

    char tmp[1];
    int nread;

    if (NULL == p_socket_pair)
    {
        ALOGE("socketpair_connect_update fail, p_socket_pair is null\n");
        ret = -1;
    }

    // if is the pipes, ignore
    if (FD_ISSET(p_socket_pair->common_info->connect_pipe_fds[0],
            &p_socket_pair->common_info->connect_read_fd_set))
    {
        //ALOGV("socketpair_connect_update, connect pipe not empty\n");
        nread = read(p_socket_pair->common_info->connect_pipe_fds[0], &tmp, 1);

        while (nread > 1)
        {
            //ALOGV("socketpair_connect_update cleanup pipes\n");
            nread = read(p_socket_pair->common_info->connect_pipe_fds[0], &tmp,
                    1);
        }
        //FD_CLR(p_socket_pair->connect_pipe_fds[0], &p_socket_pair->common_info->connect_read_fd_set);
    }

    /* Loop through all client FD's and process any with messages */
    for (i = 0; i < LINUX_SOCKET_PAIR_MAX_CLIENTS; i++)
    {
        fd = p_socket_pair->connect_fds[i];
        if ((fd != LINUX_SOCKET_INVALID_FD)
                && (FD_ISSET(fd,
                        &p_socket_pair->common_info->connect_read_fd_set)))
        {
            socketpair_handle_connect_msg(p_socket_pair, fd);
            //FD_CLR (fd, &p_socket_pair->common_info->connect_read_fd_set);
        } /* if */
    } /* for */

    return ret;
}

/*===========================================================================
 FUNCTION  socketpair_list_listener_new_task
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
void*
socketpair_list_listener_new_task(
    void *param)
{
    int num_fds_ready;
    int fd;
    int i;
    socket_pair *head, *item;
    int service_ready = 1;

    head = (socket_pair *) param;

    ALOGI("<socketpair_list_listener_new_task> start...\n");

    /*
     //wait for connect server is ready
     for (i = 0; i < LINUX_SOCKET_PAIR_CONNECT_TRIES; i++)
     {
     item = head;
     while (item != NULL && item->next != head)
     {
     item = item->next;
     if(!socketpair_connect_server_is_ready(item))
     {
     service_ready = 0;
     break;
     }
     }
     if (service_ready)
     {
     break;
     }
     sleep(1);
     }
     */
    //start all listener
    item = head;
    while (item != NULL && item->next != head)
    {
        item = item->next;
        socketpair_start_listener_socket(item);
    }

    for (;;)
    {
        head->common_info->listener_read_fd_set =
                head->common_info->listener_master_fd_set;

        num_fds_ready = select(head->common_info->listener_max_fd + 1,
                &head->common_info->listener_read_fd_set, NULL, NULL, NULL);
        if (num_fds_ready < 0)
        {
            ALOGE("listener select returns err %d, continuing\n", num_fds_ready);
            continue;
        }

        //ALOGV("<socketpair_list_listener_new_task> selected\n");

        item = head;

        while (item != NULL && item->next != head)
        {
            item = item->next;

            socketpair_listener_update(item);
        }

        //ALOGV("<socketpair_list_listener_new_task> selected done!!!!\n");
    }

    return 0;
}

/*===========================================================================
 FUNCTION  socketpair_list_connect_new_task
 ===========================================================================*/
/*!
 @brief


 @return


 @note
 - Side Effects
 */
/*=========================================================================*/
void*
socketpair_list_connect_new_task(
    void *param)
{
    int num_fds_ready;
    int fd;
    socket_pair *head, *item;

    head = (socket_pair *) param;

    ALOGI("<socketpair_list_connect_new_task> start...\n");

    for (;;)
    {
        head->common_info->connect_read_fd_set =
                head->common_info->connect_master_fd_set;

        num_fds_ready = select(head->common_info->connect_max_fd + 1,
                &head->common_info->connect_read_fd_set, NULL, NULL, NULL);
        if (num_fds_ready < 0)
        {
            ALOGE("connect select returns err %d, continuing\n", num_fds_ready);
            continue;
        }

        //ALOGV("socketpair_list_connect_new_task select\n", );

        item = head;
        while (item != NULL && item->next != head)
        {
            item = item->next;

            socketpair_connect_update(item);
        }

        //ALOGV("socketpair_list_connect_new_task select done!!!!\n");
    }

    return 0;
}

int trigger_listener(
    socket_pair *p_socket_pair)
{
    socketpair_start_listener_socket(p_socket_pair);
    // trigger the listener task to update the fds.
    if (write(p_socket_pair->common_info->listener_pipe_fds[1], "x", 1) < 0)
    {
        ALOGE("trigger_listener fails, returns errno[%d:%s]\n", errno, strerror(errno));
        return -1;
    }
    return 0;
}

int trigger_connect(
    socket_pair *p_socket_pair)
{
    int i = 0;
    int fd = LINUX_SOCKET_INVALID_FD;
    char buf[1] =
    { '1' };

    /* connect to server */
    if (strlen(p_socket_pair->connect_path) != 0)
    {
        ALOGI("<trigger_connect> socketpair_get_connect_socket path = %s\n", p_socket_pair->connect_path);
        fd = socketpair_get_connect_socket(NULL, p_socket_pair->connect_path);
        socketpair_get_connect_id(p_socket_pair, fd);
        /* Add the new fd to the master fd set */
        FD_SET(fd, &p_socket_pair->common_info->connect_master_fd_set);
        if (p_socket_pair->common_info->connect_max_fd < fd)
        {
            p_socket_pair->common_info->connect_max_fd = fd;
        }

        //trigger the connect task to update the fds.
        if (write(p_socket_pair->common_info->connect_pipe_fds[1], (void *) buf,
                sizeof(buf)) < 0)
        {
            ALOGE("write connect_pipe_fds fails, returns errno[%d:%s]\n", errno, strerror(errno));
        }
    }
    return fd;
}

