/*
 * cell_config.c
 *
 * Cells configuration file manipulation
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
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define LOG_TAG "Cells/config"
#include <cutils/log.h>

#include "celld.h"
#include "cell_config.h"
#include "util.h"

extern char *g_cell_dir;

static char *get_config_path(char *name)
{
	char *config_path = malloc(strlen(g_cell_dir) + strlen(name) +
				strlen(CONFIG_DIR) + 8); /* +8 for '//.conf\0' */
	if (config_path == NULL) {
		ALOGE("Failed to malloc for config_path: %s", strerror(errno));
		return NULL;
	}
	sprintf(config_path, "%s/%s/%s.conf", g_cell_dir, CONFIG_DIR, name);
	return config_path;
}

static char *get_config_dir()
{
	char *config_dir = malloc(strlen(g_cell_dir) + strlen(CONFIG_DIR) + 2);
	if (config_dir == NULL) {
		ALOGE("Malloc of config_dir failed: %s", strerror(errno));
		return NULL;
	}
	sprintf(config_dir, "%s/%s", g_cell_dir, CONFIG_DIR);
	return config_dir;
}

static FILE *open_config(char *name, char *mode)
{
	char *config_path;
	FILE *fp;
	config_path = get_config_path(name);
	if (config_path == NULL)
		return NULL;

	fp = fopen(config_path, mode);
	if (fp == NULL) {
		ALOGE("Could not open %s: %s", config_path, strerror(errno));
		return NULL;
	}
	free(config_path);
	return fp;
}

int cell_exists(char *name)
{
	int ret;
	char *config_path = get_config_path(name);
	if (config_path == NULL)
		return -1;
	ret = file_exists(config_path);
	free(config_path);
	return ret;
}

/* Check's if a given ID is already being used by a cell.
 * This is inefficient. It would be nice to maintain an index of IDs. */
int id_exists(int id)
{
	int i;
	int result = 0;
	struct config_info config;
	char **name_list = get_cell_names();
	if (name_list == NULL)
		return -1;

	for (i = 0; name_list[i] != NULL; ++i) {
		if (read_config(name_list[i], &config) == -1)
			return -1;
		if (config.id == id) {
			result = 1;
			break;
		}
	}

	free_cell_names(name_list);
	return result;
}

struct conf_key_offset {
	char *key;
	int offset;
};

struct conf_key_offset config_keys[] = {
	{ "newcell",	offsetof(struct config_info, newcell) },
	{ "uts_ns",	offsetof(struct config_info, uts_ns) },
	{ "ipc_ns",	offsetof(struct config_info, ipc_ns) },
	{ "user_ns",	offsetof(struct config_info, user_ns) },
	{ "net_ns",	offsetof(struct config_info, net_ns) },
	{ "pid_ns",	offsetof(struct config_info, pid_ns) },
	{ "mount_ns",	offsetof(struct config_info, mount_ns) },
	{ "mnt_rootfs",	offsetof(struct config_info, mnt_rootfs) },
	{ "mnt_tmpfs",	offsetof(struct config_info, mnt_tmpfs) },
	{ "newpts",	offsetof(struct config_info, newpts) },
	{ "newcgrp",	offsetof(struct config_info, newcgrp) },
	{ "sdcard_branch",	offsetof(struct config_info, sdcard_branch) },
	{ "autostart",	offsetof(struct config_info, autostart) },
	{ "autoswitch",	offsetof(struct config_info, autoswitch) },
	{ "initpid",	offsetof(struct config_info, initpid) },
	{ "id",		offsetof(struct config_info, id) },
	{ "console",	offsetof(struct config_info, console) },
};

static int *config_val_ptr(struct config_info *config, char *key)
{
	unsigned int i;
	struct conf_key_offset *ko;

	for (i = 0; i < sizeof(config_keys) / sizeof(*ko); i++) {
		ko = &config_keys[i];
		if (strcmp(ko->key, key) == 0)
			return (int *)((char *)config + ko->offset);
	}

	return NULL;
}

