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

#include <cerrno>
#include <memory>
#include <tuple>
#include <utility>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "wifilogd/tests/mock_command_processor.h"
#include "wifilogd/tests/mock_os.h"

#include "wifilogd/main_loop.h"
#include "wifilogd/protocol.h"

namespace android {
namespace wifilogd {
namespace {

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Ge;
using ::testing::Return;
using ::testing::StrictMock;

constexpr int kControlSocketFd = 100;
constexpr char kFakeSocketName[] = "fake-socket";

class MainLoopTest : public ::testing::Test {
 public:
  MainLoopTest()
      : os_(new StrictMock<MockOs>()),
        command_processor_(new StrictMock<MockCommandProcessor>()) {
    EXPECT_CALL(*os_, GetControlSocket(kFakeSocketName))
        .WillOnce(Return(std::tuple<size_t, Os::Errno>{kControlSocketFd, 0}));
    main_loop_ = std::make_unique<MainLoop>(
        kFakeSocketName, std::unique_ptr<Os>{os_},
        std::unique_ptr<CommandProcessor>{command_processor_});
  }

 protected:
  std::unique_ptr<MainLoop> main_loop_;
  // We use raw pointers to access the mocks, since ownership passes
  // to |main_loop_|.
  StrictMock<MockOs>* os_;
  StrictMock<MockCommandProcessor>* command_processor_;
};

}  // namespace

TEST_F(MainLoopTest, RunOnceReadsFromCorrectSocket) {
  EXPECT_CALL(*os_, ReceiveDatagram(kControlSocketFd, _, _));
  EXPECT_CALL(*command_processor_, ProcessCommand(_, _, _)).Times(AnyNumber());
  main_loop_->RunOnce();
}

TEST_F(MainLoopTest, RunOnceReadsWithSufficientlyLargeBuffer) {
  EXPECT_CALL(*os_, ReceiveDatagram(_, _, Ge(protocol::kMaxMessageSize)));
  EXPECT_CALL(*command_processor_, ProcessCommand(_, _, _)).Times(AnyNumber());
  main_loop_->RunOnce();
}

TEST_F(MainLoopTest, RunOncePassesSmallestValidMessageToCommandProcessor) {
  EXPECT_CALL(*os_, ReceiveDatagram(_, _, _))
      .WillOnce(
          Return(std::tuple<size_t, Os::Errno>{sizeof(protocol::Command), 0}));
  EXPECT_CALL(*command_processor_,
              ProcessCommand(_, sizeof(protocol::Command), _));
  main_loop_->RunOnce();
}

TEST_F(MainLoopTest, RunOncePassesLargestValidMessageToCommandProcessor) {
  EXPECT_CALL(*os_, ReceiveDatagram(_, _, _))
      .WillOnce(
          Return(std::tuple<size_t, Os::Errno>{protocol::kMaxMessageSize, 0}));
  EXPECT_CALL(*command_processor_,
              ProcessCommand(_, protocol::kMaxMessageSize, _));
  main_loop_->RunOnce();
}

TEST_F(MainLoopTest, RunOncePassesRuntMessageToCommandProcessor) {
  EXPECT_CALL(*os_, ReceiveDatagram(_, _, _))
      .WillOnce(Return(std::tuple<size_t, Os::Errno>{0, 0}));
  EXPECT_CALL(*command_processor_, ProcessCommand(_, 0, _));
  main_loop_->RunOnce();
}

TEST_F(MainLoopTest, RunOnceLimitsMaxSizeReportedToCommandProcessor) {
  EXPECT_CALL(*os_, ReceiveDatagram(_, _, _))
      .WillOnce(Return(
          std::tuple<size_t, Os::Errno>{protocol::kMaxMessageSize + 1, 0}));
  EXPECT_CALL(*command_processor_,
              ProcessCommand(_, protocol::kMaxMessageSize, _));
  main_loop_->RunOnce();
}

TEST_F(MainLoopTest, RunOnceSleepsAndDoesNotPassDataToCommandProcessorOnError) {
  EXPECT_CALL(*os_, ReceiveDatagram(_, _, protocol::kMaxMessageSize))
      .WillOnce(Return(std::tuple<size_t, Os::Errno>{0, EINTR}));
  EXPECT_CALL(*os_, Nanosleep(_));
  EXPECT_CALL(*command_processor_, ProcessCommand(_, _, _)).Times(0);
  main_loop_->RunOnce();
}

// Per
// github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests,
// death tests should be specially named.
using MainLoopDeathTest = MainLoopTest;

TEST_F(MainLoopDeathTest, CtorFailureToFetchControlSocketCausesDeath) {
  auto os = std::make_unique<StrictMock<MockOs>>();
  auto command_processor = std::make_unique<StrictMock<MockCommandProcessor>>();
  ON_CALL(*os, GetControlSocket(kFakeSocketName))
      .WillByDefault(Return(std::tuple<size_t, Os::Errno>{-1, ERANGE}));
  EXPECT_DEATH(
      MainLoop(kFakeSocketName, std::move(os), std::move(command_processor)),
      "Failed to get control socket");
}

TEST_F(MainLoopDeathTest, RunOnceTerminatesOnUnexpectedError) {
  ON_CALL(*os_, ReceiveDatagram(_, _, protocol::kMaxMessageSize))
      .WillByDefault(Return(std::tuple<size_t, Os::Errno>{0, EFAULT}));
  EXPECT_DEATH(main_loop_->RunOnce(), "Unexpected error");
}

}  // namespace wifilogd
}  // namespace android
