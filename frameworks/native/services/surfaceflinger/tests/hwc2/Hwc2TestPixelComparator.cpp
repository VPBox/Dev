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

#include <sstream>
#include <android/hardware/graphics/common/1.0/types.h>

#include "Hwc2TestPixelComparator.h"

using android::hardware::graphics::common::V1_0::BufferUsage;

uint32_t ComparatorResult::getPixel(int32_t x, int32_t y, uint32_t stride,
        uint8_t* img) const
{
    uint32_t r = img[(y * stride + x) * 4 + 0];
    uint32_t g = img[(y * stride + x) * 4 + 1];
    uint32_t b = img[(y * stride + x) * 4 + 2];
    uint32_t a = img[(y * stride + x) * 4 + 3];

    uint32_t pixel = 0;
    pixel |= r;
    pixel |= g << 8;
    pixel |= b << 16;
    pixel |= a << 24;
    return pixel;
}

void ComparatorResult::CompareBuffers(
        android::sp<android::GraphicBuffer>& resultBuffer,
        android::sp<android::GraphicBuffer>& expectedBuffer)
{
    uint8_t* resultBufferImg;
    uint8_t* expectedBufferImg;
    resultBuffer->lock(static_cast<uint32_t>(BufferUsage::CPU_READ_OFTEN),
            (void**)(&resultBufferImg));

    expectedBuffer->lock(static_cast<uint32_t>(BufferUsage::CPU_READ_OFTEN),
            (void**)(&expectedBufferImg));
    mComparisons.clear();
    int32_t mDifferentPixelCount = 0;
    int32_t mBlankPixelCount = 0;

    for (uint32_t y = 0; y < resultBuffer->getHeight(); y++) {
        for (uint32_t x = 0; x < resultBuffer->getWidth(); x++) {
            uint32_t result = getPixel(x, y, resultBuffer->getStride(),
                    resultBufferImg);
            uint32_t expected = getPixel(x, y, expectedBuffer->getStride(),
                    expectedBufferImg);

            if (result == 0)
                mBlankPixelCount++;

            if (result != expected)
                mDifferentPixelCount++;

            mComparisons.emplace_back(std::make_tuple(x, y, result, expected));
        }
    }
    resultBuffer->unlock();
    expectedBuffer->unlock();
}

std::string ComparatorResult::pixelDiff(uint32_t x, uint32_t y,
        uint32_t resultPixel, uint32_t expectedPixel) const
{
    uint32_t resultAlpha = (resultPixel >> 24) & 0xFF;
    uint32_t resultBlue = (resultPixel >> 16) & 0xFF;
    uint32_t resultGreen = (resultPixel >> 8) & 0xFF;
    uint32_t resultRed = resultPixel & 0xFF;

    uint32_t expectedAlpha = (expectedPixel >> 24) & 0xFF;
    uint32_t expectedBlue = (expectedPixel >> 16) & 0xFF;
    uint32_t expectedGreen = (expectedPixel >> 8) & 0xFF;
    uint32_t expectedRed = expectedPixel & 0xFF;

    std::ostringstream stream;

    stream << "x: " << x << " y: " << y << std::endl;
    stream << std::hex;
    stream << "Result pixel:   " << resultRed << "|" << resultGreen << "|"
           << resultBlue << "|" << resultAlpha << std::endl;

    stream << "Expected pixel: " << expectedRed << "|" << expectedGreen << "|"
           << expectedBlue << "|" << expectedAlpha << std::endl;

    return stream.str();
}

std::string ComparatorResult::dumpComparison() const
{
    std::ostringstream stream;
    stream << "Number of different pixels: " << mDifferentPixelCount;

    for (const auto& comparison : mComparisons) {
        if (std::get<2>(comparison) != std::get<3>(comparison))
            stream << pixelDiff(std::get<0>(comparison),
                    std::get<1>(comparison), std::get<2>(comparison),
                    std::get<3>(comparison));
    }
    return stream.str();
}
