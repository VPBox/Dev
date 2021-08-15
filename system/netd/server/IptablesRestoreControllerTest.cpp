/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define LOG_TAG "IptablesRestoreControllerTest"
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <log/log.h>
#include <netdutils/MockSyscalls.h>
#include <netdutils/Stopwatch.h>

#include "IptablesRestoreController.h"
#include "NetdConstants.h"
#include "bpf/BpfUtils.h"

#define XT_LOCK_NAME "/system/etc/xtables.lock"
#define XT_LOCK_ATTEMPTS 10
#define XT_LOCK_POLL_INTERVAL_MS 100

using android::base::Join;
using android::base::StringPrintf;
using android::netdutils::ScopedMockSyscalls;
using android::netdutils::Stopwatch;
using testing::Return;
using testing::StrictMock;

class IptablesRestoreControllerTest : public ::testing::Test {
public:
  IptablesRestoreController con;
  int mDefaultMaxRetries = con.MAX_RETRIES;
  int mDefaultPollTimeoutMs = con.POLL_TIMEOUT_MS;
  int mIptablesLock = -1;
  std::string mChainName;

  static void SetUpTestCase() {
      blockSigpipe();
  }

  void SetUp() {
    ASSERT_EQ(0, createTestChain());
  }

  void TearDown() {
    con.MAX_RETRIES = mDefaultMaxRetries;
    con.POLL_TIMEOUT_MS = mDefaultPollTimeoutMs;
    deleteTestChain();
  }

  void Init() {
    con.Init();
  }

  pid_t getIpRestorePid(const IptablesRestoreController::IptablesProcessType type) {
      return con.getIpRestorePid(type);
  };

  void expectNoIptablesRestoreProcess(pid_t pid) {
    // We can't readlink /proc/PID/exe, because zombie processes don't have it.
    // Parse /proc/PID/stat instead.
    std::string statPath = StringPrintf("/proc/%d/stat", pid);
    int fd = open(statPath.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd == -1) {
      // ENOENT means the process is gone (expected).
      ASSERT_EQ(errno, ENOENT)
        << "Unexpected error opening " << statPath << ": " << strerror(errno);
      return;
    }

    // If the PID exists, it's possible (though very unlikely) that the PID was reused. Check the
    // binary name as well, to ensure the test isn't flaky.
    char statBuf[1024];
    ASSERT_NE(-1, read(fd, statBuf, sizeof(statBuf)))
        << "Could not read from " << statPath << ": " << strerror(errno);
    close(fd);

    std::string statString(statBuf);
    EXPECT_FALSE(statString.find("iptables-restor") || statString.find("ip6tables-resto"))
      << "Previous iptables-restore pid " << pid << " still alive: " << statString;
  }

  int createTestChain() {
    mChainName = StringPrintf("netd_unit_test_%u", arc4random_uniform(10000)).c_str();

    // Create a chain to list.
    std::vector<std::string> createCommands = {
        "*filter",
        StringPrintf(":%s -", mChainName.c_str()),
        StringPrintf("-A %s -j RETURN", mChainName.c_str()),
        "COMMIT",
        ""
    };

    int ret = con.execute(V4V6, Join(createCommands, "\n"), nullptr);
    if (ret) mChainName = "";
    return ret;
  }

  void deleteTestChain() {
    std::vector<std::string> deleteCommands = {
        "*filter",
        StringPrintf(":%s -", mChainName.c_str()),  // Flush chain (otherwise we can't delete it).
        StringPrintf("-X %s", mChainName.c_str()),  // Delete it.
        "COMMIT",
        ""
    };
    con.execute(V4V6, Join(deleteCommands, "\n"), nullptr);
    mChainName = "";
  }

  int acquireIptablesLock() {
    mIptablesLock = open(XT_LOCK_NAME, O_CREAT | O_CLOEXEC, 0600);
    if (mIptablesLock == -1) return mIptablesLock;
    int attempts;
    for (attempts = 0; attempts < XT_LOCK_ATTEMPTS; attempts++) {
      if (flock(mIptablesLock, LOCK_EX | LOCK_NB) == 0) {
        return 0;
      }
      usleep(XT_LOCK_POLL_INTERVAL_MS * 1000);
    }
    EXPECT_LT(attempts, XT_LOCK_ATTEMPTS) <<
        "Could not acquire iptables lock after " << XT_LOCK_ATTEMPTS << " attempts " <<
        XT_LOCK_POLL_INTERVAL_MS << "ms apart";
    return -1;
  }

