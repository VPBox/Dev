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

#include "update_engine/common/hash_calculator.h"

#include <fcntl.h>

#include <base/logging.h>
#include <base/posix/eintr_wrapper.h>

#include "update_engine/common/utils.h"

using std::string;

namespace chromeos_update_engine {

HashCalculator::HashCalculator() : valid_(false) {
  valid_ = (SHA256_Init(&ctx_) == 1);
  LOG_IF(ERROR, !valid_) << "SHA256_Init failed";
}

// Update is called with all of the data that should be hashed in order.
// Mostly just passes the data through to OpenSSL's SHA256_Update()
bool HashCalculator::Update(const void* data, size_t length) {
  TEST_AND_RETURN_FALSE(valid_);
  TEST_AND_RETURN_FALSE(raw_hash_.empty());
  static_assert(sizeof(size_t) <= sizeof(unsigned long),  // NOLINT(runtime/int)
                "length param may be truncated in SHA256_Update");
  TEST_AND_RETURN_FALSE(SHA256_Update(&ctx_, data, length) == 1);
  return true;
}

off_t HashCalculator::UpdateFile(const string& name, off_t length) {
  int fd = HANDLE_EINTR(open(name.c_str(), O_RDONLY));
  if (fd < 0) {
    return -1;
  }

  const int kBufferSize = 128 * 1024;  // 128 KiB
  brillo::Blob buffer(kBufferSize);
  off_t bytes_processed = 0;
  while (length < 0 || bytes_processed < length) {
    off_t bytes_to_read = buffer.size();
    if (length >= 0 && bytes_to_read > length - bytes_processed) {
      bytes_to_read = length - bytes_processed;
    }
    ssize_t rc = HANDLE_EINTR(read(fd, buffer.data(), bytes_to_read));
    if (rc == 0) {  // EOF
      break;
    }
    if (rc < 0 || !Update(buffer.data(), rc)) {
      bytes_processed = -1;
      break;
    }
    bytes_processed += rc;
  }
  IGNORE_EINTR(close(fd));
  return bytes_processed;
}

// Call Finalize() when all data has been passed in. This mostly just
// calls OpenSSL's SHA256_Final().
bool HashCalculator::Finalize() {
  TEST_AND_RETURN_FALSE(raw_hash_.empty());
  raw_hash_.resize(SHA256_DIGEST_LENGTH);
  TEST_AND_RETURN_FALSE(SHA256_Final(raw_hash_.data(), &ctx_) == 1);
  return true;
}

bool HashCalculator::RawHashOfBytes(const void* data,
                                    size_t length,
                                    brillo::Blob* out_hash) {
  HashCalculator calc;
  TEST_AND_RETURN_FALSE(calc.Update(data, length));
  TEST_AND_RETURN_FALSE(calc.Finalize());
  *out_hash = calc.raw_hash();
  return true;
}

bool HashCalculator::RawHashOfData(const brillo::Blob& data,
                                   brillo::Blob* out_hash) {
  return RawHashOfBytes(data.data(), data.size(), out_hash);
}

off_t HashCalculator::RawHashOfFile(const string& name,
                                    off_t length,
                                    brillo::Blob* out_hash) {
  HashCalculator calc;
  off_t res = calc.UpdateFile(name, length);
  if (res < 0) {
    return res;
  }
  if (!calc.Finalize()) {
    return -1;
  }
  *out_hash = calc.raw_hash();
  return res;
}

string HashCalculator::GetContext() const {
  return string(reinterpret_cast<const char*>(&ctx_), sizeof(ctx_));
}

bool HashCalculator::SetContext(const string& context) {
  TEST_AND_RETURN_FALSE(context.size() == sizeof(ctx_));
  memcpy(&ctx_, context.data(), sizeof(ctx_));
  return true;
}

}  // namespace chromeos_update_engine
