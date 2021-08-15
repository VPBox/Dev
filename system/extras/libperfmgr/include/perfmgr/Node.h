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

#ifndef ANDROID_LIBPERFMGR_NODE_H_
#define ANDROID_LIBPERFMGR_NODE_H_

#include <cstddef>
#include <string>
#include <vector>

#include <android-base/unique_fd.h>

#include "perfmgr/RequestGroup.h"

namespace android {
namespace perfmgr {

// The Node class provides an interface for adding and cancelling powerhint
// requests, as well as checking the next time that an in-progress powerhint
// request will expire. There are additional methods for getting the Node’s name
// and the index of a value, which may be used for initialization, debugging,
// and request management. The core of the Node class is a vector of
// RequestGroups named req_sorted_, which is used to track the in-progress
// requests on the node. Each entry in the vector corresponds to a possible
// value for the node, in priority order. For example, the first entry in the
// vector for the cpu0 cluster represents the in-progress requests to boost the
// cluster’s frequency to the highest available value. The next entry represents
// the in-progress requests to boost the cluster’s frequency to the next highest
// value. For each value, there may be multiple requests because different
// powerhints may request the same value, and the requests may have different
// expiration times. All of the in-progress powerhints for a given value are
// collected in a RequestGroup. Node class is not thread safe so it needs
// protection from caller e.g. NodeLooperThread.
class Node {
  public:
    virtual ~Node() {}

    // Return true if successfully add a request
    bool AddRequest(std::size_t value_index, const std::string& hint_type,
                    ReqTime end_time);

    // Return true if successfully remove a request
    bool RemoveRequest(const std::string& hint_type);

    // Return the nearest expire time of active requests; return
    // std::chrono::milliseconds::max() if no active request on Node; update
    // node's controlled file node value and the current value index based on
    // active request.
    virtual std::chrono::milliseconds Update(bool log_error) = 0;

    const std::string& GetName() const;
    const std::string& GetPath() const;
    std::vector<std::string> GetValues() const;
    std::size_t GetDefaultIndex() const;
    bool GetResetOnInit() const;
    bool GetValueIndex(const std::string& value, std::size_t* index) const;
    virtual void DumpToFd(int fd) const = 0;

  protected:
    Node(std::string name, std::string node_path,
         std::vector<RequestGroup> req_sorted, std::size_t default_val_index,
         bool reset_on_init);
    Node(const Node& other) = delete;
    Node& operator=(Node const&) = delete;

    const std::string name_;
    const std::string node_path_;
    // request vector, one entry per possible value, sorted by priority
    std::vector<RequestGroup> req_sorted_;
    const std::size_t default_val_index_;
    const bool reset_on_init_;
    std::size_t current_val_index_;
};

}  // namespace perfmgr
}  // namespace android

#endif  // ANDROID_LIBPERFMGR_NODE_H_
