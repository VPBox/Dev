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
#include <fcntl.h>
#include <inttypes.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <atomic>
#include <string>
#include <tuple>
#include <vector>

#include <android-base/logging.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <gtest/gtest.h>

#include "Color.h"
#include "Isolate.h"
#include "NanoTime.h"
#include "Test.h"

namespace android {
namespace gtest_extras {

static std::atomic_int g_signal;

static void SignalHandler(int sig) {
  g_signal = sig;
}

static void RegisterSignalHandler() {
  auto ret = signal(SIGINT, SignalHandler);
  if (ret == SIG_ERR) {
    PLOG(FATAL) << "Setting up SIGINT handler failed";
  }
  ret = signal(SIGQUIT, SignalHandler);
  if (ret == SIG_ERR) {
    PLOG(FATAL) << "Setting up SIGQUIT handler failed";
  }
}

static void UnregisterSignalHandler() {
  auto ret = signal(SIGINT, SIG_DFL);
  if (ret == SIG_ERR) {
    PLOG(FATAL) << "Disabling SIGINT handler failed";
  }
  ret = signal(SIGQUIT, SIG_DFL);
  if (ret == SIG_ERR) {
    PLOG(FATAL) << "Disabling SIGQUIT handler failed";
  }
}

static std::string PluralizeString(size_t value, const char* name, bool uppercase = false) {
  std::string string(std::to_string(value) + name);
  if (value != 1) {
    if (uppercase) {
      string += 'S';
    } else {
      string += 's';
    }
  }
  return string;
}

void Isolate::EnumerateTests() {
  // Only apply --gtest_filter if present. This is the only option that changes
  // what tests are listed.
  std::string command(child_args_[0]);
  if (!options_.filter().empty()) {
    command += " --gtest_filter=" + options_.filter();
  }
  command += " --gtest_list_tests";
#if defined(__APPLE__)
  FILE* fp = popen(command.c_str(), "r");
#else
  FILE* fp = popen(command.c_str(), "re");
#endif
  if (fp == nullptr) {
    PLOG(FATAL) << "Unexpected failure from popen";
  }

  size_t total_shards = options_.total_shards();
  bool sharded = total_shards > 1;
  size_t test_count = 0;
  if (sharded) {
    test_count = options_.shard_index() + 1;
  }

  bool skip_until_next_suite = false;
  std::string suite_name;
  char* buffer = nullptr;
  size_t buffer_len = 0;
  bool new_suite = false;
  while (getline(&buffer, &buffer_len, fp) > 0) {
    if (buffer[0] != ' ') {
      // This is the case name.
      suite_name = buffer;
      auto space_index = suite_name.find(' ');
      if (space_index != std::string::npos) {
        suite_name.erase(space_index);
      }
      if (suite_name.back() == '\n') {
        suite_name.resize(suite_name.size() - 1);
      }

      if (!options_.allow_disabled_tests() && android::base::StartsWith(suite_name, "DISABLED_")) {
        // This whole set of tests have been disabled, skip them all.
        skip_until_next_suite = true;
      } else {
        new_suite = true;
        skip_until_next_suite = false;
      }
    } else if (buffer[0] == ' ' && buffer[1] == ' ') {
      if (!skip_until_next_suite) {
        std::string test_name = &buffer[2];
        auto space_index = test_name.find(' ');
        if (space_index != std::string::npos) {
          test_name.erase(space_index);
        }
        if (test_name.back() == '\n') {
          test_name.resize(test_name.size() - 1);
        }
        if (options_.allow_disabled_tests() || !android::base::StartsWith(test_name, "DISABLED_")) {
          if (!sharded || --test_count == 0) {
            tests_.push_back(std::make_tuple(suite_name, test_name));
            total_tests_++;
            if (new_suite) {
              // Only increment the number of suites when we find at least one test
              // for the suites.
              total_suites_++;
              new_suite = false;
            }
            if (sharded) {
              test_count = total_shards;
            }
          }
        } else {
          total_disable_tests_++;
        }
      } else {
        total_disable_tests_++;
      }
    } else {
      printf("Unexpected output from test listing.\nCommand:\n%s\nLine:\n%s\n", command.c_str(),
             buffer);
      exit(1);
    }
  }
  free(buffer);
  if (pclose(fp) == -1) {
    PLOG(FATAL) << "Unexpected failure from pclose";
  }
}

int Isolate::ChildProcessFn(const std::tuple<std::string, std::string>& test) {
  // Make sure the filter is only coming from our command-line option.
  unsetenv("GTEST_FILTER");

  // Add the filter argument.
  std::vector<const char*> args(child_args_);
  std::string filter("--gtest_filter=" + GetTestName(test));
  args.push_back(filter.c_str());

  int argc = args.size();
  // Add the null terminator.
  args.push_back(nullptr);
  ::testing::InitGoogleTest(&argc, const_cast<char**>(args.data()));
  return RUN_ALL_TESTS();
}

void Isolate::LaunchTests() {
  while (!running_indices_.empty() && cur_test_index_ < tests_.size()) {
    android::base::unique_fd read_fd, write_fd;
    if (!Pipe(&read_fd, &write_fd)) {
      PLOG(FATAL) << "Unexpected failure from pipe";
    }
    if (fcntl(read_fd.get(), F_SETFL, O_NONBLOCK) == -1) {
      PLOG(FATAL) << "Unexpected failure from fcntl";
    }

    pid_t pid = fork();
    if (pid == -1) {
      PLOG(FATAL) << "Unexpected failure from fork";
    }
    if (pid == 0) {
      read_fd.reset();
      close(STDOUT_FILENO);
      close(STDERR_FILENO);
      if (dup2(write_fd, STDOUT_FILENO) == -1) {
        exit(1);
      }
      if (dup2(write_fd, STDERR_FILENO) == -1) {
        exit(1);
      }
      UnregisterSignalHandler();
      exit(ChildProcessFn(tests_[cur_test_index_]));
    }

    size_t run_index = running_indices_.back();
    running_indices_.pop_back();
    Test* test = new Test(tests_[cur_test_index_], cur_test_index_, run_index, read_fd.release());
    running_by_pid_.emplace(pid, test);
    running_[run_index] = test;
    running_by_test_index_[cur_test_index_] = test;

    pollfd* pollfd = &running_pollfds_[run_index];
    pollfd->fd = test->fd();
    pollfd->events = POLLIN;
    cur_test_index_++;
  }
}

void Isolate::ReadTestsOutput() {
  int ready = poll(running_pollfds_.data(), running_pollfds_.size(), 0);
  if (ready <= 0) {
    return;
  }

  for (size_t i = 0; i < running_pollfds_.size(); i++) {
    pollfd* pfd = &running_pollfds_[i];
    if (pfd->revents & POLLIN) {
      Test* test = running_[i];
      if (!test->Read()) {
        test->CloseFd();
        pfd->fd = 0;
        pfd->events = 0;
      }
    }
    pfd->revents = 0;
  }
}

size_t Isolate::CheckTestsFinished() {
  size_t finished_tests = 0;
  int status;
  pid_t pid;
  while ((pid = TEMP_FAILURE_RETRY(waitpid(-1, &status, WNOHANG))) > 0) {
    auto entry = running_by_pid_.find(pid);
    if (entry == running_by_pid_.end()) {
      LOG(FATAL) << "Pid " << pid << " was not spawned by the isolation framework.";
    }

    std::unique_ptr<Test>& test_ptr = entry->second;
    Test* test = test_ptr.get();
    test->Stop();

    // Read any leftover data.
    test->ReadUntilClosed();
    if (test->result() == TEST_NONE) {
      if (WIFSIGNALED(status)) {
        std::string output(test->name() + " terminated by signal: " + strsignal(WTERMSIG(status)) +
                           ".\n");
        test->AppendOutput(output);
        test->set_result(TEST_FAIL);
      } else {
        int exit_code = WEXITSTATUS(status);
        if (exit_code != 0) {
          std::string output(test->name() + " exited with exitcode " + std::to_string(exit_code) +
                             ".\n");
          test->AppendOutput(output);
          test->set_result(TEST_FAIL);
        } else {
          // Set the result based on the output, since skipped tests and
          // passing tests have the same exit status.
          test->SetResultFromOutput();
        }
      }
    } else if (test->result() == TEST_TIMEOUT) {
      uint64_t time_ms = options_.deadline_threshold_ms();
      std::string timeout_str(test->name() + " killed because of timeout at " +
                              std::to_string(time_ms) + " ms.\n");
      test->AppendOutput(timeout_str);
    }

    if (test->ExpectFail()) {
      if (test->result() == TEST_FAIL) {
        // The test is expected to fail, it failed.
        test->set_result(TEST_XFAIL);
      } else if (test->result() == TEST_PASS) {
        // The test is expected to fail, it passed.
        test->set_result(TEST_XPASS);
      }
    }

    test->Print(options_.gtest_format());

    switch (test->result()) {
      case TEST_PASS:
        total_pass_tests_++;
        if (test->slow()) {
          total_slow_tests_++;
        }
        break;
      case TEST_XPASS:
        total_xpass_tests_++;
        break;
      case TEST_FAIL:
        total_fail_tests_++;
        break;
      case TEST_TIMEOUT:
        total_timeout_tests_++;
        break;
      case TEST_XFAIL:
        total_xfail_tests_++;
        break;
      case TEST_SKIPPED:
        total_skipped_tests_++;
        break;
      case TEST_NONE:
        LOG(FATAL) << "Test result is TEST_NONE, this should not be possible.";
    }
    finished_tests++;
    size_t test_index = test->test_index();
    finished_.emplace(test_index, test_ptr.release());
    running_indices_.push_back(test->run_index());

    // Remove it from all of the running indices.
    size_t run_index = test->run_index();
    if (running_by_pid_.erase(pid) != 1) {
      printf("Internal error: Erasing pid %d from running_by_pid_ incorrect\n", pid);
    }
    if (running_by_test_index_.erase(test_index) == 0) {
      printf("Internal error: Erasing test_index %zu from running_by_pid_ incorrect\n", test_index);
    }
    running_[run_index] = nullptr;
    running_pollfds_[run_index] = {};
  }

  // The only valid error case is if ECHILD is returned because there are
  // no more processes left running.
  if (pid == -1 && errno != ECHILD) {
    PLOG(FATAL) << "Unexpected failure from waitpid";
  }
  return finished_tests;
}

void Isolate::CheckTestsTimeout() {
  for (auto& entry : running_by_pid_) {
    Test* test = entry.second.get();
    if (test->result() == TEST_TIMEOUT) {
      continue;
    }

    if (NanoTime() > test->start_ns() + deadline_threshold_ns_) {
      test->set_result(TEST_TIMEOUT);
      // Do not mark this as slow and timed out.
      test->set_slow(false);
      // Test gets cleaned up in CheckTestsFinished.
      kill(entry.first, SIGKILL);
    } else if (!test->slow() && NanoTime() > test->start_ns() + slow_threshold_ns_) {
      // Mark the test as running slow.
      test->set_slow(true);
    }
  }
}

void Isolate::HandleSignals() {
  int signal = g_signal.exchange(0);
  if (signal == SIGINT) {
    printf("Terminating due to signal...\n");
    for (auto& entry : running_by_pid_) {
      kill(entry.first, SIGKILL);
    }
    exit(1);
  } else if (signal == SIGQUIT) {
    printf("List of current running tests:\n");
    for (const auto& entry : running_by_test_index_) {
      const Test* test = entry.second;
      uint64_t run_time_ms = (NanoTime() - test->start_ns()) / kNsPerMs;
      printf("  %s (elapsed time %" PRId64 " ms)\n", test->name().c_str(), run_time_ms);
    }
  }
}

void Isolate::RunAllTests() {
  total_pass_tests_ = 0;
  total_xpass_tests_ = 0;
  total_fail_tests_ = 0;
  total_xfail_tests_ = 0;
  total_timeout_tests_ = 0;
  total_slow_tests_ = 0;
  total_skipped_tests_ = 0;

  running_by_test_index_.clear();

  size_t job_count = options_.job_count();
  running_.clear();
  running_.resize(job_count);
  running_pollfds_.resize(job_count);
  memset(running_pollfds_.data(), 0, running_pollfds_.size() * sizeof(pollfd));
  running_indices_.clear();
  for (size_t i = 0; i < job_count; i++) {
    running_indices_.push_back(i);
  }

  finished_.clear();

  size_t finished = 0;
  cur_test_index_ = 0;
  while (finished < tests_.size()) {
    LaunchTests();

    ReadTestsOutput();

    finished += CheckTestsFinished();

    CheckTestsTimeout();

    HandleSignals();

    usleep(MIN_USECONDS_WAIT);
  }
}

void Isolate::PrintResults(size_t total, const ResultsType& results, std::string* footer) {
  ColoredPrintf(results.color, results.prefix);
  if (results.list_desc != nullptr) {
    printf(" %s %s, listed below:\n", PluralizeString(total, " test").c_str(), results.list_desc);
  } else {
    printf(" %s, listed below:\n", PluralizeString(total, " test").c_str());
  }
  for (const auto& entry : finished_) {
    const Test* test = entry.second.get();
    if (results.match_func(*test)) {
      ColoredPrintf(results.color, results.prefix);
      printf(" %s", test->name().c_str());
      if (results.print_func != nullptr) {
        results.print_func(options_, *test);
      }
      printf("\n");
    }
  }

  if (results.title == nullptr) {
    return;
  }

  if (total < 10) {
    *footer += ' ';
  }
  *footer +=
      PluralizeString(total, (std::string(" ") + results.title + " TEST").c_str(), true) + '\n';
}

Isolate::ResultsType Isolate::SlowResults = {
    .color = COLOR_YELLOW,
    .prefix = "[  SLOW    ]",
    .list_desc = nullptr,
    .title = "SLOW",
    .match_func = [](const Test& test) { return test.slow(); },
    .print_func =
        [](const Options& options, const Test& test) {
          printf(" (%" PRIu64 " ms, exceeded %" PRIu64 " ms)", test.RunTimeNs() / kNsPerMs,
                 options.slow_threshold_ms());
        },
};

Isolate::ResultsType Isolate::XpassFailResults = {
    .color = COLOR_RED,
    .prefix = "[  FAILED  ]",
    .list_desc = "should have failed",
    .title = "SHOULD HAVE FAILED",
    .match_func = [](const Test& test) { return test.result() == TEST_XPASS; },
    .print_func = nullptr,
};

Isolate::ResultsType Isolate::FailResults = {
    .color = COLOR_RED,
    .prefix = "[  FAILED  ]",
    .list_desc = nullptr,
    .title = "FAILED",
    .match_func = [](const Test& test) { return test.result() == TEST_FAIL; },
    .print_func = nullptr,
};

Isolate::ResultsType Isolate::TimeoutResults = {
    .color = COLOR_RED,
    .prefix = "[  TIMEOUT ]",
    .list_desc = nullptr,
    .title = "TIMEOUT",
    .match_func = [](const Test& test) { return test.result() == TEST_TIMEOUT; },
    .print_func =
        [](const Options&, const Test& test) {
          printf(" (stopped at %" PRIu64 " ms)", test.RunTimeNs() / kNsPerMs);
        },
};

Isolate::ResultsType Isolate::SkippedResults = {
    .color = COLOR_GREEN,
    .prefix = "[  SKIPPED ]",
    .list_desc = nullptr,
    .title = nullptr,
    .match_func = [](const Test& test) { return test.result() == TEST_SKIPPED; },
    .print_func = nullptr,
};

void Isolate::PrintFooter(uint64_t elapsed_time_ns) {
  ColoredPrintf(COLOR_GREEN, "[==========]");
  printf(" %s from %s ran. (%" PRId64 " ms total)\n",
         PluralizeString(total_tests_, " test").c_str(),
         PluralizeString(total_suites_, " test suite").c_str(), elapsed_time_ns / kNsPerMs);

  ColoredPrintf(COLOR_GREEN, "[  PASSED  ]");
  printf(" %s.", PluralizeString(total_pass_tests_ + total_xfail_tests_, " test").c_str());
  if (total_xfail_tests_ != 0) {
    printf(" (%s)", PluralizeString(total_xfail_tests_, " expected failure").c_str());
  }
  printf("\n");

  std::string footer;

  // Tests that were skipped.
  if (total_skipped_tests_ != 0) {
    PrintResults(total_skipped_tests_, SkippedResults, &footer);
  }

  // Tests that ran slow.
  if (total_slow_tests_ != 0) {
    PrintResults(total_slow_tests_, SlowResults, &footer);
  }

  // Tests that passed but should have failed.
  if (total_xpass_tests_ != 0) {
    PrintResults(total_xpass_tests_, XpassFailResults, &footer);
  }

  // Tests that timed out.
  if (total_timeout_tests_ != 0) {
    PrintResults(total_timeout_tests_, TimeoutResults, &footer);
  }

  // Tests that failed.
  if (total_fail_tests_ != 0) {
    PrintResults(total_fail_tests_, FailResults, &footer);
  }

  if (!footer.empty()) {
    printf("\n%s", footer.c_str());
  }

  if (total_disable_tests_ != 0) {
    if (footer.empty()) {
      printf("\n");
    }
    ColoredPrintf(COLOR_YELLOW, "  YOU HAVE %s\n\n",
                  PluralizeString(total_disable_tests_, " DISABLED TEST", true).c_str());
  }

  fflush(stdout);
}

std::string XmlEscape(const std::string& xml) {
  std::string escaped;
  escaped.reserve(xml.size());

  for (auto c : xml) {
    switch (c) {
      case '<':
        escaped.append("&lt;");
        break;
      case '>':
        escaped.append("&gt;");
        break;
      case '&':
        escaped.append("&amp;");
        break;
      case '\'':
        escaped.append("&apos;");
        break;
      case '"':
        escaped.append("&quot;");
        break;
      default:
        escaped.append(1, c);
        break;
    }
  }

  return escaped;
}

class TestResultPrinter : public ::testing::EmptyTestEventListener {
 public:
  TestResultPrinter() : pinfo_(nullptr) {}
  virtual void OnTestStart(const ::testing::TestInfo& test_info) {
    pinfo_ = &test_info;  // Record test_info for use in OnTestPartResult.
  }
  virtual void OnTestPartResult(const ::testing::TestPartResult& result);

