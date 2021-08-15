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

#include "netd.h"
#include <linux/bpf.h>
#include <linux/if_packet.h>

SEC("cgroupskb/ingress/stats")
int bpf_cgroup_ingress(struct __sk_buff* skb) {
    return bpf_traffic_account(skb, BPF_INGRESS);
}

SEC("cgroupskb/egress/stats")
int bpf_cgroup_egress(struct __sk_buff* skb) {
    return bpf_traffic_account(skb, BPF_EGRESS);
}

SEC("skfilter/egress/xtbpf")
int xt_bpf_egress_prog(struct __sk_buff* skb) {
    uint32_t key = skb->ifindex;
    update_iface_stats_map(skb, BPF_EGRESS, &key);
    return BPF_MATCH;
}

SEC("skfilter/ingress/xtbpf")
int xt_bpf_ingress_prog(struct __sk_buff* skb) {
    uint32_t key = skb->ifindex;
    update_iface_stats_map(skb, BPF_INGRESS, &key);
    return BPF_MATCH;
}

SEC("skfilter/whitelist/xtbpf")
int xt_bpf_whitelist_prog(struct __sk_buff* skb) {
    uint32_t sock_uid = bpf_get_socket_uid(skb);
    if (is_system_uid(sock_uid)) return BPF_MATCH;

    // 65534 is the overflow 'nobody' uid, usually this being returned means
    // that skb->sk is NULL during RX (early decap socket lookup failure),
    // which commonly happens for incoming packets to an unconnected udp socket.
    // Additionally bpf_get_socket_cookie() returns 0 if skb->sk is NULL
    if ((sock_uid == 65534) && !bpf_get_socket_cookie(skb) &&
        (skb->pkt_type == PACKET_HOST || skb->pkt_type == PACKET_BROADCAST ||
         skb->pkt_type == PACKET_MULTICAST))
        return BPF_MATCH;

    UidOwnerValue* whitelistMatch = bpf_uid_owner_map_lookup_elem(&sock_uid);
    if (whitelistMatch) return whitelistMatch->rule & HAPPY_BOX_MATCH ? BPF_MATCH : BPF_NOMATCH;
    return BPF_NOMATCH;
}

SEC("skfilter/blacklist/xtbpf")
int xt_bpf_blacklist_prog(struct __sk_buff* skb) {
    uint32_t sock_uid = bpf_get_socket_uid(skb);
    UidOwnerValue* blacklistMatch = bpf_uid_owner_map_lookup_elem(&sock_uid);
    if (blacklistMatch) return blacklistMatch->rule & PENALTY_BOX_MATCH ? BPF_MATCH : BPF_NOMATCH;
    return BPF_NOMATCH;
}

DEFINE_BPF_MAP(uid_permission_map, HASH, uint32_t, uint8_t, UID_OWNER_MAP_SIZE)

SEC("cgroupsock/inet/create")
int inet_socket_create(struct bpf_sock* sk) {
    uint64_t gid_uid = bpf_get_current_uid_gid();
    /*
     * A given app is guaranteed to have the same app ID in all the profiles in
     * which it is installed, and install permission is granted to app for all
     * user at install time so we only check the appId part of a request uid at
     * run time. See UserHandle#isSameApp for detail.
     */
    uint32_t appId = (gid_uid & 0xffffffff) % PER_USER_RANGE;
    uint8_t* permissions = bpf_uid_permission_map_lookup_elem(&appId);
    if (!permissions) {
        // UID not in map. Default to just INTERNET permission.
        return 1;
    }

    // A return value of 1 means allow, everything else means deny.
    return (*permissions & BPF_PERMISSION_INTERNET) == BPF_PERMISSION_INTERNET;
}

char _license[] SEC("license") = "Apache 2.0";
