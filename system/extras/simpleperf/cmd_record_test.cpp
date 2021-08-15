/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>

#include <map>
#include <memory>
#include <regex>
#include <thread>

#include "command.h"
#include "environment.h"
#include "event_selection_set.h"
#include "get_test_data.h"
#include "record.h"
#include "record_file.h"
#include "test_util.h"
#include "thread_tree.h"

using namespace PerfFileFormat;

static std::unique_ptr<Command> RecordCmd() {
  return CreateCommandInstance("record");
}

static bool RunRecordCmd(std::vector<std::string> v,
                         const char* output_file = nullptr) {
  std::unique_ptr<TemporaryFile> tmpfile;
  std::string out_file;
  if (output_file != nullptr) {
    out_file = output_file;
  } else {
    tmpfile.reset(new TemporaryFile);
    out_file = tmpfile->path;
  }
  v.insert(v.end(), {"-o", out_file, "sleep", SLEEP_SEC});
  return RecordCmd()->Run(v);
}

TEST(record_cmd, no_options) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({}));
}

TEST(record_cmd, system_wide_option) {
  TEST_REQUIRE_HW_COUNTER();
  TEST_IN_ROOT(ASSERT_TRUE(RunRecordCmd({"-a"})));
}

void CheckEventType(const std::string& record_file, const std::string event_type,
                    uint64_t sample_period, uint64_t sample_freq) {
  const EventType* type = FindEventTypeByName(event_type);
  ASSERT_TRUE(type != nullptr);
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(record_file);
  ASSERT_TRUE(reader);
  std::vector<EventAttrWithId> attrs = reader->AttrSection();
  for (auto& attr : attrs) {
    if (attr.attr->type == type->type && attr.attr->config == type->config) {
      if (attr.attr->freq == 0) {
        ASSERT_EQ(sample_period, attr.attr->sample_period);
        ASSERT_EQ(sample_freq, 0u);
      } else {
        ASSERT_EQ(sample_period, 0u);
        ASSERT_EQ(sample_freq, attr.attr->sample_freq);
      }
      return;
    }
  }
  FAIL();
}

TEST(record_cmd, sample_period_option) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"-c", "100000"}, tmpfile.path));
  CheckEventType(tmpfile.path, "cpu-cycles", 100000u, 0);
}

TEST(record_cmd, event_option) {
  ASSERT_TRUE(RunRecordCmd({"-e", "cpu-clock"}));
}

TEST(record_cmd, freq_option) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"-f", "99"}, tmpfile.path));
  CheckEventType(tmpfile.path, "cpu-cycles", 0, 99u);
  ASSERT_TRUE(RunRecordCmd({"-e", "cpu-clock", "-f", "99"}, tmpfile.path));
  CheckEventType(tmpfile.path, "cpu-clock", 0, 99u);
  ASSERT_FALSE(RunRecordCmd({"-f", std::to_string(UINT_MAX)}));
}

TEST(record_cmd, multiple_freq_or_sample_period_option) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"-f", "99", "-e", "cpu-cycles", "-c", "1000000", "-e",
                            "cpu-clock"}, tmpfile.path));
  CheckEventType(tmpfile.path, "cpu-cycles", 0, 99u);
  CheckEventType(tmpfile.path, "cpu-clock", 1000000u, 0u);
}

TEST(record_cmd, output_file_option) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "sleep", SLEEP_SEC}));
}

TEST(record_cmd, dump_kernel_mmap) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({}, tmpfile.path));
  std::unique_ptr<RecordFileReader> reader =
      RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader != nullptr);
  std::vector<std::unique_ptr<Record>> records = reader->DataSection();
  ASSERT_GT(records.size(), 0U);
  bool have_kernel_mmap = false;
  for (auto& record : records) {
    if (record->type() == PERF_RECORD_MMAP) {
      const MmapRecord* mmap_record =
          static_cast<const MmapRecord*>(record.get());
      if (strcmp(mmap_record->filename, DEFAULT_KERNEL_MMAP_NAME) == 0 ||
          strcmp(mmap_record->filename, DEFAULT_KERNEL_MMAP_NAME_PERF) == 0) {
        have_kernel_mmap = true;
        break;
      }
    }
  }
  ASSERT_TRUE(have_kernel_mmap);
}

