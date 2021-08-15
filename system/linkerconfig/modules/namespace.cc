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

#include "linkerconfig/namespace.h"

#include "linkerconfig/log.h"

#define LOG_TAG "linkerconfig"

namespace android {
namespace linkerconfig {
namespace modules {

constexpr const char* kDataAsanPath = "/data/asan";

void Namespace::WritePathString(ConfigWriter& writer,
                                const std::string& path_type,
                                const std::vector<std::string>& path_list) {
  std::string prefix = path_type + ".paths ";
  bool is_first = true;
  for (auto& path : path_list) {
    writer.WriteLine(prefix + (is_first ? "= " : "+= ") + path);
    is_first = false;
  }
}

std::shared_ptr<Link> Namespace::CreateLink(const std::string& target_namespace,
                                            bool allow_all_shared_libs) {
  auto new_link =
      std::make_shared<Link>(name_, target_namespace, allow_all_shared_libs);

  if (links_.find(target_namespace) != links_.end()) {
    LOG(INFO) << "Link to " << target_namespace
              << " already exists. Overwriting link.";
  }

  links_[target_namespace] = new_link;
  return new_link;
}

void Namespace::WriteConfig(ConfigWriter& writer) {
  writer.SetPrefix("namespace." + name_ + ".");

  writer.WriteLine("isolated = %s", is_isolated_ ? "true" : "false");

  if (is_visible_) {
    writer.WriteLine("visible = true");
  }

  WritePathString(writer, "search", search_paths_);
  WritePathString(writer, "permitted", permitted_paths_);
  WritePathString(writer, "asan.search", asan_search_paths_);
  WritePathString(writer, "asan.permitted", asan_permitted_paths_);

  if (!links_.empty()) {
    std::string link_list = "";

    bool is_first = true;
    for (auto& link : links_) {
      if (!is_first) {
        link_list += ",";
      }
      link_list += link.first;
      is_first = false;
    }

    writer.WriteLine("links = " + link_list);

    for (auto& link : links_) {
      link.second->WriteConfig(writer);
    }
  }

  writer.ResetPrefix();
}

void Namespace::AddSearchPath(const std::string& path, bool in_asan,
                              bool with_data_asan) {
  search_paths_.push_back(path);

  if (in_asan) {
    asan_search_paths_.push_back(path);
    if (with_data_asan) {
      asan_search_paths_.push_back(kDataAsanPath + path);
    }
  }
}

void Namespace::AddPermittedPath(const std::string& path, bool in_asan,
                                 bool with_data_asan) {
  permitted_paths_.push_back(path);

  if (in_asan) {
    asan_permitted_paths_.push_back(path);
    if (with_data_asan) {
      asan_permitted_paths_.push_back(kDataAsanPath + path);
    }
  }
}

}  // namespace modules
}  // namespace linkerconfig
}  // namespace android