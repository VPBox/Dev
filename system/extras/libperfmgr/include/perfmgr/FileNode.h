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

#ifndef ANDROID_LIBPERFMGR_FILENODE_H_
#define ANDROID_LIBPERFMGR_FILENODE_H_

#include <cstddef>
#include <string>
#include <vector>

#include <android-base/unique_fd.h>

#include "perfmgr/Node.h"

namespace android {
namespace perfmgr {

// FileNode represents file
class FileNode : public Node {
  public:
    FileNode(std::string name, std::string node_path,
             std::vector<RequestGroup> req_sorted, std::size_t default_val_index,
             bool reset_on_init, bool hold_fd = false);

    std::chrono::milliseconds Update(bool log_error) override;

    bool GetHoldFd() const;

    void DumpToFd(int fd) const override;

  private:
    FileNode(const Node& other) = delete;
    FileNode& operator=(Node const&) = delete;

    const bool hold_fd_;
    android::base::unique_fd fd_;
};

}  // namespace perfmgr
}  // namespace android

#endif  // ANDROID_LIBPERFMGR_FILENODE_H_
