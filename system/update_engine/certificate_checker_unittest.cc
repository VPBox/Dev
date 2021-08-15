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

#include "update_engine/certificate_checker.h"

#include <string>

#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/mock_prefs.h"
#include "update_engine/mock_certificate_checker.h"

using std::string;
using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

namespace chromeos_update_engine {

class MockCertificateCheckObserver : public CertificateChecker::Observer {
 public:
  MOCK_METHOD2(CertificateChecked,
               void(ServerToCheck server_to_check,
                    CertificateCheckResult result));
};

class CertificateCheckerTest : public testing::Test {
 protected:
  void SetUp() override {
    cert_key_ = base::StringPrintf("%s-%d-%d",
                                   cert_key_prefix_.c_str(),
                                   static_cast<int>(server_to_check_),
                                   depth_);
    cert_checker.Init();
    cert_checker.SetObserver(&observer_);
  }

  void TearDown() override { cert_checker.SetObserver(nullptr); }

  MockPrefs prefs_;
  MockOpenSSLWrapper openssl_wrapper_;
  // Parameters of our mock certificate digest.
  int depth_{0};
  unsigned int length_{4};
  uint8_t digest_[4]{0x17, 0x7D, 0x07, 0x5F};
  string digest_hex_{"177D075F"};
  string diff_digest_hex_{"1234ABCD"};
  string cert_key_prefix_{kPrefsUpdateServerCertificate};
  ServerToCheck server_to_check_{ServerToCheck::kUpdate};
  string cert_key_;

  testing::StrictMock<MockCertificateCheckObserver> observer_;
  CertificateChecker cert_checker{&prefs_, &openssl_wrapper_};
};

// check certificate change, new
TEST_F(CertificateCheckerTest, NewCertificate) {
  EXPECT_CALL(openssl_wrapper_, GetCertificateDigest(nullptr, _, _, _))
      .WillOnce(DoAll(SetArgPointee<1>(depth_),
                      SetArgPointee<2>(length_),
                      SetArrayArgument<3>(digest_, digest_ + 4),
                      Return(true)));
  EXPECT_CALL(prefs_, GetString(cert_key_, _)).WillOnce(Return(false));
  EXPECT_CALL(prefs_, SetString(cert_key_, digest_hex_)).WillOnce(Return(true));
  EXPECT_CALL(
      observer_,
      CertificateChecked(server_to_check_, CertificateCheckResult::kValid));
  ASSERT_TRUE(
      cert_checker.CheckCertificateChange(1, nullptr, server_to_check_));
}

// check certificate change, unchanged
TEST_F(CertificateCheckerTest, SameCertificate) {
  EXPECT_CALL(openssl_wrapper_, GetCertificateDigest(nullptr, _, _, _))
      .WillOnce(DoAll(SetArgPointee<1>(depth_),
                      SetArgPointee<2>(length_),
                      SetArrayArgument<3>(digest_, digest_ + 4),
                      Return(true)));
  EXPECT_CALL(prefs_, GetString(cert_key_, _))
      .WillOnce(DoAll(SetArgPointee<1>(digest_hex_), Return(true)));
  EXPECT_CALL(prefs_, SetString(_, _)).Times(0);
  EXPECT_CALL(
      observer_,
      CertificateChecked(server_to_check_, CertificateCheckResult::kValid));
  ASSERT_TRUE(
      cert_checker.CheckCertificateChange(1, nullptr, server_to_check_));
}

// check certificate change, changed
TEST_F(CertificateCheckerTest, ChangedCertificate) {
  EXPECT_CALL(openssl_wrapper_, GetCertificateDigest(nullptr, _, _, _))
      .WillOnce(DoAll(SetArgPointee<1>(depth_),
                      SetArgPointee<2>(length_),
                      SetArrayArgument<3>(digest_, digest_ + 4),
                      Return(true)));
  EXPECT_CALL(prefs_, GetString(cert_key_, _))
      .WillOnce(DoAll(SetArgPointee<1>(diff_digest_hex_), Return(true)));
  EXPECT_CALL(observer_,
              CertificateChecked(server_to_check_,
                                 CertificateCheckResult::kValidChanged));
  EXPECT_CALL(prefs_, SetString(cert_key_, digest_hex_)).WillOnce(Return(true));
  ASSERT_TRUE(
      cert_checker.CheckCertificateChange(1, nullptr, server_to_check_));
}

// check certificate change, failed
TEST_F(CertificateCheckerTest, FailedCertificate) {
  EXPECT_CALL(
      observer_,
      CertificateChecked(server_to_check_, CertificateCheckResult::kFailed));
  EXPECT_CALL(prefs_, GetString(_, _)).Times(0);
  EXPECT_CALL(openssl_wrapper_, GetCertificateDigest(_, _, _, _)).Times(0);
  ASSERT_FALSE(
      cert_checker.CheckCertificateChange(0, nullptr, server_to_check_));
}

}  // namespace chromeos_update_engine
