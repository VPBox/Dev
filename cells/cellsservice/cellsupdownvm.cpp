#define LOG_TAG "GuiExt"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cutils/log.h>
#include <cutils/properties.h>

static void thread_exit_handler(int sig)
{
    sig;

    pthread_exit(0);
}

int main(int argc, char** argv)
{
    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = thread_exit_handler;
    if (sigaction(SIGQUIT|SIGKILL, &actions, NULL) < 0){
        ALOGE("sigaction error: %s",strerror(errno));
    }
    
    if (argc == 2){
        if(strcmp(argv[1],"up") == 0){
            property_set("persist.sys.vm.up", "1");

            system("rm -rf /data/cells/cell1.tar.gz");
             ALOGE("rm -rf /data/cells/cell1.tar.gz \n");

            system("cd /data/cells && busybox tar zcvfp cell1.tar.gz cell1-rw");
             ALOGE("busybox tar zcvfp /data/cells/cell1.tar.gz /data/cells/cell1-rw \n");

            system("cd /data/cells && busybox ftpput -u ftp-hw-user -p 1 129.204.70.73 cell1.tar.gz");
             ALOGE("busybox ftpput -u ftp-hw-user -p 1 129.204.70.73 /data/cells/cell1.tar.gz err= %s \n", strerror(errno));

            property_set("persist.sys.vm.up", "0");

            property_set("persist.sys.vmup", "0");
        }else if(strcmp(argv[1],"down") == 0){

            property_set("persist.sys.vmstop", "1");
            property_set("persist.sys.vmstop", "0");

            property_set("persist.sys.vm.down", "1");

            if(access("/data/cells/cell1.tar.gz",F_OK)!= 0){
                system("touch /data/cells/cell1.tar.gz ");
            }

            system("cd /data/cells && busybox ftpget -c  -u ftp-hw-user -p 1 129.204.70.73 cell1.tar.gz");
             ALOGE("busybox ftpget -u ftp-hw-user -p 1 129.204.70.73 /data/cells/cell1.tar.gz  err=%s \n", strerror(errno));

            system("rm -rf /data/cells/cell1-rw && mkdir /data/cells/cell1-rw");
             ALOGE("rm -rf /data/cells/cell1-rw \n");

            system("cd /data/cells && busybox tar zxvfp cell1.tar.gz");
             ALOGE("busybox tar zxvfp /data/cells/cell1.tar.gz /data/cells/cell1-rw \n");

            property_set("persist.sys.vm.down", "0");

            property_set("persist.sys.vmdown", "0");
        }else if(strcmp(argv[1],"untar") == 0){

            if(access("/sdcard/cell1.tar.gz",F_OK) == 0){
                if(access("/data/cells/cell1.tar.gz",F_OK) == 0)
                    system("rm /data/cells/cell1.tar.gz ");
                system("mv /sdcard/cell1.tar.gz /data/cells/cell1.tar.gz ");
            }else{
                if(access("/data/cells/cell1.tar.gz",F_OK) != 0){
                    return 0;
                }
            }

            property_set("persist.sys.vmstop", "1");
            property_set("persist.sys.vmstop", "0");

            property_set("persist.sys.vm.untar", "1");

            system("rm -rf /data/cells/cell1-rw && mkdir /data/cells/cell1-rw");
                ALOGE("rm -rf /data/cells/cell1-rw \n");

            system("cd /data/cells && busybox tar zxvfp cell1.tar.gz");
                ALOGE("busybox tar zxvfp /data/cells/cell1.tar.gz /data/cells/cell1-rw \n");

            property_set("persist.sys.vm.untar", "0");
        }
    }

    return 0;
}
