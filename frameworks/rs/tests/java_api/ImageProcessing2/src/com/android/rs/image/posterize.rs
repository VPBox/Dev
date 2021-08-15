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

#include "ip.rsh"
#pragma rs_fp_relaxed

rs_allocation inputImage;

float intensityLow = 0.f;
float intensityHigh;
uchar4 color;
const static float3 mono = {0.299f, 0.587f, 0.114f};

void setParams(float intensHigh, float intensLow, uchar r, uchar g, uchar b) {
    intensityLow = intensLow;
    intensityHigh = intensHigh;
    uchar4 hats = {r, g, b, 255};
    color = hats;
}

uchar4 RS_KERNEL root(uchar4 v_in, uint32_t x, uint32_t y) {
    uchar4 refpix = rsGetElementAt_uchar4(inputImage, x, y);
    float pixelIntensity = dot(rsUnpackColor8888(refpix).rgb, mono);
    if ((pixelIntensity <= intensityHigh) && (pixelIntensity >= intensityLow)) {
        return color;
    } else {
        return v_in;
    }
}


