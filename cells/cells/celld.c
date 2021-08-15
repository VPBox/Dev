/*
 * celld.c
 *
 * The Cells controlling daemon, celld
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
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <linux/socket.h>

#include <selinux/selinux.h>
#include <selinux/label.h>
#include <selinux/android.h>

#define LOG_TAG "Cells/celld"
#include <cutils/log.h>
#include <cutils/memory.h>
#include <cutils/misc.h>
#include <cutils/properties.h>

#include "array.h"
#include "cellnet.h"
#include "celld.h"
#include "cell_config.h"
#include "cell_console.h"
#include "nsexec.h"
#include "util.h"

#define MAX_CELL_AUTOSTART_ATTEMPTS 5

static void set_properties_cells(char *name,char* v)
{
	char pname[PATH_MAX] = {0};
	sprintf(pname, "persist.sys.%s.init",  name);
	property_set(pname, v);
}

char *g_cell_dir = DEFL_CELL_DIR;
char *g_sdcard_root = DEFL_SDCARD_ROOT;

static struct cell_list g_cell_list = {
	.head = NULL,
	.tail = NULL,
	.mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
};

static struct cell_list g_dead_cell_list = {
	.head = NULL,
	.tail = NULL,
	.mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
};

static Array *autostarted_array;

struct cell_node *active_cell = NULL;
pthread_mutex_t active_cell_lock = PTHREAD_MUTEX_INITIALIZER;

/* We have to prevent cells from being destroyed while starting or while
 * changing configuration options - basically a lock for the .cellconf dir */
pthread_mutex_t config_lock = PTHREAD_MUTEX_INITIALIZER;

/* sigchld handler */
pthread_mutex_t sigchld_lock = PTHREAD_MUTEX_INITIALIZER;

static void cell_cleanup_and_free(struct cell_node *cell, const char *root_path,
				  struct cell_list *list);


/* Search through cells list by cell name */
static struct cell_node *__search_cells(char *name, struct cell_list *list)
{
	struct cell_node *cell;

	pthread_mutex_lock(&list->mutex);
	cell = list->head;
	while (cell != NULL) {
		if (strcmp(cell->name, name) == 0)
			break;
		cell = cell->next;
	}
	pthread_mutex_unlock(&list->mutex);
	return cell;
}

static struct cell_node *search_cells(char *name)
{
	return __search_cells(name, &g_cell_list);
}

static struct cell_node *search_dead_cells(char *name)
{
	return __search_cells(name, &g_dead_cell_list);
}

/* Search through cells list based on the root path. */
struct cell_node *search_cells_path(char *root_path)
{
	return search_cells(basename(root_path));
}

static struct cell_node *search_cells_pid(int pid, struct cell_list *list)
{
	struct config_info config;
	struct cell_node *cell;

	pthread_mutex_lock(&list->mutex);
	cell = list->head;
	while (cell != NULL) {
		if (read_config(cell->name, &config) == 0)
			if (config.initpid == pid ||
				config.restart_pid == pid)
				break;
		cell = cell->next;
	}
	pthread_mutex_unlock(&list->mutex);

	return cell;
}


static struct cell_node *
__get_next_or_prev_cell(struct cell_node *cur, int next)
{
	struct cell_node *cell = NULL;
	/* Lock the list so no changes can be made to cur->next/prev */
	pthread_mutex_lock(&g_cell_list.mutex);
	if (cur == NULL ||
		(cur->next == NULL && cur->prev == NULL)) {
		goto err_get_next_or_prev_cell;
	}
	if (next) {
		if (cur->next != NULL)
			cell = cur->next;
		else
			cell = g_cell_list.head;
	} else {
		if (cur->prev != NULL)
			cell = cur->prev;
		else
			cell = g_cell_list.tail;
	}
err_get_next_or_prev_cell:
	pthread_mutex_unlock(&g_cell_list.mutex);
	return cell;
}

/* Returns the cell started next after cur, loops back to head node
 * if necessary 0. NULL if cur is NULL or only 1 cell is running */
static struct cell_node *get_next_cell(struct cell_node *cur)
{
	return __get_next_or_prev_cell(cur, 1);
}
static struct cell_node *get_prev_cell(struct cell_node *cur)
{
	return __get_next_or_prev_cell(cur, 0);
}

/* Create a new cell_node */
static struct cell_node *
create_cell_node(char *name, struct config_info *config,
		 struct pty_info *console_pty)
{
	struct cell_node *new = calloc(sizeof(*new), 1);
	if (new == NULL)
		return NULL;
	memset(new, 0, sizeof(*new));
	new->init_pid = config->initpid;
	new->starting = 1;
	new->id = config->id;
	int n = snprintf(new->name, MAX_NAME_LEN, "%s", name);
	if (n >= MAX_NAME_LEN || n < 0) {
		free(new);
		ALOGE("invalid cell name (must be < %d chars): \"%s\"",
			 MAX_NAME_LEN, name);
		return NULL;
	}
	memcpy(&new->console_pty, console_pty, sizeof(*console_pty));
	return new;
}

static void __add_cell_to(struct cell_node *cell, struct cell_list *list)
{
	pthread_mutex_lock(&list->mutex);
	cell->next = NULL;
	cell->prev = NULL;
	if (list->head == NULL) { /* Empty list */
		list->head = cell;
		list->tail = cell;
	}
	else { /* Add to end of list */
		cell->prev = list->tail;
		list->tail->next = cell;
		list->tail = cell;
	}
	pthread_mutex_unlock(&list->mutex);
}

static void __del_cell_from(struct cell_node *cell, struct cell_list *list)
{
	pthread_mutex_lock(&list->mutex);
	if (cell->prev != NULL)
		cell->prev->next = cell->next;
	if (cell->next != NULL)
		cell->next->prev = cell->prev;

	if (list->head == cell)
		list->head = cell->next;
	if (list->tail == cell)
		list->tail = cell->prev;
	pthread_mutex_unlock(&list->mutex);
}

/* Add a cell_node to cells list */
static void add_cell_node(struct cell_node *new)
{
	__add_cell_to(new, &g_cell_list);
}

static void mark_cell_deleted(struct cell_node *cell)
{
	/* remote the cell from active list */
	__del_cell_from(cell, &g_cell_list);

	/*
	 * add the node to the "deleted list"
	 * so SIGCHLD handler can find it
	 */
	__add_cell_to(cell, &g_dead_cell_list);
}

char *get_root_path(const char *name)
{
	/* +2 for '\0' and '/' */
	char *root_path = malloc(strlen(g_cell_dir) + strlen(name) + 2);
	if (root_path == NULL) {
		ALOGE("Failed to malloc for root_path: %s", strerror(errno));
		return NULL;
	}
	sprintf(root_path, "%s/%s", g_cell_dir, name);
	return root_path;
}

char *get_rw_path(const char *name)
{
	/* +5 for "/-rw\0" */
	char *rw_path = malloc(strlen(g_cell_dir) + strlen(name) + 5);
	if (rw_path == NULL) {
		ALOGE("Failed to malloc for rw_path: %s", strerror(errno));
		return NULL;
	}
	sprintf(rw_path, "%s/%s-rw", g_cell_dir, name);
	return rw_path;
}

static inline int is_parent_dir(const char *file_name)
{
    if(strcmp(file_name,".") == 0|| strcmp(file_name,"..") == 0)
        return 1;
    return 0;
}

static void set_full_name(const char * dir, const char *file_name, char *output, int output_len)
{
    int len = 0;
    memset(output, 0, output_len);
    strcat(output,dir);
    if((len = strlen(output)) != 1)
    {
        if(output[len - 1] == '/')
            output[len - 1] = '\0';
    }
    if(strcmp(dir,"/")!=0)
        strcat(output,"/");
    strcat(output,file_name);
}

static void rmsubstr(char* szInput,const char* szFind)
{
    char* pos = strstr(szInput, szFind);
    if (NULL != pos)
    {
        char* pos2 = pos + strlen(szFind);
        while('\0' != *pos2)
        {
            *pos++ = *pos2++;
        }
        *pos = *pos2;
    }
}

static void rename_cells_file(const char *dir)
{
    DIR *dir_p;
    struct dirent * ptr;
    struct stat buf;
    char new_d_name[PATH_MAX];

    char file_name[PATH_MAX];
    char new_file_name[PATH_MAX];

    if(!dir)
        return ;

    dir_p = opendir(dir);
    if(dir_p == NULL)
        return ;
    
    while((ptr = readdir(dir_p))!= NULL)
    {
        if(is_parent_dir(ptr->d_name))
            continue;

        set_full_name(dir, ptr->d_name, file_name, PATH_MAX);

        memset(&buf,0,sizeof(buf));
        stat(file_name,&buf);
        if(S_ISDIR(buf.st_mode))
        {
            continue;
        }

        if(S_ISREG(buf.st_mode))
        {
            if(strstr(ptr->d_name,".cell.") != NULL){
                strcpy(new_d_name, ptr->d_name);
                rmsubstr(new_d_name,".cell");
                
                set_full_name(dir, new_d_name, new_file_name, PATH_MAX);
                rename(file_name, new_file_name);
            }

        }
    }

    closedir(dir_p);
    return;
}

void copyfs_callback(void *ctx, const char *path, const char *subpath, struct dirent *e)
{
	struct stat st;
	int use_stat;
	char newpath[PATH_MAX];
	char linkpath[PATH_MAX];
	const char *root_path = (const char *)ctx;

	if (!e || !root_path)
		return;

	use_stat = (lstat(path, &st) == 0);

	snprintf(newpath, PATH_MAX, "%s%s", root_path, subpath);

	switch (e->d_type) {
	case DT_DIR:
		mkdir(newpath, 0755);
		break;
	case DT_REG:
		/*snprintf(linkpath, PATH_MAX, CELL_ETC_PATH "/%s", path);
		if (stat(linkpath, &cell_st) == 0)
			path = linkpath;*/
		copy_file(path, newpath);
		break;
	case DT_LNK:
		memset(linkpath, 0, PATH_MAX);
		//memset(newlinkpath, 0, PATH_MAX);
		if (readlink(path, linkpath, PATH_MAX) < 0)
			break;

		//snprintf(newlinkpath, PATH_MAX, "%s%s", root_path, linkpath);
		symlink(linkpath, newpath);
		break;
	default:
		break;
	};

	if (use_stat) {
		chown(newpath, st.st_uid, st.st_gid);
		chmod(newpath, st.st_mode);
	}
}

