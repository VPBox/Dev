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

#include "update_engine/omaha_request_action.h"

#include <stdint.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <base/memory/ptr_util.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <gtest/gtest.h>
#include <policy/libpolicy.h>
#include <policy/mock_libpolicy.h>

#include "update_engine/common/action_pipe.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/common/hash_calculator.h"
#include "update_engine/common/mock_http_fetcher.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/prefs.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/fake_system_state.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/mock_connection_manager.h"
#include "update_engine/mock_payload_state.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/update_manager/rollback_prefs.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_manager::kRollforwardInfinity;
using std::string;
using std::vector;
using testing::_;
using testing::AllOf;
using testing::AnyNumber;
using testing::DoAll;
using testing::Ge;
using testing::Le;
using testing::NiceMock;
using testing::Return;
using testing::ReturnPointee;
using testing::ReturnRef;
using testing::SaveArg;
using testing::SetArgPointee;

namespace {

static_assert(kRollforwardInfinity == 0xfffffffe,
              "Don't change the value of kRollforward infinity unless its "
              "size has been changed in firmware.");

const char kCurrentVersion[] = "0.1.0.0";
const char kTestAppId[] = "test-app-id";
const char kTestAppId2[] = "test-app2-id";
const char kTestAppIdSkipUpdatecheck[] = "test-app-id-skip-updatecheck";

// This is a helper struct to allow unit tests build an update response with the
// values they care about.
struct FakeUpdateResponse {
  string GetRollbackVersionAttributes() const {
    return (rollback ? " _rollback=\"true\"" : "") +
           (!rollback_firmware_version.empty()
                ? " _firmware_version=\"" + rollback_firmware_version + "\""
                : "") +
           (!rollback_kernel_version.empty()
                ? " _kernel_version=\"" + rollback_kernel_version + "\""
                : "");
  }

  string GetNoUpdateResponse() const {
    string entity_str;
    if (include_entity)
      entity_str = "<!DOCTYPE response [<!ENTITY CrOS \"ChromeOS\">]>";
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + entity_str +
           "<response protocol=\"3.0\">"
           "<daystart elapsed_seconds=\"100\"/>"
           "<app appid=\"" +
           app_id + "\" " +
           (include_cohorts
                ? "cohort=\"" + cohort + "\" cohorthint=\"" + cohorthint +
                      "\" cohortname=\"" + cohortname + "\" "
                : "") +
           " status=\"ok\">"
           "<ping status=\"ok\"/>"
           "<updatecheck status=\"noupdate\"/></app>" +
           (multi_app_no_update
                ? "<app appid=\"" + app_id2 +
                      "\"><updatecheck status=\"noupdate\"/></app>"
                : "") +
           "</response>";
  }

  string GetUpdateResponse() const {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?><response "
           "protocol=\"3.0\">"
           "<daystart elapsed_seconds=\"100\"" +
           (elapsed_days.empty() ? ""
                                 : (" elapsed_days=\"" + elapsed_days + "\"")) +
           "/>"
           "<app appid=\"" +
           app_id + "\" " +
           (include_cohorts
                ? "cohort=\"" + cohort + "\" cohorthint=\"" + cohorthint +
                      "\" cohortname=\"" + cohortname + "\" "
                : "") +
           " status=\"ok\">"
           "<ping status=\"ok\"/><updatecheck status=\"ok\"" +
           GetRollbackVersionAttributes() + ">" + "<urls><url codebase=\"" +
           codebase +
           "\"/></urls>"
           "<manifest version=\"" +
           version +
           "\">"
           "<packages><package hash=\"not-used\" name=\"" +
           filename + "\" size=\"" + base::Int64ToString(size) +
           "\" hash_sha256=\"" + hash + "\"/>" +
           (multi_package ? "<package name=\"package2\" size=\"222\" "
                            "hash_sha256=\"hash2\"/>"
                          : "") +
           "</packages>"
           "<actions><action event=\"postinstall\" MetadataSize=\"11" +
           (multi_package ? ":22" : "") + "\" MoreInfo=\"" + more_info_url +
           "\" Prompt=\"" + prompt +
           "\" "
           "IsDeltaPayload=\"true" +
           (multi_package ? ":false" : "") +
           "\" "
           "MaxDaysToScatter=\"" +
           max_days_to_scatter +
           "\" "
           "sha256=\"not-used\" " +
           (deadline.empty() ? "" : ("deadline=\"" + deadline + "\" ")) +
           (disable_p2p_for_downloading ? "DisableP2PForDownloading=\"true\" "
                                        : "") +
           (disable_p2p_for_sharing ? "DisableP2PForSharing=\"true\" " : "") +
           (powerwash ? "Powerwash=\"true\" " : "") +
           "/></actions></manifest></updatecheck></app>" +
           (multi_app
                ? "<app appid=\"" + app_id2 + "\"" +
                      (include_cohorts ? " cohort=\"cohort2\"" : "") +
                      "><updatecheck status=\"ok\"><urls><url codebase=\"" +
                      codebase2 + "\"/></urls><manifest version=\"" + version2 +
                      "\"><packages>"
                      "<package name=\"package3\" size=\"333\" "
                      "hash_sha256=\"hash3\"/></packages>"
                      "<actions><action event=\"postinstall\" " +
                      (multi_app_self_update
                           ? "noupdate=\"true\" IsDeltaPayload=\"true\" "
                           : "IsDeltaPayload=\"false\" ") +
                      "MetadataSize=\"33\"/></actions>"
                      "</manifest></updatecheck></app>"
                : "") +
           (multi_app_no_update
                ? "<app><updatecheck status=\"noupdate\"/></app>"
                : "") +
           (multi_app_skip_updatecheck
                ? "<app appid=\"" + app_id_skip_updatecheck + "\"></app>"
                : "") +
           "</response>";
  }

  // Return the payload URL, which is split in two fields in the XML response.
  string GetPayloadUrl() { return codebase + filename; }

  string app_id = kTestAppId;
  string app_id2 = kTestAppId2;
  string app_id_skip_updatecheck = kTestAppIdSkipUpdatecheck;
  string current_version = kCurrentVersion;
  string version = "1.2.3.4";
  string version2 = "2.3.4.5";
  string more_info_url = "http://more/info";
  string prompt = "true";
  string codebase = "http://code/base/";
  string codebase2 = "http://code/base/2/";
  string filename = "file.signed";
  string hash = "4841534831323334";
  uint64_t size = 123;
  string deadline = "";
  string max_days_to_scatter = "7";
  string elapsed_days = "42";

  // P2P setting defaults to allowed.
  bool disable_p2p_for_downloading = false;
  bool disable_p2p_for_sharing = false;

  bool powerwash = false;

  // Omaha cohorts settings.
  bool include_cohorts = false;
  string cohort = "";
  string cohorthint = "";
  string cohortname = "";

  // Whether to include the CrOS <!ENTITY> in the XML response.
  bool include_entity = false;

  // Whether to include more than one app.
  bool multi_app = false;
  // Whether to include an app with noupdate="true".
  bool multi_app_self_update = false;
  // Whether to include an additional app with status="noupdate".
  bool multi_app_no_update = false;
  // Whether to include an additional app with no updatecheck tag.
  bool multi_app_skip_updatecheck = false;
  // Whether to include more than one package in an app.
  bool multi_package = false;

  // Whether the payload is a rollback.
  bool rollback = false;
  // The verified boot firmware key version for the rollback image.
  string rollback_firmware_version = "";
  // The verified boot kernel key version for the rollback image.
  string rollback_kernel_version = "";
};

}  // namespace

namespace chromeos_update_engine {

class OmahaRequestActionTestProcessorDelegate : public ActionProcessorDelegate {
 public:
  OmahaRequestActionTestProcessorDelegate()
      : expected_code_(ErrorCode::kSuccess),
        interactive_(false),
        test_http_fetcher_headers_(false) {}
  ~OmahaRequestActionTestProcessorDelegate() override = default;

  void ProcessingDone(const ActionProcessor* processor,
                      ErrorCode code) override {
    brillo::MessageLoop::current()->BreakLoop();
  }

  void ActionCompleted(ActionProcessor* processor,
                       AbstractAction* action,
                       ErrorCode code) override {
    // Make sure actions always succeed.
    if (action->Type() == OmahaRequestAction::StaticType()) {
      EXPECT_EQ(expected_code_, code);
      // Check that the headers were set in the fetcher during the action. Note
      // that we set this request as "interactive".
      auto fetcher = static_cast<const MockHttpFetcher*>(
          static_cast<OmahaRequestAction*>(action)->http_fetcher_.get());

      if (test_http_fetcher_headers_) {
        EXPECT_EQ(interactive_ ? "fg" : "bg",
                  fetcher->GetHeader("X-Goog-Update-Interactivity"));
        EXPECT_EQ(kTestAppId, fetcher->GetHeader("X-Goog-Update-AppId"));
        EXPECT_NE("", fetcher->GetHeader("X-Goog-Update-Updater"));
      }
      post_data_ = fetcher->post_data();
    } else if (action->Type() ==
               ObjectCollectorAction<OmahaResponse>::StaticType()) {
      EXPECT_EQ(ErrorCode::kSuccess, code);
      auto collector_action =
          static_cast<ObjectCollectorAction<OmahaResponse>*>(action);
      omaha_response_.reset(new OmahaResponse(collector_action->object()));
      EXPECT_TRUE(omaha_response_);
    } else {
      EXPECT_EQ(ErrorCode::kSuccess, code);
    }
  }
  ErrorCode expected_code_;
  brillo::Blob post_data_;
  bool interactive_;
  bool test_http_fetcher_headers_;
  std::unique_ptr<OmahaResponse> omaha_response_;
};

class OmahaRequestActionTest : public ::testing::Test {
 protected:
  void SetUp() override {
    request_params_.set_os_sp("service_pack");
    request_params_.set_os_board("x86-generic");
    request_params_.set_app_id(kTestAppId);
    request_params_.set_app_version(kCurrentVersion);
    request_params_.set_app_lang("en-US");
    request_params_.set_current_channel("unittest");
    request_params_.set_target_channel("unittest");
    request_params_.set_hwid("OEM MODEL 09235 7471");
    request_params_.set_fw_version("ChromeOSFirmware.1.0");
    request_params_.set_ec_version("0X0A1");
    request_params_.set_delta_okay(true);
    request_params_.set_interactive(false);
    request_params_.set_update_url("http://url");
    request_params_.set_target_version_prefix("");
    request_params_.set_rollback_allowed(false);
    request_params_.set_is_powerwash_allowed(false);
    request_params_.set_is_install(false);
    request_params_.set_dlc_module_ids({});

    fake_system_state_.set_request_params(&request_params_);
    fake_system_state_.set_prefs(&fake_prefs_);
  }

  // Returns true iff an output response was obtained from the
  // OmahaRequestAction. |prefs| may be null, in which case a local MockPrefs
  // is used. |payload_state| may be null, in which case a local mock is used.
  // |p2p_manager| may be null, in which case a local mock is used.
  // |connection_manager| may be null, in which case a local mock is used.
  // out_response may be null. If |fail_http_response_code| is non-negative,
  // the transfer will fail with that code. |ping_only| is passed through to the
  // OmahaRequestAction constructor. out_post_data may be null; if non-null, the
  // post-data received by the mock HttpFetcher is returned.
  //
  // The |expected_check_result|, |expected_check_reaction| and
  // |expected_error_code| parameters are for checking expectations
  // about reporting UpdateEngine.Check.{Result,Reaction,DownloadError}
  // UMA statistics. Use the appropriate ::kUnset value to specify that
  // the given metric should not be reported.
  bool TestUpdateCheck(const string& http_response,
                       int fail_http_response_code,
                       bool ping_only,
                       bool is_consumer_device,
                       int rollback_allowed_milestones,
                       bool is_policy_loaded,
                       ErrorCode expected_code,
                       metrics::CheckResult expected_check_result,
                       metrics::CheckReaction expected_check_reaction,
                       metrics::DownloadErrorCode expected_download_error_code,
                       OmahaResponse* out_response,
                       brillo::Blob* out_post_data);

  // Overload of TestUpdateCheck that does not supply |is_consumer_device| or
  // |rollback_allowed_milestones| which are only required for rollback tests.
  bool TestUpdateCheck(const string& http_response,
                       int fail_http_response_code,
                       bool ping_only,
                       ErrorCode expected_code,
                       metrics::CheckResult expected_check_result,
                       metrics::CheckReaction expected_check_reaction,
                       metrics::DownloadErrorCode expected_download_error_code,
                       OmahaResponse* out_response,
                       brillo::Blob* out_post_data);

