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
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

namespace android {
namespace gtest_extras {

class Options {
 public:
  Options() = default;
  ~Options() = default;

  bool Process(const std::vector<const char*>& args, std::vector<const char*>* child_args);

  size_t job_count() const { return job_count_; }
  int num_iterations() const { return num_iterations_; }

  uint64_t deadline_threshold_ms() const { return numerics_.at("deadline_threshold_ms"); }
  uint64_t slow_threshold_ms() const { return numerics_.at("slow_threshold_ms"); }

  uint64_t shard_index() const { return numerics_.at("gtest_shard_index"); }
  uint64_t total_shards() const { return numerics_.at("gtest_total_shards"); }

  bool print_time() const { return bools_.at("gtest_print_time"); }
  bool gtest_format() const { return bools_.at("gtest_format"); }
  bool allow_disabled_tests() const { return bools_.at("gtest_also_run_disabled_tests"); }
  bool list_tests() const { return bools_.at("gtest_list_tests"); }

  const std::string& color() const { return strings_.at("gtest_color"); }
  const std::string& xml_file() const { return strings_.at("xml_file"); }
  const std::string& filter() const { return strings_.at("gtest_filter"); }

 private:
  size_t job_count_;
  int num_iterations_;

  std::unordered_map<std::string, bool> bools_;
  std::unordered_map<std::string, std::string> strings_;
  std::unordered_map<std::string, uint64_t> numerics_;

  enum FlagType : uint32_t {
    FLAG_NONE = 0,
    FLAG_CHILD = 0x1,                 // Argument preserved for forked child call.
    FLAG_INCOMPATIBLE = 0x2,          // Not compatible with isolation mode.
    FLAG_ENVIRONMENT_VARIABLE = 0x4,  // Can be an environment variable.
    FLAG_REQUIRES_VALUE = 0x8,        // Flag requires a non-empty value.
    FLAG_OPTIONAL_VALUE = 0x10,       // Flag takes an optional value.
  };
  static constexpr uint32_t FLAG_TAKES_VALUE = FLAG_REQUIRES_VALUE | FLAG_OPTIONAL_VALUE;

  struct ArgInfo {
    uint32_t flags;
    bool (Options::*func)(const std::string&, const std::string&, bool);
  };

  bool HandleArg(const std::string& arg, const std::string& value, const ArgInfo& info,
                 bool from_env = false);

  bool SetNumeric(const std::string&, const std::string&, bool);
  bool SetNumericEnvOnly(const std::string&, const std::string&, bool);
  bool SetBool(const std::string&, const std::string&, bool);
  bool SetString(const std::string&, const std::string&, bool);
  bool SetIterations(const std::string&, const std::string&, bool);
  bool SetXmlFile(const std::string&, const std::string&, bool);
  bool SetPrintTime(const std::string&, const std::string&, bool);

  const static std::unordered_map<std::string, ArgInfo> kArgs;
};

}  // namespace gtest_extras
}  // namespace android
