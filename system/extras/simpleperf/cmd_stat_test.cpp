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

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include <thread>

#include "command.h"
#include "environment.h"
#include "event_selection_set.h"
#include "get_test_data.h"
#include "test_util.h"

static std::unique_ptr<Command> StatCmd() {
  return CreateCommandInstance("stat");
}

TEST(stat_cmd, no_options) { ASSERT_TRUE(StatCmd()->Run({"sleep", "1"})); }

TEST(stat_cmd, event_option) {
  ASSERT_TRUE(StatCmd()->Run({"-e", "cpu-clock,task-clock", "sleep", "1"}));
}

TEST(stat_cmd, system_wide_option) {
  TEST_IN_ROOT(ASSERT_TRUE(StatCmd()->Run({"-a", "sleep", "1"})));
}

TEST(stat_cmd, verbose_option) {
  ASSERT_TRUE(StatCmd()->Run({"--verbose", "sleep", "1"}));
}

TEST(stat_cmd, tracepoint_event) {
  TEST_IN_ROOT(ASSERT_TRUE(
      StatCmd()->Run({"-a", "-e", "sched:sched_switch", "sleep", "1"})));
}

TEST(stat_cmd, rN_event) {
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
  ASSERT_TRUE(StatCmd()->Run({"-e", event_name, "sleep", "1"}));
}

TEST(stat_cmd, event_modifier) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(
      StatCmd()->Run({"-e", "cpu-cycles:u,cpu-cycles:k", "sleep", "1"}));
}

void RunWorkloadFunction() {
  while (true) {
    for (volatile int i = 0; i < 10000; ++i);
    usleep(1);
  }
}

void CreateProcesses(size_t count,
                     std::vector<std::unique_ptr<Workload>>* workloads) {
  workloads->clear();
  // Create workloads run longer than profiling time.
  for (size_t i = 0; i < count; ++i) {
    std::unique_ptr<Workload> workload;
    workload = Workload::CreateWorkload(RunWorkloadFunction);
    ASSERT_TRUE(workload != nullptr);
    ASSERT_TRUE(workload->Start());
    workloads->push_back(std::move(workload));
  }
}

TEST(stat_cmd, existing_processes) {
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(2, &workloads);
  std::string pid_list = android::base::StringPrintf(
      "%d,%d", workloads[0]->GetPid(), workloads[1]->GetPid());
  ASSERT_TRUE(StatCmd()->Run({"-p", pid_list, "sleep", "1"}));
}

TEST(stat_cmd, existing_threads) {
  std::vector<std::unique_ptr<Workload>> workloads;
  CreateProcesses(2, &workloads);
  // Process id can be used as thread id in linux.
  std::string tid_list = android::base::StringPrintf(
      "%d,%d", workloads[0]->GetPid(), workloads[1]->GetPid());
  ASSERT_TRUE(StatCmd()->Run({"-t", tid_list, "sleep", "1"}));
}

TEST(stat_cmd, no_monitored_threads) {
  ASSERT_FALSE(StatCmd()->Run({}));
  ASSERT_FALSE(StatCmd()->Run({""}));
}

TEST(stat_cmd, group_option) {
  TEST_REQUIRE_HW_COUNTER();
  ASSERT_TRUE(
      StatCmd()->Run({"--group", "cpu-clock,page-faults", "sleep", "1"}));
  ASSERT_TRUE(StatCmd()->Run({"--group", "cpu-cycles,instructions", "--group",
                              "cpu-cycles:u,instructions:u", "--group",
                              "cpu-cycles:k,instructions:k", "sleep", "1"}));
}

TEST(stat_cmd, auto_generated_summary) {
  TEST_REQUIRE_HW_COUNTER();
  TemporaryFile tmp_file;
  ASSERT_TRUE(StatCmd()->Run({"--group", "instructions:u,instructions:k", "-o",
                              tmp_file.path, "sleep", "1"}));
  std::string s;
  ASSERT_TRUE(android::base::ReadFileToString(tmp_file.path, &s));
  size_t pos = s.find("instructions:u");
  ASSERT_NE(s.npos, pos);
  pos = s.find("instructions:k", pos);
  ASSERT_NE(s.npos, pos);
  pos += strlen("instructions:k");
  // Check if the summary of instructions is generated.
  ASSERT_NE(s.npos, s.find("instructions", pos));
}

TEST(stat_cmd, duration_option) {
  ASSERT_TRUE(
      StatCmd()->Run({"--duration", "1.2", "-p", std::to_string(getpid()), "--in-app"}));
  ASSERT_TRUE(StatCmd()->Run({"--duration", "1", "sleep", "2"}));
}

TEST(stat_cmd, interval_option) {
  TemporaryFile tmp_file;
  ASSERT_TRUE(
    StatCmd()->Run({"--interval", "500.0", "--duration", "1.2", "-o",
          tmp_file.path, "sleep", "2"}));
  std::string s;
  ASSERT_TRUE(android::base::ReadFileToString(tmp_file.path, &s));
  size_t count = 0;
  size_t pos = 0;
  std::string subs = "statistics:";
  while((pos = s.find(subs, pos)) != s.npos) {
    pos += subs.size();
    ++count ;
  }
  ASSERT_EQ(count, 2UL);
}

TEST(stat_cmd, interval_option_in_system_wide) {
  TEST_IN_ROOT(ASSERT_TRUE(StatCmd()->Run({"-a", "--interval", "100", "--duration", "0.3"})));
}

TEST(stat_cmd, interval_only_values_option) {
  ASSERT_TRUE(StatCmd()->Run({"--interval", "500", "--interval-only-values", "sleep", "2"}));
  TEST_IN_ROOT(ASSERT_TRUE(StatCmd()->Run({"-a", "--interval", "100", "--interval-only-values",
                                           "--duration", "0.3"})));
}

