//
// Copyright (C) 2009 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_HASH_CALCULATOR_H_
#define UPDATE_ENGINE_COMMON_HASH_CALCULATOR_H_

#include <openssl/sha.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <base/logging.h>
#include <base/macros.h>
#include <brillo/secure_blob.h>

// This class provides a simple wrapper around OpenSSL providing a hash of data
// passed in.
// The methods of this class must be called in a very specific order: First the
// ctor (of course), then 0 or more calls to Update(), then Finalize(), then 0
// or more calls to raw_hash().

namespace chromeos_update_engine {

class HashCalculator {
 public:
  HashCalculator();

  // Update is called with all of the data that should be hashed in order.
  // Update will read |length| bytes of |data|.
  // Returns true on success.
  bool Update(const void* data, size_t length);

  // Updates the hash with up to |length| bytes of data from |file|. If |length|
  // is negative, reads in and updates with the whole file. Returns the number
  // of bytes that the hash was updated with, or -1 on error.
  off_t UpdateFile(const std::string& name, off_t length);

  // Call Finalize() when all data has been passed in. This method tells
  // OpenSSL that no more data will come in.
  // Returns true on success.
  bool Finalize();

  const brillo::Blob& raw_hash() const {
    DCHECK(!raw_hash_.empty()) << "Call Finalize() first";
    return raw_hash_;
  }

  // Gets the current hash context. Note that the string will contain binary
  // data (including \0 characters).
  std::string GetContext() const;

  // Sets the current hash context. |context| must the string returned by a
  // previous HashCalculator::GetContext method call. Returns true on success,
  // and false otherwise.
  bool SetContext(const std::string& context);

  static bool RawHashOfBytes(const void* data,
                             size_t length,
                             brillo::Blob* out_hash);
  static bool RawHashOfData(const brillo::Blob& data, brillo::Blob* out_hash);
  static off_t RawHashOfFile(const std::string& name,
                             off_t length,
                             brillo::Blob* out_hash);

 private:
  // If non-empty, the final raw hash. Will only be set to non-empty when
  // Finalize is called.
  brillo::Blob raw_hash_;

  // Init success
  bool valid_;

  // The hash state used by OpenSSL
  SHA256_CTX ctx_;
  DISALLOW_COPY_AND_ASSIGN(HashCalculator);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_HASH_CALCULATOR_H_
