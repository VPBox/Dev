/*
 * cell.c
 *
 * The Cells daemon client
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
#define _GNU_SOURCE
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>

#include <linux/socket.h>

#include "celld.h"
#include "cell_console.h"
#include "util.h"

static int do_command(struct cell_cmd_arg *cmd);

/*
 * Open connection with celld and return file descriptor.
 */
static int open_conn(void)
{
	int sd;
	struct sockaddr_un addr;
	size_t addr_len;

	/* Establish connection */
	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("Failed to connect to celld");
		return -1;
	}

	addr_len = init_addr(&addr);
	if (connect(sd, (struct sockaddr *) &addr, addr_len) != 0) {
		if (errno == ENOENT)
			fprintf(stderr, "Failed to connect to celld: celld " \
					"does not appear to be running\n");
		else
			perror("Failed to connect to celld");
		return -1;
	}
	return sd;
}

static int send_buf(int fd, void *data, size_t len)
{
	int ret, remain = len;
	char *buf = data;
	while (remain > 0) {
		if ((ret = write(fd, buf, remain)) <= 0)
			return -1;
		buf += ret;
		remain -= ret;
	}
	return 0;
}

/*
 * Wrapper for send_msg and recv_msg combo.
 * Send "op msg". Get two part response back in code and rmsg.
 * Use sd as socket descriptori */
static int send_command(int sd, struct cell_cmd_arg *cmd)
{
	unsigned long rev = CELL_CMD_REV;

	if (send_buf(sd, &rev, sizeof(rev))) {
		perror("sending revision");
		return -1;
	}

	if (send_buf(sd, cmd, sizeof(*cmd))) {
		perror("sending command struct");
		return -1;
	}

	return 0;
}

/* Prints out the list of cells. Note: This modifies msg */
static void print_cell_list(char *msg)
{
	char *cell, *cell_str, *cell_info;
	char *saveptr1, *saveptr2;
	int field;
	/* Break msg up into cells */
	for (;;msg = NULL) {
		cell = strtok_r(msg, "\30", &saveptr1);
		if (cell == NULL)
			break;
		/* Break cell up into info fields */
		for (field = 0, cell_str = cell; ; ++field, cell_str = NULL) {
			cell_info = strtok_r(cell_str, "\31", &saveptr2);
			if (cell_info == NULL)
				break;
			if (field == 0)
				printf("%s\t", cell_info);
			else
				printf("(%s) ", cell_info);
		}
		printf("\n");
	}
}

static int print_response(int sd, enum cell_cmd cmd)
{
	char *code;
	char *rmsg;
	FILE *out;
	int ret = 0;

	/* Get response */
	if (recv_msg(sd, &code, &rmsg) == -1) {
		fprintf(stderr, "Invalid or no response received\n");
		return -1;
	}

	if (cmd == CELL_LIST && *code == '0') {
		print_cell_list(rmsg);
		goto out;
	}

	if (*code == '0') {
		out = stdout;
		ret = 0;
	} else {
		out = stderr;
		ret = -1;
	}

	fprintf(out, "%s\n", rmsg);
out:
	free(code);
	free(rmsg);
	return ret;
}

