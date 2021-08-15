/*
 * nsexec.c
 * Based on nsexec.c which is Copyright (C) 2008,2009 IBM Corp.
 *
 * routines for cloning a cell in a new set of namespaces
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
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <linux/sched.h>

#define LOG_TAG "Cells/nsexec"
#include <cutils/log.h>
#include <cutils/memory.h>
#include <cutils/misc.h>
#include <selinux/selinux.h>
#include "cutils/properties.h"

#include "celld.h"
#include "cell_console.h"
#include "util.h"
#include "network.h"
#include "cellnet.h"

#ifndef MNT_DETACH
#define MNT_DETACH 2
#endif

/* Linux clone flags not available in bionic's kernel headers */
#ifndef CLONE_NEWUTS
#define CLONE_NEWUTS			0x04000000	  /* New utsname group? */
#endif
#ifndef CLONE_NEWIPC
#define CLONE_NEWIPC			0x08000000	  /* New ipcs */
#endif
#ifndef CLONE_NEWUSER
#define CLONE_NEWUSER		   0x10000000	  /* New user namespace */
#endif
#ifndef CLONE_NEWPID
#define CLONE_NEWPID			0x20000000	  /* New pid namespace */
#endif
#ifndef CLONE_NEWNET
#define CLONE_NEWNET			0x40000000	  /* New network namespace */
#endif
#ifndef CLONE_IO
#define CLONE_IO				0x80000000	  /* Clone io context */
#endif

int g_cgroup_pipe[2];

extern int clone(int (*fn)(void *), void *child_stack, int flags, void *arg, ...);

static int load_cgroup_dir(char *dest, int len, const char* keyword)
{
	FILE *f = fopen("/proc/mounts", "r");
	char buf[200];
	char *name, *path, *fsname, *options, *p1, *p2, *s;
	if (!f)
		return 0;
	while (fgets(buf, sizeof(buf), f)) {
		name = strtok_r(buf, " ", &p1);
		path = strtok_r(NULL, " ", &p1);
		fsname = strtok_r(NULL, " ", &p1);
		options = strtok_r(NULL, " ", &p1);
		if (strcmp(fsname, "cgroup") != 0 || strstr(path, keyword) == NULL)
			continue;
		strncpy(dest, path, len);
		fclose(f);
		return 1;
	}
	fclose(f);
	return 0;
}

static char *get_cgroup_folder(char *cellname, const char* keyword)
{
	char cgroupbase[100];
	char *folder;
	int ret;

	if (!load_cgroup_dir(cgroupbase, sizeof(cgroupbase), keyword))
		return NULL;

	folder = malloc(MAX_PATH_LEN);
	if (!folder)
		return NULL;

	ret = snprintf(folder, MAX_PATH_LEN, "%s/%s", cgroupbase, cellname);
	if (ret >= MAX_PATH_LEN) {
		free(folder);
		return NULL;
	}
	return folder;
}

static char *create_cgroup(char *cellname, const char* keyword)
{
	int ret;
	char *cgroupname;

	cgroupname = get_cgroup_folder(cellname, keyword);
	if (!cgroupname)
		return NULL;

	ret = mkdir(cgroupname, 0755);
	if (ret && errno != EEXIST)
		return NULL;
	return cgroupname;
}

static int move_acct_to_new_cgroup(struct cell_args *args)
{
	char tasksfname[200];
	FILE *fout;
	char *cgroupname;
	int ret;

	ret = -1;

	cgroupname = create_cgroup(args->cellname, "acct");
	if (!cgroupname)
		goto out;

	{
		snprintf(tasksfname, sizeof(tasksfname), "%s/tasks", cgroupname);
		fout = fopen(tasksfname, "w");
		if (!fout)
			goto out;
		fprintf(fout, "%d\n", args->init_pid);
		fclose(fout);
	}

	ALOGI("Moved %s (%d) into new cgroup (%s)",
		 args->cellname, args->init_pid, cgroupname);
	ret = 0;
out:
	free(cgroupname);
	return ret;
}

