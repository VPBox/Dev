/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "simpleperf.h"

#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <mutex>
#include <sstream>
#include <android/log.h>

namespace simpleperf {

enum RecordCmd {
  CMD_PAUSE_RECORDING = 1,
  CMD_RESUME_RECORDING,
};

class RecordOptionsImpl {
 public:
  std::string output_filename;
  std::string event = "cpu-cycles";
  size_t freq = 4000;
  double duration_in_second = 0.0;
  std::vector<pid_t> threads;
  bool dwarf_callgraph = false;
  bool fp_callgraph = false;
  bool trace_offcpu = false;
};

RecordOptions::RecordOptions() : impl_(new RecordOptionsImpl) {
}

RecordOptions::~RecordOptions() {
  delete impl_;
}

RecordOptions& RecordOptions::SetOutputFilename(const std::string &filename) {
  impl_->output_filename = filename;
  return *this;
}

RecordOptions& RecordOptions::SetEvent(const std::string &event) {
  impl_->event = event;
  return *this;
}

RecordOptions& RecordOptions::SetSampleFrequency(size_t freq) {
  impl_->freq = freq;
  return *this;
}

RecordOptions& RecordOptions::SetDuration(double duration_in_second) {
  impl_->duration_in_second = duration_in_second;
  return *this;
}

RecordOptions& RecordOptions::SetSampleThreads(const std::vector<pid_t> &threads) {
  impl_->threads = threads;
  return *this;
}

RecordOptions& RecordOptions::RecordDwarfCallGraph() {
  impl_->dwarf_callgraph = true;
  impl_->fp_callgraph = false;
  return *this;
}

RecordOptions& RecordOptions::RecordFramePointerCallGraph() {
  impl_->fp_callgraph = true;
  impl_->dwarf_callgraph = false;
  return *this;
}

RecordOptions& RecordOptions::TraceOffCpu() {
  impl_->trace_offcpu = true;
  return *this;
}

static std::string GetDefaultOutputFilename() {
  time_t t = time(nullptr);
  struct tm tm;
  if (localtime_r(&t, &tm) != &tm) {
    return "perf.data";
  }
  char* buf = nullptr;
  asprintf(&buf, "perf-%02d-%02d-%02d-%02d-%02d.data", tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
           tm.tm_min, tm.tm_sec);
  std::string result = buf;
  free(buf);
  return result;
}

std::vector<std::string> RecordOptions::ToRecordArgs() const {
  std::vector<std::string> args;
  std::string output_filename = impl_->output_filename;
  if (output_filename.empty()) {
    output_filename = GetDefaultOutputFilename();
  }
  args.insert(args.end(), {"-o", output_filename});
  args.insert(args.end(), {"-e", impl_->event});
  args.insert(args.end(), {"-f", std::to_string(impl_->freq)});
  if (impl_->duration_in_second != 0.0) {
    args.insert(args.end(), {"--duration", std::to_string(impl_->duration_in_second)});
  }
  if (impl_->threads.empty()) {
    args.insert(args.end(), {"-p", std::to_string(getpid())});
  } else {
    std::ostringstream os;
    os << *(impl_->threads.begin());
    for (auto it = std::next(impl_->threads.begin()); it != impl_->threads.end(); ++it) {
      os << "," << *it;
    }
    args.insert(args.end(), {"-t", os.str()});
  }
  if (impl_->dwarf_callgraph) {
    args.push_back("-g");
  } else if (impl_->fp_callgraph) {
    args.insert(args.end(), {"--call-graph", "fp"});
  }
  if (impl_->trace_offcpu) {
    args.push_back("--trace-offcpu");
  }
  return args;
}

static void Abort(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  __android_log_vprint(ANDROID_LOG_FATAL, "simpleperf", fmt, vl);
  va_end(vl);
  abort();
}

class ProfileSessionImpl {
 public:
  ProfileSessionImpl(const std::string& app_data_dir)
      : app_data_dir_(app_data_dir),
        simpleperf_data_dir_(app_data_dir + "/simpleperf_data") {}
  ~ProfileSessionImpl();
  void StartRecording(const std::vector<std::string>& args);
  void PauseRecording();
  void ResumeRecording();
  void StopRecording();

 private:
  std::string FindSimpleperf();
  std::string FindSimpleperfInTempDir();
  void CheckIfPerfEnabled();
  void CreateSimpleperfDataDir();
  void CreateSimpleperfProcess(const std::string& simpleperf_path,
                               const std::vector<std::string>& record_args);
  void SendCmd(const std::string& cmd);
  std::string ReadReply();

