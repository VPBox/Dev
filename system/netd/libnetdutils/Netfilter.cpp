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

#include <arpa/inet.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netlink.h>
#include <ios>

#include "netdutils/Netfilter.h"

std::ostream& operator<<(std::ostream& os, const nfgenmsg& msg) {
    return os << std::hex << "nfgenmsg["
              << "family: 0x" << static_cast<int>(msg.nfgen_family) << ", version: 0x"
              << static_cast<int>(msg.version) << ", res_id: 0x" << ntohs(msg.res_id) << "]"
              << std::dec;
}