  void TestRollbackCheck(bool is_consumer_device,
                         int rollback_allowed_milestones,
                         bool is_policy_loaded,
                         OmahaResponse* out_response);

  void TestEvent(OmahaEvent* event,
                 const string& http_response,
                 brillo::Blob* out_post_data);

  // Runs and checks a ping test. |ping_only| indicates whether it should send
  // only a ping or also an updatecheck.
  void PingTest(bool ping_only);

  // InstallDate test helper function.
  bool InstallDateParseHelper(const string& elapsed_days,
                              OmahaResponse* response);

  // P2P test helper function.
  void P2PTest(bool initial_allow_p2p_for_downloading,
               bool initial_allow_p2p_for_sharing,
               bool omaha_disable_p2p_for_downloading,
               bool omaha_disable_p2p_for_sharing,
               bool payload_state_allow_p2p_attempt,
               bool expect_p2p_client_lookup,
               const string& p2p_client_result_url,
               bool expected_allow_p2p_for_downloading,
               bool expected_allow_p2p_for_sharing,
               const string& expected_p2p_url);

  FakeSystemState fake_system_state_;
  FakeUpdateResponse fake_update_response_;
  // Used by all tests.
  OmahaRequestParams request_params_{&fake_system_state_};

  FakePrefs fake_prefs_;

  OmahaRequestActionTestProcessorDelegate delegate_;

  bool test_http_fetcher_headers_{false};
};

bool OmahaRequestActionTest::TestUpdateCheck(
    const string& http_response,
    int fail_http_response_code,
    bool ping_only,
    bool is_consumer_device,
    int rollback_allowed_milestones,
    bool is_policy_loaded,
    ErrorCode expected_code,
    metrics::CheckResult expected_check_result,
    metrics::CheckReaction expected_check_reaction,
    metrics::DownloadErrorCode expected_download_error_code,
    OmahaResponse* out_response,
    brillo::Blob* out_post_data) {
  brillo::FakeMessageLoop loop(nullptr);
  loop.SetAsCurrent();
  auto fetcher = std::make_unique<MockHttpFetcher>(
      http_response.data(), http_response.size(), nullptr);
  if (fail_http_response_code >= 0) {
    fetcher->FailTransfer(fail_http_response_code);
  }
  // This ensures the tests didn't forget to update fake_system_state_ if they
  // are not using the default request_params_.
  EXPECT_EQ(&request_params_, fake_system_state_.request_params());

  auto omaha_request_action = std::make_unique<OmahaRequestAction>(
      &fake_system_state_, nullptr, std::move(fetcher), ping_only);

  auto mock_policy_provider =
      std::make_unique<NiceMock<policy::MockPolicyProvider>>();
  EXPECT_CALL(*mock_policy_provider, IsConsumerDevice())
      .WillRepeatedly(Return(is_consumer_device));

  EXPECT_CALL(*mock_policy_provider, device_policy_is_loaded())
      .WillRepeatedly(Return(is_policy_loaded));

  const policy::MockDevicePolicy device_policy;
  const bool get_allowed_milestone_succeeds = rollback_allowed_milestones >= 0;
  EXPECT_CALL(device_policy, GetRollbackAllowedMilestones(_))
      .WillRepeatedly(DoAll(SetArgPointee<0>(rollback_allowed_milestones),
                            Return(get_allowed_milestone_succeeds)));

  EXPECT_CALL(*mock_policy_provider, GetDevicePolicy())
      .WillRepeatedly(ReturnRef(device_policy));
  omaha_request_action->policy_provider_ = std::move(mock_policy_provider);

  delegate_.expected_code_ = expected_code;
  delegate_.interactive_ = request_params_.interactive();
  delegate_.test_http_fetcher_headers_ = test_http_fetcher_headers_;
  ActionProcessor processor;
  processor.set_delegate(&delegate_);

  auto collector_action =
      std::make_unique<ObjectCollectorAction<OmahaResponse>>();
  BondActions(omaha_request_action.get(), collector_action.get());
  processor.EnqueueAction(std::move(omaha_request_action));
  processor.EnqueueAction(std::move(collector_action));

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportUpdateCheckMetrics(_, _, _, _))
      .Times(AnyNumber());

  EXPECT_CALL(*fake_system_state_.mock_metrics_reporter(),
              ReportUpdateCheckMetrics(_,
                                       expected_check_result,
                                       expected_check_reaction,
                                       expected_download_error_code))
      .Times(ping_only ? 0 : 1);

  loop.PostTask(base::Bind(
      [](ActionProcessor* processor) { processor->StartProcessing(); },
      base::Unretained(&processor)));
  loop.Run();
  EXPECT_FALSE(loop.PendingTasks());
  if (delegate_.omaha_response_ && out_response)
    *out_response = *delegate_.omaha_response_;
  if (out_post_data)
    *out_post_data = delegate_.post_data_;
  return delegate_.omaha_response_ != nullptr;
}

bool OmahaRequestActionTest::TestUpdateCheck(
    const string& http_response,
    int fail_http_response_code,
    bool ping_only,
    ErrorCode expected_code,
    metrics::CheckResult expected_check_result,
    metrics::CheckReaction expected_check_reaction,
    metrics::DownloadErrorCode expected_download_error_code,
    OmahaResponse* out_response,
    brillo::Blob* out_post_data) {
  return TestUpdateCheck(http_response,
                         fail_http_response_code,
                         ping_only,
                         true,   // is_consumer_device
                         0,      // rollback_allowed_milestones
                         false,  // is_policy_loaded
                         expected_code,
                         expected_check_result,
                         expected_check_reaction,
                         expected_download_error_code,
                         out_response,
                         out_post_data);
}

void OmahaRequestActionTest::TestRollbackCheck(bool is_consumer_device,
                                               int rollback_allowed_milestones,
                                               bool is_policy_loaded,
                                               OmahaResponse* out_response) {
  fake_update_response_.deadline = "20101020";
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              is_consumer_device,
                              rollback_allowed_milestones,
                              is_policy_loaded,
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              out_response,
                              nullptr));
  ASSERT_TRUE(out_response->update_exists);
}

// Tests Event requests -- they should always succeed. |out_post_data| may be
// null; if non-null, the post-data received by the mock HttpFetcher is
// returned.
void OmahaRequestActionTest::TestEvent(OmahaEvent* event,
                                       const string& http_response,
                                       brillo::Blob* out_post_data) {
  brillo::FakeMessageLoop loop(nullptr);
  loop.SetAsCurrent();

  auto action = std::make_unique<OmahaRequestAction>(
      &fake_system_state_,
      event,
      std::make_unique<MockHttpFetcher>(
          http_response.data(), http_response.size(), nullptr),
      false);
  ActionProcessor processor;
  processor.set_delegate(&delegate_);
  processor.EnqueueAction(std::move(action));

  loop.PostTask(base::Bind(
      [](ActionProcessor* processor) { processor->StartProcessing(); },
      base::Unretained(&processor)));
  loop.Run();
  EXPECT_FALSE(loop.PendingTasks());

  if (out_post_data)
    *out_post_data = delegate_.post_data_;
}

TEST_F(OmahaRequestActionTest, RejectEntities) {
  OmahaResponse response;
  fake_update_response_.include_entity = true;
  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLHasEntityDecl,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, NoUpdateTest) {
  OmahaResponse response;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, MultiAppNoUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_app_no_update = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, MultiAppNoPartialUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_app_no_update = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, NoSelfUpdateTest) {
  OmahaResponse response;
  ASSERT_TRUE(TestUpdateCheck(
      "<response><app><updatecheck status=\"ok\"><manifest><actions><action "
      "event=\"postinstall\" noupdate=\"true\"/></actions>"
      "</manifest></updatecheck></app></response>",
      -1,
      false,  // ping_only
      ErrorCode::kSuccess,
      metrics::CheckResult::kNoUpdateAvailable,
      metrics::CheckReaction::kUnset,
      metrics::DownloadErrorCode::kUnset,
      &response,
      nullptr));
  EXPECT_FALSE(response.update_exists);
}

// Test that all the values in the response are parsed in a normal update
// response.
TEST_F(OmahaRequestActionTest, ValidUpdateTest) {
  OmahaResponse response;
  fake_update_response_.deadline = "20101020";
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.version, response.version);
  EXPECT_EQ("", response.system_version);
  EXPECT_EQ(fake_update_response_.GetPayloadUrl(),
            response.packages[0].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.more_info_url, response.more_info_url);
  EXPECT_EQ(fake_update_response_.hash, response.packages[0].hash);
  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
  EXPECT_EQ(true, response.packages[0].is_delta);
  EXPECT_EQ(fake_update_response_.prompt == "true", response.prompt);
  EXPECT_EQ(fake_update_response_.deadline, response.deadline);
  EXPECT_FALSE(response.powerwash_required);
  // Omaha cohort attributes are not set in the response, so they should not be
  // persisted.
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsOmahaCohort));
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsOmahaCohortHint));
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsOmahaCohortName));
}

TEST_F(OmahaRequestActionTest, MultiPackageUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_package = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.version, response.version);
  EXPECT_EQ(fake_update_response_.GetPayloadUrl(),
            response.packages[0].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.codebase + "package2",
            response.packages[1].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.hash, response.packages[0].hash);
  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
  EXPECT_EQ(true, response.packages[0].is_delta);
  EXPECT_EQ(11u, response.packages[0].metadata_size);
  ASSERT_EQ(2u, response.packages.size());
  EXPECT_EQ(string("hash2"), response.packages[1].hash);
  EXPECT_EQ(222u, response.packages[1].size);
  EXPECT_EQ(22u, response.packages[1].metadata_size);
  EXPECT_EQ(false, response.packages[1].is_delta);
}

TEST_F(OmahaRequestActionTest, MultiAppUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_app = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.version, response.version);
  EXPECT_EQ(fake_update_response_.GetPayloadUrl(),
            response.packages[0].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.codebase2 + "package3",
            response.packages[1].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.hash, response.packages[0].hash);
  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
  EXPECT_EQ(11u, response.packages[0].metadata_size);
  EXPECT_EQ(true, response.packages[0].is_delta);
  ASSERT_EQ(2u, response.packages.size());
  EXPECT_EQ(string("hash3"), response.packages[1].hash);
  EXPECT_EQ(333u, response.packages[1].size);
  EXPECT_EQ(33u, response.packages[1].metadata_size);
  EXPECT_EQ(false, response.packages[1].is_delta);
}

TEST_F(OmahaRequestActionTest, MultiAppAndSystemUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_app = true;
  // trigger the lining up of the app and system versions
  request_params_.set_system_app_id(fake_update_response_.app_id2);

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.version, response.version);
  EXPECT_EQ(fake_update_response_.version2, response.system_version);
  EXPECT_EQ(fake_update_response_.GetPayloadUrl(),
            response.packages[0].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.codebase2 + "package3",
            response.packages[1].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.hash, response.packages[0].hash);
  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
  EXPECT_EQ(11u, response.packages[0].metadata_size);
  EXPECT_EQ(true, response.packages[0].is_delta);
  ASSERT_EQ(2u, response.packages.size());
  EXPECT_EQ(string("hash3"), response.packages[1].hash);
  EXPECT_EQ(333u, response.packages[1].size);
  EXPECT_EQ(33u, response.packages[1].metadata_size);
  EXPECT_EQ(false, response.packages[1].is_delta);
}

TEST_F(OmahaRequestActionTest, MultiAppPartialUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_app = true;
  fake_update_response_.multi_app_self_update = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.version, response.version);
  EXPECT_EQ("", response.system_version);
  EXPECT_EQ(fake_update_response_.GetPayloadUrl(),
            response.packages[0].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.hash, response.packages[0].hash);
  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
  EXPECT_EQ(11u, response.packages[0].metadata_size);
  ASSERT_EQ(2u, response.packages.size());
  EXPECT_EQ(string("hash3"), response.packages[1].hash);
  EXPECT_EQ(333u, response.packages[1].size);
  EXPECT_EQ(33u, response.packages[1].metadata_size);
  EXPECT_EQ(true, response.packages[1].is_delta);
}

