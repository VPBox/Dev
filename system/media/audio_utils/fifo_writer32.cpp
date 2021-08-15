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

#include <atomic>
#include <stdlib.h>
#include <string.h>

// TODO templatize int32_t

#include <audio_utils/fifo_writer32.h>

static inline void memcpyWords(int32_t *dst, const int32_t *src, uint32_t count)
{
    switch (count) {
    case 0: break;
// TODO templatize here also, but first confirm no performance regression compared to current
#define _(n) \
    case n: { \
        struct s##n { int32_t a[n]; }; \
        *(struct s##n *)dst = *(const struct s##n *)src; \
        break; \
    }
    _(1) _(2) _(3) _(4) _(5) _(6) _(7) _(8) _(9) _(10) _(11) _(12) _(13) _(14) _(15) _(16)
#undef _
    default:
        memcpy(dst, src, count * sizeof(int32_t));
        break;
    }
}

audio_utils_fifo_writer32::audio_utils_fifo_writer32(audio_utils_fifo& fifo) :
    mLocalRear(0), mFrameCountP2(fifo.mFrameCountP2), mBuffer((int32_t *) fifo.mBuffer),
    mWriterRear(fifo.mWriterRear)
{
    if (fifo.mFrameSize != sizeof(int32_t) || fifo.mFudgeFactor != 0 ||
            ((size_t) mBuffer & ((sizeof(int32_t) - 1))) != 0) {
        abort();
    }
}

audio_utils_fifo_writer32::~audio_utils_fifo_writer32()
{
}

void audio_utils_fifo_writer32::write(const int32_t *buffer, uint32_t count)
        __attribute__((no_sanitize("integer")))     // mLocalRear += can wrap
{
    uint32_t availToWrite = mFrameCountP2;
    if (availToWrite > count) {
        availToWrite = count;
    }
    uint32_t rearOffset = mLocalRear & (mFrameCountP2 - 1);
    uint32_t part1 = mFrameCountP2 - rearOffset;
    if (part1 >  availToWrite) {
        part1 = availToWrite;
    }
    memcpyWords(&mBuffer[rearOffset], buffer, part1);
    // TODO apply this simplification to other copies of the code
    uint32_t part2 = availToWrite - part1;
    memcpyWords(&mBuffer[0], &buffer[part1], part2);
    mLocalRear += availToWrite;
}
