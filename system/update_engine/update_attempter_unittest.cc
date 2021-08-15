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

#include "update_engine/update_attempter.h"

#include <stdint.h>

#include <memory>

#include <base/files/file_util.h>
#include <base/message_loop/message_loop.h>
#include <brillo/message_loops/base_message_loop.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <gtest/gtest.h>
#include <policy/libpolicy.h>
#include <policy/mock_device_policy.h>
#include <policy/mock_libpolicy.h>

#include "update_engine/common/dlcservice_interface.h"
#include "update_engine/common/fake_clock.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/common/mock_action.h"
#include "update_engine/common/mock_action_processor.h"
#include "update_engine/common/mock_http_fetcher.h"
#include "update_engine/common/mock_prefs.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/prefs.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/fake_system_state.h"
#include "update_engine/mock_p2p_manager.h"
#include "update_engine/mock_payload_state.h"
#include "update_engine/mock_service_observer.h"
#include "update_engine/payload_consumer/filesystem_verifier_action.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_consumer/postinstall_runner_action.h"
#include "update_engine/update_boot_flags_action.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_manager::EvalStatus;
using chromeos_update_manager::StagingSchedule;
using chromeos_update_manager::UpdateCheckParams;
using policy::DevicePolicy;
using std::string;
using std::unique_ptr;
using std::vector;
using testing::_;
using testing::DoAll;
using testing::Field;
using testing::InSequence;
using testing::Ne;
using testing::NiceMock;
using testing::Pointee;
using testing::Property;
using testing::Return;
using testing::ReturnPointee;
using testing::ReturnRef;
using testing::SaveArg;
using testing::SetArgPointee;
using update_engine::UpdateAttemptFlags;
using update_engine::UpdateEngineStatus;
using update_engine::UpdateStatus;

namespace chromeos_update_engine {

namespace {

class MockDlcService : public DlcServiceInterface {
 public:
  MOCK_METHOD1(GetInstalled, bool(vector<string>*));
};

}  // namespace

const char kRollbackVersion[] = "10575.39.2";

// Test a subclass rather than the main class directly so that we can mock out
// methods within the class. There're explicit unit tests for the mocked out
// methods.
class UpdateAttempterUnderTest : public UpdateAttempter {
 public:
  explicit UpdateAttempterUnderTest(SystemState* system_state)
      : UpdateAttempter(system_state, nullptr) {}

  // Wrap the update scheduling method, allowing us to opt out of scheduled
  // updates for testing purposes.
  bool ScheduleUpdates() override {
    schedule_updates_called_ = true;
    if (do_schedule_updates_) {
      UpdateAttempter::ScheduleUpdates();
    } else {
      LOG(INFO) << "[TEST] Update scheduling disabled.";
    }
    return true;
  }
  void EnableScheduleUpdates() { do_schedule_updates_ = true; }
  void DisableScheduleUpdates() { do_schedule_updates_ = false; }

  // Indicates whether ScheduleUpdates() was called.
  bool schedule_updates_called() const { return schedule_updates_called_; }

  // Need to expose forced_omaha_url_ so we can test it.
  const string& forced_omaha_url() const { return forced_omaha_url_; }

 private:
  bool schedule_updates_called_ = false;
  bool do_schedule_updates_ = true;
};

class UpdateAttempterTest : public ::testing::Test {
 protected:
  UpdateAttempterTest()
      : certificate_checker_(fake_system_state_.mock_prefs(),
                             &openssl_wrapper_) {
    // Override system state members.
    fake_system_state_.set_connection_manager(&mock_connection_manager);
    fake_system_state_.set_update_attempter(&attempter_);
    fake_system_state_.set_dlcservice(&mock_dlcservice_);
    loop_.SetAsCurrent();

    certificate_checker_.Init();

    attempter_.set_forced_update_pending_callback(
        new base::Callback<void(bool, bool)>(base::Bind([](bool, bool) {})));
    // Finish initializing the attempter.
    attempter_.Init();
  }

  void SetUp() override {
    EXPECT_NE(nullptr, attempter_.system_state_);
    EXPECT_EQ(0, attempter_.http_response_code_);
    EXPECT_EQ(UpdateStatus::IDLE, attempter_.status_);
    EXPECT_EQ(0.0, attempter_.download_progress_);
    EXPECT_EQ(0, attempter_.last_checked_time_);
    EXPECT_EQ("0.0.0.0", attempter_.new_version_);
    EXPECT_EQ(0ULL, attempter_.new_payload_size_);
    processor_ = new NiceMock<MockActionProcessor>();
    attempter_.processor_.reset(processor_);  // Transfers ownership.
    prefs_ = fake_system_state_.mock_prefs();

    // Set up store/load semantics of P2P properties via the mock PayloadState.
    actual_using_p2p_for_downloading_ = false;
    EXPECT_CALL(*fake_system_state_.mock_payload_state(),
                SetUsingP2PForDownloading(_))
        .WillRepeatedly(SaveArg<0>(&actual_using_p2p_for_downloading_));
    EXPECT_CALL(*fake_system_state_.mock_payload_state(),
                GetUsingP2PForDownloading())
        .WillRepeatedly(ReturnPointee(&actual_using_p2p_for_downloading_));
    actual_using_p2p_for_sharing_ = false;
    EXPECT_CALL(*fake_system_state_.mock_payload_state(),
                SetUsingP2PForSharing(_))
        .WillRepeatedly(SaveArg<0>(&actual_using_p2p_for_sharing_));
    EXPECT_CALL(*fake_system_state_.mock_payload_state(),
                GetUsingP2PForDownloading())
        .WillRepeatedly(ReturnPointee(&actual_using_p2p_for_sharing_));
  }

 public:
  void ScheduleQuitMainLoop();

  // Callbacks to run the different tests from the main loop.
  void UpdateTestStart();
  void UpdateTestVerify();
  void RollbackTestStart(bool enterprise_rollback, bool valid_slot);
  void RollbackTestVerify();
  void PingOmahaTestStart();
  void ReadScatterFactorFromPolicyTestStart();
  void DecrementUpdateCheckCountTestStart();
  void NoScatteringDoneDuringManualUpdateTestStart();
  void P2PNotEnabledStart();
  void P2PEnabledStart();
  void P2PEnabledInteractiveStart();
  void P2PEnabledStartingFailsStart();
  void P2PEnabledHousekeepingFailsStart();
  void ResetRollbackHappenedStart(bool is_consumer,
                                  bool is_policy_available,
                                  bool expected_reset);
  // Staging related callbacks.
  void SetUpStagingTest(const StagingSchedule& schedule, FakePrefs* prefs);
  void CheckStagingOff();
  void StagingSetsPrefsAndTurnsOffScatteringStart();
  void StagingOffIfInteractiveStart();
  void StagingOffIfOobeStart();

  bool actual_using_p2p_for_downloading() {
    return actual_using_p2p_for_downloading_;
  }
  bool actual_using_p2p_for_sharing() { return actual_using_p2p_for_sharing_; }

  base::MessageLoopForIO base_loop_;
  brillo::BaseMessageLoop loop_{&base_loop_};

  FakeSystemState fake_system_state_;
  UpdateAttempterUnderTest attempter_{&fake_system_state_};
  OpenSSLWrapper openssl_wrapper_;
  CertificateChecker certificate_checker_;
  MockDlcService mock_dlcservice_;

  NiceMock<MockActionProcessor>* processor_;
  NiceMock<MockPrefs>* prefs_;  // Shortcut to fake_system_state_->mock_prefs().
  NiceMock<MockConnectionManager> mock_connection_manager;

