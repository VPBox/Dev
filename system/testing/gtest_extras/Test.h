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

#include <string>
#include <tuple>

#include <android-base/unique_fd.h>

namespace android {
namespace gtest_extras {

enum TestResult : uint8_t {
  TEST_NONE = 0,
  TEST_PASS,
  TEST_XPASS,
  TEST_FAIL,
  TEST_XFAIL,
  TEST_TIMEOUT,
  TEST_SKIPPED,
};

class Test {
 public:
  Test(std::tuple<std::string, std::string>& test, size_t test_index, size_t run_index, int fd);

  void PrintGtestFormat();

  void Print(bool gtest_format);

  void Stop();

  bool Read();

  void ReadUntilClosed();

  void CloseFd();

  void SetResultFromOutput();

  void AppendOutput(std::string& output) { output_ += output; }
  void AppendOutput(const char* output) { output_ += output; }

  uint64_t RunTimeNs() const { return end_ns_ - start_ns_; }
  uint64_t ElapsedNs(uint64_t cur_ns) const { return cur_ns - start_ns_; }

  bool ExpectFail() const { return test_name_.find("xfail") == 0; }

  const std::string& suite_name() const { return suite_name_; }
  const std::string& test_name() const { return test_name_; }
  const std::string& name() const { return name_; }

  size_t test_index() const { return test_index_; }
  size_t run_index() const { return run_index_; }

  int fd() const { return fd_; }

  uint64_t start_ns() const { return start_ns_; }

  uint64_t end_ns() const { return end_ns_; }
  void set_end_ns(uint64_t end_ns) { end_ns_ = end_ns; }

  TestResult result() const { return result_; }
  void set_result(TestResult result) { result_ = result; }

  void set_slow(bool slow) { slow_ = slow; }
  bool slow() const { return slow_; }

  const std::string& output() const { return output_; }

 private:
  std::string suite_name_;
  std::string test_name_;
  std::string name_;
  size_t test_index_;  // Index into test list.
  size_t run_index_;   // Index into running list.
  android::base::unique_fd fd_;

  uint64_t start_ns_;
  uint64_t end_ns_ = 0;
  bool slow_ = false;

  TestResult result_ = TEST_NONE;
  std::string output_;
};

}  // namespace gtest_extras
}  // namespace android