#define MEMORY_CGROUP "1536M"
static int move_memory_to_new_cgroup(struct cell_args *args)
{
	char tasksfname[200];
	FILE *fout;
	char *cgroupname;
	int ret;

	ret = -1;

	cgroupname = create_cgroup(args->cellname, "memcg");
	if (!cgroupname)
		goto out;

	{
		snprintf(tasksfname, sizeof(tasksfname), "%s/memory.limit_in_bytes", cgroupname);
		fout = fopen(tasksfname, "w");
		if (!fout)
			goto out;
		fprintf(fout, MEMORY_CGROUP);
		fclose(fout);
	}

	{
		snprintf(tasksfname, sizeof(tasksfname), "%s/tasks", cgroupname);
		fout = fopen(tasksfname, "w");
		if (!fout)
			goto out;
		fprintf(fout, "%d\n", args->init_pid);
		fclose(fout);
	}

	ALOGI("Moved %s (%d) into new cgroup (%s)",
		 args->cellname, args->init_pid, cgroupname);
	ret = 0;
out:
	free(cgroupname);
	return ret;
}

static int do_newcgroup(struct cell_args *args)
{
	if (!args->start_args.newcgrp)
		return 0;

	move_acct_to_new_cgroup(args);

	move_memory_to_new_cgroup(args);

	return 0;
}

static int rootfs_chroot_root(const char *rootfs)
{
	int i, ret;

	ret = chdir("/");
	if (ret < 0){
		ALOGD("Failed chdir /.");
		return -1;
	}

	/* We could use here MS_MOVE, but in userns this mount is locked and
	 * can't be moved.
	 */
	ret = mount(rootfs, "/", NULL, MS_REC | MS_BIND, NULL);
	if (ret < 0){
		ALOGD("Failed to mount %s onto / as MS_REC | MS_BIND.", rootfs);
		return -1;
	}

	ret = mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
	if (ret < 0){
		ALOGD("Failed to remount / .");
		return -1;
	}

	/* The following code cleans up inherited mounts which are not required
	 * for CT.
	 *
	 * The mountinfo file shows not all mounts, if a few points have been
	 * unmounted between read operations from the mountinfo. So we need to
	 * read mountinfo a few times.
	 *
	 * This loop can be skipped if a container uses userns, because all
	 * inherited mounts are locked and we should live with all this trash.
	 */
	for (;;) {
		FILE *f = NULL;
		char *line = NULL;
		char *slider1, *slider2;
		int progress = 0;
		size_t len = 0;

		f = fopen("./proc/self/mountinfo", "re");
		if (!f){
			ALOGD("Failed to open /proc/self/mountinfo .");
			return -1;
		}

		while (getline(&line, &len, f) > 0) {
			for (slider1 = line, i = 0; slider1 && i < 4; i++)
				slider1 = strchr(slider1 + 1, ' ');

			ALOGD("rootfs_chroot_root line %s", line);
			if (!slider1)
				continue;

			slider2 = strchr(slider1 + 1, ' ');
			if (!slider2)
				continue;

			*slider2 = '\0';
			*slider1 = '.';

			if (strcmp(slider1 + 1, "/") == 0)
				continue;

			if (strcmp(slider1 + 1, "/proc") == 0)
				continue;

			ALOGD("rootfs_chroot_root umount2 %s", slider1);
			ret = umount2(slider1, MNT_DETACH);
			if (ret == 0)
				progress++;
		}

		if (!progress){
			fclose(f);
			break;
		}
	}

	/* This also can be skipped if a container uses userns. */
	(void)umount2("./proc", MNT_DETACH);

	/* It is weird, but chdir("..") moves us in a new root */
	ret = chdir("..");
	if (ret < 0){
		ALOGD("Failed to chdir(..).");
		return -1;
	}

	ret = chroot(".");
	if (ret < 0){
		ALOGD("Failed to chdir(.).");
		return -1;
	}

	return 0;
}

static int
pivot_root(const char *new_root, const char *put_old)
{
	return syscall(__NR_pivot_root, new_root, put_old);
}

