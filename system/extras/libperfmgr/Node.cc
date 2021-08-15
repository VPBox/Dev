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
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "perfmgr/Node.h"

namespace android {
namespace perfmgr {

Node::Node(std::string name, std::string node_path,
           std::vector<RequestGroup> req_sorted, std::size_t default_val_index,
           bool reset_on_init)
    : name_(std::move(name)),
      node_path_(std::move(node_path)),
      req_sorted_(std::move(req_sorted)),
      default_val_index_(default_val_index),
      reset_on_init_(reset_on_init),
      // Assigning an invalid value so the next Update() will update the
      // Node's value to default
      current_val_index_(reset_on_init ? req_sorted_.size()
                                       : default_val_index) {}

bool Node::AddRequest(std::size_t value_index, const std::string& hint_type,
                      ReqTime end_time) {
    if (value_index >= req_sorted_.size()) {
        LOG(ERROR) << "Value index out of bound: " << value_index
                   << " ,size: " << req_sorted_.size();
        return false;
    }
    // Add/Update request to the new end_time for the specific hint_type
    req_sorted_[value_index].AddRequest(hint_type, end_time);
    return true;
}

bool Node::RemoveRequest(const std::string& hint_type) {
    bool ret = false;
    // Remove all requests for the specific hint_type
    for (auto& value : req_sorted_) {
        ret = value.RemoveRequest(hint_type) || ret;
    }
    return ret;
}

const std::string& Node::GetName() const {
    return name_;
}

const std::string& Node::GetPath() const {
    return node_path_;
}

bool Node::GetValueIndex(const std::string& value, std::size_t* index) const {
    bool found = false;
    for (std::size_t i = 0; i < req_sorted_.size(); i++) {
        if (req_sorted_[i].GetRequestValue() == value) {
            *index = i;
            found = true;
            break;
        }
    }
    return found;
}

std::size_t Node::GetDefaultIndex() const {
    return default_val_index_;
}

bool Node::GetResetOnInit() const {
    return reset_on_init_;
}

std::vector<std::string> Node::GetValues() const {
    std::vector<std::string> values;
    for (const auto& value : req_sorted_) {
        values.emplace_back(value.GetRequestValue());
    }
    return values;
}

}  // namespace perfmgr
}  // namespace android
