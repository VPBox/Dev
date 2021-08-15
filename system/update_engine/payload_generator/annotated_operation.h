//
// Copyright (C) 2015 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_ANNOTATED_OPERATION_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_ANNOTATED_OPERATION_H_

#include <ostream>  // NOLINT(readability/streams)
#include <string>

#include <brillo/secure_blob.h>

#include "update_engine/payload_generator/blob_file_writer.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

struct AnnotatedOperation {
  // The name given to the operation, for logging and debugging purposes only.
  // This normally includes the path to the file and the chunk used, if any.
  std::string name;

  // The InstallOperation, as defined by the protobuf.
  InstallOperation op;

  // Writes |blob| to the end of |blob_file|. It sets the data_offset and
  // data_length in AnnotatedOperation to match the offset and size of |blob|
  // in |blob_file|.
  bool SetOperationBlob(const brillo::Blob& blob, BlobFileWriter* blob_file);
};

// For logging purposes.
std::ostream& operator<<(std::ostream& os, const AnnotatedOperation& aop);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_ANNOTATED_OPERATION_H_
