/*
 * Copyright (C) 2017 The Android Open Source Project
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

#define LOG_TAG "AHardwareBuffer_test"
//#define LOG_NDEBUG 0

#include <android/hardware_buffer.h>
#include <private/android/AHardwareBufferHelpers.h>
#include <android/hardware/graphics/common/1.0/types.h>

#include <gtest/gtest.h>

using namespace android;
using android::hardware::graphics::common::V1_0::BufferUsage;

static ::testing::AssertionResult BuildHexFailureMessage(uint64_t expected,
        uint64_t actual, const char* type) {
    std::ostringstream ss;
    ss << type << " 0x" << std::hex << actual
            << " does not match expected " << type << " 0x" << std::hex
            << expected;
    return ::testing::AssertionFailure() << ss.str();
}

static ::testing::AssertionResult TestUsageConversion(
        uint64_t grallocUsage, uint64_t hardwareBufferUsage) {
    uint64_t convertedGrallocUsage = AHardwareBuffer_convertToGrallocUsageBits(hardwareBufferUsage);
    if (convertedGrallocUsage != grallocUsage)
        return BuildHexFailureMessage(grallocUsage, convertedGrallocUsage, "converToGralloc");

    uint64_t convertedHArdwareBufferUsage = AHardwareBuffer_convertFromGrallocUsageBits(grallocUsage);
    if (convertedHArdwareBufferUsage != grallocUsage)
        return BuildHexFailureMessage(grallocUsage, convertedHArdwareBufferUsage, "convertFromGralloc");

    return testing::AssertionSuccess();
}

// This is a unit test rather than going through AHardwareBuffer because not
// all flags may be supported by the host device.
TEST(AHardwareBufferTest, ConvertToAndFromGrallocBits) {
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::CPU_READ_RARELY,
            AHARDWAREBUFFER_USAGE_CPU_READ_RARELY));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::CPU_READ_OFTEN,
            AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::CPU_WRITE_RARELY,
            AHARDWAREBUFFER_USAGE_CPU_WRITE_RARELY));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::CPU_WRITE_OFTEN,
            AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN));

    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::GPU_TEXTURE,
            AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::GPU_RENDER_TARGET,
            AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::GPU_DATA_BUFFER,
            AHARDWAREBUFFER_USAGE_GPU_DATA_BUFFER));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::PROTECTED,
            AHARDWAREBUFFER_USAGE_PROTECTED_CONTENT));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::SENSOR_DIRECT_DATA,
            AHARDWAREBUFFER_USAGE_SENSOR_DIRECT_DATA));
    EXPECT_TRUE(TestUsageConversion((uint64_t)BufferUsage::VIDEO_ENCODER,
            AHARDWAREBUFFER_USAGE_VIDEO_ENCODE));

    EXPECT_TRUE(TestUsageConversion(1ull<<28, AHARDWAREBUFFER_USAGE_VENDOR_0));
    EXPECT_TRUE(TestUsageConversion(1ull<<29, AHARDWAREBUFFER_USAGE_VENDOR_1));
    EXPECT_TRUE(TestUsageConversion(1ull<<30, AHARDWAREBUFFER_USAGE_VENDOR_2));
    EXPECT_TRUE(TestUsageConversion(1ull<<31, AHARDWAREBUFFER_USAGE_VENDOR_3));
    EXPECT_TRUE(TestUsageConversion(1ull<<48, AHARDWAREBUFFER_USAGE_VENDOR_4));
    EXPECT_TRUE(TestUsageConversion(1ull<<49, AHARDWAREBUFFER_USAGE_VENDOR_5));
    EXPECT_TRUE(TestUsageConversion(1ull<<50, AHARDWAREBUFFER_USAGE_VENDOR_6));
    EXPECT_TRUE(TestUsageConversion(1ull<<51, AHARDWAREBUFFER_USAGE_VENDOR_7));
    EXPECT_TRUE(TestUsageConversion(1ull<<52, AHARDWAREBUFFER_USAGE_VENDOR_8));
    EXPECT_TRUE(TestUsageConversion(1ull<<53, AHARDWAREBUFFER_USAGE_VENDOR_9));
    EXPECT_TRUE(TestUsageConversion(1ull<<54, AHARDWAREBUFFER_USAGE_VENDOR_10));
    EXPECT_TRUE(TestUsageConversion(1ull<<55, AHARDWAREBUFFER_USAGE_VENDOR_11));
    EXPECT_TRUE(TestUsageConversion(1ull<<56, AHARDWAREBUFFER_USAGE_VENDOR_12));
    EXPECT_TRUE(TestUsageConversion(1ull<<57, AHARDWAREBUFFER_USAGE_VENDOR_13));
    EXPECT_TRUE(TestUsageConversion(1ull<<58, AHARDWAREBUFFER_USAGE_VENDOR_14));
    EXPECT_TRUE(TestUsageConversion(1ull<<59, AHARDWAREBUFFER_USAGE_VENDOR_15));
    EXPECT_TRUE(TestUsageConversion(1ull<<60, AHARDWAREBUFFER_USAGE_VENDOR_16));
    EXPECT_TRUE(TestUsageConversion(1ull<<61, AHARDWAREBUFFER_USAGE_VENDOR_17));
    EXPECT_TRUE(TestUsageConversion(1ull<<62, AHARDWAREBUFFER_USAGE_VENDOR_18));
    EXPECT_TRUE(TestUsageConversion(1ull<<63, AHARDWAREBUFFER_USAGE_VENDOR_19));

    // Test some more complex flag combinations.
    EXPECT_TRUE(TestUsageConversion(
            (uint64_t)BufferUsage::CPU_READ_RARELY |
            (uint64_t)BufferUsage::CPU_WRITE_RARELY,
            AHARDWAREBUFFER_USAGE_CPU_READ_RARELY | AHARDWAREBUFFER_USAGE_CPU_WRITE_RARELY));

EXPECT_TRUE(TestUsageConversion(
        (uint64_t)BufferUsage::GPU_RENDER_TARGET | (uint64_t)BufferUsage::GPU_TEXTURE |
            1ull << 29 | 1ull << 57,
        AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT | AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE |
        AHARDWAREBUFFER_USAGE_VENDOR_1 | AHARDWAREBUFFER_USAGE_VENDOR_13));
}
