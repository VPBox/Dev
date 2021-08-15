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

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "perfmgr/PropertyNode.h"

namespace android {
namespace perfmgr {

PropertyNode::PropertyNode(std::string name, std::string node_path,
                           std::vector<RequestGroup> req_sorted,
                           std::size_t default_val_index, bool reset_on_init)
    : Node(std::move(name), std::move(node_path), std::move(req_sorted),
           default_val_index, reset_on_init) {
    if (reset_on_init) {
        Update(false);
    }
}

std::chrono::milliseconds PropertyNode::Update(bool) {
    std::size_t value_index = default_val_index_;
    std::chrono::milliseconds expire_time = std::chrono::milliseconds::max();

    // Find the highest outstanding request's expire time
    for (std::size_t i = 0; i < req_sorted_.size(); i++) {
        if (req_sorted_[i].GetExpireTime(&expire_time)) {
            value_index = i;
            break;
        }
    }

    // Update node only if request index changes
    if (value_index != current_val_index_) {
        const std::string& req_value =
            req_sorted_[value_index].GetRequestValue();

        if (!android::base::SetProperty(node_path_, req_value)) {
            LOG(WARNING) << "Failed to set property to : " << node_path_
                         << " with value: " << req_value;
        } else {
            // Update current index only when succeed
            current_val_index_ = value_index;
        }
    }
    return expire_time;
}

void PropertyNode::DumpToFd(int fd) const {
    std::string node_value = android::base::GetProperty(node_path_, "");
    std::string buf(android::base::StringPrintf(
        "%s\t%s\t%zu\t%s\n", name_.c_str(), node_path_.c_str(),
        current_val_index_, node_value.c_str()));
    if (!android::base::WriteStringToFd(buf, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
}

}  // namespace perfmgr
}  // namespace android
