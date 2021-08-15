/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <fcntl.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/strings.h>
#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include "NanoTime.h"

// Change the slow threshold for these tests since a few can take around
// 20 seconds.
extern "C" bool GetInitialArgs(const char*** args, size_t* num_args) {
  static const char* initial_args[] = {"--slow_threshold_ms=25000"};
  *args = initial_args;
  *num_args = 1;
  return true;
}

namespace android {
namespace gtest_extras {

class SystemTests : public ::testing::Test {
 protected:
  void SetUp() override {
    raw_output_ = "";
    sanitized_output_ = "";
    exitcode_ = 0;
  }

  void SanitizeOutput();

  void Exec(std::vector<const char*> args);
  void ExecAndCapture(std::vector<const char*> args);
  void RunTest(const std::string& test_name, std::vector<const char*> extra_args = {});
  void RunTestCaptureFooter(const std::string& test_name, std::string* footer,
                            std::vector<const char*> extra_args = {});
  void Verify(const std::string& test_name, const std::string& expected_output,
              int expected_exitcode, std::vector<const char*> extra_args = {});

  std::string raw_output_;
  std::string sanitized_output_;
  int exitcode_;
  pid_t pid_;
  int fd_;
};

void SystemTests::SanitizeOutput() {
  // Change (100 ms to (XX ms
  sanitized_output_ =
      std::regex_replace(raw_output_, std::regex("\\(\\d+ ms(\\)|\\s|,)"), "(XX ms$1");

  // Change (elapsed time 100 ms to (elapsed time XX ms
  sanitized_output_ = std::regex_replace(
      sanitized_output_, std::regex("\\(elapsed time \\d+ ms(\\)|\\s|,)"), "(elapsed time XX ms$1");

  // Change stopped|timeout at 100 ms to stopped|timeout at XX ms
  sanitized_output_ = std::regex_replace(sanitized_output_,
                                         std::regex("(stopped|timeout) at \\d+ ms"), "$1 at XX ms");

  // Change any error message like .../file.cc:(200) to file:(XX)
  sanitized_output_ = std::regex_replace(
      sanitized_output_, std::regex("\\b([^/\\s]+/)*[^/\\s]+:\\(\\d+\\)\\s"), "file:(XX) ");
}

void SystemTests::Exec(std::vector<const char*> args) {
  int fds[2];
#if !defined(__APPLE__)
  ASSERT_NE(-1, pipe2(fds, O_NONBLOCK));
#else
  ASSERT_NE(-1, pipe(fds));
  ASSERT_NE(-1, fcntl(fds[0], F_SETFL, O_NONBLOCK));
#endif

  if ((pid_ = fork()) == 0) {
    // Run the test.
    close(fds[0]);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    ASSERT_NE(0, dup2(fds[1], STDOUT_FILENO));
    ASSERT_NE(0, dup2(fds[1], STDERR_FILENO));
    close(fds[1]);

    std::string exe_name = android::base::GetExecutablePath();
    args.insert(args.begin(), exe_name.c_str());
    args.push_back(nullptr);
    execv(args[0], reinterpret_cast<char* const*>(const_cast<char**>(args.data())));
    exit(1);
  }
  ASSERT_NE(-1, pid_);

  close(fds[1]);
  fd_ = fds[0];
}

void SystemTests::ExecAndCapture(std::vector<const char*> args) {
  Exec(args);

  int flags = fcntl(fd_, F_GETFL);
  ASSERT_NE(-1, flags);
  flags &= ~O_NONBLOCK;
  ASSERT_NE(-1, fcntl(fd_, F_SETFL, flags));
  ASSERT_TRUE(android::base::ReadFdToString(fd_, &raw_output_));
  close(fd_);

  int status;
  ASSERT_EQ(pid_, TEMP_FAILURE_RETRY(waitpid(pid_, &status, 0))) << "Test output:\n" << raw_output_;
  exitcode_ = WEXITSTATUS(status);
  SanitizeOutput();
}

void SystemTests::RunTest(const std::string& test_name, std::vector<const char*> extra_args) {
  std::vector<const char*> args;
  bool job_count = false;
  for (const auto& arg : extra_args) {
    if (strncmp(arg, "-j", 2) == 0) {
      job_count = true;
    }
    args.push_back(arg);
  }
  if (!job_count) {
    // Always set to only 20 jobs if no job count option is set.
    args.push_back("-j20");
  }
  args.push_back("--gtest_also_run_disabled_tests");
  std::string filter_arg("--gtest_filter=" + test_name);
  args.push_back(filter_arg.c_str());

  ExecAndCapture(args);
}

void SystemTests::RunTestCaptureFooter(const std::string& test_name, std::string* footer,
                                       std::vector<const char*> extra_args) {
  ASSERT_NO_FATAL_FAILURE(RunTest(test_name, extra_args));

  // Verify the order of the output messages.
  std::stringstream stream(sanitized_output_);
  std::string line;
  footer->clear();
  while (std::getline(stream, line, '\n')) {
    if (!footer->empty()) {
      *footer += line + '\n';
    } else if (android::base::StartsWith(line, "[  PASSED  ] ")) {
      *footer = line + '\n';
    }
  }
  ASSERT_FALSE(footer->empty()) << "Test output:\n" << raw_output_;
}

void SystemTests::Verify(const std::string& test_name, const std::string& expected_output,
                         int expected_exitcode, std::vector<const char*> extra_args) {
  ASSERT_NO_FATAL_FAILURE(RunTest(test_name, extra_args));
  ASSERT_EQ(expected_exitcode, exitcode_) << "Test output:\n" << raw_output_;
  if (!expected_output.empty()) {
    ASSERT_EQ(expected_output, sanitized_output_);
  }
}

TEST_F(SystemTests, verify_pass) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_pass (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_pass", expected, 0, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_pass_no_print_time) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_pass\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_pass", expected, 0,
             std::vector<const char*>{"--gtest_print_time=0", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_pass_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_pass\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[    OK    ]\x1B[m SystemTests.DISABLED_pass (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_pass", expected, 0,
             std::vector<const char*>{"--gtest_color=yes", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_pass_gtest_format) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] SystemTests.DISABLED_pass\n"
      "[       OK ] SystemTests.DISABLED_pass (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_pass", expected, 0));
}

TEST_F(SystemTests, verify_pass_gtest_format_no_print_time) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] SystemTests.DISABLED_pass\n"
      "[       OK ] SystemTests.DISABLED_pass\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_pass", expected, 0, std::vector<const char*>{"--gtest_print_time=0"}));
}

