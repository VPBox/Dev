/*
 * Copyright 2019 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <input/TouchVideoFrame.h>

namespace android {
namespace test {

static const struct timeval TIMESTAMP = {1, 2};

TEST(TouchVideoFrame, Constructor) {
    const std::vector<int16_t> data = {1, 2, 3, 4, 5, 6};
    constexpr uint32_t height = 3;
    constexpr uint32_t width = 2;

    TouchVideoFrame frame(height, width, data, TIMESTAMP);

    ASSERT_EQ(data, frame.getData());
    ASSERT_EQ(height, frame.getHeight());
    ASSERT_EQ(width, frame.getWidth());
    ASSERT_EQ(TIMESTAMP.tv_sec, frame.getTimestamp().tv_sec);
    ASSERT_EQ(TIMESTAMP.tv_usec, frame.getTimestamp().tv_usec);
}

TEST(TouchVideoFrame, Equality) {
    const std::vector<int16_t> data = {1, 2, 3, 4, 5, 6};
    constexpr uint32_t height = 3;
    constexpr uint32_t width = 2;
    TouchVideoFrame frame(height, width, data, TIMESTAMP);

    TouchVideoFrame identicalFrame(height, width, data, TIMESTAMP);
    ASSERT_EQ(frame, identicalFrame);

    // The two cases below create an invalid frame, but it is OK for comparison purposes.
    // There aren't any checks currently enforced on the frame dimensions and data
    // Change height
    TouchVideoFrame changedHeightFrame(height + 1, width, data, TIMESTAMP);
    ASSERT_FALSE(frame == changedHeightFrame);

    // Change width
    TouchVideoFrame changedWidthFrame(height, width + 1, data, TIMESTAMP);
    ASSERT_FALSE(frame == changedWidthFrame);

    // Change data
    const std::vector<int16_t> differentData = {1, 2, 3, 3, 5, 6};
    TouchVideoFrame changedDataFrame(height, width, differentData, TIMESTAMP);
    ASSERT_FALSE(frame == changedDataFrame);

    // Change timestamp
    const struct timeval differentTimestamp = {TIMESTAMP.tv_sec + 1, TIMESTAMP.tv_usec + 1};
    TouchVideoFrame changedTimestampFrame(height, width, data, differentTimestamp);
    ASSERT_FALSE(frame == changedTimestampFrame);
}

// --- Rotate 90 degrees ---

TEST(TouchVideoFrame, Rotate90_0x0) {
    TouchVideoFrame frame(0, 0, {}, TIMESTAMP);
    TouchVideoFrame frameRotated(0, 0, {}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_90);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate90_1x1) {
    TouchVideoFrame frame(1, 1, {1}, TIMESTAMP);
    TouchVideoFrame frameRotated(1, 1, {1}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_90);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate90_2x2) {
    TouchVideoFrame frame(2, 2, {1, 2, 3, 4}, TIMESTAMP);
    TouchVideoFrame frameRotated(2, 2, {3, 1, 4, 2}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_90);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate90_3x2) {
    TouchVideoFrame frame(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    TouchVideoFrame frameRotated(2, 3, {5, 3, 1, 6, 4, 2}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_90);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate90_3x2_4times) {
    TouchVideoFrame frame(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    TouchVideoFrame frameOriginal(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_90);
    frame.rotate(DISPLAY_ORIENTATION_90);
    frame.rotate(DISPLAY_ORIENTATION_90);
    frame.rotate(DISPLAY_ORIENTATION_90);
    ASSERT_EQ(frame, frameOriginal);
}

// --- Rotate 180 degrees ---

TEST(TouchVideoFrame, Rotate180_0x0) {
    TouchVideoFrame frame(0, 0, {}, TIMESTAMP);
    TouchVideoFrame frameRotated(0, 0, {}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_180);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate180_1x1) {
    TouchVideoFrame frame(1, 1, {1}, TIMESTAMP);
    TouchVideoFrame frameRotated(1, 1, {1}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_180);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate180_2x2) {
    TouchVideoFrame frame(2, 2, {1, 2, 3, 4}, TIMESTAMP);
    TouchVideoFrame frameRotated(2, 2, {4, 3, 2, 1}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_180);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate180_3x2) {
    TouchVideoFrame frame(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    TouchVideoFrame frameRotated(3, 2, {6, 5, 4, 3, 2, 1}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_180);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate180_3x2_2times) {
    TouchVideoFrame frame(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    TouchVideoFrame frameOriginal(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_180);
    frame.rotate(DISPLAY_ORIENTATION_180);
    ASSERT_EQ(frame, frameOriginal);
}

TEST(TouchVideoFrame, Rotate180_3x3) {
    TouchVideoFrame frame(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9}, TIMESTAMP);
    TouchVideoFrame frameRotated(3, 3, {9, 8, 7, 6, 5, 4, 3, 2, 1}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_180);
    ASSERT_EQ(frame, frameRotated);
}

// --- Rotate 270 degrees ---

TEST(TouchVideoFrame, Rotate270_0x0) {
    TouchVideoFrame frame(0, 0, {}, TIMESTAMP);
    TouchVideoFrame frameRotated(0, 0, {}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_270);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate270_1x1) {
    TouchVideoFrame frame(1, 1, {1}, TIMESTAMP);
    TouchVideoFrame frameRotated(1, 1, {1}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_270);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate270_2x2) {
    TouchVideoFrame frame(2, 2, {1, 2, 3, 4}, TIMESTAMP);
    TouchVideoFrame frameRotated(2, 2, {2, 4, 1, 3}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_270);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate270_3x2) {
    TouchVideoFrame frame(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    TouchVideoFrame frameRotated(2, 3, {2, 4, 6, 1, 3, 5}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_270);
    ASSERT_EQ(frame, frameRotated);
}

TEST(TouchVideoFrame, Rotate270_3x2_4times) {
    TouchVideoFrame frame(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    TouchVideoFrame frameOriginal(3, 2, {1, 2, 3, 4, 5, 6}, TIMESTAMP);
    frame.rotate(DISPLAY_ORIENTATION_270);
    frame.rotate(DISPLAY_ORIENTATION_270);
    frame.rotate(DISPLAY_ORIENTATION_270);
    frame.rotate(DISPLAY_ORIENTATION_270);
    ASSERT_EQ(frame, frameOriginal);
}

} // namespace test
} // namespace android
