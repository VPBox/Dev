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
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/log.h"

#define LOG_TAG "linkerconfig"

namespace android {
namespace linkerconfig {
namespace modules {
class Link {
 public:
  Link(const std::string& origin_namespace, const std::string& target_namespace,
       bool allow_all_shared_libs = false)
      : origin_namespace_(origin_namespace),
        target_namespace_(target_namespace),
        allow_all_shared_libs_(allow_all_shared_libs) {
  }
  template <typename T, typename... Args>
  void AddSharedLib(T&& lib_name, Args&&... lib_names);
  void WriteConfig(ConfigWriter& writer);

 private:
  const std::string origin_namespace_;
  const std::string target_namespace_;
  const bool allow_all_shared_libs_;
  std::vector<std::string> shared_libs_;
};

template <typename T, typename... Args>
void Link::AddSharedLib(T&& lib_name, Args&&... lib_names) {
  if (allow_all_shared_libs_) {
    LOG(WARNING) << "Tried to add shared libraries to link from "
                 << origin_namespace_ << " to " << target_namespace_
                 << "while this link is allow_all_shared_libs";
    return;
  }
  shared_libs_.push_back(std::forward<T>(lib_name));
  if constexpr (sizeof...(Args) > 0) {
    AddSharedLib(std::forward<Args>(lib_names)...);
  }
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android