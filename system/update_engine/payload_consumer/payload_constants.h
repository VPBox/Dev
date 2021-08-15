//
// Copyright (C) 2014 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_PAYLOAD_CONSTANTS_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_PAYLOAD_CONSTANTS_H_

#include <stdint.h>

#include <limits>

#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

// The major version used by Chrome OS.
extern const uint64_t kChromeOSMajorPayloadVersion;

// The major version used by Brillo.
extern const uint64_t kBrilloMajorPayloadVersion;

// The minimum and maximum supported major version.
extern const uint64_t kMinSupportedMajorPayloadVersion;
extern const uint64_t kMaxSupportedMajorPayloadVersion;

// The minor version used for all full payloads.
extern const uint32_t kFullPayloadMinorVersion;

// The minor version used by the in-place delta generator algorithm.
extern const uint32_t kInPlaceMinorPayloadVersion;

// The minor version used by the A to B delta generator algorithm.
extern const uint32_t kSourceMinorPayloadVersion;

// The minor version that allows per-operation source hash.
extern const uint32_t kOpSrcHashMinorPayloadVersion;

// The minor version that allows BROTLI_BSDIFF, ZERO and DISCARD operation.
extern const uint32_t kBrotliBsdiffMinorPayloadVersion;

// The minor version that allows PUFFDIFF operation.
extern const uint32_t kPuffdiffMinorPayloadVersion;

// The minor version that allows Verity hash tree and FEC generation.
extern const uint32_t kVerityMinorPayloadVersion;

// The minimum and maximum supported minor version.
extern const uint32_t kMinSupportedMinorPayloadVersion;
extern const uint32_t kMaxSupportedMinorPayloadVersion;

// The maximum size of the payload header (anything before the protobuf).
extern const uint64_t kMaxPayloadHeaderSize;

// The kernel and rootfs partition names used by the BootControlInterface when
// handling update payloads with a major version 1. The names of the updated
// partitions are include in the payload itself for major version 2.
extern const char kPartitionNameKernel[];
extern const char kPartitionNameRoot[];

extern const char kBspatchPath[];
extern const char kDeltaMagic[4];

// A block number denoting a hole on a sparse file. Used on Extents to refer to
// section of blocks not present on disk on a sparse file.
const uint64_t kSparseHole = std::numeric_limits<uint64_t>::max();

// Return the name of the operation type.
const char* InstallOperationTypeName(InstallOperation::Type op_type);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_PAYLOAD_CONSTANTS_H_
