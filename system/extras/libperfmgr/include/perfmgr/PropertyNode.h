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

#ifndef ANDROID_LIBPERFMGR_PROPERTYNODE_H_
#define ANDROID_LIBPERFMGR_PROPERTYNODE_H_

#include <cstddef>
#include <string>
#include <vector>

#include "perfmgr/Node.h"

namespace android {
namespace perfmgr {

// PropertyNode represents managed system properties
class PropertyNode : public Node {
  public:
    PropertyNode(std::string name, std::string node_path,
                 std::vector<RequestGroup> req_sorted,
                 std::size_t default_val_index, bool reset_on_init);

    std::chrono::milliseconds Update(bool log_error) override;

    void DumpToFd(int fd) const override;

  private:
    PropertyNode(const Node& other) = delete;
    PropertyNode& operator=(Node const&) = delete;
};

}  // namespace perfmgr
}  // namespace android

#endif  // ANDROID_LIBPERFMGR_PROPERTYNODE_H_
