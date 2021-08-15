#define _GNU_SOURCE
#include <stdio.h>
#undef _GNU_SOURCe
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/sockios.h>
#include "nl.h"
#include "network.h"
#define LOG_TAG "Cells/network"
#include <cutils/log.h>

#ifndef VETH_INFO_PEER
# define VETH_INFO_PEER 1
#endif

int wait_for_pid(pid_t pid)
{
	int status, ret;

again:
	ret = waitpid(pid, &status, 0);
	if (ret == -1) {
		if (errno == EINTR)
			goto again;
		return -1;
	}
	if (ret != pid)
		goto again;
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		return -1;
	return 0;
}

int veth_peer_create(const char *name1, const char *name2)
{
	struct nl_handler nlh;
	struct nlmsg *nlmsg = NULL, *answer = NULL;
	struct ifinfomsg *ifi;
	struct rtattr *nest1, *nest2, *nest3;
	int len, err;

	err = netlink_open(&nlh, NETLINK_ROUTE);
	if (err)
		return err;

	err = -EINVAL;
	len = strlen(name1);
	if (len == 1 || len >= IFNAMSIZ)
		goto out;

	len = strlen(name2);
	if (len == 1 || len >= IFNAMSIZ)
		goto out;

	err = -ENOMEM;
	nlmsg = nlmsg_alloc(NLMSG_GOOD_SIZE);
	if (!nlmsg)
		goto out;

	answer = nlmsg_alloc_reserve(NLMSG_GOOD_SIZE);
	if (!answer)
		goto out;

	nlmsg->nlmsghdr->nlmsg_flags =
		NLM_F_REQUEST|NLM_F_CREATE|NLM_F_EXCL|NLM_F_ACK;
	nlmsg->nlmsghdr->nlmsg_type = RTM_NEWLINK;

	ifi = nlmsg_reserve(nlmsg, sizeof(struct ifinfomsg));
	ifi->ifi_family = AF_UNSPEC;

	err = -EINVAL;
	nest1 = nla_begin_nested(nlmsg, IFLA_LINKINFO);
	if (!nest1)
		goto out;

	if (nla_put_string(nlmsg, IFLA_INFO_KIND, "veth"))
		goto out;

	nest2 = nla_begin_nested(nlmsg, IFLA_INFO_DATA);
	if (!nest2)
		goto out;

	nest3 = nla_begin_nested(nlmsg, VETH_INFO_PEER);
	if (!nest3)
		goto out;

	ifi = nlmsg_reserve(nlmsg, sizeof(struct ifinfomsg));
	if (!ifi)
		goto out;

	if (nla_put_string(nlmsg, IFLA_IFNAME, name2))
		goto out;

	nla_end_nested(nlmsg, nest3);

	nla_end_nested(nlmsg, nest2);

	nla_end_nested(nlmsg, nest1);

	if (nla_put_string(nlmsg, IFLA_IFNAME, name1))
		goto out;

	err = netlink_transaction(&nlh, nlmsg, answer);
out:
	netlink_close(&nlh);
	nlmsg_free(answer);
	nlmsg_free(nlmsg);
	return err;
}


int switch_to_ns(pid_t pid, const char *ns) {
	int fd, ret;
	char nspath[MAXPATHLEN];

	/* Switch to new ns */
	ret = snprintf(nspath, MAXPATHLEN, "/proc/%d/ns/%s", pid, ns);
	if (ret < 0 || ret >= MAXPATHLEN)
		return -1;

	fd = open(nspath, O_RDONLY);
	if (fd < 0) {
		ALOGE("failed to open %s", nspath);
		return -1;
	}

	ret = setns(fd, 0);
	if (ret) {
		ALOGE("failed to set process %d to %s of %d.", pid, ns, fd);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int netdev_rename_by_index(int ifindex, const char *newname)
{
	struct nl_handler nlh;
	struct nlmsg *nlmsg = NULL, *answer = NULL;
	struct ifinfomsg *ifi;
	int len, err;

	err = netlink_open(&nlh, NETLINK_ROUTE);
	if (err)
		return err;

	len = strlen(newname);
	if (len == 1 || len >= IFNAMSIZ)
		goto out;

	err = -ENOMEM;
	nlmsg = nlmsg_alloc(NLMSG_GOOD_SIZE);
	if (!nlmsg)
		goto out;

	answer = nlmsg_alloc_reserve(NLMSG_GOOD_SIZE);
	if (!answer)
		goto out;

	nlmsg->nlmsghdr->nlmsg_flags = NLM_F_ACK|NLM_F_REQUEST;
	nlmsg->nlmsghdr->nlmsg_type = RTM_NEWLINK;

	ifi = nlmsg_reserve(nlmsg, sizeof(struct ifinfomsg));
	ifi->ifi_family = AF_UNSPEC;
	ifi->ifi_index = ifindex;

	if (nla_put_string(nlmsg, IFLA_IFNAME, newname))
		goto out;

	err = netlink_transaction(&nlh, nlmsg, answer);
out:
	netlink_close(&nlh);
	nlmsg_free(answer);
	nlmsg_free(nlmsg);
	return err;
}


int netdev_rename_by_name(const char *oldname, const char *newname)
{
	int len, index;

	len = strlen(oldname);
	if (len == 1 || len >= IFNAMSIZ)
		return -EINVAL;

	index = if_nametoindex(oldname);
	if (!index)
		return -EINVAL;

	return netdev_rename_by_index(index, newname);
}


int
netdev_rename_by_name_in_netns(pid_t pid, const char *old, const char *new)
{
	pid_t fpid = fork();

	if (fpid < 0)
		return -1;
	if (fpid != 0)
		return wait_for_pid(fpid);
	if (0 != switch_to_ns(pid, "net"))
		return -1;
	exit(netdev_rename_by_name(old, new));
}

int
netdev_move_to_netns(const char *ifname, pid_t pid, const char* newname)
{
	int err = -1;
	pid_t fpid;
	char *cmd;
#if 0
	fpid = fork();
	if (fpid < 0)
		goto out1;
	if (fpid == 0) {
		char pidstr[30];
		sprintf(pidstr, "%d", pid);
		if (execlp("ip", "link", "set", ifname, "netns", pidstr, NULL)) {
			ALOGE("set netns fail! errno=%d\n", errno);
			exit(1);
		}
		exit(0); // notreached
	}
	if (wait_for_pid(fpid)) {		
		ALOGE("set netns fail! errno=%d\n", errno);
		goto out1;
	}
#else
	int ret;
	char system_cmd[256];
	snprintf(system_cmd, sizeof(system_cmd), "ip link set %s netns %d", ifname, pid);
	ret = system(system_cmd);
	ALOGE("set netns fail! ret=%d errno=%d\n", ret, errno);
#endif
	err = 0;
	if (newname)
		err = netdev_rename_by_name_in_netns(pid, ifname, newname);

out1:
	return err;
}


