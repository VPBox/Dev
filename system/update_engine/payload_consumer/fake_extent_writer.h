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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FAKE_EXTENT_WRITER_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FAKE_EXTENT_WRITER_H_

#include <memory>

#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/extent_writer.h"

namespace chromeos_update_engine {

// FakeExtentWriter is a concrete ExtentWriter subclass that keeps track of all
// the written data, useful for testing.
class FakeExtentWriter : public ExtentWriter {
 public:
  FakeExtentWriter() = default;
  ~FakeExtentWriter() override = default;

  // ExtentWriter overrides.
  bool Init(FileDescriptorPtr /* fd */,
            const google::protobuf::RepeatedPtrField<Extent>& /* extents */,
            uint32_t /* block_size */) override {
    init_called_ = true;
    return true;
  };
  bool Write(const void* bytes, size_t count) override {
    if (!init_called_)
      return false;
    written_data_.insert(written_data_.end(),
                         reinterpret_cast<const uint8_t*>(bytes),
                         reinterpret_cast<const uint8_t*>(bytes) + count);
    return true;
  }

  // Fake methods.
  bool InitCalled() { return init_called_; }
  brillo::Blob WrittenData() { return written_data_; }

 private:
  bool init_called_{false};
  brillo::Blob written_data_;

  DISALLOW_COPY_AND_ASSIGN(FakeExtentWriter);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FAKE_EXTENT_WRITER_H_
