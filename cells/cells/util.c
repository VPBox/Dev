/*
 * util.c
 *
 * Utilities used by cell/celld
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
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <string.h>

#define LOG_TAG "Cells/util"
#include <cutils/log.h>

#include "celld.h"

int file_exists(const char *path)
{
	struct stat buf;
	if (stat(path, &buf) < 0)
		return 0;
	return S_ISREG(buf.st_mode);
}

int dir_exists(const char *path)
{
	struct stat buf;
	if (stat(path, &buf) < 0)
		return 0;
	return S_ISDIR(buf.st_mode);
}

/* Returns 1 if dir exists, 0 if not. Note: I'm never going to check for the
 * -1, which is why the size is MAX_MSG_LEN+128. It's easy to tell when
 *  that amount will be exceeded. */
int vdir_exists(const char *fmt, ...)
{
	char path[MAX_MSG_LEN+128];
	va_list ap;
	int ret;
	va_start(ap, fmt);
	ret = vsnprintf(path, MAX_MSG_LEN+128, fmt, ap);
	if (ret >= MAX_MSG_LEN+128)
		return -1;
	ret = dir_exists(path);
	va_end(ap);
	return ret;
}

static int is_symlink(const char *path)
{
	struct stat buf;
	if (lstat(path, &buf) == -1)
		return -1;
	return S_ISLNK(buf.st_mode);
}

/* In case you're wondering why I'm doing this at such a low level, it's
 * because bionic doesn't have ftw()... */
/* This will survive recursive links, but it will not detect them */
static int rmtree_real(const char *path, int max_depth)
{
	int ret;
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	char abs_name[FILENAME_MAX];
	/* Bail if we're going too deep */
	if (max_depth == 0) {
		ALOGE("rmtree: reached max recursion depth");
		return -1;
	}

	/* If it's a symlink, remove it now to be safe. We don't want to
	   follow one outside of a cell's directory */
	ret = is_symlink(path);
	if (ret == 1) {
		unlink(path);
		return 0;
	} else if (ret == -1) {
		ALOGE("rmtree: lstat %s: %s", path, strerror(errno));
		return -1;
	}

	/* Try to treat path as a dir. Either opens it or tells us it's not */
	dir = opendir(path);
	if (dir == NULL && errno == ENOTDIR) {
		if (unlink(path) == -1) {
			ALOGE("rmtree: unlink %s: %s", path, strerror(errno));
			return -1;
		}
		return 0;
	} else if (dir == NULL) {
		ALOGE("rmtree: opendir %s: %s", path, strerror(errno));
		return -1;
	}
	/* path is a directory. Iterate through its files */
	for (ret = readdir_r(dir, &entry, &result);
		 ret == 0 && result != NULL;
		 ret = readdir_r(dir, &entry, &result))
	{
		if (!strcmp(entry.d_name, ".") || !strcmp(entry.d_name, ".."))
			continue;
		snprintf(abs_name, FILENAME_MAX, "%s/%s", path, entry.d_name);
		if (rmtree_real(abs_name, max_depth-1) == -1) {
			closedir(dir);
			return -1;
		}
	}
	if (ret != 0) {
		closedir(dir);
		ALOGE("rmtree: readdir_r %s: %s", path, strerror(errno));
		return -1;
	}
	closedir(dir);

	if (rmdir(path) == -1) {
		ALOGE("rmtree: rmdir %s: %s", path, strerror(errno));
		return -1;
	}
	return 0;
}

int rmtree(const char *path)
{
	/* there's nothing special about 128...it's just what I picked */
	return rmtree_real(path, 128);
}

int walkdir_through(void *ctx, const char *base, const char *sdir, int depth,const char *through,
		void (*callback)(void *ctx, const char *path, const char *subpath, struct dirent *e))
{
	int ret;
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	char full_name[FILENAME_MAX];
	char path[FILENAME_MAX];
	char subpath[FILENAME_MAX];

	if (!callback)
		return -1;

	snprintf(path, FILENAME_MAX, "%s%s", base, sdir);
	dir = opendir(path);
	if (!dir)
		return -1;

	for (ret = readdir_r(dir, &entry, &result);
		 ret == 0 && result != NULL;
		 ret = readdir_r(dir, &entry, &result))
	{
		if (!strcmp(entry.d_name, ".") || !strcmp(entry.d_name, ".."))
			continue;

		snprintf(full_name, FILENAME_MAX, "%s/%s", path, entry.d_name);	
		if(strstr(through, full_name) == NULL)
			continue;

		snprintf(subpath, FILENAME_MAX, "%s/%s", sdir, entry.d_name);
		callback(ctx, full_name, subpath, &entry);

		if (depth && entry.d_type == DT_DIR) {
			ret = walkdir_through(ctx, base, subpath, depth-1, through, callback);
			if (ret)
				break;
		}
	}

	closedir(dir);

	return ret;
}

