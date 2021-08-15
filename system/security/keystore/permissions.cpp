/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "keystore"

#include "permissions.h"

#include <cutils/sockets.h>
#include <log/log.h>
#include <private/android_filesystem_config.h>

#include <selinux/android.h>

#include "keystore_utils.h"

/* perm_labels associcated with keystore_key SELinux class verbs. */
const char* perm_labels[] = {
    "get_state",
    "get",
    "insert",
    "delete",
    "exist",
    "list",
    "reset",
    "password",
    "lock",
    "unlock",
    "is_empty",
    "sign",
    "verify",
    "grant",
    "duplicate",
    "clear_uid",
    "add_auth",
    "user_changed",
    "gen_unique_id",
};

struct user_euid {
    uid_t uid;
    uid_t euid;
};

user_euid user_euids[] = {{AID_VPN, AID_SYSTEM},
                          {AID_WIFI, AID_SYSTEM},
                          {AID_ROOT, AID_SYSTEM},
                          {AID_WIFI, AID_KEYSTORE},
                          {AID_KEYSTORE, AID_WIFI},

#ifdef GRANT_ROOT_ALL_PERMISSIONS
                          // Allow VTS tests to act on behalf of the wifi user
                          {AID_WIFI, AID_ROOT}
#endif
};

struct user_perm {
    uid_t uid;
    perm_t perms;
};

static user_perm user_perms[] = {
    {AID_SYSTEM, static_cast<perm_t>((uint32_t)(~0))},
    {AID_VPN, static_cast<perm_t>(P_GET | P_SIGN | P_VERIFY)},
    {AID_WIFI, static_cast<perm_t>(P_GET | P_SIGN | P_VERIFY)},
    {AID_BLUETOOTH, static_cast<perm_t>(P_GET | P_INSERT | P_DELETE | P_EXIST | P_SIGN | P_VERIFY)},

#ifdef GRANT_ROOT_ALL_PERMISSIONS
    // Allow VTS tests running as root to perform all operations
    {AID_ROOT, static_cast<perm_t>((uint32_t)(~0))},
#else
    {AID_ROOT, static_cast<perm_t>(P_GET)},
#endif
};

static const perm_t DEFAULT_PERMS = static_cast<perm_t>(
    P_GET_STATE | P_GET | P_INSERT | P_DELETE | P_EXIST | P_LIST | P_SIGN | P_VERIFY |
    P_GEN_UNIQUE_ID /* Only privileged apps can do this, but enforcement is done by SELinux */);

struct audit_data {
    pid_t pid;
    uid_t uid;
    const char* sid;
};

const char* get_perm_label(perm_t perm) {
    unsigned int index = ffs(perm);
    if (index > 0 && index <= (sizeof(perm_labels) / sizeof(perm_labels[0]))) {
        return perm_labels[index - 1];
    } else {
        ALOGE("Keystore: Failed to retrieve permission label.\n");
        abort();
    }
}

static int audit_callback(void* data, security_class_t /* cls */, char* buf, size_t len) {
    struct audit_data* ad = reinterpret_cast<struct audit_data*>(data);
    if (!ad) {
        ALOGE("No keystore audit data");
        return 0;
    }

    const char* sid = ad->sid ? ad->sid : "N/A";
    snprintf(buf, len, "pid=%d uid=%d sid=%s", ad->pid, ad->uid, sid);
    return 0;
}

static char* tctx;

int configure_selinux() {
    union selinux_callback cb;
    cb.func_audit = audit_callback;
    selinux_set_callback(SELINUX_CB_AUDIT, cb);
    cb.func_log = selinux_log_callback;
    selinux_set_callback(SELINUX_CB_LOG, cb);
    if (getcon(&tctx) != 0) {
        ALOGE("SELinux: Could not acquire target context. Aborting keystore.\n");
        return -1;
    }

    return 0;
}

static bool keystore_selinux_check_access(uid_t uid, perm_t perm, pid_t spid, const char* ssid) {
    audit_data ad;
    char* sctx = nullptr;
    const char* selinux_class = "keystore_key";
    const char* str_perm = get_perm_label(perm);

    if (!str_perm) {
        return false;
    }

    if (ssid == nullptr && getpidcon(spid, &sctx) != 0) {
        ALOGE("SELinux: Failed to get source pid context.\n");
        return false;
    }

    const char* use_sid = ssid ? ssid : sctx;

    ad.pid = spid;
    ad.uid = uid;
    ad.sid = use_sid;

    bool allowed = selinux_check_access(use_sid, tctx, selinux_class, str_perm,
                                        reinterpret_cast<void*>(&ad)) == 0;
    freecon(sctx);
    return allowed;
}

/**
 * Returns the UID that the callingUid should act as. This is here for
 * legacy support of the WiFi and VPN systems and should be removed
 * when WiFi can operate in its own namespace.
 */
uid_t get_keystore_euid(uid_t uid) {
    for (size_t i = 0; i < sizeof(user_euids) / sizeof(user_euids[0]); i++) {
        struct user_euid user = user_euids[i];
        if (user.uid == uid) {
            return user.euid;
        }
    }

    return uid;
}

bool has_permission(uid_t uid, perm_t perm, pid_t spid, const char* sid) {
    // All system users are equivalent for multi-user support.
    if (get_app_id(uid) == AID_SYSTEM) {
        uid = AID_SYSTEM;
    }

    if (sid == nullptr) {
        android_errorWriteLog(0x534e4554, "121035042");
    }

    for (size_t i = 0; i < sizeof(user_perms) / sizeof(user_perms[0]); i++) {
        struct user_perm user = user_perms[i];
        if (user.uid == uid) {
            return (user.perms & perm) && keystore_selinux_check_access(uid, perm, spid, sid);
        }
    }

    return (DEFAULT_PERMS & perm) && keystore_selinux_check_access(uid, perm, spid, sid);
}

/**
 * Returns true if the callingUid is allowed to interact in the targetUid's
 * namespace.
 */
bool is_granted_to(uid_t callingUid, uid_t targetUid) {
    if (callingUid == targetUid) {
        return true;
    }
    for (size_t i = 0; i < sizeof(user_euids) / sizeof(user_euids[0]); i++) {
        struct user_euid user = user_euids[i];
        if (user.euid == callingUid && user.uid == targetUid) {
            return true;
        }
    }

    return false;
}
