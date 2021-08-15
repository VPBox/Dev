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

#include "ip.rsh"
#pragma rs_fp_relaxed

int height;
int width;
static int radius;

rs_allocation InPixel;
rs_allocation ScratchPixel1;
rs_allocation ScratchPixel2;

const int MAX_RADIUS = 25;

// Store our coefficients here
static half gaussian[MAX_RADIUS * 2 + 1];

void setRadius(int rad) {
    radius = rad;
    // Compute gaussian weights for the blur
    // e is the euler's number
    half e = 2.718281828459045f;
    half pi = 3.1415926535897932f;
    // g(x) = ( 1 / sqrt( 2 * pi ) * sigma) * e ^ ( -x^2 / 2 * sigma^2 )
    // x is of the form [-radius .. 0 .. radius]
    // and sigma varies with radius.
    // Based on some experimental radius values and sigma's
    // we approximately fit sigma = f(radius) as
    // sigma = radius * 0.4  + 0.6
    // The larger the radius gets, the more our gaussian blur
    // will resemble a box blur since with large sigma
    // the gaussian curve begins to lose its shape
    half sigma = 0.4f * (half)radius + 0.6f;

    // Now compute the coefficints
    // We will store some redundant values to save some math during
    // the blur calculations
    // precompute some values
    half coeff1 = 1.0f / (sqrt( 2.0f * pi ) * sigma);
    half coeff2 = - 1.0f / (2.0f * sigma * sigma);

    half normalizeFactor = 0.0f;
    half halfR = 0.0f;
    for (int r = -radius; r <= radius; r ++) {
        halfR = (half)r;
        gaussian[r + radius] = coeff1 * pow(e, halfR * halfR * coeff2);
        normalizeFactor += gaussian[r + radius];
    }

    //Now we need to normalize the weights because all our coefficients need to add up to one
    normalizeFactor = 1.0f / normalizeFactor;
    for (int r = -radius; r <= radius; r ++) {
        halfR = (half)r;
        gaussian[r + radius] *= normalizeFactor;
    }
}

half4 RS_KERNEL copyIn(uchar4 in) {
    return convert_half4(in);
}

uchar4 RS_KERNEL vert(uint32_t x, uint32_t y) {
    half3 blurredPixel = 0;
    int gi = 0;
    uchar4 out;
    if ((y > radius) && (y < (height - radius))) {
        for (int r = -radius; r <= radius; r ++) {
            half4 i = rsGetElementAt_half4(ScratchPixel2, x, y + r);
            blurredPixel += i.xyz * gaussian[gi++];
        }
    } else {
        for (int r = -radius; r <= radius; r ++) {
            int validH = clamp((int)y + r, (int)0, (int)(height - 1));
            half4 i = rsGetElementAt_half4(ScratchPixel2, x, validH);
            blurredPixel += i.xyz * gaussian[gi++];
        }
    }

    out.xyz = convert_uchar3(clamp(blurredPixel, (half) 0.f, (half) 255.f));
    out.w = 0xff;
    return out;
}

half4 RS_KERNEL horz(uint32_t x, uint32_t y) {
    half4 blurredPixel = 0;
    int gi = 0;
    if ((x > radius) && (x < (width - radius))) {
        for (int r = -radius; r <= radius; r ++) {
            half4 i = rsGetElementAt_half4(ScratchPixel1, x + r, y);
            blurredPixel += i * gaussian[gi++];
        }
    } else {
        for (int r = -radius; r <= radius; r ++) {
            // Stepping left and right away from the pixel
            int validX = clamp((int)x + r, (int)0, (int)(width - 1));
            half4 i = rsGetElementAt_half4(ScratchPixel1, validX, y);
            blurredPixel += i * gaussian[gi++];
        }
    }

    return blurredPixel;
}
