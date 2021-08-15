/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <string>
#include <utils/String16.h>

#include "../keystore_attestation_id.h"
#include <keymaster/logger.h>

#include <keystore/KeyAttestationApplicationId.h>
#include <keystore/KeyAttestationPackageInfo.h>
#include <keystore/Signature.h>

using ::android::String16;
using ::android::security::KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE;
using ::android::security::keymaster::KeyAttestationApplicationId;
using ::android::security::keymaster::KeyAttestationPackageInfo;
using std::vector;

namespace keystore {

namespace test {

namespace {

constexpr const char* kDummyPackageName = "DummyPackage";

constexpr const char* kLongPackageName =
    "a.long.package.name"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

constexpr const char* kReasonablePackageName =
    "a.reasonable.length.package.name"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

constexpr const size_t kTooManyPackages = 4;

// Signatures should be 32 bytes
constexpr const uint8_t kDummySignature[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
constexpr const size_t kTooManySignatures = 35;

}  // namespace

using ::android::content::pm::Signature;
using ::android::security::build_attestation_application_id;

std::unique_ptr<KeyAttestationPackageInfo>
make_package_info_with_signatures(const char* package_name,
                                  KeyAttestationPackageInfo::SignaturesVector signatures) {
    return std::make_unique<KeyAttestationPackageInfo>(
        String16(package_name), 1 /* version code */,
        std::make_shared<KeyAttestationPackageInfo::SignaturesVector>(std::move(signatures)));
}

std::unique_ptr<KeyAttestationPackageInfo> make_package_info(const char* package_name) {
    return make_package_info_with_signatures(package_name,
                                             KeyAttestationPackageInfo::SignaturesVector());
}

TEST(AaidTruncationTest, shortPackageInfoTest) {
    KeyAttestationApplicationId app_id(make_package_info(kDummyPackageName));

    auto result = build_attestation_application_id(app_id);
    ASSERT_TRUE(result.isOk());
    std::vector<uint8_t>& encoded_app_id = result;
    ASSERT_LT(encoded_app_id.size(), KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE);
}

TEST(AaidTruncationTest, tooLongPackageNameTest) {
    KeyAttestationApplicationId app_id(make_package_info(kLongPackageName));

    auto result = build_attestation_application_id(app_id);
    ASSERT_TRUE(result.isOk());
    std::vector<uint8_t>& encoded_app_id = result;
    ASSERT_LT(encoded_app_id.size(), KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE);
}

TEST(AaidTruncationTest, tooManySignaturesTest) {
    std::vector<uint8_t> dummy_sig_data(kDummySignature, kDummySignature + 32);
    KeyAttestationPackageInfo::SignaturesVector signatures;
    // Add 35 signatures which will surely exceed the 1K limit.
    for (size_t i = 0; i < kTooManySignatures; ++i) {
        signatures.push_back(std::make_unique<Signature>(dummy_sig_data));
    }

    KeyAttestationApplicationId app_id(
        make_package_info_with_signatures(kDummyPackageName, std::move(signatures)));

    auto result = build_attestation_application_id(app_id);
    ASSERT_TRUE(result.isOk());
    std::vector<uint8_t>& encoded_app_id = result;
    ASSERT_LT(encoded_app_id.size(), KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE);
}

TEST(AaidTruncationTest, combinedPackagesAndSignaturesTest) {
    std::vector<uint8_t> dummy_sig_data(kDummySignature, kDummySignature + 32);
    KeyAttestationApplicationId::PackageInfoVector packages;

    for (size_t i = 0; i < kTooManyPackages; ++i) {
        KeyAttestationPackageInfo::SignaturesVector signatures;
        // Add a few signatures for each package
        for (int j = 0; j < 3; ++j) {
            signatures.push_back(std::make_unique<Signature>(dummy_sig_data));
        }
        packages.push_back(
            make_package_info_with_signatures(kReasonablePackageName, std::move(signatures)));
    }

    KeyAttestationApplicationId app_id(std::move(packages));
    auto result = build_attestation_application_id(app_id);
    ASSERT_TRUE(result.isOk());
    std::vector<uint8_t>& encoded_app_id = result;
    ASSERT_LT(encoded_app_id.size(), KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE);
}

}  // namespace test
}  // namespace keystore
