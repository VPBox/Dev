//
// Copyright (C) 2010 The Android Open Source Project
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

#include "update_engine/payload_generator/bzip.h"

#include <bzlib.h>
#include <stdlib.h>

#include <algorithm>
#include <limits>

#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

bool BzipCompress(const brillo::Blob& in, brillo::Blob* out) {
  TEST_AND_RETURN_FALSE(out);
  out->clear();
  if (in.size() == 0)
    return true;

  // We expect a compression ratio of about 35% with bzip2, so we start with
  // that much output space, which will then be doubled if needed.
  size_t buf_size = 40 + in.size() * 35 / 100;
  out->resize(buf_size);

  // Try increasing buffer size until it works
  for (;;) {
    if (buf_size > std::numeric_limits<uint32_t>::max())
      return false;
    uint32_t data_size = buf_size;
    int rc = BZ2_bzBuffToBuffCompress(
        reinterpret_cast<char*>(out->data()),
        &data_size,
        reinterpret_cast<char*>(const_cast<uint8_t*>(in.data())),
        in.size(),
        9,   // Best compression
        0,   // Silent verbosity
        0);  // Default work factor
    TEST_AND_RETURN_FALSE(rc == BZ_OUTBUFF_FULL || rc == BZ_OK);
    if (rc == BZ_OK) {
      // we're done!
      out->resize(data_size);
      return true;
    }

    // Data didn't fit; double the buffer size.
    buf_size *= 2;
    out->resize(buf_size);
  }
}

}  // namespace chromeos_update_engine
