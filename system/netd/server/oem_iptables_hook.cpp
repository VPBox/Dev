/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <string>

#define LOG_TAG "OemIptablesHook"
#include <log/log.h>
#include <logwrap/logwrap.h>
#include "NetdConstants.h"

namespace {

const char OEM_SCRIPT_PATH[] = "/system/bin/oem-iptables-init.sh";

bool oemCleanupHooks() {
    static const std::string cmd4 =
            "*filter\n"
            ":oem_out -\n"
            ":oem_fwd -\n"
            "COMMIT\n"
            "*nat\n"
            ":oem_nat_pre -\n"
            "COMMIT\n";

    static const std::string cmd6 =
            "*filter\n"
            ":oem_out -\n"
            ":oem_fwd -\n"
            "COMMIT\n";

    return (execIptablesRestore(V4, cmd4) == 0 && execIptablesRestore(V6, cmd6) == 0);
}

bool oemInitChains() {
    int ret = system(OEM_SCRIPT_PATH);  // NOLINT(cert-env33-c)
    if ((-1 == ret) || (0 != WEXITSTATUS(ret))) {
        ALOGE("%s failed: %s", OEM_SCRIPT_PATH, strerror(errno));
        oemCleanupHooks();
        return false;
    }
    return true;
}

}  // namespace

void setupOemIptablesHook() {
    if (0 == access(OEM_SCRIPT_PATH, R_OK | X_OK)) {
        // The call to oemCleanupHooks() is superfluous when done on bootup,
        // but is needed for the case where netd has crashed/stopped and is
        // restarted.
        if (oemCleanupHooks() && oemInitChains()) {
            ALOGI("OEM iptable hook installed.");
        }
    }
}
