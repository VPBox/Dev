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

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/namespace.h"

namespace android {
namespace linkerconfig {
namespace modules {
class Section {
 public:
  Section(const std::string& name) : name_(name) {
  }
  template <typename T, typename... Args>
  void AddBinaryPath(T&& binary_path, Args&&... binary_paths);
  std::shared_ptr<Namespace> CreateNamespace(const std::string& namespace_name,
                                             bool is_isolated = false,
                                             bool is_visible = false);
  void WriteConfig(ConfigWriter& writer);
  void WriteBinaryPaths(ConfigWriter& writer);
  std::string GetName();

 private:
  const std::string name_;
  std::vector<std::string> binary_paths_;
  std::map<std::string, std::shared_ptr<Namespace>> namespaces_;
};

template <typename T, typename... Args>
void Section::AddBinaryPath(T&& binary_path, Args&&... binary_paths) {
  binary_paths_.push_back(std::forward<T>(binary_path));
  if constexpr (sizeof...(Args) > 0) {
    AddBinaryPath(std::forward<Args>(binary_paths)...);
  }
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android