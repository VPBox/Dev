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
#include <string>
#include <vector>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/link.h"

namespace android {
namespace linkerconfig {
namespace modules {

class Namespace {
 public:
  Namespace(const std::string& name, bool is_isolated = false,
            bool is_visible = false)
      : is_isolated_(is_isolated), is_visible_(is_visible), name_(name) {
  }

  // Add path to search path
  // This function will add path to namespace.<<namespace>>.search.paths
  // If also_in_asan is true, this will add path also to
  // namespace.<<namespace>>.asan.search.paths
  // If with_data_asan is true when also_in_asan is true,
  // this will also add asan path starts with /data/asan
  //
  // AddSearchPath("/system/${LIB}", false, false) :
  //    namespace.xxx.search.paths += /system/${LIB}
  // AddSearchPath("/system/${LIB}", true, false) :
  //    namespace.xxx.search.paths += /system/${LIB}
  //    namespace.xxx.asan.search.paths += /system/${LIB}
  // AddSearchPath("/system/${LIB}", true, true) :
  //    namespace.xxx.search.paths += /system/${LIB}
  //    namespace.xxx.asan.search.paths += /system/${LIB}
  //    namespace.xxx.asan.search.paths += /data/asan/system/${LIB}
  void AddSearchPath(const std::string& path, bool also_in_asan = true,
                     bool with_data_asan = true);

  // Add path to permitted path
  // This function will add path to namespace.<<namespace>>.permitted.paths
  // If also_in_asan is true, this will add path also to
  // namespace.<<namespace>>.asan.permitted.paths
  // If with_data_asan is true when also_in_asan is true,
  // this will also add asan path starts with /data/asan
  //
  // AddSearchPath("/system/${LIB}", false, false) :
  //    namespace.xxx.permitted.paths += /system/${LIB}
  // AddSearchPath("/system/${LIB}", true, false) :
  //    namespace.xxx.permitted.paths += /system/${LIB}
  //    namespace.xxx.asan.permitted.paths += /system/${LIB}
  // AddSearchPath("/system/${LIB}", true, true) :
  //    namespace.xxx.permitted.paths += /system/${LIB}
  //    namespace.xxx.asan.permitted.paths += /system/${LIB}
  //    namespace.xxx.asan.permitted.paths += /data/asan/system/${LIB}
  void AddPermittedPath(const std::string& path, bool also_in_asan = true,
                        bool with_data_asan = true);
  std::shared_ptr<Link> CreateLink(const std::string& target_namespace,
                                   bool allow_all_shared_libs = false);
  void WriteConfig(ConfigWriter& writer);

 private:
  const bool is_isolated_;
  const bool is_visible_;
  const std::string name_;
  std::vector<std::string> search_paths_;
  std::vector<std::string> permitted_paths_;
  std::vector<std::string> asan_search_paths_;
  std::vector<std::string> asan_permitted_paths_;
  std::map<std::string, std::shared_ptr<Link>> links_;
  void WritePathString(ConfigWriter& writer, const std::string& path_type,
                       const std::vector<std::string>& path_list);
};
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android