TEST(record_cmd, dump_build_id_feature) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({}, tmpfile.path));
  std::unique_ptr<RecordFileReader> reader =
      RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader != nullptr);
  const FileHeader& file_header = reader->FileHeader();
  ASSERT_TRUE(file_header.features[FEAT_BUILD_ID / 8] &
              (1 << (FEAT_BUILD_ID % 8)));
  ASSERT_GT(reader->FeatureSectionDescriptors().size(), 0u);
}

TEST(record_cmd, tracepoint_event) {
  TEST_IN_ROOT(ASSERT_TRUE(RunRecordCmd({"-a", "-e", "sched:sched_switch"})));
}

TEST(record_cmd, rN_event) {
  TEST_REQUIRE_HW_COUNTER();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  size_t event_number;
  if (GetBuildArch() == ARCH_ARM64 || GetBuildArch() == ARCH_ARM) {
    // As in D5.10.2 of the ARMv8 manual, ARM defines the event number space for PMU. part of the
    // space is for common event numbers (which will stay the same for all ARM chips), part of the
    // space is for implementation defined events. Here 0x08 is a common event for instructions.
    event_number = 0x08;
  } else if (GetBuildArch() == ARCH_X86_32 || GetBuildArch() == ARCH_X86_64) {
    // As in volume 3 chapter 19 of the Intel manual, 0x00c0 is the event number for instruction.
    event_number = 0x00c0;
  } else {
    GTEST_LOG_(INFO) << "Omit arch " << GetBuildArch();
    return;
  }
  std::string event_name = android::base::StringPrintf("r%zx", event_number);
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"-e", event_name}, tmpfile.path));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  std::vector<EventAttrWithId> attrs = reader->AttrSection();
  ASSERT_EQ(1u, attrs.size());
  ASSERT_EQ(PERF_TYPE_RAW, attrs[0].attr->type);
  ASSERT_EQ(event_number, attrs[0].attr->config);
}

TEST(record_cmd, branch_sampling) {
  TEST_REQUIRE_HW_COUNTER();
  if (IsBranchSamplingSupported()) {
    ASSERT_TRUE(RunRecordCmd({"-b"}));
    ASSERT_TRUE(RunRecordCmd({"-j", "any,any_call,any_ret,ind_call"}));
    ASSERT_TRUE(RunRecordCmd({"-j", "any,k"}));
    ASSERT_TRUE(RunRecordCmd({"-j", "any,u"}));
    ASSERT_FALSE(RunRecordCmd({"-j", "u"}));
  } else {
    GTEST_LOG_(INFO) << "This test does nothing as branch stack sampling is "
                        "not supported on this device.";
  }
}

TEST(record_cmd, event_modifier) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"-e", "cpu-cycles:u"}));
}

TEST(record_cmd, fp_callchain_sampling) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"--call-graph", "fp"}));
}

TEST(record_cmd, fp_callchain_sampling_warning_on_arm) {
  TEST_REQUIRE_HW_COUNTER();
  if (GetBuildArch() != ARCH_ARM) {
    GTEST_LOG_(INFO) << "This test does nothing as it only tests on arm arch.";
    return;
  }
  ASSERT_EXIT(
      {
        exit(RunRecordCmd({"--call-graph", "fp"}) ? 0 : 1);
      },
      testing::ExitedWithCode(0), "doesn't work well on arm");
}

TEST(record_cmd, system_wide_fp_callchain_sampling) {
  TEST_REQUIRE_HW_COUNTER();
  TEST_IN_ROOT(ASSERT_TRUE(RunRecordCmd({"-a", "--call-graph", "fp"})));
}

bool IsInNativeAbi() {
  static int in_native_abi = -1;
  if (in_native_abi == -1) {
    FILE* fp = popen("uname -m", "re");
    char buf[40];
    memset(buf, '\0', sizeof(buf));
    CHECK_EQ(fgets(buf, sizeof(buf), fp), buf);
    pclose(fp);
    std::string s = buf;
    in_native_abi = 1;
    if (GetBuildArch() == ARCH_X86_32 || GetBuildArch() == ARCH_X86_64) {
      if (s.find("86") == std::string::npos) {
        in_native_abi = 0;
      }
    } else if (GetBuildArch() == ARCH_ARM || GetBuildArch() == ARCH_ARM64) {
      if (s.find("arm") == std::string::npos && s.find("aarch64") == std::string::npos) {
        in_native_abi = 0;
      }
    }
  }
  return in_native_abi == 1;
}

