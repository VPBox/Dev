/*
 *
 * Copyright 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSTEM_EXTRAS_PERFPROFD_CONFIG_H_
#define SYSTEM_EXTRAS_PERFPROFD_CONFIG_H_

#include <cstdint>
#include <string>
#include <vector>

struct Config {
  virtual ~Config() {}

  // Average number of seconds between perf profile collections (if
  // set to 100, then over time we want to see a perf profile
  // collected every 100 seconds). The actual time within the interval
  // for the collection is chosen randomly.
  uint32_t collection_interval_in_s = 14400;
  // Use the specified fixed seed for random number generation (unit
  // testing)
  uint32_t use_fixed_seed = 0;
  // For testing purposes, number of times to iterate through main
  // loop.  Value of zero indicates that we should loop forever.
  uint32_t main_loop_iterations = 0;

  // The pid of the process to profile. May be negative, in which case
  // the whole system will be profiled.
  int32_t process = -1;

  // Destination directory (where to write profiles).
  std::string destination_directory = "/data/misc/perfprofd";
  // Config directory (where to read configs).
  std::string config_directory = "/data/data/com.google.android.gms/files";
  // Full path to 'perf' executable.
  std::string perf_path = "/system/bin/simpleperf";

  // Desired sampling period (passed to perf -c option). Small
  // sampling periods can perturb the collected profiles, so enforce
  // min/max. A value of 0 means perf default. sampling_frequency
  // takes priority.
  uint32_t sampling_period = 0;
  // Desired sampling frequency (passed to perf -f option). A value of 0
  // means using sampling_period or default.
  uint32_t sampling_frequency = 0;
  // Length of time to collect samples (number of seconds for 'perf
  // record -a' run).
  uint32_t sample_duration_in_s = 2;

  // If this parameter is non-zero it will cause perfprofd to
  // exit immediately if the build type is not userdebug or eng.
  // Currently defaults to 1 (true).
  bool only_debug_build = true;

  // If the "mpdecision" service is running at the point we are ready
  // to kick off a profiling run, then temporarily disable the service
  // and hard-wire all cores on prior to the collection run, provided
  // that the duration of the recording is less than or equal to the value of
  // 'hardwire_cpus_max_duration'.
  bool hardwire_cpus = true;
  uint32_t hardwire_cpus_max_duration_in_s = 5;

  // Maximum number of unprocessed profiles we can accumulate in the
  // destination directory. Once we reach this limit, we continue
  // to collect, but we just overwrite the most recent profile.
  uint32_t max_unprocessed_profiles = 10;

  // If set to 1, pass the -g option when invoking 'perf' (requests
  // stack traces as opposed to flat profile).
  bool stack_profile = false;

  // For unit testing only: if set to 1, emit info messages on config
  // file parsing.
  bool trace_config_read = false;

  // Control collection of various additional profile tags
  bool collect_cpu_utilization = true;
  bool collect_charging_state = true;
  bool collect_booting = true;
  bool collect_camera_active = false;

  // If true, use an ELF symbolizer to on-device symbolize.
  bool use_elf_symbolizer = true;
  // Whether to symbolize everything. If false, objects with build ID will be skipped.
  bool symbolize_everything = false;

  // If true, use libz to compress the output proto.
  bool compress = true;

  // If true, send the proto to dropbox instead to a file.
  bool send_to_dropbox = false;

  // Whether to fail or strip unsupported events.
  bool fail_on_unsupported_events = false;

  struct PerfCounterConfigElem {
    std::vector<std::string> events;
    bool group;
    uint32_t sampling_period;
  };
  std::vector<PerfCounterConfigElem> event_config;

  // Sleep for the given number of seconds.
  virtual void Sleep(size_t seconds) = 0;

  // Should the profiling be stopped immediately?
  virtual bool ShouldStopProfiling() {
    return false;
  }

  // Is profiling enabled?
  virtual bool IsProfilingEnabled() const = 0;
};

#endif  // SYSTEM_EXTRAS_PERFPROFD_CONFIG_H_
