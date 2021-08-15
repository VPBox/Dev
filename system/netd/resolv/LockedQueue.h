/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef _DNS_LOCKED_QUEUE_H
#define _DNS_LOCKED_QUEUE_H

#include <algorithm>
#include <deque>
#include <mutex>

#include <android-base/thread_annotations.h>

namespace android {
namespace net {

template <typename T>
class LockedQueue {
  public:
    // Push an item onto the queue.
    void push(T item) {
        std::lock_guard guard(mLock);
        mQueue.push_front(std::move(item));
    }

    // Swap out the contents of the queue
    void swap(std::deque<T>& other) {
        std::lock_guard guard(mLock);
        mQueue.swap(other);
    }

  private:
    std::mutex mLock;
    std::deque<T> mQueue GUARDED_BY(mLock);
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_LOCKEDQUEUE_H