static int mount_rootfs(const char *root_path)
{
	int ret = 0;
	char dev_adb_dir_name[PATH_MAX];
	char dev_mtp_dir_name[PATH_MAX];
	char dev_ptp_dir_name[PATH_MAX];

	if (!dir_exists(root_path)) {
		errno = ENOENT;
		return -1;
	}

	if (!is_mounted(root_path))
		ret = mount("none", root_path, "tmpfs", 0, NULL);

	if (ret < 0)
		return ret;

	ret   = walkdir((void *)root_path, "/", "/", 0, copyfs_callback);
	ret += walkdir((void *)root_path, "/", "/sbin", 100, copyfs_callback);
	//ret += walkdir((void *)root_path, "/", "/root", 100, copyfs_callback);
	ret += walkdir((void *)root_path, "/", "/cells", 100, copyfs_callback);
	ret += walkdir_through((void *)root_path, "/", "/mnt", 1, "//mnt/vendor/persist", copyfs_callback);
	if (ret) {
		ALOGE("walkdir copy fail! umount...");
	//	umount(root_path);
	//	return ret;
	}

	if(is_mounted("/dev/usb-ffs/adb")){
		sprintf(dev_adb_dir_name, "%s/adb", root_path);
		mkdir(dev_adb_dir_name, 0777);
		chown(dev_adb_dir_name, 2000, 2000);
	}

	if(is_mounted("/dev/usb-ffs/mtp")){
		sprintf(dev_mtp_dir_name, "%s/mtp", root_path);
		mkdir(dev_mtp_dir_name, 0777);
		chown(dev_mtp_dir_name, 2000, 2000);
	}

	if(is_mounted("/dev/usb-ffs/ptp")){
		sprintf(dev_ptp_dir_name, "%s/ptp", root_path);
		mkdir(dev_ptp_dir_name, 0777);
		chown(dev_ptp_dir_name, 2000, 2000);
	}

	rename_cells_file(root_path);

	return 0;
}

static int mount_systemfs(const char *root_path, const char *rw_path)
{
	int ret, s_errno;
	char *mount_opts = malloc(strlen(rw_path) + 64);
	if (mount_opts == NULL)
		return -1;
	sprintf(mount_opts, "br:%s/system=rw:/system=ro", rw_path);
	char *system_path = malloc(strlen(root_path)+16);
	if (system_path == NULL) {
		free(mount_opts);
		return -1;
	}
	sprintf(system_path, "%s/system", root_path);

	ret = 0;
	if (is_mounted(system_path))
		goto out;

	ALOGD("Performing aufs mount on %s: options='%s'",
		 system_path, mount_opts);
	ret = mount("none", system_path, "aufs", 0, mount_opts);
	s_errno = errno;
out:
	free(system_path);
	free(mount_opts);
	if (ret < 0)
		errno = s_errno;
	return ret;
}

