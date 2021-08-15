#define LOG_TAG "CellsService"

#include <fcntl.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <binder/BinderService.h>
#include <CellsPrivateService.h>

#include "cellnet.h"

using namespace android;

static void init_vm_exit()
{
	/*int vmfd = open("/.cell",O_RDONLY);
	if(vmfd >= 0){
		char buf[10];
		int len = read(vmfd, buf, 10);
		if(len > 0){
			if(strcmp(buf,"0") == 0){
				property_set("persist.sys.exit", "0");
			}else{
				property_set("persist.sys.exit", "1");
			}
		}
		close(vmfd);
	}*/

	property_set("persist.sys.exit", "1");
}

static void config_vm_net_work(void)
{
	char value[PROPERTY_VALUE_MAX];
	property_get("ro.cell.wlan.mac", value, "ac:c1:ee:57:64:9b");
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd), "ifconfig wlan0 hw ether %s", value);
	system(cmd);
	ALOGD("%s ", cmd);

};

static int init_vm_name()
{
	int vmfd = open("/.name",O_RDONLY);
	if(vmfd >= 0){
		char buf[125];
		int len = read(vmfd, buf, 125);
		if(len > 0){

			buf[len]='\0';
			property_set("persist.sys.vm.name", buf);
			close(vmfd);

			ALOGI("persist.sys.vm.name = %s ", buf);

			int i = 0;
			sscanf(buf, "cell%d",&i);
			return i;
		}
		close(vmfd);
	}
	return 0;
}

int main(int /*argc*/, char** /*argv*/)
{
	ALOGI("GuiExt service start...");

	char value[PROPERTY_VALUE_MAX];
	property_get("ro.boot.vm", value, "1");
	if((strcmp(value, "0") == 0)){
		property_set("persist.sys.exit", "0");
		property_set("persist.sys.vm.name", "");
		property_set("persist.sys.active", "");
	}else{
		init_vm_exit();

		int i = init_vm_name();
		if(i > 0){
			rnameveth(i);
			config_vm_net_work();
		}
	}

	CellsPrivateService::publishAndJoinThreadPool(true);

	ProcessState::self()->setThreadPoolMaxThreadCount(4);

	ALOGD("Cells service exit...");
    return 0;
}