TEST_F(OmahaRequestActionTest, MultiAppMultiPackageUpdateTest) {
  OmahaResponse response;
  fake_update_response_.multi_app = true;
  fake_update_response_.multi_package = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.version, response.version);
  EXPECT_EQ("", response.system_version);
  EXPECT_EQ(fake_update_response_.GetPayloadUrl(),
            response.packages[0].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.codebase + "package2",
            response.packages[1].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.codebase2 + "package3",
            response.packages[2].payload_urls[0]);
  EXPECT_EQ(fake_update_response_.hash, response.packages[0].hash);
  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
  EXPECT_EQ(11u, response.packages[0].metadata_size);
  EXPECT_EQ(true, response.packages[0].is_delta);
  ASSERT_EQ(3u, response.packages.size());
  EXPECT_EQ(string("hash2"), response.packages[1].hash);
  EXPECT_EQ(222u, response.packages[1].size);
  EXPECT_EQ(22u, response.packages[1].metadata_size);
  EXPECT_EQ(false, response.packages[1].is_delta);
  EXPECT_EQ(string("hash3"), response.packages[2].hash);
  EXPECT_EQ(333u, response.packages[2].size);
  EXPECT_EQ(33u, response.packages[2].metadata_size);
  EXPECT_EQ(false, response.packages[2].is_delta);
}

TEST_F(OmahaRequestActionTest, PowerwashTest) {
  OmahaResponse response;
  fake_update_response_.powerwash = true;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_TRUE(response.powerwash_required);
}

TEST_F(OmahaRequestActionTest, ExtraHeadersSentInteractiveTest) {
  OmahaResponse response;
  request_params_.set_interactive(true);
  test_http_fetcher_headers_ = true;
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ExtraHeadersSentNoInteractiveTest) {
  OmahaResponse response;
  request_params_.set_interactive(false);
  test_http_fetcher_headers_ = true;
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ValidUpdateBlockedByConnection) {
  OmahaResponse response;
  // Set up a connection manager that doesn't allow a valid update over
  // the current ethernet connection.
  MockConnectionManager mock_cm;
  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kEthernet),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsUpdateAllowedOver(ConnectionType::kEthernet, _))
      .WillRepeatedly(Return(false));

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateIgnoredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kIgnored,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ValidUpdateOverCellularAllowedByDevicePolicy) {
  // This test tests that update over cellular is allowed as device policy
  // says yes.
  OmahaResponse response;
  MockConnectionManager mock_cm;

  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kCellular),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsAllowedConnectionTypesForUpdateSet())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(mock_cm, IsUpdateAllowedOver(ConnectionType::kCellular, _))
      .WillRepeatedly(Return(true));

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ValidUpdateOverCellularBlockedByDevicePolicy) {
  // This test tests that update over cellular is blocked as device policy
  // says no.
  OmahaResponse response;
  MockConnectionManager mock_cm;

  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kCellular),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsAllowedConnectionTypesForUpdateSet())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(mock_cm, IsUpdateAllowedOver(ConnectionType::kCellular, _))
      .WillRepeatedly(Return(false));

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateIgnoredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kIgnored,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest,
       ValidUpdateOverCellularAllowedByUserPermissionTrue) {
  // This test tests that, when device policy is not set, update over cellular
  // is allowed as permission for update over cellular is set to true.
  OmahaResponse response;
  MockConnectionManager mock_cm;

  fake_prefs_.SetBoolean(kPrefsUpdateOverCellularPermission, true);
  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kCellular),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsAllowedConnectionTypesForUpdateSet())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(mock_cm, IsUpdateAllowedOver(ConnectionType::kCellular, _))
      .WillRepeatedly(Return(true));

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest,
       ValidUpdateOverCellularBlockedByUpdateTargetNotMatch) {
  // This test tests that, when device policy is not set and permission for
  // update over cellular is set to false or does not exist, update over
  // cellular is blocked as update target does not match the omaha response.
  OmahaResponse response;
  MockConnectionManager mock_cm;
  // A version different from the version in omaha response.
  string diff_version = "99.99.99";
  // A size different from the size in omaha response.
  int64_t diff_size = 999;

  fake_prefs_.SetString(kPrefsUpdateOverCellularTargetVersion, diff_version);
  fake_prefs_.SetInt64(kPrefsUpdateOverCellularTargetSize, diff_size);
  // This test tests cellular (3G) being the only connection type being allowed.
  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kCellular),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsAllowedConnectionTypesForUpdateSet())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(mock_cm, IsUpdateAllowedOver(ConnectionType::kCellular, _))
      .WillRepeatedly(Return(true));

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateIgnoredOverCellular,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kIgnored,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest,
       ValidUpdateOverCellularAllowedByUpdateTargetMatch) {
  // This test tests that, when device policy is not set and permission for
  // update over cellular is set to false or does not exist, update over
  // cellular is allowed as update target matches the omaha response.
  OmahaResponse response;
  MockConnectionManager mock_cm;
  // A version same as the version in omaha response.
  string new_version = fake_update_response_.version;
  // A size same as the size in omaha response.
  int64_t new_size = fake_update_response_.size;

  fake_prefs_.SetString(kPrefsUpdateOverCellularTargetVersion, new_version);
  fake_prefs_.SetInt64(kPrefsUpdateOverCellularTargetSize, new_size);
  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kCellular),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsAllowedConnectionTypesForUpdateSet())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(mock_cm, IsUpdateAllowedOver(ConnectionType::kCellular, _))
      .WillRepeatedly(Return(true));

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ValidUpdateBlockedByRollback) {
  string rollback_version = "1234.0.0";
  OmahaResponse response;

  MockPayloadState mock_payload_state;
  fake_system_state_.set_payload_state(&mock_payload_state);

  EXPECT_CALL(mock_payload_state, GetRollbackVersion())
      .WillRepeatedly(Return(rollback_version));

  fake_update_response_.version = rollback_version;
  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateIgnoredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kIgnored,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

// Verify that update checks called during OOBE will not try to download an
// update if the response doesn't include the deadline field.
TEST_F(OmahaRequestActionTest, SkipNonCriticalUpdatesBeforeOOBE) {
  OmahaResponse response;
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();

  // TODO(senj): set better default value for metrics::checkresult in
  // OmahaRequestAction::ActionCompleted.
  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kNonCriticalUpdateInOOBE,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

// Verify that the IsOOBEComplete() value is ignored when the OOBE flow is not
// enabled.
TEST_F(OmahaRequestActionTest, SkipNonCriticalUpdatesBeforeOOBEDisabled) {
  OmahaResponse response;
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();
  fake_system_state_.fake_hardware()->SetIsOOBEEnabled(false);

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

// Verify that update checks called during OOBE will still try to download an
// update if the response includes the deadline field.
TEST_F(OmahaRequestActionTest, SkipNonCriticalUpdatesBeforeOOBEDeadlineSet) {
  OmahaResponse response;
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();
  fake_update_response_.deadline = "20101020";

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

// Verify that update checks called during OOBE will not try to download an
// update if a rollback happened, even when the response includes the deadline
// field.
TEST_F(OmahaRequestActionTest, SkipNonCriticalUpdatesBeforeOOBERollback) {
  OmahaResponse response;
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();
  fake_update_response_.deadline = "20101020";
  EXPECT_CALL(*(fake_system_state_.mock_payload_state()), GetRollbackHappened())
      .WillOnce(Return(true));

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kNonCriticalUpdateInOOBE,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

// Verify that non-critical updates are skipped by reporting the
// kNonCriticalUpdateInOOBE error code when attempted over cellular network -
// i.e. when the update would need user permission. Note that reporting
// kOmahaUpdateIgnoredOverCellular error in this case  might cause undesired UX
// in OOBE (warning the user about an update that will be skipped).
TEST_F(OmahaRequestActionTest, SkipNonCriticalUpdatesInOOBEOverCellular) {
  OmahaResponse response;
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();

  MockConnectionManager mock_cm;
  fake_system_state_.set_connection_manager(&mock_cm);

  EXPECT_CALL(mock_cm, GetConnectionProperties(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(ConnectionType::kCellular),
                            SetArgPointee<1>(ConnectionTethering::kUnknown),
                            Return(true)));
  EXPECT_CALL(mock_cm, IsAllowedConnectionTypesForUpdateSet())
      .WillRepeatedly(Return(false));

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kNonCriticalUpdateInOOBE,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, WallClockBasedWaitAloneCausesScattering) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_update_check_count_wait_enabled(false);
  request_params_.set_waiting_period(TimeDelta::FromDays(2));

  fake_system_state_.fake_clock()->SetWallclockTime(Time::Now());

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateDeferredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kDeferring,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);

  // Verify if we are interactive check we don't defer.
  request_params_.set_interactive(true);
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, NoWallClockBasedWaitCausesNoScattering) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(false);
  request_params_.set_waiting_period(TimeDelta::FromDays(2));
  request_params_.set_update_check_count_wait_enabled(true);
  request_params_.set_min_update_checks_needed(1);
  request_params_.set_max_update_checks_allowed(8);

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ZeroMaxDaysToScatterCausesNoScattering) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta::FromDays(2));
  request_params_.set_update_check_count_wait_enabled(true);
  request_params_.set_min_update_checks_needed(1);
  request_params_.set_max_update_checks_allowed(8);

  fake_update_response_.max_days_to_scatter = "0";
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ZeroUpdateCheckCountCausesNoScattering) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta());
  request_params_.set_update_check_count_wait_enabled(true);
  request_params_.set_min_update_checks_needed(0);
  request_params_.set_max_update_checks_allowed(0);

  fake_system_state_.fake_clock()->SetWallclockTime(Time::Now());

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  int64_t count;
  ASSERT_TRUE(fake_prefs_.GetInt64(kPrefsUpdateCheckCount, &count));
  ASSERT_EQ(count, 0);
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, NonZeroUpdateCheckCountCausesScattering) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta());
  request_params_.set_update_check_count_wait_enabled(true);
  request_params_.set_min_update_checks_needed(1);
  request_params_.set_max_update_checks_allowed(8);

  fake_system_state_.fake_clock()->SetWallclockTime(Time::Now());

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateDeferredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kDeferring,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));

  int64_t count;
  ASSERT_TRUE(fake_prefs_.GetInt64(kPrefsUpdateCheckCount, &count));
  ASSERT_GT(count, 0);
  EXPECT_FALSE(response.update_exists);

  // Verify if we are interactive check we don't defer.
  request_params_.set_interactive(true);
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, ExistingUpdateCheckCountCausesScattering) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta());
  request_params_.set_update_check_count_wait_enabled(true);
  request_params_.set_min_update_checks_needed(1);
  request_params_.set_max_update_checks_allowed(8);

  fake_system_state_.fake_clock()->SetWallclockTime(Time::Now());

  ASSERT_TRUE(fake_prefs_.SetInt64(kPrefsUpdateCheckCount, 5));

  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateDeferredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kDeferring,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));

  int64_t count;
  ASSERT_TRUE(fake_prefs_.GetInt64(kPrefsUpdateCheckCount, &count));
  // count remains the same, as the decrementing happens in update_attempter
  // which this test doesn't exercise.
  ASSERT_EQ(count, 5);
  EXPECT_FALSE(response.update_exists);

  // Verify if we are interactive check we don't defer.
  request_params_.set_interactive(true);
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, StagingTurnedOnCausesScattering) {
  // If staging is on, the value for max days to scatter should be ignored, and
  // staging's scatter value should be used.
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta::FromDays(6));
  request_params_.set_update_check_count_wait_enabled(false);

  fake_system_state_.fake_clock()->SetWallclockTime(Time::Now());

  ASSERT_TRUE(fake_prefs_.SetInt64(kPrefsWallClockStagingWaitPeriod, 6));
  // This should not prevent scattering due to staging.
  fake_update_response_.max_days_to_scatter = "0";
  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateDeferredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kDeferring,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);

  // Interactive updates should not be affected.
  request_params_.set_interactive(true);
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, CohortsArePersisted) {
  OmahaResponse response;
  fake_update_response_.include_cohorts = true;
  fake_update_response_.cohort = "s/154454/8479665";
  fake_update_response_.cohorthint = "please-put-me-on-beta";
  fake_update_response_.cohortname = "stable";

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  string value;
  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohort, &value));
  EXPECT_EQ(fake_update_response_.cohort, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortHint, &value));
  EXPECT_EQ(fake_update_response_.cohorthint, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortName, &value));
  EXPECT_EQ(fake_update_response_.cohortname, value);
}