bool HasHardwareCounter() {
  static int has_hw_counter = -1;
  if (has_hw_counter == -1) {
    has_hw_counter = 1;
#if defined(__arm__)
    std::string cpu_info;
    if (android::base::ReadFileToString("/proc/cpuinfo", &cpu_info)) {
      std::string hardware = GetHardwareFromCpuInfo(cpu_info);
      if (std::regex_search(hardware, std::regex(R"(i\.MX6.*Quad)")) ||
          std::regex_search(hardware, std::regex(R"(SC7731e)")) ) {
        has_hw_counter = 0;
      }
    }
#endif
  }
  return has_hw_counter == 1;
}

TEST(record_cmd, dwarf_callchain_sampling) {
  TEST_REQUIRE_HW_COUNTER();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  ASSERT_TRUE(IsDwarfCallChainSamplingSupported());
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "--call-graph", "dwarf"}));
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "--call-graph", "dwarf,16384"}));
  ASSERT_FALSE(RunRecordCmd({"-p", pid, "--call-graph", "dwarf,65536"}));
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "-g"}));
}

TEST(record_cmd, system_wide_dwarf_callchain_sampling) {
  TEST_REQUIRE_HW_COUNTER();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  ASSERT_TRUE(IsDwarfCallChainSamplingSupported());
  TEST_IN_ROOT(RunRecordCmd({"-a", "--call-graph", "dwarf"}));
}

TEST(record_cmd, no_unwind_option) {
  TEST_REQUIRE_HW_COUNTER();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  ASSERT_TRUE(IsDwarfCallChainSamplingSupported());
  ASSERT_TRUE(RunRecordCmd({"--call-graph", "dwarf", "--no-unwind"}));
  ASSERT_FALSE(RunRecordCmd({"--no-unwind"}));
}

TEST(record_cmd, post_unwind_option) {
  TEST_REQUIRE_HW_COUNTER();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  ASSERT_TRUE(IsDwarfCallChainSamplingSupported());
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "--call-graph", "dwarf", "--post-unwind"}));
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "--call-graph", "dwarf", "--post-unwind=yes"}));
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "--call-graph", "dwarf", "--post-unwind=no"}));
}

TEST(record_cmd, existing_processes) {
  TEST_REQUIRE_HW_COUNTER();
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(2, &workloads);
  std::string pid_list = android::base::StringPrintf(
      "%d,%d", workloads[0]->GetPid(), workloads[1]->GetPid());
  ASSERT_TRUE(RunRecordCmd({"-p", pid_list}));
}

TEST(record_cmd, existing_threads) {
  TEST_REQUIRE_HW_COUNTER();
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(2, &workloads);
  // Process id can also be used as thread id in linux.
  std::string tid_list = android::base::StringPrintf(
      "%d,%d", workloads[0]->GetPid(), workloads[1]->GetPid());
  ASSERT_TRUE(RunRecordCmd({"-t", tid_list}));
}

TEST(record_cmd, no_monitored_threads) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_FALSE(RecordCmd()->Run({"-o", tmpfile.path}));
  ASSERT_FALSE(RecordCmd()->Run({"-o", tmpfile.path, ""}));
}

TEST(record_cmd, more_than_one_event_types) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"-e", "cpu-cycles,cpu-clock"}));
  ASSERT_TRUE(RunRecordCmd({"-e", "cpu-cycles", "-e", "cpu-clock"}));
}

TEST(record_cmd, mmap_page_option) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"-m", "1"}));
  ASSERT_FALSE(RunRecordCmd({"-m", "0"}));
  ASSERT_FALSE(RunRecordCmd({"-m", "7"}));
}