int walkdir(void *ctx, const char *base, const char *sdir, int depth,
		void (*callback)(void *ctx, const char *path, const char *subpath, struct dirent *e))
{
	int ret;
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	char full_name[FILENAME_MAX];
	char path[FILENAME_MAX];
	char subpath[FILENAME_MAX];

	if (!callback)
		return -1;

	snprintf(path, FILENAME_MAX, "%s%s", base, sdir);
	dir = opendir(path);
	if (!dir)
		return -1;

	for (ret = readdir_r(dir, &entry, &result);
		 ret == 0 && result != NULL;
		 ret = readdir_r(dir, &entry, &result))
	{
		if (!strcmp(entry.d_name, ".") || !strcmp(entry.d_name, ".."))
			continue;

		snprintf(full_name, FILENAME_MAX, "%s/%s", path, entry.d_name);		
		snprintf(subpath, FILENAME_MAX, "%s/%s", sdir, entry.d_name);
		callback(ctx, full_name, subpath, &entry);
		if (depth && entry.d_type == DT_DIR) {
			ret = walkdir(ctx, base, subpath, depth-1, callback);
			if (ret)
				break;
		}
	}

	closedir(dir);

	return ret;
}

#define COPY_BUF_SZ 8192
int copy_file(const char *src, const char *dst)
{
	struct stat src_stat;
	struct stat dst_stat;
	int src_fd = 0, dst_fd = 0;
	int ret = 0;
	mode_t new_mode;

	char *buf;

	if (stat(src, &src_stat) < 0)
		return -1;

	if (S_ISLNK(src_stat.st_mode)) {
		ALOGE("not copying symlink:'%s'", src);
		return -1;
	}

	if (lstat(dst, &dst_stat) == 0) {
		if (src_stat.st_dev == dst_stat.st_dev &&
			src_stat.st_ino == dst_stat.st_ino) {
			ALOGE("'%s' and '%s' are the same file!", src, dst);
			return -1;
		}
	}

	/* get a buffer to work with */
	buf = malloc(COPY_BUF_SZ);
	if (!buf) {
		errno = ENOMEM;
		return -1;
	}

	src_fd = open(src, O_RDONLY);
	if (src_fd < 0) {
		ALOGE("error %d opening src:'%s'", errno, src);
		ret = -1;
		goto out;
	}

	new_mode = src_stat.st_mode;
	if (!S_ISREG(src_stat.st_mode))
		new_mode = 0664;

	dst_fd = open(dst, O_WRONLY|O_CREAT|O_EXCL, new_mode);
	if (dst_fd < 0) {
		/* ALOGE("error %d opening dst:'%s'", errno, dst); */
		ret = -1;
		goto out;
	}

	/* copy the contents */
	while (1) {
		ssize_t rbytes;
		ssize_t wbytes;
		rbytes = read(src_fd, buf, COPY_BUF_SZ);

		ret = (rbytes < 0 ? -1 : 0);
		if (rbytes <= 0)
			break;

		wbytes = 0;
		do {
			int b = write(dst_fd, buf + wbytes, rbytes - wbytes);
			if (b < 0) {
				ret = -1;
				goto out;
			}
			wbytes += b;
		} while (wbytes != rbytes);
	}

out:
	if (dst_fd > 0 && close(dst_fd) < 0) {
		ALOGE("error writing to '%s'", dst);
		ret = -1;
	}

	if (src_fd > 0)
		close(src_fd);

	free(buf);
	return ret;
}

void close_fds(void)
{
	/* Close all file descriptors */
	int i;

	for (i = sysconf(_SC_OPEN_MAX); i >= 0; --i)
		close(i);
}

void daemonize(void)
{
	pid_t pid, sid;

	pid = fork();
	if (pid < 0) {
		ALOGE("daemonize failed to fork.");
		exit(1);
	} else if (pid > 0) {
		ALOGI("Sacrificial celld child: exiting");
		exit(0);
	}

	/* Obtain a new process group */
	sid = setsid();
	if (sid < 0) {
		fprintf(stderr, "Failed to set sid\n");
		exit(1);
	}
	fprintf(stderr, "Closing all open FDs...\n");
	close_fds();
}

int is_mounted(const char *path)
{
	int device_len = 256, mount_len = 256, rest_len = 256;
	int line_len = 1024;
	char *parse_str = NULL;
	char *device, *mount_path, *rest, *line;
	FILE *fp;

	int found = 0;

	if (!(fp = fopen("/proc/mounts", "r"))) {
		ALOGE("Error opening /proc/mounts (%s)", strerror(errno));
		return 0;
	}

	/* allocate space for the parsed line */
	parse_str = malloc(device_len + mount_len + rest_len + line_len + 4);
	if (!parse_str) {
		ALOGE("out of memory for unmount parsing string!");
		goto out;
	}

	line = parse_str;
	device = parse_str + line_len + 1;
	mount_path = device + device_len + 1;
	rest = mount_path + mount_len + 1;

	while(fgets(line, line_len, fp)) {
		line[strnlen(line, line_len)-1] = '\0';
		memset(mount_path, 0, mount_len);
		sscanf(line, "%256s %256s %256s\n", device, mount_path, rest);
		if (strcmp(mount_path, path) == 0) {
			found = 1;
			break;
		}
	}

out:
	fclose(fp);
	free(parse_str);
	return found;
}