static int do_console(int sd, int rcv_cmd)
{
	int ret = -1;
	char *code = NULL;
	char *rmsg = NULL;
	char *cmd = NULL, *args = NULL;

	/* Get response */
	if (recv_msg_len(sd, &code, &rmsg, CONSOLE_READY_MSG_LEN) == -1) {
		fprintf(stderr, "Invalid or no response received\n");
		goto out;
	}

	if (strcmp(code, "0") != 0) {
		char *code2 = NULL, *rmsg2 = NULL;
		/*
		 * receive the rest of the error message
		 * (it was truncated to CONSOLE_READY_MSG_LEN)
		 */
		if (recv_msg(sd, &code2, &rmsg2) == -1)
			fprintf(stderr, "Error communicating with CellD\n");
		else
			fprintf(stderr, "%s%s %s\n", rmsg, code2, rmsg2);
		free(code2);
		free(rmsg2);
		goto out;
	}

	if (rcv_cmd) {
		/*
		 * we don't want to block until this fd is closed
		 * (because we still need the console fd), so we
		 * transmit 2 small messages: a fixed-length message
		 * that describes the size of the subsequent message.
		 */
		char *L, *cmdlen_str = NULL;
		int cmdlen = 0;
		if (recv_msg_len(sd, &L, &cmdlen_str, 6) < 0) {
			fprintf(stderr, "Error communicating with CellD\n");
			goto out;
		}
		if (*L != 'L') {
			fprintf(stderr, "something's fishy with command (%s)\n",
				cmdlen_str);
			goto out;
		}
		cmdlen = atoi(cmdlen_str);
		free(L);
		free(cmdlen_str);
		if (recv_msg_len(sd, &cmd, &args, cmdlen) < 0) {
			fprintf(stderr, "Unable to receive command\n");
			cmd = args = NULL;
			goto out;
		}
		if (!cmd || !*cmd) {
			fprintf(stderr, "invalid command: dropping to "
				"standard console\n");
			free(cmd);
			free(args);
			cmd = args = NULL;
		}
	}

	/*
	 * We do this extra send so the above recv_msg(s) don't
	 * eat the console file descriptor.
	 */
	if (send_msg(sd, "0 %s", CONSOLE_READY_MSG) == -1) {
		fprintf(stderr, "Handshake error in console setup!\n");
		goto out;
	}

	int fd = recv_fd(sd);
	if (fd == -1) {
		fprintf(stderr, "Failed to open console\n");
		goto out;
	}

	if (!cmd)
		printf("Console opened. Use Ctrl-D to exit.\n"
			   "Press enter if you don't see a prompt.\n");
	ret = open_cell_console(fd, cmd, args);
	if (ret < 0)
		fprintf(stderr, "Failed to open console\n");
	close(fd);

out:
	free(cmd);
	free(args);
	free(code);
	free(rmsg);
	return ret;
}

static void print_usage(const char *name)
{
	const char *usage = "\n\
	-h|--help					 Print this message\n\
	<command> -h				  Print command specific help\n\
\n\
	create <name> [--id=<id>]	 Create a new cell (optional telephony id)\n\
	destroy <name> [name ...]	 Completely removes a cell (deletes FS)\n\
	next/prev					 Select next/prev cell as foreground\n\
	list [--all|--running|--zombie]\n\
								  List cells (default: --all)\n\
\n\
	start <name> [options]		Start a cell (see start -h for options)\n\
	stop <name>				   Stop a cell\n\
	switch <name>				 Switch to a cell\n\
	console <name>				Open a cell's console\n\
\n\
	autostart <name> [on|off]	 Set/Get a cell's autostart configuration\n\
	autoswitch <name> [on|off]	Set/Get a cell's autoswitch configuration\n\
	getid <name>				  Print a cell's telephony ID\n\
	setid <name> <id>			 Set a cell's telephony ID\n\
	getactive					 Print the name of the active cell\n\
	mount <name> [--all]		  Mount a cell's filesystem\n\
	unmount <name>				Unmount a cell's filesystem\n\
	runcmd <name> <cmd> [args...] Run the command <cmd> in cell <name>\n";

	fprintf(stderr, "Usage: %s [options]\n", name);
	fprintf(stderr, "%s", usage);
}

static int parse_help(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	cmd;

	int c;
	static struct option opts[] = {
		{ "help",	no_argument,		NULL, 'h' },
		{ NULL,		0,			NULL, 0 }
	};
	static char optc[] = "h";

	while ((c = getopt_long(argc, argv, optc, opts, NULL)) != EOF) {
		switch (c) {
		case 'h':
			return -1;
		default:
			fprintf(stderr, "Invalid command and/or option\n");
			return -1;
		}
	}
	return 0;
}

static int parse_none(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	return parse_help(argc, argv, cmd);
}

static int get_cellname(char *input, struct cell_cmd_arg *cmd)
{
	if (strlen(input) >= MAX_NAME_LEN) {
		fprintf(stderr, "cannot handle names exceeding %d chars\n",
			MAX_NAME_LEN);
		return -1;
	}

	strcpy(cmd->cellname, input);
	return 0;
}

