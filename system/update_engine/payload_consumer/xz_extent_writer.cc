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

#include "update_engine/payload_consumer/xz_extent_writer.h"

using google::protobuf::RepeatedPtrField;

namespace chromeos_update_engine {

namespace {
const brillo::Blob::size_type kOutputBufferLength = 16 * 1024;

// xz uses a variable dictionary size which impacts on the compression ratio
// and is required to be reconstructed in RAM during decompression. While we
// control the required memory from the compressor side, the decompressor allows
// to set a limit on this dictionary size, rejecting compressed streams that
// require more than that. "xz -9" requires up to 64 MiB, so a 64 MiB limit
// will allow compressed streams up to -9, the maximum compression setting.
const uint32_t kXzMaxDictSize = 64 * 1024 * 1024;

const char* XzErrorString(enum xz_ret error) {
#define __XZ_ERROR_STRING_CASE(code) \
  case code:                         \
    return #code;
  switch (error) {
    __XZ_ERROR_STRING_CASE(XZ_OK)
    __XZ_ERROR_STRING_CASE(XZ_STREAM_END)
    __XZ_ERROR_STRING_CASE(XZ_UNSUPPORTED_CHECK)
    __XZ_ERROR_STRING_CASE(XZ_MEM_ERROR)
    __XZ_ERROR_STRING_CASE(XZ_MEMLIMIT_ERROR)
    __XZ_ERROR_STRING_CASE(XZ_FORMAT_ERROR)
    __XZ_ERROR_STRING_CASE(XZ_OPTIONS_ERROR)
    __XZ_ERROR_STRING_CASE(XZ_DATA_ERROR)
    __XZ_ERROR_STRING_CASE(XZ_BUF_ERROR)
    default:
      return "<unknown xz error>";
  }
#undef __XZ_ERROR_STRING_CASE
}
}  // namespace

XzExtentWriter::~XzExtentWriter() {
  xz_dec_end(stream_);
  TEST_AND_RETURN(input_buffer_.empty());
}

bool XzExtentWriter::Init(FileDescriptorPtr fd,
                          const RepeatedPtrField<Extent>& extents,
                          uint32_t block_size) {
  stream_ = xz_dec_init(XZ_DYNALLOC, kXzMaxDictSize);
  TEST_AND_RETURN_FALSE(stream_ != nullptr);
  return underlying_writer_->Init(fd, extents, block_size);
}

bool XzExtentWriter::Write(const void* bytes, size_t count) {
  // Copy the input data into |input_buffer_| only if |input_buffer_| already
  // contains unconsumed data. Otherwise, process the data directly from the
  // source.
  const uint8_t* input = reinterpret_cast<const uint8_t*>(bytes);
  if (!input_buffer_.empty()) {
    input_buffer_.insert(input_buffer_.end(), input, input + count);
    input = input_buffer_.data();
    count = input_buffer_.size();
  }

  xz_buf request;
  request.in = input;
  request.in_pos = 0;
  request.in_size = count;

  brillo::Blob output_buffer(kOutputBufferLength);
  request.out = output_buffer.data();
  request.out_size = output_buffer.size();
  for (;;) {
    request.out_pos = 0;

    xz_ret ret = xz_dec_run(stream_, &request);
    if (ret != XZ_OK && ret != XZ_STREAM_END) {
      LOG(ERROR) << "xz_dec_run returned " << XzErrorString(ret);
      return false;
    }

    if (request.out_pos == 0)
      break;

    TEST_AND_RETURN_FALSE(
        underlying_writer_->Write(output_buffer.data(), request.out_pos));
    if (ret == XZ_STREAM_END)
      CHECK_EQ(request.in_size, request.in_pos);
    if (request.in_size == request.in_pos)
      break;  // No more input to process.
  }
  output_buffer.clear();

  // Store unconsumed data (if any) in |input_buffer_|. Since |input| can point
  // to the existing |input_buffer_| we create a new one before assigning it.
  brillo::Blob new_input_buffer(request.in + request.in_pos,
                                request.in + request.in_size);
  input_buffer_ = std::move(new_input_buffer);
  return true;
}

}  // namespace chromeos_update_engine
