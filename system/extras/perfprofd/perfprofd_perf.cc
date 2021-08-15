/*
**
** Copyright 2015, The Android Open Source Project
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

#include "perfprofd_perf.h"


#include <inttypes.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>

#include "config.h"

namespace android {
namespace perfprofd {

namespace {

std::unordered_set<std::string>& GetSupportedPerfCountersInternal() {
  static std::unordered_set<std::string>& vec = *new std::unordered_set<std::string>();
  return vec;
}

}  // namespace

//
// Invoke "perf record". Return value is OK_PROFILE_COLLECTION for
// success, or some other error code if something went wrong.
//
PerfResult InvokePerf(Config& config,
                      const std::string &perf_path,
                      const char *stack_profile_opt,
                      unsigned duration,
                      const std::string &data_file_path,
                      const std::string &perf_stderr_path)
{
  std::vector<std::string> argv_backing;
  std::vector<const char*> argv_vector;
  char paranoid_env[] = "PERFPROFD_DISABLE_PERF_EVENT_PARANOID_CHANGE=1";
  char* envp[2] = {paranoid_env, nullptr};

  {
    auto add = [&argv_backing](auto arg) {
      argv_backing.push_back(arg);
    };

    add(perf_path);
    add("record");

    // -o perf.data
    add("-o");
    add(data_file_path);

    // -c/f N
    std::string p_str;
    if (config.sampling_frequency > 0) {
      add("-f");
      add(android::base::StringPrintf("%u", config.sampling_frequency));
    } else if (config.sampling_period > 0) {
      add("-c");
      add(android::base::StringPrintf("%u", config.sampling_period));
    }

    if (!config.event_config.empty()) {
      const std::unordered_set<std::string>& supported = GetSupportedPerfCountersInternal();
      for (const auto& event_set : config.event_config) {
        if (event_set.events.empty()) {
          LOG(WARNING) << "Unexpected empty event set";
          continue;
        }

        std::ostringstream event_str;
        bool added = false;
        for (const std::string& event : event_set.events) {
          if (supported.find(event) == supported.end()) {
            LOG(WARNING) << "Event " << event << " is unsupported.";
            if (config.fail_on_unsupported_events) {
              return PerfResult::kUnsupportedEvent;
            }
            continue;
          }
          if (added) {
            event_str << ',';
          }
          event_str << event;
          added = true;
        }

        if (!added) {
          continue;
        }

        if (event_set.sampling_period > 0) {
          add("-c");
          add(std::to_string(event_set.sampling_period));
        }
        add(event_set.group ? "--group" : "-e");
        add(event_str.str());
      }
    }

    // -g if desired
    if (stack_profile_opt != nullptr) {
      add(stack_profile_opt);
      add("-m");
      add("8192");
    }

    if (config.process < 0) {
      // system wide profiling
      add("-a");
    } else {
      add("-p");
      add(std::to_string(config.process));
    }

    // no need for kernel or other symbols
    add("--no-dump-kernel-symbols");
    add("--no-dump-symbols");

    // sleep <duration>
    add("--duration");
    add(android::base::StringPrintf("%u", duration));


    // Now create the char* buffer.
    argv_vector.resize(argv_backing.size() + 1, nullptr);
    std::transform(argv_backing.begin(),
                   argv_backing.end(),
                   argv_vector.begin(),
                   [](const std::string& in) { return in.c_str(); });
  }

  pid_t pid = fork();

  if (pid == -1) {
    PLOG(ERROR) << "Fork failed";
    return PerfResult::kForkFailed;
  }

  if (pid == 0) {
    // child

    // Open file to receive stderr/stdout from perf
    FILE *efp = fopen(perf_stderr_path.c_str(), "w");
    if (efp) {
      dup2(fileno(efp), STDERR_FILENO);
      dup2(fileno(efp), STDOUT_FILENO);
    } else {
      PLOG(WARNING) << "unable to open " << perf_stderr_path << " for writing";
    }

    // record the final command line in the error output file for
    // posterity/debugging purposes
    fprintf(stderr, "perf invocation (pid=%d):\n", getpid());
    for (unsigned i = 0; argv_vector[i] != nullptr; ++i) {
      fprintf(stderr, "%s%s", i ? " " : "", argv_vector[i]);
    }
    fprintf(stderr, "\n");

    // exec
    execvpe(argv_vector[0], const_cast<char* const*>(argv_vector.data()), envp);
    fprintf(stderr, "exec failed: %s\n", strerror(errno));
    exit(1);

  } else {
    // parent

    // Try to sleep.
    config.Sleep(duration);

    // We may have been woken up to stop profiling.
    if (config.ShouldStopProfiling()) {
      // Send SIGHUP to simpleperf to make it stop.
      kill(pid, SIGHUP);
    }

    // Wait for the child, so it's reaped correctly.
    int st = 0;
    pid_t reaped = TEMP_FAILURE_RETRY(waitpid(pid, &st, 0));

    auto print_perferr = [&perf_stderr_path]() {
      std::string tmp;
      if (android::base::ReadFileToString(perf_stderr_path, &tmp)) {
        LOG(WARNING) << tmp;
      } else {
        PLOG(WARNING) << "Could not read " << perf_stderr_path;
      }
    };

    if (reaped == -1) {
      PLOG(WARNING) << "waitpid failed";
    } else if (WIFSIGNALED(st)) {
      if (WTERMSIG(st) == SIGHUP && config.ShouldStopProfiling()) {
        // That was us...
        return PerfResult::kOK;
      }
      LOG(WARNING) << "perf killed by signal " << WTERMSIG(st);
      print_perferr();
    } else if (WEXITSTATUS(st) != 0) {
      LOG(WARNING) << "perf bad exit status " << WEXITSTATUS(st);
      print_perferr();
    } else {
      return PerfResult::kOK;
    }
  }

  return PerfResult::kRecordFailed;
}

bool FindSupportedPerfCounters(const std::string& perf_path) {
  const char* argv[] = { perf_path.c_str(), "list", nullptr };
  char paranoid_env[] = "PERFPROFD_DISABLE_PERF_EVENT_PARANOID_CHANGE=1";
  char* envp[2] = {paranoid_env, nullptr};

  base::unique_fd link[2];
  {
    int link_fd[2];

    if (pipe(link_fd) == -1) {
      PLOG(ERROR) << "Pipe failed";
      return false;
    }
    link[0].reset(link_fd[0]);
    link[1].reset(link_fd[1]);
  }

  pid_t pid = fork();

  if (pid == -1) {
    PLOG(ERROR) << "Fork failed";
    return PerfResult::kForkFailed;
  }

  if (pid == 0) {
    // Child

    // Redirect stdout and stderr.
    dup2(link[1].get(), STDOUT_FILENO);
    dup2(link[1].get(), STDERR_FILENO);

    link[0].reset();
    link[1].reset();

    // exec
    execvpe(argv[0], const_cast<char* const*>(argv), envp);
    PLOG(WARNING) << "exec failed";
    exit(1);
    __builtin_unreachable();
  }

  link[1].reset();

  std::string result;
  if (!android::base::ReadFdToString(link[0].get(), &result)) {
    PLOG(WARNING) << perf_path << " list reading failed.";
  }

  link[0].reset();

  int status_code;
  if (waitpid(pid, &status_code, 0) == -1) {
    LOG(WARNING) << "Failed to wait for " << perf_path << " list";
    return false;
  }

  if (!WIFEXITED(status_code) || WEXITSTATUS(status_code) != 0) {
    LOG(WARNING) << perf_path << " list did not exit normally.";
    return false;
  }

  std::unordered_set<std::string>& supported = GetSupportedPerfCountersInternal();
  supported.clear();

  // Could implement something with less memory requirements. But for now this is good
  // enough.
  std::vector<std::string> lines = base::Split(result, "\n");
  for (const std::string& line : lines) {
    if (line.length() < 2 || line.compare(0, 2, "  ") != 0) {
      continue;
    }
    const size_t comment = line.find('#');
    const size_t space = line.find(' ', 2);
    size_t end = std::min(space, comment);
    if (end != std::string::npos) {
      // Scan backwards.
      --end;
      while (end > 2 && isspace(line[end])) {
        end--;
      }
    }
    if (end > 2) {
      supported.insert(line.substr(2, end - 2));
    }
  }

  return true;
}

const std::unordered_set<std::string>& GetSupportedPerfCounters() {
  return GetSupportedPerfCountersInternal();
}

}  // namespace perfprofd
}  // namespace android