TEST_F(SystemTests, verify_pass_gtest_format_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_pass\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[ RUN      ]\x1B[m SystemTests.DISABLED_pass\n"
      "\x1B[0;32m[       OK ]\x1B[m SystemTests.DISABLED_pass (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_pass", expected, 0, std::vector<const char*>{"--gtest_color=yes"}));
}

TEST_F(SystemTests, verify_skip) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_skip_no_message\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  SKIPPED ] 1 test, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_skip_no_message", expected, 0,
                                 std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_skip_with_message) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_skip_with_message\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_with_message (XX ms)\n"
      "This is a skip message\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  SKIPPED ] 1 test, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_with_message\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_skip_with_message", expected, 0,
                                 std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_skip_no_print_time) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_skip_no_message\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  SKIPPED ] 1 test, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_skip_no_message", expected, 0,
             std::vector<const char*>{"--gtest_print_time=0", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_skip_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_skip_no_message\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[  SKIPPED ]\x1B[m SystemTests.DISABLED_skip_no_message (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 0 tests.\n"
      "\x1B[0;32m[  SKIPPED ]\x1B[m 1 test, listed below:\n"
      "\x1B[0;32m[  SKIPPED ]\x1B[m SystemTests.DISABLED_skip_no_message\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_skip_no_message", expected, 0,
             std::vector<const char*>{"--gtest_color=yes", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_skip_gtest_format) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_skip_no_message\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] SystemTests.DISABLED_skip_no_message\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  SKIPPED ] 1 test, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_skip_no_message", expected, 0));
}

TEST_F(SystemTests, verify_skip_gtest_format_with_message) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_skip_with_message\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] SystemTests.DISABLED_skip_with_message\n"
      "This is a skip message\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_with_message (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  SKIPPED ] 1 test, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_with_message\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_skip_with_message", expected, 0));
}

TEST_F(SystemTests, verify_skip_gtest_format_no_print_time) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_skip_no_message\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] SystemTests.DISABLED_skip_no_message\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  SKIPPED ] 1 test, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_skip_no_message\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_skip_no_message", expected, 0,
                                 std::vector<const char*>{"--gtest_print_time=0"}));
}

TEST_F(SystemTests, verify_skip_gtest_format_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_skip_no_message\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[ RUN      ]\x1B[m SystemTests.DISABLED_skip_no_message\n"
      "\x1B[0;32m[  SKIPPED ]\x1B[m SystemTests.DISABLED_skip_no_message (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 0 tests.\n"
      "\x1B[0;32m[  SKIPPED ]\x1B[m 1 test, listed below:\n"
      "\x1B[0;32m[  SKIPPED ]\x1B[m SystemTests.DISABLED_skip_no_message\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_skip_no_message", expected, 0,
                                 std::vector<const char*>{"--gtest_color=yes"}));
}

TEST_F(SystemTests, verify_xfail_fail_expect_to_fail) {
  std::string expected =
      "Note: Google Test filter = *.xfail_fail\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[    OK    ] DISABLED_SystemTestsXfail.xfail_fail (XX ms)\n"
      "file:(XX) Failure in test DISABLED_SystemTestsXfail.xfail_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "DISABLED_SystemTestsXfail.xfail_fail exited with exitcode 1.\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test. (1 expected failure)\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.xfail_fail", expected, 0, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_xfail_fail_expect_to_fail_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.xfail_fail\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[    OK    ]\x1B[m DISABLED_SystemTestsXfail.xfail_fail (XX ms)\n"
      "file:(XX) Failure in test DISABLED_SystemTestsXfail.xfail_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "DISABLED_SystemTestsXfail.xfail_fail exited with exitcode 1.\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 1 test. (1 expected failure)\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.xfail_fail", expected, 0,
             std::vector<const char*>{"--gtest_color=yes", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_xfail_fail_expect_to_fail_gtest_format) {
  std::string expected =
      "Note: Google Test filter = *.xfail_fail\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] DISABLED_SystemTestsXfail.xfail_fail\n"
      "file:(XX) Failure in test DISABLED_SystemTestsXfail.xfail_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "DISABLED_SystemTestsXfail.xfail_fail exited with exitcode 1.\n"
      "[       OK ] DISABLED_SystemTestsXfail.xfail_fail (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test. (1 expected failure)\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.xfail_fail", expected, 0));
}

TEST_F(SystemTests, verify_xfail_pass_expect_to_fail) {
  std::string expected =
      "Note: Google Test filter = *.xfail_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  FAILED  ] DISABLED_SystemTestsXfail.xfail_pass (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  FAILED  ] 1 test should have failed, listed below:\n"
      "[  FAILED  ] DISABLED_SystemTestsXfail.xfail_pass\n"
      "\n"
      " 1 SHOULD HAVE FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.xfail_pass", expected, 1, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_xfail_pass_expect_to_fail_gtest_format) {
  std::string expected =
      "Note: Google Test filter = *.xfail_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] DISABLED_SystemTestsXfail.xfail_pass\n"
      "[  FAILED  ] DISABLED_SystemTestsXfail.xfail_pass (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  FAILED  ] 1 test should have failed, listed below:\n"
      "[  FAILED  ] DISABLED_SystemTestsXfail.xfail_pass\n"
      "\n"
      " 1 SHOULD HAVE FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.xfail_pass", expected, 1));
}

TEST_F(SystemTests, verify_xfail_pass_expect_to_fail_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.xfail_pass\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m DISABLED_SystemTestsXfail.xfail_pass (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 0 tests.\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m 1 test should have failed, listed below:\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m DISABLED_SystemTestsXfail.xfail_pass\n"
      "\n"
      " 1 SHOULD HAVE FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.xfail_pass", expected, 1,
             std::vector<const char*>{"--gtest_color=yes", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_deathtest_pass) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_death_pass\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTestsDeathTest.DISABLED_death_pass (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_death_pass", expected, 0, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_fail) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_fail\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  FAILED  ] SystemTests.DISABLED_fail (XX ms)\n"
      "file:(XX) Failure in test SystemTests.DISABLED_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTests.DISABLED_fail exited with exitcode 1.\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  FAILED  ] 1 test, listed below:\n"
      "[  FAILED  ] SystemTests.DISABLED_fail\n"
      "\n"
      " 1 FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_fail", expected, 1, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_fail_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_fail\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m SystemTests.DISABLED_fail (XX ms)\n"
      "file:(XX) Failure in test SystemTests.DISABLED_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTests.DISABLED_fail exited with exitcode 1.\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 0 tests.\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m 1 test, listed below:\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m SystemTests.DISABLED_fail\n"
      "\n"
      " 1 FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_fail", expected, 1,
             std::vector<const char*>{"--gtest_color=yes", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_fail_gtest_format) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_fail\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[ RUN      ] SystemTests.DISABLED_fail\n"
      "file:(XX) Failure in test SystemTests.DISABLED_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTests.DISABLED_fail exited with exitcode 1.\n"
      "[  FAILED  ] SystemTests.DISABLED_fail (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  FAILED  ] 1 test, listed below:\n"
      "[  FAILED  ] SystemTests.DISABLED_fail\n"
      "\n"
      " 1 FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_fail", expected, 1));
}

