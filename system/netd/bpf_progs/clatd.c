/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <linux/bpf.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/pkt_cls.h>
#include <linux/swab.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <stdbool.h>
#include <stdint.h>

#include "bpf_helpers.h"
#include "netdbpf/bpf_shared.h"

// bionic/libc/kernel/uapi/linux/udp.h:
//   struct __kernel_udphdr {
// bionic/libc/kernel/tools/defaults.py:
//   # We want to support both BSD and Linux member names in struct udphdr.
//   "udphdr": "__kernel_udphdr",
// so instead it just doesn't work... ugh.
#define udphdr __kernel_udphdr

// From kernel:include/net/ip.h
#define IP_DF 0x4000  // Flag: "Don't Fragment"

// Android only supports little endian architectures
#define htons(x) (__builtin_constant_p(x) ? ___constant_swab16(x) : __builtin_bswap16(x))
#define htonl(x) (__builtin_constant_p(x) ? ___constant_swab32(x) : __builtin_bswap32(x))
#define ntohs(x) htons(x)
#define ntohl(x) htonl(x)

DEFINE_BPF_MAP(clat_ingress_map, HASH, ClatIngressKey, ClatIngressValue, 16)

static inline __always_inline int nat64(struct __sk_buff* skb, bool is_ethernet) {
    const int l2_header_size = is_ethernet ? sizeof(struct ethhdr) : 0;
    void* data = (void*)(long)skb->data;
    const void* data_end = (void*)(long)skb->data_end;
    const struct ethhdr* const eth = is_ethernet ? data : NULL;  // used iff is_ethernet
    const struct ipv6hdr* const ip6 = is_ethernet ? (void*)(eth + 1) : data;
    const struct tcphdr* const tcp = (void*)(ip6 + 1);
    const struct udphdr* const udp = (void*)(ip6 + 1);

    // Must be meta-ethernet IPv6 frame
    if (skb->protocol != htons(ETH_P_IPV6)) return TC_ACT_OK;

    // Must have (ethernet and) ipv6 header
    if (data + l2_header_size + sizeof(*ip6) > data_end) return TC_ACT_OK;

    // Ethertype - if present - must be IPv6
    if (is_ethernet && (eth->h_proto != htons(ETH_P_IPV6))) return TC_ACT_OK;

    // IP version must be 6
    if (ip6->version != 6) return TC_ACT_OK;

    // Maximum IPv6 payload length that can be translated to IPv4
    if (ntohs(ip6->payload_len) > 0xFFFF - sizeof(struct iphdr)) return TC_ACT_OK;

    switch (ip6->nexthdr) {
        case IPPROTO_TCP:  // If TCP, must have 20 byte minimal TCP header
            if (tcp + 1 > (struct tcphdr*)data_end) return TC_ACT_OK;
            break;

        case IPPROTO_UDP:  // If UDP, must have 8 byte minimal UDP header
            if (udp + 1 > (struct udphdr*)data_end) return TC_ACT_OK;
            break;

        default:  // do not know how to handle anything else
            return TC_ACT_OK;
    }

    ClatIngressKey k = {
            .iif = skb->ifindex,
            .pfx96.in6_u.u6_addr32 =
                    {
                            ip6->saddr.in6_u.u6_addr32[0],
                            ip6->saddr.in6_u.u6_addr32[1],
                            ip6->saddr.in6_u.u6_addr32[2],
                    },
            .local6 = ip6->daddr,
    };

    ClatIngressValue* v = bpf_clat_ingress_map_lookup_elem(&k);

    if (!v) return TC_ACT_OK;

    struct ethhdr eth2;  // used iff is_ethernet
    if (is_ethernet) {
        eth2 = *eth;                     // Copy over the ethernet header (src/dst mac)
        eth2.h_proto = htons(ETH_P_IP);  // But replace the ethertype
    }

    struct iphdr ip = {
            .version = 4,                                                      // u4
            .ihl = sizeof(struct iphdr) / sizeof(__u32),                       // u4
            .tos = (ip6->priority << 4) + (ip6->flow_lbl[0] >> 4),             // u8
            .tot_len = htons(ntohs(ip6->payload_len) + sizeof(struct iphdr)),  // u16
            .id = 0,                                                           // u16
            .frag_off = htons(IP_DF),                                          // u16
            .ttl = ip6->hop_limit,                                             // u8
            .protocol = ip6->nexthdr,                                          // u8
            .check = 0,                                                        // u16
            .saddr = ip6->saddr.in6_u.u6_addr32[3],                            // u32
            .daddr = v->local4.s_addr,                                         // u32
    };

    // Calculate the IPv4 one's complement checksum of the IPv4 header.
    __u32 sum = 0;
    for (int i = 0; i < sizeof(ip) / sizeof(__u16); ++i) {
        sum += ((__u16*)&ip)[i];
    }
    // Note that sum is guaranteed to be non-zero by virtue of ip.version == 4
    sum = (sum & 0xFFFF) + (sum >> 16);  // collapse u32 into range 1 .. 0x1FFFE
    sum = (sum & 0xFFFF) + (sum >> 16);  // collapse any potential carry into u16
    ip.check = (__u16)~sum;              // sum cannot be zero, so this is never 0xFFFF

    // Note that there is no L4 checksum update: we are relying on the checksum neutrality
    // of the ipv6 address chosen by netd's ClatdController.

    // Packet mutations begin - point of no return.
    if (bpf_skb_change_proto(skb, htons(ETH_P_IP), 0)) return TC_ACT_SHOT;

    // bpf_skb_change_proto() invalidates all pointers - reload them.
    data = (void*)(long)skb->data;
    data_end = (void*)(long)skb->data_end;

    // I cannot think of any valid way for this error condition to trigger, however I do
    // believe the explicit check is required to keep the in kernel ebpf verifier happy.
    if (data + l2_header_size + sizeof(struct iphdr) > data_end) return TC_ACT_SHOT;

    if (is_ethernet) {
        struct ethhdr* new_eth = data;

        // Copy over the updated ethernet header
        *new_eth = eth2;

        // Copy over the new ipv4 header.
        *(struct iphdr*)(new_eth + 1) = ip;
    } else {
        // Copy over the new ipv4 header without an ethernet header.
        *(struct iphdr*)data = ip;
    }

    // Redirect, possibly back to same interface, so tcpdump sees packet twice.
    if (v->oif) return bpf_redirect(v->oif, BPF_F_INGRESS);

    // Just let it through, tcpdump will not see IPv4 packet.
    return TC_ACT_OK;
}

SEC("schedcls/ingress/clat_ether")
int sched_cls_ingress_clat_ether(struct __sk_buff* skb) {
    return nat64(skb, true);
}

SEC("schedcls/ingress/clat_rawip")
int sched_cls_ingress_clat_rawip(struct __sk_buff* skb) {
    return nat64(skb, false);
}

char _license[] SEC("license") = "Apache 2.0";
