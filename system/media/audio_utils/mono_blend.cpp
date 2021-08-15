/*
 * Copyright (C) 2015 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_mono_blend"

#include <math.h>
#include <log/log.h>
#include <audio_utils/limiter.h>
#include <audio_utils/mono_blend.h>

// TODO: Speed up for special case of 2 channels?
void mono_blend(void *buf, audio_format_t format, size_t channelCount, size_t frames, bool limit) {
    if (channelCount < 2) {
        return;
    }
    switch (format) {
    case AUDIO_FORMAT_PCM_16_BIT: {
        int16_t *out = (int16_t *)buf;
        for (size_t i = 0; i < frames; ++i) {
            const int16_t *in = out;
            int accum = 0;
            for (size_t j = 0; j < channelCount; ++j) {
                accum += *in++;
            }
            accum /= channelCount; // round to 0
            for (size_t j = 0; j < channelCount; ++j) {
                *out++ = accum;
            }
        }
    } break;
    case AUDIO_FORMAT_PCM_FLOAT: {
        float *out = (float *)buf;
        const float recipdiv = 1. / channelCount;
        for (size_t i = 0; i < frames; ++i) {
            const float *in = out;
            float accum = 0;
            for (size_t j = 0; j < channelCount; ++j) {
                accum += *in++;
            }
            if (limit && channelCount == 2) {
                accum = limiter(accum * M_SQRT1_2);
            } else {
                accum *= recipdiv;
            }
            for (size_t j = 0; j < channelCount; ++j) {
                *out++ = accum;
            }
        }
    } break;
    default:
        ALOGE("mono_blend: invalid format %d", format);
        break;
    }
}
