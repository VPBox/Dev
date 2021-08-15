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

#ifndef NETDUTILS_THREADUTIL_H
#define NETDUTILS_THREADUTIL_H

#include <pthread.h>
#include <memory>

#include <android-base/logging.h>

namespace android {
namespace netdutils {

struct scoped_pthread_attr {
    scoped_pthread_attr() { pthread_attr_init(&attr); }
    ~scoped_pthread_attr() { pthread_attr_destroy(&attr); }

    int detach() { return -pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); }

    pthread_attr_t attr;
};

template <typename T>
inline void* runAndDelete(void* obj) {
    std::unique_ptr<T> handler(reinterpret_cast<T*>(obj));
    handler->run();
    return nullptr;
}

template <typename T>
inline int threadLaunch(T* obj) {
    if (obj == nullptr) {
        return -EINVAL;
    }

    scoped_pthread_attr scoped_attr;

    int rval = scoped_attr.detach();
    if (rval != 0) {
        return rval;
    }

    pthread_t thread;
    rval = pthread_create(&thread, &scoped_attr.attr, &runAndDelete<T>, obj);
    if (rval != 0) {
        LOG(WARNING) << __func__ << ": pthread_create failed: " << rval;
        return -rval;
    }

    return rval;
}

}  // namespace netdutils
}  // namespace android

#endif  // NETDUTILS_THREADUTIL_H