TEST_F(SystemTests, verify_fail_gtest_format_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_fail\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[ RUN      ]\x1B[m SystemTests.DISABLED_fail\n"
      "file:(XX) Failure in test SystemTests.DISABLED_fail\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTests.DISABLED_fail exited with exitcode 1.\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m SystemTests.DISABLED_fail (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 0 tests.\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m 1 test, listed below:\n"
      "\x1B[0;31m[  FAILED  ]\x1B[m SystemTests.DISABLED_fail\n"
      "\n"
      " 1 FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_fail", expected, 1, std::vector<const char*>{"--gtest_color=yes"}));
}

TEST_F(SystemTests, verify_deathtest_fail) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_death_fail\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  FAILED  ] SystemTestsDeathTest.DISABLED_death_fail (XX ms)\n"
      "file:(XX) Failure in test SystemTestsDeathTest.DISABLED_death_fail\n"
      "Death test: DeathTestHelperFail()\n"
      "    Result: failed to die.\n"
      " Error msg:\n"
      "[  DEATH   ] \n"
      "SystemTestsDeathTest.DISABLED_death_fail exited with exitcode 1.\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  FAILED  ] 1 test, listed below:\n"
      "[  FAILED  ] SystemTestsDeathTest.DISABLED_death_fail\n"
      "\n"
      " 1 FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_death_fail", expected, 1, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_crash) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_crash\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  FAILED  ] SystemTests.DISABLED_crash (XX ms)\n"
#if defined(__APPLE__)
      "SystemTests.DISABLED_crash terminated by signal: Segmentation fault: 11.\n"
#else
      "SystemTests.DISABLED_crash terminated by signal: Segmentation fault.\n"
