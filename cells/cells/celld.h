/*
 * celld.h
 *
 * Structures and definitions for the Cells controlling daemon, celld
 *
 * Copyright (C) 2010-2013 Columbia University
 * Authors: Christoffer Dall <cdall@cs.columbia.edu>
 *		  Jeremy C. Andrus <jeremya@cs.columbia.edu>
 *		  Alexander Van't Hof <alexvh@cs.columbia.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
#ifndef CELLD_H
#define CELLD_H

#include <pthread.h>
#include <stdarg.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/un.h>

#include <linux/sched.h>
#include <linux/socket.h>

#include "cell_console.h"

#define SOCKET_PATH "celld"
#define PRIV_SOCKET_PATH "TODO"
#define CELLD_LOCKFILE "/data/.celldlock"
#define DEFL_CELL_DIR "/data/cells"
#define DEFL_SDCARD_ROOT "/mnt/user/cells"
#define DEFL_START_OPTS "-MSWdgimpwun"

#define CONSOLE_READY_MSG "ready"
#define CONSOLE_READY_MSG_LEN 8 
#define MAX_MSG_LEN 512
#define MAX_NAME_LEN 64
#define MAX_ARGS 20
#define MAX_PATH_LEN 256

#define MAX_CELL_NUM 5

/* Update CELL_CMD_REV if you change this enum in any way */
enum cell_cmd {
	CELL_CREATE,
	CELL_DESTROY,
	CELL_LIST,
	CELL_NEXT,
	CELL_PREV,

	CELL_START,
	CELL_STOP,
	CELL_SWITCH,
	CELL_CONSOLE,

	CELL_AUTOSTART,
	CELL_AUTOSWITCH,
	CELL_GETID,
	CELL_SETID,
	CELL_GETACTIVE,
	CELL_MOUNT,
	CELL_UNMOUNT,

	CELL_RUNCMD,

	CELL_MAXCOMMAND
};

/* Update CELL_CMD_REV if you change this struct in any way */
struct cell_create_args {
	int id; /* -1: unspecified */
};

/* Update CELL_CMD_REV if you change this struct in any way */
struct cell_list_args {
	char all;
	char running;
	char zombie;
};

/* Update CELL_CMD_REV if you change this struct in any way */
struct cell_start_args {
	char noopt; /* no start opions - use default or stored */
	char uts_ns;
	char ipc_ns;
	char user_ns;
	char net_ns;
	char pid_ns;
	char mount_ns;
	char mnt_rootfs;
	char mnt_tmpfs;
	char newpts;
	char newcgrp;
	char share_dalvik_cache;
	char sdcard_branch;
	char wifiproxy;
	char open_console;
	char autoswitch;
	char pid_file[MAX_PATH_LEN];
	char wait;
};

/* Update CELL_CMD_REV if you change this struct in any way */
/* if neither field is set, the status is retrieved */
struct cell_autostart_args {
	char on;
	char off;
};

/* Update CELL_CMD_REV if you change this struct in any way */
struct cell_setid_args {
	int id;
};

/* Update CELL_CMD_REV if you change this struct in any way */
struct cell_mount_args {
	int all;
};

/* Update CELL_CMD_REV if you change this struct in any way */
struct cell_runcmd_args {
	char cmd[MAX_PATH_LEN];
};

/* Update this number if you change cell_cmd_arg in any way */
#define CELL_CMD_REV (0xbabe0010)
struct cell_cmd_arg {
	enum cell_cmd cmd;
	char cellname[MAX_NAME_LEN];
	union {
		struct cell_create_args		create_args;
		struct cell_list_args		list_args;
		struct cell_start_args		start_args;
		struct cell_setid_args		setid_args;
		struct cell_mount_args		mount_args;
		struct cell_autostart_args	autostart_args;
		struct cell_runcmd_args		runcmd_args;
	};
};

struct cell_args {
	struct cell_start_args start_args;
	char cellname[MAX_NAME_LEN];
	struct timeval start_time;
	char *rootdir;
	int init_pid;
	int restart_pid;
	char **argv;
	int argc;
	int cell_socket;
	int wifi_socket;
	int child_pipe[2];
	int init_pipe[2];
};

struct cell_node {
	short init_pid;
	char name[MAX_NAME_LEN];
	struct pty_info console_pty;
	short starting;
	short wifi_proxy;
	short id;
	short non_child;
	struct timeval start_time;
	struct cell_node *next;
	struct cell_node *prev;
};

struct cell_list {
	struct cell_node *head;
	struct cell_node *tail;
	pthread_mutex_t mutex;
};

struct cell_monitor_state {
	char name[MAX_NAME_LEN];
	int pid;
	int child_fd;
	int init_fd;
};

extern struct cell_node *active_cell;
struct cell_node *search_cells_path(char *root_path);
void switch_to_next(void);

int init_addr(struct sockaddr_un *addr);
int _send_msg(int fd, const char *fmt, va_list ap);
int send_msg(int fd, const char *fmt, ...);
int recv_msg_len(int fd, char **tok, char **msg, int len);
int recv_msg(int fd, char **tok, char **msg);
int send_fd(int conn_fd, int fd);
int recv_fd(int conn_fd);

extern char *get_rw_path(const char *name);
extern char *get_root_path(const char *name);
extern int do_share_dalvik_cache(char *root_path);

int mount_cell(char *name, int sdcard_mnt);
int unmount_all(const char *root_path, int mount_fs);

/* non-exported bionic functions */
extern pid_t __pthread_gettid(pthread_t tid);

#endif /* CELLD_H */
