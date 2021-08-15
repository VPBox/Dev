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

#ifndef NVRAM_MESSAGES_IO_H_
#define NVRAM_MESSAGES_IO_H_

extern "C" {
#include <stddef.h>
#include <stdint.h>
}

#include <nvram/messages/blob.h>
#include <nvram/messages/compiler.h>

namespace nvram {

// Abstraction used by the protobuf decoder to read data. The idea here is that
// |InputStreamBuffer| maintains a window of the data to be read. Access to the
// contents of the current window is direct, i.e. doesn't need to go through
// virtual dispatch to subclasses. Whenever the window is exhausted, the next
// window must be set up. This latter operation is left for implementation of
// the virtual |Advance()| member function in subclasses, which is entirely free
// to pull its data from anywhere.
class NVRAM_EXPORT InputStreamBuffer {
 public:
  InputStreamBuffer() = default;
  InputStreamBuffer(const void* data, size_t size);
  InputStreamBuffer(const void* start, const void* end);
  virtual ~InputStreamBuffer() = default;

  // Checks whether the stream is exhausted;
  bool Done();

  // Consume |size| bytes from the stream and store them in the provided |data|
  // buffer. Returns false if insufficient bytes are available.
  bool Read(void* data, size_t size);

  // Consume a single byte and place it in |byte|. Returns true if successful,
  // i.e. if there was a byte available.
  bool ReadByte(uint8_t* byte);

  // Discard |size| bytes from the stream. Returns false if there are fewer
  // bytes available.
  bool Skip(size_t size);

 protected:
  // Update the |pos_| and |end_| pointers for the next buffer window. Returns
  // true if the window was successfully set up, false on I/O errors or stream
  // exhaustion. The default implementation just returns false to signal
  // immediate stream exhaustion. Subclasses should override this to pull in
  // more data from the underlying data source.
  virtual bool Advance();

  // Pointers to the buffer to read from. |InputStreamBuffer| only advances
  // |pos_| until reaching |end_|. At this point, |Advance| is called for the
  // subclass to initialize the next buffer window and update the pointers.
  const uint8_t* pos_ = nullptr;
  const uint8_t* end_ = nullptr;

  // Allow |NestedInputStreamBuffer| to mess with |pos_| and |end_|, also in its
  // delegate, which isn't necessarily of type |NestedInputStreamBuffer|.
  friend class NestedInputStreamBuffer;
};

// An |InputStreamBuffer| implementation that pulls its data from a delegate,
// but only up to a predetermined limit of bytes.
class NVRAM_EXPORT NestedInputStreamBuffer : public InputStreamBuffer {
 public:
  // Initialize a |NestedInputStreamBuffer| to provide at most |size| bytes from
  // |delegate|. Note that |delegate| must remain valid throughout the life time
  // of this |NestedInputStreamBuffer|.
  NestedInputStreamBuffer(InputStreamBuffer* delegate, size_t size);
  ~NestedInputStreamBuffer() override = default;

 private:
  // InputStreamBuffer:
  bool Advance() override;

  // Determine the input window end based on |delegate|'s current window and the
  // requested |size| to read. If |size| can be satisfied from |delegate|'s
  // current window, return an end pointer within that window. If size is larger
  // than the remaining bytes available in |delegate|'s input window, return
  // |delegate|'s |end_| pointer.
  static const uint8_t* ClampEnd(InputStreamBuffer* delegate, size_t size);

  InputStreamBuffer* delegate_;
  size_t remaining_;
};

// Abstraction used by the protobuf decoder to output data. This class maintains
// a current window of memory to write output to. Access to the current window's
// bytes is direct and doesn't require virtual dispatch. Once the capacity of
// the current window is exhausted, the virtual |Advance()| member function is
// invoked to set up a new window. Subclasses are entirely free to implement
// this operation as appropriate for their I/O mechanism, for example a
// socket-based implementations might flush the buffer to the socket and reset
// the window pointers to accept more output.
class NVRAM_EXPORT OutputStreamBuffer {
 public:
  OutputStreamBuffer() = default;
  OutputStreamBuffer(void* data, size_t size);
  OutputStreamBuffer(void* data, void* end);
  virtual ~OutputStreamBuffer() = default;