  bool actual_using_p2p_for_downloading_;
  bool actual_using_p2p_for_sharing_;
};

void UpdateAttempterTest::ScheduleQuitMainLoop() {
  loop_.PostTask(
      FROM_HERE,
      base::Bind([](brillo::BaseMessageLoop* loop) { loop->BreakLoop(); },
                 base::Unretained(&loop_)));
}

TEST_F(UpdateAttempterTest, ActionCompletedDownloadTest) {
  unique_ptr<MockHttpFetcher> fetcher(new MockHttpFetcher("", 0, nullptr));
  fetcher->FailTransfer(503);  // Sets the HTTP response code.
  DownloadAction action(prefs_,
                        nullptr,
                        nullptr,
                        nullptr,
                        fetcher.release(),
                        false /* interactive */);
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _)).Times(0);
  attempter_.ActionCompleted(nullptr, &action, ErrorCode::kSuccess);
  EXPECT_EQ(UpdateStatus::FINALIZING, attempter_.status());
  EXPECT_EQ(0.0, attempter_.download_progress_);
  ASSERT_EQ(nullptr, attempter_.error_event_.get());
}

TEST_F(UpdateAttempterTest, ActionCompletedErrorTest) {
  MockAction action;
  EXPECT_CALL(action, Type()).WillRepeatedly(Return("MockAction"));
  attempter_.status_ = UpdateStatus::DOWNLOADING;
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _))
      .WillOnce(Return(false));
  attempter_.ActionCompleted(nullptr, &action, ErrorCode::kError);
  ASSERT_NE(nullptr, attempter_.error_event_.get());
}

TEST_F(UpdateAttempterTest, DownloadProgressAccumulationTest) {
  // Simple test case, where all the values match (nothing was skipped)
  uint64_t bytes_progressed_1 = 1024 * 1024;  // 1MB
  uint64_t bytes_progressed_2 = 1024 * 1024;  // 1MB
  uint64_t bytes_received_1 = bytes_progressed_1;
  uint64_t bytes_received_2 = bytes_received_1 + bytes_progressed_2;
  uint64_t bytes_total = 20 * 1024 * 1024;  // 20MB

  double progress_1 =
      static_cast<double>(bytes_received_1) / static_cast<double>(bytes_total);
  double progress_2 =
      static_cast<double>(bytes_received_2) / static_cast<double>(bytes_total);

  EXPECT_EQ(0.0, attempter_.download_progress_);
  // This is set via inspecting the InstallPlan payloads when the
  // OmahaResponseAction is completed
  attempter_.new_payload_size_ = bytes_total;
  NiceMock<MockServiceObserver> observer;
  EXPECT_CALL(observer,
              SendStatusUpdate(AllOf(
                  Field(&UpdateEngineStatus::progress, progress_1),
                  Field(&UpdateEngineStatus::status, UpdateStatus::DOWNLOADING),
                  Field(&UpdateEngineStatus::new_size_bytes, bytes_total))));
  EXPECT_CALL(observer,
              SendStatusUpdate(AllOf(
                  Field(&UpdateEngineStatus::progress, progress_2),
                  Field(&UpdateEngineStatus::status, UpdateStatus::DOWNLOADING),
                  Field(&UpdateEngineStatus::new_size_bytes, bytes_total))));
  attempter_.AddObserver(&observer);
  attempter_.BytesReceived(bytes_progressed_1, bytes_received_1, bytes_total);
  EXPECT_EQ(progress_1, attempter_.download_progress_);
  // This iteration validates that a later set of updates to the variables are
  // properly handled (so that |getStatus()| will return the same progress info
  // as the callback is receiving.
  attempter_.BytesReceived(bytes_progressed_2, bytes_received_2, bytes_total);
  EXPECT_EQ(progress_2, attempter_.download_progress_);
}

TEST_F(UpdateAttempterTest, ChangeToDownloadingOnReceivedBytesTest) {
  // The transition into UpdateStatus::DOWNLOADING happens when the
  // first bytes are received.
  uint64_t bytes_progressed = 1024 * 1024;    // 1MB
  uint64_t bytes_received = 2 * 1024 * 1024;  // 2MB
  uint64_t bytes_total = 20 * 1024 * 1024;    // 300MB
  attempter_.status_ = UpdateStatus::CHECKING_FOR_UPDATE;
  // This is set via inspecting the InstallPlan payloads when the
  // OmahaResponseAction is completed
  attempter_.new_payload_size_ = bytes_total;
  EXPECT_EQ(0.0, attempter_.download_progress_);
  NiceMock<MockServiceObserver> observer;
  EXPECT_CALL(observer,
              SendStatusUpdate(AllOf(
                  Field(&UpdateEngineStatus::status, UpdateStatus::DOWNLOADING),
                  Field(&UpdateEngineStatus::new_size_bytes, bytes_total))));
  attempter_.AddObserver(&observer);
  attempter_.BytesReceived(bytes_progressed, bytes_received, bytes_total);
  EXPECT_EQ(UpdateStatus::DOWNLOADING, attempter_.status_);
}

TEST_F(UpdateAttempterTest, BroadcastCompleteDownloadTest) {
  // There is a special case to ensure that at 100% downloaded,
  // download_progress_ is updated and that value broadcast. This test confirms
  // that.
  uint64_t bytes_progressed = 0;              // ignored
  uint64_t bytes_received = 5 * 1024 * 1024;  // ignored
  uint64_t bytes_total = 5 * 1024 * 1024;     // 300MB
  attempter_.status_ = UpdateStatus::DOWNLOADING;
  attempter_.new_payload_size_ = bytes_total;
  EXPECT_EQ(0.0, attempter_.download_progress_);
  NiceMock<MockServiceObserver> observer;
  EXPECT_CALL(observer,
              SendStatusUpdate(AllOf(
                  Field(&UpdateEngineStatus::progress, 1.0),
                  Field(&UpdateEngineStatus::status, UpdateStatus::DOWNLOADING),
                  Field(&UpdateEngineStatus::new_size_bytes, bytes_total))));
  attempter_.AddObserver(&observer);
  attempter_.BytesReceived(bytes_progressed, bytes_received, bytes_total);
  EXPECT_EQ(1.0, attempter_.download_progress_);
}

TEST_F(UpdateAttempterTest, ActionCompletedOmahaRequestTest) {
  unique_ptr<MockHttpFetcher> fetcher(new MockHttpFetcher("", 0, nullptr));
  fetcher->FailTransfer(500);  // Sets the HTTP response code.
  OmahaRequestAction action(
      &fake_system_state_, nullptr, std::move(fetcher), false);
  ObjectCollectorAction<OmahaResponse> collector_action;
  BondActions(&action, &collector_action);
  OmahaResponse response;
  response.poll_interval = 234;
  action.SetOutputObject(response);
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _)).Times(0);
  attempter_.ActionCompleted(nullptr, &action, ErrorCode::kSuccess);
  EXPECT_EQ(500, attempter_.http_response_code());
  EXPECT_EQ(UpdateStatus::IDLE, attempter_.status());
  EXPECT_EQ(234U, attempter_.server_dictated_poll_interval_);
  ASSERT_TRUE(attempter_.error_event_.get() == nullptr);
}

TEST_F(UpdateAttempterTest, ConstructWithUpdatedMarkerTest) {
  FakePrefs fake_prefs;
  string boot_id;
  EXPECT_TRUE(utils::GetBootId(&boot_id));
  fake_prefs.SetString(kPrefsUpdateCompletedOnBootId, boot_id);
  fake_system_state_.set_prefs(&fake_prefs);
  attempter_.Init();
  EXPECT_EQ(UpdateStatus::UPDATED_NEED_REBOOT, attempter_.status());
}

TEST_F(UpdateAttempterTest, GetErrorCodeForActionTest) {
  EXPECT_EQ(ErrorCode::kSuccess,
            GetErrorCodeForAction(nullptr, ErrorCode::kSuccess));

  FakeSystemState fake_system_state;
  OmahaRequestAction omaha_request_action(
      &fake_system_state, nullptr, nullptr, false);
  EXPECT_EQ(ErrorCode::kOmahaRequestError,
            GetErrorCodeForAction(&omaha_request_action, ErrorCode::kError));
  OmahaResponseHandlerAction omaha_response_handler_action(&fake_system_state_);
  EXPECT_EQ(
      ErrorCode::kOmahaResponseHandlerError,
      GetErrorCodeForAction(&omaha_response_handler_action, ErrorCode::kError));
  FilesystemVerifierAction filesystem_verifier_action;
  EXPECT_EQ(
      ErrorCode::kFilesystemVerifierError,
      GetErrorCodeForAction(&filesystem_verifier_action, ErrorCode::kError));
  PostinstallRunnerAction postinstall_runner_action(
      fake_system_state.fake_boot_control(), fake_system_state.fake_hardware());
  EXPECT_EQ(
      ErrorCode::kPostinstallRunnerError,
      GetErrorCodeForAction(&postinstall_runner_action, ErrorCode::kError));
  MockAction action_mock;
  EXPECT_CALL(action_mock, Type()).WillOnce(Return("MockAction"));
  EXPECT_EQ(ErrorCode::kError,
            GetErrorCodeForAction(&action_mock, ErrorCode::kError));
}

