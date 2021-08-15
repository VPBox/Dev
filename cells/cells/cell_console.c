/*
 * cell_console.c
 *
 * Routines supporting consoles on pseudo terminals within a cell
 *
 * Copyright (C) 2010-2013 Columbia University
 * Authors: Christoffer Dall <cdall@cs.columbia.edu>
 *		  Jeremy C. Andrus <jeremya@cs.columbia.edu>
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
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>

#define LOG_TAG "Cells/console"
#include <cutils/log.h>

#include "cell_console.h"
#include "ext/glibc_pty.h"

#define PRINT_BUF_LEN 256
static void cell_console_rw_loop(int ptm, const char *cmd, const char *args)
{
	fd_set rfds;
	fd_set efds;
	char buf[PRINT_BUF_LEN];
	int ret;

	FD_ZERO(&rfds);
	FD_ZERO(&efds);

	if (cmd) {
		write(ptm, cmd, strlen(cmd));
		if (args && *args) {
			write(ptm, " ", 1);
			write(ptm, args, strlen(args));
		}
		write(ptm, "\r\n", 2);
	}

	while (1) {
		FD_SET(0, &rfds);
		FD_SET(ptm, &rfds);
		FD_SET(ptm, &efds);

		ret = select(ptm + 1, &rfds, NULL, &efds, NULL);
		if (ret < 0 && errno != EINTR) {
			ALOGE("select error:%d", errno);
			break;
		}

		if (FD_ISSET(0, &rfds)) {
			/* Read something from stdin and write to pty */
			if ((ret = read(0, buf, PRINT_BUF_LEN)) > 0) {
				if (ret == 1 && buf[0] == 0x4) {
					/* Catch ctrl+d control code */
					kill(getppid(), SIGINT);
					break;
				}
				write(ptm, buf, ret);
			}
		}
		if (FD_ISSET(ptm, &rfds)) {
			/* Read something from pty and write to stdout */
			ret = read(ptm, buf, PRINT_BUF_LEN);
			if (ret > 0)
				write(1, buf, ret);
			if (cmd && (ret <= 0 || buf[ret-1] == 0x4))
				break;
		}
		if (FD_ISSET(ptm, &efds)) {
			ALOGE("console ptm:%d error: exiting", ptm);
			break;
		}
	}

	if (ret < 0)
		perror("read tty's ptm");
}

static void console_run_cmd(int ptm, const char *cmd)
{
	fd_set rfds;
	fd_set efds;
	char buf[PRINT_BUF_LEN];
	int ret;

	FD_ZERO(&rfds);
	FD_ZERO(&efds);
	while (1) {
		FD_SET(0, &rfds);
		FD_SET(ptm, &rfds);
		FD_SET(ptm, &efds);

		ret = select(ptm + 1, &rfds, NULL, &efds, NULL);
		if (ret < 0 && errno != EINTR) {
			ALOGE("select error:%d", errno);
			break;
		}

		if (FD_ISSET(0, &rfds)) {
			/* Read something from stdin and write to pty */
			if ((ret = read(0, buf, PRINT_BUF_LEN)) > 0) {
				if (ret == 1 && buf[0] == 0x4) {
					/* Catch ctrl+d control code */
					kill(getppid(), SIGINT);
					break;
				}
				write(ptm, buf, ret);
			}
		}
		if (FD_ISSET(ptm, &rfds)) {
			/* Read something from pty and write to stdout */
			if ((ret = read(ptm, buf, PRINT_BUF_LEN)) > 0)
				write(1, buf, ret);
		}
		if (FD_ISSET(ptm, &efds)) {
			ALOGE("console ptm:%d error: exiting", ptm);
			break;
		}
	}

	if (ret < 0)
		perror("read tty's ptm");
}

int create_cell_console(const char *container_root, struct pty_info *pi)
{
	struct stat tty_stat;
	int fd;
	char tty_path[PATH_MAX];	/* Host tty path to file inside container */

	if (!pi || !container_root)
		return -EINVAL;

	if (openpty(&pi->ptm, &pi->pty, pi->name, NULL, NULL)) {
		perror("openpty");
		return -errno;
	}

	/* Prevent leaking the file descriptors to the container */
	fcntl(pi->ptm, F_SETFD, FD_CLOEXEC);
	fcntl(pi->pty, F_SETFD, FD_CLOEXEC);

	/* construct <root>/dev/console path */
	snprintf(pi->cont_path, sizeof(pi->cont_path),
		 "%s/dev/console", container_root);

	ALOGI("[create_cell_console] pi->cont_path[%s]", pi->cont_path);
	/* create a file to mount on if doesn't exist */
	if (stat(pi->cont_path, &tty_stat) < 0) {
		if ((fd = creat(pi->cont_path, 0666)) < 0) {
			perror("creat(pi->cont_path)");
			return -errno;
		} else {
			close(fd);
		}
	}

	if (mount(pi->name, pi->cont_path, "none", MS_BIND, 0) < 0) {
		perror("mount --bind /dev/pts/X <root>/dev/console");
		return -errno;
	}

	snprintf(tty_path, sizeof(tty_path),
		 "%s/dev/ttyHSL0", container_root);


	if (stat(tty_path, &tty_stat) < 0) {
		if ((fd = creat(tty_path, 0666)) < 0) {
			perror("creat(tty_path)");
			//return -errno;
		} else {
			close(fd);
		}
	}
	if (mount(pi->name, tty_path, "none", MS_BIND, 0) < 0) {
		perror("mount --bind /dev/pts/X <root>/dev/tty");
		//return -errno;
	}

	return 0;
}

/**
 * Opens a console using a pseudo-terminal master file descriptor.
 *
 * Catches ctrl+d to exit the container gracefully.
 *
 * @ptm: The ptm fd associated with the console
 * @cmd: an initial command to send to the console
 */
int open_cell_console(int ptm, const char *cmd, const char *args)
{
	struct termios oldtios, newtios;

	/* Save current terminal settings */
	if (tcgetattr(STDIN_FILENO, &oldtios) < 0) {
		perror("cannot get stdin termios attr");
		return -1;
	}

	/* Copy the old terminal settings and change required bits */
	memcpy(&newtios, &oldtios, sizeof(struct termios));
	newtios.c_iflag &= ~IGNBRK;
	newtios.c_iflag &= BRKINT;
	newtios.c_lflag &= ~(ECHO|ICANON|ISIG);
	newtios.c_cc[VMIN] = 1;
	newtios.c_cc[VTIME] = 0;

	/* Set new terminal settings */
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newtios) < 0) {
		perror("set stdin termoios attr");
		return -1;
	}

	/* Do console i/o to the terminal until a signal or EOF */
	cell_console_rw_loop(ptm, cmd, args);

	/* Reset the terminal settings */
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldtios) < 0) {
		perror("set stdin termoios attr");
		return -1;
	}
	return 0;
}

/**
 * Closes file descriptors and conatiner mounted slave pts.
 * Should be called when the container has exited.
 */
void cleanup_cell_console(struct pty_info *pi)
{
	close(pi->ptm);
	close(pi->pty);
	/*printf("Unmounting %s\n", pi->cont_path);*/
	umount2(pi->cont_path, MNT_FORCE); /* Ignore failure */
}
