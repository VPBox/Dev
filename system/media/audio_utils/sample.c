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

#include <math.h>
#include <audio_utils/sample.h>

#define SAMPLE_NEG_MAX  0xFFFF
#define SAMPLE_POS_MAX  0x7FFF

#define SIGN_MASK       0x8000
#define SIGN_BITS       1

#define EXPONENT_BITS   3
#define EXPONENT_MAX    ((1 << EXPONENT_BITS) - 1)
#define EXCESS          ((1 << EXPONENT_BITS) - 2)

#define MANTISSA_BITS   12
#define MANTISSA_MAX    ((1 << MANTISSA_BITS) - 1)
#define HIDDEN_BIT      (1 << MANTISSA_BITS)
#define ONE_FLOAT       ((float) (1 << (MANTISSA_BITS + 1)))


#if SIGN_BITS + EXPONENT_BITS + MANTISSA_BITS != 16
#error SIGN_BITS, EXPONENT_BITS and MANTISSA_BITS must sum to 16
#endif

sample_minifloat_t sample_from_float(float v)
{
    if (isnan(v)) {
        return 0;
    }
    sample_minifloat_t sign = 0;
    if (v < 0.0f) {
        sign = SIGN_MASK;
        v = -v;
    }
    // This check could conceivably be tighter: v < constexpr float_from_sample(1).
    // Probably only useful to be made more accurate if this is changed to
    // manipulate the raw IEEE single precision float bit fields.
    if (v <= 0.0f) {
        // originally returned sign, but now seems better not to return negative 0
        return 0;
    }
    if (v >= 2.0f) {
        return SAMPLE_POS_MAX | sign;
    }
    int exp;
    float r = frexpf(v, &exp);
    if ((exp += EXCESS) > EXPONENT_MAX) {
        return SAMPLE_POS_MAX | sign;
    }
    if (-exp >= MANTISSA_BITS) {
        // originally returned sign, but now seems better not to return negative 0
        return 0;
    }
    int mantissa = (int) (r * ONE_FLOAT);
    sample_minifloat_t ret = exp > 0 ? (exp << MANTISSA_BITS) | (mantissa & ~HIDDEN_BIT) :
            (mantissa >> (1 - exp)) & MANTISSA_MAX;
    // ret != 0
    return ret | sign;
}

float float_from_sample(sample_minifloat_t a)
{
    int mantissa = a & MANTISSA_MAX;
    int exponent = (a >> MANTISSA_BITS) & EXPONENT_MAX;
    float f = ldexpf((exponent > 0 ? HIDDEN_BIT | mantissa : mantissa << 1) / ONE_FLOAT,
            exponent - EXCESS);
    return a & SIGN_MASK ? -f : f;
}
