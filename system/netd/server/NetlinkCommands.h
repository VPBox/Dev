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

#ifndef NETD_SERVER_NETLINK_UTIL_H
#define NETD_SERVER_NETLINK_UTIL_H

#include <functional>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include "NetdConstants.h"

namespace android {
namespace net {

const sockaddr_nl KERNEL_NLADDR = {AF_NETLINK, 0, 0, 0};

const uint16_t NETLINK_REQUEST_FLAGS = NLM_F_REQUEST | NLM_F_ACK;
const uint16_t NETLINK_ROUTE_CREATE_FLAGS = NETLINK_REQUEST_FLAGS | NLM_F_CREATE | NLM_F_EXCL;
// Don't create rules with NLM_F_EXCL, because operations such as changing network permissions rely
// on make-before-break. The kernel did not complain about duplicate rules until ~4.9, at which
// point it started returning EEXIST. See for example b/69607866 . We can't just ignore the EEXIST
// because if we hit it, the rule was not created, but we will think it was, and we'll then trip up
// trying to delete it.
const uint16_t NETLINK_RULE_CREATE_FLAGS = NETLINK_REQUEST_FLAGS | NLM_F_CREATE;
const uint16_t NETLINK_DUMP_FLAGS = NLM_F_REQUEST | NLM_F_DUMP;

// Generic code for processing netlink dumps.
const int kNetlinkDumpBufferSize = 8192;
typedef std::function<void(nlmsghdr *)> NetlinkDumpCallback;
typedef std::function<bool(nlmsghdr *)> NetlinkDumpFilter;

// Opens an RTNetlink socket and connects it to the kernel.
WARN_UNUSED_RESULT int openNetlinkSocket(int protocol);

// Receives a netlink ACK. Returns 0 if the command succeeded or negative errno if the command
// failed or receiving the ACK failed.
WARN_UNUSED_RESULT int recvNetlinkAck(int sock);

// Sends a netlink request and possibly expects an ACK. The first element of iov should be null and
// will be set to the netlink message headerheader. The subsequent elements are the contents of the
// request.

// Disable optimizations in ASan build.
// ASan reports an out-of-bounds 32-bit(!) access in the first loop of the
// function (over iov[]).
#ifdef __clang__
#if __has_feature(address_sanitizer)
__attribute__((optnone))
#endif
#endif
WARN_UNUSED_RESULT int sendNetlinkRequest(uint16_t action, uint16_t flags, iovec* iov, int iovlen,
                                          const NetlinkDumpCallback *callback);

// Processes a netlink dump, passing every message to the specified |callback|.
WARN_UNUSED_RESULT int processNetlinkDump(int sock, const NetlinkDumpCallback& callback);

// Flushes netlink objects that take an rtmsg structure (FIB rules, routes...). |getAction| and
// |deleteAction| specify the netlink message types, e.g., RTM_GETRULE and RTM_DELRULE.
// |shouldDelete| specifies whether a given object should be deleted or not. |what| is a
// human-readable name for the objects being flushed, e.g. "rules".
WARN_UNUSED_RESULT int rtNetlinkFlush(uint16_t getAction, uint16_t deleteAction,
                                      const char *what, const NetlinkDumpFilter& shouldDelete);

// Returns the value of the specific __u32 attribute, or 0 if the attribute was not present.
uint32_t getRtmU32Attribute(const nlmsghdr *nlh, int attribute);

}  // namespace net
}  // namespace android

#endif  // NETD_SERVER_NETLINK_UTIL_H
