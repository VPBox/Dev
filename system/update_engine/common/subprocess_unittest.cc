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
#include <poll.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>
#include <vector>

#include <base/bind.h>
#include <base/files/scoped_temp_dir.h>
#include <base/location.h>
#include <base/message_loop/message_loop.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>
#include <brillo/message_loops/base_message_loop.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <brillo/strings/string_utils.h>
#include <brillo/unittest_utils.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

using base::TimeDelta;
using brillo::MessageLoop;
using std::string;
using std::vector;

namespace {

#ifdef __ANDROID__
#define kBinPath "/system/bin"
#define kUsrBinPath "/system/bin"
#else
#define kBinPath "/bin"
#define kUsrBinPath "/usr/bin"
#endif  // __ANDROID__

}  // namespace

namespace chromeos_update_engine {

class SubprocessTest : public ::testing::Test {
 protected:
  void SetUp() override {
    loop_.SetAsCurrent();
    async_signal_handler_.Init();
    subprocess_.Init(&async_signal_handler_);
  }

  base::MessageLoopForIO base_loop_;
  brillo::BaseMessageLoop loop_{&base_loop_};
  brillo::AsynchronousSignalHandler async_signal_handler_;
  Subprocess subprocess_;
};

namespace {

void ExpectedResults(int expected_return_code,
                     const string& expected_output,
                     int return_code,
                     const string& output) {
  EXPECT_EQ(expected_return_code, return_code);
  EXPECT_EQ(expected_output, output);
  MessageLoop::current()->BreakLoop();
}

void ExpectedEnvVars(int return_code, const string& output) {
  EXPECT_EQ(0, return_code);
  const std::set<string> allowed_envs = {"LD_LIBRARY_PATH", "PATH"};
  for (const string& key_value : brillo::string_utils::Split(output, "\n")) {
    auto key_value_pair =
        brillo::string_utils::SplitAtFirst(key_value, "=", true);
    EXPECT_NE(allowed_envs.end(), allowed_envs.find(key_value_pair.first));
  }
  MessageLoop::current()->BreakLoop();
}

void ExpectedDataOnPipe(const Subprocess* subprocess,
                        pid_t* pid,
                        int child_fd,
                        const string& child_fd_data,
                        int expected_return_code,
                        int return_code,
                        const string& /* output */) {
  EXPECT_EQ(expected_return_code, return_code);

  // Verify that we can read the data from our end of |child_fd|.
  int fd = subprocess->GetPipeFd(*pid, child_fd);
  EXPECT_NE(-1, fd);
  vector<char> buf(child_fd_data.size() + 1);
  EXPECT_EQ(static_cast<ssize_t>(child_fd_data.size()),
            HANDLE_EINTR(read(fd, buf.data(), buf.size())));
  EXPECT_EQ(child_fd_data,
            string(buf.begin(), buf.begin() + child_fd_data.size()));

  MessageLoop::current()->BreakLoop();
}

}  // namespace

TEST_F(SubprocessTest, IsASingleton) {
  EXPECT_EQ(&subprocess_, &Subprocess::Get());
}

TEST_F(SubprocessTest, InactiveInstancesDontChangeTheSingleton) {
  std::unique_ptr<Subprocess> another_subprocess(new Subprocess());
  EXPECT_EQ(&subprocess_, &Subprocess::Get());
  another_subprocess.reset();
  EXPECT_EQ(&subprocess_, &Subprocess::Get());
}

TEST_F(SubprocessTest, SimpleTest) {
  EXPECT_TRUE(subprocess_.Exec({kBinPath "/false"},
                               base::Bind(&ExpectedResults, 1, "")));
  loop_.Run();
}

TEST_F(SubprocessTest, EchoTest) {
  EXPECT_TRUE(subprocess_.Exec(
      {kBinPath "/sh", "-c", "echo this is stdout; echo this is stderr >&2"},
      base::Bind(&ExpectedResults, 0, "this is stdout\nthis is stderr\n")));
  loop_.Run();
}

TEST_F(SubprocessTest, StderrNotIncludedInOutputTest) {
  EXPECT_TRUE(subprocess_.ExecFlags(
      {kBinPath "/sh", "-c", "echo on stdout; echo on stderr >&2"},
      0,
      {},
      base::Bind(&ExpectedResults, 0, "on stdout\n")));
  loop_.Run();
}

TEST_F(SubprocessTest, PipeRedirectFdTest) {
  pid_t pid;
  pid = subprocess_.ExecFlags(
      {kBinPath "/sh", "-c", "echo on pipe >&3"},
      0,
      {3},
      base::Bind(&ExpectedDataOnPipe, &subprocess_, &pid, 3, "on pipe\n", 0));
  EXPECT_NE(0, pid);

  // Wrong file descriptor values should return -1.
  EXPECT_EQ(-1, subprocess_.GetPipeFd(pid, 123));
  loop_.Run();
  // Calling GetPipeFd() after the callback runs is invalid.
  EXPECT_EQ(-1, subprocess_.GetPipeFd(pid, 3));
}

// Test that a pipe file descriptor open in the parent is not open in the child.
TEST_F(SubprocessTest, PipeClosedWhenNotRedirectedTest) {
  brillo::ScopedPipe pipe;

  // test_subprocess will return with the errno of fstat, which should be EBADF
  // if the passed file descriptor is closed in the child.
  const vector<string> cmd = {
      test_utils::GetBuildArtifactsPath("test_subprocess"),
      "fstat",
      std::to_string(pipe.writer)};
  EXPECT_TRUE(subprocess_.ExecFlags(
      cmd, 0, {}, base::Bind(&ExpectedResults, EBADF, "")));
  loop_.Run();
}

TEST_F(SubprocessTest, EnvVarsAreFiltered) {
  EXPECT_TRUE(
      subprocess_.Exec({kUsrBinPath "/env"}, base::Bind(&ExpectedEnvVars)));
  loop_.Run();
}

TEST_F(SubprocessTest, SynchronousTrueSearchsOnPath) {
  int rc = -1;
  EXPECT_TRUE(Subprocess::SynchronousExecFlags(
      {"true"}, Subprocess::kSearchPath, &rc, nullptr));
  EXPECT_EQ(0, rc);
}

TEST_F(SubprocessTest, SynchronousEchoTest) {
  vector<string> cmd = {
      kBinPath "/sh", "-c", "echo -n stdout-here; echo -n stderr-there >&2"};
  int rc = -1;
  string stdout;
  ASSERT_TRUE(Subprocess::SynchronousExec(cmd, &rc, &stdout));
  EXPECT_EQ(0, rc);
  EXPECT_EQ("stdout-herestderr-there", stdout);
}

TEST_F(SubprocessTest, SynchronousEchoNoOutputTest) {
  int rc = -1;
  ASSERT_TRUE(Subprocess::SynchronousExec(
      {kBinPath "/sh", "-c", "echo test"}, &rc, nullptr));
  EXPECT_EQ(0, rc);
}

namespace {
void CallbackBad(int return_code, const string& output) {
  ADD_FAILURE() << "should never be called.";
}
}  // namespace

// Test that you can cancel a program that's already running.
TEST_F(SubprocessTest, CancelTest) {
  base::ScopedTempDir tempdir;
  ASSERT_TRUE(tempdir.CreateUniqueTempDir());
  string fifo_path = tempdir.GetPath().Append("fifo").value();
  EXPECT_EQ(0, mkfifo(fifo_path.c_str(), 0666));

  // Start a process, make sure it is running and try to cancel it. We write
  // two bytes to the fifo, the first one marks that the program is running and
  // the second one marks that the process waited for a timeout and was not
  // killed. We should read the first byte but not the second one.
  vector<string> cmd = {
      kBinPath "/sh",
      "-c",
      base::StringPrintf(
          // The 'sleep' launched below could be left behind as an orphaned
          // process when the 'sh' process is terminated by SIGTERM. As a
          // remedy, trap SIGTERM and kill the 'sleep' process, which requires
          // launching 'sleep' in background and then waiting for it.
          "cleanup() { kill \"${sleep_pid}\"; exit 0; }; "
          "trap cleanup TERM; "
          "sleep 60 & "
          "sleep_pid=$!; "
          "printf X >\"%s\"; "
          "wait; "
          "printf Y >\"%s\"; "
          "exit 1",
          fifo_path.c_str(),
          fifo_path.c_str())};
  uint32_t tag = Subprocess::Get().Exec(cmd, base::Bind(&CallbackBad));
  EXPECT_NE(0U, tag);

  int fifo_fd = HANDLE_EINTR(open(fifo_path.c_str(), O_RDONLY));
  EXPECT_GE(fifo_fd, 0);

  loop_.WatchFileDescriptor(FROM_HERE,
                            fifo_fd,
                            MessageLoop::WatchMode::kWatchRead,
                            false,
                            base::Bind(
                                [](int fifo_fd, uint32_t tag) {
                                  char c;
                                  EXPECT_EQ(1,
                                            HANDLE_EINTR(read(fifo_fd, &c, 1)));
                                  EXPECT_EQ('X', c);
                                  LOG(INFO) << "Killing tag " << tag;
                                  Subprocess::Get().KillExec(tag);
                                },
                                fifo_fd,
                                tag));

  // This test would leak a callback that runs when the child process exits
  // unless we wait for it to run.
  brillo::MessageLoopRunUntil(
      &loop_, TimeDelta::FromSeconds(120), base::Bind([] {
        return Subprocess::Get().subprocess_records_.empty();
      }));
  EXPECT_TRUE(Subprocess::Get().subprocess_records_.empty());
  // Check that there isn't anything else to read from the pipe.
  char c;
  EXPECT_EQ(0, HANDLE_EINTR(read(fifo_fd, &c, 1)));
  IGNORE_EINTR(close(fifo_fd));
}

}  // namespace chromeos_update_engine