static void CheckKernelSymbol(const std::string& path, bool need_kallsyms,
                              bool* success) {
  *success = false;
  std::unique_ptr<RecordFileReader> reader =
      RecordFileReader::CreateInstance(path);
  ASSERT_TRUE(reader != nullptr);
  std::vector<std::unique_ptr<Record>> records = reader->DataSection();
  bool has_kernel_symbol_records = false;
  for (const auto& record : records) {
    if (record->type() == SIMPLE_PERF_RECORD_KERNEL_SYMBOL) {
      has_kernel_symbol_records = true;
    }
  }
  bool require_kallsyms = need_kallsyms && CheckKernelSymbolAddresses();
  ASSERT_EQ(require_kallsyms, has_kernel_symbol_records);
  *success = true;
}

TEST(record_cmd, kernel_symbol) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"--no-dump-symbols"}, tmpfile.path));
  bool success;
  CheckKernelSymbol(tmpfile.path, true, &success);
  ASSERT_TRUE(success);
  ASSERT_TRUE(RunRecordCmd({"--no-dump-symbols", "--no-dump-kernel-symbols"}, tmpfile.path));
  CheckKernelSymbol(tmpfile.path, false, &success);
  ASSERT_TRUE(success);
}

// Check if dumped symbols in perf.data matches our expectation.
static bool CheckDumpedSymbols(const std::string& path, bool allow_dumped_symbols) {
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(path);
  if (!reader) {
    return false;
  }
  std::string file_path;
  uint32_t file_type;
  uint64_t min_vaddr;
  uint64_t file_offset_of_min_vaddr;
  std::vector<Symbol> symbols;
  std::vector<uint64_t> dex_file_offsets;
  size_t read_pos = 0;
  bool has_dumped_symbols = false;
  while (reader->ReadFileFeature(read_pos, &file_path, &file_type, &min_vaddr,
                                 &file_offset_of_min_vaddr, &symbols, &dex_file_offsets)) {
    if (!symbols.empty()) {
      has_dumped_symbols = true;
    }
  }
  // It is possible that there are no samples hitting functions having symbols.
  // So "allow_dumped_symbols = true" doesn't guarantee "has_dumped_symbols = true".
  if (!allow_dumped_symbols && has_dumped_symbols) {
    return false;
  }
  return true;
}

TEST(record_cmd, no_dump_symbols) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({}, tmpfile.path));
  ASSERT_TRUE(CheckDumpedSymbols(tmpfile.path, true));
  ASSERT_TRUE(RunRecordCmd({"--no-dump-symbols", "--no-dump-kernel-symbols"}, tmpfile.path));
  ASSERT_TRUE(CheckDumpedSymbols(tmpfile.path, false));
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  ASSERT_TRUE(IsDwarfCallChainSamplingSupported());
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "-g"}, tmpfile.path));
  ASSERT_TRUE(CheckDumpedSymbols(tmpfile.path, true));
  ASSERT_TRUE(RunRecordCmd({"-p", pid, "-g", "--no-dump-symbols", "--no-dump-kernel-symbols"},
                           tmpfile.path));
  ASSERT_TRUE(CheckDumpedSymbols(tmpfile.path, false));
}

TEST(record_cmd, dump_kernel_symbols) {
  TEST_REQUIRE_HW_COUNTER();
  if (!IsRoot()) {
    GTEST_LOG_(INFO) << "Test requires root privilege";
    return;
  }
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"-a", "-o", tmpfile.path, "sleep", "1"}));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader != nullptr);
  std::map<int, SectionDesc> section_map = reader->FeatureSectionDescriptors();
  ASSERT_NE(section_map.find(FEAT_FILE), section_map.end());
  std::string file_path;
  uint32_t file_type;
  uint64_t min_vaddr;
  uint64_t file_offset_of_min_vaddr;
  std::vector<Symbol> symbols;
  std::vector<uint64_t> dex_file_offsets;
  size_t read_pos = 0;
  bool has_kernel_symbols = false;
  while (reader->ReadFileFeature(read_pos, &file_path, &file_type, &min_vaddr,
                                 &file_offset_of_min_vaddr, &symbols, &dex_file_offsets)) {
    if (file_type == DSO_KERNEL && !symbols.empty()) {
      has_kernel_symbols = true;
    }
  }
  ASSERT_TRUE(has_kernel_symbols);
}