static int rootfs_pivot_root(const char *rootfs)
{
	int oldroot = -EBADF, newroot = -EBADF;
	int ret;

	oldroot = open("/", O_DIRECTORY | O_RDONLY | O_CLOEXEC);
	if (oldroot < 0){
		ALOGD("Failed to open old root directory.");
		return -1;
	}

	newroot = open(rootfs, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
	if (newroot < 0){
		ALOGD("Failed to open new root directory.");
		close(oldroot);
		return -1;
	}

	/* change into new root fs */
	ret = fchdir(newroot);
	if (ret < 0){
		ALOGD("Failed to change to new rootfs %s .", rootfs);
		close(oldroot);
		close(newroot);
		return -1;
	}

	/* pivot_root into our new root fs */
	ret = pivot_root(".", ".");
	if (ret < 0){
		ALOGD("Failed to pivot_root().");
		close(oldroot);
		close(newroot);
		return -1;
	}

	/* At this point the old-root is mounted on top of our new-root. To
	 * unmounted it we must not be chdir'd into it, so escape back to
	 * old-root.
	 */
	ret = fchdir(oldroot);
	if (ret < 0){
		ALOGD("Failed to enter old root directory.");
		close(oldroot);
		close(newroot);
		return -1;
	}

	/* Make oldroot a depedent mount to make sure our umounts don't propagate to the
	 * host.
	 */
	ret = mount("", ".", "", MS_SLAVE | MS_REC, NULL);
	if (ret < 0){
		ALOGD("Failed to recursively turn old root mount tree into dependent mount.");
		close(oldroot);
		close(newroot);
		return -1;
	}

	ret = umount2(".", MNT_DETACH);
	if (ret < 0){
		ALOGD("Failed to detach old root directory.");
		close(oldroot);
		close(newroot);
		return -1;
	}

	ret = fchdir(newroot);
	if (ret < 0){
		ALOGD("Failed to re-enter new root directory.");
		close(oldroot);
		close(newroot);
		return -1;
	}

	close(oldroot);
	close(newroot);
	ALOGD("pivot_root(%s) successful", rootfs);
	return 0;
}

static int do_child(void *vargv)
{
	struct cell_args *cell_args = (struct cell_args *)vargv;
	struct cell_start_args *start_args = &cell_args->start_args;
	char **argv;
	char *rootdir;
	char *cellname;
	char *syserr;
	int ret;
	char buf[20];
	sigset_t sigset;

	argv = cell_args->argv;
	cellname = cell_args->cellname;
	rootdir = cell_args->rootdir;

	errno = 0;
	int nsfd = open("/proc/drv_ns/ns_tag", O_WRONLY);
	if (nsfd >= 0) {
		ret = write(nsfd, cellname, strlen(cellname));
		close(nsfd);
		ALOGD("/proc/drv_ns/ns_tag ret = %d errno = %s",ret,strerror(errno));
	}else{
		ALOGE("/proc/drv_ns/ns_tag nsfd = %d errno = %s",nsfd,strerror(errno));
	}
	errno = 0;

	ALOGD("Starting cell:");
	ALOGD("==============");
	ALOGD("start_args");
	ALOGD("----------");
	ALOGD("noopt: %d", start_args->noopt);
	ALOGD("uts_ns: %d", start_args->uts_ns);
	ALOGD("ipc_ns: %d", start_args->ipc_ns);
	ALOGD("user_ns: %d", start_args->user_ns);
	ALOGD("net_ns: %d", start_args->net_ns);
	ALOGD("pid_ns: %d", start_args->pid_ns);
	ALOGD("mount_ns: %d", start_args->mount_ns);
	ALOGD("mnt_rootfs: %d", start_args->mnt_rootfs);
	ALOGD("mnt_tmpfs: %d", start_args->mnt_tmpfs);
	ALOGD("newpts: %d", start_args->newpts);
	ALOGD("newcgrp: %d", start_args->newcgrp);
	ALOGD("share_dalvik_cache: %d", start_args->share_dalvik_cache);
	ALOGD("sdcard_branch: %d", start_args->sdcard_branch);
	ALOGD("open_console: %d", start_args->open_console);
	ALOGD("autoswitch: %d", start_args->autoswitch);
	ALOGD("pid_file: %s", start_args->pid_file);
	ALOGD("wait: %d", start_args->wait);
	ALOGD("\ncell_args");
	ALOGD("---------");
	ALOGD("cellname: %s", cell_args->cellname);
	ALOGD("rootdir: %s", cell_args->rootdir);
	ALOGD("init_pid: %d", cell_args->init_pid);
	ALOGD("restart_pid: %d", cell_args->restart_pid);
	ALOGD("argc: %d", cell_args->argc);
	ALOGD("cell_socket: %d", cell_args->cell_socket);

	/* reset out umask and sigmask for init */
	umask(0000);
	sigemptyset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, NULL);

	/* Make sure init doesn't kill CellD on bad cell errors */
	ret = setpgid(0, 0);
	if (ret < 0)
		ALOGE("error setting pgid: %s", strerror(errno));

	/* Close cell utility socket */
	close(cell_args->cell_socket);

	ALOGI("%s: do_child, mnt_rootfs:%d, rootdir=%s",
		  cellname, start_args->mnt_rootfs, rootdir);

	if(rootfs_chroot_root(rootdir) != 0){
		syserr = "chroot";
		goto out_err;
	}

	ALOGD("%s: waiting for cgroup entry...", cellname);
	close(g_cgroup_pipe[1]);
	ret = read(g_cgroup_pipe[0], buf, sizeof(buf));
	close(g_cgroup_pipe[0]);
	if (ret == -1 || atoi(buf) < 1) {
		syserr = "cgroup entry";
		goto out_err;
	}

	close(cell_args->child_pipe[0]);
	buf[0] = 1;
	write(cell_args->child_pipe[1], buf, 1);
	close(cell_args->child_pipe[1]);

	ALOGD("%s: waiting for CellD...", cellname);
	close(cell_args->init_pipe[1]);
	ret = read(cell_args->init_pipe[0], buf, sizeof(buf));
	close(cell_args->init_pipe[0]);
	if (ret == -1 || atoi(buf) < 1) {
		syserr = "CellD communication";
		goto out_err;
	}
	ALOGD("%s: Starting init!", cellname);

	int fdcell = open("/.cell",O_WRONLY|O_CREAT,0660);
	if(fdcell >= 0){
		char value[PROPERTY_VALUE_MAX];
		property_get("persist.sys.exit", value, "1");
		if (strcmp(value, "0") == 0) {
			write(fdcell, "1", strlen("1"));
		}else{
			write(fdcell, "0", strlen("0"));
		}
		close(fdcell);
		fdcell = 0;
	}

	int fdname = open("/.name",O_WRONLY|O_CREAT,0660);
	if(fdname >= 0){
		write(fdname, cellname, strlen(cellname));
		close(fdname);
		fdname = 0;
	}

	execve(cell_args->argv[0], cell_args->argv, NULL);
	syserr = "execve";

out_err:
	{
		int e = errno;
		ALOGE("ERROR{%s: errno=%d (%s)}", syserr, e, strerror(e));
	}
	return -1;
}

