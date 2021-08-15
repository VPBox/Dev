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

#include <stdio.h>
#include <map>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>

#include "command.h"
#include "environment.h"
#include "event_attr.h"
#include "event_fd.h"
#include "event_selection_set.h"
#include "event_type.h"

static bool IsEventTypeSupported(const EventType& event_type) {
  if (event_type.type != PERF_TYPE_RAW) {
    perf_event_attr attr = CreateDefaultPerfEventAttr(event_type);
    // Exclude kernel to list supported events even when
    // /proc/sys/kernel/perf_event_paranoid is 2.
    attr.exclude_kernel = 1;
    return IsEventAttrSupported(attr);
  }
  if (event_type.limited_arch == "arm" && GetBuildArch() != ARCH_ARM &&
      GetBuildArch() != ARCH_ARM64) {
    return false;
  }
  // Because the kernel may not check whether the raw event is supported by the cpu pmu.
  // We can't decide whether the raw event is supported by calling perf_event_open().
  // Instead, we can check if it can collect some real number.
  perf_event_attr attr = CreateDefaultPerfEventAttr(event_type);
  std::unique_ptr<EventFd> event_fd = EventFd::OpenEventFile(attr, gettid(), -1, nullptr);
  if (event_fd == nullptr) {
    return false;
  }
  auto work_function = []() {
    TemporaryFile tmpfile;
    FILE* fp = fopen(tmpfile.path, "w");
    if (fp == nullptr) {
      return;
    }
    for (int i = 0; i < 10; ++i) {
      fprintf(fp, "output some data\n");
    }
    fclose(fp);
  };
  work_function();
  PerfCounter counter;
  if (!event_fd->ReadCounter(&counter)) {
    return false;
  }
  return (counter.value != 0u);
}

static void PrintEventTypesOfType(uint32_t type, const std::string& type_name,
                                  const std::set<EventType>& event_types) {
  printf("List of %s:\n", type_name.c_str());
  if (type == PERF_TYPE_RAW && (GetBuildArch() == ARCH_ARM || GetBuildArch() == ARCH_ARM64)) {
    printf("  # Please refer to PMU event numbers listed in ARMv8 manual for details.\n");
    printf("  # A possible link is https://developer.arm.com/docs/ddi0487/latest/arm-architecture-reference-manual-armv8-for-armv8-a-architecture-profile.\n");
  }
  for (auto& event_type : event_types) {
    if (event_type.type == type) {
      if (IsEventTypeSupported(event_type)) {
        printf("  %s", event_type.name.c_str());
        if (!event_type.description.empty()) {
          printf("\t\t# %s", event_type.description.c_str());
        }
        printf("\n");
      }
    }
  }
  printf("\n");
}

class ListCommand : public Command {
 public:
  ListCommand()
      : Command("list", "list available event types",
                // clang-format off
"Usage: simpleperf list [options] [hw|sw|cache|raw|tracepoint]\n"
"       List all available event types.\n"
"       Filters can be used to show only event types belong to selected types:\n"
"         hw          hardware events\n"
"         sw          software events\n"
"         cache       hardware cache events\n"
"         raw         raw pmu events\n"
"         tracepoint  tracepoint events\n"
"Options:\n"
"--show-features    Show features supported on the device, including:\n"
"                     dwarf-based-call-graph\n"
"                     trace-offcpu\n"
                // clang-format on
                ) {
  }

  bool Run(const std::vector<std::string>& args) override;

 private:
  void ShowFeatures();
};

bool ListCommand::Run(const std::vector<std::string>& args) {
  if (!CheckPerfEventLimit()) {
    return false;
  }

  static std::map<std::string, std::pair<int, std::string>> type_map = {
      {"hw", {PERF_TYPE_HARDWARE, "hardware events"}},
      {"sw", {PERF_TYPE_SOFTWARE, "software events"}},
      {"cache", {PERF_TYPE_HW_CACHE, "hw-cache events"}},
      {"raw", {PERF_TYPE_RAW, "raw events provided by cpu pmu"}},
      {"tracepoint", {PERF_TYPE_TRACEPOINT, "tracepoint events"}},
      {"user-space-sampler", {SIMPLEPERF_TYPE_USER_SPACE_SAMPLERS, "user-space samplers"}},
  };

  std::vector<std::string> names;
  if (args.empty()) {
    for (auto& item : type_map) {
      names.push_back(item.first);
    }
  } else {
    for (auto& arg : args) {
      if (type_map.find(arg) != type_map.end()) {
        names.push_back(arg);
      } else if (arg == "--show-features") {
        ShowFeatures();
        return true;
      } else {
        LOG(ERROR) << "unknown event type category: " << arg << ", try using \"help list\"";
        return false;
      }
    }
  }

  auto& event_types = GetAllEventTypes();

  for (auto& name : names) {
    auto it = type_map.find(name);
    PrintEventTypesOfType(it->second.first, it->second.second, event_types);
  }
  return true;
}

void ListCommand::ShowFeatures() {
  if (IsDwarfCallChainSamplingSupported()) {
    printf("dwarf-based-call-graph\n");
  }
  if (IsDumpingRegsForTracepointEventsSupported()) {
    printf("trace-offcpu\n");
  }
  if (IsSettingClockIdSupported()) {
    printf("set-clockid\n");
  }
}

void RegisterListCommand() {
  RegisterCommand("list", [] { return std::unique_ptr<Command>(new ListCommand); });
}