 private:
  const ::testing::TestInfo* pinfo_;
};

// Called after an assertion failure.
void TestResultPrinter::OnTestPartResult(const ::testing::TestPartResult& result) {
  // If the test part succeeded, we don't need to do anything.
  if (result.type() == ::testing::TestPartResult::kSuccess) {
    return;
  }

  // Print failure message from the assertion (e.g. expected this and got that).
  printf("%s:(%d) Failure in test %s.%s\n%s\n", result.file_name(), result.line_number(),
         pinfo_->test_suite_name(), pinfo_->name(), result.message());
  fflush(stdout);
}

// Output xml file when --gtest_output is used, write this function as we can't reuse
// gtest.cc:XmlUnitTestResultPrinter. The reason is XmlUnitTestResultPrinter is totally
// defined in gtest.cc and not expose to outside. What's more, as we don't run gtest in
// the parent process, we don't have gtest classes which are needed by XmlUnitTestResultPrinter.
void Isolate::WriteXmlResults(uint64_t elapsed_time_ns, time_t start_time) {
  FILE* fp = fopen(options_.xml_file().c_str(), "w");
  if (fp == nullptr) {
    printf("Cannot open xml file '%s': %s\n", options_.xml_file().c_str(), strerror(errno));
    exit(1);
  }

  const tm* time_struct = localtime(&start_time);
  if (time_struct == nullptr) {
    PLOG(FATAL) << "Unexpected failure from localtime";
  }
  char timestamp[40];
  snprintf(timestamp, sizeof(timestamp), "%4d-%02d-%02dT%02d:%02d:%02d",
           time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday,
           time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec);

  fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", fp);
  fprintf(fp, "<testsuites tests=\"%zu\" failures=\"%zu\" disabled=\"0\" errors=\"0\"",
          tests_.size(), total_fail_tests_ + total_timeout_tests_ + total_xpass_tests_);
  fprintf(fp, " timestamp=\"%s\" time=\"%.3lf\" name=\"AllTests\">\n", timestamp,
          double(elapsed_time_ns) / kNsPerMs);

  // Construct the suite information.
  struct SuiteInfo {
    std::string suite_name;
    size_t fails = 0;
    double elapsed_ms = 0;
    std::vector<const Test*> tests;
  };
  std::string last_suite_name;
  std::vector<SuiteInfo> suites;
  SuiteInfo* info = nullptr;
  for (const auto& entry : finished_) {
    const Test* test = entry.second.get();
    const std::string& suite_name = test->suite_name();
    if (test->result() == TEST_XFAIL) {
      // Skip XFAIL tests.
      continue;
    }
    if (last_suite_name != suite_name) {
      SuiteInfo suite_info{.suite_name = suite_name.substr(0, suite_name.size() - 1)};
      last_suite_name = suite_name;
      suites.push_back(suite_info);
      info = &suites.back();
    }
    info->tests.push_back(test);
    info->elapsed_ms += double(test->RunTimeNs()) / kNsPerMs;
    if (test->result() != TEST_PASS) {
      info->fails++;
    }
  }

  for (auto& suite_entry : suites) {
    fprintf(fp,
            "  <testsuite name=\"%s\" tests=\"%zu\" failures=\"%zu\" disabled=\"0\" errors=\"0\"",
            suite_entry.suite_name.c_str(), suite_entry.tests.size(), suite_entry.fails);
    fprintf(fp, " time=\"%.3lf\">\n", suite_entry.elapsed_ms);

    for (auto test : suite_entry.tests) {
      fprintf(fp, "    <testcase name=\"%s\" status=\"run\" time=\"%.3lf\" classname=\"%s\"",
              test->test_name().c_str(), double(test->RunTimeNs()) / kNsPerMs,
              suite_entry.suite_name.c_str());
      if (test->result() == TEST_PASS) {
        fputs(" />\n", fp);
      } else {
        fputs(">\n", fp);
        const std::string escaped_output = XmlEscape(test->output());
        fprintf(fp, "      <failure message=\"%s\" type=\"\">\n", escaped_output.c_str());
        fputs("      </failure>\n", fp);
        fputs("    </testcase>\n", fp);
      }
    }
    fputs("  </testsuite>\n", fp);
  }
  fputs("</testsuites>\n", fp);
  fclose(fp);
}

int Isolate::Run() {
  slow_threshold_ns_ = options_.slow_threshold_ms() * kNsPerMs;
  deadline_threshold_ns_ = options_.deadline_threshold_ms() * kNsPerMs;

  bool sharding_enabled = options_.total_shards() > 1;
  if (sharding_enabled &&
      (options_.shard_index() < 0 || options_.shard_index() >= options_.total_shards())) {
    ColoredPrintf(COLOR_RED,
                  "Invalid environment variables: we require 0 <= GTEST_SHARD_INDEX < "
                  "GTEST_TOTAL_SHARDS, but you have GTEST_SHARD_INDEX=%" PRId64
                  ", GTEST_TOTAL_SHARDS=%" PRId64,
                  options_.shard_index(), options_.total_shards());
    printf("\n");
    return 1;
  }

  if (!options_.filter().empty()) {
    ColoredPrintf(COLOR_YELLOW, "Note: Google Test filter = %s", options_.filter().c_str());
    printf("\n");
  }

  if (sharding_enabled) {
    ColoredPrintf(COLOR_YELLOW, "Note: This is test shard %" PRId64 " of %" PRId64,
                  options_.shard_index() + 1, options_.total_shards());
    printf("\n");
  }

  EnumerateTests();

  // Stop default result printer to avoid environment setup/teardown information for each test.
  ::testing::UnitTest::GetInstance()->listeners().Release(
      ::testing::UnitTest::GetInstance()->listeners().default_result_printer());
  ::testing::UnitTest::GetInstance()->listeners().Append(new TestResultPrinter);
  RegisterSignalHandler();

  std::string job_info("Running " + PluralizeString(total_tests_, " test") + " from " +
                       PluralizeString(total_suites_, " test suite") + " (" +
                       PluralizeString(options_.job_count(), " job") + ").");

  int exit_code = 0;
  for (int i = 0; options_.num_iterations() < 0 || i < options_.num_iterations(); i++) {
    if (i > 0) {
      printf("\nRepeating all tests (iteration %d) . . .\n\n", i + 1);
    }
    ColoredPrintf(COLOR_GREEN, "[==========]");
    printf(" %s\n", job_info.c_str());
    fflush(stdout);

    time_t start_time = time(nullptr);
    uint64_t time_ns = NanoTime();
    RunAllTests();
    time_ns = NanoTime() - time_ns;

    PrintFooter(time_ns);

    if (!options_.xml_file().empty()) {
      WriteXmlResults(time_ns, start_time);
    }

    if (total_pass_tests_ + total_skipped_tests_ + total_xfail_tests_ != tests_.size()) {
      exit_code = 1;
    }
  }

  return exit_code;
}

}  // namespace gtest_extras
}  // namespace android
