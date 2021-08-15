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
#ifndef _HWC2_TEST_PIXEL_COMPARATOR_H
#define _HWC2_TEST_PIXEL_COMPARATOR_H

#include <ui/GraphicBuffer.h>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

class ComparatorResult {
public:
    static ComparatorResult& get()
    {
        static ComparatorResult instance;
        return instance;
    }

    void CompareBuffers(android::sp<android::GraphicBuffer>& resultBuffer,
            android::sp<android::GraphicBuffer>& expectedBuffer);

    std::string dumpComparison() const;

    ComparatorResult(const ComparatorResult&) = delete;
    ComparatorResult(ComparatorResult&&) = delete;
    ComparatorResult& operator=(ComparatorResult const&) = delete;
    ComparatorResult& operator=(ComparatorResult&&) = delete;

    int32_t getDifferentPixelCount() const { return mDifferentPixelCount; }
    int32_t getBlankPixelCount() const { return mBlankPixelCount; }

private:
    ComparatorResult() = default;
    uint32_t getPixel(int32_t x, int32_t y, uint32_t stride, uint8_t* img) const;
    std::string pixelDiff(uint32_t x, uint32_t y, uint32_t resultPixel,
            uint32_t expectedPixel) const;

    int32_t mDifferentPixelCount;
    int32_t mBlankPixelCount;
    /* std::tuple<X coordinate, Y coordinate, resultPixel, expectedPixel> */
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>>
            mComparisons;
};

#endif /* ifndef _HWC2_TEST_PIXEL_COMPARATOR_H */
