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

#include "update_engine/payload_consumer/bzip_extent_writer.h"

using google::protobuf::RepeatedPtrField;

namespace chromeos_update_engine {

namespace {
const brillo::Blob::size_type kOutputBufferLength = 16 * 1024;
}

BzipExtentWriter::~BzipExtentWriter() {
  TEST_AND_RETURN(BZ2_bzDecompressEnd(&stream_) == BZ_OK);
  TEST_AND_RETURN(input_buffer_.empty());
}

bool BzipExtentWriter::Init(FileDescriptorPtr fd,
                            const RepeatedPtrField<Extent>& extents,
                            uint32_t block_size) {
  // Init bzip2 stream
  int rc = BZ2_bzDecompressInit(&stream_,
                                0,   // verbosity. (0 == silent)
                                0);  // 0 = faster algo, more memory

  TEST_AND_RETURN_FALSE(rc == BZ_OK);

  return next_->Init(fd, extents, block_size);
}

bool BzipExtentWriter::Write(const void* bytes, size_t count) {
  brillo::Blob output_buffer(kOutputBufferLength);

  // Copy the input data into |input_buffer_| only if |input_buffer_| already
  // contains unconsumed data. Otherwise, process the data directly from the
  // source.
  const uint8_t* input = reinterpret_cast<const uint8_t*>(bytes);
  const uint8_t* input_end = input + count;
  if (!input_buffer_.empty()) {
    input_buffer_.insert(input_buffer_.end(), input, input_end);
    input = input_buffer_.data();
    input_end = input + input_buffer_.size();
  }
  stream_.next_in = reinterpret_cast<char*>(const_cast<uint8_t*>(input));
  stream_.avail_in = input_end - input;

  for (;;) {
    stream_.next_out = reinterpret_cast<char*>(output_buffer.data());
    stream_.avail_out = output_buffer.size();

    int rc = BZ2_bzDecompress(&stream_);
    TEST_AND_RETURN_FALSE(rc == BZ_OK || rc == BZ_STREAM_END);

    if (stream_.avail_out == output_buffer.size())
      break;  // got no new bytes

    TEST_AND_RETURN_FALSE(next_->Write(
        output_buffer.data(), output_buffer.size() - stream_.avail_out));

    if (rc == BZ_STREAM_END)
      CHECK_EQ(stream_.avail_in, 0u);
    if (stream_.avail_in == 0)
      break;  // no more input to process
  }

  // Store unconsumed data (if any) in |input_buffer_|.
  if (stream_.avail_in || !input_buffer_.empty()) {
    brillo::Blob new_input_buffer(input_end - stream_.avail_in, input_end);
    new_input_buffer.swap(input_buffer_);
  }

  return true;
}

}  // namespace chromeos_update_engine
