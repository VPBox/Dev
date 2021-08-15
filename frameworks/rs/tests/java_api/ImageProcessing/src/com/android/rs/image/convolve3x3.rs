/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "ip.rsh"
#pragma rs_fp_relaxed

int32_t gWidth;
int32_t gHeight;
rs_allocation gIn;

float gCoeffs[9];

uchar4 RS_KERNEL root(uint32_t x, uint32_t y) {
    uint32_t x1 = min((int32_t)x+1, gWidth-1);
    uint32_t x2 = max((int32_t)x-1, 0);
    uint32_t y1 = min((int32_t)y+1, gHeight-1);
    uint32_t y2 = max((int32_t)y-1, 0);

    float4 sum = convert_float4(rsGetElementAt_uchar4(gIn, x1, y1)) * gCoeffs[0];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x, y1)) * gCoeffs[1];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x2, y1)) * gCoeffs[2];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x1, y)) * gCoeffs[3];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x, y)) * gCoeffs[4];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x2, y)) * gCoeffs[5];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x1, y2)) * gCoeffs[6];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x, y2)) * gCoeffs[7];
    sum += convert_float4(rsGetElementAt_uchar4(gIn, x2, y2)) * gCoeffs[8];

    sum = clamp(sum, 0.f, 255.f);
    return convert_uchar4(sum);
}