  // Checks whether the stream is exhausted.
  bool Done();

  // Writes a blob of |size| bytes provided in |data| to the underlying buffer.
  // Returns false if there is not enough space available.
  bool Write(const void* data, size_t size);

  // Writes |byte| to the underlying buffer. Returns false if there is not
  // enough space available.
  bool WriteByte(uint8_t byte);

 protected:
  // Set up the next data buffer window in |pos_| and |end_|. Returns true on
  // success, false on I/O errors or stream exhaustion. The default
  // implementation unconditionally returns false, i.e. signaling stream
  // exhaustion after the initial window is filled. Subclasses should override
  // this to flush buffers to the underlying data sink and set up a fresh buffer
  // for more data as appropriate.
  virtual bool Advance();

  // The |pos_| and |end_| pointers define a window of writable buffer space for
  // |OutputStreamBuffer| to place data in. |pos_| grows towards |end_| as
  // writes occur. Once |pos_| hits |end_|, |OutputStreamBuffer| will call
  // |Advance|, which subclasses can implement to provide a new buffer window in
  // |pos_| and |end_|.
  uint8_t* pos_ = nullptr;
  uint8_t* end_ = nullptr;
};

// An |OutputStreamBuffer| backed by a single data buffer.
class NVRAM_EXPORT ArrayOutputStreamBuffer : public OutputStreamBuffer {
 public:
  ArrayOutputStreamBuffer() = default;
  ArrayOutputStreamBuffer(void* data, size_t size)
      : OutputStreamBuffer(data, size), data_(pos_) {}
  ArrayOutputStreamBuffer(void* data, void* end)
      : OutputStreamBuffer(data, end), data_(pos_) {}
  ~ArrayOutputStreamBuffer() override = default;

  // Returns the number of bytes already written.
  size_t bytes_written() const { return pos_ - data_; }

 private:
  uint8_t* data_ = nullptr;
};

// An |OutputStream| implementation that doesn't write anything, but just counts
// the number of bytes written.
class NVRAM_EXPORT CountingOutputStreamBuffer : public OutputStreamBuffer {
 public:
  CountingOutputStreamBuffer();
  ~CountingOutputStreamBuffer() override = default;

  size_t bytes_written() const {
    return bytes_written_ + (pos_ - scratch_space_);
  }

 protected:
  // OutputStreamBuffer:
  bool Advance() override;

 private:
  // We share a single scratch buffer that all |CountingOutputStreamBuffer|
  // instances use as the destination for writes. Its contents are pretty much
  // unpredictable.
  //
  // TODO(mnissler): This adds a static 256 bytes memory allocation to each
  // process linking to this code. If that becomes a problem, we might want to
  // be smarter here and dynamically allocate a chunk of memory only when it's
  // needed, or maybe even map some address space that's not even backed by
  // actual memory (not sure that's possible).
  static constexpr size_t kScratchSpaceSize = 256;
  static uint8_t scratch_space_[kScratchSpaceSize];

  // Number of bytes that had been written when the last |Advance()| call
  // occurred.
  size_t bytes_written_ = 0;
};

// An |OutputStreamBuffer| implementation that stores all data to a wrapped
// |Blob|, growing it as necessary.
class NVRAM_EXPORT BlobOutputStreamBuffer : public OutputStreamBuffer {
 public:
  // Construct a |BlobOutputStreamBuffer| that stores all written data to
  // |blob|, which will get resized as necessary. Note that |blob| must remain
  // valid for the life time of the |BlobOutputStreamBuffer| object.
  explicit BlobOutputStreamBuffer(Blob* blob);
  ~BlobOutputStreamBuffer() override = default;

  // Truncate the blob to match the current output size.
  bool Truncate();

 protected:
  // OutputStreamBuffer:
  bool Advance() override;