TEST(record_cmd, group_option) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"--group", "cpu-cycles,cpu-clock", "-m", "16"}));
  ASSERT_TRUE(RunRecordCmd({"--group", "cpu-cycles,cpu-clock", "--group",
                            "cpu-cycles:u,cpu-clock:u", "--group",
                            "cpu-cycles:k,cpu-clock:k", "-m", "16"}));
}

TEST(record_cmd, symfs_option) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"--symfs", "/"}));
}

TEST(record_cmd, duration_option) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"--duration", "1.2", "-p",
                                std::to_string(getpid()), "-o", tmpfile.path, "--in-app"}));
  ASSERT_TRUE(
      RecordCmd()->Run({"--duration", "1", "-o", tmpfile.path, "sleep", "2"}));
}

TEST(record_cmd, support_modifier_for_clock_events) {
  for (const std::string& e : {"cpu-clock", "task-clock"}) {
    for (const std::string& m : {"u", "k"}) {
      ASSERT_TRUE(RunRecordCmd({"-e", e + ":" + m})) << "event " << e << ":"
                                                     << m;
    }
  }
}

TEST(record_cmd, handle_SIGHUP) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  int pipefd[2];
  ASSERT_EQ(0, pipe(pipefd));
  int read_fd = pipefd[0];
  int write_fd = pipefd[1];
  char data[8] = {};
  std::thread thread([&]() {
    android::base::ReadFully(read_fd, data, 7);
    kill(getpid(), SIGHUP);
  });
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "--start_profiling_fd",
                                std::to_string(write_fd), "sleep", "1000000"}));
  thread.join();
  close(write_fd);
  close(read_fd);
  ASSERT_STREQ(data, "STARTED");
}

TEST(record_cmd, stop_when_no_more_targets) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  std::atomic<int> tid(0);
  std::thread thread([&]() {
    tid = gettid();
    sleep(1);
  });
  thread.detach();
  while (tid == 0);
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "-t", std::to_string(tid), "--in-app"}));
}

TEST(record_cmd, donot_stop_when_having_targets) {
  TEST_REQUIRE_HW_COUNTER();
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  uint64_t start_time_in_ns = GetSystemClock();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "-p", pid, "--duration", "3"}));
  uint64_t end_time_in_ns = GetSystemClock();
  ASSERT_GT(end_time_in_ns - start_time_in_ns, static_cast<uint64_t>(2e9));
}

TEST(record_cmd, start_profiling_fd_option) {
  TEST_REQUIRE_HW_COUNTER();
  int pipefd[2];
  ASSERT_EQ(0, pipe(pipefd));
  int read_fd = pipefd[0];
  int write_fd = pipefd[1];
  ASSERT_EXIT(
      {
        close(read_fd);
        exit(RunRecordCmd({"--start_profiling_fd", std::to_string(write_fd)}) ? 0 : 1);
      },
      testing::ExitedWithCode(0), "");
  close(write_fd);
  std::string s;
  ASSERT_TRUE(android::base::ReadFdToString(read_fd, &s));
  close(read_fd);
  ASSERT_EQ("STARTED", s);
}

TEST(record_cmd, record_meta_info_feature) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({}, tmpfile.path));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  std::unordered_map<std::string, std::string> info_map;
  ASSERT_TRUE(reader->ReadMetaInfoFeature(&info_map));
  ASSERT_NE(info_map.find("simpleperf_version"), info_map.end());
  ASSERT_NE(info_map.find("timestamp"), info_map.end());
#if defined(__ANDROID__)
  ASSERT_NE(info_map.find("product_props"), info_map.end());
  ASSERT_NE(info_map.find("android_version"), info_map.end());
#endif
}

// See http://b/63135835.
TEST(record_cmd, cpu_clock_for_a_long_time) {
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run(
      {"-e", "cpu-clock", "-o", tmpfile.path, "-p", pid, "--duration", "3"}));
}

