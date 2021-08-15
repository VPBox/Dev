/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef NETUTILS_OPERATIONLIMITER_H
#define NETUTILS_OPERATIONLIMITER_H

#include <mutex>
#include <unordered_map>

#include <android-base/logging.h>
#include <android-base/thread_annotations.h>

namespace android {
namespace netdutils {

// Tracks the number of operations in progress on behalf of a particular key or
// ID, rejecting further attempts to start new operations after a configurable
// limit has been reached.
//
// The intended usage pattern is:
//     OperationLimiter<UserId> connections_per_user;
//     ...
//     // Before opening a new connection
//     if (!limiter.start(user)) {
//         return error;
//     } else {
//         // open the connection
//         // ...do some work...
//         // close the connection
//         limiter.finish(user);
//     }
//
// This class is thread-safe.
template<typename KeyType>
class OperationLimiter {
public:
    explicit OperationLimiter(int limit) : mLimitPerKey(limit) {}

    ~OperationLimiter() {
        DCHECK(mCounters.empty())
                << "Destroying OperationLimiter with active operations";
    }

    // Returns false if |key| has reached the maximum number of concurrent
    // operations, otherwise increments the counter and returns true.
    //
    // Note: each successful start(key) must be matched by exactly one call to
    // finish(key).
    bool start(KeyType key) EXCLUDES(mMutex) {
        std::lock_guard lock(mMutex);
        auto& cnt = mCounters[key];  // operator[] creates new entries as needed.
        if (cnt >= mLimitPerKey) {
            // Oh, no!
            return false;
        }
        ++cnt;
        return true;
    }

    // Decrements the number of operations in progress accounted to |key|.
    // See usage notes on start().
    void finish(KeyType key) EXCLUDES(mMutex) {
        std::lock_guard lock(mMutex);
        auto it = mCounters.find(key);
        if (it == mCounters.end()) {
            LOG(FATAL_WITHOUT_ABORT) << "Decremented non-existent counter for key=" << key;
            return;
        }
        auto& cnt = it->second;
        --cnt;
        if (cnt <= 0) {
            // Cleanup counters once they drop down to zero.
            mCounters.erase(it);
        }
    }

private:
    // Protects access to the map below.
    std::mutex mMutex;

    // Tracks the number of outstanding queries by key.
    std::unordered_map<KeyType, int> mCounters GUARDED_BY(mMutex);

    // Maximum number of outstanding queries from a single key.
    const int mLimitPerKey;
};

}  // namespace netdutils
}  // namespace android

#endif // NETUTILS_OPERATIONLIMITER_H