TEST_F(OmahaRequestActionTest, CohortsAreUpdated) {
  OmahaResponse response;
  EXPECT_TRUE(fake_prefs_.SetString(kPrefsOmahaCohort, "old_value"));
  EXPECT_TRUE(fake_prefs_.SetString(kPrefsOmahaCohortHint, "old_hint"));
  EXPECT_TRUE(fake_prefs_.SetString(kPrefsOmahaCohortName, "old_name"));
  fake_update_response_.include_cohorts = true;
  fake_update_response_.cohort = "s/154454/8479665";
  fake_update_response_.cohorthint = "please-put-me-on-beta";
  fake_update_response_.cohortname = "";

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  string value;
  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohort, &value));
  EXPECT_EQ(fake_update_response_.cohort, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortHint, &value));
  EXPECT_EQ(fake_update_response_.cohorthint, value);

  EXPECT_FALSE(fake_prefs_.GetString(kPrefsOmahaCohortName, &value));
}

TEST_F(OmahaRequestActionTest, CohortsAreNotModifiedWhenMissing) {
  OmahaResponse response;
  EXPECT_TRUE(fake_prefs_.SetString(kPrefsOmahaCohort, "old_value"));

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  string value;
  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohort, &value));
  EXPECT_EQ("old_value", value);

  EXPECT_FALSE(fake_prefs_.GetString(kPrefsOmahaCohortHint, &value));
  EXPECT_FALSE(fake_prefs_.GetString(kPrefsOmahaCohortName, &value));
}

TEST_F(OmahaRequestActionTest, CohortsArePersistedWhenNoUpdate) {
  OmahaResponse response;
  fake_update_response_.include_cohorts = true;
  fake_update_response_.cohort = "s/154454/8479665";
  fake_update_response_.cohorthint = "please-put-me-on-beta";
  fake_update_response_.cohortname = "stable";

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  string value;
  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohort, &value));
  EXPECT_EQ(fake_update_response_.cohort, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortHint, &value));
  EXPECT_EQ(fake_update_response_.cohorthint, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortName, &value));
  EXPECT_EQ(fake_update_response_.cohortname, value);
}

TEST_F(OmahaRequestActionTest, MultiAppCohortTest) {
  OmahaResponse response;
  fake_update_response_.multi_app = true;
  fake_update_response_.include_cohorts = true;
  fake_update_response_.cohort = "s/154454/8479665";
  fake_update_response_.cohorthint = "please-put-me-on-beta";
  fake_update_response_.cohortname = "stable";

  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  string value;
  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohort, &value));
  EXPECT_EQ(fake_update_response_.cohort, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortHint, &value));
  EXPECT_EQ(fake_update_response_.cohorthint, value);

  EXPECT_TRUE(fake_prefs_.GetString(kPrefsOmahaCohortName, &value));
  EXPECT_EQ(fake_update_response_.cohortname, value);
}

TEST_F(OmahaRequestActionTest, NoOutputPipeTest) {
  const string http_response(fake_update_response_.GetNoUpdateResponse());

  brillo::FakeMessageLoop loop(nullptr);
  loop.SetAsCurrent();

  auto action = std::make_unique<OmahaRequestAction>(
      &fake_system_state_,
      nullptr,
      std::make_unique<MockHttpFetcher>(
          http_response.data(), http_response.size(), nullptr),
      false);
  ActionProcessor processor;
  processor.set_delegate(&delegate_);
  processor.EnqueueAction(std::move(action));

  loop.PostTask(base::Bind(
      [](ActionProcessor* processor) { processor->StartProcessing(); },
      base::Unretained(&processor)));
  loop.Run();
  EXPECT_FALSE(loop.PendingTasks());
  EXPECT_FALSE(processor.IsRunning());
}

TEST_F(OmahaRequestActionTest, InvalidXmlTest) {
  OmahaResponse response;
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, EmptyResponseTest) {
  OmahaResponse response;
  ASSERT_FALSE(TestUpdateCheck("",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestEmptyResponseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, MissingStatusTest) {
  OmahaResponse response;
  ASSERT_FALSE(TestUpdateCheck(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?><response protocol=\"3.0\">"
      "<daystart elapsed_seconds=\"100\"/>"
      "<app appid=\"foo\" status=\"ok\">"
      "<ping status=\"ok\"/>"
      "<updatecheck/></app></response>",
      -1,
      false,  // ping_only
      ErrorCode::kOmahaResponseInvalid,
      metrics::CheckResult::kParsingError,
      metrics::CheckReaction::kUnset,
      metrics::DownloadErrorCode::kUnset,
      &response,
      nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, InvalidStatusTest) {
  OmahaResponse response;
  ASSERT_FALSE(TestUpdateCheck(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?><response protocol=\"3.0\">"
      "<daystart elapsed_seconds=\"100\"/>"
      "<app appid=\"foo\" status=\"ok\">"
      "<ping status=\"ok\"/>"
      "<updatecheck status=\"InvalidStatusTest\"/></app></response>",
      -1,
      false,  // ping_only
      ErrorCode::kOmahaResponseInvalid,
      metrics::CheckResult::kParsingError,
      metrics::CheckReaction::kUnset,
      metrics::DownloadErrorCode::kUnset,
      &response,
      nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, MissingNodesetTest) {
  OmahaResponse response;
  ASSERT_FALSE(TestUpdateCheck(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?><response protocol=\"3.0\">"
      "<daystart elapsed_seconds=\"100\"/>"
      "<app appid=\"foo\" status=\"ok\">"
      "<ping status=\"ok\"/>"
      "</app></response>",
      -1,
      false,  // ping_only
      ErrorCode::kOmahaResponseInvalid,
      metrics::CheckResult::kParsingError,
      metrics::CheckReaction::kUnset,
      metrics::DownloadErrorCode::kUnset,
      &response,
      nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, MissingFieldTest) {
  string input_response =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?><response protocol=\"3.0\">"
      "<daystart elapsed_seconds=\"100\"/>"
      // the appid needs to match that in the request params
      "<app appid=\"" +
      fake_update_response_.app_id +
      "\" status=\"ok\">"
      "<updatecheck status=\"ok\">"
      "<urls><url codebase=\"http://missing/field/test/\"/></urls>"
      "<manifest version=\"10.2.3.4\">"
      "<packages><package hash=\"not-used\" name=\"f\" "
      "size=\"587\" hash_sha256=\"lkq34j5345\"/></packages>"
      "<actions><action event=\"postinstall\" "
      "Prompt=\"false\" "
      "IsDeltaPayload=\"false\" "
      "sha256=\"not-used\" "
      "/></actions></manifest></updatecheck></app></response>";
  LOG(INFO) << "Input Response = " << input_response;

  OmahaResponse response;
  ASSERT_TRUE(TestUpdateCheck(input_response,
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ("10.2.3.4", response.version);
  EXPECT_EQ("http://missing/field/test/f",
            response.packages[0].payload_urls[0]);
  EXPECT_EQ("", response.more_info_url);
  EXPECT_EQ("lkq34j5345", response.packages[0].hash);
  EXPECT_EQ(587u, response.packages[0].size);
  EXPECT_FALSE(response.prompt);
  EXPECT_TRUE(response.deadline.empty());
}

namespace {
class TerminateEarlyTestProcessorDelegate : public ActionProcessorDelegate {
 public:
  void ProcessingStopped(const ActionProcessor* processor) {
    brillo::MessageLoop::current()->BreakLoop();
  }
};

void TerminateTransferTestStarter(ActionProcessor* processor) {
  processor->StartProcessing();
  CHECK(processor->IsRunning());
  processor->StopProcessing();
}
}  // namespace

TEST_F(OmahaRequestActionTest, TerminateTransferTest) {
  brillo::FakeMessageLoop loop(nullptr);
  loop.SetAsCurrent();

  string http_response("doesn't matter");
  auto action = std::make_unique<OmahaRequestAction>(
      &fake_system_state_,
      nullptr,
      std::make_unique<MockHttpFetcher>(
          http_response.data(), http_response.size(), nullptr),
      false);
  TerminateEarlyTestProcessorDelegate delegate;
  ActionProcessor processor;
  processor.set_delegate(&delegate);
  processor.EnqueueAction(std::move(action));

  loop.PostTask(base::Bind(&TerminateTransferTestStarter, &processor));
  loop.Run();
  EXPECT_FALSE(loop.PendingTasks());
}

TEST_F(OmahaRequestActionTest, XmlEncodeTest) {
  string output;
  EXPECT_TRUE(XmlEncode("ab", &output));
  EXPECT_EQ("ab", output);
  EXPECT_TRUE(XmlEncode("a<b", &output));
  EXPECT_EQ("a&lt;b", output);
  EXPECT_TRUE(XmlEncode("<&>\"\'\\", &output));
  EXPECT_EQ("&lt;&amp;&gt;&quot;&apos;\\", output);
  EXPECT_TRUE(XmlEncode("&lt;&amp;&gt;", &output));
  EXPECT_EQ("&amp;lt;&amp;amp;&amp;gt;", output);
  // Check that unterminated UTF-8 strings are handled properly.
  EXPECT_FALSE(XmlEncode("\xc2", &output));
  // Fail with invalid ASCII-7 chars.
  EXPECT_FALSE(XmlEncode("This is an 'n' with a tilde: \xc3\xb1", &output));
}

TEST_F(OmahaRequestActionTest, XmlEncodeWithDefaultTest) {
  EXPECT_EQ("&lt;&amp;&gt;", XmlEncodeWithDefault("<&>", "something else"));
  EXPECT_EQ("<not escaped>", XmlEncodeWithDefault("\xc2", "<not escaped>"));
}

TEST_F(OmahaRequestActionTest, XmlEncodeIsUsedForParams) {
  brillo::Blob post_data;

  // Make sure XML Encode is being called on the params
  request_params_.set_os_sp("testtheservice_pack>");
  request_params_.set_os_board("x86 generic<id");
  request_params_.set_current_channel("unittest_track&lt;");
  request_params_.set_target_channel("unittest_track&lt;");
  request_params_.set_hwid("<OEM MODEL>");
  fake_prefs_.SetString(kPrefsOmahaCohort, "evil\nstring");
  fake_prefs_.SetString(kPrefsOmahaCohortHint, "evil&string\\");
  fake_prefs_.SetString(
      kPrefsOmahaCohortName,
      base::JoinString(vector<string>(100, "My spoon is too big."), " "));
  OmahaResponse response;
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               &post_data));
  // convert post_data to string
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(string::npos, post_str.find("testtheservice_pack&gt;"));
  EXPECT_EQ(string::npos, post_str.find("testtheservice_pack>"));
  EXPECT_NE(string::npos, post_str.find("x86 generic&lt;id"));
  EXPECT_EQ(string::npos, post_str.find("x86 generic<id"));
  EXPECT_NE(string::npos, post_str.find("unittest_track&amp;lt;"));
  EXPECT_EQ(string::npos, post_str.find("unittest_track&lt;"));
  EXPECT_NE(string::npos, post_str.find("&lt;OEM MODEL&gt;"));
  EXPECT_EQ(string::npos, post_str.find("<OEM MODEL>"));
  EXPECT_NE(string::npos, post_str.find("cohort=\"evil\nstring\""));
  EXPECT_EQ(string::npos, post_str.find("cohorthint=\"evil&string\\\""));
  EXPECT_NE(string::npos, post_str.find("cohorthint=\"evil&amp;string\\\""));
  // Values from Prefs that are too big are removed from the XML instead of
  // encoded.
  EXPECT_EQ(string::npos, post_str.find("cohortname="));
}

TEST_F(OmahaRequestActionTest, XmlDecodeTest) {
  OmahaResponse response;
  fake_update_response_.deadline = "&lt;20110101";
  fake_update_response_.more_info_url = "testthe&lt;url";
  fake_update_response_.codebase = "testthe&amp;codebase/";
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  EXPECT_EQ("testthe<url", response.more_info_url);
  EXPECT_EQ("testthe&codebase/file.signed",
            response.packages[0].payload_urls[0]);
  EXPECT_EQ("<20110101", response.deadline);
}

TEST_F(OmahaRequestActionTest, ParseIntTest) {
  OmahaResponse response;
  // overflows int32_t:
  fake_update_response_.size = 123123123123123ull;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  EXPECT_EQ(fake_update_response_.size, response.packages[0].size);
}

TEST_F(OmahaRequestActionTest, FormatUpdateCheckOutputTest) {
  brillo::Blob post_data;
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);

  EXPECT_CALL(prefs, GetString(kPrefsPreviousVersion, _))
      .WillOnce(DoAll(SetArgPointee<1>(string("")), Return(true)));
  // An existing but empty previous version means that we didn't reboot to a new
  // update, therefore, no need to update the previous version.
  EXPECT_CALL(prefs, SetString(kPrefsPreviousVersion, _)).Times(0);
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               nullptr,  // response
                               &post_data));
  // convert post_data to string
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(
      post_str.find("        <ping active=\"1\" a=\"-1\" r=\"-1\"></ping>\n"
                    "        <updatecheck></updatecheck>\n"),
      string::npos);
  EXPECT_NE(post_str.find("hardware_class=\"OEM MODEL 09235 7471\""),
            string::npos);
  EXPECT_NE(post_str.find("fw_version=\"ChromeOSFirmware.1.0\""), string::npos);
  EXPECT_NE(post_str.find("ec_version=\"0X0A1\""), string::npos);
  // No <event> tag should be sent if we didn't reboot to an update.
  EXPECT_EQ(post_str.find("<event"), string::npos);
}

TEST_F(OmahaRequestActionTest, FormatSuccessEventOutputTest) {
  brillo::Blob post_data;
  TestEvent(new OmahaEvent(OmahaEvent::kTypeUpdateDownloadStarted),
            "invalid xml>",
            &post_data);
  // convert post_data to string
  string post_str(post_data.begin(), post_data.end());
  string expected_event = base::StringPrintf(
      "        <event eventtype=\"%d\" eventresult=\"%d\"></event>\n",
      OmahaEvent::kTypeUpdateDownloadStarted,
      OmahaEvent::kResultSuccess);
  EXPECT_NE(post_str.find(expected_event), string::npos);
  EXPECT_EQ(post_str.find("ping"), string::npos);
  EXPECT_EQ(post_str.find("updatecheck"), string::npos);
}

TEST_F(OmahaRequestActionTest, FormatErrorEventOutputTest) {
  brillo::Blob post_data;
  TestEvent(new OmahaEvent(OmahaEvent::kTypeDownloadComplete,
                           OmahaEvent::kResultError,
                           ErrorCode::kError),
            "invalid xml>",
            &post_data);
  // convert post_data to string
  string post_str(post_data.begin(), post_data.end());
  string expected_event = base::StringPrintf(
      "        <event eventtype=\"%d\" eventresult=\"%d\" "
      "errorcode=\"%d\"></event>\n",
      OmahaEvent::kTypeDownloadComplete,
      OmahaEvent::kResultError,
      static_cast<int>(ErrorCode::kError));
  EXPECT_NE(post_str.find(expected_event), string::npos);
  EXPECT_EQ(post_str.find("updatecheck"), string::npos);
}

TEST_F(OmahaRequestActionTest, IsEventTest) {
  string http_response("doesn't matter");
  OmahaRequestAction update_check_action(
      &fake_system_state_,
      nullptr,
      std::make_unique<MockHttpFetcher>(
          http_response.data(), http_response.size(), nullptr),
      false);
  EXPECT_FALSE(update_check_action.IsEvent());

  OmahaRequestAction event_action(
      &fake_system_state_,
      new OmahaEvent(OmahaEvent::kTypeUpdateComplete),
      std::make_unique<MockHttpFetcher>(
          http_response.data(), http_response.size(), nullptr),
      false);
  EXPECT_TRUE(event_action.IsEvent());
}

TEST_F(OmahaRequestActionTest, FormatDeltaOkayOutputTest) {
  for (int i = 0; i < 2; i++) {
    bool delta_okay = i == 1;
    const char* delta_okay_str = delta_okay ? "true" : "false";
    brillo::Blob post_data;

    request_params_.set_delta_okay(delta_okay);

    ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                                 -1,
                                 false,  // ping_only
                                 ErrorCode::kOmahaRequestXMLParseError,
                                 metrics::CheckResult::kParsingError,
                                 metrics::CheckReaction::kUnset,
                                 metrics::DownloadErrorCode::kUnset,
                                 nullptr,
                                 &post_data));
    // convert post_data to string
    string post_str(post_data.begin(), post_data.end());
    EXPECT_NE(
        post_str.find(base::StringPrintf(" delta_okay=\"%s\"", delta_okay_str)),
        string::npos)
        << "i = " << i;
  }
}

TEST_F(OmahaRequestActionTest, FormatInteractiveOutputTest) {
  for (int i = 0; i < 2; i++) {
    bool interactive = i == 1;
    const char* interactive_str = interactive ? "ondemandupdate" : "scheduler";
    brillo::Blob post_data;
    FakeSystemState fake_system_state;

    request_params_.set_interactive(interactive);

    ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                                 -1,
                                 false,  // ping_only
                                 ErrorCode::kOmahaRequestXMLParseError,
                                 metrics::CheckResult::kParsingError,
                                 metrics::CheckReaction::kUnset,
                                 metrics::DownloadErrorCode::kUnset,
                                 nullptr,
                                 &post_data));
    // convert post_data to string
    string post_str(post_data.begin(), post_data.end());
    EXPECT_NE(post_str.find(
                  base::StringPrintf("installsource=\"%s\"", interactive_str)),
              string::npos)
        << "i = " << i;
  }
}

TEST_F(OmahaRequestActionTest, FormatTargetVersionPrefixOutputTest) {
  for (int i = 0; i < 2; i++) {
    bool target_version_set = i == 1;
    const char* target_version_prefix = target_version_set ? "10032." : "";
    brillo::Blob post_data;
    FakeSystemState fake_system_state;

    request_params_.set_target_version_prefix(target_version_prefix);

    ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                                 -1,
                                 false,  // ping_only
                                 ErrorCode::kOmahaRequestXMLParseError,
                                 metrics::CheckResult::kParsingError,
                                 metrics::CheckReaction::kUnset,
                                 metrics::DownloadErrorCode::kUnset,
                                 nullptr,
                                 &post_data));
    // convert post_data to string
    string post_str(post_data.begin(), post_data.end());
    if (target_version_set) {
      EXPECT_NE(post_str.find("<updatecheck targetversionprefix=\"10032.\">"),
                string::npos)
          << "i = " << i;
    } else {
      EXPECT_EQ(post_str.find("targetversionprefix"), string::npos)
          << "i = " << i;
    }
  }
}