  void releaseIptablesLock() {
    if (mIptablesLock != -1) {
      close(mIptablesLock);
    }
  }

  void setRetryParameters(int maxRetries, int pollTimeoutMs) {
    con.MAX_RETRIES = maxRetries;
    con.POLL_TIMEOUT_MS = pollTimeoutMs;
  }
};

TEST_F(IptablesRestoreControllerTest, TestBasicCommand) {
  std::string output;

  EXPECT_EQ(0, con.execute(IptablesTarget::V4V6, "#Test\n", nullptr));

  pid_t pid4 = getIpRestorePid(IptablesRestoreController::IPTABLES_PROCESS);
  pid_t pid6 = getIpRestorePid(IptablesRestoreController::IP6TABLES_PROCESS);

  EXPECT_EQ(0, con.execute(IptablesTarget::V6, "#Test\n", nullptr));
  EXPECT_EQ(0, con.execute(IptablesTarget::V4, "#Test\n", nullptr));

  EXPECT_EQ(0, con.execute(IptablesTarget::V4V6, "#Test\n", &output));
  EXPECT_EQ("#Test\n#Test\n", output);  // One for IPv4 and one for IPv6.

  // Check the PIDs are the same as they were before. If they're not, the child processes were
  // restarted, which causes a 30-60ms delay.
  EXPECT_EQ(pid4, getIpRestorePid(IptablesRestoreController::IPTABLES_PROCESS));
  EXPECT_EQ(pid6, getIpRestorePid(IptablesRestoreController::IP6TABLES_PROCESS));
}

TEST_F(IptablesRestoreControllerTest, TestRestartOnMalformedCommand) {
  std::string buffer;
  for (int i = 0; i < 50; i++) {
      IptablesTarget target = (IptablesTarget) (i % 3);
      std::string *output = (i % 2) ? &buffer : nullptr;
      ASSERT_EQ(-1, con.execute(target, "malformed command\n", output)) <<
          "Malformed command did not fail at iteration " << i;
      ASSERT_EQ(0, con.execute(target, "#Test\n", output)) <<
          "No-op command did not succeed at iteration " << i;
  }
}

TEST_F(IptablesRestoreControllerTest, TestRestartOnProcessDeath) {
  std::string output;

  // Run a command to ensure that the processes are running.
  EXPECT_EQ(0, con.execute(IptablesTarget::V4V6, "#Test\n", &output));

  pid_t pid4 = getIpRestorePid(IptablesRestoreController::IPTABLES_PROCESS);
  pid_t pid6 = getIpRestorePid(IptablesRestoreController::IP6TABLES_PROCESS);

  ASSERT_EQ(0, kill(pid4, 0)) << "iptables-restore pid " << pid4 << " does not exist";
  ASSERT_EQ(0, kill(pid6, 0)) << "ip6tables-restore pid " << pid6 << " does not exist";
  ASSERT_EQ(0, kill(pid4, SIGTERM)) << "Failed to send SIGTERM to iptables-restore pid " << pid4;
  ASSERT_EQ(0, kill(pid6, SIGTERM)) << "Failed to send SIGTERM to ip6tables-restore pid " << pid6;

  // Wait 100ms for processes to terminate.
  TEMP_FAILURE_RETRY(usleep(100 * 1000));

  // Ensure that running a new command properly restarts the processes.
  EXPECT_EQ(0, con.execute(IptablesTarget::V4V6, "#Test\n", nullptr));
  EXPECT_NE(pid4, getIpRestorePid(IptablesRestoreController::IPTABLES_PROCESS));
  EXPECT_NE(pid6, getIpRestorePid(IptablesRestoreController::IP6TABLES_PROCESS));

  // Check there are no zombies.
  expectNoIptablesRestoreProcess(pid4);
  expectNoIptablesRestoreProcess(pid6);
}