static int write_pid(char *pid_file, int pid)
{
	FILE *fp;

	if (!pid_file)
		return 0;

	fp = fopen(pid_file, "w");
	if (!fp)
		return -1;
	fprintf(fp, "%d", pid);
	fflush(fp);
	fclose(fp);
	return 0;
}

int do_share_dalvik_cache(char *root_path)
{
	char target[PATH_MAX];
	int ret = -1;

	ALOGI("Dalvik Cache: relocating %s/data/dalvik-cache...", root_path);

	snprintf(target, sizeof(target), "%s/data/dalvik-cache", root_path);
	mkdir(target, 0755);

	/* bind-mount the host's dalvik-cache directory into the cell */
	ret = mount("/data/dalvik-cache", target, "none", MS_BIND, 0);
	if (ret < 0)
		ALOGW("Couldn't share Dalvik cache");

	return (ret < 0) ? -1 : 0;
}

int mount_dev_tmpfs(char *root_path)
{
	char target[PATH_MAX];
	struct stat st;
	int ret = -1;
	
	snprintf(target, sizeof(target), "%s/dev", root_path);
	if (stat(target, &st) < 0) {
		/* try to create the directory */
		if (mkdir(target, 0755) < 0) {
			ALOGE("cannot create <root>/dev: %s", strerror(errno));
			return -1;
		}
	}

	ret = mount("tmpfs", target, "tmpfs", 0, "mode=0755");
	if (ret < 0) {
		ALOGE("unable to mount tmpfs: %s", strerror(errno));
		return -1;
	}

	return 0;
}