TEST(stat_cmd, no_modifier_for_clock_events) {
  for (const std::string& e : {"cpu-clock", "task-clock"}) {
    for (const std::string& m : {"u", "k"}) {
      ASSERT_FALSE(StatCmd()->Run({"-e", e + ":" + m, "sleep", "0.1"}))
          << "event " << e << ":" << m;
    }
  }
}

TEST(stat_cmd, handle_SIGHUP) {
  std::thread thread([]() {
    sleep(1);
    kill(getpid(), SIGHUP);
  });
  thread.detach();
  ASSERT_TRUE(StatCmd()->Run({"sleep", "1000000"}));
}

TEST(stat_cmd, stop_when_no_more_targets) {
  std::atomic<int> tid(0);
  std::thread thread([&]() {
    tid = gettid();
    sleep(1);
  });
  thread.detach();
  while (tid == 0);
  ASSERT_TRUE(StatCmd()->Run({"-t", std::to_string(tid), "--in-app"}));
}

TEST(stat_cmd, sample_speed_should_be_zero) {
  TEST_REQUIRE_HW_COUNTER();
  EventSelectionSet set(true);
  ASSERT_TRUE(set.AddEventType("cpu-cycles"));
  set.AddMonitoredProcesses({getpid()});
  ASSERT_TRUE(set.OpenEventFiles({-1}));
  std::vector<EventAttrWithId> attrs = set.GetEventAttrWithId();
  ASSERT_GT(attrs.size(), 0u);
  for (auto& attr : attrs) {
    ASSERT_EQ(attr.attr->sample_period, 0u);
    ASSERT_EQ(attr.attr->sample_freq, 0u);
    ASSERT_EQ(attr.attr->freq, 0u);
  }
}

TEST(stat_cmd, calculating_cpu_frequency) {
  TEST_REQUIRE_HW_COUNTER();
  CaptureStdout capture;
  ASSERT_TRUE(capture.Start());
  ASSERT_TRUE(StatCmd()->Run({"--csv", "--group", "task-clock,cpu-cycles", "sleep", "1"}));
  std::string output = capture.Finish();
  double task_clock_in_ms = 0;
  uint64_t cpu_cycle_count = 0;
  double cpu_frequency = 0;
  for (auto& line : android::base::Split(output, "\n")) {
    if (line.find("task-clock") != std::string::npos) {
      ASSERT_EQ(sscanf(line.c_str(), "%lf(ms)", &task_clock_in_ms), 1);
    } else if (line.find("cpu-cycles") != std::string::npos) {
      ASSERT_EQ(sscanf(line.c_str(), "%" SCNu64 ",cpu-cycles,%lf", &cpu_cycle_count,
                       &cpu_frequency), 2);
    }
  }
  ASSERT_NE(task_clock_in_ms, 0.0f);
  ASSERT_NE(cpu_cycle_count, 0u);
  ASSERT_NE(cpu_frequency, 0.0f);
  double calculated_frequency = cpu_cycle_count / task_clock_in_ms / 1e6;
  // Accept error up to 1e-3. Because the stat cmd print values with precision 1e-6.
  ASSERT_NEAR(cpu_frequency, calculated_frequency, 1e-3);
}

TEST(stat_cmd, set_comm_in_another_thread) {
  // Test a kernel bug which was fixed in 3.15. If kernel panic happens, please cherry pick kernel
  // patch: e041e328c4b41e perf: Fix perf_event_comm() vs. exec() assumption
  TEST_REQUIRE_HW_COUNTER();

  for (size_t loop = 0; loop < 3; ++loop) {
    std::atomic<int> child_tid(0);
    std::atomic<bool> stop_child(false);
    std::thread child([&]() {
      child_tid = gettid();
      // stay on a cpu to make the monitored events of the child thread on that cpu.
      while (!stop_child) {}
    });

    while (child_tid == 0) {}

    {
      EventSelectionSet set(true);
      ASSERT_TRUE(set.AddEventType("cpu-cycles"));
      set.AddMonitoredThreads({child_tid});
      ASSERT_TRUE(set.OpenEventFiles({-1}));

      EventSelectionSet set2(true);
      ASSERT_TRUE(set2.AddEventType("instructions"));
      set2.AddMonitoredThreads({gettid()});
      ASSERT_TRUE(set2.OpenEventFiles({-1}));

      // For kernels with the bug, setting comm will make the monitored events of the child thread
      // on the cpu of the current thread.
      ASSERT_TRUE(android::base::WriteStringToFile("child",
                                                   "/proc/" + std::to_string(child_tid) + "/comm"));
      // Release monitored events. For kernels with the bug, the events still exist on the cpu of
      // the child thread.
    }

    stop_child = true;
    child.join();
    // Sleep 1s to enter and exit cpu idle, which may abort the kernel.
    sleep(1);
  }
}

static void TestStatingApps(const std::string& app_name) {
  // Bring the app to foreground.
  ASSERT_TRUE(Workload::RunCmd({"am", "start", app_name + "/.MainActivity"}));
  ASSERT_TRUE(StatCmd()->Run({"--app", app_name, "--duration", "3"}));
}

TEST(stat_cmd, app_option_for_debuggable_app) {
  TEST_REQUIRE_APPS();
  TestStatingApps("com.android.simpleperf.debuggable");
}

TEST(stat_cmd, app_option_for_profileable_app) {
  TEST_REQUIRE_APPS();
  TestStatingApps("com.android.simpleperf.profileable");
}