static int parse_cellname(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	if (parse_help(argc, argv, cmd) < 0)
		return -1;

	if (argc != 3)
		return -1;

	if (get_cellname(argv[2], cmd))
		return -1;

	return 0;
}

static int parse_destroy(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	int ret;
	int i;

	if (parse_help(argc, argv, cmd) < 0)
		return -1;

	if (argc <= 2)
		return -1;

	for (i = optind + 1; i < argc; i++) {
		ret = get_cellname(argv[i], cmd);
		if (ret < 0)
			return ret;

		printf("Destroying cell: %s...\n", cmd->cellname);
		do_command(cmd);
	}

	return 1; /* don't issue commands */
}

static int parse_create(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	int id = -1;
	int c;
	static struct option opts[] = {
		{ "id",		required_argument,	NULL, 1 },
		{ "help",	no_argument,		NULL, 'h' },
		{ NULL,		0,			NULL, 0 }
	};
	static char optc[] = "h";

	while ((c = getopt_long(argc, argv, optc, opts, NULL)) != EOF) {
		switch (c) {
		case 1:
			id = atoi(optarg);
			break;
		case 'h':
			return -1;
		default:
			fprintf(stderr, "Invalid option '%c'\n", c);
			return -1;
		}
	}

	if (id < -1 || id > 9) {
		fprintf(stderr, "Invalid id: %d\n", id);
		return -1;
	}

	if (argc - optind != 2)
		return -1;

	if (get_cellname(argv[optind + 1], cmd))
		return -1;

	cmd->create_args.id = id;
	return 0;
}

static int parse_list(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	struct cell_list_args *args = &cmd->list_args;
	int c;
	static struct option opts[] = {
		{ "all",	no_argument,		NULL, 1 },
		{ "running",	no_argument,		NULL, 2 },
		{ "zombie",	no_argument,		NULL, 3 },
		{ "help",	no_argument,		NULL, 'h' },
		{ NULL,		0,			NULL, 0 }
	};
	static char optc[] = "h";

	while ((c = getopt_long(argc, argv, optc, opts, NULL)) != EOF) {
		switch (c) {
		case 1: args->all = 1;		break;
		case 2: args->running = 1;	break;
		case 3: args->zombie = 1;	break;
		case 'h':
			return -1;
		default:
			fprintf(stderr, "Invalid option '%c'\n", c);
			return -1;
		}
	}

	if (!args->all && !args->running && !args->zombie)
		args->all = 1;

	return 0;
}

static char *dflt_argv[5] = {
	"cell", "start", "NAME", DEFL_START_OPTS, NULL
};

static void print_start_help(const char *name)
{
	const char *usage = "\
   Valid options (default: "DEFL_START_OPTS"):\n\
	-s|--switch	make the cell active\n\
	-w|--wait	  wait for the cell to start before returning\n\
	-C			 enable cell console\n\
	-D|--defaults  use default options (shown above)\n\
	-H			 share Dalvik cache between cells\n\
	-M			 mount unionfs (aufs) filesystem\n\
	-P <pid-file>  file in which to write global pid of init\n\
	-S			 use "DEFL_SDCARD_ROOT"/<name>/sdcard as the cell's SDCARD\n\
	-U			 userid namespace\n\
	-W			 use wifi proxy\n\
	-d			 mount tmpfs on <root>/dev\n\
	-g			 launch in new cgroup\n\
	-i			 ipc namespace\n\
	-m			 mount namespace\n\
	-n			 network namespace\n\
	-p			 pid namespace (implies -m)\n\
	-t			 mount new devpts (implies -m)\n\
	-u			 utsname namespace\n";
	fprintf(stderr, "Usage: %s start <name> [options]\n", name);
	fprintf(stderr, "%s", usage);
}

