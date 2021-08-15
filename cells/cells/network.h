#ifndef _NETWORK_H_
#define _NETWORK_H_

int veth_peer_create(const char *name1, const char *name2);
int switch_to_ns(pid_t pid, const char *ns);
int netdev_rename_by_index(int ifindex, const char *newname);
int netdev_rename_by_name(const char *oldname, const char *newname);
int netdev_rename_by_name_in_netns(pid_t pid, const char *old, const char *new);
int netdev_move_to_netns(const char *ifname, pid_t pid, const char* newname);

#endif