TEST_F(UpdateAttempterTest, DisableDeltaUpdateIfNeededTest) {
  attempter_.omaha_request_params_->set_delta_okay(true);
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _))
      .WillOnce(Return(false));
  attempter_.DisableDeltaUpdateIfNeeded();
  EXPECT_TRUE(attempter_.omaha_request_params_->delta_okay());
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(UpdateAttempter::kMaxDeltaUpdateFailures - 1),
                Return(true)));
  attempter_.DisableDeltaUpdateIfNeeded();
  EXPECT_TRUE(attempter_.omaha_request_params_->delta_okay());
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(UpdateAttempter::kMaxDeltaUpdateFailures),
                Return(true)));
  attempter_.DisableDeltaUpdateIfNeeded();
  EXPECT_FALSE(attempter_.omaha_request_params_->delta_okay());
  EXPECT_CALL(*prefs_, GetInt64(_, _)).Times(0);
  attempter_.DisableDeltaUpdateIfNeeded();
  EXPECT_FALSE(attempter_.omaha_request_params_->delta_okay());
}

TEST_F(UpdateAttempterTest, MarkDeltaUpdateFailureTest) {
  EXPECT_CALL(*prefs_, GetInt64(kPrefsDeltaUpdateFailures, _))
      .WillOnce(Return(false))
      .WillOnce(DoAll(SetArgPointee<1>(-1), Return(true)))
      .WillOnce(DoAll(SetArgPointee<1>(1), Return(true)))
      .WillOnce(
          DoAll(SetArgPointee<1>(UpdateAttempter::kMaxDeltaUpdateFailures),
                Return(true)));
  EXPECT_CALL(*prefs_, SetInt64(Ne(kPrefsDeltaUpdateFailures), _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*prefs_, SetInt64(kPrefsDeltaUpdateFailures, 1)).Times(2);
  EXPECT_CALL(*prefs_, SetInt64(kPrefsDeltaUpdateFailures, 2));
  EXPECT_CALL(*prefs_,
              SetInt64(kPrefsDeltaUpdateFailures,
                       UpdateAttempter::kMaxDeltaUpdateFailures + 1));
  for (int i = 0; i < 4; i++)
    attempter_.MarkDeltaUpdateFailure();
}

TEST_F(UpdateAttempterTest, ScheduleErrorEventActionNoEventTest) {
  EXPECT_CALL(*processor_, EnqueueAction(_)).Times(0);
  EXPECT_CALL(*processor_, StartProcessing()).Times(0);
  EXPECT_CALL(*fake_system_state_.mock_payload_state(), UpdateFailed(_))
      .Times(0);
  OmahaResponse response;
  string url1 = "http://url1";
  response.packages.push_back({.payload_urls = {url1, "https://url"}});
  EXPECT_CALL(*(fake_system_state_.mock_payload_state()), GetCurrentUrl())
      .WillRepeatedly(Return(url1));
  fake_system_state_.mock_payload_state()->SetResponse(response);
  attempter_.ScheduleErrorEventAction();
  EXPECT_EQ(url1, fake_system_state_.mock_payload_state()->GetCurrentUrl());
}

TEST_F(UpdateAttempterTest, ScheduleErrorEventActionTest) {
  EXPECT_CALL(*processor_,
              EnqueueAction(Pointee(Property(
                  &AbstractAction::Type, OmahaRequestAction::StaticType()))));
  EXPECT_CALL(*processor_, StartProcessing());
  ErrorCode err = ErrorCode::kError;
  EXPECT_CALL(*fake_system_state_.mock_payload_state(), UpdateFailed(err));
  attempter_.error_event_.reset(new OmahaEvent(
      OmahaEvent::kTypeUpdateComplete, OmahaEvent::kResultError, err));
  attempter_.ScheduleErrorEventAction();
  EXPECT_EQ(UpdateStatus::REPORTING_ERROR_EVENT, attempter_.status());
}

namespace {
// Actions that will be built as part of an update check.
const string kUpdateActionTypes[] = {  // NOLINT(runtime/string)
    OmahaRequestAction::StaticType(),
    OmahaResponseHandlerAction::StaticType(),
    UpdateBootFlagsAction::StaticType(),
    OmahaRequestAction::StaticType(),
    DownloadAction::StaticType(),
    OmahaRequestAction::StaticType(),
    FilesystemVerifierAction::StaticType(),
    PostinstallRunnerAction::StaticType(),
    OmahaRequestAction::StaticType()};

// Actions that will be built as part of a user-initiated rollback.
const string kRollbackActionTypes[] = {
    // NOLINT(runtime/string)
    InstallPlanAction::StaticType(),
    PostinstallRunnerAction::StaticType(),
};

const StagingSchedule kValidStagingSchedule = {
    {4, 10}, {10, 40}, {19, 70}, {26, 100}};

}  // namespace

void UpdateAttempterTest::UpdateTestStart() {
  attempter_.set_http_response_code(200);

  // Expect that the device policy is loaded by the UpdateAttempter at some
  // point by calling RefreshDevicePolicy.
  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  EXPECT_CALL(*device_policy, LoadPolicy())
      .Times(testing::AtLeast(1))
      .WillRepeatedly(Return(true));
  attempter_.policy_provider_.reset(
      new policy::PolicyProvider(std::move(device_policy)));

  {
    InSequence s;
    for (size_t i = 0; i < arraysize(kUpdateActionTypes); ++i) {
      EXPECT_CALL(*processor_,
                  EnqueueAction(Pointee(
                      Property(&AbstractAction::Type, kUpdateActionTypes[i]))));
    }
    EXPECT_CALL(*processor_, StartProcessing());
  }

  attempter_.Update("", "", "", "", false, false, false);
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::UpdateTestVerify,
                            base::Unretained(this)));
}

void UpdateAttempterTest::UpdateTestVerify() {
  EXPECT_EQ(0, attempter_.http_response_code());
  EXPECT_EQ(&attempter_, processor_->delegate());
  EXPECT_EQ(UpdateStatus::CHECKING_FOR_UPDATE, attempter_.status());
  loop_.BreakLoop();
}

void UpdateAttempterTest::RollbackTestStart(bool enterprise_rollback,
                                            bool valid_slot) {
  // Create a device policy so that we can change settings.
  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  EXPECT_CALL(*device_policy, LoadPolicy()).WillRepeatedly(Return(true));
  fake_system_state_.set_device_policy(device_policy.get());
  if (enterprise_rollback) {
    // We return an empty owner as this is an enterprise.
    EXPECT_CALL(*device_policy, GetOwner(_))
        .WillRepeatedly(DoAll(SetArgPointee<0>(string("")), Return(true)));
  } else {
    // We return a fake owner as this is an owned consumer device.
    EXPECT_CALL(*device_policy, GetOwner(_))
        .WillRepeatedly(DoAll(SetArgPointee<0>(string("fake.mail@fake.com")),
                              Return(true)));
  }

  attempter_.policy_provider_.reset(
      new policy::PolicyProvider(std::move(device_policy)));

  if (valid_slot) {
    BootControlInterface::Slot rollback_slot = 1;
    LOG(INFO) << "Test Mark Bootable: "
              << BootControlInterface::SlotName(rollback_slot);
    fake_system_state_.fake_boot_control()->SetSlotBootable(rollback_slot,
                                                            true);
  }

  bool is_rollback_allowed = false;

  // We only allow rollback on devices that are not enterprise enrolled and
  // which have a valid slot to rollback to.
  if (!enterprise_rollback && valid_slot) {
    is_rollback_allowed = true;
  }

  if (is_rollback_allowed) {
    InSequence s;
    for (size_t i = 0; i < arraysize(kRollbackActionTypes); ++i) {
      EXPECT_CALL(*processor_,
                  EnqueueAction(Pointee(Property(&AbstractAction::Type,
                                                 kRollbackActionTypes[i]))));
    }
    EXPECT_CALL(*processor_, StartProcessing());

    EXPECT_TRUE(attempter_.Rollback(true));
    loop_.PostTask(FROM_HERE,
                   base::Bind(&UpdateAttempterTest::RollbackTestVerify,
                              base::Unretained(this)));
  } else {
    EXPECT_FALSE(attempter_.Rollback(true));
    loop_.BreakLoop();
  }
}

