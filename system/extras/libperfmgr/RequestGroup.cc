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

#define LOG_TAG "libperfmgr"

#include "perfmgr/RequestGroup.h"

namespace android {
namespace perfmgr {

bool RequestGroup::AddRequest(const std::string& hint_type, ReqTime end_time) {
    if (request_map_.find(hint_type) == request_map_.end()) {
        request_map_.emplace(hint_type, end_time);
        return true;
    } else {
        if (request_map_[hint_type] < end_time) {
            request_map_[hint_type] = end_time;
        }
        return false;
    }
}

bool RequestGroup::RemoveRequest(const std::string& hint_type) {
    return request_map_.erase(hint_type);
}

const std::string& RequestGroup::GetRequestValue() const {
    return request_value_;
}

bool RequestGroup::GetExpireTime(std::chrono::milliseconds* expire_time) {
    ReqTime now = std::chrono::steady_clock::now();
    *expire_time = std::chrono::milliseconds::max();

    bool active = false;
    for (auto it = request_map_.begin(); it != request_map_.end();) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            it->second - now);
        if (duration <= std::chrono::milliseconds::zero()) {
            it = request_map_.erase(it);
        } else {
            *expire_time = std::min(duration, *expire_time);
            active = true;
            ++it;
        }
    }
    return active;
}

}  // namespace perfmgr
}  // namespace android
