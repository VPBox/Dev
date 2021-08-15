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

#ifndef LOG_TAG
#define LOG_TAG "bpfloader"
#endif

#include <arpa/inet.h>
#include <dirent.h>
#include <elf.h>
#include <error.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/bpf.h>
#include <linux/unistd.h>
#include <net/if.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <libbpf_android.h>
#include <log/log.h>
#include <netdutils/Misc.h>
#include <netdutils/Slice.h>
#include "bpf/BpfUtils.h"

using android::base::EndsWith;
using android::base::unique_fd;
using std::string;

#define BPF_PROG_PATH "/system/etc/bpf/"

#define CLEANANDEXIT(ret, mapPatterns)                    \
    do {                                                  \
        for (size_t i = 0; i < mapPatterns.size(); i++) { \
            if (mapPatterns[i].fd > -1) {                 \
                close(mapPatterns[i].fd);                 \
            }                                             \
        }                                                 \
        return ret;                                       \
    } while (0)

using android::bpf::BpfMapInfo;
using android::bpf::BpfProgInfo;

void loadAllElfObjects(void) {
    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(BPF_PROG_PATH)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string s = ent->d_name;
            if (!EndsWith(s, ".o")) continue;

            string progPath = BPF_PROG_PATH + s;

            int ret = android::bpf::loadProg(progPath.c_str());
            ALOGI("Attempted load object: %s, ret: %s", progPath.c_str(), std::strerror(-ret));
        }
        closedir(dir);
    }
}

int main() {
    std::string value = android::base::GetProperty("bpf.progs_loaded", "");
    if (value == "1") {
        ALOGI("Property bpf.progs_loaded is set, progs already loaded.\n");
        return 0;
    }

    if (android::bpf::getBpfSupportLevel() != android::bpf::BpfLevel::NONE) {
        // Load all ELF objects, create programs and maps, and pin them
        loadAllElfObjects();
    }

    if (android::base::SetProperty("bpf.progs_loaded", "1") == false) {
        ALOGE("Failed to set bpf.progs_loaded property\n");
        return 1;
    }

    return 0;
}