void UpdateAttempterTest::RollbackTestVerify() {
  // Verifies the actions that were enqueued.
  EXPECT_EQ(&attempter_, processor_->delegate());
  EXPECT_EQ(UpdateStatus::ATTEMPTING_ROLLBACK, attempter_.status());
  EXPECT_EQ(0U, attempter_.install_plan_->partitions.size());
  EXPECT_EQ(attempter_.install_plan_->powerwash_required, true);
  loop_.BreakLoop();
}

TEST_F(UpdateAttempterTest, UpdateTest) {
  UpdateTestStart();
  loop_.Run();
}

TEST_F(UpdateAttempterTest, RollbackTest) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::RollbackTestStart,
                            base::Unretained(this),
                            false,
                            true));
  loop_.Run();
}

TEST_F(UpdateAttempterTest, InvalidSlotRollbackTest) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::RollbackTestStart,
                            base::Unretained(this),
                            false,
                            false));
  loop_.Run();
}

TEST_F(UpdateAttempterTest, EnterpriseRollbackTest) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::RollbackTestStart,
                            base::Unretained(this),
                            true,
                            true));
  loop_.Run();
}

void UpdateAttempterTest::PingOmahaTestStart() {
  EXPECT_CALL(*processor_,
              EnqueueAction(Pointee(Property(
                  &AbstractAction::Type, OmahaRequestAction::StaticType()))));
  EXPECT_CALL(*processor_, StartProcessing());
  attempter_.PingOmaha();
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, PingOmahaTest) {
  EXPECT_FALSE(attempter_.waiting_for_scheduled_check_);
  EXPECT_FALSE(attempter_.schedule_updates_called());
  // Disable scheduling of subsequnet checks; we're using the DefaultPolicy in
  // testing, which is more permissive than we want to handle here.
  attempter_.DisableScheduleUpdates();
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::PingOmahaTestStart,
                            base::Unretained(this)));
  brillo::MessageLoopRunMaxIterations(&loop_, 100);
  EXPECT_EQ(UpdateStatus::UPDATED_NEED_REBOOT, attempter_.status());
  EXPECT_TRUE(attempter_.schedule_updates_called());
}

TEST_F(UpdateAttempterTest, CreatePendingErrorEventTest) {
  MockAction action;
  const ErrorCode kCode = ErrorCode::kDownloadTransferError;
  attempter_.CreatePendingErrorEvent(&action, kCode);
  ASSERT_NE(nullptr, attempter_.error_event_.get());
  EXPECT_EQ(OmahaEvent::kTypeUpdateComplete, attempter_.error_event_->type);
  EXPECT_EQ(OmahaEvent::kResultError, attempter_.error_event_->result);
  EXPECT_EQ(
      static_cast<ErrorCode>(static_cast<int>(kCode) |
                             static_cast<int>(ErrorCode::kTestOmahaUrlFlag)),
      attempter_.error_event_->error_code);
}

TEST_F(UpdateAttempterTest, CreatePendingErrorEventResumedTest) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_resume = true;
  MockAction action;
  const ErrorCode kCode = ErrorCode::kInstallDeviceOpenError;
  attempter_.CreatePendingErrorEvent(&action, kCode);
  ASSERT_NE(nullptr, attempter_.error_event_.get());
  EXPECT_EQ(OmahaEvent::kTypeUpdateComplete, attempter_.error_event_->type);
  EXPECT_EQ(OmahaEvent::kResultError, attempter_.error_event_->result);
  EXPECT_EQ(
      static_cast<ErrorCode>(static_cast<int>(kCode) |
                             static_cast<int>(ErrorCode::kResumedFlag) |
                             static_cast<int>(ErrorCode::kTestOmahaUrlFlag)),
      attempter_.error_event_->error_code);
}

TEST_F(UpdateAttempterTest, P2PNotStartedAtStartupWhenNotEnabled) {
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetP2PEnabled(false);
  EXPECT_CALL(mock_p2p_manager, EnsureP2PRunning()).Times(0);
  attempter_.UpdateEngineStarted();
}

TEST_F(UpdateAttempterTest, P2PNotStartedAtStartupWhenEnabledButNotSharing) {
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetP2PEnabled(true);
  EXPECT_CALL(mock_p2p_manager, EnsureP2PRunning()).Times(0);
  attempter_.UpdateEngineStarted();
}

TEST_F(UpdateAttempterTest, P2PStartedAtStartupWhenEnabledAndSharing) {
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetP2PEnabled(true);
  mock_p2p_manager.fake().SetCountSharedFilesResult(1);
  EXPECT_CALL(mock_p2p_manager, EnsureP2PRunning());
  attempter_.UpdateEngineStarted();
}

