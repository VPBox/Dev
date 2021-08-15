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

#include "linkerconfig/section.h"

#include "linkerconfig/log.h"

#define LOG_TAG "linkerconfig"

namespace android {
namespace linkerconfig {
namespace modules {
std::shared_ptr<Namespace> Section::CreateNamespace(
    const std::string& namespace_name, bool is_isolated, bool is_visible) {
  auto new_namespace =
      std::make_shared<Namespace>(namespace_name, is_isolated, is_visible);

  if (namespaces_.find(namespace_name) != namespaces_.end()) {
    LOG(INFO) << "Namespace " << namespace_name
              << " already exists. Overwriting namespace.";
  }

  namespaces_[namespace_name] = new_namespace;
  return new_namespace;
}

void Section::WriteConfig(ConfigWriter& writer) {
  writer.WriteLine("[%s]", name_.c_str());

  std::string additional_namespaces = "";

  bool is_first = true;
  for (auto& ns : namespaces_) {
    if (ns.first != "default") {
      if (!is_first) {
        additional_namespaces += ",";
      }

      additional_namespaces += ns.first;
      is_first = false;
    }
  }

  if (!is_first) {
    writer.WriteLine("additional.namespaces = " + additional_namespaces);
  }

  for (auto& ns : namespaces_) {
    ns.second->WriteConfig(writer);
  }
}

void Section::WriteBinaryPaths(ConfigWriter& writer) {
  writer.SetPrefix("dir." + name_ + " = ");

  for (auto& path : binary_paths_) {
    writer.WriteLine(path);
  }

  writer.ResetPrefix();
}

std::string Section::GetName() {
  return name_;
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android