TEST_F(OmahaRequestActionTest, FormatRollbackAllowedOutputTest) {
  for (int i = 0; i < 4; i++) {
    bool rollback_allowed = i / 2 == 0;
    bool target_version_set = i % 2 == 0;
    brillo::Blob post_data;
    FakeSystemState fake_system_state;

    request_params_.set_target_version_prefix(target_version_set ? "10032."
                                                                 : "");
    request_params_.set_rollback_allowed(rollback_allowed);

    ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                                 -1,
                                 false,  // ping_only
                                 ErrorCode::kOmahaRequestXMLParseError,
                                 metrics::CheckResult::kParsingError,
                                 metrics::CheckReaction::kUnset,
                                 metrics::DownloadErrorCode::kUnset,
                                 nullptr,
                                 &post_data));
    // convert post_data to string
    string post_str(post_data.begin(), post_data.end());
    if (rollback_allowed && target_version_set) {
      EXPECT_NE(post_str.find("rollback_allowed=\"true\""), string::npos)
          << "i = " << i;
    } else {
      EXPECT_EQ(post_str.find("rollback_allowed"), string::npos) << "i = " << i;
    }
  }
}

TEST_F(OmahaRequestActionTest, OmahaEventTest) {
  OmahaEvent default_event;
  EXPECT_EQ(OmahaEvent::kTypeUnknown, default_event.type);
  EXPECT_EQ(OmahaEvent::kResultError, default_event.result);
  EXPECT_EQ(ErrorCode::kError, default_event.error_code);

  OmahaEvent success_event(OmahaEvent::kTypeUpdateDownloadStarted);
  EXPECT_EQ(OmahaEvent::kTypeUpdateDownloadStarted, success_event.type);
  EXPECT_EQ(OmahaEvent::kResultSuccess, success_event.result);
  EXPECT_EQ(ErrorCode::kSuccess, success_event.error_code);

  OmahaEvent error_event(OmahaEvent::kTypeUpdateDownloadFinished,
                         OmahaEvent::kResultError,
                         ErrorCode::kError);
  EXPECT_EQ(OmahaEvent::kTypeUpdateDownloadFinished, error_event.type);
  EXPECT_EQ(OmahaEvent::kResultError, error_event.result);
  EXPECT_EQ(ErrorCode::kError, error_event.error_code);
}

void OmahaRequestActionTest::PingTest(bool ping_only) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(kPrefsMetricsCheckLastReportingTime, _))
      .Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  // Add a few hours to the day difference to test no rounding, etc.
  int64_t five_days_ago =
      (Time::Now() - TimeDelta::FromHours(5 * 24 + 13)).ToInternalValue();
  int64_t six_days_ago =
      (Time::Now() - TimeDelta::FromHours(6 * 24 + 11)).ToInternalValue();
  EXPECT_CALL(prefs, GetInt64(kPrefsInstallDateDays, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(six_days_ago), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(five_days_ago), Return(true)));
  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              ping_only,
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(post_str.find("<ping active=\"1\" a=\"6\" r=\"5\"></ping>"),
            string::npos);
  if (ping_only) {
    EXPECT_EQ(post_str.find("updatecheck"), string::npos);
    EXPECT_EQ(post_str.find("previousversion"), string::npos);
  } else {
    EXPECT_NE(post_str.find("updatecheck"), string::npos);
    EXPECT_NE(post_str.find("previousversion"), string::npos);
  }
}

TEST_F(OmahaRequestActionTest, PingTestSendOnlyAPing) {
  PingTest(true /* ping_only */);
}

TEST_F(OmahaRequestActionTest, PingTestSendAlsoAnUpdateCheck) {
  PingTest(false /* ping_only */);
}

TEST_F(OmahaRequestActionTest, ActivePingTest) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(kPrefsMetricsCheckLastReportingTime, _))
      .Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  int64_t three_days_ago =
      (Time::Now() - TimeDelta::FromHours(3 * 24 + 12)).ToInternalValue();
  int64_t now = Time::Now().ToInternalValue();
  EXPECT_CALL(prefs, GetInt64(kPrefsInstallDateDays, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(three_days_ago), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(now), Return(true)));
  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(post_str.find("<ping active=\"1\" a=\"3\"></ping>"), string::npos);
}

TEST_F(OmahaRequestActionTest, RollCallPingTest) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(kPrefsMetricsCheckLastReportingTime, _))
      .Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  int64_t four_days_ago =
      (Time::Now() - TimeDelta::FromHours(4 * 24)).ToInternalValue();
  int64_t now = Time::Now().ToInternalValue();
  EXPECT_CALL(prefs, GetInt64(kPrefsInstallDateDays, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(now), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(four_days_ago), Return(true)));
  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(post_str.find("<ping active=\"1\" r=\"4\"></ping>\n"),
            string::npos);
}

TEST_F(OmahaRequestActionTest, NoPingTest) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(kPrefsMetricsCheckLastReportingTime, _))
      .Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  int64_t one_hour_ago =
      (Time::Now() - TimeDelta::FromHours(1)).ToInternalValue();
  EXPECT_CALL(prefs, GetInt64(kPrefsInstallDateDays, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(one_hour_ago), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(one_hour_ago), Return(true)));
  // LastActivePingDay and PrefsLastRollCallPingDay are set even if we didn't
  // send a ping.
  EXPECT_CALL(prefs, SetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(Return(true));
  EXPECT_CALL(prefs, SetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(Return(true));
  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  string post_str(post_data.begin(), post_data.end());
  EXPECT_EQ(post_str.find("ping"), string::npos);
}

TEST_F(OmahaRequestActionTest, IgnoreEmptyPingTest) {
  // This test ensures that we ignore empty ping only requests.
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  int64_t now = Time::Now().ToInternalValue();
  EXPECT_CALL(prefs, GetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(now), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(now), Return(true)));
  EXPECT_CALL(prefs, SetInt64(kPrefsLastActivePingDay, _)).Times(0);
  EXPECT_CALL(prefs, SetInt64(kPrefsLastRollCallPingDay, _)).Times(0);
  brillo::Blob post_data;
  EXPECT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              true,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUnset,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  EXPECT_EQ(0U, post_data.size());
}

