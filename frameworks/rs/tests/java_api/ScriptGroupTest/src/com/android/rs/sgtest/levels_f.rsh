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

float inBlack;
float outBlack;
float inWMinInB;
float outWMinOutB;
float overInWMinInB;
rs_matrix3x3 colorMat;
/*
uchar4 __attribute__((kernel)) root(uchar4 in) {
    uchar4 out;
    float3 pixel = convert_float4(in).rgb;
    pixel = rsMatrixMultiply(&colorMat, pixel);
    pixel = clamp(pixel, 0.f, 255.f);
    pixel = (pixel - inBlack) * overInWMinInB;
    pixel = pixel * outWMinOutB + outBlack;
    pixel = clamp(pixel, 0.f, 255.f);
    out.xyz = convert_uchar3(pixel);
    out.w = 0xff;
    return out;
}
*/

void initialize(float inBlack_, float outBlack_, float inWMinInB_, float outWMinOutB_,
                float overInWMinInB_, rs_matrix3x3 colorMat_) {
    inBlack = inBlack_;
    outBlack = outBlack_;
    inWMinInB = inWMinInB_;
    outWMinOutB = outWMinOutB_;
    overInWMinInB = overInWMinInB_;
    colorMat = colorMat_;
}

float4 __attribute__((kernel)) levels_v4(float4 in) {
    float4 pixel;
    pixel.rgb = rsMatrixMultiply(&colorMat, in.rgb);
    pixel = clamp(pixel, 0.f, 255.f);
    pixel = (pixel - inBlack) * overInWMinInB;
    pixel = pixel * outWMinOutB + outBlack;
    return pixel;
}

