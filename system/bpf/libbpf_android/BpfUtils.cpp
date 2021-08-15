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

#define LOG_TAG "BpfUtils"

#include "bpf/BpfUtils.h"

#include <elf.h>
#include <inttypes.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/pfkeyv2.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sstream>
#include <string>

#include <android-base/properties.h>
#include <android-base/unique_fd.h>
#include <log/log.h>
#include <netdutils/MemBlock.h>
#include <netdutils/Slice.h>
#include <processgroup/processgroup.h>

using android::base::GetUintProperty;
using android::base::unique_fd;
using android::netdutils::MemBlock;
using android::netdutils::Slice;

// The buffer size for the buffer that records program loading logs, needs to be large enough for
// the largest kernel program.

namespace android {
namespace bpf {

/*  The bpf_attr is a union which might have a much larger size then the struct we are using, while
 *  The inline initializer only reset the field we are using and leave the reset of the memory as
 *  is. The bpf kernel code will performs a much stricter check to ensure all unused field is 0. So
 *  this syscall will normally fail with E2BIG if we don't do a memset to bpf_attr.
 */
bool operator==(const StatsKey& lhs, const StatsKey& rhs) {
    return ((lhs.uid == rhs.uid) && (lhs.tag == rhs.tag) && (lhs.counterSet == rhs.counterSet) &&
            (lhs.ifaceIndex == rhs.ifaceIndex));
}

bool operator==(const UidTag& lhs, const UidTag& rhs) {
    return ((lhs.uid == rhs.uid) && (lhs.tag == rhs.tag));
}

bool operator==(const StatsValue& lhs, const StatsValue& rhs) {
    return ((lhs.rxBytes == rhs.rxBytes) && (lhs.txBytes == rhs.txBytes) &&
            (lhs.rxPackets == rhs.rxPackets) && (lhs.txPackets == rhs.txPackets));
}

int bpf(int cmd, Slice bpfAttr) {
    return syscall(__NR_bpf, cmd, bpfAttr.base(), bpfAttr.size());
}

int createMap(bpf_map_type map_type, uint32_t key_size, uint32_t value_size, uint32_t max_entries,
              uint32_t map_flags) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_type = map_type;
    attr.key_size = key_size;
    attr.value_size = value_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    return bpf(BPF_MAP_CREATE, Slice(&attr, sizeof(attr)));
}

int writeToMapEntry(const base::unique_fd& map_fd, void* key, void* value, uint64_t flags) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = map_fd.get();
    attr.key = ptr_to_u64(key);
    attr.value = ptr_to_u64(value);
    attr.flags = flags;

    return bpf(BPF_MAP_UPDATE_ELEM, Slice(&attr, sizeof(attr)));
}

int findMapEntry(const base::unique_fd& map_fd, void* key, void* value) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = map_fd.get();
    attr.key = ptr_to_u64(key);
    attr.value = ptr_to_u64(value);

    return bpf(BPF_MAP_LOOKUP_ELEM, Slice(&attr, sizeof(attr)));
}

int deleteMapEntry(const base::unique_fd& map_fd, void* key) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = map_fd.get();
    attr.key = ptr_to_u64(key);

    return bpf(BPF_MAP_DELETE_ELEM, Slice(&attr, sizeof(attr)));
}

int getNextMapKey(const base::unique_fd& map_fd, void* key, void* next_key) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = map_fd.get();
    attr.key = ptr_to_u64(key);
    attr.next_key = ptr_to_u64(next_key);

    return bpf(BPF_MAP_GET_NEXT_KEY, Slice(&attr, sizeof(attr)));
}

int getFirstMapKey(const base::unique_fd& map_fd, void* firstKey) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = map_fd.get();
    attr.key = 0;
    attr.next_key = ptr_to_u64(firstKey);

    return bpf(BPF_MAP_GET_NEXT_KEY, Slice(&attr, sizeof(attr)));
}

int bpfProgLoad(bpf_prog_type prog_type, Slice bpf_insns, const char* license,
                uint32_t kern_version, Slice bpf_log) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.prog_type = prog_type;
    attr.insns = ptr_to_u64(bpf_insns.base());
    attr.insn_cnt = bpf_insns.size() / sizeof(struct bpf_insn);
    attr.license = ptr_to_u64((void*)license);
    attr.log_buf = ptr_to_u64(bpf_log.base());
    attr.log_size = bpf_log.size();
    attr.log_level = DEFAULT_LOG_LEVEL;
    attr.kern_version = kern_version;
    int ret = bpf(BPF_PROG_LOAD, Slice(&attr, sizeof(attr)));

    if (ret < 0) {
        std::string prog_log = netdutils::toString(bpf_log);
        std::istringstream iss(prog_log);
        for (std::string line; std::getline(iss, line);) {
            ALOGE("%s", line.c_str());
        }
    }
    return ret;
}