TEST(record_cmd, dump_regs_for_tracepoint_events) {
  TEST_REQUIRE_HW_COUNTER();
  TEST_REQUIRE_HOST_ROOT();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  // Check if the kernel can dump registers for tracepoint events.
  // If not, probably a kernel patch below is missing:
  // "5b09a094f2 arm64: perf: Fix callchain parse error with kernel tracepoint events"
  ASSERT_TRUE(IsDumpingRegsForTracepointEventsSupported());
}

TEST(record_cmd, trace_offcpu_option) {
  TEST_REQUIRE_HW_COUNTER();
  // On linux host, we need root privilege to read tracepoint events.
  TEST_REQUIRE_HOST_ROOT();
  OMIT_TEST_ON_NON_NATIVE_ABIS();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RunRecordCmd({"--trace-offcpu", "-f", "1000"}, tmpfile.path));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  std::unordered_map<std::string, std::string> info_map;
  ASSERT_TRUE(reader->ReadMetaInfoFeature(&info_map));
  ASSERT_EQ(info_map["trace_offcpu"], "true");
  CheckEventType(tmpfile.path, "sched:sched_switch", 1u, 0u);
}

TEST(record_cmd, exit_with_parent_option) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"--exit-with-parent"}));
}

TEST(record_cmd, clockid_option) {
  TEST_REQUIRE_HW_COUNTER();
  if (!IsSettingClockIdSupported()) {
    ASSERT_FALSE(RunRecordCmd({"--clockid", "monotonic"}));
  } else {
    TemporaryFile tmpfile;
    ASSERT_TRUE(RunRecordCmd({"--clockid", "monotonic"}, tmpfile.path));
    std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
    ASSERT_TRUE(reader);
    std::unordered_map<std::string, std::string> info_map;
    ASSERT_TRUE(reader->ReadMetaInfoFeature(&info_map));
    ASSERT_EQ(info_map["clockid"], "monotonic");
  }
}

TEST(record_cmd, generate_samples_by_hw_counters) {
  TEST_REQUIRE_HW_COUNTER();
  std::vector<std::string> events = {"cpu-cycles", "instructions"};
  for (auto& event : events) {
    TemporaryFile tmpfile;
    ASSERT_TRUE(RecordCmd()->Run({"-e", event, "-o", tmpfile.path, "sleep", "1"}));
    std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
    ASSERT_TRUE(reader);
    bool has_sample = false;
    ASSERT_TRUE(reader->ReadDataSection([&](std::unique_ptr<Record> r) {
      if (r->type() == PERF_RECORD_SAMPLE) {
        has_sample = true;
      }
      return true;
    }));
    ASSERT_TRUE(has_sample);
  }
}

TEST(record_cmd, callchain_joiner_options) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"--no-callchain-joiner"}));
  ASSERT_TRUE(RunRecordCmd({"--callchain-joiner-min-matching-nodes", "2"}));
}

TEST(record_cmd, dashdash) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "--", "sleep", "1"}));
}

TEST(record_cmd, size_limit_option) {
  TEST_REQUIRE_HW_COUNTER();
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "-p", pid, "--size-limit", "1k", "--duration",
                                "1"}));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  ASSERT_GT(reader->FileHeader().data.size, 1000u);
  ASSERT_LT(reader->FileHeader().data.size, 2000u);
  ASSERT_FALSE(RunRecordCmd({"--size-limit", "0"}));
}

TEST(record_cmd, support_mmap2) {
  // mmap2 is supported in kernel >= 3.16. If not supported, please cherry pick below kernel
  // patches:
  //   13d7a2410fa637 perf: Add attr->mmap2 attribute to an event
  //   f972eb63b1003f perf: Pass protection and flags bits through mmap2 interface.
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(IsMmap2Supported());
}

TEST(record_cmd, kernel_bug_making_zero_dyn_size) {
  // Test a kernel bug that makes zero dyn_size in kernel < 3.13. If it fails, please cherry pick
  // below kernel patch: 0a196848ca365e perf: Fix arch_perf_out_copy_user default
  TEST_REQUIRE_HW_COUNTER();
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "-p", pid, "--call-graph", "dwarf,8",
                                "--no-unwind", "--duration", "1"}));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  bool has_sample = false;
  ASSERT_TRUE(reader->ReadDataSection([&](std::unique_ptr<Record> r) {
    if (r->type() == PERF_RECORD_SAMPLE && !r->InKernel()) {
      SampleRecord* sr = static_cast<SampleRecord*>(r.get());
      if (sr->stack_user_data.dyn_size == 0) {
        return false;
      }
      has_sample = true;
    }
    return true;
  }));
  ASSERT_TRUE(has_sample);
}