TEST_F(OmahaRequestActionTest, BackInTimePingTest) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(kPrefsMetricsCheckLastReportingTime, _))
      .Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  int64_t future =
      (Time::Now() + TimeDelta::FromHours(3 * 24 + 4)).ToInternalValue();
  EXPECT_CALL(prefs, GetInt64(kPrefsInstallDateDays, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(future), Return(true)));
  EXPECT_CALL(prefs, GetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(DoAll(SetArgPointee<1>(future), Return(true)));
  EXPECT_CALL(prefs, SetInt64(kPrefsLastActivePingDay, _))
      .WillOnce(Return(true));
  EXPECT_CALL(prefs, SetInt64(kPrefsLastRollCallPingDay, _))
      .WillOnce(Return(true));
  brillo::Blob post_data;
  ASSERT_TRUE(
      TestUpdateCheck("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response "
                      "protocol=\"3.0\"><daystart elapsed_seconds=\"100\"/>"
                      "<app appid=\"foo\" status=\"ok\"><ping status=\"ok\"/>"
                      "<updatecheck status=\"noupdate\"/></app></response>",
                      -1,
                      false,  // ping_only
                      ErrorCode::kSuccess,
                      metrics::CheckResult::kNoUpdateAvailable,
                      metrics::CheckReaction::kUnset,
                      metrics::DownloadErrorCode::kUnset,
                      nullptr,
                      &post_data));
  string post_str(post_data.begin(), post_data.end());
  EXPECT_EQ(post_str.find("ping"), string::npos);
}

TEST_F(OmahaRequestActionTest, LastPingDayUpdateTest) {
  // This test checks that the action updates the last ping day to now
  // minus 200 seconds with a slack of 5 seconds. Therefore, the test
  // may fail if it runs for longer than 5 seconds. It shouldn't run
  // that long though.
  int64_t midnight =
      (Time::Now() - TimeDelta::FromSeconds(200)).ToInternalValue();
  int64_t midnight_slack =
      (Time::Now() - TimeDelta::FromSeconds(195)).ToInternalValue();
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(_, _)).Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  EXPECT_CALL(prefs,
              SetInt64(kPrefsLastActivePingDay,
                       AllOf(Ge(midnight), Le(midnight_slack))))
      .WillOnce(Return(true));
  EXPECT_CALL(prefs,
              SetInt64(kPrefsLastRollCallPingDay,
                       AllOf(Ge(midnight), Le(midnight_slack))))
      .WillOnce(Return(true));
  ASSERT_TRUE(
      TestUpdateCheck("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response "
                      "protocol=\"3.0\"><daystart elapsed_seconds=\"200\"/>"
                      "<app appid=\"foo\" status=\"ok\"><ping status=\"ok\"/>"
                      "<updatecheck status=\"noupdate\"/></app></response>",
                      -1,
                      false,  // ping_only
                      ErrorCode::kSuccess,
                      metrics::CheckResult::kNoUpdateAvailable,
                      metrics::CheckReaction::kUnset,
                      metrics::DownloadErrorCode::kUnset,
                      nullptr,
                      nullptr));
}

TEST_F(OmahaRequestActionTest, NoElapsedSecondsTest) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(_, _)).Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(kPrefsLastActivePingDay, _)).Times(0);
  EXPECT_CALL(prefs, SetInt64(kPrefsLastRollCallPingDay, _)).Times(0);
  ASSERT_TRUE(
      TestUpdateCheck("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response "
                      "protocol=\"3.0\"><daystart blah=\"200\"/>"
                      "<app appid=\"foo\" status=\"ok\"><ping status=\"ok\"/>"
                      "<updatecheck status=\"noupdate\"/></app></response>",
                      -1,
                      false,  // ping_only
                      ErrorCode::kSuccess,
                      metrics::CheckResult::kNoUpdateAvailable,
                      metrics::CheckReaction::kUnset,
                      metrics::DownloadErrorCode::kUnset,
                      nullptr,
                      nullptr));
}

TEST_F(OmahaRequestActionTest, BadElapsedSecondsTest) {
  NiceMock<MockPrefs> prefs;
  fake_system_state_.set_prefs(&prefs);
  EXPECT_CALL(prefs, GetInt64(_, _)).Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(_, _)).Times(AnyNumber());
  EXPECT_CALL(prefs, SetInt64(kPrefsLastActivePingDay, _)).Times(0);
  EXPECT_CALL(prefs, SetInt64(kPrefsLastRollCallPingDay, _)).Times(0);
  ASSERT_TRUE(
      TestUpdateCheck("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response "
                      "protocol=\"3.0\"><daystart elapsed_seconds=\"x\"/>"
                      "<app appid=\"foo\" status=\"ok\"><ping status=\"ok\"/>"
                      "<updatecheck status=\"noupdate\"/></app></response>",
                      -1,
                      false,  // ping_only
                      ErrorCode::kSuccess,
                      metrics::CheckResult::kNoUpdateAvailable,
                      metrics::CheckReaction::kUnset,
                      metrics::DownloadErrorCode::kUnset,
                      nullptr,
                      nullptr));
}

TEST_F(OmahaRequestActionTest, ParseUpdateCheckAttributesTest) {
  // Test that the "eol" flags is only parsed from the "_eol" attribute and not
  // the "eol" attribute.
  ASSERT_TRUE(
      TestUpdateCheck("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response "
                      "protocol=\"3.0\"><app appid=\"foo\" status=\"ok\">"
                      "<ping status=\"ok\"/><updatecheck status=\"noupdate\" "
                      "_eol=\"security-only\" eol=\"eol\" _foo=\"bar\"/>"
                      "</app></response>",
                      -1,
                      false,  // ping_only
                      ErrorCode::kSuccess,
                      metrics::CheckResult::kNoUpdateAvailable,
                      metrics::CheckReaction::kUnset,
                      metrics::DownloadErrorCode::kUnset,
                      nullptr,
                      nullptr));
  string eol_pref;
  EXPECT_TRUE(
      fake_system_state_.prefs()->GetString(kPrefsOmahaEolStatus, &eol_pref));
  // Note that the eol="eol" attribute should be ignored and the _eol should be
  // used instead.
  EXPECT_EQ("security-only", eol_pref);
}

TEST_F(OmahaRequestActionTest, NoUniqueIDTest) {
  brillo::Blob post_data;
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               nullptr,  // response
                               &post_data));
  // convert post_data to string
  string post_str(post_data.begin(), post_data.end());
  EXPECT_EQ(post_str.find("machineid="), string::npos);
  EXPECT_EQ(post_str.find("userid="), string::npos);
}

TEST_F(OmahaRequestActionTest, NetworkFailureTest) {
  OmahaResponse response;
  const int http_error_code =
      static_cast<int>(ErrorCode::kOmahaRequestHTTPResponseBase) + 501;
  ASSERT_FALSE(TestUpdateCheck("",
                               501,
                               false,  // ping_only
                               static_cast<ErrorCode>(http_error_code),
                               metrics::CheckResult::kDownloadError,
                               metrics::CheckReaction::kUnset,
                               static_cast<metrics::DownloadErrorCode>(501),
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, NetworkFailureBadHTTPCodeTest) {
  OmahaResponse response;
  const int http_error_code =
      static_cast<int>(ErrorCode::kOmahaRequestHTTPResponseBase) + 999;
  ASSERT_FALSE(TestUpdateCheck("",
                               1500,
                               false,  // ping_only
                               static_cast<ErrorCode>(http_error_code),
                               metrics::CheckResult::kDownloadError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kHttpStatusOther,
                               &response,
                               nullptr));
  EXPECT_FALSE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, TestUpdateFirstSeenAtGetsPersistedFirstTime) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta().FromDays(1));
  request_params_.set_update_check_count_wait_enabled(false);

  Time arbitrary_date;
  ASSERT_TRUE(Time::FromString("6/4/1989", &arbitrary_date));
  fake_system_state_.fake_clock()->SetWallclockTime(arbitrary_date);
  ASSERT_FALSE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaUpdateDeferredPerPolicy,
                               metrics::CheckResult::kUpdateAvailable,
                               metrics::CheckReaction::kDeferring,
                               metrics::DownloadErrorCode::kUnset,
                               &response,
                               nullptr));

  int64_t timestamp = 0;
  ASSERT_TRUE(fake_prefs_.GetInt64(kPrefsUpdateFirstSeenAt, &timestamp));
  EXPECT_EQ(arbitrary_date.ToInternalValue(), timestamp);
  EXPECT_FALSE(response.update_exists);

  // Verify if we are interactive check we don't defer.
  request_params_.set_interactive(true);
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
}

TEST_F(OmahaRequestActionTest, TestUpdateFirstSeenAtGetsUsedIfAlreadyPresent) {
  OmahaResponse response;
  request_params_.set_wall_clock_based_wait_enabled(true);
  request_params_.set_waiting_period(TimeDelta().FromDays(1));
  request_params_.set_update_check_count_wait_enabled(false);

  Time t1, t2;
  ASSERT_TRUE(Time::FromString("1/1/2012", &t1));
  ASSERT_TRUE(Time::FromString("1/3/2012", &t2));
  ASSERT_TRUE(
      fake_prefs_.SetInt64(kPrefsUpdateFirstSeenAt, t1.ToInternalValue()));
  fake_system_state_.fake_clock()->SetWallclockTime(t2);
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));

  EXPECT_TRUE(response.update_exists);

  // Make sure the timestamp t1 is unchanged showing that it was reused.
  int64_t timestamp = 0;
  ASSERT_TRUE(fake_prefs_.GetInt64(kPrefsUpdateFirstSeenAt, &timestamp));
  ASSERT_TRUE(timestamp == t1.ToInternalValue());
}

TEST_F(OmahaRequestActionTest, TestChangingToMoreStableChannel) {
  // Create a uniquely named test directory.
  base::ScopedTempDir tempdir;
  ASSERT_TRUE(tempdir.CreateUniqueTempDir());

  brillo::Blob post_data;
  request_params_.set_root(tempdir.GetPath().value());
  request_params_.set_app_id("{22222222-2222-2222-2222-222222222222}");
  request_params_.set_app_version("1.2.3.4");
  request_params_.set_product_components("o.bundle=1");
  request_params_.set_current_channel("canary-channel");
  EXPECT_TRUE(
      request_params_.SetTargetChannel("stable-channel", true, nullptr));
  request_params_.UpdateDownloadChannel();
  EXPECT_TRUE(request_params_.ShouldPowerwash());
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               nullptr,  // response
                               &post_data));
  // convert post_data to string
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(
      string::npos,
      post_str.find("appid=\"{22222222-2222-2222-2222-222222222222}\" "
                    "version=\"0.0.0.0\" from_version=\"1.2.3.4\" "
                    "track=\"stable-channel\" from_track=\"canary-channel\" "));
  EXPECT_EQ(string::npos, post_str.find("o.bundle"));
}

TEST_F(OmahaRequestActionTest, TestChangingToLessStableChannel) {
  // Create a uniquely named test directory.
  base::ScopedTempDir tempdir;
  ASSERT_TRUE(tempdir.CreateUniqueTempDir());

  brillo::Blob post_data;
  request_params_.set_root(tempdir.GetPath().value());
  request_params_.set_app_id("{11111111-1111-1111-1111-111111111111}");
  request_params_.set_app_version("5.6.7.8");
  request_params_.set_product_components("o.bundle=1");
  request_params_.set_current_channel("stable-channel");
  EXPECT_TRUE(
      request_params_.SetTargetChannel("canary-channel", false, nullptr));
  request_params_.UpdateDownloadChannel();
  EXPECT_FALSE(request_params_.ShouldPowerwash());
  ASSERT_FALSE(TestUpdateCheck("invalid xml>",
                               -1,
                               false,  // ping_only
                               ErrorCode::kOmahaRequestXMLParseError,
                               metrics::CheckResult::kParsingError,
                               metrics::CheckReaction::kUnset,
                               metrics::DownloadErrorCode::kUnset,
                               nullptr,  // response
                               &post_data));
  // Convert post_data to string.
  string post_str(post_data.begin(), post_data.end());
  EXPECT_NE(
      string::npos,
      post_str.find("appid=\"{11111111-1111-1111-1111-111111111111}\" "
                    "version=\"5.6.7.8\" "
                    "track=\"canary-channel\" from_track=\"stable-channel\""));
  EXPECT_EQ(string::npos, post_str.find("from_version"));
  EXPECT_NE(string::npos, post_str.find("o.bundle.version=\"1\""));
}