static int do_clone(struct cell_args *cell_args)
{
	struct cell_start_args *args = &cell_args->start_args;
	int pid;
	size_t stacksize = 4 * sysconf(_SC_PAGESIZE);
	void *childstack, *stack = malloc(stacksize);
	unsigned long flags;
	char buf[20];

	if (!stack) {
		ALOGE("cannot allocate stack: %s", strerror(errno));
		return -1;
	}

	childstack = (char *)stack + stacksize;

	flags = SIGCHLD;
	if (args->uts_ns)
		flags |= CLONE_NEWUTS;
	if (args->ipc_ns)
		flags |= CLONE_NEWIPC;
	if (args->user_ns)
		flags |= CLONE_NEWUSER;
	if (args->net_ns)
		flags |= CLONE_NEWNET;
	if (args->pid_ns)
		flags |= CLONE_NEWPID;
	if (args->mount_ns)
		flags |= CLONE_NEWNS;

	pid = clone(do_child, childstack, flags, cell_args);

	free(stack);
	if (pid < 0) {
		ALOGE("clone: %s", strerror(errno));
		return -1;
	}
	if (gettimeofday(&cell_args->start_time, NULL) == -1)
		ALOGE("%s: gettimeofday failed: %s", __func__, strerror(errno));

	/*
	 * Put the new process in a cgroup if requested.
	 * Note that the child will block until we release it with a write
	 * into the global pipe. This ensures that all children of the cell's
	 * init process will inherit the cgroup (i.e. a child will _not_ be
	 * forked before we can put init into a cgroup)
	 */
	cell_args->init_pid = pid;
	do_newcgroup(cell_args);

	//cells_init_net(cell_args);

	snprintf(buf, sizeof(buf), "%d", pid);
	close(g_cgroup_pipe[0]);
	write(g_cgroup_pipe[1], buf, strlen(buf)+1);
	close(g_cgroup_pipe[1]);

	return pid;
}

/* Careful: When this is called, it's called from a different process.
 * That means, no signaling to celld's threads (ex: destroy_wifi) */
void tear_down_cell(struct cell_args *cell_args, struct pty_info *console_pty)
{
	struct cell_start_args *args = &cell_args->start_args;
	cleanup_cell_console(console_pty);

	if (unmount_all(cell_args->rootdir, args->mnt_rootfs) == -1)
		ALOGW("Couldn't unmount_all() on %s", cell_args->rootdir);
}

static void thread_exit_handler(int sig)
{
	sig;

	pthread_exit(0);
}

/* sd is used for sending more detailed error messages to client.
 * console_pty is filled in after returning. pty_info.ptm will be -1 if no
 * console is requested. */
int cell_nsexec(int sd, struct cell_args *cell_args,
		char *name, struct pty_info *console_pty)
{
	struct cell_start_args *args = &cell_args->start_args;
	int pid = -1;
	char *rootdir = cell_args->rootdir;
	struct sigaction actions;

	/* Setup signal handler for SIGUSR2 (fake pthread_cancel) */
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = thread_exit_handler;
	if (sigaction(SIGUSR2, &actions, NULL) < 0)
		ALOGE("sigaction(%s): %s", name, strerror(errno));

	/* pipe to synchronize child execution and entry into new cgroup */
	if (pipe(g_cgroup_pipe)) {
		ALOGE("pipe: %s", strerror(errno));
		send_msg(sd, "nsexec failed: pipe() failed");
		goto err_cleanup;
	}

	/* pipes to synchronize child start and CellD monitoring */
	if (pipe(cell_args->child_pipe) || pipe(cell_args->init_pipe)) {
		ALOGE("Can't create child/init pipes for '%s': %s",
			  name, strerror(errno));
		send_msg(sd, "nsexec failed: child/init pipe creating failed");
		goto err_cleanup;
	}

	if (args->mnt_rootfs) {
		if (mount_cell(name, args->sdcard_branch)) {
			ALOGE("couldn't mount '%s' rootfs: %d", name, errno);
			send_msg(sd, "nsexec failed: mount() rootfs failed");
			goto err_cleanup;
		}
	}

	if (args->share_dalvik_cache)
		do_share_dalvik_cache(rootdir);

	if (args->open_console) {
		ALOGD("Opening console for '%s'", name);
		int ret = create_cell_console(rootdir, console_pty);
		if (ret < 0) {
			ALOGE("Couldn't open console in '%s'. "
				  "Continuing nsexec..", name);
			console_pty->ptm = -1;
		}
	} else
		console_pty->ptm = -1;

	cell_args->cell_socket = sd;
	ALOGI("Cloning '%s'", name);
	pid = do_clone(cell_args);

	if (pid == -1) {
		ALOGE("clone(%s) failed: tearing down cell", name);
		goto err_cleanup;
	}

	if(args->net_ns){
		int i = 0;
		sscanf(cell_args->cellname, "cell%d",&i);
		if(i > 0){
			createveth(i);

			vethtons(pid,i);
		}
	}

	write_pid(args->pid_file, pid);

	close(cell_args->child_pipe[1]);
	close(cell_args->init_pipe[0]);

	ALOGI("Successfully initialized '%s' with init PID %d", name, pid);
	return pid;

err_cleanup:
	tear_down_cell(cell_args, console_pty);
	return -1;
}