TEST_F(UpdateAttempterTest, P2PNotEnabled) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::P2PNotEnabledStart,
                            base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::P2PNotEnabledStart() {
  // If P2P is not enabled, check that we do not attempt housekeeping
  // and do not convey that p2p is to be used.
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetP2PEnabled(false);
  EXPECT_CALL(mock_p2p_manager, PerformHousekeeping()).Times(0);
  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_FALSE(actual_using_p2p_for_downloading_);
  EXPECT_FALSE(actual_using_p2p_for_sharing());
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, P2PEnabledStartingFails) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::P2PEnabledStartingFailsStart,
                            base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::P2PEnabledStartingFailsStart() {
  // If p2p is enabled, but starting it fails ensure we don't do
  // any housekeeping and do not convey that p2p should be used.
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetP2PEnabled(true);
  mock_p2p_manager.fake().SetEnsureP2PRunningResult(false);
  mock_p2p_manager.fake().SetPerformHousekeepingResult(false);
  EXPECT_CALL(mock_p2p_manager, PerformHousekeeping()).Times(0);
  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_FALSE(actual_using_p2p_for_downloading());
  EXPECT_FALSE(actual_using_p2p_for_sharing());
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, P2PEnabledHousekeepingFails) {
  loop_.PostTask(
      FROM_HERE,
      base::Bind(&UpdateAttempterTest::P2PEnabledHousekeepingFailsStart,
                 base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::P2PEnabledHousekeepingFailsStart() {
  // If p2p is enabled, starting it works but housekeeping fails, ensure
  // we do not convey p2p is to be used.
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetP2PEnabled(true);
  mock_p2p_manager.fake().SetEnsureP2PRunningResult(true);
  mock_p2p_manager.fake().SetPerformHousekeepingResult(false);
  EXPECT_CALL(mock_p2p_manager, PerformHousekeeping());
  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_FALSE(actual_using_p2p_for_downloading());
  EXPECT_FALSE(actual_using_p2p_for_sharing());
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, P2PEnabled) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::P2PEnabledStart,
                            base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::P2PEnabledStart() {
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  // If P2P is enabled and starting it works, check that we performed
  // housekeeping and that we convey p2p should be used.
  mock_p2p_manager.fake().SetP2PEnabled(true);
  mock_p2p_manager.fake().SetEnsureP2PRunningResult(true);
  mock_p2p_manager.fake().SetPerformHousekeepingResult(true);
  EXPECT_CALL(mock_p2p_manager, PerformHousekeeping());
  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_TRUE(actual_using_p2p_for_downloading());
  EXPECT_TRUE(actual_using_p2p_for_sharing());
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, P2PEnabledInteractive) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::P2PEnabledInteractiveStart,
                            base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::P2PEnabledInteractiveStart() {
  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  // For an interactive check, if P2P is enabled and starting it
  // works, check that we performed housekeeping and that we convey
  // p2p should be used for sharing but NOT for downloading.
  mock_p2p_manager.fake().SetP2PEnabled(true);
  mock_p2p_manager.fake().SetEnsureP2PRunningResult(true);
  mock_p2p_manager.fake().SetPerformHousekeepingResult(true);
  EXPECT_CALL(mock_p2p_manager, PerformHousekeeping());
  attempter_.Update("",
                    "",
                    "",
                    "",
                    false,
                    false,
                    /*interactive=*/true);
  EXPECT_FALSE(actual_using_p2p_for_downloading());
  EXPECT_TRUE(actual_using_p2p_for_sharing());
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, ReadScatterFactorFromPolicy) {
  loop_.PostTask(
      FROM_HERE,
      base::Bind(&UpdateAttempterTest::ReadScatterFactorFromPolicyTestStart,
                 base::Unretained(this)));
  loop_.Run();
}

// Tests that the scatter_factor_in_seconds value is properly fetched
// from the device policy.
void UpdateAttempterTest::ReadScatterFactorFromPolicyTestStart() {
  int64_t scatter_factor_in_seconds = 36000;

  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  EXPECT_CALL(*device_policy, LoadPolicy()).WillRepeatedly(Return(true));
  fake_system_state_.set_device_policy(device_policy.get());

  EXPECT_CALL(*device_policy, GetScatterFactorInSeconds(_))
      .WillRepeatedly(
          DoAll(SetArgPointee<0>(scatter_factor_in_seconds), Return(true)));

  attempter_.policy_provider_.reset(
      new policy::PolicyProvider(std::move(device_policy)));

  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_EQ(scatter_factor_in_seconds, attempter_.scatter_factor_.InSeconds());

  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, DecrementUpdateCheckCountTest) {
  loop_.PostTask(
      FROM_HERE,
      base::Bind(&UpdateAttempterTest::DecrementUpdateCheckCountTestStart,
                 base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::DecrementUpdateCheckCountTestStart() {
  // Tests that the scatter_factor_in_seconds value is properly fetched
  // from the device policy and is decremented if value > 0.
  int64_t initial_value = 5;
  FakePrefs fake_prefs;
  attempter_.prefs_ = &fake_prefs;

  fake_system_state_.fake_hardware()->SetIsOOBEComplete(Time::UnixEpoch());

  EXPECT_TRUE(fake_prefs.SetInt64(kPrefsUpdateCheckCount, initial_value));

  int64_t scatter_factor_in_seconds = 10;

  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  EXPECT_CALL(*device_policy, LoadPolicy()).WillRepeatedly(Return(true));
  fake_system_state_.set_device_policy(device_policy.get());

  EXPECT_CALL(*device_policy, GetScatterFactorInSeconds(_))
      .WillRepeatedly(
          DoAll(SetArgPointee<0>(scatter_factor_in_seconds), Return(true)));

  attempter_.policy_provider_.reset(
      new policy::PolicyProvider(std::move(device_policy)));

  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_EQ(scatter_factor_in_seconds, attempter_.scatter_factor_.InSeconds());

  // Make sure the file still exists.
  EXPECT_TRUE(fake_prefs.Exists(kPrefsUpdateCheckCount));

  int64_t new_value;
  EXPECT_TRUE(fake_prefs.GetInt64(kPrefsUpdateCheckCount, &new_value));
  EXPECT_EQ(initial_value - 1, new_value);

  EXPECT_TRUE(
      attempter_.omaha_request_params_->update_check_count_wait_enabled());

  // However, if the count is already 0, it's not decremented. Test that.
  initial_value = 0;
  EXPECT_TRUE(fake_prefs.SetInt64(kPrefsUpdateCheckCount, initial_value));
  attempter_.Update("", "", "", "", false, false, false);
  EXPECT_TRUE(fake_prefs.Exists(kPrefsUpdateCheckCount));
  EXPECT_TRUE(fake_prefs.GetInt64(kPrefsUpdateCheckCount, &new_value));
  EXPECT_EQ(initial_value, new_value);

  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, NoScatteringDoneDuringManualUpdateTestStart) {
  loop_.PostTask(
      FROM_HERE,
      base::Bind(
          &UpdateAttempterTest::NoScatteringDoneDuringManualUpdateTestStart,
          base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::NoScatteringDoneDuringManualUpdateTestStart() {
  // Tests that no scattering logic is enabled if the update check
  // is manually done (as opposed to a scheduled update check)
  int64_t initial_value = 8;
  FakePrefs fake_prefs;
  attempter_.prefs_ = &fake_prefs;

  fake_system_state_.fake_hardware()->SetIsOOBEComplete(Time::UnixEpoch());
  fake_system_state_.set_prefs(&fake_prefs);

  EXPECT_TRUE(
      fake_prefs.SetInt64(kPrefsWallClockScatteringWaitPeriod, initial_value));
  EXPECT_TRUE(fake_prefs.SetInt64(kPrefsUpdateCheckCount, initial_value));

  // make sure scatter_factor is non-zero as scattering is disabled
  // otherwise.
  int64_t scatter_factor_in_seconds = 50;

  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  EXPECT_CALL(*device_policy, LoadPolicy()).WillRepeatedly(Return(true));
  fake_system_state_.set_device_policy(device_policy.get());

  EXPECT_CALL(*device_policy, GetScatterFactorInSeconds(_))
      .WillRepeatedly(
          DoAll(SetArgPointee<0>(scatter_factor_in_seconds), Return(true)));

  attempter_.policy_provider_.reset(
      new policy::PolicyProvider(std::move(device_policy)));

  // Trigger an interactive check so we can test that scattering is disabled.
  attempter_.Update("",
                    "",
                    "",
                    "",
                    false,
                    false,
                    /*interactive=*/true);
  EXPECT_EQ(scatter_factor_in_seconds, attempter_.scatter_factor_.InSeconds());

  // Make sure scattering is disabled for manual (i.e. user initiated) update
  // checks and all artifacts are removed.
  EXPECT_FALSE(
      attempter_.omaha_request_params_->wall_clock_based_wait_enabled());
  EXPECT_FALSE(fake_prefs.Exists(kPrefsWallClockScatteringWaitPeriod));
  EXPECT_EQ(0, attempter_.omaha_request_params_->waiting_period().InSeconds());
  EXPECT_FALSE(
      attempter_.omaha_request_params_->update_check_count_wait_enabled());
  EXPECT_FALSE(fake_prefs.Exists(kPrefsUpdateCheckCount));

  ScheduleQuitMainLoop();
}

void UpdateAttempterTest::SetUpStagingTest(const StagingSchedule& schedule,
                                           FakePrefs* prefs) {
  attempter_.prefs_ = prefs;
  fake_system_state_.set_prefs(prefs);

  int64_t initial_value = 8;
  EXPECT_TRUE(
      prefs->SetInt64(kPrefsWallClockScatteringWaitPeriod, initial_value));
  EXPECT_TRUE(prefs->SetInt64(kPrefsUpdateCheckCount, initial_value));
  attempter_.scatter_factor_ = TimeDelta::FromSeconds(20);

  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  EXPECT_CALL(*device_policy, LoadPolicy()).WillRepeatedly(Return(true));
  fake_system_state_.set_device_policy(device_policy.get());
  EXPECT_CALL(*device_policy, GetDeviceUpdateStagingSchedule(_))
      .WillRepeatedly(DoAll(SetArgPointee<0>(schedule), Return(true)));

  attempter_.policy_provider_.reset(
      new policy::PolicyProvider(std::move(device_policy)));
}

TEST_F(UpdateAttempterTest, StagingSetsPrefsAndTurnsOffScattering) {
  loop_.PostTask(
      FROM_HERE,
      base::Bind(
          &UpdateAttempterTest::StagingSetsPrefsAndTurnsOffScatteringStart,
          base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::StagingSetsPrefsAndTurnsOffScatteringStart() {
  // Tests that staging sets its prefs properly and turns off scattering.
  fake_system_state_.fake_hardware()->SetIsOOBEComplete(Time::UnixEpoch());
  FakePrefs fake_prefs;
  SetUpStagingTest(kValidStagingSchedule, &fake_prefs);

  attempter_.Update("", "", "", "", false, false, false);
  // Check that prefs have the correct values.
  int64_t update_count;
  EXPECT_TRUE(fake_prefs.GetInt64(kPrefsUpdateCheckCount, &update_count));
  int64_t waiting_time_days;
  EXPECT_TRUE(fake_prefs.GetInt64(kPrefsWallClockStagingWaitPeriod,
                                  &waiting_time_days));
  EXPECT_GT(waiting_time_days, 0);
  // Update count should have been decremented.
  EXPECT_EQ(7, update_count);
  // Check that Omaha parameters were updated correctly.
  EXPECT_TRUE(
      attempter_.omaha_request_params_->update_check_count_wait_enabled());
  EXPECT_TRUE(
      attempter_.omaha_request_params_->wall_clock_based_wait_enabled());
  EXPECT_EQ(waiting_time_days,
            attempter_.omaha_request_params_->waiting_period().InDays());
  // Check class variables.
  EXPECT_EQ(waiting_time_days, attempter_.staging_wait_time_.InDays());
  EXPECT_EQ(kValidStagingSchedule, attempter_.staging_schedule_);
  // Check that scattering is turned off
  EXPECT_EQ(0, attempter_.scatter_factor_.InSeconds());
  EXPECT_FALSE(fake_prefs.Exists(kPrefsWallClockScatteringWaitPeriod));

  ScheduleQuitMainLoop();
}

void UpdateAttempterTest::CheckStagingOff() {
  // Check that all prefs were removed.
  EXPECT_FALSE(attempter_.prefs_->Exists(kPrefsUpdateCheckCount));
  EXPECT_FALSE(attempter_.prefs_->Exists(kPrefsWallClockScatteringWaitPeriod));
  EXPECT_FALSE(attempter_.prefs_->Exists(kPrefsWallClockStagingWaitPeriod));
  // Check that the Omaha parameters have the correct value.
  EXPECT_EQ(0, attempter_.omaha_request_params_->waiting_period().InDays());
  EXPECT_EQ(attempter_.omaha_request_params_->waiting_period(),
            attempter_.staging_wait_time_);
  EXPECT_FALSE(
      attempter_.omaha_request_params_->update_check_count_wait_enabled());
  EXPECT_FALSE(
      attempter_.omaha_request_params_->wall_clock_based_wait_enabled());
  // Check that scattering is turned off too.
  EXPECT_EQ(0, attempter_.scatter_factor_.InSeconds());
}

TEST_F(UpdateAttempterTest, StagingOffIfInteractive) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::StagingOffIfInteractiveStart,
                            base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::StagingOffIfInteractiveStart() {
  // Tests that staging is turned off when an interactive update is requested.
  fake_system_state_.fake_hardware()->SetIsOOBEComplete(Time::UnixEpoch());
  FakePrefs fake_prefs;
  SetUpStagingTest(kValidStagingSchedule, &fake_prefs);

  attempter_.Update("", "", "", "", false, false, /* interactive = */ true);
  CheckStagingOff();

  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, StagingOffIfOobe) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::StagingOffIfOobeStart,
                            base::Unretained(this)));
  loop_.Run();
}

void UpdateAttempterTest::StagingOffIfOobeStart() {
  // Tests that staging is turned off if OOBE hasn't been completed.
  fake_system_state_.fake_hardware()->SetIsOOBEEnabled(true);
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();
  FakePrefs fake_prefs;
  SetUpStagingTest(kValidStagingSchedule, &fake_prefs);

  attempter_.Update("", "", "", "", false, false, /* interactive = */ true);
  CheckStagingOff();

  ScheduleQuitMainLoop();
}

// Checks that we only report daily metrics at most every 24 hours.
TEST_F(UpdateAttempterTest, ReportDailyMetrics) {
  FakeClock fake_clock;
  FakePrefs fake_prefs;

  fake_system_state_.set_clock(&fake_clock);
  fake_system_state_.set_prefs(&fake_prefs);

  Time epoch = Time::FromInternalValue(0);
  fake_clock.SetWallclockTime(epoch);

  // If there is no kPrefsDailyMetricsLastReportedAt state variable,
  // we should report.
  EXPECT_TRUE(attempter_.CheckAndReportDailyMetrics());
  // We should not report again if no time has passed.
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // We should not report if only 10 hours has passed.
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(10));
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // We should not report if only 24 hours - 1 sec has passed.
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(24) -
                              TimeDelta::FromSeconds(1));
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // We should report if 24 hours has passed.
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(24));
  EXPECT_TRUE(attempter_.CheckAndReportDailyMetrics());

  // But then we should not report again..
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // .. until another 24 hours has passed
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(47));
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(48));
  EXPECT_TRUE(attempter_.CheckAndReportDailyMetrics());
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // .. and another 24 hours
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(71));
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(72));
  EXPECT_TRUE(attempter_.CheckAndReportDailyMetrics());
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // If the span between time of reporting and present time is
  // negative, we report. This is in order to reset the timestamp and
  // avoid an edge condition whereby a distant point in the future is
  // in the state variable resulting in us never ever reporting again.
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(71));
  EXPECT_TRUE(attempter_.CheckAndReportDailyMetrics());
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());

  // In this case we should not update until the clock reads 71 + 24 = 95.
  // Check that.
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(94));
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());
  fake_clock.SetWallclockTime(epoch + TimeDelta::FromHours(95));
  EXPECT_TRUE(attempter_.CheckAndReportDailyMetrics());
  EXPECT_FALSE(attempter_.CheckAndReportDailyMetrics());
}

