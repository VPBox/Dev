//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/payload_generator/xz.h"

#include <base/logging.h>
#include <lzma.h>

namespace chromeos_update_engine {

void XzCompressInit() {}

bool XzCompress(const brillo::Blob& in, brillo::Blob* out) {
  out->clear();
  if (in.empty())
    return true;

  // Resize the output buffer to get enough memory for writing the compressed
  // data.
  out->resize(lzma_stream_buffer_bound(in.size()));

  const uint32_t kLzmaPreset = 6;
  size_t out_pos = 0;
  int rc = lzma_easy_buffer_encode(kLzmaPreset,
                                   LZMA_CHECK_NONE,  // We do not need CRC.
                                   nullptr,
                                   in.data(),
                                   in.size(),
                                   out->data(),
                                   &out_pos,
                                   out->size());
  if (rc != LZMA_OK) {
    LOG(ERROR) << "Failed to compress data to LZMA stream with return code: "
               << rc;
    return false;
  }
  out->resize(out_pos);
  return true;
}

}  // namespace chromeos_update_engine
