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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_XZ_EXTENT_WRITER_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_XZ_EXTENT_WRITER_H_

#include <xz.h>

#include <memory>
#include <utility>

#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/extent_writer.h"

// XzExtentWriter is a concrete ExtentWriter subclass that xz-decompresses
// what it's given in Write using xz-embedded. Note that xz-embedded only
// supports files with either no CRC or CRC-32. It passes the decompressed data
// to an underlying ExtentWriter.

namespace chromeos_update_engine {

class XzExtentWriter : public ExtentWriter {
 public:
  explicit XzExtentWriter(std::unique_ptr<ExtentWriter> underlying_writer)
      : underlying_writer_(std::move(underlying_writer)) {}
  ~XzExtentWriter() override;

  bool Init(FileDescriptorPtr fd,
            const google::protobuf::RepeatedPtrField<Extent>& extents,
            uint32_t block_size) override;
  bool Write(const void* bytes, size_t count) override;

 private:
  // The underlying ExtentWriter.
  std::unique_ptr<ExtentWriter> underlying_writer_;
  // The opaque xz decompressor struct.
  xz_dec* stream_{nullptr};
  brillo::Blob input_buffer_;

  DISALLOW_COPY_AND_ASSIGN(XzExtentWriter);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_XZ_EXTENT_WRITER_H_