/* recursively unmount all directories underneath root_path/dir */
int __unmount_dir(const char *root_path, char *dir)
{
	/* AVOID nearly 2k of stack usage... */
	int device_len = 256, mount_len = 256, rest_len = 256;
	int line_len = 1024;
	char *parse_str = NULL;
	char *extra_path = NULL;
	int extra_path_len = 0;
	int extra_path_mounted = 0;
	char *device, *mount_path, *rest, *line;

	FILE *fp;
	int err, nr = 0;

	ALOGV("unmounting %s/%s (and all sub-mounts)", root_path, dir);

	if (!(fp = fopen("/proc/mounts", "r"))) {
		ALOGE("Error opening /proc/mounts (%s)", strerror(errno));
		return -1;
	}

	extra_path_len = strlen(root_path) + strlen(dir) + 2; /* for '/\0' */
	extra_path = malloc(extra_path_len);
	if (!extra_path) {
		ALOGE("out of memory for unmount pathname!");
		err = -1;
		goto out;
	}
	sprintf(extra_path, "%s/%s", root_path, dir);

	/* allocate space for the parsed line */
	parse_str = malloc(device_len + mount_len + rest_len + line_len + 4);
	if (!parse_str) {
		ALOGE("out of memory for unmount parsing string!");
		err = -1;
		goto out;
	}
	line = parse_str;
	device = parse_str + line_len + 1;
	mount_path = device + device_len + 1;
	rest = mount_path + mount_len + 1;

	err = 0;
	while(fgets(line, line_len, fp)) {
		line[strnlen(line, line_len)-1] = '\0';
		memset(mount_path, 0, mount_len);
		sscanf(line, "%256s %256s %256s\n", device, mount_path, rest);
		if (strcmp(mount_path, extra_path) == 0) {
			extra_path_mounted = 1;
			continue; /* save this for last */
		}
		if (strncmp(mount_path, extra_path, extra_path_len-1) == 0) {
			if (umount(mount_path) < 0) {
				ALOGE("Error unmounting %s: %s",
					  mount_path, strerror(errno));
				err -= 1;
			} else
				++nr;
		}
	}

	if (extra_path_mounted && umount(extra_path) < 0) {
		ALOGE("Error unmounting %s: %s", extra_path, strerror(errno));
		err -= 1;
	} else
		nr += extra_path_mounted;

	ALOGV("unmounted %d directories", nr);
out:
	fclose(fp);
	free(extra_path);
	free(parse_str);
	return err;
}

static int init_module(const char *mod_image, unsigned long image_len,
			   const char *mod_params)
{
	return syscall(__NR_init_module, mod_image, image_len, mod_params);
}

static int finit_module(const char *name, int flags)
{
	return syscall(__NR_delete_module, name, flags);
}

int insert_module(const char *path, const char *params)
{
	struct stat image_stat;
	int image_fd = -1;
	unsigned long image_size = 0;
	void *image = NULL;
	const char *elfhdr = NULL;
	int rc = 0;

	image_fd = open(path, O_RDONLY);
	if (image_fd < 0) {
		ALOGE("Error %d opening '%s'", errno, path);
		return -1;
	}

	if (fstat(image_fd, &image_stat) < 0 || !S_ISREG(image_stat.st_mode))
		goto out_err;

	if (image_stat.st_size > (off_t)(INT_MAX - 4096)) {
		ALOGE("File '%s' too large(%ld)", path, image_stat.st_size);
		goto out_err;
	}
	image_size = (unsigned long)image_stat.st_size;

	image = mmap(NULL, image_size, PROT_READ, MAP_PRIVATE, image_fd, 0);
	if (image == MAP_FAILED)
		goto out_err;

	close(image_fd);
	image_fd = -1;

	elfhdr = (const char *)image;
	if ( !(elfhdr[0] == 0x7F &&
		   elfhdr[1] == 'E' && elfhdr[2] == 'L' && elfhdr[3] == 'F')) {
		ALOGE("Invalid ELF signature in '%s' - compressed?", path);
		goto out_err;
	}

	errno = 0;
	rc = init_module(image, image_size, params ? params : "");
	if (rc < 0)
		goto out_err;
	rc = -errno;
	munmap(image, image_size);
	return rc;

out_err:
	if (image && image_size)
		munmap(image, image_size);
	if (image_fd >= 0)
		close(image_fd);
	if (errno == EEXIST)
		ALOGI("Module '%s' already loaded.", path);
	else
		ALOGE("Error %d inserting module '%s'", errno, path);
	return -1;
}