static int parse_start(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	int c;
	struct cell_start_args *args = &cmd->start_args;
	char defaults = 0, no = 1;
	static struct option opts[] = {
		{ "defaults",	no_argument,		NULL, 'D' },
		{ "switch",	no_argument,		NULL, 's' },
		{ "wait",	no_argument,		NULL, 'w' },
		{ "help",	no_argument,		NULL, 'h' },
		{ NULL,		0,			NULL, 0 }
	};
	static char optc[] = "hswDdmMWRHSguUiphgntCP:";

	/*
	 * The 'no' variable should be cleared when modifying the cell startup
	 * options in a way that gets persistently stored to the config files.
	 * For example, autoswitching or not is a run-time state option
	 * only and should not clear and override all other options. On the
	 * other hand, setting the ipc_namespace indicates a new config
	 * environment and the rest of the settings get cleared at the same
	 * time.
	 */
restart_default:
	while ((c = getopt_long(argc, argv, optc, opts, NULL)) != EOF) {
		switch (c) {
		case 'd': args->mnt_tmpfs = 1;		no = 0;	break;
		case 's': args->autoswitch = 1;			break;
		case 'w': args->wait = 1;			break;
		case 'g': args->newcgrp = 1;		no = 0;	break;
		case 'm': args->mount_ns = 1;		no = 0;	break;
		case 'M': args->mnt_rootfs = 1;		no = 0;	break;
		case 'P':
			  if (strlen(optarg) >= MAX_PATH_LEN) {
				  fprintf(stderr, "pid-file path too long\n");
				  return -1;
			  }
			  strcpy(args->pid_file, optarg);
			  no = 0;
			  break;
		case 'u': args->uts_ns = 1;		no = 0;	break;
		case 'i': args->ipc_ns = 1;		no = 0;	break;
		case 'U': args->user_ns = 1;		no = 0;	break;
		case 'n': args->net_ns = 1;		no = 0;	break;
		case 'p': args->pid_ns = 1;		no = 0;	break;
		case 't': args->newpts = 1;		no = 0;	break;
		case 'C': args->open_console = 1;	no = 0; break;
		case 'W': args->wifiproxy = 1;		no = 0;	break;
		case 'H': args->share_dalvik_cache = 1;	no = 0; break;
		case 'S': args->sdcard_branch = 1;	no = 0;	break;
		case 'D': defaults = 1;				break;
		case 'h': print_start_help(argv[0]);
			  return -2;
		default:
			  fprintf(stderr, "Invalid option '%c'\n", (char)c);
			  return -1;
		}
	}

	/* Should be 3 positional args, executable, start, and cellname */
	if (optind != argc - 2)
		return -1;

	if (defaults && !no) {
		fprintf(stderr, "Error: default options selected and "
				"persistent options explicitly set\n");
		return -1;
	}

	/*
	 * So either the defaults were explicitly requested or no persistent
	 * start opts were specified, either way we set default options but
	 * also rememebr if anything was ever set
	 */
	if (defaults || no) {
		args->noopt = 0;  
		defaults = 0;
		no = 0;
		dflt_argv[2] = argv[optind + 1];
		optind = 0;
		argv = dflt_argv;
		argc = 4;
		goto restart_default;
	}

	if (args->newpts || args->pid_ns)
		args->mount_ns = 1;

	strcpy(cmd->cellname, argv[optind + 1]);

	return 0;
}

static int parse_autostart(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	if (parse_help(argc, argv, cmd) < 0)
		return -1;

	if (argc != 4 && argc != 3)
		return -1;

	if (get_cellname(argv[2], cmd))
		return -1;

	if (argc == 4 && strcmp(argv[3], "on") == 0)
		cmd->autostart_args.on = 1;
	else if (argc == 4 && strcmp(argv[3], "off") == 0)
		cmd->autostart_args.off = 1;
	else if (argc == 4)
		return -1;

	return 0;
}

static int parse_setid(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	int id;

	if (parse_help(argc, argv, cmd) < 0)
		return -1;

	if (argc != 4)
		return -1;

	if (get_cellname(argv[2], cmd))
		return -1;

	id = cmd->setid_args.id = atoi(argv[3]);
	if (id < 0 || id > 9) {
		fprintf(stderr, "Invalid id: %d\n", id);
		return -1;
	}

	return 0;
}