#endif
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  FAILED  ] 1 test, listed below:\n"
      "[  FAILED  ] SystemTests.DISABLED_crash\n"
      "\n"
      " 1 FAILED TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_crash", expected, 1, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_warning_slow) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_sleep5\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_sleep5 (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n"
      "[  SLOW    ] 1 test, listed below:\n"
      "[  SLOW    ] SystemTests.DISABLED_sleep5 (XX ms, exceeded 3000 ms)\n"
      "\n"
      " 1 SLOW TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_sleep5", expected, 0,
             std::vector<const char*>{"--slow_threshold_ms=3000", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_warning_slow_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_sleep5\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;32m[    OK    ]\x1B[m SystemTests.DISABLED_sleep5 (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 1 test.\n"
      "\x1B[0;33m[  SLOW    ]\x1B[m 1 test, listed below:\n"
      "\x1B[0;33m[  SLOW    ]\x1B[m SystemTests.DISABLED_sleep5 (XX ms, exceeded 3000 ms)\n"
      "\n"
      " 1 SLOW TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_sleep5", expected, 0,
             std::vector<const char*>{"--slow_threshold_ms=3000", "--gtest_color=yes",
                                      "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_timeout) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_sleep_forever\n"
      "[==========] Running 1 test from 1 test suite (20 jobs).\n"
      "[  TIMEOUT ] SystemTests.DISABLED_sleep_forever (XX ms)\n"
      "SystemTests.DISABLED_sleep_forever killed because of timeout at XX ms.\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n"
      "[  TIMEOUT ] 1 test, listed below:\n"
      "[  TIMEOUT ] SystemTests.DISABLED_sleep_forever (stopped at XX ms)\n"
      "\n"
      " 1 TIMEOUT TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_sleep_forever", expected, 1,
             std::vector<const char*>{"--deadline_threshold_ms=3000", "--no_gtest_format"}));
}

// Verify that tests that timeout do not get marked as slow too when
// another test is marked as slow.
TEST_F(SystemTests, verify_timeout_not_slow) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_sleep*\n"
      "[==========] Running 2 tests from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_sleep5 (XX ms)\n"
      "[  TIMEOUT ] SystemTests.DISABLED_sleep_forever (XX ms)\n"
      "SystemTests.DISABLED_sleep_forever killed because of timeout at XX ms.\n"
      "[==========] 2 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n"
      "[  SLOW    ] 1 test, listed below:\n"
      "[  SLOW    ] SystemTests.DISABLED_sleep5 (XX ms, exceeded 1000 ms)\n"
      "[  TIMEOUT ] 1 test, listed below:\n"
      "[  TIMEOUT ] SystemTests.DISABLED_sleep_forever (stopped at XX ms)\n"
      "\n"
      " 1 SLOW TEST\n"
      " 1 TIMEOUT TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_sleep*", expected, 1,
             std::vector<const char*>{"--slow_threshold_ms=1000", "--deadline_threshold_ms=10000",
                                      "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_timeout_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *.DISABLED_sleep_forever\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (20 jobs).\n"
      "\x1B[0;31m[  TIMEOUT ]\x1B[m SystemTests.DISABLED_sleep_forever (XX ms)\n"
      "SystemTests.DISABLED_sleep_forever killed because of timeout at XX ms.\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 0 tests.\n"
      "\x1B[0;31m[  TIMEOUT ]\x1B[m 1 test, listed below:\n"
      "\x1B[0;31m[  TIMEOUT ]\x1B[m SystemTests.DISABLED_sleep_forever (stopped at XX ms)\n"
      "\n"
      " 1 TIMEOUT TEST\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_sleep_forever", expected, 1,
             std::vector<const char*>{"--deadline_threshold_ms=3000", "--gtest_color=yes",
                                      "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_order_isolated) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_order_*\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_order_3 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_1 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_order_*", expected, 0, std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_order_not_isolated) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_order_*\n"
      "[==========] Running 3 tests from 1 test suite.\n"
      "[----------] Global test environment set-up.\n"
      "[----------] 3 tests from SystemTests\n"
      "[ RUN      ] SystemTests.DISABLED_order_1\n"
      "[       OK ] SystemTests.DISABLED_order_1 (XX ms)\n"
      "[ RUN      ] SystemTests.DISABLED_order_2\n"
      "[       OK ] SystemTests.DISABLED_order_2 (XX ms)\n"
      "[ RUN      ] SystemTests.DISABLED_order_3\n"
      "[       OK ] SystemTests.DISABLED_order_3 (XX ms)\n"
      "[----------] 3 tests from SystemTests (XX ms total)\n"
      "\n"
      "[----------] Global test environment tear-down\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_order_*", expected, 0,
                                 std::vector<const char*>{"--no_isolate", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_fail_ge10) {
  ASSERT_NO_FATAL_FAILURE(RunTest("*.DISABLED_fail_*"));
  // Verify the failed output at the end has no space in front.
  std::regex regex("\\n.*\\d+ FAILED TESTS\\n");
  std::cmatch match;
  ASSERT_TRUE(std::regex_search(sanitized_output_.c_str(), match, regex)) << "Test Output:\n"
                                                                          << raw_output_;
  ASSERT_EQ("\n10 FAILED TESTS\n", match[0]);
  ASSERT_NE(0, exitcode_);
}

TEST_F(SystemTests, verify_title_order) {
  std::string footer;
  ASSERT_NO_FATAL_FAILURE(RunTestCaptureFooter(
      "*.DISABLED_all_*", &footer,
      std::vector<const char*>{"--slow_threshold_ms=2000", "--deadline_threshold_ms=4000"}));

  ASSERT_EQ(
      "[  PASSED  ] 4 tests.\n"
      "[  SKIPPED ] 2 tests, listed below:\n"
      "[  SKIPPED ] SystemTests.DISABLED_all_skip_1\n"
      "[  SKIPPED ] SystemTests.DISABLED_all_skip_2\n"
      "[  SLOW    ] 2 tests, listed below:\n"
      "[  SLOW    ] SystemTests.DISABLED_all_slow_1 (XX ms, exceeded 2000 ms)\n"
      "[  SLOW    ] SystemTests.DISABLED_all_slow_2 (XX ms, exceeded 2000 ms)\n"
      "[  TIMEOUT ] 2 tests, listed below:\n"
      "[  TIMEOUT ] SystemTests.DISABLED_all_timeout_1 (stopped at XX ms)\n"
      "[  TIMEOUT ] SystemTests.DISABLED_all_timeout_2 (stopped at XX ms)\n"
      "[  FAILED  ] 2 tests, listed below:\n"
      "[  FAILED  ] SystemTests.DISABLED_all_fail_1\n"
      "[  FAILED  ] SystemTests.DISABLED_all_fail_2\n"
      "\n"
      " 2 SLOW TESTS\n"
      " 2 TIMEOUT TESTS\n"
      " 2 FAILED TESTS\n",
      footer);
}

TEST_F(SystemTests, verify_job_count_single) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_job_*\n"
      "[==========] Running 3 tests from 1 test suite (1 job).\n"
      "[    OK    ] SystemTests.DISABLED_job_1 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_3 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_job_*", expected, 0,
                                 std::vector<const char*>{"-j1", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_job_count_multiple) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_job_*\n"
      "[==========] Running 3 tests from 1 test suite (2 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_job_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_1 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_3 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NO_FATAL_FAILURE(Verify("*.DISABLED_job_*", expected, 0,
                                 std::vector<const char*>{"-j", "2", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_help) {
  // This tests verifies that the help options display the help for
  // the isolated test run, and for the gtest data.
  std::vector<const char*> help_args{"-h", "--help"};
  for (auto arg : help_args) {
    ASSERT_NO_FATAL_FAILURE(RunTest("*.DISABLED_pass", std::vector<const char*>{arg}));
    ASSERT_EQ(0, exitcode_) << "Test output:\n" << raw_output_;
    // First find something from the isolation help.
    std::size_t isolation_help = sanitized_output_.find("In isolation mode,");
    ASSERT_NE(std::string::npos, isolation_help) << "Cannot find isolation help:\n" << raw_output_;
    std::size_t gtest_help = sanitized_output_.find("Assertion Behavior:");
    ASSERT_NE(std::string::npos, gtest_help) << "Cannot find gtest help:\n" << raw_output_;

    ASSERT_GT(gtest_help, isolation_help) << "Gtest help before isolation help:\n" << raw_output_;
  }
}

TEST_F(SystemTests, verify_help_color) {
  // Verify that the color option does change the help display.
  std::vector<const char*> help_args{"-h", "--help"};
  for (auto arg : help_args) {
    ASSERT_NO_FATAL_FAILURE(
        RunTest("*.DISABLED_pass", std::vector<const char*>{arg, "--gtest_color=yes"}));
    ASSERT_EQ(0, exitcode_) << "Test output:\n" << raw_output_;
    // First find something from the isolation help that is in color.
    std::size_t isolation_help =
        sanitized_output_.find("Unit Test Options:\n\x1B[0;32m  -j \x1B[m");
    ASSERT_NE(std::string::npos, isolation_help) << "Cannot find isolation help:\n" << raw_output_;
    std::size_t gtest_help = sanitized_output_.find("\x1B[0;32m--gtest_list_tests\x1B[m");
    ASSERT_NE(std::string::npos, gtest_help) << "Cannot find gtest help:\n" << raw_output_;

    ASSERT_GT(gtest_help, isolation_help) << "Gtest help before isolation help:\n" << raw_output_;
  }
}

TEST_F(SystemTests, verify_repeat) {
  std::string expected =
      "Note: Google Test filter = *.DISABLED_order_*\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_order_3 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_1 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n"
      "\n"
      "Repeating all tests (iteration 2) . . .\n"
      "\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_order_3 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_1 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n"
      "\n"
      "Repeating all tests (iteration 3) . . .\n"
      "\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_order_3 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_order_1 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  uint64_t time_ns = NanoTime();
  ASSERT_NO_FATAL_FAILURE(
      Verify("*.DISABLED_order_*", expected, 0,
             std::vector<const char*>{"--gtest_repeat=3", "--no_gtest_format"}));
  time_ns = NanoTime() - time_ns;
  // Make sure that the total test time is about 18 seconds.
  double seconds = double(time_ns) / 1000000000;
  ASSERT_LE(18.0, seconds) << "Repeat test should take at least 18 seconds.\n"
                           << "Test output:\n"
                           << raw_output_;
  ASSERT_GT(20.0, seconds) << "Repeat test should take about 18 seconds.\n"
                           << "Test output:\n"
                           << raw_output_;
}

TEST_F(SystemTests, verify_results_as_tests_finish) {
  // This test verifies that test output comes out as the test finishes.
  Exec(std::vector<const char*>{"--gtest_filter=*.DISABLED_order_*",
                                "--gtest_also_run_disabled_tests", "-j20", "--no_gtest_format"});

  std::string output;
  std::vector<char> buffer(4096);
  uint64_t time_ns = NanoTime();
  while (true) {
    ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer.data(), buffer.size() - 1));
    if (bytes == -1 && errno == EAGAIN) {
      continue;
    }
    ASSERT_NE(-1, bytes);
    ASSERT_NE(0, bytes) << "Did not find test output before test finished:\n" << output;
    buffer[bytes] = '\0';
    output += buffer.data();
    // See if the output has come out now.
    if (output.find("[    OK    ] SystemTests.DISABLED_order_2") != std::string::npos) {
      uint64_t test_ns = NanoTime() - time_ns;
      double test_sec = double(test_ns) / 1000000000;
      // This should happen after 3 seconds, but before 4.5 seconds.
      ASSERT_LE(3.0, test_sec) << "Test output:\n" << output;
      ASSERT_GT(4.5, test_sec) << "Test output:\n" << output;
      break;
    }
  }

  // Read the rest of the output.
  while (true) {
    ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer.data(), buffer.size() - 1));
    if (bytes == -1 && errno == EAGAIN) {
      continue;
    }
    ASSERT_NE(-1, bytes);
    if (bytes == 0) {
      break;
    }
    buffer[bytes] = '\0';
    output += buffer.data();
  }
  close(fd_);
  time_ns = NanoTime() - time_ns;
  ASSERT_EQ(pid_, TEMP_FAILURE_RETRY(waitpid(pid_, nullptr, 0))) << "Test output:\n" << output;
  // Verify that the total test time is > 6 seconds.
  ASSERT_LE(6.0, double(time_ns) / 1000000000) << "Test output:\n" << output;
}

