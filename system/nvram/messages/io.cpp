/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <nvram/messages/io.h>

extern "C" {
#include <string.h>
}

#include <nvram/messages/compiler.h>

namespace nvram {
namespace {

template <typename T>
T min(T x, T y) {
  return x < y ? x : y;
}

template <typename T>
T max(T x, T y) {
  return x > y ? x : y;
}

// Encodes |value| in varint format and writes the result to |stream|.
bool EncodeVarint(OutputStreamBuffer* stream, uint64_t value) {
  do {
    uint8_t byte = (value & 0x7f) | (((value >> 7) == 0) ? 0x00 : 0x80);
    if (!stream->WriteByte(byte)) {
      return false;
    }
    value >>= 7;
  } while (value != 0);
  return true;
}

// Read a varint-encoded number from stream, decode it and store the result in
// |value|.
bool DecodeVarint(InputStreamBuffer* stream_buffer, uint64_t* value) {
  // Maximum number of bytes required to encode an |uint64_t| as varint. Each
  // byte in a varint has 7 payload bytes, so encoding 64 bits yields at most 10
  // bytes.
  static constexpr int kMaxVarintBytes = 10;

  *value = 0;
  for (int i = 0; i < kMaxVarintBytes; ++i) {
    uint8_t byte = 0;
    if (!stream_buffer->ReadByte(&byte)) {
      return false;
    }
    *value |= static_cast<uint64_t>(byte & 0x7f) << (i * 7);
    if ((byte & 0x80) == 0) {
      return true;
    }
  }
  return false;
}

}  // namespace

InputStreamBuffer::InputStreamBuffer(const void* data, size_t size)
    : InputStreamBuffer(data, static_cast<const uint8_t*>(data) + size) {}

InputStreamBuffer::InputStreamBuffer(const void* start, const void* end)
    : pos_(static_cast<const uint8_t*>(start)),
      end_(static_cast<const uint8_t*>(end)) {
  NVRAM_CHECK(pos_ <= end_);
}

bool InputStreamBuffer::Done() {
  return pos_ >= end_ && !Advance();
}

bool InputStreamBuffer::Read(void* data, size_t size) {
  uint8_t* buffer = static_cast<uint8_t*>(data);
  NVRAM_CHECK(pos_ <= end_);
  while (size > static_cast<size_t>(end_ - pos_)) {
    memcpy(buffer, pos_, end_ - pos_);
    buffer += end_ - pos_;
    size -= end_ - pos_;
    pos_ = end_;
    if (!Advance()) {
      return false;
    }
    NVRAM_CHECK(pos_ < end_);
  }
  memcpy(buffer, pos_, size);
  pos_ += size;
  return true;
}

bool InputStreamBuffer::ReadByte(uint8_t* byte) {
  if (pos_ >= end_) {
    if (!Advance()) {
      return false;
    }
    NVRAM_CHECK(pos_ < end_);
  }
  *byte = *pos_;
  ++pos_;
  return true;
}

bool InputStreamBuffer::Skip(size_t size) {
  NVRAM_CHECK(pos_ <= end_);
  while (size > static_cast<size_t>(end_ - pos_)) {
    size -= end_ - pos_;
    pos_ = end_;
    if (!Advance()) {
      return false;
    }
    NVRAM_CHECK(pos_ < end_);
  }
  pos_ += size;
  return true;
}

bool InputStreamBuffer::Advance() {
  return false;
}

NestedInputStreamBuffer::NestedInputStreamBuffer(InputStreamBuffer* delegate,
                                                 size_t size)
    : InputStreamBuffer(delegate->pos_, ClampEnd(delegate, size)),
      delegate_(delegate),
      remaining_(size) {}

bool NestedInputStreamBuffer::Advance() {
  remaining_ -= end_ - delegate_->pos_;
  if (remaining_ == 0) {
    delegate_->pos_ = end_;
    return false;
  }
  bool status = delegate_->Advance();
  pos_ = delegate_->pos_;
  end_ = ClampEnd(delegate_, remaining_);
  return status;
}

// static
const uint8_t* NestedInputStreamBuffer::ClampEnd(InputStreamBuffer* delegate,
                                                 size_t size) {
  NVRAM_CHECK(delegate->pos_ <= delegate->end_);
  return size < static_cast<size_t>(delegate->end_ - delegate->pos_)
             ? delegate->pos_ + size
             : delegate->end_;
}

OutputStreamBuffer::OutputStreamBuffer(void* data, size_t size)
    : OutputStreamBuffer(data, static_cast<uint8_t*>(data) + size) {}

OutputStreamBuffer::OutputStreamBuffer(void* start, void* end)
    : pos_(static_cast<uint8_t*>(start)), end_(static_cast<uint8_t*>(end)) {
  NVRAM_CHECK(pos_ <= end_);
}

bool OutputStreamBuffer::Done() {
  return pos_ >= end_ && !Advance();
}

bool OutputStreamBuffer::Write(const void* data, size_t size) {
  const uint8_t* buffer = static_cast<const uint8_t*>(data);
  NVRAM_CHECK(pos_ <= end_);
  while (size > static_cast<size_t>(end_ - pos_)) {
    memcpy(pos_, buffer, end_ - pos_);
    buffer += end_ - pos_;
    size -= end_ - pos_;
    pos_ = end_;
    if (!Advance()) {
      return false;
    }
    NVRAM_CHECK(pos_ < end_);
  }
  memcpy(pos_, buffer, size);
  pos_ += size;
  return true;
}

bool OutputStreamBuffer::WriteByte(uint8_t byte) {
  if (pos_ >= end_) {
    if (!Advance()) {
      return false;
    }
    NVRAM_CHECK(pos_ < end_);
  }
  *pos_ = byte;
  ++pos_;
  return true;
}

bool OutputStreamBuffer::Advance() {
  return false;
}

CountingOutputStreamBuffer::CountingOutputStreamBuffer()
    : OutputStreamBuffer(scratch_space_, kScratchSpaceSize) {}

bool CountingOutputStreamBuffer::Advance() {
  bytes_written_ += pos_ - scratch_space_;
  pos_ = scratch_space_;
  end_ = scratch_space_ + kScratchSpaceSize;
  return true;
}

uint8_t CountingOutputStreamBuffer::scratch_space_[kScratchSpaceSize];

BlobOutputStreamBuffer::BlobOutputStreamBuffer(Blob* blob)
    : OutputStreamBuffer(blob->data(), blob->size()), blob_(blob) {}

bool BlobOutputStreamBuffer::Advance() {
  ptrdiff_t offset = pos_ - blob_->data();
  if (!blob_->Resize(max<size_t>(blob_->size() * 2, 32))) {
    return false;
  }
  pos_ = blob_->data() + offset;
  end_ = blob_->data() + blob_->size();
  return true;
}

bool BlobOutputStreamBuffer::Truncate() {
  if (!blob_->Resize(pos_ - blob_->data())) {
    return false;
  }
  end_ = blob_->data() + blob_->size();
  pos_ = end_;
  return true;
}

ProtoReader::ProtoReader(InputStreamBuffer* stream_buffer)
    : stream_buffer_(stream_buffer) {}

bool ProtoReader::ReadWireTag() {
  uint64_t wire_tag;
  if (!DecodeVarint(stream_buffer_, &wire_tag)) {
    return false;
  }

  wire_type_ = wire_tag & 0x7;
  field_number_ = wire_tag >> 3;
  switch (wire_type()) {
    case WireType::kLengthDelimited: {
      uint64_t size;
      if (!DecodeVarint(stream_buffer_, &size)) {
        return false;
      }
      field_size_ = static_cast<size_t>(size);
      if (static_cast<uint64_t>(field_size_) != size) {
        return false;
      }
      break;
    }
    case WireType::kFixed64:
      field_size_ = sizeof(uint64_t);
      break;
    case WireType::kFixed32:
      field_size_ = sizeof(uint32_t);
      break;
    case WireType::kVarint:
    case WireType::kStartGroup:
    case WireType::kEndGroup:
      field_size_ = 0;
      break;
  }

  return true;
}

bool ProtoReader::ReadVarint(uint64_t* value) {
  NVRAM_CHECK(wire_type() == WireType::kVarint);
  return DecodeVarint(stream_buffer_, value);
}

bool ProtoReader::ReadLengthDelimited(void* data, size_t size) {
  NVRAM_CHECK(wire_type() == WireType::kLengthDelimited);
  return stream_buffer_->Read(data, size);
}

bool ProtoReader::SkipField() {
  if (wire_type() == WireType::kVarint) {
    uint64_t dummy;
    return DecodeVarint(stream_buffer_, &dummy);
  } else if (field_size_ > 0) {
    return stream_buffer_->Skip(field_size_);
  }

  return true;
}

ProtoWriter::ProtoWriter(OutputStreamBuffer* stream_buffer)
    : stream_buffer_(stream_buffer) {}

bool ProtoWriter::WriteVarint(uint64_t value) {
  return WriteWireTag(WireType::kVarint) &&
         EncodeVarint(stream_buffer_, value);
}

bool ProtoWriter::WriteLengthDelimited(const void* data, size_t size) {
  return WriteWireTag(WireType::kLengthDelimited) &&
         EncodeVarint(stream_buffer_, size) &&
         stream_buffer_->Write(data, size);
}

bool ProtoWriter::WriteLengthHeader(size_t size) {
  return WriteWireTag(WireType::kLengthDelimited) &&
         EncodeVarint(stream_buffer_, size);
}

bool ProtoWriter::WriteWireTag(WireType wire_type) {
  return EncodeVarint(stream_buffer_,
                      (field_number_ << 3) | static_cast<uint64_t>(wire_type));
}

}  // namespace nvram
