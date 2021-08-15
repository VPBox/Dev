/*
**
** Copyright 2015, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "perfprofd_io.h"

#include <fcntl.h>
#include <unistd.h>

#include <memory>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/stringprintf.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <zlib.h>

#include "perfprofd_record.pb.h"

namespace android {
namespace perfprofd {

using android::base::StringPrintf;
using android::base::unique_fd;
using android::base::WriteFully;

namespace {

// Protobuf's file implementation is not available in protobuf-lite. :-(
class FileCopyingOutputStream : public ::google::protobuf::io::CopyingOutputStream {
 public:
  explicit FileCopyingOutputStream(android::base::unique_fd&& fd_in) : fd_(std::move(fd_in)) {
  };
  bool Write(const void * buffer, int size) override {
    return WriteFully(fd_.get(), buffer, size);
  }

 private:
  android::base::unique_fd fd_;
};

using google::protobuf::io::ZeroCopyOutputStream;

// Protobuf's Gzip implementation is not available in protobuf-lite. :-(
class GzipOutputStream : public ZeroCopyOutputStream {
 public:
  ~GzipOutputStream();

  static std::unique_ptr<GzipOutputStream> Create(ZeroCopyOutputStream* next,
                                                  std::string* error_msg);

  bool Next(void** data, int* size) override;

  void BackUp(int count) override;

  google::protobuf::int64 ByteCount() const override;

  bool WriteAliasedRaw(const void* data, int size) override;
  bool AllowsAliasing() const override;

  bool Flush();
  bool Close();

 private:
  GzipOutputStream(ZeroCopyOutputStream* next, z_stream* stream);

  int Write(int flush_flags);
  bool NextBuffer();

  ZeroCopyOutputStream* next_;
  void* next_data_;
  int next_size_;

  z_stream* stream_;
  std::unique_ptr<uint8_t[]> stream_buffer_;
  bool had_error_;
};

constexpr size_t kStreamBufferSize = 16u * 1024u;

GzipOutputStream::GzipOutputStream(ZeroCopyOutputStream* next, z_stream* stream)
    : next_(next),
      next_data_(nullptr),
      next_size_(0),
      stream_(stream),
      stream_buffer_(nullptr),
      had_error_(false) {
}

GzipOutputStream::~GzipOutputStream() {
  if (stream_ != nullptr) {
    deflateEnd(stream_);
    delete stream_;
    stream_ = nullptr;
  }
}

bool GzipOutputStream::WriteAliasedRaw(const void* data ATTRIBUTE_UNUSED,
                                       int size ATTRIBUTE_UNUSED) {
  LOG(FATAL) << "Not supported";
  __builtin_unreachable();
}
bool GzipOutputStream::AllowsAliasing() const {
  return false;
}

google::protobuf::int64 GzipOutputStream::ByteCount() const {
  return stream_->total_in + stream_->avail_in;
}

std::unique_ptr<GzipOutputStream> GzipOutputStream::Create(ZeroCopyOutputStream* next,
                                                           std::string* error_msg) {
  std::unique_ptr<z_stream> stream(new z_stream);

  stream->zalloc = Z_NULL;
  stream->zfree = Z_NULL;
  stream->opaque = Z_NULL;
  stream->msg = nullptr;
  stream->avail_in = 0;
  stream->total_in = 0;
  stream->next_in = nullptr;
  stream->total_out = 0;

  {
    constexpr int kWindowBits = 15;
    constexpr int kGzipEncoding = 16;
    constexpr int kMemLevel = 8;  // Default.
    int init_result = deflateInit2(stream.get(),
                                   Z_DEFAULT_COMPRESSION,
                                   Z_DEFLATED,
                                   kWindowBits | kGzipEncoding,
                                   kMemLevel,
                                   Z_DEFAULT_STRATEGY);
    if (init_result != Z_OK) {
      *error_msg = StringPrintf("Could not initialize compression: %d (%s)",
                                init_result,
                                stream->msg != nullptr ? stream->msg : "no message");
      return nullptr;
    }
  }

  return std::unique_ptr<GzipOutputStream>(new GzipOutputStream(next, stream.release()));
}

bool GzipOutputStream::NextBuffer() {
  for (;;) {
    if (!next_->Next(&next_data_, &next_size_)) {
      next_data_ = nullptr;
      next_size_ = 0;
      return false;
    }
    if (next_size_ == 0) {
      continue;
    }
    stream_->next_out = static_cast<Bytef*>(next_data_);
    stream_->avail_out = next_size_;
    return true;
  }
}

int GzipOutputStream::Write(int flush_flags) {
  CHECK(flush_flags == Z_NO_FLUSH || flush_flags == Z_FULL_FLUSH || flush_flags == Z_FINISH);

  int res;
  do {
    if ((next_data_ == nullptr || stream_->avail_out == 0) && !NextBuffer()) {
      return Z_BUF_ERROR;
    }
    res = deflate(stream_, flush_flags);
  } while (res == Z_OK && stream_->avail_out == 0);

  if (flush_flags == Z_FULL_FLUSH || flush_flags == Z_FINISH) {
    next_->BackUp(stream_->avail_out);
    next_data_ = nullptr;
    next_size_ = 0;
  }

  return res;
}

bool GzipOutputStream::Next(void** data, int* size) {
  if (had_error_) {
    return false;
  }

  // Write all pending data.
  if (stream_->avail_in > 0) {
    int write_error = Write(Z_NO_FLUSH);
    if (write_error != Z_OK) {
      had_error_ = true;
      return false;
    }
    CHECK_EQ(stream_->avail_in, 0);
  }

  if (stream_buffer_ == nullptr) {
    stream_buffer_.reset(new uint8_t[kStreamBufferSize]);
  }

  stream_->next_in = static_cast<Bytef*>(stream_buffer_.get());
  stream_->avail_in = kStreamBufferSize;
  *data = stream_buffer_.get();
  *size = kStreamBufferSize;
  return true;
}

void GzipOutputStream::BackUp(int count) {
  CHECK_GE(stream_->avail_in, count);
  stream_->avail_in -= count;
}

bool GzipOutputStream::Flush() {
  if (had_error_) {
    return false;
  }

  int res = Write(Z_FULL_FLUSH);
  had_error_ |= (res != Z_OK)
      && !(res == Z_BUF_ERROR && stream_->avail_in == 0 && stream_->avail_out > 0);
  return !had_error_;
}

bool GzipOutputStream::Close() {
  if (had_error_) {
    return false;
  }

  {
    int res;
    do {
      res = Write(Z_FINISH);
    } while (res == Z_OK);
  }

  int res = deflateEnd(stream_);
  delete stream_;
  stream_ = nullptr;

  had_error_ = true;  // Pretend an error so no other operations succeed.

  return res == Z_OK;
}

}  // namespace

bool SerializeProtobuf(android::perfprofd::PerfprofdRecord* encodedProfile,
                       android::base::unique_fd&& fd,
                       bool compress) {
  FileCopyingOutputStream fcos(std::move(fd));
  google::protobuf::io::CopyingOutputStreamAdaptor cosa(&fcos);

  ZeroCopyOutputStream* out;

  std::unique_ptr<GzipOutputStream> gzip;
  if (compress) {
    std::string error_msg;
    gzip = GzipOutputStream::Create(&cosa, &error_msg);
    if (gzip == nullptr) {
      LOG(ERROR) << error_msg;
      return false;
    }
    out = gzip.get();
  } else {
    out = &cosa;
  }

  bool serialized = encodedProfile->SerializeToZeroCopyStream(out);
  if (!serialized) {
    LOG(WARNING) << "SerializeToZeroCopyStream failed";
    return false;
  }

  bool zip_ok = true;
  if (gzip != nullptr) {
    zip_ok = gzip->Flush();
    zip_ok = gzip->Close() && zip_ok;
  }
  cosa.Flush();
  return zip_ok;
}

bool SerializeProtobuf(PerfprofdRecord* encodedProfile,
                       const char* encoded_file_path,
                       bool compress) {
  unlink(encoded_file_path);  // Attempt to unlink for a clean slate.
  constexpr int kFlags = O_CREAT | O_WRONLY | O_TRUNC | O_NOFOLLOW | O_CLOEXEC;
  unique_fd fd(open(encoded_file_path, kFlags, 0664));
  if (fd.get() == -1) {
    PLOG(WARNING) << "Could not open " << encoded_file_path << " for serialization";
    return false;
  }
  return SerializeProtobuf(encodedProfile, std::move(fd), compress);
}

}  // namespace perfprofd
}  // namespace android