TEST_F(SystemTests, verify_xml) {
  std::string tmp_arg("--gtest_output=xml:");
  TemporaryFile tf;
  ASSERT_TRUE(tf.fd != -1);
  close(tf.fd);
  tmp_arg += tf.path;

  ASSERT_NO_FATAL_FAILURE(RunTest("*.DISABLED_xml_*", std::vector<const char*>{tmp_arg.c_str()}));
  ASSERT_EQ(1, exitcode_) << "Test output:\n" << raw_output_;

  // Check that the xml file exists.
  FILE* xml_file = fopen(tf.path, "r");
  ASSERT_TRUE(xml_file != nullptr) << "Failed to find xml file:\n" << raw_output_;
  // Read the entire file in.
  std::string xml_output;
  std::vector<char> buffer(4096);
  size_t bytes;
  while ((bytes = fread(buffer.data(), 1, buffer.size(), xml_file)) > 0) {
    xml_output += std::string(buffer.data(), bytes);
  }
  fclose(xml_file);
  unlink(tf.path);

  // Change time|timestamp="" to time|timestamp="XX"
  xml_output =
      std::regex_replace(xml_output, std::regex("(time|timestamp)=\"[^\"]+\""), "$1=\"XX\"");
  // Change ".*.cc:(XX) to "file:(XX)
  xml_output = std::regex_replace(xml_output, std::regex("\"([^/\\s]+/)*[^/\\s]+:\\(\\d+\\)\\s"),
                                  "\"file:(XX) ");

  std::string expected =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<testsuites tests=\"6\" failures=\"3\" disabled=\"0\" errors=\"0\" timestamp=\"XX\" "
      "time=\"XX\" name=\"AllTests\">\n"
      "  <testsuite name=\"SystemTestsXml1\" tests=\"2\" failures=\"1\" disabled=\"0\" "
      "errors=\"0\" time=\"XX\">\n"
      "    <testcase name=\"DISABLED_xml_1\" status=\"run\" time=\"XX\" "
      "classname=\"SystemTestsXml1\" />\n"
      "    <testcase name=\"DISABLED_xml_2\" status=\"run\" time=\"XX\" "
      "classname=\"SystemTestsXml1\">\n"
      "      <failure message=\"file:(XX) Failure in test SystemTestsXml1.DISABLED_xml_2\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTestsXml1.DISABLED_xml_2 exited with exitcode 1.\n"
      "\" type=\"\">\n"
      "      </failure>\n"
      "    </testcase>\n"
      "  </testsuite>\n"
      "  <testsuite name=\"SystemTestsXml2\" tests=\"2\" failures=\"1\" disabled=\"0\" "
      "errors=\"0\" time=\"XX\">\n"
      "    <testcase name=\"DISABLED_xml_1\" status=\"run\" time=\"XX\" "
      "classname=\"SystemTestsXml2\">\n"
      "      <failure message=\"file:(XX) Failure in test SystemTestsXml2.DISABLED_xml_1\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTestsXml2.DISABLED_xml_1 exited with exitcode 1.\n"
      "\" type=\"\">\n"
      "      </failure>\n"
      "    </testcase>\n"
      "    <testcase name=\"DISABLED_xml_2\" status=\"run\" time=\"XX\" "
      "classname=\"SystemTestsXml2\" />\n"
      "  </testsuite>\n"
      "  <testsuite name=\"SystemTestsXml3\" tests=\"2\" failures=\"1\" disabled=\"0\" "
      "errors=\"0\" time=\"XX\">\n"
      "    <testcase name=\"DISABLED_xml_1\" status=\"run\" time=\"XX\" "
      "classname=\"SystemTestsXml3\" />\n"
      "    <testcase name=\"DISABLED_xml_2\" status=\"run\" time=\"XX\" "
      "classname=\"SystemTestsXml3\">\n"
      "      <failure message=\"file:(XX) Failure in test SystemTestsXml3.DISABLED_xml_2\n"
      "Expected equality of these values:\n"
      "  1\n"
      "  0\n"
      "SystemTestsXml3.DISABLED_xml_2 exited with exitcode 1.\n"
      "\" type=\"\">\n"
      "      </failure>\n"
      "    </testcase>\n"
      "  </testsuite>\n"
      "</testsuites>\n";
  ASSERT_EQ(expected, xml_output);
}

TEST_F(SystemTests, verify_disabled_not_displayed_with_no_tests) {
  std::vector<const char*> args{"--gtest_filter=NO_TEST_FILTER_MATCH", "-j2"};

  ASSERT_NO_FATAL_FAILURE(ExecAndCapture(args));
  ASSERT_EQ(0, exitcode_);
  std::string expected =
      "Note: Google Test filter = NO_TEST_FILTER_MATCH\n"
      "[==========] Running 0 tests from 0 test suites (2 jobs).\n"
      "[==========] 0 tests from 0 test suites ran. (XX ms total)\n"
      "[  PASSED  ] 0 tests.\n";
  ASSERT_EQ(expected, sanitized_output_) << "Test output:\n" << raw_output_;
}

