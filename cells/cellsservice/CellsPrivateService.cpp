#define LOG_TAG "CELLSSERVICE"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include "CellsPrivateService.h"

#include <binder/IServiceManager.h>
#include <gui/ISurfaceComposer.h>
#include <utils/String16.h>

#include <powermanager/IPowerManager.h>
#include <powermanager/PowerManager.h>

namespace android {

#define SYSTEMPRIVATE_LOGV(x, ...) ALOGV("[CellsPrivate] " x, ##__VA_ARGS__)
#define SYSTEMPRIVATE_LOGD(x, ...) ALOGD("[CellsPrivate] " x, ##__VA_ARGS__)
#define SYSTEMPRIVATE_LOGI(x, ...) ALOGI("[CellsPrivate] " x, ##__VA_ARGS__)
#define SYSTEMPRIVATE_LOGW(x, ...) ALOGW("[CellsPrivate] " x, ##__VA_ARGS__)
#define SYSTEMPRIVATE_LOGE(x, ...) ALOGE("[CellsPrivate] " x, ##__VA_ARGS__)

CellsPrivateService::CellsPrivateService()
{
}

CellsPrivateService::~CellsPrivateService()
{
}

int CellsPrivateService::isInCellstar()
{
    return mtar_pthread_t;
}

void CellsPrivateService::setCellstaring()
{
    mtar_pthread_t = 1;
}

void CellsPrivateService::setCellstared()
{
    mtar_pthread_t = 0;
}

void CellsPrivateService::startCellstar()
{

}

status_t CellsPrivateService::setProperty(const String16& name,const String16& value)
{
    SYSTEMPRIVATE_LOGD("SETPROPERTY arg %s %s", String8(name).string(), String8(value).string());
    status_t result = property_set(String8(name).string(), String8(value).string());
    SYSTEMPRIVATE_LOGD("SETPROPERTY result = %d", result);
    return result;
}

status_t CellsPrivateService::startCellsVM(const String16& name)
{
    name;

    char cmd[200];
    snprintf(cmd, sizeof(cmd), "cellc start %s",String8(name).string());
    SYSTEMPRIVATE_LOGD("STARTCELLSVM cmd = %s", cmd);
    system(cmd);
    return NO_ERROR;
}

status_t CellsPrivateService::stopCellsVM(const String16& name)
{
    name;

    char cmd[200];
    snprintf(cmd, sizeof(cmd), "cellc stop %s",String8(name).string());
    SYSTEMPRIVATE_LOGD("STOPCELLSVM cmd = %s", cmd);
    system(cmd);
    return NO_ERROR;
}

status_t CellsPrivateService::cellsSwitchVM(const String16& name)
{
    name;

    char cmd[200];
    snprintf(cmd, sizeof(cmd), "cellc switch %s",String8(name).string());
    SYSTEMPRIVATE_LOGD("CELLSSWITCHVM cmd = %s", cmd);
    system(cmd);
    property_set("persist.sys.active", String8(name).string());
    return NO_ERROR;
}

status_t CellsPrivateService::cellsSwitchHOST(const String16& name)
{
    name;

    char cmd[200];
    snprintf(cmd, sizeof(cmd), "cellc switch host");
    SYSTEMPRIVATE_LOGD("CELLSSWITCHHOST cmd = %s", cmd);
    system(cmd);
    property_set("persist.sys.active", "");
    return NO_ERROR;
}

static void* gotosleep(void* o)
{
    o;

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IPowerManager> mPowerManager = 
            android::interface_cast<android::IPowerManager>(sm->checkService(android::String16("power")));
        if(mPowerManager != NULL){
            mPowerManager->goToSleep(long(ns2ms(systemTime())),GO_TO_SLEEP_REASON_POWER_BUTTON,0);
        }
    }

    ALOGD("BACK SYSTEM go to sleep...");

    return (void*)0;
};

static void create_gotosleep_pthread()
{
    int ret;
    pthread_t daemon_thread;
    
    /* Start listening for connections in a new thread */
    ret = pthread_create(&daemon_thread, NULL,gotosleep, NULL);
    if (ret) {
        ALOGE("create_gotosleep_pthread err: %s", strerror(errno));
    }
};

status_t CellsPrivateService::switchCellsVM(const String16& name)
{
    int i = 0;
    char value[PROPERTY_VALUE_MAX];
    char pname[PATH_MAX];

    ALOGD("switchCellsVM: %s",String8(name).string());

    property_get("ro.boot.vm", value, "1");
    if((strcmp(value, "0") == 0)){

        {
            sscanf(String8(name).string(), "cell%d",&i);
            if(i <= 0)
                return 0;

            memset(value,0,PROPERTY_VALUE_MAX);
            memset(pname,0,PATH_MAX);
            sprintf(pname, "persist.sys.%s.init",  String8(name).string());
            property_get(pname, value, "0");
            if((strcmp(value, "0") == 0))
                return 0;
        }

        {
            exitHost(android::String16("host"));
        }
    
        {
            cellsSwitchVM(name);
        }
    
        {
            android::sp<android::IServiceManager> sm = android::OtherServiceManager(i);
            android::sp<android::ICellsPrivateService> mCellsPrivateService = 
                android::interface_cast<android::ICellsPrivateService>(sm->checkService(android::String16("CellsPrivateService")));
            if(mCellsPrivateService != NULL){
                mCellsPrivateService->enterCell(name);
            }else{
                SYSTEMPRIVATE_LOGD("OtherServiceManager = 0");
            }
        }
    }else{

        {
            if(strcmp(String8(name).string(), "host") != 0)
                return 0;
        }

        {
            exitCell(name);
        }

        {
            android::sp<android::IServiceManager> sm = android::initdefaultServiceManager();
            android::sp<android::ICellsPrivateService> mCellsPrivateService = 
            android::interface_cast<android::ICellsPrivateService>(sm->checkService(android::String16("CellsPrivateService")));
            if(mCellsPrivateService != NULL){
                mCellsPrivateService->cellsSwitchHOST(android::String16("host"));
            }
        }
    
        {
            android::sp<android::IServiceManager> sm = android::initdefaultServiceManager();
            android::sp<android::ICellsPrivateService> mCellsPrivateService = 
            android::interface_cast<android::ICellsPrivateService>(sm->checkService(android::String16("CellsPrivateService")));
            if(mCellsPrivateService != NULL){
                mCellsPrivateService->enterHost(android::String16("host"));
            }
        }
    }

    {
        create_gotosleep_pthread();
    }

    SYSTEMPRIVATE_LOGD("SWITCHCELLSVM result = %d", 0);
    return NO_ERROR;
}