TEST_F(UpdateAttempterTest, BootTimeInUpdateMarkerFile) {
  FakeClock fake_clock;
  fake_clock.SetBootTime(Time::FromTimeT(42));
  fake_system_state_.set_clock(&fake_clock);
  FakePrefs fake_prefs;
  fake_system_state_.set_prefs(&fake_prefs);
  attempter_.Init();

  Time boot_time;
  EXPECT_FALSE(attempter_.GetBootTimeAtUpdate(&boot_time));

  attempter_.WriteUpdateCompletedMarker();

  EXPECT_TRUE(attempter_.GetBootTimeAtUpdate(&boot_time));
  EXPECT_EQ(boot_time.ToTimeT(), 42);
}

TEST_F(UpdateAttempterTest, AnyUpdateSourceAllowedUnofficial) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(false);
  EXPECT_TRUE(attempter_.IsAnyUpdateSourceAllowed());
}

TEST_F(UpdateAttempterTest, AnyUpdateSourceAllowedOfficialDevmode) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(true);
  fake_system_state_.fake_hardware()->SetAreDevFeaturesEnabled(true);
  EXPECT_TRUE(attempter_.IsAnyUpdateSourceAllowed());
}

TEST_F(UpdateAttempterTest, AnyUpdateSourceDisallowedOfficialNormal) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(true);
  fake_system_state_.fake_hardware()->SetAreDevFeaturesEnabled(false);
  EXPECT_FALSE(attempter_.IsAnyUpdateSourceAllowed());
}

TEST_F(UpdateAttempterTest, CheckForUpdateAUDlcTest) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(true);
  fake_system_state_.fake_hardware()->SetAreDevFeaturesEnabled(false);

  const string dlc_module_id = "a_dlc_module_id";
  vector<string> dlc_module_ids = {dlc_module_id};
  ON_CALL(mock_dlcservice_, GetInstalled(testing::_))
      .WillByDefault(DoAll(testing::SetArgPointee<0>(dlc_module_ids),
                           testing::Return(true)));

  attempter_.CheckForUpdate("", "autest", UpdateAttemptFlags::kNone);
  EXPECT_EQ(attempter_.dlc_module_ids_.size(), 1);
  EXPECT_EQ(attempter_.dlc_module_ids_[0], dlc_module_id);
}

