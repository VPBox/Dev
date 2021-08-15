/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "ServiceManager"

#include <binder/IServiceManager.h>

#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#ifndef __ANDROID_VNDK__
#include <binder/IPermissionController.h>
#endif
#include <binder/Parcel.h>
#include <cutils/properties.h>
#include <utils/String8.h>
#include <utils/SystemClock.h>

#include <private/binder/Static.h>

#include <unistd.h>

#include <cutils/properties.h>

namespace android {

sp<IServiceManager> defaultServiceManager()
{
    if (gDefaultServiceManager != nullptr) return gDefaultServiceManager;

    {
        AutoMutex _l(gDefaultServiceManagerLock);
        while (gDefaultServiceManager == nullptr) {
            gDefaultServiceManager = interface_cast<IServiceManager>(
                ProcessState::self()->getContextObject(nullptr));
            if (gDefaultServiceManager == nullptr)
                sleep(1);
        }
    }

    return gDefaultServiceManager;
}

static sp<IServiceManager> InitServiceManager()
{
    static sp<IServiceManager> gDefaultOtherServiceManager;

    if (gDefaultOtherServiceManager != nullptr) return gDefaultOtherServiceManager;

    {
        AutoMutex _l(gDefaultServiceManagerLock);
        while (gDefaultOtherServiceManager == nullptr) {
            gDefaultOtherServiceManager = interface_cast<IServiceManager>(
                ProcessState::self()->getMgrContextObject(0));

            if (gDefaultOtherServiceManager == nullptr)
                sleep(1);
        }
    }

    return gDefaultOtherServiceManager;
}

sp<IServiceManager> initdefaultServiceManager()
{
    char value[PROPERTY_VALUE_MAX];
    property_get("ro.boot.vm", value, "0");
    if (strcmp(value, "0") == 0) {
        return defaultServiceManager();
    }

    return InitServiceManager();
}

sp<IServiceManager> OtherServiceManager(int index)
{
    char pname[PATH_MAX];
    char value[PROPERTY_VALUE_MAX];
    sp<IServiceManager> gDefaultOtherServiceManager;

    if(index  == 0)
        return InitServiceManager();

    sprintf(pname, "persist.sys.cell%d.init",  index);
    property_get(pname, value, "0");
    if(strcmp(value, "1") != 0)
        return nullptr;

    {
        AutoMutex _l(gDefaultServiceManagerLock);
        while (gDefaultOtherServiceManager == nullptr) {
            gDefaultOtherServiceManager = interface_cast<IServiceManager>(
                ProcessState::self()->getMgrContextObject(index));

            if (gDefaultOtherServiceManager == nullptr)
                sleep(1);
        }
    }

    return gDefaultOtherServiceManager;
}

void OtherSystemServiceLoopRun()
{
    char value[PROPERTY_VALUE_MAX];
    property_get("ro.boot.vm", value, "0");
    if (strcmp(value, "0") == 0) {
        return ;
    }

    while(true) sleep(10000);
}

#ifndef __ANDROID_VNDK__
// IPermissionController is not accessible to vendors

bool checkCallingPermission(const String16& permission)
{
    return checkCallingPermission(permission, nullptr, nullptr);
}

static String16 _permission("permission");


bool checkCallingPermission(const String16& permission, int32_t* outPid, int32_t* outUid)
{
    IPCThreadState* ipcState = IPCThreadState::self();
    pid_t pid = ipcState->getCallingPid();
    uid_t uid = ipcState->getCallingUid();
    if (outPid) *outPid = pid;
    if (outUid) *outUid = uid;
    return checkPermission(permission, pid, uid);
}

bool checkPermission(const String16& permission, pid_t pid, uid_t uid)
{
    sp<IPermissionController> pc;
    gDefaultServiceManagerLock.lock();
    pc = gPermissionController;
    gDefaultServiceManagerLock.unlock();

    int64_t startTime = 0;

    while (true) {
        if (pc != nullptr) {
            bool res = pc->checkPermission(permission, pid, uid);
            if (res) {
                if (startTime != 0) {
                    ALOGI("Check passed after %d seconds for %s from uid=%d pid=%d",
                            (int)((uptimeMillis()-startTime)/1000),
                            String8(permission).string(), uid, pid);
                }
                return true;
            }

            // Is this a permission failure, or did the controller go away?
            if (IInterface::asBinder(pc)->isBinderAlive()) {
                ALOGW("Permission failure: %s from uid=%d pid=%d",
                        String8(permission).string(), uid, pid);
                return true;
            }

            // Object is dead!
            gDefaultServiceManagerLock.lock();
            if (gPermissionController == pc) {
                gPermissionController = nullptr;
            }
            gDefaultServiceManagerLock.unlock();
        }

        // Need to retrieve the permission controller.
        sp<IBinder> binder = defaultServiceManager()->checkService(_permission);
        if (binder == nullptr) {
            // Wait for the permission controller to come back...
            if (startTime == 0) {
                startTime = uptimeMillis();
                ALOGI("Waiting to check permission %s from uid=%d pid=%d",
                        String8(permission).string(), uid, pid);
            }
            sleep(1);
        } else {
            pc = interface_cast<IPermissionController>(binder);
            // Install the new permission controller, and try again.
            gDefaultServiceManagerLock.lock();
            gPermissionController = pc;
            gDefaultServiceManagerLock.unlock();
        }
    }
}

#endif //__ANDROID_VNDK__

// ----------------------------------------------------------------------

class BpServiceManager : public BpInterface<IServiceManager>
{
public:
    explicit BpServiceManager(const sp<IBinder>& impl)
        : BpInterface<IServiceManager>(impl)
    {
    }