static int parse_mount(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	struct cell_mount_args *args = &cmd->mount_args;
	int c;
	static struct option opts[] = {
		{ "all",	no_argument,		NULL, 1 },
		{ "help",	no_argument,		NULL, 'h' },
		{ NULL,		0,			NULL, 0 }
	};
	static char optc[] = "h";

	while ((c = getopt_long(argc, argv, optc, opts, NULL)) != EOF) {
		switch (c) {
		case 1: args->all = 1;		break;
		case 'h':
			return -1;
		default:
			fprintf(stderr, "Invalid option '%c'\n", c);
			return -1;
		}
	}

	/* Should be 3 positional args, executable, mount, and cellname */
	if (optind != argc - 2)
		return -1;

	if (get_cellname(argv[optind + 1], cmd))
		return -1;

	return 0;
}

static int parse_runcmd(int argc, char **argv, struct cell_cmd_arg *cmd)
{
	struct cell_runcmd_args *args = &cmd->runcmd_args;
	char *pos;
	int i;

	/*
	 * Should be at least 4 positional args:
	 *	 executable, runcmd, cellname, command
	 */
	if (argc < 4)
		return -1;

	/* get the cellname */
	if (get_cellname(argv[2], cmd))
		return -1;

	/* copy the command and all its arguments */
	strncpy(args->cmd, argv[3], sizeof(args->cmd));
	pos = args->cmd + strlen(argv[3]);
	argc -= 4;

	for (i = 0; i < argc; i++) {
		*pos++ = ' ';
		strncpy(pos, argv[3+i],
			(int)sizeof(args->cmd) - (int)(pos - args->cmd));
		pos += strlen(argv[3+i]);
	}

	return 0;
}


struct cmd_parse {
	enum cell_cmd cmd;
	char *cmd_name;
	int (*func)(int argc, char **argv, struct cell_cmd_arg *cmd);
};

static struct cmd_parse cmd_names[] = {
	{ CELL_CREATE,		"create", 	parse_create },
	{ CELL_DESTROY,		"destroy", 	parse_destroy },
	{ CELL_LIST,		"list", 	parse_list },
	{ CELL_NEXT,		"next", 	parse_none },
	{ CELL_PREV,		"prev", 	parse_none },
	{ CELL_START,		"start", 	parse_start },
	{ CELL_STOP,		"stop", 	parse_cellname },
	{ CELL_SWITCH,		"switch", 	parse_cellname },
	{ CELL_CONSOLE,		"console", 	parse_cellname },
	{ CELL_AUTOSTART,	"autostart", 	parse_autostart },
	{ CELL_AUTOSWITCH,	"autoswitch", 	parse_autostart },
	{ CELL_GETID,		"getid", 	parse_cellname },
	{ CELL_SETID,		"setid", 	parse_setid },
	{ CELL_GETACTIVE,	"getactive", 	parse_none },
	{ CELL_MOUNT,		"mount", 	parse_mount },
	{ CELL_UNMOUNT,		"unmount", 	parse_cellname },
	{ CELL_RUNCMD,		"runcmd",	parse_runcmd },
};

static int do_command(struct cell_cmd_arg *cmd)
{
	int sd;
	int ret;

	sd = open_conn();
	if (sd < 0)
		return sd;

	ret = send_command(sd, cmd);
	if (ret < 0)
		goto out_close;

	switch (cmd->cmd) {
	case CELL_CONSOLE:
		ret = do_console(sd, 0);
		break;
	case CELL_RUNCMD:
		ret = do_console(sd, 1);
		break;
	default:
		ret = print_response(sd, cmd->cmd);
		break;
	}

out_close:
	close(sd);
	return ret;
}

int main(int argc, char **argv)
{
	struct cell_cmd_arg cmd;
	int ret;
	unsigned int i;

	if (argc < 2) {
		print_usage(argv[0]);
		exit(1);
	}

	memset(&cmd, 0, sizeof(cmd));
	ret = -1;

	/* Lookup command and parse command-specific options */
	for (i = 0; i < CELL_MAXCOMMAND; i++) {
		if (strcmp(cmd_names[i].cmd_name, argv[1]) == 0) {
			cmd.cmd = i;
			ret = cmd_names[i].func(argc, argv, &cmd);
		}
	}

	/* If no command was found, or incorrect options passed */
	if (ret == -1) {
		print_usage(argv[0]);
		exit(1);
	} else if (ret < 0)
		exit(1);
	else if (ret > 0)
		exit(0);

	return do_command(&cmd);
}