TEST_F(SystemTests, verify_disabled) {
  std::vector<const char*> args{"--gtest_filter=*always_pass", "-j2", "--no_gtest_format"};

  ASSERT_NO_FATAL_FAILURE(ExecAndCapture(args));
  ASSERT_EQ(0, exitcode_) << "Test output:\n" << raw_output_;
  std::string expected =
      "Note: Google Test filter = *always_pass\n"
      "[==========] Running 1 test from 1 test suite (2 jobs).\n"
      "[    OK    ] SystemTests.always_pass (XX ms)\n"
      "[==========] 1 test from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 1 test.\n"
      "\n"
      "  YOU HAVE 1 DISABLED TEST\n"
      "\n";
  ASSERT_EQ(expected, sanitized_output_);
}

TEST_F(SystemTests, verify_disabled_color) {
  std::vector<const char*> args{"--gtest_filter=*always_pass", "-j2", "--gtest_color=yes",
                                "--no_gtest_format"};

  ASSERT_NO_FATAL_FAILURE(ExecAndCapture(args));
  ASSERT_EQ(0, exitcode_) << "Test output:\n" << raw_output_;
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = *always_pass\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 1 test from 1 test suite (2 jobs).\n"
      "\x1B[0;32m[    OK    ]\x1B[m SystemTests.always_pass (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 1 test from 1 test suite ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 1 test.\n"
      "\n"
      "\x1B[0;33m  YOU HAVE 1 DISABLED TEST\n"
      "\n\x1B[m";
  ASSERT_EQ(expected, sanitized_output_);
}

TEST_F(SystemTests, verify_SIGINT) {
  // Verify that SIGINT kills all of the tests.
  Exec(std::vector<const char*>{"--gtest_filter=*.DISABLED_job*", "--gtest_also_run_disabled_tests",
                                "-j20"});
  // It is expected that all of the tests will be sleeping so nothing will
  // complete by the time the signal is sent.
  sleep(1);
  ASSERT_NE(-1, kill(pid_, SIGINT));

  std::string output;
  std::vector<char> buffer(4096);
  while (true) {
    ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer.data(), buffer.size() - 1));
    if (bytes == -1 && errno == EAGAIN) {
      continue;
    }
    ASSERT_NE(-1, bytes);
    if (bytes == 0) {
      break;
    }
    buffer[bytes] = '\0';
    output += buffer.data();
  }
  close(fd_);
  int status;
  ASSERT_EQ(pid_, TEMP_FAILURE_RETRY(waitpid(pid_, &status, 0))) << "Test output:\n" << output;
  ASSERT_EQ(
      "Note: Google Test filter = *.DISABLED_job*\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "Terminating due to signal...\n",
      output);
  ASSERT_EQ(1, WEXITSTATUS(status));
}

TEST_F(SystemTests, verify_SIGQUIT) {
  // Verify that SIGQUIT prints all of the running tests.
  Exec(std::vector<const char*>{"--gtest_filter=*.DISABLED_job*", "--gtest_also_run_disabled_tests",
                                "-j20", "--no_gtest_format"});
  // It is expected that all of the tests will be sleeping so nothing will
  // complete by the time the signal is sent.
  sleep(1);
  ASSERT_NE(-1, kill(pid_, SIGQUIT));

  std::vector<char> buffer(4096);
  while (true) {
    ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer.data(), buffer.size() - 1));
    if (bytes == -1 && errno == EAGAIN) {
      continue;
    }
    ASSERT_NE(-1, bytes);
    if (bytes == 0) {
      break;
    }
    buffer[bytes] = '\0';
    raw_output_ += buffer.data();
  }
  close(fd_);
  int status;
  ASSERT_EQ(pid_, TEMP_FAILURE_RETRY(waitpid(pid_, &status, 0))) << "Test output:\n" << raw_output_;
  SanitizeOutput();
  ASSERT_EQ(
      "Note: Google Test filter = *.DISABLED_job*\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "List of current running tests:\n"
      "  SystemTests.DISABLED_job_1 (elapsed time XX ms)\n"
      "  SystemTests.DISABLED_job_2 (elapsed time XX ms)\n"
      "  SystemTests.DISABLED_job_3 (elapsed time XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_2 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_3 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_job_1 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n",
      sanitized_output_);
  ASSERT_EQ(0, WEXITSTATUS(status));
}

TEST_F(SystemTests, verify_SIGQUIT_after_test_finish) {
  // Verify that SIGQUIT prints all of the tests after a test finishes.
  Exec(std::vector<const char*>{"--gtest_filter=*.DISABLED_sigquit_*",
                                "--gtest_also_run_disabled_tests", "-j20", "--no_gtest_format"});
  // It is expected that one tests will have finished, but the rest will still
  // be running.
  sleep(1);
  ASSERT_NE(-1, kill(pid_, SIGQUIT));

  std::vector<char> buffer(4096);
  while (true) {
    ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer.data(), buffer.size() - 1));
    if (bytes == -1 && errno == EAGAIN) {
      continue;
    }
    ASSERT_NE(-1, bytes);
    if (bytes == 0) {
      break;
    }
    buffer[bytes] = '\0';
    raw_output_ += buffer.data();
  }
  close(fd_);
  int status;
  ASSERT_EQ(pid_, TEMP_FAILURE_RETRY(waitpid(pid_, &status, 0))) << "Test output:\n" << raw_output_;
  SanitizeOutput();
  ASSERT_EQ(
      "Note: Google Test filter = *.DISABLED_sigquit_*\n"
      "[==========] Running 3 tests from 1 test suite (20 jobs).\n"
      "[    OK    ] SystemTests.DISABLED_sigquit_no_sleep (XX ms)\n"
      "List of current running tests:\n"
      "  SystemTests.DISABLED_sigquit_sleep_5 (elapsed time XX ms)\n"
      "  SystemTests.DISABLED_sigquit_sleep_6 (elapsed time XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_sigquit_sleep_5 (XX ms)\n"
      "[    OK    ] SystemTests.DISABLED_sigquit_sleep_6 (XX ms)\n"
      "[==========] 3 tests from 1 test suite ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n",
      sanitized_output_);
  ASSERT_EQ(0, WEXITSTATUS(status));
}

