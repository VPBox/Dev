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

#define LOG_TAG "NFLogListener"

#include <sstream>
#include <vector>

#include <arpa/inet.h>
#include <linux/netfilter/nfnetlink_log.h>

#include <log/log.h>
#include <netdutils/Misc.h>
#include <netdutils/Netfilter.h>
#include <netdutils/Syscalls.h>

#include "NFLogListener.h"

namespace android {
namespace net {

using netdutils::extract;
using netdutils::findWithDefault;
using netdutils::makeSlice;
using netdutils::Slice;
using netdutils::sSyscalls;
using netdutils::Status;
using netdutils::StatusOr;
using netdutils::status::ok;

constexpr int kNFLogConfigMsgType = (NFNL_SUBSYS_ULOG << 8) | NFULNL_MSG_CONFIG;
constexpr int kNFLogPacketMsgType = (NFNL_SUBSYS_ULOG << 8) | NFULNL_MSG_PACKET;
constexpr int kNetlinkDoneMsgType = (NFNL_SUBSYS_NONE << 8) | NLMSG_DONE;
constexpr size_t kDefaultPacketRange = 0;

namespace {

const NFLogListener::DispatchFn kDefaultDispatchFn = [](const nlmsghdr& nlmsg,
                                                        const nfgenmsg& nfmsg, const Slice msg) {
    std::stringstream ss;
    ss << nlmsg << " " << nfmsg << " " << msg << " " << netdutils::toHex(msg, 32);
    ALOGE("unhandled nflog message: %s", ss.str().c_str());
};

using SendFn = std::function<Status(const Slice msg)>;

// Required incantation?
Status cfgCmdPfUnbind(const SendFn& send) {
    struct {
        nlmsghdr nlhdr;
        nfgenmsg nfhdr;
        nfattr attr;
        nfulnl_msg_config_cmd cmd;
    } __attribute__((packed)) msg = {};

    msg.nlhdr.nlmsg_len = sizeof(msg);
    msg.nlhdr.nlmsg_type = kNFLogConfigMsgType;
    msg.nlhdr.nlmsg_flags = NLM_F_REQUEST;
    msg.nfhdr.nfgen_family = AF_UNSPEC;
    msg.attr.nfa_len = sizeof(msg.attr) + sizeof(msg.cmd);
    msg.attr.nfa_type = NFULA_CFG_CMD;
    msg.cmd.command = NFULNL_CFG_CMD_PF_UNBIND;
    return send(makeSlice(msg));
}

// Control delivery mode for NFLOG messages marked with nfLogGroup.
// range controls maximum bytes to copy
// mode must be one of: NFULNL_COPY_NONE, NFULNL_COPY_META, NFULNL_COPY_PACKET
Status cfgMode(const SendFn& send, uint16_t nfLogGroup, uint32_t range, uint8_t mode) {
    struct {
        nlmsghdr nlhdr;
        nfgenmsg nfhdr;
        nfattr attr;
        nfulnl_msg_config_mode mode;
    } __attribute__((packed)) msg = {};

    msg.nlhdr.nlmsg_len = sizeof(msg);
    msg.nlhdr.nlmsg_type = kNFLogConfigMsgType;
    msg.nlhdr.nlmsg_flags = NLM_F_REQUEST;
    msg.nfhdr.nfgen_family = AF_UNSPEC;
    msg.nfhdr.res_id = htons(nfLogGroup);
    msg.attr.nfa_len = sizeof(msg.attr) + sizeof(msg.mode);
    msg.attr.nfa_type = NFULA_CFG_MODE;
    msg.mode.copy_mode = mode;
    msg.mode.copy_range = htonl(range);
    return send(makeSlice(msg));
}

// Request that NFLOG messages marked with nfLogGroup are delivered to this socket
Status cfgCmdBind(const SendFn& send, uint16_t nfLogGroup) {
    struct {
        nlmsghdr nlhdr;
        nfgenmsg nfhdr;
        nfattr attr;
        nfulnl_msg_config_cmd cmd;
    } __attribute__((packed)) msg = {};

    msg.nlhdr.nlmsg_len = sizeof(msg);
    msg.nlhdr.nlmsg_type = kNFLogConfigMsgType;
    msg.nlhdr.nlmsg_flags = NLM_F_REQUEST;
    msg.nfhdr.nfgen_family = AF_UNSPEC;
    msg.nfhdr.res_id = htons(nfLogGroup);
    msg.attr.nfa_len = sizeof(msg.attr) + sizeof(msg.cmd);
    msg.attr.nfa_type = NFULA_CFG_CMD;
    msg.cmd.command = NFULNL_CFG_CMD_BIND;
    return send(makeSlice(msg));
}

// Request that NFLOG messages marked with nfLogGroup are not delivered to this socket
Status cfgCmdUnbind(const SendFn& send, uint16_t nfLogGroup) {
    struct {
        nlmsghdr nlhdr;
        nfgenmsg nfhdr;
        nfattr attr;
        nfulnl_msg_config_cmd cmd;
    } __attribute__((packed)) msg = {};

    msg.nlhdr.nlmsg_len = sizeof(msg);
    msg.nlhdr.nlmsg_type = kNFLogConfigMsgType;
    msg.nlhdr.nlmsg_flags = NLM_F_REQUEST;
    msg.nfhdr.nfgen_family = AF_UNSPEC;
    msg.nfhdr.res_id = htons(nfLogGroup);
    msg.attr.nfa_len = sizeof(msg.attr) + sizeof(msg.cmd);
    msg.attr.nfa_type = NFULA_CFG_CMD;
    msg.cmd.command = NFULNL_CFG_CMD_UNBIND;
    return send(makeSlice(msg));
}

}  // namespace

NFLogListener::NFLogListener(std::shared_ptr<NetlinkListenerInterface> listener)
    : mListener(std::move(listener)) {
    // Rx handler extracts nfgenmsg looks up and invokes registered dispatch function.
    const auto rxHandler = [this](const nlmsghdr& nlmsg, const Slice msg) {
        nfgenmsg nfmsg = {};
        extract(msg, nfmsg);
        std::lock_guard guard(mMutex);
        const auto& fn = findWithDefault(mDispatchMap, ntohs(nfmsg.res_id), kDefaultDispatchFn);
        fn(nlmsg, nfmsg, drop(msg, sizeof(nfmsg)));
    };
    expectOk(mListener->subscribe(kNFLogPacketMsgType, rxHandler));

    // Each batch of NFLOG messages is terminated with NLMSG_DONE which is useless to us
    const auto rxDoneHandler = [](const nlmsghdr&, const Slice msg) {
        // Ignore NLMSG_DONE  messages
        nfgenmsg nfmsg = {};
        extract(msg, nfmsg);
        // TODO: why is nfmsg filled with garbage?
    };
    expectOk(mListener->subscribe(kNetlinkDoneMsgType, rxDoneHandler));
}

NFLogListener::~NFLogListener() {
    expectOk(mListener->unsubscribe(kNFLogPacketMsgType));
    expectOk(mListener->unsubscribe(kNetlinkDoneMsgType));
    const auto sendFn = [this](const Slice msg) { return mListener->send(msg); };
    for (const auto& [key, value] : mDispatchMap) {
        expectOk(cfgCmdUnbind(sendFn, key));
    }
}

Status NFLogListener::subscribe(uint16_t nfLogGroup, const DispatchFn& fn) {
    return subscribe(nfLogGroup, kDefaultPacketRange, fn);
}

Status NFLogListener::subscribe(
        uint16_t nfLogGroup, uint32_t copyRange, const DispatchFn& fn) {
    const auto sendFn = [this](const Slice msg) { return mListener->send(msg); };
    // Install fn into the dispatch map BEFORE requesting delivery of messages
    {
        std::lock_guard guard(mMutex);
        mDispatchMap[nfLogGroup] = fn;
    }
    RETURN_IF_NOT_OK(cfgCmdBind(sendFn, nfLogGroup));

    // Mode must be set for every nfLogGroup
    const uint8_t copyMode = copyRange > 0 ? NFULNL_COPY_PACKET : NFULNL_COPY_NONE;
    return cfgMode(sendFn, nfLogGroup, copyRange, copyMode);
}

Status NFLogListener::unsubscribe(uint16_t nfLogGroup) {
    const auto sendFn = [this](const Slice msg) { return mListener->send(msg); };
    RETURN_IF_NOT_OK(cfgCmdUnbind(sendFn, nfLogGroup));
    // Remove from the dispatch map AFTER stopping message delivery.
    {
        std::lock_guard guard(mMutex);
        mDispatchMap.erase(nfLogGroup);
    }
    return ok;
}

StatusOr<std::unique_ptr<NFLogListener>> makeNFLogListener() {
    const auto& sys = sSyscalls.get();
    ASSIGN_OR_RETURN(auto event, sys.eventfd(0, EFD_CLOEXEC));
    const auto domain = AF_NETLINK;
    const auto flags = SOCK_DGRAM | SOCK_CLOEXEC | SOCK_NONBLOCK;
    const auto protocol = NETLINK_NETFILTER;
    ASSIGN_OR_RETURN(auto sock, sys.socket(domain, flags, protocol));

    // Timestamps are disabled by default. Request RX timestamping
    RETURN_IF_NOT_OK(sys.setsockopt<int32_t>(sock, SOL_SOCKET, SO_TIMESTAMP, 1));

    std::shared_ptr<NetlinkListenerInterface> listener =
            std::make_unique<NetlinkListener>(std::move(event), std::move(sock), "NFLogListener");
    const auto sendFn = [&listener](const Slice msg) { return listener->send(msg); };
    RETURN_IF_NOT_OK(cfgCmdPfUnbind(sendFn));
    return std::unique_ptr<NFLogListener>(new NFLogListener(std::move(listener)));
}

}  // namespace net
}  // namespace android