static int mount_rw_fs(const char *root_path, const char *rw_path)
{
	int ret = 0;
	char dev_data_dir_name[PATH_MAX];
	char dev_data_host_dir_name[PATH_MAX];
	char dev_persist_dir_name[PATH_MAX];
	char dev_persist_host_dir_name[PATH_MAX];
	char dev_mnt_vendor_persist_dir_name[PATH_MAX];
	char dev_mnt_vendor_persist_host_dir_name[PATH_MAX];
	char dev_metadata_dir_name[PATH_MAX];
	char dev_metadata_host_dir_name[PATH_MAX];
	//char dev_cache_dir_name[PATH_MAX];
	//char dev_cache_host_dir_name[PATH_MAX];

	errno = 0;
	sprintf(dev_data_dir_name, "%s/data", root_path);
	sprintf(dev_data_host_dir_name, "%s/data", rw_path);
	if(is_mounted("/data")){
		ret |= mount(dev_data_host_dir_name, dev_data_dir_name, NULL, MS_BIND, NULL);//rw
		ALOGD("mount %s %s = %s", dev_data_host_dir_name, dev_data_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_persist_dir_name, "%s/persist", root_path);
	sprintf(dev_persist_host_dir_name, "%s/persist", rw_path);
	if(is_mounted("/persist")){
		ret |= mount(dev_persist_host_dir_name, dev_persist_dir_name, NULL, MS_BIND, NULL);//rw
		ALOGD("mount %s %s = %s",dev_persist_host_dir_name , dev_persist_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_mnt_vendor_persist_dir_name, "%s/mnt/vendor/persist", root_path);
	sprintf(dev_mnt_vendor_persist_host_dir_name, "%s/mnt/vendor/persist", rw_path);
	if(is_mounted("/mnt/vendor/persist")){
		ret |= mount(dev_mnt_vendor_persist_host_dir_name, dev_mnt_vendor_persist_dir_name, NULL, MS_BIND, NULL);//rw
		ALOGD("mount %s %s = %s",dev_mnt_vendor_persist_host_dir_name , dev_mnt_vendor_persist_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_metadata_dir_name, "%s/metadata", root_path);
	sprintf(dev_metadata_host_dir_name, "%s/metadata", rw_path);
	if(is_mounted("/metadata")){
		ret |= mount(dev_metadata_host_dir_name, dev_metadata_dir_name, NULL, MS_BIND, NULL);//rw
		ALOGD("mount %s %s = %s",dev_metadata_host_dir_name , dev_metadata_dir_name, strerror(errno));
	}

	/*errno = 0;
	sprintf(dev_cache_dir_name, "%s/cache", root_path);
	sprintf(dev_cache_host_dir_name, "%s/cache", rw_path);
	if(is_mounted("/cache")){
		ret |= mount(dev_cache_host_dir_name, dev_cache_dir_name, NULL, MS_BIND, NULL);//rw
		ALOGD("mount %s %s = %s",dev_cache_host_dir_name , dev_cache_dir_name, strerror(errno));
	}*/

	return ret;
}

static int mount_ro_fs(const char *root_path)
{
	int ret = 0;
	char dev_system_dir_name[PATH_MAX];
	char dev_vendor_dir_name[PATH_MAX];
	char dev_vendor_dsp_dir_name[PATH_MAX];
	char dev_vendor_bt_firmware_dir_name[PATH_MAX];
	char dev_vendor_firmware_mnt_dir_name[PATH_MAX];
	char dev_firmware_dir_name[PATH_MAX];
	char dev_oem_dir_name[PATH_MAX];
	char dev_product_dir_name[PATH_MAX];
	char dev_adb_dir_name[PATH_MAX];
	char dev_mtp_dir_name[PATH_MAX];
	char dev_ptp_dir_name[PATH_MAX];

	errno = 0;
	sprintf(dev_system_dir_name, "%s/system", root_path);
	ret |= mount("/system", dev_system_dir_name, NULL, MS_BIND, NULL);//ro
	ALOGD("mount /system %s = %s", dev_system_dir_name, strerror(errno));

	errno = 0;
	sprintf(dev_vendor_dir_name, "%s/vendor", root_path);
	if(is_mounted("/vendor")){
		ret |= mount("/vendor", dev_vendor_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /vendor %s = %s", dev_vendor_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_vendor_dsp_dir_name, "%s/vendor/dsp", root_path);
	if(is_mounted("/vendor/dsp")){
		ret |= mount("/vendor/dsp", dev_vendor_dsp_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /vendor/dsp %s = %s", dev_vendor_dsp_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_vendor_bt_firmware_dir_name, "%s/vendor/bt_firmware", root_path);
	if(is_mounted("/vendor/bt_firmware")){
		ret |= mount("/vendor/bt_firmware", dev_vendor_bt_firmware_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /vendor/bt_firmware %s = %s", dev_vendor_bt_firmware_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_vendor_firmware_mnt_dir_name, "%s/vendor/firmware_mnt", root_path);
	if(is_mounted("/vendor/firmware_mnt")){
		ret |= mount("/vendor/firmware_mnt", dev_vendor_firmware_mnt_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /vendor/firmware_mnt %s = %s", dev_vendor_firmware_mnt_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_oem_dir_name, "%s/oem", root_path);
	if(is_mounted("/oem")){
		ret |= mount("/oem", dev_oem_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /oem %s = %s", dev_oem_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_firmware_dir_name, "%s/firmware", root_path);
	if(is_mounted("/firmware")){
		ret |= mount("/firmware", dev_firmware_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /firmware %s = %s", dev_firmware_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_product_dir_name, "%s/product", root_path);
	if(is_mounted("/product")){
		ret |= mount("/product", dev_product_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /product %s = %s", dev_product_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_adb_dir_name, "%s/adb", root_path);
	if(is_mounted("/dev/usb-ffs/adb")){
		ret |= mount("/dev/usb-ffs/adb", dev_adb_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /dev/usb-ffs/adb %s = %s", dev_adb_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_mtp_dir_name, "%s/mtp", root_path);
	if(is_mounted("/dev/usb-ffs/mtp")){
		ret |= mount("/dev/usb-ffs/mtp", dev_mtp_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /dev/usb-ffs/mtp %s = %s", dev_mtp_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_ptp_dir_name, "%s/ptp", root_path);
	if(is_mounted("/dev/usb-ffs/ptp")){
		ret |= mount("/dev/usb-ffs/ptp", dev_ptp_dir_name, NULL, MS_BIND, NULL);//ro
		ALOGD("mount /dev/usb-ffs/ptp %s = %s", dev_ptp_dir_name, strerror(errno));
	}

	return ret;
}

/* Mount a cell's filesystem */
int mount_cell(char *name, int sdcard_mnt)
{
	sdcard_mnt;

	int ret = -1;
	char *root_path = get_root_path(name);
	if (root_path == NULL) {
		errno = -EINVAL;
		return -1;
	}

	char *rw_path = get_rw_path(name);
	if (rw_path == NULL) {
		errno = -ENOMEM;
		free(root_path);
		return -1;
	}

	if (mount_rootfs(root_path) < 0) {
		ALOGE("Failed to mount cell's root dir");
		goto err_free_paths;
	}

	if (mount_ro_fs(root_path) < 0) {
		ALOGE("Failed to mount cell's ro dir: %s", strerror(errno));
		goto err_free_paths;
	}

	if (mount_rw_fs(root_path, rw_path) < 0) {
		ALOGE("Failed to mount cell's rw dir: %s", strerror(errno));
		goto err_free_paths;
	}

	ret = 0;
err_free_paths:
	free(rw_path);
	free(root_path);

	return ret;
}

/* construct a simple argv array mostly so I can use getopt()... */
static int construct_argv(char *options, char ***argv, int *argc)
{
	char *str, *opt;
	char *saveptr;

	*argv = malloc(sizeof(char *) * MAX_ARGS);
	if (!(*argv))
		return -1;
	(*argv)[0] = "__celld__";
	*argc = 1;
	for (str = options; *argc < MAX_ARGS; str = NULL) {
		opt = strtok_r(str, " ", &saveptr);
		if (opt == NULL)
			break;
		(*argv)[(*argc)++] = opt;
	}

	return 0;
}

/*
 * Constructs cell start args from cell_start_args
 * If the args differ from those in the cell's config, updates config.
 *
 * If noopt is set in cell_start_args, just load settings from config.
 */
static int get_cell_args(char *name, struct cell_args *cell_args)
{
	struct cell_start_args *args = &cell_args->start_args;
	int len;
	struct config_info config;

	if (read_config(name, &config) == -1) {
		ALOGE("failed to read config for %s", name);
		return -1;
	}

	ALOGD("%s is %s new cell\n", name, config.newcell ? "a" : "NOT a");
	if (args->noopt && !config.newcell) {
		/* Use options from config */
		ALOGI("start(%s): Using stored config option", name);
		config_to_start_args(&config, args);
	} else {
		/* Custom options */
		ALOGI("start(%s): Using custom options", name);
		start_args_to_config(args, &config);
		config.newcell = 0;

		/* Update config with new startopts */
		write_config(name, &config); /* (doesn't matter if it fails) */
	}

	/* Construct root_path for cell */
	cell_args->rootdir = get_root_path(name);
	if (!cell_args->rootdir) {
		ALOGE("no memory for rooth_path!");
		return -1;
	}

	/* Remove possible trailing '/' from root_path */
	len = strlen(cell_args->rootdir);
	if (cell_args->rootdir[len - 1] == '/')
		cell_args->rootdir[len - 1] = '\0';

	strcpy(cell_args->cellname, name);

	return 0;
}

static time_t tv_to_usec(struct timeval *tv)
{
	return tv->tv_usec + (1000000 * tv->tv_sec);
}

static void usec_to_tv(struct timeval *tv, time_t usec)
{
	tv->tv_sec  = usec / 1000000;
	tv->tv_usec = usec % 1000000;
}

static int finish_cell_startup(char *name)
{
	struct timeval stop_time;
	time_t delta = 0;

	if (gettimeofday(&stop_time, NULL) == -1)
		ALOGE("gettimeofday(%s) failed: %s", name, strerror(errno));
	pthread_mutex_lock(&g_cell_list.mutex);
	struct cell_node *cell = search_cells(name);
	if (cell == NULL) {
		pthread_mutex_unlock(&g_cell_list.mutex);
		return -1;
	}
	cell->starting = 0;
	pthread_mutex_unlock(&g_cell_list.mutex);
	delta = tv_to_usec(&stop_time) - tv_to_usec(&cell->start_time);
	usec_to_tv(&stop_time, delta);
	ALOGI("start(%s) duration: %ld seconds %ld microsec",
		 name, stop_time.tv_sec, stop_time.tv_usec);

	return 0;
}

static int __umount_rootmount(const char* root_path)
{
	char dev_system_dir_name[PATH_MAX];
	char dev_vendor_dir_name[PATH_MAX];
	char dev_vendor_dsp_dir_name[PATH_MAX];
	char dev_vendor_bt_firmware_dir_name[PATH_MAX];
	char dev_vendor_firmware_mnt_dir_name[PATH_MAX];
	char dev_firmware_dir_name[PATH_MAX];
	char dev_oem_dir_name[PATH_MAX];
	char dev_product_dir_name[PATH_MAX];
	char dev_adb_dir_name[PATH_MAX];
	char dev_mtp_dir_name[PATH_MAX];
	char dev_ptp_dir_name[PATH_MAX];

	char dev_data_dir_name[PATH_MAX];
	char dev_persist_dir_name[PATH_MAX];
	char dev_mnt_vendor_persist_dir_name[PATH_MAX];
	char dev_metadata_dir_name[PATH_MAX];
	//char dev_cache_dir_name[PATH_MAX];

	int ret = 0;

	errno = 0;
	sprintf(dev_vendor_dsp_dir_name, "%s/vendor/dsp", root_path);
	if(is_mounted(dev_vendor_dsp_dir_name)){
		ret |= umount(dev_vendor_dsp_dir_name);//ro
		ALOGD("umount %s = %s", dev_vendor_dsp_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_vendor_bt_firmware_dir_name, "%s/vendor/bt_firmware", root_path);
	if(is_mounted(dev_vendor_bt_firmware_dir_name)){
		ret |= umount(dev_vendor_bt_firmware_dir_name);//ro
		ALOGD("umount %s = %s", dev_vendor_bt_firmware_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_vendor_firmware_mnt_dir_name, "%s/vendor/firmware_mnt", root_path);
	if(is_mounted(dev_vendor_firmware_mnt_dir_name)){
		ret |= umount(dev_vendor_firmware_mnt_dir_name);//ro
		ALOGD("umount %s = %s", dev_vendor_firmware_mnt_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_mnt_vendor_persist_dir_name, "%s/mnt/vendor/persist", root_path);
	if(is_mounted(dev_mnt_vendor_persist_dir_name)){
		ret |= umount(dev_mnt_vendor_persist_dir_name);//rw
		ALOGD("umount %s = %s", dev_mnt_vendor_persist_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_persist_dir_name, "%s/persist", root_path);
	if(is_mounted(dev_persist_dir_name)){
		ret |= umount(dev_persist_dir_name);//rw
		ALOGD("umount %s = %s", dev_persist_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_metadata_dir_name, "%s/metadata", root_path);
	if(is_mounted(dev_metadata_dir_name)){
		ret |= umount(dev_metadata_dir_name);//rw
		ALOGD("umount %s = %s", dev_metadata_dir_name, strerror(errno));
	}

	//errno = 0;
	//sprintf(dev_cache_dir_name, "%s/cache", root_path);
	//if(is_mounted(dev_cache_dir_name)){
	//	ret |= umount(dev_cache_dir_name);//rw
	//	ALOGD("umount %s = %s" , dev_cache_dir_name, strerror(errno));
	//}

	errno = 0;
	sprintf(dev_oem_dir_name, "%s/oem", root_path);
	if(is_mounted(dev_oem_dir_name)){
		ret |= umount(dev_oem_dir_name);//ro
		ALOGD("umount %s = %s", dev_oem_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_firmware_dir_name, "%s/firmware", root_path);
	if(is_mounted(dev_firmware_dir_name)){
		ret |= umount(dev_firmware_dir_name);//ro
		ALOGD("umount %s = %s", dev_firmware_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_product_dir_name, "%s/product", root_path);
	if(is_mounted(dev_product_dir_name)){
		ret |= umount(dev_product_dir_name);//ro
		ALOGD("umount %s = %s", dev_product_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_ptp_dir_name, "%s/ptp", root_path);
	if(is_mounted(dev_ptp_dir_name)){
		ret |= umount(dev_ptp_dir_name);//ro
		ALOGD("umount %s = %s", dev_ptp_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_mtp_dir_name, "%s/mtp", root_path);
	if(is_mounted(dev_mtp_dir_name)){
		ret |= umount(dev_mtp_dir_name);//ro
		ALOGD("umount %s = %s", dev_mtp_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_adb_dir_name, "%s/adb", root_path);
	if(is_mounted(dev_adb_dir_name)){
		ret |= umount(dev_adb_dir_name);//ro
		ALOGD("umount %s = %s", dev_adb_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_data_dir_name, "%s/data", root_path);
	if(is_mounted(dev_data_dir_name)){
		ret |= umount(dev_data_dir_name);//rw
		ALOGD("umount %s = %s", dev_data_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_vendor_dir_name, "%s/vendor", root_path);
	if(is_mounted(dev_vendor_dir_name)){
		ret |= umount(dev_vendor_dir_name);//ro
		ALOGD("umount %s = %s", dev_vendor_dir_name, strerror(errno));
	}

	errno = 0;
	sprintf(dev_system_dir_name, "%s/system", root_path);
	ret |= umount(dev_system_dir_name);//ro
	ALOGD("umount  %s = %s", dev_system_dir_name, strerror(errno));

	errno = 0;
	if(is_mounted(root_path)){
		ret |= umount(root_path);//rootfs
		ALOGD("umount %s = %s" , root_path, strerror(errno));
	}

	errno = 0;
	if(is_mounted("/")){
		ret |= umount("/");//rootfs
		ALOGD("umount %s = %s" , "/", strerror(errno));
	}

	return ret;
}

static void *__monitor_start_state(void *arg)
{
	char *root_path = NULL;
	char buf[20];
	int ret, i = 0;

	struct cell_monitor_state *cms = (struct cell_monitor_state *)arg;
	root_path = get_root_path(cms->name);

	set_properties_cells(cms->name,"0");

	ALOGI("Waiting for '%s' to initialize...", cms->name);
	ret = read(cms->child_fd, buf, 1);
	if (ret == -1 || buf[0] != 1)
		ALOGE("Error waiting for '%s' initialization", cms->name);

	snprintf(buf, sizeof(buf), "%d", cms->pid);
	write(cms->init_fd, buf, strlen(buf) + 1);
	close(cms->init_fd);

	__umount_rootmount(root_path);

	sleep(2);
	sscanf(cms->name, "cell%d",&i);
	if(i > 0)starttether(i);

	finish_cell_startup(cms->name);

	ALOGI("Cell '%s' started!", cms->name);

	free(cms);
	free(root_path);
	return (void*)0;
}

/* Monitors a named pipe inside a cell. Waits for Launcher2 to send a message
 * that the cell is ready. Updates starting state of cell upon receipt.
 */
static void *__monitor_start_state_bak(void *arg)
{
	int fd;
	fd_set rfds;
	struct timeval tv;
	int ret;
	char *pipe_name = NULL;
	char *root_path = NULL;
	char buf[20];

	/* Can't keep reference to cell node if we're going to be thread safe */
	struct cell_monitor_state *cms = (struct cell_monitor_state *)arg;
	root_path = get_root_path(cms->name);

	ALOGI("Waiting for '%s' to initialize...", cms->name);
	ret = read(cms->child_fd, buf, 1);
	if (ret == -1 || buf[0] != 1)
		ALOGE("Error waiting for '%s' initialization", cms->name);

	/* Create named pipe for Launcher2 to tell us cell is ready via */
	pipe_name = (char *)malloc(PATH_MAX);
	if (!pipe_name)
		goto __monitor_start_state_err;
	memset(pipe_name, 0, PATH_MAX);
	snprintf(pipe_name, PATH_MAX, "%s/dev/celld.startpipe", root_path);
	unlink(pipe_name);
	if (mkfifo(pipe_name, 0777) < 0) {
		ALOGE("Failed to create pipe for start state update");
		goto __monitor_start_state_err;
	}
	/* Unblock the new cell and wait for it to start up */
	snprintf(buf, sizeof(buf), "%d", cms->pid);
	write(cms->init_fd, buf, strlen(buf) + 1);
	close(cms->init_fd);

	fd = open(pipe_name, O_RDONLY);
	if (fd == -1) {
		ALOGE("Cannot open start pipe %s ",strerror(errno));
		unlink(pipe_name);
		goto __monitor_start_state_err;
	}
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	/* Wait up to ten minutes - this way we can just terminate ourselves
	   if something went wrong and this thread doesn't have to be tracked */
	tv.tv_sec = 600;
	tv.tv_usec = 0;

	ALOGI("Waiting on for '%s' to start...", cms->name);
	ret = select(fd+1, &rfds, NULL, NULL, &tv);
	if (ret == -1)
		ALOGE("select() on start pipe failed %s ",strerror(errno));
	else if (ret)
		finish_cell_startup(cms->name);
	close(fd);
	unlink(pipe_name);

	ALOGI("Cell '%s' started!", cms->name);

	free(cms);
	free(pipe_name);
	free(root_path);

	return (void *)0;

__monitor_start_state_err:
	finish_cell_startup(cms->name);
	free(cms);
	free(pipe_name);
	free(root_path);
	return (void *)-1;
}

static void monitor_start_state(struct cell_node *cell, int pid,
				int child_fd, int init_fd)
{
	pthread_t tid;
	struct cell_monitor_state *cms;

	/* we can't trust that cell will still exist so copy name out of it */
	cms = (struct cell_monitor_state *)malloc(sizeof(*cms));
	if (!cms)
		return;
	memset(cms, 0, sizeof(*cms));

	strncpy(cms->name, cell->name, MAX_NAME_LEN);
	cms->pid = pid;
	cms->child_fd = child_fd;
	cms->init_fd = init_fd;
	if (pthread_create(&tid, NULL, __monitor_start_state, (void *)cms) < 0) {
		char buf[20];
		ALOGE("NOT monitoring startup state of cell!");
		snprintf(buf, sizeof(buf), "%d", pid);
		write(init_fd, buf, strlen(buf)+1);
		close(init_fd);
		close(child_fd);
	} else {
		ALOGI("thread %d monitoring start %s(%d) state",
			  (int)pthread_gettid_np(tid), cms->name, cms->pid);
	}
}

/* Creates root_path and rw_path directories if they don't already exist */
/* If (warn) this will ALOGW directory creation */
static int create_cell_dirs(const char *name, int warn)
{
	/* directories that I need to create in the r/w path
	 * to work around aufs quirks (two write branches, one on a vfat)
	 */
	static const char *rwdirs[] = {
		"system","data","metadata","persist","cache","mnt","mnt/vendor","mnt/vendor/persist",NULL
	};
	const char *pdir;
	char dpath[255];
	int i;

	char *root_path = get_root_path(name);
	if (root_path == NULL)
		return -1;

	/* Create mount point for container if it doesn't exist */
	if (!dir_exists(root_path)) {
		if (warn)
			ALOGW("Had to mkdir %s\n", root_path);
		if (mkdir(root_path, 0750) == -1) {
			ALOGE("mkdir %s failed: %s", root_path, strerror(errno));
			free(root_path);
			return -1;
		}

		selinux_android_restorecon(root_path, 0);
	}

	char *rw_path = get_rw_path(name);
	if (rw_path == NULL)
		return -1;

	/* Create read write folder for container */
	if (!dir_exists(rw_path)) {
		if (warn)
			ALOGW("Had to mkdir %s\n", rw_path);
		if (mkdir(rw_path, 0750) == -1) {
			ALOGE("mkdir %s failed: %s", rw_path, strerror(errno));
			free(rw_path);
			rmdir(root_path);
			free(root_path);
			return -1;
		}
	}

	/* make a couple directories in rw path to work around aufs quirks */
	for (i = 0, pdir=rwdirs[0]; pdir; i++, pdir = rwdirs[i]) {
		snprintf(dpath, sizeof(dpath), "%s/%s", rw_path, pdir);
		if (mkdir(dpath, 0755) == -1 && errno != EEXIST)
			ALOGE("mkdir %s failed(%d): %s", dpath, errno, strerror(errno));
	}

	free(root_path);
	free(rw_path);
	return 0;
}

/*
 * Unlocks the config_lock and then sends the message
 * (Don't perform a blocking call with a mutex held )
 */
static int unlock_send_msg(pthread_mutex_t *mtx, int fd, char *fmt, ...)
{
	int ret;
	va_list ap;

	pthread_mutex_unlock(mtx);

	va_start(ap, fmt);
	ret = _send_msg(fd, fmt, ap);
	va_end(ap);

	return ret;
}

/* Lists all created cells and their current status */
#define CELL_STOPPED	0x01
#define CELL_ZOMBIE	0x02
#define CELL_STARTING	0x04
#define CELL_ACTIVE	0x08
#define CELL_RUNNING	0x10
#define CELL_RUNNING_MASK (CELL_STARTING | CELL_ACTIVE | CELL_RUNNING)

static char *status_to_name(int status)
{
	switch (status) {
	case CELL_STOPPED:	return "";
	case CELL_ZOMBIE:	return "zombie";
	case CELL_STARTING:	return "starting";
	case CELL_ACTIVE:	return "active";
	case CELL_RUNNING:	return "running";
	default:		return "unknown";
	}
}

static void list_cells(int fd, int mask)
{
	int i, n, s;
	struct cell_node *cell;
	char **name_list;
	char msg[MAX_MSG_LEN];
	msg[0] = '\0'; /* In case cell list is empty */
	char *cur = msg;
	int n_avail = MAX_MSG_LEN;

	name_list = get_cell_names();
	if (name_list == NULL) {
		send_msg(fd, "Failed to get name list");
		return;
	}

	for (i = 0; name_list[i] != NULL; ++i) {
		cell = search_cells(name_list[i]);
		if (!cell && !search_dead_cells(name_list[i]))
			s = CELL_STOPPED;
		else if (!cell)
			s = CELL_ZOMBIE;
		else if (cell->starting)
			s = CELL_STARTING;
		else if (cell == active_cell)
			s = CELL_ACTIVE;
		else
			s = CELL_RUNNING;

		if (s & mask && s & CELL_RUNNING_MASK) {
			n = snprintf(cur, n_avail, "%s\31%s\31%i\30",
					 name_list[i], status_to_name(s),
					 cell->init_pid);
		} else if (s & mask) {
			n = snprintf(cur, n_avail, "%s\31%s\30",
					 name_list[i], status_to_name(s));
		} else {
			ALOGE("Unknown state!");
			n = 0;
		}

		n_avail -= n;
		cur += n;
		if (n_avail <= 0)
			break;
	}
	free_cell_names(name_list);
	send_msg(fd, "%s", msg);
}

/* Sends back a msg with info for each cell separated by the record separator
 * (\30) and each field of info separated by the unit separator (\31) */
static void do_list(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_list_args *args = &cmd_args->list_args;
	int mask = 0;

	if (args->all)
		mask = ~0;
	else if (args->running)
		mask = CELL_RUNNING_MASK;
	else if (args->zombie)
		mask = CELL_ZOMBIE;
	list_cells(fd, mask);
}

int __do_switch_host_and_vm(char *name)
{
	char buf[64];
	int proc_id = 1;
	int fd;
	struct cell_node *cell =NULL;

	if(0 == strncmp(name,"cell",strlen("cell"))){
		struct cell_node *cell = search_cells(name);
		if (cell == NULL) { /* Make sure cell is running */
			return -1;
		}

		proc_id = cell->init_pid;
	}

	/* Perform the switch by writing to active_ns_pid */
	fd = open("/proc/drv_ns/active_ns_pid", O_WRONLY);
	if (fd == -1) {
		return -1;
	}

	/* Do the switch */
	snprintf(buf, sizeof(buf), "%d", proc_id);
	if (write(fd, buf, strnlen(buf, sizeof(buf))) == -1) {
		close(fd);
		return -1;
	}

	pthread_mutex_lock(&active_cell_lock);
	active_cell = cell;
	pthread_mutex_unlock(&active_cell_lock);

	close(fd);
	return 0;
}


/*
 * 0 on success,
 * -code on error
 */
int __do_switch(struct cell_node *cell)
{
	char buf[64];
	int proc_fd;

	if (!cell)
		return -1;

	/* Perform the switch by writing to active_ns_pid */
	proc_fd = open("/proc/drv_ns/active_ns_pid", O_WRONLY);
	if (proc_fd == -1) {
		ALOGE("Failed to open active_ns_pid: %s", strerror(errno));
		return -1;
	}

	/* Make sure specified cell is not already active */
	pthread_mutex_lock(&active_cell_lock);
	if (cell == active_cell) {
		pthread_mutex_unlock(&active_cell_lock);
		close(proc_fd);
		return -2;
	}

	/* Do the switch */
	snprintf(buf, sizeof(buf), "%d", cell->init_pid);
	if (write(proc_fd, buf, strnlen(buf, sizeof(buf))) == -1) {
		ALOGE("Failed to write to active_ns_pid: %s", strerror(errno));
		close(proc_fd);
		pthread_mutex_unlock(&active_cell_lock);
		return -3;
	}

	close(proc_fd);

	active_cell = cell;
	pthread_mutex_unlock(&active_cell_lock);
	return 0;
}

/* Attempts to switch to the next running cell started after the active cell */
void switch_to_next(void)
{
	ALOGI("(switch to next): Looking for 'next' cell");
	struct cell_node *cell;
	pthread_mutex_lock(&g_cell_list.mutex);
	cell = get_next_cell(active_cell);
	if (cell)
		ALOGI("(next) Switching to '%s'", cell->name);
	(void)__do_switch(cell);
	pthread_mutex_unlock(&g_cell_list.mutex);
}

/* Send response message back after __do_switch */
static void switch_response(int fd, int ret, char *name)
{
	switch (ret) {
	case -1:
		send_msg(fd, "Switch failed. Couldn't open proc file");
		break;
	case -2:
		send_msg(fd, "Cell is already active");
		break;
	case -3:
		send_msg(fd, "Switch failed. Couldn't write to proc file");
		break;
	}

	/* Send success response */
	send_msg(fd, "Switched to %s", name);
}

/* Perform the next and prev commands. Set next to true if performing next. */
static void __do_next_or_prev(int fd, int next)
{
	int ret;
	struct cell_node *orig = active_cell;
	struct cell_node *cell = orig;
	pthread_mutex_lock(&g_cell_list.mutex);
	/* Loop until we've exhausted the list or we've found a running cell */
	do {
		if (next)
			cell = get_next_cell(cell);
		else
			cell = get_prev_cell(cell);
	} while (cell != NULL && cell != orig && cell->starting);

	if (cell == NULL)
		cell = g_cell_list.head;

	if (cell == NULL) {
		unlock_send_msg(&g_cell_list.mutex, fd,
				"There are no running cells");
		return;
	}

	if (cell == orig) {
		unlock_send_msg(&g_cell_list.mutex, fd,
				"Only one cell running");
		return;
	}

	ALOGI("(%s) Switching to '%s'", next ? "next" : "prev", cell->name);
	ret = __do_switch(cell);
	pthread_mutex_unlock(&g_cell_list.mutex);

	switch_response(fd, ret, cell->name);
}

/* Helper functions for prev and next commands */
static void do_next(int fd) {
	ALOGI("next: Attempting to switch to next cell");
	__do_next_or_prev(fd, 1);
}
static void do_prev(int fd) {
	ALOGI("prev: Attempting to switch to previous cell");
	__do_next_or_prev(fd, 0);
}

static void do_switch(int fd, struct cell_cmd_arg *args)
{
	char *name = args->cellname;

	int ret;
	pthread_mutex_lock(&g_cell_list.mutex);
	/*struct cell_node *cell = search_cells(name);
	if (cell == NULL) {
		unlock_send_msg(&g_cell_list.mutex, fd,
				"Switch failed. Given cell not running");
		return;
	}
	ALOGI("(switch) Switching to '%s'", name);
	ret = __do_switch(cell);*/
	ALOGI("(switch) Switching to '%s'", name);
	ret = __do_switch_host_and_vm(name);
	pthread_mutex_unlock(&g_cell_list.mutex);

	if (args->cmd == CELL_SWITCH)
		switch_response(fd, ret, name);
}

static void do_getactive(int fd, struct cell_cmd_arg *cmd_args)
{
	cmd_args;

	struct cell_node *cell;
	char name[MAX_NAME_LEN];

	name[0] = '\0';
	pthread_mutex_lock(&active_cell_lock);
	cell = active_cell;
	if (cell)
		strcpy(name, cell->name);
	pthread_mutex_unlock(&active_cell_lock);

	if (cell != NULL)
		send_msg(fd, "%s", name);
	else
		send_msg(fd, "No cell is active");
}

/* Creates a new cell. "Creation" consists only of making 2 directories.
 * This func is used largely for design reasons and error checking. I don't
 * think you should be able to start a cell you've never explicity made. */
static int __do_create(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_create_args *args = &cmd_args->create_args;
	char *name = cmd_args->cellname;
	struct config_info config;

	if (strcmp(cmd_args->cellname, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return -1;
	}

	pthread_mutex_lock(&config_lock);

	if (cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Create failed. Given name already exists.");
		return -1;
	}

	if (args->id > -1 && id_exists(args->id)) {
		unlock_send_msg(&config_lock, fd,
				"Create failed. ID already exists.");
		return -1;
	}

	init_config(&config);
	if (args->id != -1)
		config.id = args->id;
	if (write_config(cmd_args->cellname, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Create failed. Couldn't create config file.");
		return -1;
	}

	if (create_cell_dirs(cmd_args->cellname, 0) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Create failed. Couldn't create directories");
		remove_config(cmd_args->cellname);
		return -1;
	}

	pthread_mutex_unlock(&config_lock);
	return 0;
}

static void do_create(int fd, struct cell_cmd_arg *cmd_args)
{
	int ret = __do_create(fd, cmd_args);
	if (ret == 0)
		send_msg(fd, "Created %s", cmd_args->cellname);
}

/* Returns the newly created cell_node on success. NULL on failure */
static struct cell_node *__do_start(int fd, char *name,
					struct cell_start_args *args)
{
	int pid = -1;
	int config_ret;
	struct config_info config;
	struct cell_node *new;
	struct pty_info console_pty;

	struct cell_args cell_args;
	memcpy(&cell_args.start_args, args, sizeof(cell_args.start_args));

	if (get_cell_args(name, &cell_args) == -1) {
		send_msg(fd, "Start failed. Error parsing start options");
		return NULL;
	}

	/*
	 * Allocates argv on struct cell_args and sets to /init
	 */
	cell_args.argv = malloc(sizeof(char *)*2);
	if (!cell_args.argv) {
		ALOGE("No memory for cell argv");
		send_msg(fd, "Start failed. No memory for cell argv");
		return NULL;
	}

	cell_args.argv[0] = "/init";
	cell_args.argv[1] = NULL;
	cell_args.argc = 1;

	/*
	 * Start init in new namespace
	 */
	pid = cell_nsexec(fd, &cell_args, name, &console_pty);
	free(cell_args.argv);

	if (pid == -1) {
		/* nsexec() already sent and logged an error message. It
		 * will have also cleaned up after itself on an error */
		return NULL;
	}

	/* Try our best to update the config file. Not fatal if we fail. */
	config_ret = read_config(name, &config);
	config.initpid = pid;
	if (config_ret != -1)
		write_config(name, &config);

	/* Create a new node and add it to cells list */
	new = create_cell_node(name, &config, &console_pty);
	if (new == NULL) {
		/*
		 * we have to manually clean up a few things b/c we couldn't
		 * add this cell to a list!
		 */
		kill(pid, SIGKILL); /* cell's parent will handle rest */
		tear_down_cell(&cell_args, &console_pty);
		send_msg(fd, "Start failed. Couldn't create cell node (this is really bad)");
		return NULL;
	}
	new->start_time = cell_args.start_time;

	add_cell_node(new);

	if (cell_args.start_args.autoswitch) {
		ALOGI("(startup) Switching to '%s'", name);
		__do_switch(new);
	}

	monitor_start_state(new, pid,
				cell_args.child_pipe[0],
				cell_args.init_pipe[1]);

	return new;
}

static void do_start(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_start_args *args = &cmd_args->start_args;
	char *name = cmd_args->cellname;
	struct cell_node *ret;
	int index = -1;

	ALOGI("start: Start %s\n", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}

	/* Make sure cell INDEX */
	sscanf(name, "cell%d", &index);
	if(index <= 0 || index > MAX_CELL_NUM){
		send_msg(fd, "Start failed. Cell INDEX is 1~ %d.", MAX_CELL_NUM);
		return;
	}

	/* Make sure cell exists */
	pthread_mutex_lock(&config_lock);
	if (!cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Start failed. Cell does not exist.");
		return;
	}
	/* Make sure cell is not already running */
	if (search_cells(name) != NULL) {
		unlock_send_msg(&config_lock, fd,
				"Start failed. Cell is already running.");
		return;
	}

	/* Make sure the cell's directories exist */
	if (create_cell_dirs(name, 1) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Start failed. Could not create cell dirs.");
		return;
	}

	ret = __do_start(fd, name, args);
	pthread_mutex_unlock(&config_lock);

	/*
	 * __do_start already sent and logged an error message.
	 * It will have also clean up after itself on an error
	 */
	if (ret == NULL)
		return;

	if (args->wait) {
		ALOGI("cell client is waiting for '%s'...", name);
		while (ret->starting)
			sleep(1);
	}

	send_msg(fd, "Started %s", name);
	ALOGI("start: Start %s\n end ", name);
}

static void do_stop(int fd, struct cell_cmd_arg *cmd_args)
{
	char *name = cmd_args->cellname;
	int i = 0;

	ALOGI("stop: Stop %s", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}

	pthread_mutex_lock(&g_cell_list.mutex);
	struct cell_node *cell = search_cells(name);
	if (cell == NULL) {
		unlock_send_msg(&g_cell_list.mutex, fd,
				"Cell, %s, is not running", name);
		return;
	}

	char *root_path = get_root_path(name);
	if (root_path == NULL) {
		unlock_send_msg(&g_cell_list.mutex, fd,
				"Stop failed. Could not get root path.");
		return;
	}

	pthread_mutex_lock(&active_cell_lock);
	if (cell == active_cell)
		active_cell = NULL;
	pthread_mutex_unlock(&active_cell_lock);

	sscanf(name, "cell%d",&i);
	if(i > 0){
		stoptether(i);
		delveth(i);
	}

	/* Remove cell from list */
	mark_cell_deleted(cell);
	pthread_mutex_unlock(&g_cell_list.mutex);

	/* kill the init process */
	kill(cell->init_pid, SIGKILL);

	/*
	 * if we re-attached to this cell, then it's init process is not one
	 * of our children, and we have to do some manual cleanup work,
	 * otherwise our SIGCHLD handler will complete the cleanup.
	 */
	if (cell->non_child)
		cell_cleanup_and_free(cell, root_path, &g_dead_cell_list);

	free(root_path);

	send_msg(fd, "Stopped %s", name);

	set_properties_cells(name,"0");
}

static void do_destroy(int fd, struct cell_cmd_arg *cmd_args)
{
	char *name = cmd_args->cellname;

	ALOGI("stop: Destroy %s", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}

	pthread_mutex_lock(&config_lock);

	/* Make sure cell exists */
	if (!cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Destroy failed. Cell does not exist.");
		return;
	}

	/* Make sure cell is not currently running */
	if (search_cells(name) != NULL) {
		unlock_send_msg(&config_lock, fd,
				"Destroy failed. Cell is currently running.");
		return;
	}

	/* Get the paths to remove */
	char *root_path = get_root_path(name);
	if (root_path == NULL) {
		unlock_send_msg(&config_lock, fd,
				"Destroy failed. Could not get root path.");
		return;
	}
	char *rw_path = get_rw_path(name);
	if (rw_path == NULL) {
		unlock_send_msg(&config_lock, fd,
				"Destroy failed. Could not get rw path.");
		goto err_free_rootpath;
	}

	/* Remove the cell's files */
	if (dir_exists(rw_path)) {
		if (rmtree(rw_path) == -1) {
			unlock_send_msg(&config_lock, fd,
					"Could not remove rw path.");
			goto err_free_path;
		}
	}
	if (dir_exists(root_path)) {
		if (rmtree(root_path) == -1) {
			unlock_send_msg(&config_lock, fd,
					"Could not remove root path.");
			/*
			 * we've already removed the rw_path, so we'll
			 * error out, but first let's at least make it
			 * runnable again
			 */
			create_cell_dirs(name, 0);
			goto err_free_path;
		}
	}

	/* Remove config */
	if (remove_config(name) == -1) {
		create_cell_dirs(name, 0);
		unlock_send_msg(&config_lock, fd,
				"Could not remove config file.");
		goto err_free_path;
	}

	unlock_send_msg(&config_lock, fd, "Destroyed %s", name);

err_free_path:
	free(rw_path);
err_free_rootpath:
	free(root_path);
}

/* Sends back the fd associated with a cell's console */
static void do_console(int fd, struct cell_cmd_arg *args)
{
	char *code, *rmsg;
	char *name = args->cellname;
	if (strcmp(name, "") == 0) {
		send_msg(fd,"Failed to get console. You must specify a cell");
		return;
	}

	ALOGI("console: Console requested for %s", name);

	pthread_mutex_lock(&g_cell_list.mutex);
	struct cell_node *cell = search_cells(name);
	if (cell == NULL) {
		send_msg(fd, "Cell is not running");
		goto err_do_console;
	} else if (cell->console_pty.ptm == -1) {
		send_msg(fd, "Console unavailable for given cell");
		goto err_do_console;
	}

	/* Console avaiable. Send success msg followed by fd */
	if (send_msg(fd, "%s", CONSOLE_READY_MSG) == -1) {
		ALOGE("Error sending console message");
		goto err_do_console;
	}

	/*
	 * We do this extra hand-shake so the recv_msg (in cell utility)
	 * on the send_msg above doesn't eat the console file descriptor.
	 */
	if (recv_msg_len(fd, &code, &rmsg, CONSOLE_READY_MSG_LEN) < 0) {
		ALOGE("Error receiving ready message");
		goto err_do_console;
	}

	if (send_fd(fd, cell->console_pty.ptm) == -1)
		ALOGE("Failed to send console file descriptor: %s",
			 strerror(errno));

err_do_console:
	ALOGI("Finished console for %s", name);
	pthread_mutex_unlock(&g_cell_list.mutex);
}

static void do_mount(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_mount_args *args = &cmd_args->mount_args;
	char *name = cmd_args->cellname;

	ALOGI("mount: Mount request for \"%s\"", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "Failed to mount. You must specify a cell");
		return;
	}

	/* Mount the filesystem */
	if (mount_cell(name, args->all) == -1) {
		send_msg(fd, "Failed to mount filesystem for cell");
		return;
	}
	send_msg(fd, "Mounted filesystem at %s/%s", g_cell_dir, name);
}

int unmount_all(const char *root_path, int free_root)
{
	int root_path_len;
	char *mtpoint;
	int ret = 0;

	ALOGD("%s: unmounting %s (and all sub-mounts)", __func__, root_path);
	root_path_len = strlen(root_path) + 1;

	mtpoint = malloc(root_path_len);
	if (mtpoint == NULL)
		return -1;

	/* Play with pointers to save some typing */
	strcpy(mtpoint, root_path);
	if (mtpoint[root_path_len-2] == '/') {
		mtpoint[root_path_len-2] = '\0';
		root_path_len -= 1;
	}

	ret += __unmount_dir(mtpoint, "data/dalvik-cache");
	ret += __unmount_dir(mtpoint, "dev");
	ret += __unmount_dir(mtpoint, "mnt/asec");
	ret += __unmount_dir(mtpoint, "mnt/obb");
	ret += __unmount_dir(mtpoint, "mnt/secure");
	ret += __unmount_dir(mtpoint, "mnt");
	ret += __unmount_dir(mtpoint, "sys");
	ret += __unmount_dir(mtpoint, "proc");
	ret += __unmount_dir(mtpoint, "acct");

	if (free_root)
		ret += umount2(mtpoint, MNT_DETACH);
	free(mtpoint);
	return ret;
}

static void do_unmount(int fd, struct cell_cmd_arg *cmd_args)
{
	char *name = cmd_args->cellname;
	int ret;

	ALOGI("unmount: Unmounting request for \"%s\"", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "Unmount failed. You must specify a cell");
		return;
	}

	/* lock to prevent starting a cell while we're mounting */
	pthread_mutex_lock(&config_lock);
	if (search_cells(name) != NULL) {
		unlock_send_msg(&config_lock, fd,
				"Unmount failed. Cell is currently running.");
		return;
	}

	char *root_path = get_root_path(name);
	if (root_path == NULL) {
		unlock_send_msg(&config_lock, fd,
				"Failed to find rootfs for %s", name);
		return;
	}

	ret = unmount_all(root_path, 1);

	if (ret != 0)
		unlock_send_msg(&config_lock, fd,
				"Failed to unmount rootfs %s (ret=%d)",
				root_path, ret);
	else
		unlock_send_msg(&config_lock, fd,
				"Unmounted filesystem at %s", root_path);

	free(root_path);
}

static void do_runcmd(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_runcmd_args *args = &cmd_args->runcmd_args;
	char *name = cmd_args->cellname;
	struct cell_node *cell;
	char *code, *rmsg;
	int cmdlen;

	ALOGI("Running command \"%s\" in \"%s\"", args->cmd, name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "logcat failed. You must specify a cell");
		return;
	}
	cmdlen = strnlen(args->cmd, sizeof(args->cmd));
	if (cmdlen == 0 || args->cmd[0] == 0) {
		send_msg(fd, "invalid command.");
		return;
	}

	pthread_mutex_lock(&g_cell_list.mutex);
	cell = search_cells(name);
	if (cell == NULL) {
		send_msg(fd, "Cell is not running");
		goto err;
	} else if (cell->console_pty.ptm == -1) {
		send_msg(fd, "Console unavailable for given cell");
		goto err;
	}

	if (send_msg(fd, "%s L %4d%s",
			 CONSOLE_READY_MSG, cmdlen+1, args->cmd) == -1) {
		ALOGE("Error sending command");
		goto err;
	}

	/*
	 * We do this extra hand-shake so the recv_msg (in cell utility)
	 * on the send_msg above doesn't eat the console file descriptor.
	 */
	if (recv_msg_len(fd, &code, &rmsg, CONSOLE_READY_MSG_LEN) < 0) {
		ALOGE("Error receiving console ready message");
		goto err;
	}

	if (send_fd(fd, cell->console_pty.ptm) == -1)
		ALOGE("Failed to send console file descriptor: %s",
			 strerror(errno));

err:
	pthread_mutex_unlock(&g_cell_list.mutex);
}

static void do_autostart(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_autostart_args *args = &cmd_args->autostart_args;
	char *name = cmd_args->cellname;
	struct config_info config;

	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}
	/* Make sure cell exists */
	pthread_mutex_lock(&config_lock);
	if (!cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Cell, %s, does not exist", name);
		return;
	}

	if (read_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Could not read configuration file");
		return;
	}

	if (args->on)
		config.autostart = 1;
	else if (args->off) /* off */
		config.autostart = 0;
	else {
		unlock_send_msg(&config_lock, fd, "Cell autostart is %s",
				config.autostart ? "on" : "off");
		return;
	}

	ALOGI("autostart: Configure autostart for %s\n", name);

	if (write_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Could not write configuration file");
		return;
	}
	unlock_send_msg(&config_lock, fd, "Cell autostart %s",
			config.autostart ? "enabled" : "disabled");
}

static void do_autoswitch(int fd, struct cell_cmd_arg *cmd_args)
{
	struct cell_autostart_args *args = &cmd_args->autostart_args;
	char *name = cmd_args->cellname;
	struct config_info config;

	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}
	/* Make sure cell exists */
	pthread_mutex_lock(&config_lock);
	if (!cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Cell '%s' does not exist", name);
		return;
	}

	if (read_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Could not read configuration file");
		return;
	}

	if (args->on)
		config.autoswitch = 1;
	else if (args->off) /* off */
		config.autoswitch = 0;
	else {
		unlock_send_msg(&config_lock, fd, "Cell autoswitch is %s",
				config.autoswitch ? "on" : "off");
		return;
	}

	ALOGI("autoswitch: Configure autoswitch for %s\n", name);

	if (write_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Could not write configuration file");
		return;
	}
	unlock_send_msg(&config_lock, fd, "Cell autoswitch %s",
			config.autoswitch ? "enabled" : "disabled");
}

static void do_setid(int fd, struct cell_cmd_arg *cmd_args)
{
	char *name = cmd_args->cellname;
	int id = cmd_args->setid_args.id;
	struct config_info config;
	struct cell_node *cell;

	ALOGI("setid: Set ID of %s", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}

	/* Make sure cell exists */
	pthread_mutex_lock(&config_lock);
	if (!cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Setting ID failed. Cell does not exist.");
		return;
	}

	if (id < 0 || id > 9) {
		unlock_send_msg(&config_lock, fd,
				"Invalid ID given. Expecting 0-9.");
		return;
	}
	if (id_exists(id)) {
		unlock_send_msg(&config_lock, fd,
				"ID is already in use.");
		return;
	}
	if (read_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Failed to read configuration file.");
		return;
	}
	config.id = id;
	if (write_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Failed to write configuration file.");
		return;
	}

	/* Update running ID if cell is running */
	pthread_mutex_lock(&g_cell_list.mutex);
	cell = search_cells(name);
	if (cell != NULL)
		cell->id = id;
	pthread_mutex_unlock(&g_cell_list.mutex);

	unlock_send_msg(&config_lock, fd, "Changed %s's ID to %d", name, id);
}

static void do_getid(int fd, struct cell_cmd_arg *cmd_args)
{
	char *name = cmd_args->cellname;
	struct config_info config;
	ALOGI("getid: Get ID of %s", name);
	if (strcmp(name, "") == 0) {
		send_msg(fd, "You must specify a cell");
		return;
	}
	/* Make sure cell exists */
	pthread_mutex_unlock(&config_lock);
	if (!cell_exists(name)) {
		unlock_send_msg(&config_lock, fd,
				"Getting ID failed. Cell does not exist.");
		return;
	}
	if (read_config(name, &config) == -1) {
		unlock_send_msg(&config_lock, fd,
				"Failed to read configuration file.");
		return;
	}
	unlock_send_msg(&config_lock, fd, "%d", config.id);
}

static void dispatch_cell_cmd(int fd, struct cell_cmd_arg *arg)
{
	switch (arg->cmd) {
	case CELL_CREATE:	do_create(fd, arg);	break;
	case CELL_DESTROY:	do_destroy(fd, arg);	break;
	case CELL_LIST:		do_list(fd, arg);	break;
	case CELL_NEXT:		do_next(fd);		break;
	case CELL_PREV:		do_prev(fd);		break;
	case CELL_START:	do_start(fd, arg);	break;
	case CELL_STOP:		do_stop(fd, arg);	break;
	case CELL_SWITCH:	do_switch(fd, arg);	break;
	case CELL_CONSOLE:	do_console(fd, arg);	break;
	case CELL_AUTOSTART:	do_autostart(fd, arg);	break;
	case CELL_AUTOSWITCH:	do_autoswitch(fd, arg);	break;
	case CELL_GETID:	do_getid(fd, arg);	break;
	case CELL_SETID:	do_setid(fd, arg);	break;
	case CELL_GETACTIVE:	do_getactive(fd, arg);	break;
	case CELL_MOUNT:	do_mount(fd, arg);	break;
	case CELL_UNMOUNT:	do_unmount(fd, arg);	break;
	case CELL_RUNCMD:	do_runcmd(fd, arg);	break;
	default:
		ALOGE("Unknown cell command: %d\n", arg->cmd);
	}
}

static void *handle_conn(void *arg)
{
	int fd = (int)((long)arg & 0xFFFFFFFF);
	struct cell_cmd_arg cmd_args;
	char *buf;
	int remain, ret;
	unsigned long rev;

	if (read(fd, &rev, sizeof(rev)) != sizeof(rev)) {
		ALOGE("Error receiving cell cmd rev nr.: %s\n", strerror(errno));
		goto out;
	}

	if (rev != CELL_CMD_REV) {
		ALOGE("Incorrect revision %lx, expected %x\n", rev, CELL_CMD_REV);
		goto out;
	}

	buf = (char *)&cmd_args;
	remain = sizeof(cmd_args);
	while (remain > 0) {
		ret = read(fd, buf, remain);
		if (ret < 0) {
			ALOGE("Error receiving cmd args: %s\n", strerror(errno));
			goto out;
		} else if (ret == 0) {
			ALOGE("Unexpected EOF for cmd args\n");
			goto out;
		}

		buf += ret;
		remain -= ret;
	}

	dispatch_cell_cmd(fd, &cmd_args);
out:
	close(fd);
	return NULL;
}

static int is_running(int pid, const char *name)
{
	int fd;
	int ret;
	char buf[32];
	char fbuf[PATH_MAX];
	int  statpid;
	char pidname[16];
	char pidstate = '\0';

	if (!vdir_exists("/proc/%d", pid))
		return 0;

	snprintf(buf, sizeof(buf), "/proc/%d/stat", pid);
	if (!file_exists(buf))
		return 0;

	fd = open(buf, O_RDONLY);
	if (fd == -1)
		return 0;

	ret = read(fd, fbuf, sizeof(fbuf));
	close(fd);

	if (ret == -1) {
		ALOGE("Could not read %s: %s", buf, strerror(errno));
		return 0;
	}

	memset(pidname, 0, sizeof(pidname));
	sscanf(fbuf, "%d %17s %c", &statpid, pidname, &pidstate);
	ALOGD("RUNING PID %d %s state:%c", statpid, pidname, pidstate);

	/* zombies don't count */
	if (pidstate == 'Z')
		return 0;

	snprintf(buf, sizeof(buf), "/proc/%d/cmdline", pid);
	fd = open(buf, O_RDONLY);

	ret = read(fd, fbuf, sizeof(fbuf));
	close(fd);

	if (ret < 0 || !*fbuf)
		return 0;

	if (strcmp(fbuf, name) != 0)
		return 0;

	return 1;
}

/* TODO: set active_cell based on active_ns_pid */
static int try_reattach(void)
{
	int ret, i;
	struct config_info config;
	struct pty_info console_pty;
	struct cell_node *new;
	char **name_list;
	name_list = get_cell_names();
	if (name_list == NULL)
		return -1;

	/* Set console as unavailable */
	console_pty.ptm = -1;

	for (i = 0; name_list[i] != NULL; ++i) {
		if (read_config(name_list[i], &config) == -1)
			continue;
		if (config.initpid == -1)
			continue;

		ALOGI("Trying to re-attach to cell '%s' (init=%d)",
			 name_list[i], config.initpid);
		ret = is_running(config.initpid, "/init");
		if (!ret) {
			/* Fix the config to indicate cell has stopped */
			config.initpid = -1;
			write_config(name_list[i], &config);
			ALOGI("cell '%s' doesn't appear to be running",
				  name_list[i]);
			continue;
		} else if (ret == -1) {
			free_cell_names(name_list);
			return -1;
		}

		new = create_cell_node(name_list[i], &config, &console_pty);
		if (new == NULL) {
			free_cell_names(name_list);
			return -1;
		}
		add_cell_node(new);
		new->starting = 0;
		new->non_child = 1;
		ALOGI("Re-attached to cell '%s' (init_pid = %d)",
			 name_list[i], config.initpid);
	}
	free_cell_names(name_list);
	return 0;
}

static struct cell_node *celld_auto_start_cell(char *name)
{
	int fd;
	struct cell_node *ret = NULL;
	struct cell_start_args args;

	fd = open("/dev/null", O_WRONLY);
	if (fd == -1) {
		ALOGE("Could not open /dev/null");
		return NULL;
	}

	args.noopt = 1;
	pthread_mutex_lock(&config_lock);

	if (create_cell_dirs(name, 1) == -1)
		return ret;

	ret = __do_start(fd, name, &args);

	pthread_mutex_unlock(&config_lock);
	close(fd);
	return ret;
}

struct autostart_cell_info {
	char *name;
	int   attempts;
};

static struct autostart_cell_info *
alloc_autostart_cell_info(int namelen)
{
	struct autostart_cell_info *aci;

	aci = malloc(sizeof(*aci) + namelen);
	if (!aci)
		return NULL;

	aci->name = (char *)aci + sizeof(*aci);
	aci->attempts = 0;
	return aci;
}

static void free_autostart_cell_info(struct autostart_cell_info *aci)
{
	free(aci);
}

/* Search autostarted array */
static struct autostart_cell_info *search_autostarted(char *name)
{
	int i;
	for (i = 0; i < arraySize(autostarted_array); ++i) {
		struct autostart_cell_info *aci = arrayGet(autostarted_array, i);
		if (!aci)
			continue;
		char *cur = aci->name;
		if (strcmp(cur, name) == 0)
			return aci;
	}
	return NULL;
}

static int __autostart_cells(void)
{
	int i, idx;
	struct config_info config;
	struct cell_node *cell;
	char **name_list;
	struct autostart_cell_info *aci;

	name_list = get_cell_names();
	if (name_list == NULL)
		return -1;

	for (i = 0; name_list[i] != NULL; ++i) {
		if (read_config(name_list[i], &config) == -1)
			continue;
		if (config.autostart == 0)
			continue;
		/* Don't start cells that were re-attached already */
		if (search_cells(name_list[i]) != NULL)
			continue;
		/* Don't start cells we've already started */
		aci = search_autostarted(name_list[i]);
		if (aci && aci->attempts >= MAX_CELL_AUTOSTART_ATTEMPTS)
			continue;
		/* Add cell to list of already autostarted cells */
		if (!aci) {
			aci = alloc_autostart_cell_info(strlen(name_list[i]+1));
			if (aci == NULL) {
				ALOGE("No memory for cell autostart info!");
				continue;
			}
			strcpy(aci->name, name_list[i]);
			idx = arrayAdd(autostarted_array, (void *)aci);
			if (idx == -1) {
				free_autostart_cell_info(aci);
				continue;
			}
		}

		/* Start cell */
		ALOGI("Auto starting %s", name_list[i]);
		pthread_mutex_lock(&g_cell_list.mutex);
		aci->attempts += 1;
		cell = celld_auto_start_cell(name_list[i]);
		if (cell == NULL) {
			ALOGE("failed to start %s (attempt=%d)",
				 name_list[i], aci->attempts);
			pthread_mutex_unlock(&g_cell_list.mutex);
			continue;
		}
		pthread_mutex_unlock(&g_cell_list.mutex);

		sleep(5);
	}
	free_cell_names(name_list);
	return 0;
}

static void *autostart_runner(void* arg)
{
	arg;

	__autostart_cells();

	while (1) {
		sleep(60*60*24);
	}

	return (void *)NULL;
}

static void autostart_cells(void)
{
	pthread_t tid;
	autostarted_array = arrayCreate();
	pthread_create(&tid, NULL, autostart_runner, (void *)NULL);
}

static void log_start(void)
{
	struct timeval tv;
	char tm_buf[128];
	tm_buf[0] = 0; /* just in case... */
	gettimeofday(&tv, NULL);
	ctime_r(&tv.tv_sec, tm_buf);
	ALOGI("--- celld started on %s", tm_buf);
}

static void cell_cleanup_and_free(struct cell_node *cell, const char *root_path,
				  struct cell_list *list)
{
	struct config_info config;

	cleanup_cell_console(&cell->console_pty);
	if (unmount_all(root_path, 1) == -1)
		ALOGW("Couldn't unmount_all() on %s", root_path);

	/* Try our best to update the config file. Not fatal if we fail. */
	if (read_config(cell->name, &config) == 0) {
		config.initpid = -1;
		write_config(cell->name, &config);
	}

	__del_cell_from(cell, list);
	free(cell);
}

static void *cell_exit_handler(void *unused)
{
	unused;

	int pid;
	int status, exit_status, exit_signal;
	struct cell_node *cell;
	struct cell_list *list;
	char *root_path;

	ALOGI("Handling SIGCHLD");
	pthread_mutex_lock(&sigchld_lock);

	while (1) {
		pid = waitpid(-1, &status, WNOHANG); /* -1 == WAIT_ANY */
		if (pid == 0 || pid < 0) {
			ALOGD("%s: waitpid out pid: %d, %s", __func__,
				 pid, strerror(errno));
			break;
		}
		exit_status = WEXITSTATUS(status);
		exit_signal = WTERMSIG(status);
		ALOGD("%d exit %s (sig=%d)",
			 pid, (exit_status ? "ERROR" : "SUCCESS"), exit_signal);
		list = &g_dead_cell_list;
		cell = search_cells_pid(pid, list);
		if (!cell) {
			ALOGE("pid %d not in deleted cell list...", pid);
			list = &g_cell_list;
			cell = search_cells_pid(pid, list);
			if (!cell) {
				ALOGE("Couldn't find cell for pid %d", pid);
				break;
			}
		}
		ALOGI("Cell %s terminated by sig %d (%s)", cell->name, status,
			 (exit_status ? "ERROR" : "SUCCESS"));

		root_path = get_root_path(cell->name);
		if (!root_path) {
			ALOGE("%s out of memory for cell rootdir", __func__);
			break;
		}

		/* do the final cleanup and free the cell_node struct */
		cell_cleanup_and_free(cell, root_path, list);
		free(root_path);
		break;
	}

	pthread_mutex_unlock(&sigchld_lock);

	return NULL;
}

/* We need this extra level of indirection because cell_exit_handler grabs
 * locks that we are not supposed to grab in a signal handler */
static void sigchld_handler(int sig)
{
	sig;

	pthread_t tid;
	pthread_create(&tid, NULL, cell_exit_handler, NULL);
}

static void celld_main(void)
{
	/* TODO: Create a privileged socket for registering/unregistering.
	 * Create 2 threads here. one for priv, one for regular socket listen */
	int sd, fd;
	struct sockaddr_un addr;
	socklen_t addr_len;
	pthread_t tid;

	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sd == -1) {
		ALOGE("socket() failed: %s", strerror(errno));
		return;
	}

	fcntl(sd, F_SETFD, FD_CLOEXEC);

	addr_len = init_addr(&addr);

	int ov = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(ov)) == -1) {
		ALOGE("Failed to set socket options: %s", strerror(errno));
		goto err;
	}

	if (bind(sd, (struct sockaddr *)&addr, addr_len) == -1) {
		ALOGE("Failed to bind to SOCKET_PATH: %s", strerror(errno));
		goto err;
	}

	if (listen(sd, 10) == -1) {
		ALOGE("listen() failed: %s", strerror(errno));
		goto err;
	}

	while (1) {
		fd = accept(sd, (struct sockaddr *)&addr, &addr_len);
		if (fd != -1) {
			pthread_create(&tid, NULL, handle_conn, (void *)((long)fd));
		} else if (errno != EINTR) {
			break;
		}
	}

	ALOGE("cannot accept more incoming connections on socket?");
err:
	close(sd);
	return;
}

static int init_celld(const char *procname)
{
	struct stat st;
	int fd = -1, lockpid = -1;
	char lockval[64];

	/* ignore errors */
	mkdir(g_cell_dir, 0750);

	/* look for the celld lockfile */
	if (stat(CELLD_LOCKFILE, &st) < 0)
		goto get_lock;

	if (!S_ISREG(st.st_mode)) { /* eh? this would be weird... */
		unlink(CELLD_LOCKFILE);
		goto get_lock;
	}

check_lock:
	fd = open(CELLD_LOCKFILE, O_RDONLY);
	if (fd < 0) {
		if (errno == EEXIST) {
			unlink(CELLD_LOCKFILE);
			goto get_lock;
		}
		goto out_err;
	}

	if (read(fd, lockval, sizeof(lockval)) < 0)
		goto out_err;

	lockpid = atoi(lockval);
	/*
	* If some other process claims to hold the lock, check if that
	* process is in fact around, if not, just grab the lock.  If our pid
	* is in here it means celld has been restarted with the same pid,
	* which means that either there was a pid rollover (unlikely) or we
	* rebooted and started with the same pid.  In either case, nobody
	* else can be running celld and we are free to grab the lock.
	*/
	if (lockpid != getpid()) {
		if (is_running(lockpid, procname))
			goto out_err;
		/* stale lock */
		ALOGI("removing stale celld lockfile");
		unlink(CELLD_LOCKFILE);
		goto get_lock;
	}

	/* we have the lock */
	close(fd);
	return 0;

get_lock:
	fd = open(CELLD_LOCKFILE, O_RDWR | O_TRUNC | O_CREAT, 0600);
	if (fd < 0)
		goto out_err;

	/*
	 * write our pid into the file, then check to
	 * make sure we didn't race with another instance
	 */
	snprintf(lockval, sizeof(lockval), "%d", getpid());
	if (write(fd, lockval, strlen(lockval)+1) < 0)
		goto out_err;
	close(fd);

	goto check_lock;

out_err:
	if (fd >= 0)
		close(fd);
	ALOGE("Could not obtain celld lockfile");
	return -1;
}

static void print_usage(const char *name)
{
	static const char *usage = "\
	-a			 Automatically start cells with autostart enabled\n\
	-c <dir>	   Set Cells directory to <dir> \n\
						   (default: "DEFL_CELL_DIR")\n\
	-s <dir>	   Set cell SDCARD directory to <dir> \n\
						   (default: "DEFL_SDCARD_ROOT")\n\
	-F			 Run in foreground. don't daemonize.\n\
	-M <path>	  Load the system module at <path> before continuing.\n\
						   pass multiple -M to load multiple modules.\n\
	-r			 celld will attempt to \"re-attach\" to previously\n\
				   running cells (eg. after celld has restarted)\n\
	-p			 Collects power info while running. Info saved\n\
				   to /data/power_info. 10 second polling.\n\
	-P			 Only collects power_info. The rest of celld\n\
				   does not run. Use for stock images\n\
	-h			 displays this message\n";

	printf("Usage: %s [options]\n", name);
	printf("%s", usage);
}

static void init_cells_config(void)
{
	int i = 0;
	char cellname[64] = {0};

	property_set("persist.sys.exit", "0");
	property_set("persist.sys.vm.name", "");
	property_set("persist.sys.active", "");

	for( i = 1 ; i <= MAX_CELL_NUM ; i++)
	{
		memset(cellname, 0, 64);
		sprintf(cellname, "cell%d", i);
		set_properties_cells(cellname, "0");
		init_cellvm_config(cellname);
	}
}

int main(int argc, char **argv)
{
	int c;
	int daemon = 1;
	int reattach_cells = 0;
	int autostart = 0;
	int ret;
	struct sigaction sigact;

	/* reset our umask */
	umask(0000);

	while ((c = getopt(argc, argv, "ac:s:FM:rh")) != -1) {
		switch (c) {
		case 'a':
			autostart = 1;
			break;
		case 'c':
			g_cell_dir = optarg;
			ALOGI("Set root Cell dir to %s", g_cell_dir);
			break;
		case 's':
			g_sdcard_root = optarg;
			ALOGI("Set cell sdcard root dir to %s", g_sdcard_root);
			break;
		case 'F':
			ALOGI("Running in foreground...");
			daemon = 0;
			break;
		case 'M':
			ALOGI("Loading '%s'...", optarg);
			if (insert_module(optarg, NULL) == 0)
				ALOGI("Module '%s' loaded.", optarg);
			break;
		case 'r':
			reattach_cells = 1;
			break;
		case 'h':
			print_usage(argv[0]);
			exit(0);
		default:
			ALOGE("Invalid start options");
			print_usage(argv[0]);
			exit(1);
		}
	}

	if (init_celld(argv[0]) < 0) {
		ALOGI("Could not initialize celld. Exiting.");
		fprintf(stderr, "Could not initialize celld. Exiting.\n");
		exit(1);
	}

	if (daemon)
		daemonize();

	log_start();

	/*
	 * Initialize Radio Interface Layer (RIL)
	 *	 (*) possibly load custom library
	 *	 (*) setup proxy host
	 *	 (*) pass
	 *	 (*) custom arguments to rild
	 */

	/* Setup SIGCHLD handler and ignore SIGPIPEs */
	signal(SIGPIPE, SIG_IGN);
	memset(&sigact, 0, sizeof(sigact));
	sigact.sa_handler = sigchld_handler;
	sigact.sa_flags = SA_NOCLDSTOP;
	ret = sigaction(SIGCHLD, &sigact, NULL);
	if (ret < 0) {
		ALOGE("SIGCHLD sigaction failed\n");
		return EXIT_FAILURE;
	}

	init_cells_config();

	if (reattach_cells)
		try_reattach();

	if (autostart)
		autostart_cells();

	/*
	 * open the main CellD socket and listen for incoming connections
	 */
	celld_main();

	/* Only on error do we get here */
	return EXIT_FAILURE;
}
