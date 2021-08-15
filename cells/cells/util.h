/*
 * util.h
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
#ifndef UTIL_H
#define UTIL_H

#include <dirent.h>

#define __STR(X) #X
#define _STR(X) __STR(X)
#define STR(X) _STR(X)

int vdir_exists(const char *fmt, ...);
int dir_exists(const char *path);
int file_exists(const char *path);
void daemonize(void);
void close_fds(void);
int rmtree(const char *path);
int walkdir_through(void *ctx, const char *base, const char *sdir, int depth, const char *through,
		void (*callback)(void *ctx, const char *path, const char *subpath, struct dirent *e));
int walkdir(void *ctx, const char *base, const char *sdir, int depth,
		void (*callback)(void *ctx, const char *path, const char *subpath, struct dirent *e));
int copy_file(const char *src, const char *dst);

int is_mounted(const char *path);
int __unmount_dir(const char *root_path, char *dir);

int insert_module(const char *path, const char *params);

#endif
