//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/update_manager/real_system_provider.h"

#include <memory>

#include <base/time/time.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "update_engine/common/fake_boot_control.h"
#include "update_engine/common/fake_hardware.h"
#include "update_engine/update_manager/umtest_utils.h"
#if USE_CHROME_KIOSK_APP
#include "kiosk-app/dbus-proxies.h"
#include "kiosk-app/dbus-proxy-mocks.h"

using org::chromium::KioskAppServiceInterfaceProxyMock;
#endif  // USE_CHROME_KIOSK_APP
using std::unique_ptr;
using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

#if USE_CHROME_KIOSK_APP
namespace {
const char kRequiredPlatformVersion[] = "1234.0.0";
}  // namespace
#endif  // USE_CHROME_KIOSK_APP

namespace chromeos_update_manager {

class UmRealSystemProviderTest : public ::testing::Test {
 protected:
  void SetUp() override {
#if USE_CHROME_KIOSK_APP
    kiosk_app_proxy_mock_.reset(new KioskAppServiceInterfaceProxyMock());
    ON_CALL(*kiosk_app_proxy_mock_, GetRequiredPlatformVersion(_, _, _))
        .WillByDefault(
            DoAll(SetArgPointee<0>(kRequiredPlatformVersion), Return(true)));

    provider_.reset(new RealSystemProvider(
        &fake_hardware_, &fake_boot_control_, kiosk_app_proxy_mock_.get()));
#else
    provider_.reset(
        new RealSystemProvider(&fake_hardware_, &fake_boot_control_, nullptr));
#endif  // USE_CHROME_KIOSK_APP
    EXPECT_TRUE(provider_->Init());
  }

  chromeos_update_engine::FakeHardware fake_hardware_;
  chromeos_update_engine::FakeBootControl fake_boot_control_;
  unique_ptr<RealSystemProvider> provider_;

#if USE_CHROME_KIOSK_APP
  unique_ptr<KioskAppServiceInterfaceProxyMock> kiosk_app_proxy_mock_;
#endif  // USE_CHROME_KIOSK_APP
};

TEST_F(UmRealSystemProviderTest, InitTest) {
  EXPECT_NE(nullptr, provider_->var_is_normal_boot_mode());
  EXPECT_NE(nullptr, provider_->var_is_official_build());
  EXPECT_NE(nullptr, provider_->var_is_oobe_complete());
  EXPECT_NE(nullptr, provider_->var_kiosk_required_platform_version());
}

TEST_F(UmRealSystemProviderTest, IsOOBECompleteTrue) {
  fake_hardware_.SetIsOOBEComplete(base::Time());
  UmTestUtils::ExpectVariableHasValue(true, provider_->var_is_oobe_complete());
}

TEST_F(UmRealSystemProviderTest, IsOOBECompleteFalse) {
  fake_hardware_.UnsetIsOOBEComplete();
  UmTestUtils::ExpectVariableHasValue(false, provider_->var_is_oobe_complete());
}

#if USE_CHROME_KIOSK_APP
TEST_F(UmRealSystemProviderTest, KioskRequiredPlatformVersion) {
  UmTestUtils::ExpectVariableHasValue(
      std::string(kRequiredPlatformVersion),
      provider_->var_kiosk_required_platform_version());
}

TEST_F(UmRealSystemProviderTest, KioskRequiredPlatformVersionFailure) {
  EXPECT_CALL(*kiosk_app_proxy_mock_, GetRequiredPlatformVersion(_, _, _))
      .WillOnce(Return(false));

  UmTestUtils::ExpectVariableNotSet(
      provider_->var_kiosk_required_platform_version());
}

TEST_F(UmRealSystemProviderTest,
       KioskRequiredPlatformVersionRecoveryFromFailure) {
  EXPECT_CALL(*kiosk_app_proxy_mock_, GetRequiredPlatformVersion(_, _, _))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(
      provider_->var_kiosk_required_platform_version());
  testing::Mock::VerifyAndClearExpectations(kiosk_app_proxy_mock_.get());

  EXPECT_CALL(*kiosk_app_proxy_mock_, GetRequiredPlatformVersion(_, _, _))
      .WillOnce(
          DoAll(SetArgPointee<0>(kRequiredPlatformVersion), Return(true)));
  UmTestUtils::ExpectVariableHasValue(
      std::string(kRequiredPlatformVersion),
      provider_->var_kiosk_required_platform_version());
}
#else
TEST_F(UmRealSystemProviderTest, KioskRequiredPlatformVersion) {
  UmTestUtils::ExpectVariableHasValue(
      std::string(), provider_->var_kiosk_required_platform_version());
}
#endif  // USE_CHROME_KIOSK_APP

}  // namespace chromeos_update_manager