// Checks that the initial ping with a=-1 r=-1 is not send when the device
// was powerwashed.
TEST_F(OmahaRequestActionTest, PingWhenPowerwashed) {
  fake_prefs_.SetString(kPrefsPreviousVersion, "");

  // Flag that the device was powerwashed in the past.
  fake_system_state_.fake_hardware()->SetPowerwashCount(1);

  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  // We shouldn't send a ping in this case since powerwash > 0.
  string post_str(post_data.begin(), post_data.end());
  EXPECT_EQ(string::npos, post_str.find("<ping"));
}

// Checks that the initial ping with a=-1 r=-1 is not send when the device
// first_active_omaha_ping_sent is set.
TEST_F(OmahaRequestActionTest, PingWhenFirstActiveOmahaPingIsSent) {
  fake_prefs_.SetString(kPrefsPreviousVersion, "");

  // Flag that the device was not powerwashed in the past.
  fake_system_state_.fake_hardware()->SetPowerwashCount(0);

  // Flag that the device has sent first active ping in the past.
  fake_system_state_.fake_hardware()->SetFirstActiveOmahaPingSent();

  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  // We shouldn't send a ping in this case since
  // first_active_omaha_ping_sent=true
  string post_str(post_data.begin(), post_data.end());
  EXPECT_EQ(string::npos, post_str.find("<ping"));
}

// Checks that the event 54 is sent on a reboot to a new update.
TEST_F(OmahaRequestActionTest, RebootAfterUpdateEvent) {
  // Flag that the device was updated in a previous boot.
  fake_prefs_.SetString(kPrefsPreviousVersion, "1.2.3.4");

  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              nullptr,
                              &post_data));
  string post_str(post_data.begin(), post_data.end());

  // An event 54 is included and has the right version.
  EXPECT_NE(
      string::npos,
      post_str.find(base::StringPrintf("<event eventtype=\"%d\"",
                                       OmahaEvent::kTypeRebootedAfterUpdate)));
  EXPECT_NE(string::npos,
            post_str.find("previousversion=\"1.2.3.4\"></event>"));

  // The previous version flag should have been removed.
  EXPECT_TRUE(fake_prefs_.Exists(kPrefsPreviousVersion));
  string prev_version;
  EXPECT_TRUE(fake_prefs_.GetString(kPrefsPreviousVersion, &prev_version));
  EXPECT_TRUE(prev_version.empty());
}

void OmahaRequestActionTest::P2PTest(bool initial_allow_p2p_for_downloading,
                                     bool initial_allow_p2p_for_sharing,
                                     bool omaha_disable_p2p_for_downloading,
                                     bool omaha_disable_p2p_for_sharing,
                                     bool payload_state_allow_p2p_attempt,
                                     bool expect_p2p_client_lookup,
                                     const string& p2p_client_result_url,
                                     bool expected_allow_p2p_for_downloading,
                                     bool expected_allow_p2p_for_sharing,
                                     const string& expected_p2p_url) {
  OmahaResponse response;
  bool actual_allow_p2p_for_downloading = initial_allow_p2p_for_downloading;
  bool actual_allow_p2p_for_sharing = initial_allow_p2p_for_sharing;
  string actual_p2p_url;

  MockPayloadState mock_payload_state;
  fake_system_state_.set_payload_state(&mock_payload_state);
  EXPECT_CALL(mock_payload_state, P2PAttemptAllowed())
      .WillRepeatedly(Return(payload_state_allow_p2p_attempt));
  EXPECT_CALL(mock_payload_state, GetUsingP2PForDownloading())
      .WillRepeatedly(ReturnPointee(&actual_allow_p2p_for_downloading));
  EXPECT_CALL(mock_payload_state, GetUsingP2PForSharing())
      .WillRepeatedly(ReturnPointee(&actual_allow_p2p_for_sharing));
  EXPECT_CALL(mock_payload_state, SetUsingP2PForDownloading(_))
      .WillRepeatedly(SaveArg<0>(&actual_allow_p2p_for_downloading));
  EXPECT_CALL(mock_payload_state, SetUsingP2PForSharing(_))
      .WillRepeatedly(SaveArg<0>(&actual_allow_p2p_for_sharing));
  EXPECT_CALL(mock_payload_state, SetP2PUrl(_))
      .WillRepeatedly(SaveArg<0>(&actual_p2p_url));

  MockP2PManager mock_p2p_manager;
  fake_system_state_.set_p2p_manager(&mock_p2p_manager);
  mock_p2p_manager.fake().SetLookupUrlForFileResult(p2p_client_result_url);

  TimeDelta timeout = TimeDelta::FromSeconds(kMaxP2PNetworkWaitTimeSeconds);
  EXPECT_CALL(mock_p2p_manager, LookupUrlForFile(_, _, timeout, _))
      .Times(expect_p2p_client_lookup ? 1 : 0);

  fake_update_response_.disable_p2p_for_downloading =
      omaha_disable_p2p_for_downloading;
  fake_update_response_.disable_p2p_for_sharing = omaha_disable_p2p_for_sharing;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);

  EXPECT_EQ(omaha_disable_p2p_for_downloading,
            response.disable_p2p_for_downloading);
  EXPECT_EQ(omaha_disable_p2p_for_sharing, response.disable_p2p_for_sharing);

  EXPECT_EQ(expected_allow_p2p_for_downloading,
            actual_allow_p2p_for_downloading);
  EXPECT_EQ(expected_allow_p2p_for_sharing, actual_allow_p2p_for_sharing);
  EXPECT_EQ(expected_p2p_url, actual_p2p_url);
}

TEST_F(OmahaRequestActionTest, P2PWithPeer) {
  P2PTest(true,                   // initial_allow_p2p_for_downloading
          true,                   // initial_allow_p2p_for_sharing
          false,                  // omaha_disable_p2p_for_downloading
          false,                  // omaha_disable_p2p_for_sharing
          true,                   // payload_state_allow_p2p_attempt
          true,                   // expect_p2p_client_lookup
          "http://1.3.5.7/p2p",   // p2p_client_result_url
          true,                   // expected_allow_p2p_for_downloading
          true,                   // expected_allow_p2p_for_sharing
          "http://1.3.5.7/p2p");  // expected_p2p_url
}

TEST_F(OmahaRequestActionTest, P2PWithoutPeer) {
  P2PTest(true,   // initial_allow_p2p_for_downloading
          true,   // initial_allow_p2p_for_sharing
          false,  // omaha_disable_p2p_for_downloading
          false,  // omaha_disable_p2p_for_sharing
          true,   // payload_state_allow_p2p_attempt
          true,   // expect_p2p_client_lookup
          "",     // p2p_client_result_url
          false,  // expected_allow_p2p_for_downloading
          true,   // expected_allow_p2p_for_sharing
          "");    // expected_p2p_url
}

TEST_F(OmahaRequestActionTest, P2PDownloadNotAllowed) {
  P2PTest(false,    // initial_allow_p2p_for_downloading
          true,     // initial_allow_p2p_for_sharing
          false,    // omaha_disable_p2p_for_downloading
          false,    // omaha_disable_p2p_for_sharing
          true,     // payload_state_allow_p2p_attempt
          false,    // expect_p2p_client_lookup
          "unset",  // p2p_client_result_url
          false,    // expected_allow_p2p_for_downloading
          true,     // expected_allow_p2p_for_sharing
          "");      // expected_p2p_url
}

TEST_F(OmahaRequestActionTest, P2PWithPeerDownloadDisabledByOmaha) {
  P2PTest(true,     // initial_allow_p2p_for_downloading
          true,     // initial_allow_p2p_for_sharing
          true,     // omaha_disable_p2p_for_downloading
          false,    // omaha_disable_p2p_for_sharing
          true,     // payload_state_allow_p2p_attempt
          false,    // expect_p2p_client_lookup
          "unset",  // p2p_client_result_url
          false,    // expected_allow_p2p_for_downloading
          true,     // expected_allow_p2p_for_sharing
          "");      // expected_p2p_url
}

TEST_F(OmahaRequestActionTest, P2PWithPeerSharingDisabledByOmaha) {
  P2PTest(true,                   // initial_allow_p2p_for_downloading
          true,                   // initial_allow_p2p_for_sharing
          false,                  // omaha_disable_p2p_for_downloading
          true,                   // omaha_disable_p2p_for_sharing
          true,                   // payload_state_allow_p2p_attempt
          true,                   // expect_p2p_client_lookup
          "http://1.3.5.7/p2p",   // p2p_client_result_url
          true,                   // expected_allow_p2p_for_downloading
          false,                  // expected_allow_p2p_for_sharing
          "http://1.3.5.7/p2p");  // expected_p2p_url
}

TEST_F(OmahaRequestActionTest, P2PWithPeerBothDisabledByOmaha) {
  P2PTest(true,     // initial_allow_p2p_for_downloading
          true,     // initial_allow_p2p_for_sharing
          true,     // omaha_disable_p2p_for_downloading
          true,     // omaha_disable_p2p_for_sharing
          true,     // payload_state_allow_p2p_attempt
          false,    // expect_p2p_client_lookup
          "unset",  // p2p_client_result_url
          false,    // expected_allow_p2p_for_downloading
          false,    // expected_allow_p2p_for_sharing
          "");      // expected_p2p_url
}

bool OmahaRequestActionTest::InstallDateParseHelper(const string& elapsed_days,
                                                    OmahaResponse* response) {
  fake_update_response_.elapsed_days = elapsed_days;
  return TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                         -1,
                         false,  // ping_only
                         ErrorCode::kSuccess,
                         metrics::CheckResult::kUpdateAvailable,
                         metrics::CheckReaction::kUpdating,
                         metrics::DownloadErrorCode::kUnset,
                         response,
                         nullptr);
}

TEST_F(OmahaRequestActionTest, ParseInstallDateFromResponse) {
  OmahaResponse response;

  // Simulate a successful update check that happens during OOBE.  The
  // deadline in the response is needed to force the update attempt to
  // occur; responses without a deadline seen during OOBE will normally
  // return ErrorCode::kNonCriticalUpdateInOOBE.
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();
  fake_update_response_.deadline = "20101020";

  // Check that we parse elapsed_days in the Omaha Response correctly.
  // and that the kPrefsInstallDateDays value is written to.
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsInstallDateDays));
  EXPECT_TRUE(InstallDateParseHelper("42", &response));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(42, response.install_date_days);
  EXPECT_TRUE(fake_prefs_.Exists(kPrefsInstallDateDays));
  int64_t prefs_days;
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsInstallDateDays, &prefs_days));
  EXPECT_EQ(prefs_days, 42);

  // If there already is a value set, we shouldn't do anything.
  EXPECT_TRUE(InstallDateParseHelper("7", &response));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(7, response.install_date_days);
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsInstallDateDays, &prefs_days));
  EXPECT_EQ(prefs_days, 42);

  // Note that elapsed_days is not necessarily divisible by 7 so check
  // that we round down correctly when populating kPrefsInstallDateDays.
  EXPECT_TRUE(fake_prefs_.Delete(kPrefsInstallDateDays));
  EXPECT_TRUE(InstallDateParseHelper("23", &response));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(23, response.install_date_days);
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsInstallDateDays, &prefs_days));
  EXPECT_EQ(prefs_days, 21);

  // Check that we correctly handle elapsed_days not being included in
  // the Omaha Response.
  EXPECT_TRUE(InstallDateParseHelper("", &response));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(-1, response.install_date_days);
}

// If there is no prefs and OOBE is not complete, we should not
// report anything to Omaha.
TEST_F(OmahaRequestActionTest, GetInstallDateWhenNoPrefsNorOOBE) {
  fake_system_state_.fake_hardware()->UnsetIsOOBEComplete();
  EXPECT_EQ(OmahaRequestAction::GetInstallDate(&fake_system_state_), -1);
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsInstallDateDays));
}

