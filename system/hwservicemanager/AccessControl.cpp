/*
 * Copyright (C) 2017 The Android Open Source Project
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

#define LOG_TAG "hwservicemanager"

#include <android-base/logging.h>
#include <hidl-util/FQName.h>
#include <log/log.h>

#include "AccessControl.h"

namespace android {

static const char *kPermissionAdd = "add";
static const char *kPermissionGet = "find";
static const char *kPermissionList = "list";

struct audit_data {
    const char* interfaceName;
    const char* sid;
    pid_t       pid;
};

using android::FQName;

AccessControl::AccessControl() {
    mSeHandle = selinux_android_hw_service_context_handle();
    LOG_ALWAYS_FATAL_IF(mSeHandle == nullptr, "Failed to acquire SELinux handle.");

    if (getcon(&mSeContext) != 0) {
        LOG_ALWAYS_FATAL("Failed to acquire hwservicemanager context.");
    }

    selinux_status_open(true);

    mSeCallbacks.func_audit = AccessControl::auditCallback;
    selinux_set_callback(SELINUX_CB_AUDIT, mSeCallbacks);

    mSeCallbacks.func_log = selinux_log_callback; /* defined in libselinux */
    selinux_set_callback(SELINUX_CB_LOG, mSeCallbacks);
}

bool AccessControl::canAdd(const std::string& fqName, const CallingContext& callingContext) {
    FQName fqIface;

    if (!FQName::parse(fqName, &fqIface)) {
        return false;
    }
    const std::string checkName = fqIface.package() + "::" + fqIface.name();

    return checkPermission(callingContext, kPermissionAdd, checkName.c_str());
}

bool AccessControl::canGet(const std::string& fqName, const CallingContext& callingContext) {
    FQName fqIface;

    if (!FQName::parse(fqName, &fqIface)) {
        return false;
    }
    const std::string checkName = fqIface.package() + "::" + fqIface.name();

    return checkPermission(callingContext, kPermissionGet, checkName.c_str());
}

bool AccessControl::canList(const CallingContext& callingContext) {
    return checkPermission(callingContext, mSeContext, kPermissionList, nullptr);
}

AccessControl::CallingContext AccessControl::getCallingContext(pid_t sourcePid) {
    char *sourceContext = nullptr;

    if (getpidcon(sourcePid, &sourceContext) < 0) {
        ALOGE("SELinux: failed to retrieve process context for pid %d", sourcePid);
        return { false, "", sourcePid };
    }

    std::string context = sourceContext;
    freecon(sourceContext);
    return { true, context, sourcePid };
}

bool AccessControl::checkPermission(const CallingContext& source, const char *targetContext, const char *perm, const char *interface) {
    if (!source.sidPresent) {
        return false;
    }

    bool allowed = false;

    struct audit_data ad;
    ad.pid = source.pid;
    ad.sid = source.sid.c_str();
    ad.interfaceName = interface;

    allowed = (selinux_check_access(source.sid.c_str(), targetContext, "hwservice_manager",
                                    perm, (void *) &ad) == 0);

    return allowed;
}

bool AccessControl::checkPermission(const CallingContext& source, const char *perm, const char *interface) {
    char *targetContext = nullptr;
    bool allowed = false;

    // Lookup service in hwservice_contexts
    if (selabel_lookup(mSeHandle, &targetContext, interface, 0) != 0) {
        ALOGE("No match for interface %s in hwservice_contexts", interface);
        return false;
    }

    allowed = checkPermission(source, targetContext, perm, interface);

    freecon(targetContext);

    return allowed;
}

int AccessControl::auditCallback(void *data, security_class_t /*cls*/, char *buf, size_t len) {
    struct audit_data *ad = (struct audit_data *)data;

    if (!ad || !ad->interfaceName) {
        ALOGE("No valid hwservicemanager audit data");
        return 0;
    }

    const char* sid = ad->sid ? ad->sid : "N/A";

    snprintf(buf, len, "interface=%s sid=%s pid=%d", ad->interfaceName, sid, ad->pid);
    return 0;
}

} // namespace android