int read_config(char *name, struct config_info *config)
{
	char line[MAX_CONFIG_LINE_LEN];
	char scanf_fmt[25];
	char key[MAX_CONFIG_KEY_LEN];
	char value[MAX_MSG_LEN];
	int ret;
	FILE *fp;

	init_cellvm_config(name);
	fp = open_config(name, "r");
	if (fp == NULL)
		return -1;

	/* init the config struct in case the config file is incomplete */
	init_config(config);

	/* Generate sscanf format string so that we can use MAX_MSG_LEN */
	/* aka a lot of complication just to utilize #defs */
	if (snprintf(scanf_fmt, 25, "%%%ds %%%d[^\n]s",
			 MAX_CONFIG_KEY_LEN-1, MAX_MSG_LEN) >= 25) {
		/* Never gonna happen... */
		ALOGE("How big are the #def LENs!? geeesh...");
		goto err_read_config;
	}

	/* Read through the configuration file */
	while (fgets(line, MAX_CONFIG_LINE_LEN, fp) != NULL) {
		ret = sscanf(line, scanf_fmt, key, value);
		if (ret != 2) {
			ALOGE("Invalid config line: %s", line);
			goto err_read_config;
		}

		int *ptr = config_val_ptr(config, key);
		if (!ptr) {
			ALOGE("Invalid config line: %s", line);
			goto err_read_config;
		}

		*ptr = atoi(value);
	}
	fclose(fp);
	return 0;

err_read_config:
	fclose(fp);
	if(remove_config(name) == 0) return read_config(name, config);
	else return -1;
}

/* Creates the configuration directory if it doesn't already exist */
static int cond_create_config_dir(void)
{
	char *config_dir = get_config_dir();
	if (config_dir == NULL)
		return -1;

	if (dir_exists(config_dir))
		goto cond_create_config_dir_out;

	if (mkdir(config_dir, 0700) == -1) {
		ALOGE("mkdir %s failed: %s", config_dir, strerror(errno));
		free(config_dir);
		return -1;
	}

cond_create_config_dir_out:
	free(config_dir);
	return 0;
}

int write_config(char *name, struct config_info *config)
{
	FILE *fp;
	int *ptr;
	struct conf_key_offset *ko;
	unsigned int i;

	if (cond_create_config_dir() == -1)
		return -1;

	fp = open_config(name, "w+");
	if (fp == NULL)
		return -1;

	for (i = 0; i < sizeof(config_keys) / sizeof(*ko); i++) {
		ko = &config_keys[i];
		ptr = (int *)((char *)config + ko->offset);
		if (fprintf(fp, "%s %d\n", ko->key, *ptr) == -1)
			goto err_write_config;
	}

	fclose(fp);
	return 0;

err_write_config:
	ALOGE("Error writing to config file: %s", strerror(errno));
	fclose(fp);
	return -1;
}

int remove_config(char *name)
{
	char *config_path;
	int ret;

	if (!cell_exists(name))
		return 0;

	config_path = get_config_path(name);
	if (config_path == NULL)
		return -1;

	ret = unlink(config_path);
	free(config_path);
	return ret;
}

/* Used to free the list of cells obtained from get_cell_names() */
void free_cell_names(char **name_list)
{
	int i;
	for (i = 0; name_list[i] != NULL; ++i) {
		free(name_list[i]);
	}
	free(name_list);
}

