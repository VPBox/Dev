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

#define LOG_TAG "NetlinkListener"

#include "NetlinkListener.h"

#include <sstream>
#include <vector>

#include <linux/netfilter/nfnetlink.h>

#include <log/log.h>
#include <netdutils/Misc.h>
#include <netdutils/Syscalls.h>

namespace android {
namespace net {

using netdutils::Fd;
using netdutils::Slice;
using netdutils::Status;
using netdutils::UniqueFd;
using netdutils::findWithDefault;
using netdutils::forEachNetlinkMessage;
using netdutils::makeSlice;
using netdutils::sSyscalls;
using netdutils::status::ok;
using netdutils::statusFromErrno;

namespace {

constexpr int kNetlinkMsgErrorType = (NFNL_SUBSYS_NONE << 8) | NLMSG_ERROR;

constexpr sockaddr_nl kKernelAddr = {
    .nl_family = AF_NETLINK, .nl_pad = 0, .nl_pid = 0, .nl_groups = 0,
};

const NetlinkListener::DispatchFn kDefaultDispatchFn = [](const nlmsghdr& nlmsg, const Slice) {
    std::stringstream ss;
    ss << nlmsg;
    ALOGE("unhandled netlink message: %s", ss.str().c_str());
};

}  // namespace

NetlinkListener::NetlinkListener(UniqueFd event, UniqueFd sock, const std::string& name)
    : mEvent(std::move(event)), mSock(std::move(sock)), mThreadName(name) {
    const auto rxErrorHandler = [](const nlmsghdr& nlmsg, const Slice msg) {
        std::stringstream ss;
        ss << nlmsg << " " << msg << " " << netdutils::toHex(msg, 32);
        ALOGE("unhandled netlink message: %s", ss.str().c_str());
    };
    expectOk(NetlinkListener::subscribe(kNetlinkMsgErrorType, rxErrorHandler));

    mErrorHandler = [& name = mThreadName](const int fd, const int err) {
        ALOGE("Error on NetlinkListener(%s) fd=%d: %s", name.c_str(), fd, strerror(err));
    };

    // Start the thread
    mWorker = std::thread([this]() { run().ignoreError(); });
}

NetlinkListener::~NetlinkListener() {
    const auto& sys = sSyscalls.get();
    const uint64_t data = 1;
    // eventfd should never enter an error state unexpectedly
    expectOk(sys.write(mEvent, makeSlice(data)).status());
    mWorker.join();
}

Status NetlinkListener::send(const Slice msg) {
    const auto& sys = sSyscalls.get();
    ASSIGN_OR_RETURN(auto sent, sys.sendto(mSock, msg, 0, kKernelAddr));
    if (sent != msg.size()) {
        return statusFromErrno(EMSGSIZE, "unexpect message size");
    }
    return ok;
}

Status NetlinkListener::subscribe(uint16_t type, const DispatchFn& fn) {
    std::lock_guard guard(mMutex);
    mDispatchMap[type] = fn;
    return ok;
}

Status NetlinkListener::unsubscribe(uint16_t type) {
    std::lock_guard guard(mMutex);
    mDispatchMap.erase(type);
    return ok;
}

void NetlinkListener::registerSkErrorHandler(const SkErrorHandler& handler) {
    mErrorHandler = handler;
}

Status NetlinkListener::run() {
    std::vector<char> rxbuf(4096);

    const auto rxHandler = [this](const nlmsghdr& nlmsg, const Slice& buf) {
        std::lock_guard guard(mMutex);
        const auto& fn = findWithDefault(mDispatchMap, nlmsg.nlmsg_type, kDefaultDispatchFn);
        fn(nlmsg, buf);
    };

    if (mThreadName.length() > 0) {
        int ret = pthread_setname_np(pthread_self(), mThreadName.c_str());
        if (ret) {
            ALOGE("thread name set failed, name: %s, ret: %s", mThreadName.c_str(), strerror(ret));
        }
    }
    const auto& sys = sSyscalls.get();
    const std::array<Fd, 2> fds{{{mEvent}, {mSock}}};
    const int events = POLLIN;
    const double timeout = 3600;
    while (true) {
        ASSIGN_OR_RETURN(auto revents, sys.ppoll(fds, events, timeout));
        // After mEvent becomes readable, we should stop servicing mSock and return
        if (revents[0] & POLLIN) {
            break;
        }
        if (revents[1] & (POLLIN|POLLERR)) {
            auto rx = sys.recvfrom(mSock, makeSlice(rxbuf), 0);
            int err = rx.status().code();
            if (err) {
                // Ignore errors. The only error we expect to see here is ENOBUFS, and there's
                // nothing we can do about that. The recvfrom above will already have cleared the
                // error indication and ensured we won't get EPOLLERR again.
                // TODO: Consider using NETLINK_NO_ENOBUFS.
                mErrorHandler(((Fd) mSock).get(), err);
                continue;
            }
            forEachNetlinkMessage(rx.value(), rxHandler);
        }
    }
    return ok;
}

}  // namespace net
}  // namespace android
