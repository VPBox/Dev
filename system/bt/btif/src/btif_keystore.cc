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

#include "btif_keystore.h"
#include "keystore_client.pb.h"
#include "string.h"

#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>
#include <base/strings/utf_string_conversions.h>
#include <sys/stat.h>

using namespace keystore;
using namespace bluetooth;

const std::string kKeyStore = "bluetooth-key-encrypted";
constexpr uint32_t kAESKeySize = 256;     // bits
constexpr uint32_t kMACOutputSize = 128;  // bits

namespace bluetooth {

BtifKeystore::BtifKeystore(keystore::KeystoreClient* keystore_client)
    : keystore_client_(keystore_client) {}

std::string BtifKeystore::Encrypt(const std::string& data, int32_t flags) {
  std::lock_guard<std::mutex> lock(api_mutex_);
  std::string output;
  if (data.empty()) {
    LOG(ERROR) << __func__ << ": empty data";
    return output;
  }
  if (!GenerateKey(kKeyStore, flags)) {
    return output;
  }

  AuthorizationSetBuilder encrypt_params;
  encrypt_params.Authorization(TAG_BLOCK_MODE, BlockMode::GCM)
      .Authorization(TAG_MAC_LENGTH, kMACOutputSize)
      .Padding(PaddingMode::NONE);
  AuthorizationSet output_params;
  std::string raw_encrypted_data;
  if (!keystore_client_->oneShotOperation(
          KeyPurpose::ENCRYPT, kKeyStore, encrypt_params, data,
          std::string() /* signature_to_verify */, &output_params,
          &raw_encrypted_data)) {
    LOG(ERROR) << __func__ << ": AES operation failed.";
    return output;
  }
  auto init_vector_blob = output_params.GetTagValue(TAG_NONCE);
  if (!init_vector_blob.isOk()) {
    LOG(ERROR) << __func__ << ": Missing initialization vector.";
    return output;
  }

  const hidl_vec<uint8_t>& value = init_vector_blob.value();
  std::string init_vector =
      std::string(reinterpret_cast<const std::string::value_type*>(&value[0]),
                  value.size());

  if (memcmp(&init_vector_blob, &init_vector, init_vector.length()) == 0) {
    LOG(ERROR) << __func__
               << ": Protobuf nonce data doesn't match the actual nonce.";
  }

  EncryptedData protobuf;
  protobuf.set_init_vector(init_vector);
  protobuf.set_encrypted_data(raw_encrypted_data);
  if (!protobuf.SerializeToString(&output)) {
    LOG(ERROR) << __func__ << ": Failed to serialize EncryptedData protobuf.";
  }
  return output;
}

std::string BtifKeystore::Decrypt(const std::string& input) {
  std::lock_guard<std::mutex> lock(api_mutex_);
  if (input.empty()) {
    LOG(ERROR) << __func__ << ": empty input data";
    return "";
  }
  std::string output;
  EncryptedData protobuf;
  if (!protobuf.ParseFromString(input)) {
    LOG(ERROR) << __func__ << ": Failed to parse EncryptedData protobuf.";
    return output;
  }
  AuthorizationSetBuilder encrypt_params;
  encrypt_params.Authorization(TAG_BLOCK_MODE, BlockMode::GCM)
      .Authorization(TAG_MAC_LENGTH, kMACOutputSize)
      .Authorization(TAG_NONCE, protobuf.init_vector().data(),
                     protobuf.init_vector().size())
      .Padding(PaddingMode::NONE);
  AuthorizationSet output_params;
  if (!keystore_client_->oneShotOperation(
          KeyPurpose::DECRYPT, kKeyStore, encrypt_params,
          protobuf.encrypted_data(), std::string() /* signature_to_verify */,
          &output_params, &output)) {
    LOG(ERROR) << __func__ << ": AES operation failed.";
  }
  return output;
}

bool BtifKeystore::GenerateKey(const std::string& name, int32_t flags) {
  if (!DoesKeyExist()) {
    AuthorizationSetBuilder params;
    params.AesEncryptionKey(kAESKeySize)
        .Authorization(TAG_NO_AUTH_REQUIRED)
        .Authorization(TAG_BLOCK_MODE, BlockMode::GCM)
        .Authorization(TAG_PURPOSE, KeyPurpose::ENCRYPT)
        .Authorization(TAG_PURPOSE, KeyPurpose::DECRYPT)
        .Padding(PaddingMode::NONE)
        .Authorization(TAG_MIN_MAC_LENGTH, kMACOutputSize);
    AuthorizationSet hardware_enforced_characteristics;
    AuthorizationSet software_enforced_characteristics;
    auto result = keystore_client_->generateKey(
        name, params, flags, &hardware_enforced_characteristics,
        &software_enforced_characteristics);
    if (!result.isOk()) {
      LOG(FATAL) << __func__ << "Failed to generate key: name: " << name
                 << ", error code: " << result.getErrorCode();
      return false;
    }
  }
  return true;
}

bool BtifKeystore::DoesKeyExist() {
  return keystore_client_->doesKeyExist(kKeyStore);
}

}  // namespace bluetooth
