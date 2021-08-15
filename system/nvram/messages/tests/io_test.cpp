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

#include <stdlib.h>
#include <string.h>

#include <gtest/gtest.h>

#include <nvram/messages/io.h>

namespace nvram {

namespace {

// A simple |InputStreamBuffer| implementation that sets up a sequence of
// windows of |sizes| specified by the template parameters. Each byte read from
// the buffer has a value corresponding to its position in the stream.
template<size_t... sizes>
class TestInputStreamBuffer : public InputStreamBuffer {
 public:
  TestInputStreamBuffer() {
    Advance();
  }

 private:
  bool Advance() override {
    if (index_ >= (sizeof(kSizes) / sizeof(kSizes[0]))) {
      return false;
    }

    memset(buffer, 0xff, kMaxSize);
    const size_t size = kSizes[index_] < kMaxSize ? kSizes[index_] : kMaxSize;
    pos_ = buffer;
    end_ = buffer + size;
    for (uint8_t* p = buffer; p < end_; ++p) {
      *p = static_cast<uint8_t>(count_++ % 256);
    }
    ++index_;
    return true;
  }

  static constexpr size_t kMaxSize = 256;
  static constexpr size_t kSizes[] = { sizes... };

  uint8_t buffer[kMaxSize];
  size_t index_ = 0;
  size_t count_ = 0;
};

template<size_t... sizes>
constexpr size_t TestInputStreamBuffer<sizes...>::kSizes[];

// Tests whether a read of the given size returns the correct data, i.e. bytes
// with consecutive values starting at |pos|.
void CheckRead(InputStreamBuffer* buffer, size_t size, size_t pos) {
  uint8_t data[256];
  ASSERT_LE(size, sizeof(data));
  EXPECT_TRUE(buffer->Read(data, size));
  for (uint8_t* p = data; p < data + size; ++p) {
    EXPECT_EQ(pos++ % 256, *p);
  }
}

}  // namespace

TEST(InputStreamBufferTest, Basic) {
  TestInputStreamBuffer<10> buf;
  EXPECT_FALSE(buf.Done());

  uint8_t byte = 0;
  EXPECT_TRUE(buf.ReadByte(&byte));
  EXPECT_EQ(0, byte);
  EXPECT_FALSE(buf.Done());

  CheckRead(&buf, 6, 1);
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.Skip(3));
  EXPECT_TRUE(buf.Done());
}

TEST(InputStreamBufferTest, Empty) {
  InputStreamBuffer buf(nullptr, nullptr);
  EXPECT_TRUE(buf.Done());
  uint8_t byte = 0;
  EXPECT_FALSE(buf.ReadByte(&byte));
}

TEST(InputStreamBufferTest, LargeRead) {
  TestInputStreamBuffer<10> buf;
  uint8_t read_buf[10];
  EXPECT_FALSE(buf.Read(read_buf, SIZE_MAX));
}

TEST(InputStreamBufferTest, LargeSkip) {
  TestInputStreamBuffer<10> buf;
  EXPECT_FALSE(buf.Skip(SIZE_MAX));
}

TEST(InputStreamBufferTest, OverlappingReadByte) {
  TestInputStreamBuffer<1, 1> buf;

  uint8_t byte = 0;
  EXPECT_TRUE(buf.ReadByte(&byte));
  EXPECT_EQ(0, byte);
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.ReadByte(&byte));
  EXPECT_EQ(1, byte);
  EXPECT_TRUE(buf.Done());
}

TEST(InputStreamBufferTest, OverlappingRead) {
  TestInputStreamBuffer<10, 10, 10> buf;
  CheckRead(&buf, 15, 0);
  CheckRead(&buf, 10, 15);
  CheckRead(&buf, 5, 25);
  EXPECT_TRUE(buf.Done());
}

TEST(InputStreamBufferTest, OverlappingSkip) {
  TestInputStreamBuffer<10, 10, 10> buf;
  EXPECT_TRUE(buf.Skip(15));
  EXPECT_TRUE(buf.Skip(10));
  EXPECT_TRUE(buf.Skip(5));
  EXPECT_TRUE(buf.Done());
}

TEST(NestedInputStreamBufferTest, Large) {
  TestInputStreamBuffer<10> buf;
  NestedInputStreamBuffer nested(&buf, SIZE_MAX);
  EXPECT_FALSE(nested.Skip(SIZE_MAX));
}

TEST(NestedInputStreamBufferTest, Short) {
  TestInputStreamBuffer<10> buf;
  NestedInputStreamBuffer nested(&buf, 5);
  CheckRead(&nested, 5, 0);
  EXPECT_TRUE(nested.Done());
  EXPECT_FALSE(nested.Skip(1));
}

TEST(NestedInputStreamBufferTest, Matching) {
  TestInputStreamBuffer<10, 5> buf;
  NestedInputStreamBuffer nested(&buf, 10);
  CheckRead(&nested, 10, 0);
  EXPECT_TRUE(nested.Done());
  EXPECT_FALSE(nested.Skip(1));
}

TEST(NestedInputStreamBufferTest, Overlapping) {
  TestInputStreamBuffer<2, 3, 5, 8> buf;
  NestedInputStreamBuffer nested(&buf, 16);
  CheckRead(&nested, 8, 0);
  EXPECT_FALSE(nested.Done());
  CheckRead(&nested, 8, 8);
  EXPECT_TRUE(nested.Done());
  EXPECT_FALSE(nested.Skip(1));
}

