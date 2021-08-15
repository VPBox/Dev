/*
 * cell_config.h
 *
 * Cell configuration definitions and routines
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
#ifndef CELL_CONFIG_H
#define CELL_CONFIG_H

#define MAX_CONFIG_KEY_LEN 50 /* Max length of config keywords */
#define MAX_CONFIG_LINE_LEN   MAX_CONFIG_KEY_LEN + MAX_MSG_LEN
#define CONFIG_DIR ".cellconf"

struct config_info {
	int newcell;
	int uts_ns;
	int ipc_ns;
	int user_ns;
	int net_ns;
	int pid_ns;
	int mount_ns;
	int mnt_rootfs;
	int mnt_tmpfs;
	int newpts;
	int newcgrp;
	int share_dalvik_cache;
	int sdcard_branch;
	int autostart;
	int autoswitch;
	int wifiproxy;
	int initpid;
	int restart_pid; /* pid of restart process */
	int id;
	int console;
};

int cell_exists(char *name);
int id_exists(int id);
int read_config(char *name, struct config_info *config);
int write_config(char *name, struct config_info *config);
int remove_config(char *name);
void init_config(struct config_info *config);

char **get_cell_names(void);
void free_cell_names(char **name_list);
void start_args_to_config(struct cell_start_args *args,
			  struct config_info *config);
void config_to_start_args(struct config_info *config,
			  struct cell_start_args *args);

void init_cellvm_config(char* name);

#endif
