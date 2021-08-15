/*
**
** Copyright 2018, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_PERF_H_
#define SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_PERF_H_

#include <string>
#include <unordered_set>

struct Config;

namespace android {
namespace perfprofd {



enum PerfResult {
  kOK,
  kForkFailed,
  kRecordFailed,
  kUnsupportedEvent,
};

//
// Invoke "perf record". Return value is PerfResult::kOK for
// success, or some other error code if something went wrong.
//
PerfResult InvokePerf(Config& config,
                      const std::string &perf_path,
                      const char *stack_profile_opt,
                      unsigned duration,
                      const std::string &data_file_path,
                      const std::string &perf_stderr_path);

// Prepare the internal list of supported perf counters.
bool FindSupportedPerfCounters(const std::string& perf_path);
// Get the list of supported perf counters.
const std::unordered_set<std::string>& GetSupportedPerfCounters();

}  // namespace perfprofd
}  // namespace android

#endif  // SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_PERF_H_
