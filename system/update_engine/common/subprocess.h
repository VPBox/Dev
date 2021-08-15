//
// Copyright (C) 2011 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_COMMON_SUBPROCESS_H_
#define UPDATE_ENGINE_COMMON_SUBPROCESS_H_

#include <unistd.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <base/callback.h>
#include <base/logging.h>
#include <base/macros.h>
#include <brillo/asynchronous_signal_handler_interface.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/process.h>
#include <brillo/process_reaper.h>
#include <gtest/gtest_prod.h>  // for FRIEND_TEST

// The Subprocess class is a singleton. It's used to spawn off a subprocess
// and get notified when the subprocess exits. The result of Exec() can
// be saved and used to cancel the callback request and kill your process. If
// you know you won't call KillExec(), you may safely lose the return value
// from Exec().

// To create the Subprocess singleton just instantiate it with and call Init().
// You can't have two Subprocess instances initialized at the same time.

namespace chromeos_update_engine {

class Subprocess {
 public:
  enum Flags {
    kSearchPath = 1 << 0,
    kRedirectStderrToStdout = 1 << 1,
  };

  // Callback type used when an async process terminates. It receives the exit
  // code and the stdout output (and stderr if redirected).
  using ExecCallback = base::Callback<void(int, const std::string&)>;

  Subprocess() = default;

  // Destroy and unregister the Subprocess singleton.
  ~Subprocess();

  // Initialize and register the Subprocess singleton.
  void Init(brillo::AsynchronousSignalHandlerInterface* async_signal_handler);

  // Launches a process in the background and calls the passed |callback| when
  // the process exits. The file descriptors specified in |output_pipes| will
  // be available in the child as the writer end of a pipe. Use GetPipeFd() to
  // know the reader end in the parent. Only stdin, stdout, stderr and the file
  // descriptors in |output_pipes| will be open in the child.
  // Returns the process id of the new launched process or 0 in case of failure.
  pid_t Exec(const std::vector<std::string>& cmd, const ExecCallback& callback);
  pid_t ExecFlags(const std::vector<std::string>& cmd,
                  uint32_t flags,
                  const std::vector<int>& output_pipes,
                  const ExecCallback& callback);

  // Kills the running process with SIGTERM and ignores the callback.
  void KillExec(pid_t pid);

  // Return the parent end of the pipe mapped onto |fd| in the child |pid|. This
  // file descriptor is available until the callback for the child |pid|
  // returns. After that the file descriptor will be closed. The passed |fd|
  // must be one of the file descriptors passed to ExecFlags() in
  // |output_pipes|, otherwise returns -1.
  int GetPipeFd(pid_t pid, int fd) const;

  // Executes a command synchronously. Returns true on success. If |stdout| is
  // non-null, the process output is stored in it, otherwise the output is
  // logged. Note that stderr is redirected to stdout.
  static bool SynchronousExec(const std::vector<std::string>& cmd,
                              int* return_code,
                              std::string* stdout);
  static bool SynchronousExecFlags(const std::vector<std::string>& cmd,
                                   uint32_t flags,
                                   int* return_code,
                                   std::string* stdout);

  // Gets the one instance.
  static Subprocess& Get() { return *subprocess_singleton_; }

  // Tries to log all in flight processes's output. It is used right before
  // exiting the update_engine, probably when the subprocess caused a system
  // shutdown.
  void FlushBufferedLogsAtExit();

 private:
  FRIEND_TEST(SubprocessTest, CancelTest);

  struct SubprocessRecord {
    explicit SubprocessRecord(const ExecCallback& callback)
        : callback(callback) {}

    // The callback supplied by the caller.
    ExecCallback callback;

    // The ProcessImpl instance managing the child process. Destroying this
    // will close our end of the pipes we have open.
    brillo::ProcessImpl proc;

    // These are used to monitor the stdout of the running process, including
    // the stderr if it was redirected.
    brillo::MessageLoop::TaskId stdout_task_id{
        brillo::MessageLoop::kTaskIdNull};
    int stdout_fd{-1};
    std::string stdout;
  };

  // Callback which runs whenever there is input available on the subprocess
  // stdout pipe.
  static void OnStdoutReady(SubprocessRecord* record);

  // Callback for when any subprocess terminates. This calls the user
  // requested callback.
  void ChildExitedCallback(const siginfo_t& info);

  // The global instance.
  static Subprocess* subprocess_singleton_;

  // A map from the asynchronous subprocess tag (see Exec) to the subprocess
  // record structure for all active asynchronous subprocesses.
  std::map<pid_t, std::unique_ptr<SubprocessRecord>> subprocess_records_;

  // Used to watch for child processes.
  brillo::ProcessReaper process_reaper_;

  DISALLOW_COPY_AND_ASSIGN(Subprocess);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_SUBPROCESS_H_