 private:
  Blob* blob_;
};

// Protobuf wire types.
enum class WireType : int8_t {
  kVarint = 0,
  kFixed64 = 1,
  kLengthDelimited = 2,
  kStartGroup = 3,
  kEndGroup = 4,
  kFixed32 = 5,
};

// A class implementing a parser for the low-level protobuf wire format. It
// obtains raw data from a wrapped |InputStream| and offers member functions
// that facilitate decoding the data according to the protobuf wire format.
class NVRAM_EXPORT ProtoReader {
 public:
  // Construct a new |ProtoReader| that consumes data from |stream_buffer|.
  // |stream_buffer| must remain valid throughout the life time of the
  // |ProtoReader|.
  explicit ProtoReader(InputStreamBuffer* stream_buffer);

  // Access to the underlying stream buffer.
  InputStreamBuffer* stream_buffer() { return stream_buffer_; }

  // Wire type of the current field.
  WireType wire_type() const { return static_cast<WireType>(wire_type_); }

  // Field number of the current field.
  uint64_t field_number() const { return field_number_; }

  // Size of the field data, if known in advance.
  size_t field_size() const { return field_size_; }

  // Whether all data is consumed.
  bool Done() const { return stream_buffer_->Done(); }

  // Reads the next wire tag from the current position in the underlying
  // |stream_buffer_| and initializes internal fields. Previous state is
  // discarded silently.
  bool ReadWireTag();

  // Read a varint-encoded field and advances to the next field. Returns true if
  // successful.
  bool ReadVarint(uint64_t* value);

  // Read field data. Checks that |size| matches |field_size()| and copies out
  // the data to the provided |data| buffer. Advances to the next field and
  // returns true if successful.
  bool ReadLengthDelimited(void* data, size_t size);

  // Skips over the current field data.
  bool SkipField();

 private:
  static constexpr int8_t kInvalidWireType = -1;

  InputStreamBuffer* stream_buffer_;

  // Information about the current field. |wire_type == kInvalidWireType|
  // indicates that there is no current field to be consumed.
  int8_t wire_type_ = kInvalidWireType;
  uint64_t field_number_ = 0;
  size_t field_size_ = 0;
};

// |ProtoWriter| contains logic to write raw data according to the protobuf wire
// format to an |OutputStreamBuffer|.
class NVRAM_EXPORT ProtoWriter {
 public:
  // Construct a |ProtoWriter| which will send its output to |stream_buffer|.
  // |stream_buffer| must remain valid for the life time of the |ProtoWriter|.
  explicit ProtoWriter(OutputStreamBuffer* stream_buffer);

  // Access to the underlying stream buffer.
  OutputStreamBuffer* stream_buffer() { return stream_buffer_; }

  // Sets the field number to use when emitting a tag.
  void set_field_number(uint64_t field_number) { field_number_ = field_number; }

  // Whether the writer has exhausted the underlying |OutputStream|'s capacity.
  bool Done() const { return stream_buffer_->Done(); }

  // Write |value| as a varint-encoded field. Returns true if successful, i.e.
  // the data was successfully written to |stream_buffer_|.
  bool WriteVarint(uint64_t value);

  // Write |size| bytes stored at |data| to |stream_buffer_|. Returns true if
  // successful, i.e. the data was successfully written to |stream_buffer_|.
  bool WriteLengthDelimited(const void* data, size_t size);

  // Writes a wire tag for a length-delimited field, followed by a length
  // indication for |size| data bytes. It is up to the caller to emit exactly
  // |size| bytes to |stream_buffer()|, otherwise the encoded data will be
  // malformed.
  bool WriteLengthHeader(size_t size);

 private:
  // A helper to write a wire tag using the current field number and the
  // provided wire type.
  bool WriteWireTag(WireType wire_type);

  OutputStreamBuffer* stream_buffer_;
  uint64_t field_number_ = 0;
};

}  // namespace nvram

#endif  // NVRAM_MESSAGES_IO_H_
