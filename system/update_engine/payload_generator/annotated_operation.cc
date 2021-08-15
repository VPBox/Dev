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

#include "update_engine/payload_generator/annotated_operation.h"

#include <base/format_macros.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"

namespace chromeos_update_engine {

namespace {
// Output the list of extents as (start_block, num_blocks) in the passed output
// stream.
void OutputExtents(std::ostream* os,
                   const google::protobuf::RepeatedPtrField<Extent>& extents) {
  for (const auto& extent : extents) {
    *os << " (" << extent.start_block() << ", " << extent.num_blocks() << ")";
  }
}
}  // namespace

bool AnnotatedOperation::SetOperationBlob(const brillo::Blob& blob,
                                          BlobFileWriter* blob_file) {
  if (blob.empty()) {
    op.clear_data_offset();
    op.clear_data_length();
    return true;
  }
  off_t data_offset = blob_file->StoreBlob(blob);
  TEST_AND_RETURN_FALSE(data_offset != -1);
  op.set_data_offset(data_offset);
  op.set_data_length(blob.size());
  return true;
}

std::ostream& operator<<(std::ostream& os, const AnnotatedOperation& aop) {
  // For example, this prints:
  // REPLACE_BZ 500 @3000
  //   name: /foo/bar
  //    dst: (123, 3) (127, 2)
  os << InstallOperationTypeName(aop.op.type()) << " " << aop.op.data_length();
  if (aop.op.data_length() > 0)
    os << " @" << aop.op.data_offset();
  if (!aop.name.empty()) {
    os << std::endl << "  name: " << aop.name;
  }
  if (aop.op.src_extents_size() != 0) {
    os << std::endl << "   src:";
    OutputExtents(&os, aop.op.src_extents());
  }
  if (aop.op.dst_extents_size() != 0) {
    os << std::endl << "   dst:";
    OutputExtents(&os, aop.op.dst_extents());
  }
  return os;
}

}  // namespace chromeos_update_engine
