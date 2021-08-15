/******************************************************************************
 *
 *  Copyright 2019 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <base/logging.h>
#include <binder/ProcessState.h>
#include <gtest/gtest.h>
#include <fstream>

#include "btif/include/btif_keystore.h"

using namespace bluetooth;

class BtifKeystoreTest : public ::testing::Test {
 protected:
  std::unique_ptr<BtifKeystore> btif_keystore_;
  void SetUp() override {
    android::ProcessState::self()->startThreadPool();
    btif_keystore_ =
        std::make_unique<BtifKeystore>(static_cast<keystore::KeystoreClient*>(
            new keystore::KeystoreClientImpl));
  };
  void TearDown() override { btif_keystore_ = nullptr; };
};

TEST_F(BtifKeystoreTest, test_encrypt_decrypt) {
  std::string hash = "test";

  std::string encrypted_hash = btif_keystore_->Encrypt(hash, 0);
  std::string decrypted_hash = btif_keystore_->Decrypt(encrypted_hash);

  EXPECT_FALSE(encrypted_hash.empty());
  EXPECT_EQ(hash, decrypted_hash);
}

TEST_F(BtifKeystoreTest, test_encrypt_empty_hash) {
  std::string hash = "";

  std::string encrypted_hash = btif_keystore_->Encrypt(hash, 0);

  EXPECT_TRUE(encrypted_hash.empty());
}

TEST_F(BtifKeystoreTest, test_decrypt_empty_hash) {
  std::string hash = "";

  std::string decrypted_hash = btif_keystore_->Decrypt(hash);

  EXPECT_TRUE(decrypted_hash.empty());
}