namespace {

// An |OutputStreamBuffer| implementation backed by a sequence of buffer windows
// of |sizes| specified as template parameters. The output is expected to be
// sequential byte values starting at 0.
template<size_t... sizes>
class TestOutputStreamBuffer : public OutputStreamBuffer {
 public:
  TestOutputStreamBuffer() {
    Advance();
  }

  ~TestOutputStreamBuffer() {
    EXPECT_TRUE(Verify());
  }

  bool Verify() {
    for (; check_pos_ < pos_; check_pos_++, count_++) {
      data_matches_ &= *check_pos_ == (count_ % 256);
    }

    return data_matches_;
  }

 private:
  bool Advance() override {
    if (index_ >= (sizeof(kSizes) / sizeof(kSizes[0]))) {
      return false;
    }

    pos_ = end_;
    Verify();

    memset(buffer, 0xff, kMaxSize);
    const size_t size = kSizes[index_] < kMaxSize ? kSizes[index_] : kMaxSize;
    pos_ = buffer;
    check_pos_ = buffer;
    end_ = buffer + size;
    ++index_;
    return true;
  }

  static constexpr size_t kMaxSize = 256;
  static constexpr size_t kSizes[] = { sizes... };

  uint8_t buffer[kMaxSize];
  size_t index_ = 0;

  // The pointer in buffer until which the data has been checked to match the
  // expectations.
  uint8_t* check_pos_ = nullptr;

  // The counter that determines the expected value for the buffer bytes.
  size_t count_ = 0;

  // Whether all bytes that have been checked so far had the expected value.
  bool data_matches_ = true;
};

template<size_t... sizes>
constexpr size_t TestOutputStreamBuffer<sizes...>::kSizes[];

// Writes a buffer of |size| to |buf|. The buffer contains consecutive byte
// value starting at pos.
void WriteBuf(OutputStreamBuffer* buffer, size_t size, size_t pos) {
  uint8_t data[1024];
  ASSERT_LE(size, sizeof(data));
  for (uint8_t* p = data; p < data + size; ++p) {
    *p = pos++ % 256;
  }
  EXPECT_TRUE(buffer->Write(data, size));
}

}  // namespace

TEST(OutputStreamBufferTest, Basic) {
  TestOutputStreamBuffer<10> buf;
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.WriteByte(0));
  EXPECT_TRUE(buf.WriteByte(1));
  EXPECT_FALSE(buf.Done());
  EXPECT_TRUE(buf.Verify());

  WriteBuf(&buf, 6, 2);
  EXPECT_FALSE(buf.Done());
  EXPECT_TRUE(buf.Verify());

  WriteBuf(&buf, 2, 8);
  EXPECT_TRUE(buf.Done());
}

TEST(OutputStreamBufferTest, Empty) {
  OutputStreamBuffer buf(nullptr, nullptr);
  EXPECT_TRUE(buf.Done());
  EXPECT_FALSE(buf.WriteByte(0));
}

TEST(OutputStreamBufferTest, ShortWrite) {
  TestOutputStreamBuffer<10> buf;
  WriteBuf(&buf, 5, 0);
}

TEST(OutputStreamBufferTest, LargeWrite) {
  TestOutputStreamBuffer<5> buf;
  uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_FALSE(buf.Write(data, sizeof(data)));
}

TEST(OutputStreamBufferTest, OverlappingWriteByte) {
  TestOutputStreamBuffer<1, 1> buf;
  EXPECT_TRUE(buf.WriteByte(0));
  EXPECT_FALSE(buf.Done());
  EXPECT_TRUE(buf.WriteByte(1));
  EXPECT_TRUE(buf.Done());
}

TEST(OutputStreamBufferTest, OverlappingWrite) {
  TestOutputStreamBuffer<10, 10, 10> buf;
  WriteBuf(&buf, 15, 0);
  EXPECT_FALSE(buf.Done());
  WriteBuf(&buf, 10, 15);
  EXPECT_FALSE(buf.Done());
  WriteBuf(&buf, 5, 25);
  EXPECT_TRUE(buf.Done());
}

TEST(CountingOutputStreamBuffer, Basic) {
  CountingOutputStreamBuffer buf;
  EXPECT_EQ(0U, buf.bytes_written());
  EXPECT_FALSE(buf.Done());

  WriteBuf(&buf, 15, 0);
  EXPECT_EQ(15U, buf.bytes_written());
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.WriteByte(0));
  EXPECT_EQ(16U, buf.bytes_written());
  EXPECT_FALSE(buf.Done());

  WriteBuf(&buf, 1024, 0);
  EXPECT_EQ(1040U, buf.bytes_written());
  EXPECT_FALSE(buf.Done());
}

TEST(BlobOutputStreamBuffer, Basic) {
  Blob blob;
  ASSERT_TRUE(blob.Resize(1024 * 1024));
  BlobOutputStreamBuffer buf(&blob);

  WriteBuf(&buf, 15, 0);
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.WriteByte(15));
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.Truncate());
  EXPECT_EQ(16U, blob.size());
  for (size_t i = 0; i < blob.size(); ++i) {
    EXPECT_EQ(i % 256, blob.data()[i]);
  }

  WriteBuf(&buf, 1024, 16);
  EXPECT_FALSE(buf.Done());

  EXPECT_TRUE(buf.Truncate());
  EXPECT_EQ(1040U, blob.size());
  for (size_t i = 0; i < blob.size(); ++i) {
    EXPECT_EQ(i % 256, blob.data()[i]);
  }
}

}  // namespace nvram