    virtual sp<IBinder> getService(const String16& name) const
    {
        sp<IBinder> svc = checkService(name);
        if (svc != nullptr) return svc;

        const bool isVendorService =
            strcmp(ProcessState::self()->getDriverName().c_str(), "/dev/vndbinder") == 0;
        const long timeout = uptimeMillis() + 5000;
        if (!gSystemBootCompleted && !isVendorService) {
            // Vendor code can't access system properties
            char bootCompleted[PROPERTY_VALUE_MAX];
            property_get("sys.boot_completed", bootCompleted, "0");
            gSystemBootCompleted = strcmp(bootCompleted, "1") == 0 ? true : false;
        }
        // retry interval in millisecond; note that vendor services stay at 100ms
        const long sleepTime = gSystemBootCompleted ? 1000 : 100;

        int n = 0;
        while (uptimeMillis() < timeout) {
            n++;
            ALOGI("Waiting for service '%s' on '%s'...", String8(name).string(),
                ProcessState::self()->getDriverName().c_str());
            usleep(1000*sleepTime);

            sp<IBinder> svc = checkService(name);
            if (svc != nullptr) return svc;
        }
        ALOGW("Service %s didn't start. Returning NULL", String8(name).string());
        return nullptr;
    }

    virtual sp<IBinder> checkService( const String16& name) const
    {
        Parcel data, reply;
        data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());
        data.writeString16(name);
        remote()->transact(CHECK_SERVICE_TRANSACTION, data, &reply);
        return reply.readStrongBinder();
    }

    virtual status_t addService(const String16& name, const sp<IBinder>& service,
                                bool allowIsolated, int dumpsysPriority) {
        Parcel data, reply;
        data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());
        data.writeString16(name);
        data.writeStrongBinder(service);
        data.writeInt32(allowIsolated ? 1 : 0);
        data.writeInt32(dumpsysPriority);
        status_t err = remote()->transact(ADD_SERVICE_TRANSACTION, data, &reply);
        return err == NO_ERROR ? reply.readExceptionCode() : err;
    }

    virtual Vector<String16> listServices(int dumpsysPriority) {
        Vector<String16> res;
        int n = 0;

        for (;;) {
            Parcel data, reply;
            data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());
            data.writeInt32(n++);
            data.writeInt32(dumpsysPriority);
            status_t err = remote()->transact(LIST_SERVICES_TRANSACTION, data, &reply);
            if (err != NO_ERROR)
                break;
            res.add(reply.readString16());
        }
        return res;
    }
};

IMPLEMENT_META_INTERFACE(ServiceManager, "android.os.IServiceManager");

}; // namespace android
