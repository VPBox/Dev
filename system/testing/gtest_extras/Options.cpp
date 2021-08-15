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

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

#include <android-base/parseint.h>
#include <gtest/gtest.h>

#include "Options.h"

namespace android {
namespace gtest_extras {

// The total time each test can run before timing out and being killed.
constexpr uint64_t kDefaultDeadlineThresholdMs = 90000;

// The total time each test can run before a warning is issued.
constexpr uint64_t kDefaultSlowThresholdMs = 2000;

const std::unordered_map<std::string, Options::ArgInfo> Options::kArgs = {
    {"deadline_threshold_ms", {FLAG_REQUIRES_VALUE, &Options::SetNumeric}},
    {"slow_threshold_ms", {FLAG_REQUIRES_VALUE, &Options::SetNumeric}},
    {"gtest_format", {FLAG_NONE, &Options::SetBool}},
    {"no_gtest_format", {FLAG_NONE, &Options::SetBool}},
    {"gtest_list_tests", {FLAG_NONE, &Options::SetBool}},
    {"gtest_filter", {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE, &Options::SetString}},
    {
        "gtest_repeat",
        {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE, &Options::SetIterations},
    },
    {"gtest_output", {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE, &Options::SetXmlFile}},
    {"gtest_print_time", {FLAG_ENVIRONMENT_VARIABLE | FLAG_OPTIONAL_VALUE, &Options::SetPrintTime}},
    {
        "gtest_also_run_disabled_tests",
        {FLAG_ENVIRONMENT_VARIABLE | FLAG_CHILD, &Options::SetBool},
    },
    {"gtest_color",
     {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE | FLAG_CHILD, &Options::SetString}},
    {"gtest_death_test_style",
     {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE | FLAG_CHILD, nullptr}},
    {"gtest_break_on_failure", {FLAG_ENVIRONMENT_VARIABLE | FLAG_INCOMPATIBLE, nullptr}},
    {"gtest_catch_exceptions", {FLAG_ENVIRONMENT_VARIABLE | FLAG_INCOMPATIBLE, nullptr}},
    {"gtest_random_seed", {FLAG_ENVIRONMENT_VARIABLE | FLAG_INCOMPATIBLE, nullptr}},
    {"gtest_shuffle", {FLAG_ENVIRONMENT_VARIABLE | FLAG_INCOMPATIBLE, nullptr}},
    {"gtest_stream_result_to", {FLAG_ENVIRONMENT_VARIABLE | FLAG_INCOMPATIBLE, nullptr}},
    {"gtest_throw_on_failure", {FLAG_ENVIRONMENT_VARIABLE | FLAG_INCOMPATIBLE, nullptr}},
    {"gtest_shard_index",
     {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE, &Options::SetNumericEnvOnly}},
    {"gtest_total_shards",
     {FLAG_ENVIRONMENT_VARIABLE | FLAG_REQUIRES_VALUE, &Options::SetNumericEnvOnly}},
};

static void PrintError(const std::string& arg, std::string msg, bool from_env) {
  if (from_env) {
    std::string variable(arg);
    std::transform(variable.begin(), variable.end(), variable.begin(),
                   [](char c) { return std::toupper(c); });
    printf("env[%s] %s\n", variable.c_str(), msg.c_str());
  } else if (arg[0] == '-') {
    printf("%s %s\n", arg.c_str(), msg.c_str());
  } else {
    printf("--%s %s\n", arg.c_str(), msg.c_str());
  }
}

template <typename IntType>
static bool GetNumeric(const char* arg, const char* value, IntType* numeric_value, bool from_env) {
  bool result = false;
  if constexpr (std::is_unsigned<IntType>::value) {
    result = android::base::ParseUint<IntType>(value, numeric_value);
  } else {
    result = android::base::ParseInt<IntType>(value, numeric_value);
  }
  if (!result) {
    if (errno == ERANGE) {
      PrintError(arg, std::string("value overflows (") + value + ")", from_env);
    } else {
      PrintError(arg, std::string("value is not formatted as a numeric value (") + value + ")",
                 from_env);
    }
    return false;
  }
  return true;
}

bool Options::SetPrintTime(const std::string&, const std::string& value, bool) {
  if (!value.empty() && strtol(value.c_str(), nullptr, 10) == 0) {
    bools_.find("gtest_print_time")->second = false;
  }
  return true;
}

bool Options::SetNumeric(const std::string& arg, const std::string& value, bool from_env) {
  uint64_t* numeric = &numerics_.find(arg)->second;
  if (!GetNumeric<uint64_t>(arg.c_str(), value.c_str(), numeric, from_env)) {
    return false;
  }
  if (*numeric == 0) {
    PrintError(arg, "requires a number greater than zero.", from_env);
    return false;
  }
  return true;
}

bool Options::SetNumericEnvOnly(const std::string& arg, const std::string& value, bool from_env) {
  if (!from_env) {
    PrintError(arg, "is only supported as an environment variable.", false);
    return false;
  }
  uint64_t* numeric = &numerics_.find(arg)->second;
  if (!GetNumeric<uint64_t>(arg.c_str(), value.c_str(), numeric, from_env)) {
    return false;
  }
  return true;
}

bool Options::SetBool(const std::string& arg, const std::string&, bool) {
  bools_.find(arg)->second = true;
  return true;
}

bool Options::SetIterations(const std::string& arg, const std::string& value, bool from_env) {
  if (!GetNumeric<int>(arg.c_str(), value.c_str(), &num_iterations_, from_env)) {
    return false;
  }
  return true;
}

bool Options::SetString(const std::string& arg, const std::string& value, bool) {
  strings_.find(arg)->second = value;
  return true;
}

bool Options::SetXmlFile(const std::string& arg, const std::string& value, bool from_env) {
  if (value.substr(0, 4) != "xml:") {
    PrintError(arg, "only supports an xml output file.", from_env);
    return false;
  }
  std::string xml_file(value.substr(4));
  if (xml_file.empty()) {
    PrintError(arg, "requires a file name after xml:", from_env);
    return false;
  }
  // Need an absolute file.
  if (xml_file[0] != '/') {
    char* cwd = getcwd(nullptr, 0);
    if (cwd == nullptr) {
      PrintError(arg,
                 std::string("cannot get absolute pathname, getcwd() is failing: ") +
                     strerror(errno) + '\n',
                 from_env);
      return false;
    }
    xml_file = std::string(cwd) + '/' + xml_file;
    free(cwd);
  }

  // If the output file is a directory, add the name of a file.
  if (xml_file.back() == '/') {
    xml_file += "test_details.xml";
  }
  strings_.find("xml_file")->second = xml_file;
  return true;
}

bool Options::HandleArg(const std::string& arg, const std::string& value, const ArgInfo& info,
                        bool from_env) {
  if (info.flags & FLAG_INCOMPATIBLE) {
    PrintError(arg, "is not compatible with isolation runs.", from_env);
    return false;
  }

  if (info.flags & FLAG_TAKES_VALUE) {
    if ((info.flags & FLAG_REQUIRES_VALUE) && value.empty()) {
      PrintError(arg, "requires an argument.", from_env);
      return false;
    }

    if (info.func != nullptr && !(this->*(info.func))(arg, value, from_env)) {
      return false;
    }
  } else if (!value.empty()) {
    PrintError(arg, "does not take an argument.", from_env);
    return false;
  } else if (info.func != nullptr) {
    return (this->*(info.func))(arg, value, from_env);
  }
  return true;
}

bool Options::Process(const std::vector<const char*>& args, std::vector<const char*>* child_args) {
  // Initialize the variables.
  job_count_ = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
  num_iterations_ = ::testing::GTEST_FLAG(repeat);
  numerics_.clear();
  numerics_["deadline_threshold_ms"] = kDefaultDeadlineThresholdMs;
  numerics_["slow_threshold_ms"] = kDefaultSlowThresholdMs;
  numerics_["gtest_shard_index"] = 0;
  numerics_["gtest_total_shards"] = 0;
  strings_.clear();
  strings_["gtest_color"] = ::testing::GTEST_FLAG(color);
  strings_["xml_file"] = ::testing::GTEST_FLAG(output);
  strings_["gtest_filter"] = "";
  bools_.clear();
  bools_["gtest_print_time"] = ::testing::GTEST_FLAG(print_time);
  bools_["gtest_format"] = true;
  bools_["no_gtest_format"] = false;
  bools_["gtest_also_run_disabled_tests"] = ::testing::GTEST_FLAG(also_run_disabled_tests);
  bools_["gtest_list_tests"] = false;

  child_args->clear();

  // Loop through all of the possible environment variables.
  for (const auto& entry : kArgs) {
    if (entry.second.flags & FLAG_ENVIRONMENT_VARIABLE) {
      std::string variable(entry.first);
      std::transform(variable.begin(), variable.end(), variable.begin(),
                     [](char c) { return std::toupper(c); });
      char* env = getenv(variable.c_str());
      if (env == nullptr) {
        continue;
      }
      std::string value(env);
      if (!HandleArg(entry.first, value, entry.second, true)) {
        return false;
      }
    }
  }

  child_args->push_back(args[0]);

  // Assumes the first value is not an argument, so skip it.
  for (size_t i = 1; i < args.size(); i++) {
    // Special handle of -j or -jXX.
    if (strncmp(args[i], "-j", 2) == 0) {
      const char* value = &args[i][2];
      if (*value == '\0') {
        // Get the next argument.
        if (i == args.size() - 1) {
          printf("-j requires an argument.\n");
          return false;
        }
        i++;
        value = args[i];
      }
      if (!GetNumeric<size_t>("-j", value, &job_count_, false)) {
        return false;
      }
    } else if (strncmp("--", args[i], 2) == 0) {
      // See if this is a name=value argument.
      std::string name;
      std::string value;
      const char* equal = strchr(args[i], '=');
      if (equal != nullptr) {
        name = std::string(&args[i][2], static_cast<size_t>(equal - args[i]) - 2);
        value = equal + 1;
      } else {
        name = args[i] + 2;
      }
      auto entry = kArgs.find(name);
      if (entry == kArgs.end()) {
        printf("Unknown argument: %s\n", args[i]);
        return false;
      }

      if (entry->second.flags & FLAG_CHILD) {
        child_args->push_back(args[i]);
      }

      if (!HandleArg(name, value, entry->second)) {
        return false;
      }
    } else if (args[i][0] == '-') {
      printf("Unknown argument: %s\n", args[i]);
      return false;
    } else {
      printf("Unexpected argument '%s'\n", args[i]);
      return false;
    }
  }

  // If no_gtest_format was specified, it overrides gtest_format.
  if (bools_.at("no_gtest_format")) {
    bools_["gtest_format"] = false;
  }
  return true;
}

}  // namespace gtest_extras
}  // namespace android
