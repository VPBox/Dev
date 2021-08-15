/*
 * nsexec.h
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
#ifndef NSEXEC_H
#define NSEXEC_H

extern int mount_dev_tmpfs(char *root_path);
extern int mount_freezer_on_dev(char *root_path);

extern void tear_down_cell(struct cell_args *args,
			   struct pty_info *console_pty);

extern int cell_nsexec(int sd, struct cell_args *args,
			   char *name, struct pty_info *pty);

extern char *create_cgroup(char *cellname);

#endif