// If OOBE is complete and happened on a valid date (e.g. after Jan
// 1 2007 0:00 PST), that date should be used and written to
// prefs. However, first try with an invalid date and check we do
// nothing.
TEST_F(OmahaRequestActionTest, GetInstallDateWhenOOBECompletedWithInvalidDate) {
  Time oobe_date = Time::FromTimeT(42);  // Dec 31, 1969 16:00:42 PST.
  fake_system_state_.fake_hardware()->SetIsOOBEComplete(oobe_date);
  EXPECT_EQ(OmahaRequestAction::GetInstallDate(&fake_system_state_), -1);
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsInstallDateDays));
}

// Then check with a valid date. The date Jan 20, 2007 0:00 PST
// should yield an InstallDate of 14.
TEST_F(OmahaRequestActionTest, GetInstallDateWhenOOBECompletedWithValidDate) {
  Time oobe_date = Time::FromTimeT(1169280000);  // Jan 20, 2007 0:00 PST.
  fake_system_state_.fake_hardware()->SetIsOOBEComplete(oobe_date);
  EXPECT_EQ(OmahaRequestAction::GetInstallDate(&fake_system_state_), 14);
  EXPECT_TRUE(fake_prefs_.Exists(kPrefsInstallDateDays));

  int64_t prefs_days;
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsInstallDateDays, &prefs_days));
  EXPECT_EQ(prefs_days, 14);
}

// Now that we have a valid date in prefs, check that we keep using
// that even if OOBE date reports something else. The date Jan 30,
// 2007 0:00 PST should yield an InstallDate of 28... but since
// there's a prefs file, we should still get 14.
TEST_F(OmahaRequestActionTest, GetInstallDateWhenOOBECompletedDateChanges) {
  // Set a valid date in the prefs first.
  EXPECT_TRUE(fake_prefs_.SetInt64(kPrefsInstallDateDays, 14));

  Time oobe_date = Time::FromTimeT(1170144000);  // Jan 30, 2007 0:00 PST.
  fake_system_state_.fake_hardware()->SetIsOOBEComplete(oobe_date);
  EXPECT_EQ(OmahaRequestAction::GetInstallDate(&fake_system_state_), 14);

  int64_t prefs_days;
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsInstallDateDays, &prefs_days));
  EXPECT_EQ(prefs_days, 14);

  // If we delete the prefs file, we should get 28 days.
  EXPECT_TRUE(fake_prefs_.Delete(kPrefsInstallDateDays));
  EXPECT_EQ(OmahaRequestAction::GetInstallDate(&fake_system_state_), 28);
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsInstallDateDays, &prefs_days));
  EXPECT_EQ(prefs_days, 28);
}

// Verifies that a device with no device policy, and is not a consumer
// device sets the max kernel key version to the current version.
// ie. the same behavior as if rollback is enabled.
TEST_F(OmahaRequestActionTest, NoPolicyEnterpriseDevicesSetMaxRollback) {
  FakeHardware* fake_hw = fake_system_state_.fake_hardware();

  // Setup and verify some initial default values for the kernel TPM
  // values that control verified boot and rollback.
  const int min_kernel_version = 4;
  fake_hw->SetMinKernelKeyVersion(min_kernel_version);
  fake_hw->SetMaxKernelKeyRollforward(kRollforwardInfinity);
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(kRollforwardInfinity, fake_hw->GetMaxKernelKeyRollforward());

  EXPECT_CALL(
      *fake_system_state_.mock_metrics_reporter(),
      ReportKeyVersionMetrics(min_kernel_version, min_kernel_version, true))
      .Times(1);

  OmahaResponse response;
  TestRollbackCheck(false /* is_consumer_device */,
                    3 /* rollback_allowed_milestones */,
                    false /* is_policy_loaded */,
                    &response);

  // Verify kernel_max_rollforward was set to the current minimum
  // kernel key version. This has the effect of freezing roll
  // forwards indefinitely. This will hold the rollback window
  // open until a future change will be able to move this forward
  // relative the configured window.
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(min_kernel_version, fake_hw->GetMaxKernelKeyRollforward());
}

// Verifies that a conmsumer device with no device policy sets the
// max kernel key version to the current version. ie. the same
// behavior as if rollback is enabled.
TEST_F(OmahaRequestActionTest, NoPolicyConsumerDevicesSetMaxRollback) {
  FakeHardware* fake_hw = fake_system_state_.fake_hardware();

  // Setup and verify some initial default values for the kernel TPM
  // values that control verified boot and rollback.
  const int min_kernel_version = 3;
  fake_hw->SetMinKernelKeyVersion(min_kernel_version);
  fake_hw->SetMaxKernelKeyRollforward(kRollforwardInfinity);
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(kRollforwardInfinity, fake_hw->GetMaxKernelKeyRollforward());

  EXPECT_CALL(
      *fake_system_state_.mock_metrics_reporter(),
      ReportKeyVersionMetrics(min_kernel_version, kRollforwardInfinity, true))
      .Times(1);

  OmahaResponse response;
  TestRollbackCheck(true /* is_consumer_device */,
                    3 /* rollback_allowed_milestones */,
                    false /* is_policy_loaded */,
                    &response);

  // Verify that with rollback disabled that kernel_max_rollforward
  // was set to logical infinity. This is the expected behavior for
  // consumer devices and matches the existing behavior prior to the
  // rollback features.
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(kRollforwardInfinity, fake_hw->GetMaxKernelKeyRollforward());
}

// Verifies that a device with rollback enabled sets kernel_max_rollforward
// in the TPM to prevent roll forward.
TEST_F(OmahaRequestActionTest, RollbackEnabledDevicesSetMaxRollback) {
  FakeHardware* fake_hw = fake_system_state_.fake_hardware();

  // Setup and verify some initial default values for the kernel TPM
  // values that control verified boot and rollback.
  const int allowed_milestones = 4;
  const int min_kernel_version = 3;
  fake_hw->SetMinKernelKeyVersion(min_kernel_version);
  fake_hw->SetMaxKernelKeyRollforward(kRollforwardInfinity);
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(kRollforwardInfinity, fake_hw->GetMaxKernelKeyRollforward());

  EXPECT_CALL(
      *fake_system_state_.mock_metrics_reporter(),
      ReportKeyVersionMetrics(min_kernel_version, min_kernel_version, true))
      .Times(1);

  OmahaResponse response;
  TestRollbackCheck(false /* is_consumer_device */,
                    allowed_milestones,
                    true /* is_policy_loaded */,
                    &response);

  // Verify that with rollback enabled that kernel_max_rollforward
  // was set to the current minimum kernel key version. This has
  // the effect of freezing roll forwards indefinitely. This will
  // hold the rollback window open until a future change will
  // be able to move this forward relative the configured window.
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(min_kernel_version, fake_hw->GetMaxKernelKeyRollforward());
}

// Verifies that a device with rollback disabled sets kernel_max_rollforward
// in the TPM to logical infinity, to allow roll forward.
TEST_F(OmahaRequestActionTest, RollbackDisabledDevicesSetMaxRollback) {
  FakeHardware* fake_hw = fake_system_state_.fake_hardware();

  // Setup and verify some initial default values for the kernel TPM
  // values that control verified boot and rollback.
  const int allowed_milestones = 0;
  const int min_kernel_version = 3;
  fake_hw->SetMinKernelKeyVersion(min_kernel_version);
  fake_hw->SetMaxKernelKeyRollforward(kRollforwardInfinity);
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(kRollforwardInfinity, fake_hw->GetMaxKernelKeyRollforward());

  EXPECT_CALL(
      *fake_system_state_.mock_metrics_reporter(),
      ReportKeyVersionMetrics(min_kernel_version, kRollforwardInfinity, true))
      .Times(1);

  OmahaResponse response;
  TestRollbackCheck(false /* is_consumer_device */,
                    allowed_milestones,
                    true /* is_policy_loaded */,
                    &response);

  // Verify that with rollback disabled that kernel_max_rollforward
  // was set to logical infinity.
  EXPECT_EQ(min_kernel_version, fake_hw->GetMinKernelKeyVersion());
  EXPECT_EQ(kRollforwardInfinity, fake_hw->GetMaxKernelKeyRollforward());
}

TEST_F(OmahaRequestActionTest, RollbackResponseParsedNoEntries) {
  OmahaResponse response;
  fake_update_response_.rollback = true;
  TestRollbackCheck(false /* is_consumer_device */,
                    4 /* rollback_allowed_milestones */,
                    true /* is_policy_loaded */,
                    &response);
  EXPECT_TRUE(response.is_rollback);
}

TEST_F(OmahaRequestActionTest, RollbackResponseValidVersionsParsed) {
  OmahaResponse response;
  fake_update_response_.rollback_firmware_version = "1.2";
  fake_update_response_.rollback_kernel_version = "3.4";
  fake_update_response_.rollback = true;
  TestRollbackCheck(false /* is_consumer_device */,
                    4 /* rollback_allowed_milestones */,
                    true /* is_policy_loaded */,
                    &response);
  EXPECT_TRUE(response.is_rollback);
  EXPECT_EQ(1, response.rollback_key_version.firmware_key);
  EXPECT_EQ(2, response.rollback_key_version.firmware);
  EXPECT_EQ(3, response.rollback_key_version.kernel_key);
  EXPECT_EQ(4, response.rollback_key_version.kernel);
}

TEST_F(OmahaRequestActionTest,
       TestUpdateFirstSeenAtPrefPersistedIfUpdateExists) {
  FakeClock fake_clock;
  Time now = Time::Now();
  fake_clock.SetWallclockTime(now);
  fake_system_state_.set_clock(&fake_clock);

  OmahaResponse response;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_TRUE(fake_prefs_.Exists(kPrefsUpdateFirstSeenAt));

  int64_t stored_first_seen_at_time;
  EXPECT_TRUE(fake_prefs_.GetInt64(kPrefsUpdateFirstSeenAt,
                                   &stored_first_seen_at_time));
  EXPECT_EQ(now.ToInternalValue(), stored_first_seen_at_time);
}

TEST_F(OmahaRequestActionTest,
       TestUpdateFirstSeenAtPrefNotPersistedIfUpdateFails) {
  FakeClock fake_clock;
  Time now = Time::Now();
  fake_clock.SetWallclockTime(now);
  fake_system_state_.set_clock(&fake_clock);

  OmahaResponse response;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetNoUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kNoUpdateAvailable,
                              metrics::CheckReaction::kUnset,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_FALSE(response.update_exists);
  EXPECT_FALSE(fake_prefs_.Exists(kPrefsUpdateFirstSeenAt));
}

TEST_F(OmahaRequestActionTest, InstallTest) {
  OmahaResponse response;
  request_params_.set_is_install(true);
  request_params_.set_dlc_module_ids({"dlc_no_0", "dlc_no_1"});
  brillo::Blob post_data;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              true,   // is_consumer_device
                              0,      // rollback_allowed_milestones
                              false,  // is_policy_loaded
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              &post_data));
  // Convert post_data to string.
  string post_str(post_data.begin(), post_data.end());
  for (const auto& dlc_module_id : request_params_.dlc_module_ids()) {
    EXPECT_NE(string::npos,
              post_str.find("appid=\"" + fake_update_response_.app_id + "_" +
                            dlc_module_id + "\""));
  }
  EXPECT_NE(string::npos,
            post_str.find("appid=\"" + fake_update_response_.app_id + "\""));

  // Count number of updatecheck tag in response.
  int updatecheck_count = 0;
  size_t pos = 0;
  while ((pos = post_str.find("<updatecheck", pos)) != string::npos) {
    updatecheck_count++;
    pos++;
  }
  EXPECT_EQ(request_params_.dlc_module_ids().size(), updatecheck_count);
}

TEST_F(OmahaRequestActionTest, InstallMissingPlatformVersionTest) {
  fake_update_response_.multi_app_skip_updatecheck = true;
  fake_update_response_.multi_app_no_update = false;
  request_params_.set_is_install(true);
  request_params_.set_dlc_module_ids({"dlc_no_0", "dlc_no_1"});
  request_params_.set_app_id(fake_update_response_.app_id_skip_updatecheck);
  OmahaResponse response;
  ASSERT_TRUE(TestUpdateCheck(fake_update_response_.GetUpdateResponse(),
                              -1,
                              false,  // ping_only
                              ErrorCode::kSuccess,
                              metrics::CheckResult::kUpdateAvailable,
                              metrics::CheckReaction::kUpdating,
                              metrics::DownloadErrorCode::kUnset,
                              &response,
                              nullptr));
  EXPECT_TRUE(response.update_exists);
  EXPECT_EQ(fake_update_response_.current_version, response.version);
}

}  // namespace chromeos_update_engine
