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
#include <keystore/keystore_client_impl.h>
#include <mutex>

#include "osi/include/alarm.h"
#include "osi/include/allocator.h"
#include "osi/include/compat.h"
#include "osi/include/config.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "osi/include/properties.h"

namespace bluetooth {
/**
 * Client wrapper to access AndroidKeystore.
 *
 * <p>Use to encrypt/decrypt data and store to disk.
 */
class BtifKeystore {
 public:
  /**
   * @param keystore_client injected pre-created client object for keystore
   */
  BtifKeystore(keystore::KeystoreClient* keystore_client);

  /**
   * Encrypts given data
   *
   * <p>Returns a string representation of the encrypted data
   *
   * @param data to be encrypted
   * @param flags for keystore
   */
  std::string Encrypt(const std::string& data, int32_t flags);

  /**
   * Returns a decrypted string representation of the encrypted data or empty
   * string on error.
   *
   * @param input encrypted data
   */
  std::string Decrypt(const std::string& input_filename);

  /**
   * Check for existence of keystore key.
   *
   * This key can be cleared if a user manually wipes bluetooth storage data
   * b/133214365
   */
  bool DoesKeyExist();

 private:
  std::unique_ptr<keystore::KeystoreClient> keystore_client_;
  std::mutex api_mutex_;
  bool GenerateKey(const std::string& name, int32_t flags);
};

}  // namespace bluetooth