TEST_F(UpdateAttempterTest, CheckForUpdateAUTest) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(true);
  fake_system_state_.fake_hardware()->SetAreDevFeaturesEnabled(false);
  attempter_.CheckForUpdate("", "autest", UpdateAttemptFlags::kNone);
  EXPECT_EQ(constants::kOmahaDefaultAUTestURL, attempter_.forced_omaha_url());
}

TEST_F(UpdateAttempterTest, CheckForUpdateScheduledAUTest) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(true);
  fake_system_state_.fake_hardware()->SetAreDevFeaturesEnabled(false);
  attempter_.CheckForUpdate("", "autest-scheduled", UpdateAttemptFlags::kNone);
  EXPECT_EQ(constants::kOmahaDefaultAUTestURL, attempter_.forced_omaha_url());
}

TEST_F(UpdateAttempterTest, CheckForInstallTest) {
  fake_system_state_.fake_hardware()->SetIsOfficialBuild(true);
  fake_system_state_.fake_hardware()->SetAreDevFeaturesEnabled(false);
  attempter_.CheckForInstall({}, "autest");
  EXPECT_EQ(constants::kOmahaDefaultAUTestURL, attempter_.forced_omaha_url());

  attempter_.CheckForInstall({}, "autest-scheduled");
  EXPECT_EQ(constants::kOmahaDefaultAUTestURL, attempter_.forced_omaha_url());

  attempter_.CheckForInstall({}, "http://omaha.phishing");
  EXPECT_EQ("", attempter_.forced_omaha_url());
}

TEST_F(UpdateAttempterTest, InstallSetsStatusIdle) {
  attempter_.CheckForInstall({}, "http://foo.bar");
  attempter_.status_ = UpdateStatus::DOWNLOADING;
  EXPECT_TRUE(attempter_.is_install_);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
  UpdateEngineStatus status;
  attempter_.GetStatus(&status);
  // Should set status to idle after an install operation.
  EXPECT_EQ(UpdateStatus::IDLE, status.status);
}

TEST_F(UpdateAttempterTest, RollbackAfterInstall) {
  attempter_.is_install_ = true;
  attempter_.Rollback(false);
  EXPECT_FALSE(attempter_.is_install_);
}

TEST_F(UpdateAttempterTest, UpdateAfterInstall) {
  attempter_.is_install_ = true;
  attempter_.CheckForUpdate("", "", UpdateAttemptFlags::kNone);
  EXPECT_FALSE(attempter_.is_install_);
}

TEST_F(UpdateAttempterTest, TargetVersionPrefixSetAndReset) {
  attempter_.CalculateUpdateParams("", "", "", "1234", false, false, false);
  EXPECT_EQ("1234",
            fake_system_state_.request_params()->target_version_prefix());

  attempter_.CalculateUpdateParams("", "", "", "", false, false, false);
  EXPECT_TRUE(
      fake_system_state_.request_params()->target_version_prefix().empty());
}

TEST_F(UpdateAttempterTest, RollbackAllowedSetAndReset) {
  attempter_.CalculateUpdateParams("",
                                   "",
                                   "",
                                   "1234",
                                   /*rollback_allowed=*/true,
                                   false,
                                   false);
  EXPECT_TRUE(fake_system_state_.request_params()->rollback_allowed());

  attempter_.CalculateUpdateParams("",
                                   "",
                                   "",
                                   "1234",
                                   /*rollback_allowed=*/false,
                                   false,
                                   false);
  EXPECT_FALSE(fake_system_state_.request_params()->rollback_allowed());
}

TEST_F(UpdateAttempterTest, UpdateDeferredByPolicyTest) {
  // Construct an OmahaResponseHandlerAction that has processed an InstallPlan,
  // but the update is being deferred by the Policy.
  OmahaResponseHandlerAction response_action(&fake_system_state_);
  response_action.install_plan_.version = "a.b.c.d";
  response_action.install_plan_.system_version = "b.c.d.e";
  response_action.install_plan_.payloads.push_back(
      {.size = 1234ULL, .type = InstallPayloadType::kFull});
  // Inform the UpdateAttempter that the OmahaResponseHandlerAction has
  // completed, with the deferred-update error code.
  attempter_.ActionCompleted(
      nullptr, &response_action, ErrorCode::kOmahaUpdateDeferredPerPolicy);
  {
    UpdateEngineStatus status;
    attempter_.GetStatus(&status);
    EXPECT_EQ(UpdateStatus::UPDATE_AVAILABLE, status.status);
    EXPECT_TRUE(attempter_.install_plan_);
    EXPECT_EQ(attempter_.install_plan_->version, status.new_version);
    EXPECT_EQ(attempter_.install_plan_->system_version,
              status.new_system_version);
    EXPECT_EQ(attempter_.install_plan_->payloads[0].size,
              status.new_size_bytes);
  }
  // An "error" event should have been created to tell Omaha that the update is
  // being deferred.
  EXPECT_TRUE(nullptr != attempter_.error_event_);
  EXPECT_EQ(OmahaEvent::kTypeUpdateComplete, attempter_.error_event_->type);
  EXPECT_EQ(OmahaEvent::kResultUpdateDeferred, attempter_.error_event_->result);
  ErrorCode expected_code = static_cast<ErrorCode>(
      static_cast<int>(ErrorCode::kOmahaUpdateDeferredPerPolicy) |
      static_cast<int>(ErrorCode::kTestOmahaUrlFlag));
  EXPECT_EQ(expected_code, attempter_.error_event_->error_code);
  // End the processing
  attempter_.ProcessingDone(nullptr, ErrorCode::kOmahaUpdateDeferredPerPolicy);
  // Validate the state of the attempter.
  {
    UpdateEngineStatus status;
    attempter_.GetStatus(&status);
    EXPECT_EQ(UpdateStatus::REPORTING_ERROR_EVENT, status.status);
    EXPECT_EQ(response_action.install_plan_.version, status.new_version);
    EXPECT_EQ(response_action.install_plan_.system_version,
              status.new_system_version);
    EXPECT_EQ(response_action.install_plan_.payloads[0].size,
              status.new_size_bytes);
  }
}

TEST_F(UpdateAttempterTest, UpdateIsNotRunningWhenUpdateAvailable) {
  EXPECT_FALSE(attempter_.IsUpdateRunningOrScheduled());
  // Verify in-progress update with UPDATE_AVAILABLE is running
  attempter_.status_ = UpdateStatus::UPDATE_AVAILABLE;
  EXPECT_TRUE(attempter_.IsUpdateRunningOrScheduled());
}

TEST_F(UpdateAttempterTest, UpdateAttemptFlagsCachedAtUpdateStart) {
  attempter_.SetUpdateAttemptFlags(UpdateAttemptFlags::kFlagRestrictDownload);

  UpdateCheckParams params = {.updates_enabled = true};
  attempter_.OnUpdateScheduled(EvalStatus::kSucceeded, params);

  EXPECT_EQ(UpdateAttemptFlags::kFlagRestrictDownload,
            attempter_.GetCurrentUpdateAttemptFlags());
}

TEST_F(UpdateAttempterTest, RollbackNotAllowed) {
  UpdateCheckParams params = {.updates_enabled = true,
                              .rollback_allowed = false};
  attempter_.OnUpdateScheduled(EvalStatus::kSucceeded, params);
  EXPECT_FALSE(fake_system_state_.request_params()->rollback_allowed());
}

TEST_F(UpdateAttempterTest, RollbackAllowed) {
  UpdateCheckParams params = {.updates_enabled = true,
                              .rollback_allowed = true};
  attempter_.OnUpdateScheduled(EvalStatus::kSucceeded, params);
  EXPECT_TRUE(fake_system_state_.request_params()->rollback_allowed());
}

TEST_F(UpdateAttempterTest, InteractiveUpdateUsesPassedRestrictions) {
  attempter_.SetUpdateAttemptFlags(UpdateAttemptFlags::kFlagRestrictDownload);

  attempter_.CheckForUpdate("", "", UpdateAttemptFlags::kNone);
  EXPECT_EQ(UpdateAttemptFlags::kNone,
            attempter_.GetCurrentUpdateAttemptFlags());
}