  enum State {
    NOT_YET_STARTED,
    STARTED,
    PAUSED,
    STOPPED,
  };

  const std::string app_data_dir_;
  const std::string simpleperf_data_dir_;
  std::mutex lock_;  // Protect all members below.
  State state_ = NOT_YET_STARTED;
  pid_t simpleperf_pid_ = -1;
  int control_fd_ = -1;
  int reply_fd_ = -1;
};

ProfileSessionImpl::~ProfileSessionImpl() {
  if (control_fd_ != -1) {
    close(control_fd_);
  }
  if (reply_fd_ != -1) {
    close(reply_fd_);
  }
}

void ProfileSessionImpl::StartRecording(const std::vector<std::string> &args) {
  std::lock_guard<std::mutex> guard(lock_);
  if (state_ != NOT_YET_STARTED) {
    Abort("startRecording: session in wrong state %d", state_);
  }
  std::string simpleperf_path = FindSimpleperf();
  CheckIfPerfEnabled();
  CreateSimpleperfDataDir();
  CreateSimpleperfProcess(simpleperf_path, args);
  state_ = STARTED;
}

void ProfileSessionImpl::PauseRecording() {
  std::lock_guard<std::mutex> guard(lock_);
  if (state_ != STARTED) {
    Abort("pauseRecording: session in wrong state %d", state_);
  }
  SendCmd("pause");
  state_ = PAUSED;
}

void ProfileSessionImpl::ResumeRecording() {
  std::lock_guard<std::mutex> guard(lock_);
  if (state_ != PAUSED) {
    Abort("resumeRecording: session in wrong state %d", state_);
  }
  SendCmd("resume");
  state_ = STARTED;
}

void ProfileSessionImpl::StopRecording() {
  std::lock_guard<std::mutex> guard(lock_);
  if (state_ != STARTED && state_ != PAUSED) {
    Abort("stopRecording: session in wrong state %d", state_);
  }
  // Send SIGINT to simpleperf to stop recording.
  if (kill(simpleperf_pid_, SIGINT) == -1) {
    Abort("failed to stop simpleperf: %s", strerror(errno));
  }
  int status;
  pid_t result = TEMP_FAILURE_RETRY(waitpid(simpleperf_pid_, &status, 0));
  if (result == -1) {
    Abort("failed to call waitpid: %s", strerror(errno));
  }
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    Abort("simpleperf exited with error, status = 0x%x", status);
  }
  state_ = STOPPED;
}

void ProfileSessionImpl::SendCmd(const std::string& cmd) {
  std::string data = cmd + "\n";
  if (TEMP_FAILURE_RETRY(write(control_fd_, &data[0], data.size())) !=
                         static_cast<ssize_t>(data.size())) {
    Abort("failed to send cmd to simpleperf: %s", strerror(errno));
  }
  if (ReadReply() != "ok") {
    Abort("failed to run cmd in simpleperf: %s", cmd.c_str());
  }
}

static bool IsExecutableFile(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == 0) {
    if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR)) {
      return true;
    }
  }
  return false;
}

std::string ProfileSessionImpl::FindSimpleperf() {
  // 1. Try /data/local/tmp/simpleperf first. Probably it's newer than /system/bin/simpleperf.
  std::string simpleperf_path = FindSimpleperfInTempDir();
  if (!simpleperf_path.empty()) {
    return simpleperf_path;
  }
  // 2. Try /system/bin/simpleperf, which is available on Android >= Q.
  simpleperf_path = "/system/bin/simpleperf";
  if (IsExecutableFile(simpleperf_path)) {
    return simpleperf_path;
  }
  Abort("can't find simpleperf on device. Please run api_profiler.py.");
  return "";
}

std::string ProfileSessionImpl::FindSimpleperfInTempDir() {
  const std::string path = "/data/local/tmp/simpleperf";
  if (!IsExecutableFile(path)) {
    return "";
  }
  // Copy it to app_dir to execute it.
  const std::string to_path = app_data_dir_ + "/simpleperf";
  const std::string copy_cmd = "cp " + path + " " + to_path;
  if (system(copy_cmd.c_str()) != 0) {
    return "";
  }
  const std::string test_cmd = to_path;
  // For apps with target sdk >= 29, executing app data file isn't allowed. So test executing it.
  if (system(test_cmd.c_str()) != 0) {
    return "";
  }
  return to_path;
}

static std::string ReadFile(FILE* fp) {
  std::string s;
  char buf[200];
  while (true) {
    ssize_t n = fread(buf, 1, sizeof(buf), fp);
    if (n <= 0) {
      break;
    }
    s.insert(s.end(), buf, buf + n);
  }
  return s;
}

