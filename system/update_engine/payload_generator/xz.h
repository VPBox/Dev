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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_XZ_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_XZ_H_

#include <brillo/secure_blob.h>

namespace chromeos_update_engine {

// Initialize the xz compression unit. Call once before any call to
// XzCompress().
void XzCompressInit();

// Compresses the input buffer |in| into |out| with xz. The compressed stream
// will be the equivalent of running xz -9 --check=none
bool XzCompress(const brillo::Blob& in, brillo::Blob* out);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_XZ_H_
