//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/common/subprocess.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/logging.h>
#include <base/posix/eintr_wrapper.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <brillo/process.h>
#include <brillo/secure_blob.h>

#include "update_engine/common/utils.h"

using brillo::MessageLoop;
using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

namespace {

bool SetupChild(const std::map<string, string>& env, uint32_t flags) {
  // Setup the environment variables.
  clearenv();
  for (const auto& key_value : env) {
    setenv(key_value.first.c_str(), key_value.second.c_str(), 0);
  }

  if ((flags & Subprocess::kRedirectStderrToStdout) != 0) {
    if (HANDLE_EINTR(dup2(STDOUT_FILENO, STDERR_FILENO)) != STDERR_FILENO)
      return false;
  }

  int fd = HANDLE_EINTR(open("/dev/null", O_RDONLY));
  if (fd < 0)
    return false;
  if (HANDLE_EINTR(dup2(fd, STDIN_FILENO)) != STDIN_FILENO)
    return false;
  IGNORE_EINTR(close(fd));

  return true;
}

// Helper function to launch a process with the given Subprocess::Flags.
// This function only sets up and starts the process according to the |flags|.
// The caller is responsible for watching the termination of the subprocess.
// Return whether the process was successfully launched and fills in the |proc|
// Process.
bool LaunchProcess(const vector<string>& cmd,
                   uint32_t flags,
                   const vector<int>& output_pipes,
                   brillo::Process* proc) {
  for (const string& arg : cmd)
    proc->AddArg(arg);
  proc->SetSearchPath((flags & Subprocess::kSearchPath) != 0);

  // Create an environment for the child process with just the required PATHs.
  std::map<string, string> env;
  for (const char* key : {"LD_LIBRARY_PATH", "PATH"}) {
    const char* value = getenv(key);
    if (value)
      env.emplace(key, value);
  }

  for (const int fd : output_pipes) {
    proc->RedirectUsingPipe(fd, false);
  }
  proc->SetCloseUnusedFileDescriptors(true);
  proc->RedirectUsingPipe(STDOUT_FILENO, false);
  proc->SetPreExecCallback(base::Bind(&SetupChild, env, flags));

  return proc->Start();
}

}  // namespace

void Subprocess::Init(
    brillo::AsynchronousSignalHandlerInterface* async_signal_handler) {
  if (subprocess_singleton_ == this)
    return;
  CHECK(subprocess_singleton_ == nullptr);
  subprocess_singleton_ = this;

  process_reaper_.Register(async_signal_handler);
}

Subprocess::~Subprocess() {
  if (subprocess_singleton_ == this)
    subprocess_singleton_ = nullptr;
}

void Subprocess::OnStdoutReady(SubprocessRecord* record) {
  char buf[1024];
  size_t bytes_read;
  do {
    bytes_read = 0;
    bool eof;
    bool ok = utils::ReadAll(
        record->stdout_fd, buf, arraysize(buf), &bytes_read, &eof);
    record->stdout.append(buf, bytes_read);
    if (!ok || eof) {
      // There was either an error or an EOF condition, so we are done watching
      // the file descriptor.
      MessageLoop::current()->CancelTask(record->stdout_task_id);
      record->stdout_task_id = MessageLoop::kTaskIdNull;
      return;
    }
  } while (bytes_read);
}

void Subprocess::ChildExitedCallback(const siginfo_t& info) {
  auto pid_record = subprocess_records_.find(info.si_pid);
  if (pid_record == subprocess_records_.end())
    return;
  SubprocessRecord* record = pid_record->second.get();

  // Make sure we read any remaining process output and then close the pipe.
  OnStdoutReady(record);

  MessageLoop::current()->CancelTask(record->stdout_task_id);
  record->stdout_task_id = MessageLoop::kTaskIdNull;

  // Don't print any log if the subprocess exited with exit code 0.
  if (info.si_code != CLD_EXITED) {
    LOG(INFO) << "Subprocess terminated with si_code " << info.si_code;
  } else if (info.si_status != 0) {
    LOG(INFO) << "Subprocess exited with si_status: " << info.si_status;
  }

  if (!record->stdout.empty()) {
    LOG(INFO) << "Subprocess output:\n" << record->stdout;
  }
  if (!record->callback.is_null()) {
    record->callback.Run(info.si_status, record->stdout);
  }
  // Release and close all the pipes after calling the callback so our
  // redirected pipes are still alive. Releasing the process first makes
  // Reset(0) not attempt to kill the process, which is already a zombie at this
  // point.
  record->proc.Release();
  record->proc.Reset(0);

  subprocess_records_.erase(pid_record);
}

pid_t Subprocess::Exec(const vector<string>& cmd,
                       const ExecCallback& callback) {
  return ExecFlags(cmd, kRedirectStderrToStdout, {}, callback);
}

