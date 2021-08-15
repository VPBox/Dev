/*
 * Copyright (C) 2016, The Android Open Source Project
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

#include "wificond/tests/shell_utils.h"

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/unique_fd.h>

using android::base::unique_fd;

namespace android {
namespace wificond {
namespace tests {
namespace integration {
namespace {

#ifdef __ANDROID__
const char kShellPath[] = "/system/bin/sh";
#else
const char kShellPath[] = "/bin/sh";
#endif

const int kShellTimeoutMs = 30 * 1000;
const int kMillisecondsPerSecond = 1000;
const int kNanosecondsPerMillisecond = 1000 * 1000;

// Represents some arbitrary, non-decreasing time in milliseconds.
int64_t GetCurrentTimeMs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (int64_t{ts.tv_sec} * kMillisecondsPerSecond) +
         (ts.tv_nsec / kNanosecondsPerMillisecond);
}

}  // namespace

int RunShellCommand(const std::string& shell_command, std::string* output) {
  int fds[2];
  if (pipe2(fds, O_NONBLOCK) != 0) {
    LOG(FATAL) << "Failed to create pipe";
  }
  unique_fd read_fd(fds[0]);
  unique_fd write_fd(fds[1]);
  fcntl(read_fd.get(), F_SETFL, O_CLOEXEC | O_NONBLOCK);

  const pid_t child_pid = fork();
  if (child_pid == -1) {
    LOG(FATAL) << "Failed to fork child for shell command: " << shell_command;
  }

  if (child_pid == 0) {  // We are in the child process.
    close(0);  // Don't want to read anything in this process.
    dup2(write_fd.get(), 1);  // Replace existing stdout with the pipe.
    read_fd.reset();
    write_fd.reset();
    // Note that we're keeping parent stderr.
    execl(kShellPath, "sh", "-c", shell_command.c_str(), nullptr);
    PLOG(FATAL) << "exec() of child failed";
  }

  // We are in the parent process.
  write_fd.reset();  // Close this or we never get HUP from child.
  struct pollfd shell_output;
  memset(&shell_output, 0, sizeof(shell_output));
  shell_output.fd = read_fd.get();
  shell_output.events = POLLIN;

  ssize_t nread;
  char buf[512];
  int64_t start_time_ms = GetCurrentTimeMs();
  while (GetCurrentTimeMs() - start_time_ms < kShellTimeoutMs) {
    int64_t time_left_ms = kShellTimeoutMs - (GetCurrentTimeMs() - start_time_ms);
    poll(&shell_output, 1, (time_left_ms < 0) ? 0 : time_left_ms);
    // Blindly read from this file descriptor until there is no data available.
    do {
      nread = TEMP_FAILURE_RETRY(read(shell_output.fd, buf, sizeof(buf)));
      if (output && nread > 0) {
        output->append(buf, nread);
      }
    } while (nread > 0);

    // We're done if the child process has closed its stdout.
    if (shell_output.revents & POLLHUP) {
      break;
    }
  }

  // Reap our child's exit status.
  int wait_status = 0;
  int waitpid_ret = 0;
  start_time_ms = GetCurrentTimeMs();
  auto NeedToWaitForChild = [child_pid, &wait_status, &waitpid_ret]() {
    if (waitpid_ret == 0) {
      waitpid_ret = waitpid(child_pid, &wait_status, WNOHANG);
      if (waitpid_ret == -1) {
        PLOG(ERROR) << "waitpid() failed";
      }
    }
    return waitpid_ret == 0;
  };

  start_time_ms = GetCurrentTimeMs();
  while (NeedToWaitForChild() && GetCurrentTimeMs() - start_time_ms < 1000) {
    usleep(1000);
  }

  // Child still hasn't died.  Send our child the big hammer.
  if (waitpid_ret != child_pid) {
    int kill_ret = kill(child_pid, SIGKILL);
    // Allow kill to fail with ESRCH, since it indicated that the child may
    // have already died.
    if (kill_ret != 0 && errno != ESRCH) {
      PLOG(ERROR) << "Failed to send signal to child";
    }

    // Wait for the child to die after receiving that signal.
    start_time_ms = GetCurrentTimeMs();
    while (NeedToWaitForChild() && GetCurrentTimeMs() - start_time_ms < 1000) {
      usleep(1000);
    }
  }

  if (waitpid_ret == child_pid && WIFEXITED(wait_status)) {
    return WEXITSTATUS(wait_status);
  }

  LOG(ERROR) << "Shell command timed out.";
  return -1;
}

}  // namespace integration
}  // namespace tests
}  // namespace wificond
}  // namespace android
