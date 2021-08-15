/*
 * cell_console.h
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
#ifndef CELL_CONSOLE_H
#define CELL_CONSOLE_H

#define PATH_MAX 4096

struct pty_info {
	int  ptm;			/* Master pts file descriptor		 */
	int  pty;			/* Slave file descriptor			  */
	char name[PATH_MAX];		/* Path to slave in host /dev/pts	 */
	char cont_path[PATH_MAX];	/* Host path to file inside container */
};

int open_cell_console(int ptm, const char *cmd, const char *args);
int create_cell_console(const char *container_root, struct pty_info *pi);
void cleanup_cell_console(struct pty_info *pi);

#endif