pid_t Subprocess::ExecFlags(const vector<string>& cmd,
                            uint32_t flags,
                            const vector<int>& output_pipes,
                            const ExecCallback& callback) {
  unique_ptr<SubprocessRecord> record(new SubprocessRecord(callback));

  if (!LaunchProcess(cmd, flags, output_pipes, &record->proc)) {
    LOG(ERROR) << "Failed to launch subprocess";
    return 0;
  }

  pid_t pid = record->proc.pid();
  CHECK(process_reaper_.WatchForChild(
      FROM_HERE,
      pid,
      base::Bind(&Subprocess::ChildExitedCallback, base::Unretained(this))));

  record->stdout_fd = record->proc.GetPipe(STDOUT_FILENO);
  // Capture the subprocess output. Make our end of the pipe non-blocking.
  int fd_flags = fcntl(record->stdout_fd, F_GETFL, 0) | O_NONBLOCK;
  if (HANDLE_EINTR(fcntl(record->stdout_fd, F_SETFL, fd_flags)) < 0) {
    LOG(ERROR) << "Unable to set non-blocking I/O mode on fd "
               << record->stdout_fd << ".";
  }

  record->stdout_task_id = MessageLoop::current()->WatchFileDescriptor(
      FROM_HERE,
      record->stdout_fd,
      MessageLoop::WatchMode::kWatchRead,
      true,
      base::Bind(&Subprocess::OnStdoutReady, record.get()));

  subprocess_records_[pid] = std::move(record);
  return pid;
}

void Subprocess::KillExec(pid_t pid) {
  auto pid_record = subprocess_records_.find(pid);
  if (pid_record == subprocess_records_.end())
    return;
  pid_record->second->callback.Reset();
  // We don't care about output/return code, so we use SIGKILL here to ensure it
  // will be killed, SIGTERM might lead to leaked subprocess.
  if (kill(pid, SIGKILL) != 0) {
    PLOG(WARNING) << "Error sending SIGKILL to " << pid;
  }
  // Release the pid now so we don't try to kill it if Subprocess is destroyed
  // before the corresponding ChildExitedCallback() is called.
  pid_record->second->proc.Release();
}

int Subprocess::GetPipeFd(pid_t pid, int fd) const {
  auto pid_record = subprocess_records_.find(pid);
  if (pid_record == subprocess_records_.end())
    return -1;
  return pid_record->second->proc.GetPipe(fd);
}

bool Subprocess::SynchronousExec(const vector<string>& cmd,
                                 int* return_code,
                                 string* stdout) {
  // The default for SynchronousExec is to use kSearchPath since the code relies
  // on that.
  return SynchronousExecFlags(
      cmd, kRedirectStderrToStdout | kSearchPath, return_code, stdout);
}

bool Subprocess::SynchronousExecFlags(const vector<string>& cmd,
                                      uint32_t flags,
                                      int* return_code,
                                      string* stdout) {
  brillo::ProcessImpl proc;
  // It doesn't make sense to redirect some pipes in the synchronous case
  // because we won't be reading on our end, so we don't expose the output_pipes
  // in this case.
  if (!LaunchProcess(cmd, flags, {}, &proc)) {
    LOG(ERROR) << "Failed to launch subprocess";
    return false;
  }

  if (stdout) {
    stdout->clear();
  }

  int fd = proc.GetPipe(STDOUT_FILENO);
  vector<char> buffer(32 * 1024);
  while (true) {
    int rc = HANDLE_EINTR(read(fd, buffer.data(), buffer.size()));
    if (rc < 0) {
      PLOG(ERROR) << "Reading from child's output";
      break;
    } else if (rc == 0) {
      break;
    } else {
      if (stdout)
        stdout->append(buffer.data(), rc);
    }
  }
  // At this point, the subprocess already closed the output, so we only need to
  // wait for it to finish.
  int proc_return_code = proc.Wait();
  if (return_code)
    *return_code = proc_return_code;
  return proc_return_code != brillo::Process::kErrorExitStatus;
}

void Subprocess::FlushBufferedLogsAtExit() {
  if (!subprocess_records_.empty()) {
    LOG(INFO) << "We are exiting, but there are still in flight subprocesses!";
    for (auto& pid_record : subprocess_records_) {
      SubprocessRecord* record = pid_record.second.get();
      // Make sure we read any remaining process output.
      OnStdoutReady(record);
      if (!record->stdout.empty()) {
        LOG(INFO) << "Subprocess(" << pid_record.first << ") output:\n"
                  << record->stdout;
      }
    }
  }
}

Subprocess* Subprocess::subprocess_singleton_ = nullptr;

}  // namespace chromeos_update_engine
