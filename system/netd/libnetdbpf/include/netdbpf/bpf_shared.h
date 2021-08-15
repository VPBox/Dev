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

#ifndef NETDBPF_BPF_SHARED_H
#define NETDBPF_BPF_SHARED_H

#include <linux/in.h>
#include <linux/in6.h>
#include <netdutils/UidConstants.h>
// const values shared by bpf kernel program bpfloader and netd


// Since we cannot garbage collect the stats map since device boot, we need to make these maps as
// large as possible. The maximum size of number of map entries we can have is depend on the rlimit
// of MEM_LOCK granted to netd. The memory space needed by each map can be calculated by the
// following fomula:
//      elem_size = 40 + roundup(key_size, 8) + roundup(value_size, 8)
//      cost = roundup_pow_of_two(max_entries) * 16 + elem_size * max_entries +
//              elem_size * number_of_CPU
// And the cost of each map currently used is(assume the device have 8 CPUs):
// cookie_tag_map:      key:  8 bytes, value:  8 bytes, cost:  822592 bytes    =   823Kbytes
// uid_counter_set_map: key:  4 bytes, value:  1 bytes, cost:  145216 bytes    =   145Kbytes
// app_uid_stats_map:   key:  4 bytes, value: 32 bytes, cost: 1062784 bytes    =  1063Kbytes
// uid_stats_map:       key: 16 bytes, value: 32 bytes, cost: 1142848 bytes    =  1143Kbytes
// tag_stats_map:       key: 16 bytes, value: 32 bytes, cost: 1142848 bytes    =  1143Kbytes
// iface_index_name_map:key:  4 bytes, value: 16 bytes, cost:   80896 bytes    =    81Kbytes
// iface_stats_map:     key:  4 bytes, value: 32 bytes, cost:   97024 bytes    =    97Kbytes
// dozable_uid_map:     key:  4 bytes, value:  1 bytes, cost:  145216 bytes    =   145Kbytes
// standby_uid_map:     key:  4 bytes, value:  1 bytes, cost:  145216 bytes    =   145Kbytes
// powersave_uid_map:   key:  4 bytes, value:  1 bytes, cost:  145216 bytes    =   145Kbytes
// total:                                                                         4930Kbytes
// It takes maximum 4.9MB kernel memory space if all maps are full, which requires any devices
// running this module to have a memlock rlimit to be larger then 5MB. In the old qtaguid module,
// we don't have a total limit for data entries but only have limitation of tags each uid can have.
// (default is 1024 in kernel);

const int COOKIE_UID_MAP_SIZE = 10000;
const int UID_COUNTERSET_MAP_SIZE = 2000;
const int APP_STATS_MAP_SIZE = 10000;
const int STATS_MAP_SIZE = 5000;
const int IFACE_INDEX_NAME_MAP_SIZE = 1000;
const int IFACE_STATS_MAP_SIZE = 1000;
const int CONFIGURATION_MAP_SIZE = 2;
const int UID_OWNER_MAP_SIZE = 2000;

#define BPF_PATH "/sys/fs/bpf"

#define BPF_EGRESS_PROG_PATH BPF_PATH "/prog_netd_cgroupskb_egress_stats"
#define BPF_INGRESS_PROG_PATH BPF_PATH "/prog_netd_cgroupskb_ingress_stats"
#define XT_BPF_INGRESS_PROG_PATH BPF_PATH "/prog_netd_skfilter_ingress_xtbpf"
#define XT_BPF_EGRESS_PROG_PATH BPF_PATH "/prog_netd_skfilter_egress_xtbpf"
#define XT_BPF_WHITELIST_PROG_PATH BPF_PATH "/prog_netd_skfilter_whitelist_xtbpf"
#define XT_BPF_BLACKLIST_PROG_PATH BPF_PATH "/prog_netd_skfilter_blacklist_xtbpf"
#define CGROUP_SOCKET_PROG_PATH BPF_PATH "/prog_netd_cgroupsock_inet_create"

#define COOKIE_TAG_MAP_PATH BPF_PATH "/map_netd_cookie_tag_map"
#define UID_COUNTERSET_MAP_PATH BPF_PATH "/map_netd_uid_counterset_map"
#define APP_UID_STATS_MAP_PATH BPF_PATH "/map_netd_app_uid_stats_map"
#define STATS_MAP_A_PATH BPF_PATH "/map_netd_stats_map_A"
#define STATS_MAP_B_PATH BPF_PATH "/map_netd_stats_map_B"
#define IFACE_INDEX_NAME_MAP_PATH BPF_PATH "/map_netd_iface_index_name_map"
#define IFACE_STATS_MAP_PATH BPF_PATH "/map_netd_iface_stats_map"
#define CONFIGURATION_MAP_PATH BPF_PATH "/map_netd_configuration_map"
#define UID_OWNER_MAP_PATH BPF_PATH "/map_netd_uid_owner_map"
#define UID_PERMISSION_MAP_PATH BPF_PATH "/map_netd_uid_permission_map"

enum UidOwnerMatchType {
    NO_MATCH = 0,
    HAPPY_BOX_MATCH = (1 << 0),
    PENALTY_BOX_MATCH = (1 << 1),
    DOZABLE_MATCH = (1 << 2),
    STANDBY_MATCH = (1 << 3),
    POWERSAVE_MATCH = (1 << 4),
    IIF_MATCH = (1 << 5),
};

enum BpfPemissionMatch {
    BPF_PERMISSION_INTERNET = 1 << 2,
    BPF_PERMISSION_UPDATE_DEVICE_STATS = 1 << 3,
};
// In production we use two identical stats maps to record per uid stats and
// do swap and clean based on the configuration specified here. The statsMapType
// value in configuration map specified which map is currently in use.
enum StatsMapType {
    SELECT_MAP_A,
    SELECT_MAP_B,
};

// TODO: change the configuration object from an 8-bit bitmask to an object with clearer
// semantics, like a struct.
typedef uint8_t BpfConfig;
const BpfConfig DEFAULT_CONFIG = 0;

typedef struct {
    // Allowed interface index. Only applicable if IIF_MATCH is set in the rule bitmask above.
    uint32_t iif;
    // A bitmask of enum values in UidOwnerMatchType.
    uint8_t rule;
} UidOwnerValue;

#define UID_RULES_CONFIGURATION_KEY 1
#define CURRENT_STATS_MAP_CONFIGURATION_KEY 2

#define CLAT_INGRESS_PROG_RAWIP_NAME "prog_clatd_schedcls_ingress_clat_rawip"
#define CLAT_INGRESS_PROG_ETHER_NAME "prog_clatd_schedcls_ingress_clat_ether"

#define CLAT_INGRESS_PROG_RAWIP_PATH BPF_PATH "/" CLAT_INGRESS_PROG_RAWIP_NAME
#define CLAT_INGRESS_PROG_ETHER_PATH BPF_PATH "/" CLAT_INGRESS_PROG_ETHER_NAME

#define CLAT_INGRESS_MAP_PATH BPF_PATH "/map_clatd_clat_ingress_map"

typedef struct {
    uint32_t iif;            // The input interface index
    struct in6_addr pfx96;   // The source /96 nat64 prefix, bottom 32 bits must be 0
    struct in6_addr local6;  // The full 128-bits of the destination IPv6 address
} ClatIngressKey;

typedef struct {
    uint32_t oif;           // The output interface to redirect to (0 means don't redirect)
    struct in_addr local4;  // The destination IPv4 address
} ClatIngressValue;

#endif  // NETDBPF_BPF_SHARED_H
