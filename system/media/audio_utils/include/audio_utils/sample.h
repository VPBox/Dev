/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_SAMPLE_H
#define ANDROID_AUDIO_SAMPLE_H

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/* A single signed sample expressed as minifloat, in linear range
 *   -2.0f < x < 2.0f
 * normalized precision about 4 decimal digits, and dynamic range of approximately -104 dB to +6 dB.
 * Representation has 13 significand bits (1 sign, 12 mantissa, 1 hidden) and 3 exponent bits.
 * There is no signed zero because of lack of hardware support for efficient comparisons,
 * no infinity, and no NaN.
 */
typedef int16_t sample_minifloat_t;
sample_minifloat_t sample_from_float(float f);
float float_from_sample(sample_minifloat_t sample);

__END_DECLS

#endif // ANDROID_AUDIO_SAMPLE_H