TEST_F(UpdateAttempterTest, NonInteractiveUpdateUsesSetRestrictions) {
  attempter_.SetUpdateAttemptFlags(UpdateAttemptFlags::kNone);

  // This tests that when CheckForUpdate() is called with the non-interactive
  // flag set, that it doesn't change the current UpdateAttemptFlags.
  attempter_.CheckForUpdate("",
                            "",
                            UpdateAttemptFlags::kFlagNonInteractive |
                                UpdateAttemptFlags::kFlagRestrictDownload);
  EXPECT_EQ(UpdateAttemptFlags::kNone,
            attempter_.GetCurrentUpdateAttemptFlags());
}

void UpdateAttempterTest::ResetRollbackHappenedStart(bool is_consumer,
                                                     bool is_policy_loaded,
                                                     bool expected_reset) {
  EXPECT_CALL(*fake_system_state_.mock_payload_state(), GetRollbackHappened())
      .WillRepeatedly(Return(true));
  auto mock_policy_provider =
      std::make_unique<NiceMock<policy::MockPolicyProvider>>();
  EXPECT_CALL(*mock_policy_provider, IsConsumerDevice())
      .WillRepeatedly(Return(is_consumer));
  EXPECT_CALL(*mock_policy_provider, device_policy_is_loaded())
      .WillRepeatedly(Return(is_policy_loaded));
  const policy::MockDevicePolicy device_policy;
  EXPECT_CALL(*mock_policy_provider, GetDevicePolicy())
      .WillRepeatedly(ReturnRef(device_policy));
  EXPECT_CALL(*fake_system_state_.mock_payload_state(),
              SetRollbackHappened(false))
      .Times(expected_reset ? 1 : 0);
  attempter_.policy_provider_ = std::move(mock_policy_provider);
  attempter_.Update("", "", "", "", false, false, false);
  ScheduleQuitMainLoop();
}

TEST_F(UpdateAttempterTest, ResetRollbackHappenedOobe) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::ResetRollbackHappenedStart,
                            base::Unretained(this),
                            /*is_consumer=*/false,
                            /*is_policy_loaded=*/false,
                            /*expected_reset=*/false));
  loop_.Run();
}

TEST_F(UpdateAttempterTest, ResetRollbackHappenedConsumer) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::ResetRollbackHappenedStart,
                            base::Unretained(this),
                            /*is_consumer=*/true,
                            /*is_policy_loaded=*/false,
                            /*expected_reset=*/true));
  loop_.Run();
}

TEST_F(UpdateAttempterTest, ResetRollbackHappenedEnterprise) {
  loop_.PostTask(FROM_HERE,
                 base::Bind(&UpdateAttempterTest::ResetRollbackHappenedStart,
                            base::Unretained(this),
                            /*is_consumer=*/false,
                            /*is_policy_loaded=*/true,
                            /*expected_reset=*/true));
  loop_.Run();
}

TEST_F(UpdateAttempterTest, SetRollbackHappenedRollback) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_rollback = true;

  EXPECT_CALL(*fake_system_state_.mock_payload_state(),
              SetRollbackHappened(true))
      .Times(1);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

TEST_F(UpdateAttempterTest, SetRollbackHappenedNotRollback) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_rollback = false;

  EXPECT_CALL(*fake_system_state_.mock_payload_state(),
              SetRollbackHappened(true))
      .Times(0);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

TEST_F(UpdateAttempterTest, RollbackMetricsRollbackSuccess) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_rollback = true;
  attempter_.install_plan_->version = kRollbackVersion;

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseRollbackMetrics(true, kRollbackVersion))
      .Times(1);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

TEST_F(UpdateAttempterTest, RollbackMetricsNotRollbackSuccess) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_rollback = false;
  attempter_.install_plan_->version = kRollbackVersion;

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseRollbackMetrics(_, _))
      .Times(0);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

TEST_F(UpdateAttempterTest, RollbackMetricsRollbackFailure) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_rollback = true;
  attempter_.install_plan_->version = kRollbackVersion;

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseRollbackMetrics(false, kRollbackVersion))
      .Times(1);
  MockAction action;
  attempter_.CreatePendingErrorEvent(&action, ErrorCode::kRollbackNotPossible);
  attempter_.ProcessingDone(nullptr, ErrorCode::kRollbackNotPossible);
}

TEST_F(UpdateAttempterTest, RollbackMetricsNotRollbackFailure) {
  attempter_.install_plan_.reset(new InstallPlan);
  attempter_.install_plan_->is_rollback = false;
  attempter_.install_plan_->version = kRollbackVersion;

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseRollbackMetrics(_, _))
      .Times(0);
  MockAction action;
  attempter_.CreatePendingErrorEvent(&action, ErrorCode::kRollbackNotPossible);
  attempter_.ProcessingDone(nullptr, ErrorCode::kRollbackNotPossible);
}

TEST_F(UpdateAttempterTest, TimeToUpdateAppliedMetricFailure) {
  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseUpdateSeenToDownloadDays(_, _))
      .Times(0);
  attempter_.ProcessingDone(nullptr, ErrorCode::kOmahaUpdateDeferredPerPolicy);
}

TEST_F(UpdateAttempterTest, TimeToUpdateAppliedOnNonEnterprise) {
  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  fake_system_state_.set_device_policy(device_policy.get());
  // Make device policy return that this is not enterprise enrolled
  EXPECT_CALL(*device_policy, IsEnterpriseEnrolled()).WillOnce(Return(false));

  // Ensure that the metric is not recorded.
  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseUpdateSeenToDownloadDays(_, _))
      .Times(0);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

TEST_F(UpdateAttempterTest,
       TimeToUpdateAppliedWithTimeRestrictionMetricSuccess) {
  constexpr int kDaysToUpdate = 15;
  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  fake_system_state_.set_device_policy(device_policy.get());
  // Make device policy return that this is enterprise enrolled
  EXPECT_CALL(*device_policy, IsEnterpriseEnrolled()).WillOnce(Return(true));
  // Pretend that there's a time restriction policy in place
  EXPECT_CALL(*device_policy, GetDisallowedTimeIntervals(_))
      .WillOnce(Return(true));

  FakePrefs fake_prefs;
  Time update_first_seen_at = Time::Now();
  fake_prefs.SetInt64(kPrefsUpdateFirstSeenAt,
                      update_first_seen_at.ToInternalValue());

  FakeClock fake_clock;
  Time update_finished_at =
      update_first_seen_at + TimeDelta::FromDays(kDaysToUpdate);
  fake_clock.SetWallclockTime(update_finished_at);

  fake_system_state_.set_clock(&fake_clock);
  fake_system_state_.set_prefs(&fake_prefs);

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseUpdateSeenToDownloadDays(true, kDaysToUpdate))
      .Times(1);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

TEST_F(UpdateAttempterTest,
       TimeToUpdateAppliedWithoutTimeRestrictionMetricSuccess) {
  constexpr int kDaysToUpdate = 15;
  auto device_policy = std::make_unique<policy::MockDevicePolicy>();
  fake_system_state_.set_device_policy(device_policy.get());
  // Make device policy return that this is enterprise enrolled
  EXPECT_CALL(*device_policy, IsEnterpriseEnrolled()).WillOnce(Return(true));
  // Pretend that there's no time restriction policy in place
  EXPECT_CALL(*device_policy, GetDisallowedTimeIntervals(_))
      .WillOnce(Return(false));

  FakePrefs fake_prefs;
  Time update_first_seen_at = Time::Now();
  fake_prefs.SetInt64(kPrefsUpdateFirstSeenAt,
                      update_first_seen_at.ToInternalValue());

  FakeClock fake_clock;
  Time update_finished_at =
      update_first_seen_at + TimeDelta::FromDays(kDaysToUpdate);
  fake_clock.SetWallclockTime(update_finished_at);

  fake_system_state_.set_clock(&fake_clock);
  fake_system_state_.set_prefs(&fake_prefs);

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportEnterpriseUpdateSeenToDownloadDays(false, kDaysToUpdate))
      .Times(1);
  attempter_.ProcessingDone(nullptr, ErrorCode::kSuccess);
}

}  // namespace chromeos_update_engine
