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

#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <tuple>
#include <vector>

#include <android-base/logging.h>
#include <gtest/gtest.h>

#include "Color.h"
#include "NanoTime.h"
#include "Test.h"

namespace android {
namespace gtest_extras {

Test::Test(std::tuple<std::string, std::string>& test, size_t index, size_t run_index, int fd)
    : suite_name_(std::get<0>(test)),
      test_name_(std::get<1>(test)),
      name_(suite_name_ + test_name_),
      test_index_(index),
      run_index_(run_index),
      fd_(fd),
      start_ns_(NanoTime()) {}

void Test::Stop() {
  end_ns_ = NanoTime();
}

void Test::CloseFd() {
  fd_.reset();
}

void Test::PrintGtestFormat() {
  ColoredPrintf(COLOR_GREEN, "[ RUN      ]");
  printf(" %s\n", name_.c_str());
  printf("%s", output_.c_str());

  switch (result_) {
    case TEST_PASS:
    case TEST_XFAIL:
      ColoredPrintf(COLOR_GREEN, "[       OK ]");
      break;
    case TEST_SKIPPED:
      ColoredPrintf(COLOR_GREEN, "[  SKIPPED ]");
      break;
    default:
      ColoredPrintf(COLOR_RED, "[  FAILED  ]");
      break;
  }
  printf(" %s", name_.c_str());
  if (::testing::GTEST_FLAG(print_time)) {
    printf(" (%" PRId64 " ms)", RunTimeNs() / kNsPerMs);
  }
  printf("\n");
  fflush(stdout);
}

void Test::Print(bool gtest_format) {
  if (gtest_format) {
    PrintGtestFormat();
    return;
  }

  switch (result_) {
    case TEST_XFAIL:
    case TEST_PASS:
      ColoredPrintf(COLOR_GREEN, "[    OK    ]");
      break;
    case TEST_XPASS:
    case TEST_FAIL:
      ColoredPrintf(COLOR_RED, "[  FAILED  ]");
      break;
    case TEST_TIMEOUT:
      ColoredPrintf(COLOR_RED, "[  TIMEOUT ]");
      break;
    case TEST_SKIPPED:
      ColoredPrintf(COLOR_GREEN, "[  SKIPPED ]");
      break;
    case TEST_NONE:
      LOG(FATAL) << "Test result is TEST_NONE, this should not be possible.";
  }

  printf(" %s", name_.c_str());
  if (::testing::GTEST_FLAG(print_time)) {
    printf(" (%" PRId64 " ms)", (end_ns_ - start_ns_) / kNsPerMs);
  }
  printf("\n");

  printf("%s", output_.c_str());
  fflush(stdout);
}

bool Test::Read() {
  char buffer[2048];
  ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer, sizeof(buffer) - 1));
  if (bytes < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // Reading would block. Since this is not an error keep going.
      return true;
    }
    PLOG(FATAL) << "Unexpected failure from read";
    return false;
  }

  if (bytes == 0) {
    return false;
  }
  buffer[bytes] = '\0';
  output_ += buffer;
  return true;
}

void Test::ReadUntilClosed() {
  uint64_t start_ns = NanoTime();
  while (fd_ != -1) {
    if (!Read()) {
      CloseFd();
      break;
    }
    if (NanoTime() - start_ns > 2 * kNsPerS) {
      printf("Reading of done process did not finish after 2 seconds.\n");
      CloseFd();
      break;
    }
  }
}

void Test::SetResultFromOutput() {
  result_ = TEST_PASS;

  // Need to parse the output to determine if this test was skipped.
  // Format of a skipped test:
  //   <filename>:(<line_number>) Failure in test <testname>
  //   Skipped
  //   <Skip Message>
  size_t line_end = output_.find('\n');
  if (line_end == std::string::npos) {
    return;
  }
  std::string second_line(output_.substr(line_end, 9));
  if (output_.substr(line_end, 9) != "\nSkipped\n") {
    return;
  }
  size_t failure_index = output_.find(" Failure in test ");
  if (failure_index == std::string::npos || failure_index >= line_end) {
    return;
  }

  // Only leave the output from the skip message.
  output_ = output_.substr(line_end + 9);

  result_ = TEST_SKIPPED;
}

}  // namespace gtest_extras
}  // namespace android
