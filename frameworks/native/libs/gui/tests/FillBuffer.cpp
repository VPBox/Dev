/*
 * Copyright 2013 The Android Open Source Project
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

#include "FillBuffer.h"

#include <ui/GraphicBuffer.h>

#include <gtest/gtest.h>

namespace android {

void fillYV12Buffer(uint8_t* buf, int w, int h, int stride) {
    const int blockWidth = w > 16 ? w / 16 : 1;
    const int blockHeight = h > 16 ? h / 16 : 1;
    const int yuvTexOffsetY = 0;
    int yuvTexStrideY = stride;
    int yuvTexOffsetV = yuvTexStrideY * h;
    int yuvTexStrideV = (yuvTexStrideY/2 + 0xf) & ~0xf;
    int yuvTexOffsetU = yuvTexOffsetV + yuvTexStrideV * h/2;
    int yuvTexStrideU = yuvTexStrideV;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int parityX = (x / blockWidth) & 1;
            int parityY = (y / blockHeight) & 1;
            unsigned char intensity = (parityX ^ parityY) ? 63 : 191;
            buf[yuvTexOffsetY + (y * yuvTexStrideY) + x] = intensity;
            if (x < w / 2 && y < h / 2) {
                buf[yuvTexOffsetU + (y * yuvTexStrideU) + x] = intensity;
                if (x * 2 < w / 2 && y * 2 < h / 2) {
                    buf[yuvTexOffsetV + (y*2 * yuvTexStrideV) + x*2 + 0] =
                    buf[yuvTexOffsetV + (y*2 * yuvTexStrideV) + x*2 + 1] =
                    buf[yuvTexOffsetV + ((y*2+1) * yuvTexStrideV) + x*2 + 0] =
                    buf[yuvTexOffsetV + ((y*2+1) * yuvTexStrideV) + x*2 + 1] =
                        intensity;
                }
            }
        }
    }
}

void fillYV12BufferRect(uint8_t* buf, int w, int h, int stride,
        const android_native_rect_t& rect) {
    const int yuvTexOffsetY = 0;
    int yuvTexStrideY = stride;
    int yuvTexOffsetV = yuvTexStrideY * h;
    int yuvTexStrideV = (yuvTexStrideY/2 + 0xf) & ~0xf;
    int yuvTexOffsetU = yuvTexOffsetV + yuvTexStrideV * h/2;
    int yuvTexStrideU = yuvTexStrideV;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            bool inside = rect.left <= x && x < rect.right &&
                    rect.top <= y && y < rect.bottom;
            buf[yuvTexOffsetY + (y * yuvTexStrideY) + x] = inside ? 240 : 64;
            if (x < w / 2 && y < h / 2) {
                bool inside = rect.left <= 2*x && 2*x < rect.right &&
                        rect.top <= 2*y && 2*y < rect.bottom;
                buf[yuvTexOffsetU + (y * yuvTexStrideU) + x] = 16;
                buf[yuvTexOffsetV + (y * yuvTexStrideV) + x] =
                        inside ? 16 : 255;
            }
        }
    }
}

void fillRGBA8Buffer(uint8_t* buf, int w, int h, int stride) {
    const size_t PIXEL_SIZE = 4;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            off_t offset = (y * stride + x) * PIXEL_SIZE;
            for (int c = 0; c < 4; c++) {
                int parityX = (x / (1 << (c+2))) & 1;
                int parityY = (y / (1 << (c+2))) & 1;
                buf[offset + c] = (parityX ^ parityY) ? 231 : 35;
            }
        }
    }
}

void produceOneRGBA8Frame(const sp<ANativeWindow>& anw) {
    android_native_buffer_t* anb;
    ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(anw.get(),
            &anb));
    ASSERT_TRUE(anb != nullptr);

    sp<GraphicBuffer> buf(GraphicBuffer::from(anb));

    uint8_t* img = nullptr;
    ASSERT_EQ(NO_ERROR, buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN,
            (void**)(&img)));
    fillRGBA8Buffer(img, buf->getWidth(), buf->getHeight(), buf->getStride());
    ASSERT_EQ(NO_ERROR, buf->unlock());
    ASSERT_EQ(NO_ERROR, anw->queueBuffer(anw.get(), buf->getNativeBuffer(),
            -1));
}
} // namespace android