TEST(record_cmd, kernel_bug_making_zero_dyn_size_for_kernel_samples) {
  // Test a kernel bug that makes zero dyn_size for syscalls of 32-bit applications in 64-bit
  // kernels. If it fails, please cherry pick below kernel patch:
  // 02e184476eff8 perf/core: Force USER_DS when recording user stack data
  TEST_REQUIRE_HW_COUNTER();
  TEST_REQUIRE_HOST_ROOT();
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(1, &workloads);
  std::string pid = std::to_string(workloads[0]->GetPid());
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-e", "sched:sched_switch", "-o", tmpfile.path, "-p", pid,
                                "--call-graph", "dwarf,8", "--no-unwind", "--duration", "1"}));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  bool has_sample = false;
  ASSERT_TRUE(reader->ReadDataSection([&](std::unique_ptr<Record> r) {
    if (r->type() == PERF_RECORD_SAMPLE && r->InKernel()) {
      SampleRecord* sr = static_cast<SampleRecord*>(r.get());
      if (sr->stack_user_data.dyn_size == 0) {
        return false;
      }
      has_sample = true;
    }
    return true;
  }));
  ASSERT_TRUE(has_sample);
}

TEST(record_cmd, cpu_percent_option) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(RunRecordCmd({"--cpu-percent", "50"}));
  ASSERT_FALSE(RunRecordCmd({"--cpu-percent", "0"}));
  ASSERT_FALSE(RunRecordCmd({"--cpu-percent", "101"}));
}

static void TestRecordingApps(const std::string& app_name) {
  // Bring the app to foreground to avoid no samples.
  ASSERT_TRUE(Workload::RunCmd({"am", "start", app_name + "/.MainActivity"}));
  TemporaryFile tmpfile;
  ASSERT_TRUE(RecordCmd()->Run({"-o", tmpfile.path, "--app", app_name, "-g", "--duration", "3"}));
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile.path);
  ASSERT_TRUE(reader);
  // Check if having samples.
  bool has_sample = false;
  ASSERT_TRUE(reader->ReadDataSection([&](std::unique_ptr<Record> r) {
    if (r->type() == PERF_RECORD_SAMPLE) {
      has_sample = true;
    }
    return true;
  }));
  ASSERT_TRUE(has_sample);

  // Check if we can profile Java code by looking for a Java method name in dumped symbols, which
  // is app_name + ".MainActivity$1.run".
  const std::string expected_class_name = app_name + ".MainActivity";
  const std::string expected_method_name = "run";
  std::string file_path;
  uint32_t file_type;
  uint64_t min_vaddr;
  uint64_t file_offset_of_min_vaddr;
  std::vector<Symbol> symbols;
  std::vector<uint64_t> dex_file_offsets;
  size_t read_pos = 0;
  bool has_java_symbol = false;
  ASSERT_TRUE(reader->HasFeature(FEAT_FILE));
  while (reader->ReadFileFeature(read_pos, &file_path, &file_type, &min_vaddr,
                                 &file_offset_of_min_vaddr, &symbols, &dex_file_offsets)) {
    for (const auto& symbol : symbols) {
      const char* name = symbol.DemangledName();
      if (strstr(name, expected_class_name.c_str()) != nullptr &&
          strstr(name, expected_method_name.c_str()) != nullptr) {
        has_java_symbol = true;
      }
    }
  }
  ASSERT_TRUE(has_java_symbol);
}

TEST(record_cmd, app_option_for_debuggable_app) {
  TEST_REQUIRE_HW_COUNTER();
  TEST_REQUIRE_APPS();
  TestRecordingApps("com.android.simpleperf.debuggable");
}

TEST(record_cmd, app_option_for_profileable_app) {
  TEST_REQUIRE_HW_COUNTER();
  TEST_REQUIRE_APPS();
  TestRecordingApps("com.android.simpleperf.profileable");
}
