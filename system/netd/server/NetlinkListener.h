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

#ifndef NETLINK_LISTENER_H
#define NETLINK_LISTENER_H

#include <functional>
#include <map>
#include <mutex>
#include <thread>

#include <android-base/thread_annotations.h>
#include <netdutils/Netlink.h>
#include <netdutils/Slice.h>
#include <netdutils/Status.h>
#include <netdutils/UniqueFd.h>

namespace android {
namespace net {

class NetlinkListenerInterface {
  public:
    using DispatchFn = std::function<void(const nlmsghdr& nlmsg, const netdutils::Slice msg)>;

    using SkErrorHandler = std::function<void(const int fd, const int err)>;

    virtual ~NetlinkListenerInterface() = default;

    // Send message to the kernel using the underlying netlink socket
    virtual netdutils::Status send(const netdutils::Slice msg) = 0;

    // Deliver future messages with nlmsghdr.nlmsg_type == type to fn.
    //
    // Threadsafe.
    // All dispatch functions invoked on a single service thread.
    // subscribe() and join() must not be called from the stack of fn().
    virtual netdutils::Status subscribe(uint16_t type, const DispatchFn& fn) = 0;

    // Halt delivery of future messages with nlmsghdr.nlmsg_type == type.
    // Threadsafe.
    virtual netdutils::Status unsubscribe(uint16_t type) = 0;

    virtual void registerSkErrorHandler(const SkErrorHandler& handler) = 0;
};

// NetlinkListener manages a netlink socket and associated blocking
// service thread.
//
// This class is written in a generic way to allow multiple different
// netlink subsystems to share this common infrastructure. If multiple
// subsystems share the same message delivery requirements (drops ok,
// no drops) they may share a single listener by calling subscribe()
// with multiple types.
//
// This class is suitable for moderate performance message
// processing. In particular it avoids extra copies of received
// message data and allows client code to control which message
// attributes are processed.
//
// Note that NetlinkListener is capable of processing multiple batched
// netlink messages in a single system call. This is useful to
// netfilter extensions that allow batching of events like NFLOG.
class NetlinkListener : public NetlinkListenerInterface {
  public:
    NetlinkListener(netdutils::UniqueFd event, netdutils::UniqueFd sock, const std::string& name);

    ~NetlinkListener() override;

    netdutils::Status send(const netdutils::Slice msg) override;

    netdutils::Status subscribe(uint16_t type, const DispatchFn& fn) override EXCLUDES(mMutex);

    netdutils::Status unsubscribe(uint16_t type) override EXCLUDES(mMutex);

    void registerSkErrorHandler(const SkErrorHandler& handler) override;

  private:
    netdutils::Status run();

    const netdutils::UniqueFd mEvent;
    const netdutils::UniqueFd mSock;
    const std::string mThreadName;
    std::mutex mMutex;
    std::map<uint16_t, DispatchFn> mDispatchMap GUARDED_BY(mMutex);
    std::thread mWorker;
    SkErrorHandler mErrorHandler;
};

}  // namespace net
}  // namespace android

#endif /* NETLINK_LISTENER_H */
