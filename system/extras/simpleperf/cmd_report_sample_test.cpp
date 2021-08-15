/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <android-base/file.h>

#include "command.h"
#include "get_test_data.h"

static std::unique_ptr<Command> ReportSampleCmd() {
  return CreateCommandInstance("report-sample");
}

TEST(cmd_report_sample, text) {
  ASSERT_TRUE(
      ReportSampleCmd()->Run({"-i", GetTestData(PERF_DATA_WITH_SYMBOLS)}));
}

TEST(cmd_report_sample, output_option) {
  TemporaryFile tmpfile;
  ASSERT_TRUE(ReportSampleCmd()->Run(
      {"-i", GetTestData(PERF_DATA_WITH_SYMBOLS), "-o", tmpfile.path}));
}

TEST(cmd_report_sample, show_callchain_option) {
  TemporaryFile tmpfile;
  ASSERT_TRUE(ReportSampleCmd()->Run({"-i", GetTestData(CALLGRAPH_FP_PERF_DATA),
                                      "-o", tmpfile.path, "--show-callchain"}));
}

static void GetProtobufReport(const std::string& test_data_file, std::string* protobuf_report,
                              const std::vector<std::string>& extra_args = {}) {
  TemporaryFile tmpfile;
  TemporaryFile tmpfile2;
  std::vector<std::string> args = {"-i", GetTestData(test_data_file), "-o", tmpfile.path,
                                   "--protobuf"};
  args.insert(args.end(), extra_args.begin(), extra_args.end());
  ASSERT_TRUE(ReportSampleCmd()->Run(args));
  ASSERT_TRUE(ReportSampleCmd()->Run({"--dump-protobuf-report", tmpfile.path,
                                      "-o", tmpfile2.path}));
  ASSERT_TRUE(android::base::ReadFileToString(tmpfile2.path, protobuf_report));
}

TEST(cmd_report_sample, protobuf_option) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_SYMBOLS, &data);
  ASSERT_NE(data.find("magic: SIMPLEPERF"), std::string::npos);
  ASSERT_NE(data.find("version: 1"), std::string::npos);
  ASSERT_NE(data.find("file:"), std::string::npos);
}

TEST(cmd_report_sample, no_skipped_file_id) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_WRONG_IP_IN_CALLCHAIN, &data);
  // If wrong ips in callchain are omitted, "unknown" file path will not be generated.
  ASSERT_EQ(data.find("unknown"), std::string::npos);
}

TEST(cmd_report_sample, sample_has_event_count) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_SYMBOLS, &data);
  ASSERT_NE(data.find("event_count:"), std::string::npos);
}

TEST(cmd_report_sample, has_thread_record) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_SYMBOLS, &data);
  ASSERT_NE(data.find("thread:"), std::string::npos);
}

TEST(cmd_report_sample, trace_offcpu) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_TRACE_OFFCPU, &data);
  ASSERT_NE(data.find("event_type: sched:sched_switch"), std::string::npos);
}

TEST(cmd_report_sample, have_clear_callchain_end_in_protobuf_output) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_TRACE_OFFCPU, &data, {"--show-callchain"});
  ASSERT_NE(data.find("__libc_init"), std::string::npos);
  ASSERT_EQ(data.find("_start_main"), std::string::npos);
}

TEST(cmd_report_sample, app_package_name_in_meta_info) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_APP_PACKAGE_NAME, &data);
  ASSERT_NE(data.find("app_package_name: com.google.sample.tunnel"), std::string::npos);
}

TEST(cmd_report_sample, remove_unknown_kernel_symbols) {
  std::string data;
  // Test --remove-unknown-kernel-symbols on perf.data with kernel_symbols_available=false.
  GetProtobufReport(PERF_DATA_WITH_KERNEL_SYMBOLS_AVAILABLE_FALSE, &data,
                    {"--show-callchain"});
  ASSERT_NE(data.find("time: 1368182962424044"), std::string::npos);
  ASSERT_NE(data.find("path: [kernel.kallsyms]"), std::string::npos);
  ASSERT_NE(data.find("path: /system/lib64/libc.so"), std::string::npos);
  GetProtobufReport(PERF_DATA_WITH_KERNEL_SYMBOLS_AVAILABLE_FALSE, &data,
                    {"--show-callchain", "--remove-unknown-kernel-symbols"});
  // The sample dumped at time 1368182962424044 shouldn't be removed. Because it has user space
  // callchains.
  ASSERT_NE(data.find("time: 1368182962424044"), std::string::npos);
  // Kernel callchains shouldn't be removed.
  ASSERT_EQ(data.find("path: [kernel.kallsyms]"), std::string::npos);
  // User space callchains still exist.
  ASSERT_NE(data.find("path: /system/lib64/libc.so"), std::string::npos);

  // Test --remove-unknown-kernel-symbols on perf.data with kernel_symbols_available=true.
  GetProtobufReport(PERF_DATA_WITH_KERNEL_SYMBOLS_AVAILABLE_TRUE, &data,
                    {"--show-callchain"});
  ASSERT_NE(data.find("time: 1368297633794862"), std::string::npos);
  ASSERT_NE(data.find("path: [kernel.kallsyms]"), std::string::npos);
  ASSERT_NE(data.find("symbol: binder_ioctl_write_read"), std::string::npos);
  ASSERT_NE(data.find("path: /system/lib64/libc.so"), std::string::npos);
  GetProtobufReport(PERF_DATA_WITH_KERNEL_SYMBOLS_AVAILABLE_TRUE, &data,
                    {"--show-callchain", "--remove-unknown-kernel-symbols"});
  ASSERT_NE(data.find("time: 1368297633794862"), std::string::npos);
  ASSERT_NE(data.find("path: [kernel.kallsyms]"), std::string::npos);
  ASSERT_NE(data.find("symbol: binder_ioctl_write_read"), std::string::npos);
  ASSERT_NE(data.find("path: /system/lib64/libc.so"), std::string::npos);
}

TEST(cmd_report_sample, show_art_frames_option) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_INTERPRETER_FRAMES, &data, {"--show-callchain"});
  ASSERT_EQ(data.find("artMterpAsmInstructionStart"), std::string::npos);
  GetProtobufReport(PERF_DATA_WITH_INTERPRETER_FRAMES, &data,
                    {"--show-callchain", "--show-art-frames"});
  ASSERT_NE(data.find("artMterpAsmInstructionStart"), std::string::npos);
}

TEST(cmd_report_sample, show_symbols_before_and_after_demangle) {
  std::string data;
  GetProtobufReport(PERF_DATA_WITH_INTERPRETER_FRAMES, &data, {"--show-callchain"});
  ASSERT_NE(data.find("symbol: android::hardware::IPCThreadState::talkWithDriver(bool)"),
            std::string::npos);
  ASSERT_NE(data.find("mangled_symbol: _ZN7android8hardware14IPCThreadState14talkWithDriverEb"),
            std::string::npos);
}

TEST(cmd_report_sample, symdir_option) {
  std::string data;
  GetProtobufReport(PERF_DATA_FOR_BUILD_ID_CHECK, &data);
  ASSERT_EQ(data.find("symbol: main"), std::string::npos);
  GetProtobufReport(PERF_DATA_FOR_BUILD_ID_CHECK, &data,
                    {"--symdir", GetTestDataDir() + CORRECT_SYMFS_FOR_BUILD_ID_CHECK});
  ASSERT_NE(data.find("symbol: main"), std::string::npos);
}
