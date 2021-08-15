/******************************************************************************
 @file    qmuxproxy.c
 @brief   The QMUX proxy to connect the cells and host by socket

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

#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cutils/log.h>
#include <private/android_filesystem_config.h>

#include "socketproxy.h"

#define MAX_CELLS_NUM 2

#define QMI_QMUX_IF_CONN_SOCKET_FILE            "qmux_connect_socket"

#define QMI_QMUX_IF_CLIENT_SOCKET_FILE    "qmux_client_socket"

#define LOG_TAG "QMUX-PROXY"

static int cells[MAX_CELLS_NUM];

//#define QMUXPROXY_DEBUG
#ifdef QMUXPROXY_DEBUG
int user_process_server_msg(
    void *param,
    int client_fd);

int user_process_client_msg(
    void *param,
    int client_fd);


int debug_process_client_msg(
    void *param,
    int client_fd)
{
    char str[4*1024];
    socket_pair *p_socket_pair = (socket_pair*) param;

    memcpy(str, p_socket_pair->listener_rx_buf,
            p_socket_pair->listener_rx_buf_index);
    str[p_socket_pair->listener_rx_buf_index] = '\0';

    ALOGD("%s: >>>>receive the msg from client(%d): %s\n",
            LOG_TAG, client_fd, str);

    return 0;
}

int debug_process_server_msg(
    void *param,
    int connect_fd)
{
    char str[4*1024];
    socket_pair *p_socket_pair = (socket_pair*) param;

    memcpy(str, p_socket_pair->connect_rx_buf,
            p_socket_pair->connect_rx_buf_index);
    str[p_socket_pair->connect_rx_buf_index] = '\0';

    ALOGD("%s: <<<<client(%d) receive the msg: %s\n",
            LOG_TAG, connect_fd, str);

    return 0;
}
#endif

int client_addr_convert_connect_bind_addr(
    void *param,
    const char *client_addr,
    char *convert_client_addr,
    char *connect_bind_addr)
{
    socket_pair *p_socket_pair = (socket_pair*) param;

    sprintf(convert_client_addr, "/data/cells/cell%d%s",
            *((int*) (p_socket_pair->user_data)), client_addr);

    sprintf(connect_bind_addr, "%s_%d",
            client_addr, *((int*) (p_socket_pair->user_data)));

    ALOGD("%s: client_addr_convert_connect_bind_addr: %s, %s\n", LOG_TAG,
            convert_client_addr, connect_bind_addr);

    return 0;
}

static int dir_exists(
    const char *path)
{
    struct stat buf;
    if (stat(path, &buf) < 0)
        return 0;
    return S_ISDIR(buf.st_mode);
}

void add_socketpair_by_cells_and_client(
    socket_pair *head_socket_pair,
    int cell,
    char* module)
{
    char dir_path[1024];
    char path_client[1024];
    char path_server[1024];
    sprintf(dir_path, "/data/cells/cell%d/dev/socket/qmux_%s/", cell, module);
    while (!dir_exists(dir_path))
    {
        sleep(1);
    }
    ALOGD("%s: add cell%d %s\n", LOG_TAG, cell, module);
    sprintf(path_client, "/data/cells/cell%d/dev/socket/qmux_%s/%s_proxy", cell,
            module, QMI_QMUX_IF_CONN_SOCKET_FILE);
    sprintf(path_server, "/dev/socket/qmux_%s/%s", module,
            QMI_QMUX_IF_CONN_SOCKET_FILE);
#ifdef QMUXPROXY_DEBUG
    socketpair_list_add(head_socket_pair, path_client, path_server,
            client_addr_convert_connect_bind_addr,
            NULL, NULL, debug_process_client_msg, debug_process_server_msg,
            (void*) &cells[cell - 1], NULL);
#else
    socketpair_list_add(head_socket_pair, path_client, path_server,
            client_addr_convert_connect_bind_addr,
            NULL, NULL, NULL, NULL,
            (void*) &cells[cell - 1], NULL);
#endif
}

void add_socketpair_by_host_and_client(
    socket_pair *head_socket_pair,
    int cell,
    char* module)
{
    char dir_path[1024];
    char path_client[1024];
    char path_server[1024];
    sprintf(dir_path, "/dev/socket/qmux_%s/", module);
    while (!dir_exists(dir_path))
    {
        sleep(1);
    }
    ALOGD("%s: add cell%d %s\n", LOG_TAG, cell, module);
    sprintf(path_client, "/dev/socket/qmux_%s/%s_proxy",
            module, QMI_QMUX_IF_CONN_SOCKET_FILE);
    sprintf(path_server, "/dev/socket/qmux_%s/%s", module,
            QMI_QMUX_IF_CONN_SOCKET_FILE);
#ifdef QMUXPROXY_DEBUG
    socketpair_list_add(head_socket_pair, path_client, path_server,
            client_addr_convert_connect_bind_addr,
            NULL, NULL, debug_process_client_msg, debug_process_server_msg,
            (void*) &cells[cell - 1], NULL);
#else
    socketpair_list_add(head_socket_pair, path_client, path_server,
            client_addr_convert_connect_bind_addr,
            NULL, NULL, NULL, NULL,
            (void*) &cells[cell - 1], NULL);
#endif
}

int main(
    int argc,
    char *argv[])
{
    int i, j;
    char path_client[1024];
    char path_server[1024];
    socket_pair *head_socket_pair;

    for (i = 0; i < MAX_CELLS_NUM; i++)
    {
        cells[i] = i + 1;
    }

    socketpair_list_init(&head_socket_pair);
    add_socketpair_by_cells_and_client(head_socket_pair, 1, "gps");
    add_socketpair_by_cells_and_client(head_socket_pair, 1, "audio");
    add_socketpair_by_cells_and_client(head_socket_pair, 1, "radio");
    //add_socketpair_by_cells_and_client(head_socket_pair, 1, "bluetooth");
    add_socketpair_by_host_and_client(head_socket_pair, 2, "gps");
    add_socketpair_by_host_and_client(head_socket_pair, 2, "audio");
    add_socketpair_by_host_and_client(head_socket_pair, 2, "radio");
    //add_socketpair_by_cells_and_client(head_socket_pair, 2, "bluetooth");

    pthread_t thread_listenner, thread_connect;
    if ((pthread_create(&thread_listenner,
    NULL, socketpair_list_listener_new_task, (void *) head_socket_pair))
            != 0)
    {
        ALOGD("%s: can't create listenner thread", LOG_TAG);
    }

    if ((pthread_create(&thread_connect,
    NULL, socketpair_list_connect_new_task, (void *) head_socket_pair))
            != 0)
    {
        ALOGD("%s: can't create connect thread", LOG_TAG);
    }

    while (1)
    {
        sleep(UINT32_MAX);
    }

    return 0;
}

