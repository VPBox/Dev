#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOG_TAG "Cells/cellnet"
#include <cutils/log.h>
#include <cutils/memory.h>
#include <cutils/misc.h>

#define INTERFACE "wlan0"
#define VETH0 "vm_wlan_%d_0"
#define VETH1 "vm_wlan_%d_1"

#define VETHIFACEADDRBASE 16

#define VETHMASK 16
#define VETHADDRMASK "255.255.0.0"
#define VETHGATEADDR   "172.%d.0.0"

#define VETHIFACEADDR0 "172.%d.3.2"
#define VETHIFACEADDR1 "172.%d.3.3"

void init_cell_net()
{

}

void createveth(int index)
{
    int ret  = 0;
    char vethname0[24] = {0};
    char vethname1[24] = {0};
    char vethaddr0[64] = {0};
    char vethgateaddr[64] = {0};

    char cmd[256] = {0};

    snprintf(vethname0, sizeof(vethname0), VETH0, index);
    snprintf(vethname1, sizeof(vethname1), VETH1, index);
    snprintf(vethgateaddr, sizeof(vethgateaddr), VETHGATEADDR, VETHIFACEADDRBASE + index);
    snprintf(vethaddr0, sizeof(vethaddr0), VETHIFACEADDR0, VETHIFACEADDRBASE + index);

    errno = 0;
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ip link add name %s type veth peer name %s", vethname0, vethname1);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd ,strerror(errno));

    errno = 0;
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ifconfig %s %s netmask %s up", vethname0, vethaddr0, VETHADDRMASK);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd ,strerror(errno));

    errno = 0;
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "iptables -t nat -A POSTROUTING -s %s/%d -o %s -j MASQUERADE", vethgateaddr, VETHMASK, INTERFACE);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd ,strerror(errno));

    errno = 0;
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "iptables -t filter -A FORWARD -i %s -o %s -j ACCEPT", INTERFACE, vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd ,strerror(errno));

    errno = 0;
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "iptables -t filter -A FORWARD -o %s -i %s -j ACCEPT", INTERFACE, vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd ,strerror(errno));
}

void vethtons(int pid,int index)
{
    int ret  = 0;
    char vethname1[24] = {0};
    char cmd[256];

    snprintf(vethname1, sizeof(vethname1), VETH1, index);

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ip link set %s netns %d",vethname1 ,pid );
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));
}

void rnameveth(int index)
{
    int ret  = 0;
    char vethname1[24] = {0};
    char vethaddr0[64] = {0};
    char vethaddr1[64] = {0};
    char cmd[256];

    snprintf(vethname1, sizeof(vethname1), VETH1, index);
    snprintf(vethaddr0, sizeof(vethaddr0), VETHIFACEADDR0, VETHIFACEADDRBASE + index);
    snprintf(vethaddr1, sizeof(vethaddr1), VETHIFACEADDR1, VETHIFACEADDRBASE + index);

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ip link set %s name %s",vethname1 ,INTERFACE );
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ifconfig %s %s netmask %s up",INTERFACE,vethaddr1,VETHADDRMASK);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "route add default gw %s",vethaddr0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));
}

void starttether(int index)
{
    int ret  = 0;
    char vethname0[24] = {0};
    char vethaddr0[64] = {0};
    char vethgateaddr[64] = {0};

    char cmd[256] = {0};

    snprintf(vethname0, sizeof(vethname0), VETH0, index);
    snprintf(vethgateaddr, sizeof(vethgateaddr), VETHGATEADDR, VETHIFACEADDRBASE + index);
    snprintf(vethaddr0, sizeof(vethaddr0), VETHIFACEADDR0, VETHIFACEADDRBASE + index);

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc interface setcfg %s %s %d up multicast broadcast",vethname0 ,vethaddr0 ,VETHMASK);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc tether interface add %s",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc network interface add local %s",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc network route add local %s %s/%d",vethname0,vethgateaddr,VETHMASK);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc ipfwd enable %s",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc nat enable %s %s 1 %s/%d",vethname0,INTERFACE,vethgateaddr,VETHMASK);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc ipfwd add %s %s",vethname0,INTERFACE);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));
}

void stoptether(int index)
{
    int ret  = 0;
    char vethname0[24] = {0};

    char cmd[256] = {0};

    snprintf(vethname0, sizeof(vethname0), VETH0, index);

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc tether interface remove %s",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc network interface remove local %s",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ndc ipfwd disable %s",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));
}

void delveth(int index)
{
    int ret  = 0;
    char vethname0[24] = {0};
    char vethaddr0[64] = {0};
    char vethgateaddr[64] = {0};

    char cmd[256] = {0};

    snprintf(vethname0, sizeof(vethname0), VETH0, index);
    snprintf(vethgateaddr, sizeof(vethgateaddr), VETHGATEADDR, VETHIFACEADDRBASE + index);
    snprintf(vethaddr0, sizeof(vethaddr0), VETHIFACEADDR0, VETHIFACEADDRBASE + index);

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "iptables -t nat -D POSTROUTING -s %s/%d -o %s -j MASQUERADE",vethgateaddr,VETHMASK,INTERFACE);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "iptables -t filter -D FORWARD -i %s -o %s -j ACCEPT",INTERFACE,vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "iptables -t filter -D FORWARD -o %s -i %s -j ACCEPT",INTERFACE,vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));

    errno = 0;
    memset(cmd,0,sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ip link delete %s type veth",vethname0);
    ret = system(cmd);
    ALOGD("%s errno = %s",cmd,strerror(errno));
}
