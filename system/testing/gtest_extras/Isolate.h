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

#include <poll.h>
#include <stdint.h>
#include <sys/types.h>

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "Color.h"
#include "Options.h"
#include "Test.h"

namespace android {
namespace gtest_extras {

class Isolate {
 public:
  Isolate(const Options& options, const std::vector<const char*>& child_args)
      : options_(options), child_args_(child_args) {}

  void EnumerateTests();

  int Run();

 private:
  struct ResultsType {
    const char* color;
    const char* prefix;
    const char* list_desc;
    const char* title;
    bool (*match_func)(const Test&);
    void (*print_func)(const Options&, const Test&);
  };

  size_t CheckTestsFinished();

  void CheckTestsTimeout();

  int ChildProcessFn(const std::tuple<std::string, std::string>& test);

  void HandleSignals();

  void LaunchTests();

  void ReadTestsOutput();

  void RunAllTests();

  void PrintFooter(uint64_t elapsed_time_ns);

  void PrintResults(size_t total, const ResultsType& results, std::string* footer);

  void WriteXmlResults(uint64_t elapsed_time_ns, time_t start_time);

  static std::string GetTestName(const std::tuple<std::string, std::string>& test) {
    return std::get<0>(test) + std::get<1>(test);
  }

  const Options& options_;
  const std::vector<const char*>& child_args_;

  size_t total_suites_ = 0;
  size_t total_tests_ = 0;
  size_t total_disable_tests_ = 0;
  size_t total_pass_tests_;
  size_t total_xpass_tests_;
  size_t total_fail_tests_;
  size_t total_xfail_tests_;
  size_t total_timeout_tests_;
  size_t total_slow_tests_;
  size_t total_skipped_tests_;
  size_t cur_test_index_ = 0;

  uint64_t slow_threshold_ns_;
  uint64_t deadline_threshold_ns_;
  std::vector<std::tuple<std::string, std::string>> tests_;

  std::vector<Test*> running_;
  std::vector<pollfd> running_pollfds_;
  std::vector<size_t> running_indices_;
  std::unordered_map<pid_t, std::unique_ptr<Test>> running_by_pid_;
  std::map<size_t, Test*> running_by_test_index_;

  std::map<size_t, std::unique_ptr<Test>> finished_;

  static constexpr useconds_t MIN_USECONDS_WAIT = 1000;

  static ResultsType SlowResults;
  static ResultsType XpassFailResults;
  static ResultsType FailResults;
  static ResultsType TimeoutResults;
  static ResultsType SkippedResults;
};

}  // namespace gtest_extras
}  // namespace android
