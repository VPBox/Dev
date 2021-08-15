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

int32_t gWidthIn;
int32_t gHeightIn;
rs_allocation gIn;
float scale;


uchar4 __attribute__((kernel)) nearest(uint32_t x, uint32_t y) {
    float xf = clamp(x * scale, 0.f, (float)gWidthIn - 1.f);
    float yf = clamp(y * scale, 0.f, (float)gHeightIn - 1.f);
    uint32_t ix = xf;
    uint32_t iy = yf;

    uchar4 tmp = rsGetElementAt_uchar4(gIn, ix, iy);
    tmp.a = 0xff;
    return tmp;
}


static float4 cubicInterpolate (float4 p0,float4 p1,float4 p2,float4 p3 , float x) {
    return p1 + 0.5f * x * (p2 - p0 + x * (2.f * p0 - 5.f * p1 + 4.f * p2 - p3
            + x * (3.f * (p1 - p2) + p3 - p0)));
}

uchar4 __attribute__((kernel)) bicubic(uint32_t x, uint32_t y) {
    float xf = (x + 0.5f) * scale - 0.5f;
    float yf = (y + 0.5f) * scale - 0.5f;

    int startx = (int) floor(xf - 1);
    int starty = (int) floor(yf - 1);
    xf = xf - floor(xf);
    yf = yf - floor(yf);
    int maxx = gWidthIn - 1;
    int maxy = gHeightIn - 1;

    uint32_t xs0 = (uint32_t) clamp(startx + 0, 0, maxx);
    uint32_t xs1 = (uint32_t) clamp(startx + 1, 0, maxx);
    uint32_t xs2 = (uint32_t) clamp(startx + 2, 0, maxx);
    uint32_t xs3 = (uint32_t) clamp(startx + 3, 0, maxx);

    uint32_t ys0 = (uint32_t) clamp(starty + 0, 0 , maxy);
    uint32_t ys1 = (uint32_t) clamp(starty + 1, 0 , maxy);
    uint32_t ys2 = (uint32_t) clamp(starty + 2, 0 , maxy);
    uint32_t ys3 = (uint32_t) clamp(starty + 3, 0 , maxy);

    float4 p00 = convert_float4(rsGetElementAt_uchar4(gIn, xs0, ys0));
    float4 p01 = convert_float4(rsGetElementAt_uchar4(gIn, xs1, ys0));
    float4 p02 = convert_float4(rsGetElementAt_uchar4(gIn, xs2, ys0));
    float4 p03 = convert_float4(rsGetElementAt_uchar4(gIn, xs3, ys0));
    float4 p0  = cubicInterpolate(p00, p01, p02, p03, xf);

    float4 p10 = convert_float4(rsGetElementAt_uchar4(gIn, xs0, ys1));
    float4 p11 = convert_float4(rsGetElementAt_uchar4(gIn, xs1, ys1));
    float4 p12 = convert_float4(rsGetElementAt_uchar4(gIn, xs2, ys1));
    float4 p13 = convert_float4(rsGetElementAt_uchar4(gIn, xs3, ys1));
    float4 p1  = cubicInterpolate(p10, p11, p12, p13, xf);

    float4 p20 = convert_float4(rsGetElementAt_uchar4(gIn, xs0, ys2));
    float4 p21 = convert_float4(rsGetElementAt_uchar4(gIn, xs1, ys2));
    float4 p22 = convert_float4(rsGetElementAt_uchar4(gIn, xs2, ys2));
    float4 p23 = convert_float4(rsGetElementAt_uchar4(gIn, xs3, ys2));
    float4 p2  = cubicInterpolate(p20, p21, p22, p23, xf);

    float4 p30 = convert_float4(rsGetElementAt_uchar4(gIn, xs0, ys3));
    float4 p31 = convert_float4(rsGetElementAt_uchar4(gIn, xs1, ys3));
    float4 p32 = convert_float4(rsGetElementAt_uchar4(gIn, xs2, ys3));
    float4 p33 = convert_float4(rsGetElementAt_uchar4(gIn, xs3, ys3));
    float4 p3  = cubicInterpolate(p30, p31, p32, p33, xf);

    float4 p  = cubicInterpolate(p0, p1, p2, p3, yf);
    p = clamp(p + 0.5f, 0.f, 255.f);
    return convert_uchar4(p);
}

