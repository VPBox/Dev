/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "gtest/gtest.h"

#include "chre/util/buffer.h"
#include "chre/util/macros.h"

using chre::Buffer;

void fillBufferWithSequentialValues(float *buffer, size_t size) {
  for (size_t i = 0; i < size; i++) {
    buffer[i] = static_cast<float>(i);
  }
}

TEST(Buffer, EmptyByDefault) {
  Buffer<float> buffer;
  EXPECT_EQ(buffer.data(), nullptr);
  EXPECT_EQ(buffer.size(), 0);
}

TEST(Buffer, Wrap) {
  float buf[128];
  fillBufferWithSequentialValues(buf, ARRAY_SIZE(buf));

  Buffer<float> buffer;
  buffer.wrap(buf, ARRAY_SIZE(buf));
  EXPECT_EQ(buffer.data(), buf);
  EXPECT_EQ(buffer.size(), ARRAY_SIZE(buf));
}

TEST(Buffer, CopyArray) {
  float buf[128];
  fillBufferWithSequentialValues(buf, ARRAY_SIZE(buf));

  Buffer<float> buffer;
  EXPECT_TRUE(buffer.copy_array(buf, ARRAY_SIZE(buf)));
  EXPECT_EQ(buffer.size(), ARRAY_SIZE(buf));

  for (size_t i = 0; i < ARRAY_SIZE(buf); i++) {
    EXPECT_EQ(buffer.data()[i], static_cast<float>(i));
  }
}

TEST(Buffer, CopyArrayEmpty) {
  Buffer<float> buffer;
  EXPECT_TRUE(buffer.copy_array(nullptr, 0));
  EXPECT_EQ(buffer.data(), nullptr);
  EXPECT_EQ(buffer.size(), 0);
}

TEST(Buffer, CopyArrayEmptyAfterWrap) {
  float buf[128];
  fillBufferWithSequentialValues(buf, ARRAY_SIZE(buf));

  Buffer<float> buffer;
  buffer.wrap(buf, ARRAY_SIZE(buf));
  EXPECT_TRUE(buffer.copy_array(nullptr, 0));
  EXPECT_EQ(buffer.data(), nullptr);
  EXPECT_EQ(buffer.size(), 0);
}

TEST(Buffer, CopyArrayEmptyAfterCopy) {
  float buf[128];
  fillBufferWithSequentialValues(buf, ARRAY_SIZE(buf));

  Buffer<float> buffer;
  EXPECT_TRUE(buffer.copy_array(buf, ARRAY_SIZE(buf)));
  EXPECT_TRUE(buffer.copy_array(nullptr, 0));
  EXPECT_EQ(buffer.data(), nullptr);
  EXPECT_EQ(buffer.size(), 0);
}
