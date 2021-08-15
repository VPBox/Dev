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

#ifndef NETUTILS_NETLINK_H
#define NETUTILS_NETLINK_H

#include <functional>
#include <ostream>
#include <linux/netlink.h>

#include "netdutils/Slice.h"

namespace android {
namespace netdutils {

// Invoke onMsg once for each netlink message in buf. onMsg will be
// invoked with an aligned and deserialized header along with a Slice
// containing the message payload.
//
// Assume that the first message begins at offset zero within buf.
void forEachNetlinkMessage(const Slice buf,
                           const std::function<void(const nlmsghdr&, const Slice)>& onMsg);

// Invoke onAttr once for each netlink attribute in buf. onAttr will be
// invoked with an aligned and deserialized header along with a Slice
// containing the attribute payload.
//
// Assume that the first attribute begins at offset zero within buf.
void forEachNetlinkAttribute(const Slice buf,
                             const std::function<void(const nlattr&, const Slice)>& onAttr);

}  // namespace netdutils
}  // namespace android

bool operator==(const sockaddr_nl& lhs, const sockaddr_nl& rhs);
bool operator!=(const sockaddr_nl& lhs, const sockaddr_nl& rhs);

std::ostream& operator<<(std::ostream& os, const nlmsghdr& hdr);
std::ostream& operator<<(std::ostream& os, const nlattr& attr);
std::ostream& operator<<(std::ostream& os, const sockaddr_nl& addr);

#endif /* NETUTILS_NETLINK_H */
