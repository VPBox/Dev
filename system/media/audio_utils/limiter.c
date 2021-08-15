/*
 * Copyright (C) 2016 The Android Open Source Project
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
#include <audio_utils/limiter.h>

#undef USE_ATAN_APPROXIMATION

#ifdef USE_ATAN_APPROXIMATION
static inline float atan_approximation(float in)
{
    static const float kPi_2 = 1.5625;  // pi/2 ~= 2.0 * atan_approximation(1.0)
    if (in <= 1.0) {
        return in / (1.0 + 0.28 * in * in);
    } else {
        const float in_inverse = 1.0 / in;
        return kPi_2 - in_inverse / (1.0 + 0.28 * in_inverse * in_inverse);
    }
}
#endif

float limiter(float in)
{
    static const float crossover = M_SQRT1_2;
    float in_abs = fabsf(in);
    if (in_abs <= crossover) {
        return in;
    }
    float out;
#ifdef USE_ATAN_APPROXIMATION
    // atan approximation
    static const float factor = 4.3125;
    static const float ratio = 1.0 / factor;
    out = crossover + atan_approximation((in_abs - crossover)*factor)*ratio;
    if (out > 1.0) {
        out = 1.0;
    }
#else
    // polynomial spline
    static const float A = 0.3431457505;
    static const float B = -1.798989873;
    static const float C = 3.029437252;
    static const float D = -0.6568542495;
    // Cubic solution Ax^3 + Bx^2 + Cx + D
    if (in_abs < M_SQRT2) {
        // TODO Use Estrin's method P3
        out = ((A*in_abs + B)*in_abs + C)*in_abs + D;
    } else {
        out = 1.0;
    }
#endif
    if (in < 0) {
        out = -out;
    }
    return out;
}
