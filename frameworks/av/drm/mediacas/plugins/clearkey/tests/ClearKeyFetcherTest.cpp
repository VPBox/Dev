/*
 * Copyright (C) 2017 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "ClearKeyFetcherTest"
#include <utils/Log.h>
#include <gtest/gtest.h>
#include <stddef.h>
#include <algorithm>
#include <string>

#include "ClearKeyFetcher.h"
#include "ClearKeyLicenseFetcher.h"
#include "protos/license_protos.pb.h"

namespace android {
namespace clearkeycas {

const char *kTestAssetInJson =
        "{                                                   "
        "  \"id\": 21140844,                                 "
        "  \"name\": \"Test Title\",                         "
        "  \"lowercase_organization_name\": \"Android\",     "
        "  \"asset_key\": {                                  "
        "  \"encryption_key\": \"nezAr3CHFrmBR9R8Tedotw==\"  "
        "  },                                                "
        "  \"cas_type\": 1,                                  "
        "  \"track_types\": [ ]                              "
        "}                                                   " ;

const uint8_t kTestEcmContainer[] = {
        0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x27, 0x10, 0x02, 0x00, 0x01, 0x77,
        0x01, 0x42, 0x95, 0x6c, 0x0e, 0xe3, 0x91, 0xbc,
        0xfd, 0x05, 0xb1, 0x60, 0x4f, 0x17, 0x82, 0xa4,
        0x86, 0x9b, 0x23, 0x56, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x27, 0x10, 0x02, 0x00,
        0x01, 0x77, 0x01, 0x42, 0x95, 0x6c, 0xd7, 0x43,
        0x62, 0xf8, 0x1c, 0x62, 0x19, 0x05, 0xc7, 0x3a,
        0x42, 0xcd, 0xfd, 0xd9, 0x13, 0x48,
};

const uint8_t kTestContentKey0[] = {
        0x0e, 0xe3, 0x91, 0xbc, 0xfd, 0x05, 0xb1, 0x60,
        0x4f, 0x17, 0x82, 0xa4, 0x86, 0x9b, 0x23, 0x56};

const uint8_t kTestContentKey1[] = {
        0xd7, 0x43, 0x62, 0xf8, 0x1c, 0x62, 0x19, 0x05,
        0xc7, 0x3a, 0x42, 0xcd, 0xfd, 0xd9, 0x13, 0x48};

constexpr uint32_t kTestEcmCount = 2;

class ClearKeyFetcherTest : public testing::Test {
protected:
    virtual void SetUp();

protected:
    std::unique_ptr<ClearKeyLicenseFetcher> license_fetcher_;
    sp<ABuffer> ecm_;
    sp<ABuffer> content_key_[kTestEcmCount];
};

void ClearKeyFetcherTest::SetUp() {
    license_fetcher_.reset(new ClearKeyLicenseFetcher());
    EXPECT_EQ(OK, license_fetcher_->Init(kTestAssetInJson));
    ecm_ = new ABuffer((void*) (kTestEcmContainer), sizeof(kTestEcmContainer));
    content_key_[0] = new ABuffer(
            (void*)kTestContentKey0, sizeof(kTestContentKey0));
    content_key_[1] = new ABuffer(
            (void*)kTestContentKey1, sizeof(kTestContentKey1));
}

TEST_F(ClearKeyFetcherTest, Ctor) {
    ClearKeyFetcher fetcher(std::move(license_fetcher_));
}

TEST_F(ClearKeyFetcherTest, Success) {
    ClearKeyFetcher fetcher(std::move(license_fetcher_));
    EXPECT_EQ(OK, fetcher.Init());
    uint64_t asset_id;
    std::vector<KeyFetcher::KeyInfo> keys;
    EXPECT_EQ(OK, fetcher.ObtainKey(ecm_, &asset_id, &keys));
    EXPECT_EQ(2U, keys.size());
    EXPECT_EQ(0, keys[0].key_id);
    EXPECT_EQ(content_key_[0]->size(), keys[0].key_bytes->size());
    EXPECT_EQ(0, memcmp(content_key_[0]->data(),
            keys[0].key_bytes->data(), content_key_[0]->size()));
    EXPECT_EQ(1, keys[1].key_id);
    EXPECT_EQ(content_key_[1]->size(), keys[1].key_bytes->size());
    EXPECT_EQ(0, memcmp(content_key_[1]->data(),
            keys[1].key_bytes->data(), content_key_[1]->size()));
}

} // namespace clearkeycas
} // namespace android