int bpfFdPin(const base::unique_fd& map_fd, const char* pathname) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.pathname = ptr_to_u64((void*)pathname);
    attr.bpf_fd = map_fd.get();

    return bpf(BPF_OBJ_PIN, Slice(&attr, sizeof(attr)));
}

int bpfFdGet(const char* pathname, uint32_t flag) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.pathname = ptr_to_u64((void*)pathname);
    attr.file_flags = flag;
    return bpf(BPF_OBJ_GET, Slice(&attr, sizeof(attr)));
}

int mapRetrieve(const char* pathname, uint32_t flag) {
    return bpfFdGet(pathname, flag);
}

int attachProgram(bpf_attach_type type, uint32_t prog_fd, uint32_t cg_fd) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.target_fd = cg_fd;
    attr.attach_bpf_fd = prog_fd;
    attr.attach_type = type;

    return bpf(BPF_PROG_ATTACH, Slice(&attr, sizeof(attr)));
}

int detachProgram(bpf_attach_type type, uint32_t cg_fd) {
    bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.target_fd = cg_fd;
    attr.attach_type = type;

    return bpf(BPF_PROG_DETACH, Slice(&attr, sizeof(attr)));
}

uint64_t getSocketCookie(int sockFd) {
    uint64_t sock_cookie;
    socklen_t cookie_len = sizeof(sock_cookie);
    int res = getsockopt(sockFd, SOL_SOCKET, SO_COOKIE, &sock_cookie, &cookie_len);
    if (res < 0) {
        res = -errno;
        ALOGE("Failed to get socket cookie: %s\n", strerror(errno));
        errno = -res;
        // 0 is an invalid cookie. See sock_gen_cookie.
        return NONEXISTENT_COOKIE;
    }
    return sock_cookie;
}

int synchronizeKernelRCU() {
    // This is a temporary hack for network stats map swap on devices running
    // 4.9 kernels. The kernel code of socket release on pf_key socket will
    // explicitly call synchronize_rcu() which is exactly what we need.
    int pfSocket = socket(AF_KEY, SOCK_RAW | SOCK_CLOEXEC, PF_KEY_V2);

    if (pfSocket < 0) {
        int ret = -errno;
        ALOGE("create PF_KEY socket failed: %s", strerror(errno));
        return ret;
    }

    // When closing socket, synchronize_rcu() gets called in sock_release().
    if (close(pfSocket)) {
        int ret = -errno;
        ALOGE("failed to close the PF_KEY socket: %s", strerror(errno));
        return ret;
    }
    return 0;
}

int setrlimitForTest() {
    // Set the memory rlimit for the test process if the default MEMLOCK rlimit is not enough.
    struct rlimit limit = {
            .rlim_cur = TEST_LIMIT,
            .rlim_max = TEST_LIMIT,
    };
    int res = setrlimit(RLIMIT_MEMLOCK, &limit);
    if (res) {
        ALOGE("Failed to set the default MEMLOCK rlimit: %s", strerror(errno));
    }
    return res;
}

std::string BpfLevelToString(BpfLevel bpfLevel) {
    switch (bpfLevel) {
        case BpfLevel::NONE:      return "NONE_SUPPORT";
        case BpfLevel::BASIC:     return "BPF_LEVEL_BASIC";
        case BpfLevel::EXTENDED:  return "BPF_LEVEL_EXTENDED";
        // No default statement. We want to see errors of the form:
        // "enumeration value 'BPF_LEVEL_xxx' not handled in switch [-Werror,-Wswitch]".
    }
}

BpfLevel getBpfSupportLevel() {
    struct utsname buf;
    int kernel_version_major;
    int kernel_version_minor;

    uint64_t api_level = GetUintProperty<uint64_t>("ro.product.first_api_level", 0);
    if (api_level == 0) {
        ALOGE("Cannot determine initial API level of the device");
        api_level = GetUintProperty<uint64_t>("ro.build.version.sdk", 0);
    }

    // Check if the device is shipped originally with android P.
    if (api_level < MINIMUM_API_REQUIRED) return BpfLevel::NONE;

    int ret = uname(&buf);
    if (ret) {
        return BpfLevel::NONE;
    }
    char dummy;
    ret = sscanf(buf.release, "%d.%d%c", &kernel_version_major, &kernel_version_minor, &dummy);
    // Check the device kernel version
    if (ret < 2) return BpfLevel::NONE;
    if (kernel_version_major > 4 || (kernel_version_major == 4 && kernel_version_minor >= 14))
        return BpfLevel::EXTENDED;
    if (kernel_version_major == 4 && kernel_version_minor >= 9) return BpfLevel::BASIC;

    return BpfLevel::NONE;
}

}  // namespace bpf
}  // namespace android