void ProfileSessionImpl::CheckIfPerfEnabled() {
  FILE* fp = popen("/system/bin/getprop security.perf_harden", "re");
  if (fp == nullptr) {
    return;  // Omit check if getprop doesn't exist.
  }
  std::string s = ReadFile(fp);
  pclose(fp);
  if (!s.empty() && s[0] == '1') {
    Abort("linux perf events aren't enabled on the device. Please run api_profiler.py.");
  }
}

void ProfileSessionImpl::CreateSimpleperfDataDir() {
  struct stat st;
  if (stat(simpleperf_data_dir_.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
    return;
  }
  if (mkdir(simpleperf_data_dir_.c_str(), 0700) == -1) {
    Abort("failed to create simpleperf data dir %s: %s", simpleperf_data_dir_.c_str(),
          strerror(errno));
  }
}

void ProfileSessionImpl::CreateSimpleperfProcess(const std::string &simpleperf_path,
                                                 const std::vector<std::string> &record_args) {
  // 1. Create control/reply pips.
  int control_fd[2];
  int reply_fd[2];
  if (pipe(control_fd) != 0 || pipe(reply_fd) != 0) {
    Abort("failed to call pipe: %s", strerror(errno));
  }

  // 2. Prepare simpleperf arguments.
  std::vector<std::string> args;
  args.emplace_back(simpleperf_path);
  args.emplace_back("record");
  args.emplace_back("--log-to-android-buffer");
  args.insert(args.end(), {"--log", "debug"});
  args.emplace_back("--stdio-controls-profiling");
  args.emplace_back("--in-app");
  args.insert(args.end(), {"--tracepoint-events", "/data/local/tmp/tracepoint_events"});
  args.insert(args.end(), record_args.begin(), record_args.end());
  char* argv[args.size() + 1];
  for (size_t i = 0; i < args.size(); ++i) {
    argv[i] = &args[i][0];
  }
  argv[args.size()] = nullptr;

  // 3. Start simpleperf process.
  int pid = fork();
  if (pid == -1) {
    Abort("failed to fork: %s", strerror(errno));
  }
  if (pid == 0) {
    // child process
    close(control_fd[1]);
    dup2(control_fd[0], 0);  // simpleperf read control cmd from fd 0.
    close(control_fd[0]);
    close(reply_fd[0]);
    dup2(reply_fd[1], 1);  // simpleperf writes reply to fd 1.
    close(reply_fd[0]);
    chdir(simpleperf_data_dir_.c_str());
    execvp(argv[0], argv);
    Abort("failed to call exec: %s", strerror(errno));
  }
  // parent process
  close(control_fd[0]);
  control_fd_ = control_fd[1];
  close(reply_fd[1]);
  reply_fd_ = reply_fd[0];
  simpleperf_pid_ = pid;

  // 4. Wait until simpleperf starts recording.
  std::string start_flag = ReadReply();
  if (start_flag != "started") {
    Abort("failed to receive simpleperf start flag");
  }
}

std::string ProfileSessionImpl::ReadReply() {
  std::string s;
  while (true) {
    char c;
    ssize_t result = TEMP_FAILURE_RETRY(read(reply_fd_, &c, 1));
    if (result <= 0 || c == '\n') {
      break;
    }
    s.push_back(c);
  }
  return s;
}

ProfileSession::ProfileSession() {
  FILE* fp = fopen("/proc/self/cmdline", "r");
  if (fp == nullptr) {
    Abort("failed to open /proc/self/cmdline: %s", strerror(errno));
  }
  std::string s = ReadFile(fp);
  fclose(fp);
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '\0') {
      s = s.substr(0, i);
      break;
    }
  }
  std::string app_data_dir = "/data/data/" + s;
  impl_ = new ProfileSessionImpl(app_data_dir);
}

ProfileSession::ProfileSession(const std::string& app_data_dir)
    : impl_(new ProfileSessionImpl(app_data_dir)) {}

ProfileSession::~ProfileSession() {
  delete impl_;
}

void ProfileSession::StartRecording(const RecordOptions &options) {
  StartRecording(options.ToRecordArgs());
}

void ProfileSession::StartRecording(const std::vector<std::string> &record_args) {
   impl_->StartRecording(record_args);
}

void ProfileSession::PauseRecording() {
  impl_->PauseRecording();
}

void ProfileSession::ResumeRecording() {
  impl_->ResumeRecording();
}

void ProfileSession::StopRecording() {
  impl_->StopRecording();
}

}  // namespace simpleperf
