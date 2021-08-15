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
 * See the License for the specic language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_LIBPERFMGR_NODELOOPERTHREAD_H_
#define ANDROID_LIBPERFMGR_NODELOOPERTHREAD_H_

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <utils/Thread.h>

#include "perfmgr/Node.h"

namespace android {
namespace perfmgr {

// The NodeAction specifies the sysfs node, the value to be assigned, and the
// timeout for this action:
struct NodeAction {
    NodeAction(std::size_t node_index, std::size_t value_index,
               std::chrono::milliseconds timeout_ms)
        : node_index(node_index),
          value_index(value_index),
          timeout_ms(timeout_ms) {}
    std::size_t node_index;
    std::size_t value_index;
    std::chrono::milliseconds timeout_ms;  // 0ms for forever
};

// The NodeLooperThread is responsible for managing each of the sysfs nodes
// specified in the configuration. At initialization, the NodeLooperThrea holds
// a vector containing the nodes defined in the configuration. The NodeManager
// gets powerhint requests and cancellations from the HintManager, maintains
// state about the current set of powerhint requests on each sysfs node, and
// decides how to apply the requests. The NodeLooperThread contains a ThreadLoop
// to maintain the sysfs nodes, and that thread is woken up both to handle
// powerhint requests and when the timeout expires for an in-progress powerhint.
class NodeLooperThread : public ::android::Thread {
  public:
    explicit NodeLooperThread(std::vector<std::unique_ptr<Node>> nodes)
        : Thread(false), nodes_(std::move(nodes)) {}
    virtual ~NodeLooperThread() { Stop(); }

    // Need call Stop() as the threadloop will hold a strong pointer
    // itself and wait for Condition fired or timeout (60s) before
    // the out looper can call deconstructor to Stop() thread
    void Stop();

    // Return true when successfully adds request from actions for the hint_type
    // in each individual node. Return false if any of the actions has either
    // invalid node index or value index.
    bool Request(const std::vector<NodeAction>& actions,
                 const std::string& hint_type);
    // Return when successfully cancels request from actions for the hint_type
    // in each individual node. Return false if any of the actions has invalid
    // node index.
    bool Cancel(const std::vector<NodeAction>& actions,
                const std::string& hint_type);

    // Dump all nodes to fd
    void DumpToFd(int fd);

  private:
    NodeLooperThread(NodeLooperThread const&) = delete;
    void operator=(NodeLooperThread const&) = delete;
    bool threadLoop() override;
    void onFirstRef() override;

    static constexpr auto kMaxUpdatePeriod = std::chrono::milliseconds::max();

    std::vector<std::unique_ptr<Node>> nodes_;  // parsed from Config

    // conditional variable from C++ standard library can be affected by wall
    // time change as it is using CLOCK_REAL (b/35756266). The component should
    // not be impacted by wall time, thus need use Android specific Condition
    // class for waking up threadloop.
    ::android::Condition wake_cond_;

    // lock to protect nodes_
    ::android::Mutex lock_;
};

}  // namespace perfmgr
}  // namespace android

#endif  // ANDROID_LIBPERFMGR_NODELOOPERTHREAD_H_