TEST_F(SystemTests, verify_memory) {
  // This test verifies that memory isn't leaking when running repeatedly.
  std::vector<const char*> args{"--gtest_filter=*.DISABLED_memory",
                                "--gtest_also_run_disabled_tests", "--gtest_repeat=400"};
  ASSERT_NO_FATAL_FAILURE(ExecAndCapture(args));
  ASSERT_EQ(0, exitcode_) << "Test output:\n" << raw_output_;
  std::vector<std::string> lines(android::base::Split(raw_output_, "\n"));

  constexpr static size_t kMaxLeakBytes = 32 * 1024;
  size_t memory_iteration = 0;
  size_t memory_start = 0;
  size_t memory_last = 0;
  for (auto& line : lines) {
    size_t memory;
    if (android::base::StartsWith(line, "Allocated ") &&
        sscanf(line.c_str(), "Allocated %zu", &memory) == 1) {
      if (memory_iteration == 0) {
        memory_start = memory;
      } else {
        // Check the increase from the last loop.
        if (memory > memory_last) {
          ASSERT_GT(kMaxLeakBytes, memory - memory_last)
              << "On iteration " << memory_iteration << " memory increased beyond expected value."
              << std::endl
              << "Last memory bytes " << memory_last << std::endl
              << "Current memory bytes " << memory;
        }
        // Check the increase from the first loop.
        if (memory > memory_start) {
          ASSERT_GT(kMaxLeakBytes, memory - memory_start)
              << "On iteration " << memory_iteration
              << " total memory increased beyond expected value." << std::endl
              << "Starting memory bytes " << memory_start << std::endl
              << "Current memory bytes " << memory;
        }
      }
      memory_last = memory;
      memory_iteration++;
    }
  }
  ASSERT_EQ(400, memory_iteration)
      << "Did not find the expected 400 lines of memory data." << std::endl
      << "Raw output:" << std::endl
      << raw_output_;
}

TEST_F(SystemTests, verify_sharding) {
  std::string expected =
      "Note: Google Test filter = SystemTestsShard*.DISABLED*\n"
      "Note: This is test shard 1 of 4\n"
      "[==========] Running 3 tests from 3 test suites (20 jobs).\n"
      "[    OK    ] SystemTestsShard1.DISABLED_case1_test1 (XX ms)\n"
      "[    OK    ] SystemTestsShard2.DISABLED_case2_test1 (XX ms)\n"
      "[    OK    ] SystemTestsShard3.DISABLED_case3_test1 (XX ms)\n"
      "[==========] 3 tests from 3 test suites ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NE(-1, setenv("GTEST_TOTAL_SHARDS", "4", 1));
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "0", 1));
  ASSERT_NO_FATAL_FAILURE(Verify("SystemTestsShard*.DISABLED*", expected, 0,
                                 std::vector<const char*>{"--no_gtest_format"}));

  expected =
      "Note: Google Test filter = SystemTestsShard*.DISABLED*\n"
      "Note: This is test shard 2 of 4\n"
      "[==========] Running 3 tests from 3 test suites (20 jobs).\n"
      "[    OK    ] SystemTestsShard1.DISABLED_case1_test2 (XX ms)\n"
      "[    OK    ] SystemTestsShard2.DISABLED_case2_test2 (XX ms)\n"
      "[    OK    ] SystemTestsShard3.DISABLED_case3_test2 (XX ms)\n"
      "[==========] 3 tests from 3 test suites ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "1", 1));
  ASSERT_NO_FATAL_FAILURE(Verify("SystemTestsShard*.DISABLED*", expected, 0,
                                 std::vector<const char*>{"--no_gtest_format"}));

  expected =
      "Note: Google Test filter = SystemTestsShard*.DISABLED*\n"
      "Note: This is test shard 3 of 4\n"
      "[==========] Running 3 tests from 3 test suites (20 jobs).\n"
      "[    OK    ] SystemTestsShard1.DISABLED_case1_test3 (XX ms)\n"
      "[    OK    ] SystemTestsShard2.DISABLED_case2_test3 (XX ms)\n"
      "[    OK    ] SystemTestsShard3.DISABLED_case3_test3 (XX ms)\n"
      "[==========] 3 tests from 3 test suites ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "2", 1));
  ASSERT_NO_FATAL_FAILURE(Verify("SystemTestsShard*.DISABLED*", expected, 0,
                                 std::vector<const char*>{"--no_gtest_format"}));

  expected =
      "Note: Google Test filter = SystemTestsShard*.DISABLED*\n"
      "Note: This is test shard 4 of 4\n"
      "[==========] Running 3 tests from 3 test suites (20 jobs).\n"
      "[    OK    ] SystemTestsShard1.DISABLED_case1_test4 (XX ms)\n"
      "[    OK    ] SystemTestsShard2.DISABLED_case2_test4 (XX ms)\n"
      "[    OK    ] SystemTestsShard3.DISABLED_case3_test4 (XX ms)\n"
      "[==========] 3 tests from 3 test suites ran. (XX ms total)\n"
      "[  PASSED  ] 3 tests.\n";
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "3", 1));
  ASSERT_NO_FATAL_FAILURE(Verify("SystemTestsShard*.DISABLED*", expected, 0,
                                 std::vector<const char*>{"--no_gtest_format"}));
}

TEST_F(SystemTests, verify_sharding_color) {
  std::string expected =
      "\x1B[0;33mNote: Google Test filter = SystemTestsShard*.DISABLED*\x1B[m\n"
      "\x1B[0;33mNote: This is test shard 1 of 4\x1B[m\n"
      "\x1B[0;32m[==========]\x1B[m Running 3 tests from 3 test suites (20 jobs).\n"
      "\x1B[0;32m[    OK    ]\x1B[m SystemTestsShard1.DISABLED_case1_test1 (XX ms)\n"
      "\x1B[0;32m[    OK    ]\x1B[m SystemTestsShard2.DISABLED_case2_test1 (XX ms)\n"
      "\x1B[0;32m[    OK    ]\x1B[m SystemTestsShard3.DISABLED_case3_test1 (XX ms)\n"
      "\x1B[0;32m[==========]\x1B[m 3 tests from 3 test suites ran. (XX ms total)\n"
      "\x1B[0;32m[  PASSED  ]\x1B[m 3 tests.\n";
  ASSERT_NE(-1, setenv("GTEST_TOTAL_SHARDS", "4", 1));
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "0", 1));
  ASSERT_NO_FATAL_FAILURE(
      Verify("SystemTestsShard*.DISABLED*", expected, 0,
             std::vector<const char*>{"--gtest_color=yes", "--no_gtest_format"}));
}

TEST_F(SystemTests, verify_sharding_error) {
  std::string expected =
      "Invalid environment variables: we require 0 <= GTEST_SHARD_INDEX < GTEST_TOTAL_SHARDS, but "
      "you have GTEST_SHARD_INDEX=4, GTEST_TOTAL_SHARDS=4\n";
  ASSERT_NE(-1, setenv("GTEST_TOTAL_SHARDS", "4", 1));
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "4", 1));
  ASSERT_NO_FATAL_FAILURE(Verify("SystemTestsShard*.DISABLED*", expected, 1));
}

