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

#define LOG_TAG "WakeupController"

#include <arpa/inet.h>
#include <iostream>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_log.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <android-base/strings.h>
#include <android-base/stringprintf.h>
#include <log/log.h>
#include <netdutils/Netfilter.h>
#include <netdutils/Netlink.h>

#include "IptablesRestoreController.h"
#include "NetlinkManager.h"
#include "WakeupController.h"

namespace android {
namespace net {

using base::StringPrintf;
using netdutils::Slice;
using netdutils::Status;

const char WakeupController::LOCAL_MANGLE_INPUT[] = "wakeupctrl_mangle_INPUT";

const uint32_t WakeupController::kDefaultPacketCopyRange =
        sizeof(struct tcphdr) + sizeof(struct ip6_hdr);

static void extractIpPorts(WakeupController::ReportArgs& args, Slice payload) {
    switch (args.ipNextHeader) {
        case IPPROTO_TCP: {
            struct tcphdr header;
            if (extract(payload, header) < sizeof(struct tcphdr)) {
                return;
            }
            args.srcPort = ntohs(header.th_sport);
            args.dstPort = ntohs(header.th_dport);
            break;
        }
        case IPPROTO_UDP: {
            struct udphdr header;
            if (extract(payload, header) < sizeof(struct udphdr)) {
                return;
            }
            args.srcPort = ntohs(header.uh_sport);
            args.dstPort = ntohs(header.uh_dport);
            break;
        }
        default:
            break;
    }
}

static void extractIpHeader(WakeupController::ReportArgs& args, Slice payload) {
    switch (args.ethertype) {
        case ETH_P_IP: {
            struct iphdr header;
            if (extract(payload, header) < sizeof(struct iphdr)) {
                return;
            }
            args.ipNextHeader = header.protocol;
            char addr[INET_ADDRSTRLEN] = {};
            inet_ntop(AF_INET, &header.saddr, addr, sizeof(addr));
            args.srcIp = addr;
            inet_ntop(AF_INET, &header.daddr, addr, sizeof(addr));
            args.dstIp = addr;
            extractIpPorts(args, drop(payload, header.ihl * 4)); // ipv4 IHL counts 32 bit words.
            break;
        }
        case ETH_P_IPV6: {
            struct ip6_hdr header;
            if (extract(payload, header) < sizeof(struct ip6_hdr)) {
                return;
            }
            args.ipNextHeader = header.ip6_nxt;
            char addr[INET6_ADDRSTRLEN] = {};
            inet_ntop(AF_INET6, &header.ip6_src, addr, sizeof(addr));
            args.srcIp = addr;
            inet_ntop(AF_INET6, &header.ip6_dst, addr, sizeof(addr));
            args.dstIp = addr;
            // TODO: also deal with extension headers
            if (args.ipNextHeader == IPPROTO_TCP || args.ipNextHeader == IPPROTO_UDP) {
                extractIpPorts(args, drop(payload, sizeof(header)));
            }
            break;
        }
        default:
            break;
    }
}

WakeupController::~WakeupController() {
    expectOk(mListener->unsubscribe(NetlinkManager::NFLOG_WAKEUP_GROUP));
}

netdutils::Status WakeupController::init(NFLogListenerInterface* listener) {
    mListener = listener;
    const auto msgHandler = [this](const nlmsghdr&, const nfgenmsg&, const Slice msg) {

        struct WakeupController::ReportArgs args = {
            .uid = -1,
            .gid = -1,
            .ethertype = -1,
            .ipNextHeader = -1,
            .srcPort = -1,
            .dstPort = -1,
            // and all other fields set to 0 as the default
        };
        bool parseAgain = false;

        const auto attrHandler = [&args, &parseAgain](const nlattr attr, const Slice payload) {
            switch (attr.nla_type) {
                case NFULA_TIMESTAMP: {
                    timespec ts = {};
                    extract(payload, ts);
                    constexpr uint64_t kNsPerS = 1000000000ULL;
                    args.timestampNs = ntohl(ts.tv_nsec) + (ntohl(ts.tv_sec) * kNsPerS);
                    break;
                }
                case NFULA_PREFIX:
                    // Strip trailing '\0'
                    args.prefix = toString(take(payload, payload.size() - 1));
                    break;
                case NFULA_UID:
                    extract(payload, args.uid);
                    args.uid = ntohl(args.uid);
                    break;
                case NFULA_GID:
                    extract(payload, args.gid);
                    args.gid = ntohl(args.gid);
                    break;
                case NFULA_HWADDR: {
                    struct nfulnl_msg_packet_hw hwaddr = {};
                    extract(payload, hwaddr);
                    size_t hwAddrLen = ntohs(hwaddr.hw_addrlen);
                    hwAddrLen = std::min(hwAddrLen, sizeof(hwaddr.hw_addr));
                    args.dstHw.assign(hwaddr.hw_addr, hwaddr.hw_addr + hwAddrLen);
                    break;
                }
                case NFULA_PACKET_HDR: {
                    struct nfulnl_msg_packet_hdr packetHdr = {};
                    extract(payload, packetHdr);
                    args.ethertype = ntohs(packetHdr.hw_protocol);
                    break;
                }
                case NFULA_PAYLOAD:
                    // The packet payload is expected to come last in the Netlink message.
                    // At that point NFULA_PACKET_HDR has already been parsed and processed.
                    // If this is not the case, set parseAgain to true.
                    parseAgain = (args.ethertype == -1);
                    extractIpHeader(args, payload);
                    break;
                default:
                    break;
            }
        };

        forEachNetlinkAttribute(msg, attrHandler);
        if (parseAgain) {
            // NFULA_PAYLOAD was parsed before NFULA_PACKET_HDR.
            // Now that the ethertype is known, reparse msg for correctly extracting the payload.
            forEachNetlinkAttribute(msg, attrHandler);
        }
        mReport(args);
    };
    return mListener->subscribe(NetlinkManager::NFLOG_WAKEUP_GROUP,
            WakeupController::kDefaultPacketCopyRange, msgHandler);
}

Status WakeupController::addInterface(const std::string& ifName, const std::string& prefix,
                                    uint32_t mark, uint32_t mask) {
    return execIptables("-A", ifName, prefix, mark, mask);
}

Status WakeupController::delInterface(const std::string& ifName, const std::string& prefix,
                                    uint32_t mark, uint32_t mask) {
    return execIptables("-D", ifName, prefix, mark, mask);
}

Status WakeupController::execIptables(const std::string& action, const std::string& ifName,
                                      const std::string& prefix, uint32_t mark, uint32_t mask) {
    // NFLOG messages to batch before releasing to userspace
    constexpr int kBatch = 8;
    // Max log message rate in packets/second
    constexpr int kRateLimit = 10;
    const char kFormat[] =
        "*mangle\n%s %s -i %s -j NFLOG --nflog-prefix %s --nflog-group %d --nflog-threshold %d"
        " -m mark --mark 0x%08x/0x%08x -m limit --limit %d/s\nCOMMIT\n";
    const auto cmd = StringPrintf(
            kFormat, action.c_str(), WakeupController::LOCAL_MANGLE_INPUT, ifName.c_str(),
            prefix.c_str(), NetlinkManager::NFLOG_WAKEUP_GROUP, kBatch, mark, mask, kRateLimit);

    std::string out;
    auto rv = mIptables->execute(V4V6, cmd, &out);
    if (rv != 0) {
        auto s = Status(rv, "Failed to execute iptables cmd: " + cmd + ", out: " + out);
        ALOGE("%s", toString(s).c_str());
        return s;
    }
    return netdutils::status::ok;
}

}  // namespace net
}  // namespace android
