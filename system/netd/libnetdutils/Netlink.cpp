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

#include <ios>
#include <linux/netlink.h>

#include "netdutils/Math.h"
#include "netdutils/Netlink.h"

namespace android {
namespace netdutils {

void forEachNetlinkMessage(const Slice buf,
                           const std::function<void(const nlmsghdr&, const Slice)>& onMsg) {
    Slice tail = buf;
    while (tail.size() >= sizeof(nlmsghdr)) {
        nlmsghdr hdr = {};
        extract(tail, hdr);
        const auto len = std::max<size_t>(hdr.nlmsg_len, sizeof(hdr));
        onMsg(hdr, drop(take(tail, len), sizeof(hdr)));
        tail = drop(tail, align(len, 2));
    }
}

void forEachNetlinkAttribute(const Slice buf,
                             const std::function<void(const nlattr&, const Slice)>& onAttr) {
    Slice tail = buf;
    while (tail.size() >= sizeof(nlattr)) {
        nlattr hdr = {};
        extract(tail, hdr);
        const auto len = std::max<size_t>(hdr.nla_len, sizeof(hdr));
        onAttr(hdr, drop(take(tail, len), sizeof(hdr)));
        tail = drop(tail, align(len, 2));
    }
}

}  // namespace netdutils
}  // namespace android

bool operator==(const sockaddr_nl& lhs, const sockaddr_nl& rhs) {
    return (lhs.nl_family == rhs.nl_family) && (lhs.nl_pid == rhs.nl_pid) &&
           (lhs.nl_groups == rhs.nl_groups);
}

bool operator!=(const sockaddr_nl& lhs, const sockaddr_nl& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const nlmsghdr& hdr) {
    return os << std::hex << "nlmsghdr["
              << "len: 0x" << hdr.nlmsg_len << ", type: 0x" << hdr.nlmsg_type << ", flags: 0x"
              << hdr.nlmsg_flags << ", seq: 0x" << hdr.nlmsg_seq << ", pid: 0x" << hdr.nlmsg_pid
              << "]" << std::dec;
}

std::ostream& operator<<(std::ostream& os, const nlattr& attr) {
    return os << std::hex << "nlattr["
              << "len: 0x" << attr.nla_len << ", type: 0x" << attr.nla_type << "]" << std::dec;
}

std::ostream& operator<<(std::ostream& os, const sockaddr_nl& addr) {
    return os << std::hex << "sockaddr_nl["
              << "family: " << addr.nl_family << ", pid: " << addr.nl_pid
              << ", groups: " << addr.nl_groups << "]" << std::dec;
}