TEST_F(SystemTests, verify_sharding_error_color) {
  std::string expected =
      "\x1B[0;31mInvalid environment variables: we require 0 <= GTEST_SHARD_INDEX < "
      "GTEST_TOTAL_SHARDS, but you have GTEST_SHARD_INDEX=4, GTEST_TOTAL_SHARDS=4\x1B[m\n";
  ASSERT_NE(-1, setenv("GTEST_TOTAL_SHARDS", "4", 1));
  ASSERT_NE(-1, setenv("GTEST_SHARD_INDEX", "4", 1));
  ASSERT_NO_FATAL_FAILURE(Verify("SystemTestsShard*.DISABLED*", expected, 1,
                                 std::vector<const char*>{"--gtest_color=yes"}));
}

// These tests are used by the verify_disabled tests.
TEST_F(SystemTests, always_pass) {}

TEST_F(SystemTests, DISABLED_always_pass) {}

// The tests listed below will not run by default. They are executed by
// the above tests.
TEST_F(SystemTests, DISABLED_pass) {}

TEST_F(SystemTests, DISABLED_fail) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_crash) {
  char* p = reinterpret_cast<char*>(static_cast<intptr_t>(atoi("0")));
  *p = 3;
}

TEST_F(SystemTests, DISABLED_sigquit_no_sleep) {}

TEST_F(SystemTests, DISABLED_sigquit_sleep_5) {
  sleep(5);
}

TEST_F(SystemTests, DISABLED_sigquit_sleep_6) {
  sleep(6);
}

TEST_F(SystemTests, DISABLED_sleep_forever) {
  while (true) {
    sleep(10000);
  }
}

TEST_F(SystemTests, DISABLED_sleep5) {
  sleep(5);
}

// These tests will finish 1, 2, 3 in non-isolated mode and 3, 2, 1 in isolated
// mode.
TEST_F(SystemTests, DISABLED_order_1) {
  sleep(6);
}

TEST_F(SystemTests, DISABLED_order_2) {
  sleep(3);
}

TEST_F(SystemTests, DISABLED_order_3) {}

TEST_F(SystemTests, DISABLED_fail_0) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_1) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_2) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_3) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_4) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_5) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_6) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_7) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_8) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_fail_9) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_all_pass_1) {}

TEST_F(SystemTests, DISABLED_all_pass_2) {}

TEST_F(SystemTests, DISABLED_all_skip_1) {
  GTEST_SKIP();
}

TEST_F(SystemTests, DISABLED_all_skip_2) {
  GTEST_SKIP() << "Skip message present";
}

TEST_F(SystemTests, DISABLED_all_slow_1) {
  sleep(3);
}

TEST_F(SystemTests, DISABLED_all_slow_2) {
  sleep(3);
}

TEST_F(SystemTests, DISABLED_all_fail_1) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_all_fail_2) {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTests, DISABLED_all_timeout_1) {
  sleep(6);
}

TEST_F(SystemTests, DISABLED_all_timeout_2) {
  sleep(6);
}

TEST_F(SystemTests, DISABLED_job_1) {
  sleep(5);
}

TEST_F(SystemTests, DISABLED_job_2) {
  sleep(3);
}

TEST_F(SystemTests, DISABLED_job_3) {
  sleep(4);
}

TEST_F(SystemTests, DISABLED_skip_no_message) {
  GTEST_SKIP();
}

TEST_F(SystemTests, DISABLED_skip_with_message) {
  GTEST_SKIP() << "This is a skip message";
}

class DISABLED_SystemTestsXfail : public ::testing::Test {};

TEST_F(DISABLED_SystemTestsXfail, xfail_fail) {
  ASSERT_EQ(1, 0);
}

TEST_F(DISABLED_SystemTestsXfail, xfail_pass) {}

class SystemTestsDeathTest : public ::testing::Test {
 protected:
  virtual void SetUp() { ::testing::FLAGS_gtest_death_test_style = "threadsafe"; }
};

static void DeathTestHelperPass() {
  ASSERT_EQ(1, 1);
  exit(0);
}

TEST_F(SystemTestsDeathTest, DISABLED_death_pass) {
  ASSERT_EXIT(DeathTestHelperPass(), ::testing::ExitedWithCode(0), "");
}

static void DeathTestHelperFail() {
  ASSERT_EQ(1, 0);
}

TEST_F(SystemTestsDeathTest, DISABLED_death_fail) {
  ASSERT_EXIT(DeathTestHelperFail(), ::testing::ExitedWithCode(0), "");
}

TEST(SystemTestsXml1, DISABLED_xml_1) {}

TEST(SystemTestsXml1, DISABLED_xml_2) {
  ASSERT_EQ(1, 0);
}

TEST(SystemTestsXml2, DISABLED_xml_1) {
  ASSERT_EQ(1, 0);
}

TEST(SystemTestsXml2, DISABLED_xml_2) {}

TEST(SystemTestsXml3, DISABLED_xml_1) {}

TEST(SystemTestsXml3, DISABLED_xml_2) {
  ASSERT_EQ(1, 0);
}

TEST(SystemTestsMemory, DISABLED_memory) {
#if !defined(__APPLE__)
  struct mallinfo info = mallinfo();
#if defined(__ANDROID__)
  printf("Allocated %zu\n", info.uordblks);
#else
  printf("Allocated %d\n", info.uordblks);
#endif
#else
  printf("Allocated 0\n");
#endif
}

TEST(SystemTestsShard1, DISABLED_case1_test1) {}

TEST(SystemTestsShard1, DISABLED_case1_test2) {}

TEST(SystemTestsShard1, DISABLED_case1_test3) {}

TEST(SystemTestsShard1, DISABLED_case1_test4) {}

TEST(SystemTestsShard2, DISABLED_case2_test1) {}

TEST(SystemTestsShard2, DISABLED_case2_test2) {}

TEST(SystemTestsShard2, DISABLED_case2_test3) {}

TEST(SystemTestsShard2, DISABLED_case2_test4) {}

TEST(SystemTestsShard3, DISABLED_case3_test1) {}

TEST(SystemTestsShard3, DISABLED_case3_test2) {}

TEST(SystemTestsShard3, DISABLED_case3_test3) {}

TEST(SystemTestsShard3, DISABLED_case3_test4) {}

}  // namespace gtest_extras
}  // namespace android