status_t CellsPrivateService::enterHost(const String16& name)
{
    name;

    {
        property_set("persist.sys.exit", "0");
    }

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IPowerManager> mPowerManager = 
            android::interface_cast<android::IPowerManager>(sm->checkService(android::String16("power")));
        if(mPowerManager != NULL){
            mPowerManager->wakeUp(long(ns2ms(systemTime())),android::String16("enter_self"),android::String16(""));
        }
    }

    {
        property_set("ctl.restart", "adbd");
    }

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::ISurfaceComposer> mComposer = 
            android::interface_cast<android::ISurfaceComposer>(sm->checkService(android::String16("SurfaceFlinger")));
        if(mComposer != NULL){
            mComposer->enterSelf();
        }
    }

    {
        //startCellstar();
    }

    SYSTEMPRIVATE_LOGD("ENTERHOST result = %d", 0);
    return NO_ERROR;
}

status_t CellsPrivateService::exitHost(const String16& name)
{
    name;

    {
        property_set("persist.sys.exit", "1");
    }

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::ISurfaceComposer> mComposer = 
            android::interface_cast<android::ISurfaceComposer>(sm->checkService(android::String16("SurfaceFlinger")));
        if(mComposer != NULL){
            mComposer->exitSelf();
        }
    }

    {
        property_set("ctl.stop", "adbd");
    }

    SYSTEMPRIVATE_LOGD("EXITHOST result = %d", 0);
    return NO_ERROR;
}

static void write_vm_exit(bool bexit){
    int vmfd = open("/.cell",O_WRONLY);
    if(vmfd>=0){
        if(bexit)
            write(vmfd,"1",strlen("1"));
        else
            write(vmfd,"0",strlen("0"));
        close(vmfd);
    }
}

status_t CellsPrivateService::enterCell(const String16& name)
{
    name;

    {
        write_vm_exit(false);
        property_set("persist.sys.exit", "0");
    }

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IPowerManager> mPowerManager = 
            android::interface_cast<android::IPowerManager>(sm->checkService(android::String16("power")));
        if(mPowerManager != NULL){
            mPowerManager->wakeUp(long(ns2ms(systemTime())),android::String16("enter_self"),android::String16(""));
        }
    }

    {
        property_set("ctl.restart", "adbd");
    }

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::ISurfaceComposer> mComposer = 
            android::interface_cast<android::ISurfaceComposer>(sm->checkService(android::String16("SurfaceFlinger")));
        if(mComposer != NULL){
            mComposer->enterSelf();
        }
    }

    SYSTEMPRIVATE_LOGD("ENTERCELL result = %d", 0);
    return NO_ERROR;
}

status_t CellsPrivateService::exitCell(const String16& name)
{
    name;

    {
        write_vm_exit(true);
        property_set("persist.sys.exit", "1");
    }

    {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::ISurfaceComposer> mComposer = 
            android::interface_cast<android::ISurfaceComposer>(sm->checkService(android::String16("SurfaceFlinger")));
        if(mComposer != NULL){
            mComposer->exitSelf();
        }
    }

    {
        property_set("ctl.stop", "adbd");
    }

    SYSTEMPRIVATE_LOGD("EXITCELL result = %d", 0);
    return NO_ERROR;
}

status_t CellsPrivateService::uploadCellsVM(const String16& name)
{
    name;

    return NO_ERROR;
}

status_t CellsPrivateService::downloadCellsVM(const String16& name)
{
    name;

    return NO_ERROR;
}

status_t CellsPrivateService::untarCellsVM(const String16& name)
{
    name;

    return NO_ERROR;
}

status_t CellsPrivateService::tarCellsVM(const String16& name)
{
    name;

    return NO_ERROR;
}

status_t CellsPrivateService::sendCellsVM(const String16& path, const String16& address)
{
    path;
    address;

    return NO_ERROR;
}

status_t CellsPrivateService::vmSystemReady(const String16& name)
{
    char pname[PATH_MAX] = {0};

    sprintf(pname, "persist.sys.%s.init",  String8(name).string());
    property_set(pname, "1");

    SYSTEMPRIVATE_LOGD("SYSTEMREADY name = %s", String8(name).string());
    return NO_ERROR;
}

};
