/*
 * Copyright 2016 The Android Open Source Project
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

#include <fstream>

#include <gtest/gtest.h>

#include <keymaster/keymaster_context.h>

#include "android_keymaster_test_utils.h"
#include <keymaster/attestation_record.h>

namespace keymaster {
namespace test {

class TestContext : public AttestationRecordContext {
  public:
    keymaster_security_level_t GetSecurityLevel() const override {
        return KM_SECURITY_LEVEL_TRUSTED_ENVIRONMENT;
    }
    keymaster_error_t GenerateUniqueId(uint64_t /* creation_date_time */,
                                       const keymaster_blob_t& application_id,
                                       bool /* reset_since_rotation */,
                                       Buffer* unique_id) const override {
        // Use the application ID directly as the unique ID.
        unique_id->Reinitialize(application_id.data, application_id.data_length);
        return KM_ERROR_OK;
    }
    keymaster_error_t GetVerifiedBootParams(keymaster_blob_t* verified_boot_key,
                                            keymaster_verified_boot_t* verified_boot_state,
                                            bool* device_locked) const override {
        verified_boot_key->data = vboot_key_;
        verified_boot_key->data_length = sizeof(vboot_key_);
        *verified_boot_state = KM_VERIFIED_BOOT_VERIFIED;
        *device_locked = true;
        return KM_ERROR_OK;
    }

    void VerifyRootOfTrust(const keymaster_blob_t& verified_boot_key,
                           keymaster_verified_boot_t verified_boot_state, bool device_locked) {
        EXPECT_EQ(sizeof(vboot_key_), verified_boot_key.data_length);
        if (sizeof(vboot_key_) == verified_boot_key.data_length) {
            EXPECT_EQ(0, memcmp(verified_boot_key.data, vboot_key_, sizeof(vboot_key_)));
        }
        EXPECT_TRUE(device_locked);
        EXPECT_EQ(KM_VERIFIED_BOOT_VERIFIED, verified_boot_state);
    }

  private:
    uint8_t vboot_key_[32]{"test_vboot_key"};
};

TEST(AttestTest, Simple) {
    TestContext context;
    AuthorizationSet hw_set(AuthorizationSetBuilder()
                                .RsaSigningKey(512, 3)
                                .Digest(KM_DIGEST_SHA_2_256)
                                .Digest(KM_DIGEST_SHA_2_384)
                                .Authorization(TAG_OS_VERSION, 60000)
                                .Authorization(TAG_OS_PATCHLEVEL, 201512)
                                .Authorization(TAG_INCLUDE_UNIQUE_ID));
    AuthorizationSet sw_set(AuthorizationSetBuilder()
                                .Authorization(TAG_ACTIVE_DATETIME, 10)
                                .Authorization(TAG_CREATION_DATETIME, 10)
                                .Authorization(TAG_APPLICATION_ID, "fake_app_id", 11));

    UniquePtr<uint8_t[]> asn1;
    size_t asn1_len = 0;
    AuthorizationSet attest_params(
        AuthorizationSetBuilder()
            .Authorization(TAG_ATTESTATION_CHALLENGE, "fake_challenge", 14)
            .Authorization(TAG_ATTESTATION_APPLICATION_ID, "fake_attest_app_id", 18));
    ASSERT_EQ(KM_ERROR_OK,
              build_attestation_record(attest_params, sw_set, hw_set, context, &asn1, &asn1_len));
    EXPECT_GT(asn1_len, 0U);

    std::ofstream output("attest.der",
                         std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
    if (output)
        output.write(reinterpret_cast<const char*>(asn1.get()), asn1_len);
    output.close();

    AuthorizationSet parsed_hw_set;
    AuthorizationSet parsed_sw_set;
    uint32_t attestation_version;
    uint32_t keymaster_version;
    keymaster_security_level_t attestation_security_level;
    keymaster_security_level_t keymaster_security_level;
    keymaster_blob_t attestation_challenge = {};
    keymaster_blob_t unique_id = {};
    EXPECT_EQ(KM_ERROR_OK,
              parse_attestation_record(asn1.get(), asn1_len, &attestation_version,
                                       &attestation_security_level, &keymaster_version,
                                       &keymaster_security_level, &attestation_challenge,
                                       &parsed_sw_set, &parsed_hw_set, &unique_id));

    // Check that the challenge is consistent across build and parse.
    EXPECT_EQ("fake_challenge",
              std::string(reinterpret_cast<const char*>(attestation_challenge.data), 14));
    delete[] attestation_challenge.data;

    // Check that the unique id was populated as expected.
    EXPECT_EQ("fake_app_id", std::string(reinterpret_cast<const char*>(unique_id.data), 11));
    delete[] unique_id.data;

    // The attestation ID is expected to appear in parsed_sw_set.
    sw_set.push_back(TAG_ATTESTATION_APPLICATION_ID, "fake_attest_app_id", 18);

    // The TAG_INCLUDE_UNIQUE_ID tag is not expected to appear in parsed_hw_set.
    hw_set.erase(hw_set.find(TAG_INCLUDE_UNIQUE_ID));

    // Check that the list of tags is consistent across build and parse.
    hw_set.Sort();
    sw_set.Sort();
    parsed_hw_set.Sort();
    parsed_sw_set.Sort();
    EXPECT_EQ(hw_set, parsed_hw_set);
    EXPECT_EQ(sw_set, parsed_sw_set);

    // Check the root of trust values.
    keymaster_blob_t verified_boot_key;
    keymaster_verified_boot_t verified_boot_state;
    bool device_locked;
    EXPECT_EQ(KM_ERROR_OK, parse_root_of_trust(asn1.get(), asn1_len, &verified_boot_key,
                                               &verified_boot_state, &device_locked));
    context.VerifyRootOfTrust(verified_boot_key, verified_boot_state, device_locked);
    delete[] verified_boot_key.data;
}

}  // namespace test
}  // namespace keymaster
