/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef _BPF_NETWORKSTATS_H
#define _BPF_NETWORKSTATS_H

#include <bpf/BpfMap.h>

namespace android {
namespace bpf {

// TODO: set this to a proper value based on the map size;
constexpr int TAG_STATS_MAP_SOFT_LIMIT = 3;
constexpr int UID_ALL = -1;
constexpr int TAG_ALL = -1;
constexpr int TAG_NONE = 0;
constexpr int SET_ALL = -1;
constexpr int SET_DEFAULT = 0;
constexpr int SET_FOREGROUND = 1;

// The limit for stats received by a unknown interface;
constexpr const int64_t MAX_UNKNOWN_IFACE_BYTES = 100 * 1000;

// This is used by
// frameworks/base/core/jni/com_android_internal_net_NetworkStatsFactory.cpp
// make sure it is consistent with the JNI code before changing this.
struct stats_line {
    char iface[32];
    uint32_t uid;
    uint32_t set;
    uint32_t tag;
    int64_t rxBytes;
    int64_t rxPackets;
    int64_t txBytes;
    int64_t txPackets;

    stats_line& operator=(const stats_line& rhs);
    stats_line& operator+=(const stats_line& rhs);
};

bool operator==(const stats_line& lhs, const stats_line& rhs);
bool operator<(const stats_line& lhs, const stats_line& rhs);

// For test only
int bpfGetUidStatsInternal(uid_t uid, struct Stats* stats,
                           const BpfMap<uint32_t, StatsValue>& appUidStatsMap);
// For test only
int bpfGetIfaceStatsInternal(const char* iface, Stats* stats,
                             const BpfMap<uint32_t, StatsValue>& ifaceStatsMap,
                             const BpfMap<uint32_t, IfaceValue>& ifaceNameMap);
// For test only
int parseBpfNetworkStatsDetailInternal(std::vector<stats_line>* lines,
                                       const std::vector<std::string>& limitIfaces, int limitTag,
                                       int limitUid, const BpfMap<StatsKey, StatsValue>& statsMap,
                                       const BpfMap<uint32_t, IfaceValue>& ifaceMap);
// For test only
int cleanStatsMapInternal(const base::unique_fd& cookieTagMap, const base::unique_fd& tagStatsMap);
// For test only
template <class Key>
int getIfaceNameFromMap(const BpfMap<uint32_t, IfaceValue>& ifaceMap,
                        const BpfMap<Key, StatsValue>& statsMap, uint32_t ifaceIndex, char* ifname,
                        const Key& curKey, int64_t* unknownIfaceBytesTotal) {
    auto iface = ifaceMap.readValue(ifaceIndex);
    if (!isOk(iface)) {
        maybeLogUnknownIface(ifaceIndex, statsMap, curKey, unknownIfaceBytesTotal);
        return -ENODEV;
    }
    strlcpy(ifname, iface.value().name, sizeof(IfaceValue));
    return 0;
}

template <class Key>
void maybeLogUnknownIface(int ifaceIndex, const BpfMap<Key, StatsValue>& statsMap,
                          const Key& curKey, int64_t* unknownIfaceBytesTotal) {
    // Have we already logged an error?
    if (*unknownIfaceBytesTotal == -1) {
        return;
    }

    // Are we undercounting enough data to be worth logging?
    auto statsEntry = statsMap.readValue(curKey);
    if (!netdutils::isOk(statsEntry)) {
        // No data is being undercounted.
        return;
    }

    *unknownIfaceBytesTotal += (statsEntry.value().rxBytes + statsEntry.value().txBytes);
    if (*unknownIfaceBytesTotal >= MAX_UNKNOWN_IFACE_BYTES) {
        ALOGE("Unknown name for ifindex %d with more than %" PRId64 " bytes of traffic", ifaceIndex,
              *unknownIfaceBytesTotal);
        *unknownIfaceBytesTotal = -1;
    }
}

// For test only
int parseBpfNetworkStatsDevInternal(std::vector<stats_line>* lines,
                                    const BpfMap<uint32_t, StatsValue>& statsMap,
                                    const BpfMap<uint32_t, IfaceValue>& ifaceMap);

int bpfGetUidStats(uid_t uid, struct Stats* stats);
int bpfGetIfaceStats(const char* iface, struct Stats* stats);
int parseBpfNetworkStatsDetail(std::vector<stats_line>* lines,
                               const std::vector<std::string>& limitIfaces, int limitTag,
                               int limitUid);

int parseBpfNetworkStatsDev(std::vector<stats_line>* lines);
void groupNetworkStats(std::vector<stats_line>* lines);
int cleanStatsMap();
}  // namespace bpf
}  // namespace android

#endif  // _BPF_NETWORKSTATS_H
