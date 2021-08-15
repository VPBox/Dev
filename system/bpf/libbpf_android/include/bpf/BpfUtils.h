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

#ifndef BPF_BPFUTILS_H
#define BPF_BPFUTILS_H

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/unistd.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "android-base/unique_fd.h"
#include "netdutils/Slice.h"
#include "netdutils/StatusOr.h"

#define BPF_PASS 1
#define BPF_DROP 0

#define ptr_to_u64(x) ((uint64_t)(uintptr_t)(x))
#define DEFAULT_LOG_LEVEL 1

#define MAP_LD_CMD_HEAD 0x18
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))

// The BPF instruction bytes that we need to replace. x is a placeholder (e.g., COOKIE_TAG_MAP).
#define BPF_MAP_SEARCH_PATTERN(x)                                                               \
    {                                                                                           \
        0x18, 0x01, 0x00, 0x00,                                                                 \
        (x)[0], (x)[1], (x)[2], (x)[3],                                                         \
        0x00, 0x00, 0x00, 0x00,                                                                 \
        (x)[4], (x)[5], (x)[6], (x)[7]                                                          \
    }

// The bytes we'll replace them with. x is the actual fd number for the map at runtime.
// The second byte is changed from 0x01 to 0x11 since 0x11 is the special command used
// for bpf map fd loading. The original 0x01 is only a normal load command.
#define BPF_MAP_REPLACE_PATTERN(x)                                                              \
    {                                                                                           \
        0x18, 0x11, 0x00, 0x00,                                                                 \
        (x)[0], (x)[1], (x)[2], (x)[3],                                                         \
        0x00, 0x00, 0x00, 0x00,                                                                 \
        (x)[4], (x)[5], (x)[6], (x)[7]                                                          \
    }

#define MAP_CMD_SIZE 16

#define TEST_LIMIT 8388608

namespace android {
namespace bpf {

struct UidTag {
    uint32_t uid;
    uint32_t tag;
};

struct StatsKey {
    uint32_t uid;
    uint32_t tag;
    uint32_t counterSet;
    uint32_t ifaceIndex;
};

struct StatsValue {
    uint64_t rxPackets;
    uint64_t rxBytes;
    uint64_t txPackets;
    uint64_t txBytes;
};

struct Stats {
    uint64_t rxBytes;
    uint64_t rxPackets;
    uint64_t txBytes;
    uint64_t txPackets;
    uint64_t tcpRxPackets;
    uint64_t tcpTxPackets;
};

struct IfaceValue {
    char name[IFNAMSIZ];
};

struct BpfProgInfo {
    bpf_attach_type attachType;
    const char* path;
    const char* name;
    bpf_prog_type loadType;
    base::unique_fd fd;
};

int mapRetrieve(const char* pathname, uint32_t flags);

struct BpfMapInfo {
    std::array<uint8_t, MAP_CMD_SIZE> search;
    std::array<uint8_t, MAP_CMD_SIZE> replace;
    const int fd;
    std::string path;

    BpfMapInfo(uint64_t dummyFd, const char* mapPath)
        : BpfMapInfo(dummyFd, android::bpf::mapRetrieve(mapPath, 0)) {}

    BpfMapInfo(uint64_t dummyFd, int realFd, const char* mapPath = "") : fd(realFd), path(mapPath) {
        search = BPF_MAP_SEARCH_PATTERN((uint8_t*)&dummyFd);
        replace = BPF_MAP_REPLACE_PATTERN((uint8_t*)&realFd);
    }
};

enum class BpfLevel {
    // Devices shipped before P or kernel version is lower than 4.9 do not
    // have eBPF enabled.
    NONE,
    // Devices shipped in P with android 4.9 kernel only have the basic eBPF
    // functionalities such as xt_bpf and cgroup skb filter.
    BASIC,
    // For devices that have 4.14 kernel. It supports advanced features like
    // map_in_map and cgroup socket filter.
    EXTENDED,
};

#ifndef DEFAULT_OVERFLOWUID
#define DEFAULT_OVERFLOWUID 65534
#endif

constexpr const int OVERFLOW_COUNTERSET = 2;

constexpr const uint64_t NONEXISTENT_COOKIE = 0;

constexpr const int MINIMUM_API_REQUIRED = 28;

int createMap(bpf_map_type map_type, uint32_t key_size, uint32_t value_size, uint32_t max_entries,
              uint32_t map_flags);
int writeToMapEntry(const base::unique_fd& map_fd, void* key, void* value, uint64_t flags);
int findMapEntry(const base::unique_fd& map_fd, void* key, void* value);
int deleteMapEntry(const base::unique_fd& map_fd, void* key);
int getNextMapKey(const base::unique_fd& map_fd, void* key, void* next_key);
int getFirstMapKey(const base::unique_fd& map_fd, void* firstKey);
int bpfProgLoad(bpf_prog_type prog_type, netdutils::Slice bpf_insns, const char* license,
                uint32_t kern_version, netdutils::Slice bpf_log);
int bpfFdPin(const base::unique_fd& map_fd, const char* pathname);
int bpfFdGet(const char* pathname, uint32_t flags);
int attachProgram(bpf_attach_type type, uint32_t prog_fd, uint32_t cg_fd);
int detachProgram(bpf_attach_type type, uint32_t cg_fd);
uint64_t getSocketCookie(int sockFd);
int setrlimitForTest();
std::string BpfLevelToString(BpfLevel BpfLevel);
BpfLevel getBpfSupportLevel();
int synchronizeKernelRCU();

#define SKIP_IF_BPF_NOT_SUPPORTED                                                    \
    do {                                                                             \
        if (android::bpf::getBpfSupportLevel() == android::bpf::BpfLevel::NONE) {    \
            GTEST_LOG_(INFO) << "This test is skipped since bpf is not available\n"; \
            return;                                                                  \
        }                                                                            \
    } while (0)

#define SKIP_IF_BPF_SUPPORTED                                                           \
    do {                                                                                \
        if (android::bpf::getBpfSupportLevel() != android::bpf::BpfLevel::NONE) return; \
    } while (0)

bool operator==(const StatsValue& lhs, const StatsValue& rhs);
bool operator==(const UidTag& lhs, const UidTag& rhs);
bool operator==(const StatsKey& lhs, const StatsKey& rhs);

}  // namespace bpf
}  // namespace android

#endif