TEST_F(IptablesRestoreControllerTest, TestCommandTimeout) {
  // Don't wait 10 seconds for this test to fail.
  setRetryParameters(3, 50);

  // Expected contents of the chain.
  std::vector<std::string> expectedLines = {
      StringPrintf("Chain %s (0 references)", mChainName.c_str()),
      "target     prot opt source               destination         ",
      "RETURN     all  --  0.0.0.0/0            0.0.0.0/0           ",
      StringPrintf("Chain %s (0 references)", mChainName.c_str()),
      "target     prot opt source               destination         ",
      "RETURN     all      ::/0                 ::/0                ",
      ""
  };
  std::string expected = Join(expectedLines, "\n");

  std::vector<std::string> listCommands = {
      "*filter",
      StringPrintf("-n -L %s", mChainName.c_str()),  // List chain.
      "COMMIT",
      ""
  };
  std::string commandString = Join(listCommands, "\n");
  std::string output;

  EXPECT_EQ(0, con.execute(IptablesTarget::V4V6, commandString, &output));
  EXPECT_EQ(expected, output);

  ASSERT_EQ(0, acquireIptablesLock());
  EXPECT_EQ(-1, con.execute(IptablesTarget::V4V6, commandString, &output));
  EXPECT_EQ(-1, con.execute(IptablesTarget::V4V6, commandString, &output));
  releaseIptablesLock();

  EXPECT_EQ(0, con.execute(IptablesTarget::V4V6, commandString, &output));
  EXPECT_EQ(expected, output);
}


TEST_F(IptablesRestoreControllerTest, TestUidRuleBenchmark) {
    const std::vector<int> ITERATIONS = { 1, 5, 10 };

    const std::string IPTABLES_RESTORE_ADD =
            StringPrintf("*filter\n-I %s -m owner --uid-owner 2000000000 -j RETURN\nCOMMIT\n",
                         mChainName.c_str());
    const std::string IPTABLES_RESTORE_DEL =
            StringPrintf("*filter\n-D %s -m owner --uid-owner 2000000000 -j RETURN\nCOMMIT\n",
                         mChainName.c_str());

    for (const int iterations : ITERATIONS) {
        Stopwatch s;
        for (int i = 0; i < iterations; i++) {
            EXPECT_EQ(0, con.execute(V4V6, IPTABLES_RESTORE_ADD, nullptr));
            EXPECT_EQ(0, con.execute(V4V6, IPTABLES_RESTORE_DEL, nullptr));
        }
        float timeTaken = s.getTimeAndReset();
        fprintf(stderr, "    Add/del %d UID rules via restore: %.1fms (%.2fms per operation)\n",
                iterations, timeTaken, timeTaken / 2 / iterations);
    }
}

TEST_F(IptablesRestoreControllerTest, TestStartup) {
  // Tests that IptablesRestoreController::Init never sets its processes to null pointers if
  // fork() succeeds.
  {
    // Mock fork(), and check that initializing 100 times never results in a null pointer.
    constexpr int NUM_ITERATIONS = 100;  // Takes 100-150ms on angler.
    constexpr pid_t FAKE_PID = 2000000001;
    StrictMock<ScopedMockSyscalls> sys;

    EXPECT_CALL(sys, fork()).Times(NUM_ITERATIONS * 2).WillRepeatedly(Return(FAKE_PID));
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      Init();
      EXPECT_NE(0, getIpRestorePid(IptablesRestoreController::IPTABLES_PROCESS));
      EXPECT_NE(0, getIpRestorePid(IptablesRestoreController::IP6TABLES_PROCESS));
    }
  }

  // The controller is now in an invalid state: the pipes are connected to working iptables
  // processes, but the PIDs are set to FAKE_PID. Send a malformed command to ensure that the
  // processes terminate and close the pipes, then send a valid command to have the controller
  // re-initialize properly now that fork() is no longer mocked.
  EXPECT_EQ(-1, con.execute(V4V6, "malformed command\n", nullptr));
  EXPECT_EQ(0, con.execute(V4V6, "#Test\n", nullptr));
}