/* Returns a list of cells with config files AKA a list of cells that exist */
char **get_cell_names(void)
{
	int ret, i = 0;
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	char **name_list;
	char **tmp;
	int max_size = 10;
	char *dot_pos;
	char *new;
	char *config_dir = get_config_dir();
	if (config_dir == NULL)
		return NULL;

	/* Initialize the list */
	name_list = malloc(sizeof(char *)*max_size);
	if (name_list == NULL) {
		ALOGE("malloc of name_list failed");
		return NULL;
	}
	name_list[0] = NULL;

	/* Try to treat path as a dir. Either opens it or tells us it's not */
	dir = opendir(config_dir);
	free(config_dir);
	if (dir == NULL)
		return name_list; /* Assume no configs exist */

	/* Go through every file in the config directory to find configs */
	for (ret = readdir_r(dir, &entry, &result);
		 ret == 0 && result != NULL;
		 ret = readdir_r(dir, &entry, &result))
	{
		/* Verify it's a config file - ignore others */
		dot_pos = strrchr(entry.d_name, '.');
		if (dot_pos == NULL)
			continue;
		if (strcmp(dot_pos, ".conf"))
			continue;

		/* pseudo rstrip the ".conf" */
		dot_pos[0] = '\0';
		/* Add name of cell to list */
		/* Expand array if necessary...guess I could of used cutils */
		if (i == max_size - 1) { /* -1 to leave room for NULL */
			tmp = realloc(name_list, sizeof(char *)*max_size*2);
			if (tmp == NULL)
				goto err_get_cell_names;
			name_list = tmp;
			max_size = max_size*2;
		}
		new = malloc(strlen(entry.d_name) + 1);
		if (new == NULL)
			goto err_get_cell_names;
		strcpy(new, entry.d_name);
		name_list[i] = new;
		++i;
	}
	closedir(dir);
	/* Add terminating NULL */
	name_list[i] = NULL;
	return name_list;

err_get_cell_names:
	ALOGE("malloc/realloc during get_cell_names failed");
	closedir(dir);
	name_list[i] = NULL;
	free_cell_names(name_list);
	return NULL;
}

/* Initializes a configuration structure to defaults */
void init_config(struct config_info *config)
{
	memset(config, 0, sizeof(*config));
	config->newcell = 1;
	config->id = -1;
	config->initpid = -1;
	config->restart_pid = -1;
}

void config_to_start_args(struct config_info *config,
			  struct cell_start_args *args)
{
	args->uts_ns = config->uts_ns;
	args->ipc_ns = config->ipc_ns;
	args->user_ns = config->user_ns;
	args->net_ns = config->net_ns;
	args->pid_ns = config->pid_ns;
	args->mount_ns = config->mount_ns;
	args->mnt_rootfs = config->mnt_rootfs;
	args->mnt_tmpfs = config->mnt_tmpfs;
	args->newpts = config->newpts;
	args->newcgrp = config->newcgrp;
	args->sdcard_branch = config->sdcard_branch;
	args->open_console = config->console;
	/* only set autoswitch if it was _not_ given on the command-line */
	if (!args->autoswitch)
		args->autoswitch = config->autoswitch;
}

void start_args_to_config(struct cell_start_args *args,
			  struct config_info *config)
{
	config->uts_ns = args->uts_ns;
	config->ipc_ns = args->ipc_ns;
	config->user_ns = args->user_ns;
	config->net_ns = args->net_ns;
	config->pid_ns = args->pid_ns;
	config->mount_ns = args->mount_ns;
	config->mnt_rootfs = args->mnt_rootfs;
	config->mnt_tmpfs = args->mnt_tmpfs;
	config->newpts = args->newpts;
	config->newcgrp = args->newcgrp;
	config->sdcard_branch = args->sdcard_branch;
	config->console = args->open_console;

	/*
	 * don't automatically include autoswitch here: force the use of
	 * the 'autoswitch' celld command instead
	 */
}

void init_cellvm_config(char* name)
{
	char *config_path;
	struct config_info config;

	config_path = get_config_path(name);
	if (config_path == NULL)
		return;

	if(file_exists(config_path)){
		free(config_path);
		return;
	}

	free(config_path);

	init_config(&config);
	config.uts_ns = 1;
	config.ipc_ns = 1;
	config.user_ns = 0;
	config.net_ns = 1;
	config.pid_ns = 1;
	config.mount_ns = 1;
	config.mnt_rootfs = 1;
	config.mnt_tmpfs = 0;
	config.newpts = 0;
	config.newcgrp = 1;
	config.sdcard_branch = 0;
	config.console = 0;
	config.autostart = 1;
	config.autoswitch = 0;
	config.newcell = 0;
	config.id = 1;

	write_config(name,&config);

	/*
	 * init cell1's config and dir if the config don't has
	 * 
	 */
}
