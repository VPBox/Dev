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

#pragma version(1)
#pragma rs java_package_name(com.example.android.rs.vr.engine)
#pragma rs_fp_relaxed

int size;
int z;
rs_allocation volume;

/* Unused function:
static float3 nylander(float3 p, int n) {
    float3 out;
    float r = length(p);
    float phi = atan2(p.y, p.x);
    float theta = acos(p.z / r);
    float rn = pow(r, n);
    out.x = sin(n * theta) * cos(n * phi);
    out.y = sin(n * theta) * sin(n * phi);
    out.z = cos(n * theta);
    return out * rn;
}
*/

/**
* 8 x faster than the above for n = 3
*/
static float3 nylander3(float3 p) {
    float3 out = (float3){0.f, 0.f, 0.f};
    float xy2 = p.x * p.x + p.y * p.y;
    if (xy2 == 0) return out;
    float z23x2y2 = (3 * p.z * p.z - p.x * p.x - p.y * p.y);
    out.x = (z23x2y2 * p.x * (p.x * p.x - 3 * p.y * p.y)) / xy2;
    out.y = (z23x2y2 * p.y * (3 * p.x * p.x - p.y * p.y)) / xy2;
    out.z = p.z * (p.z * p.z - 3 * p.x * p.x - 3 * p.y * p.y);
    return out;
}

short __attribute__((kernel)) mandelbulb(uint32_t x, uint32_t y) {
    int size2 = size / 2;
     if (z < size2) {
          return 256-4*(size2-z+4)*hypot((float)x-size2,(float)y-size2) / size2 ;
    }
    float3 c = (float3) {(float) x, (float) y, (float) z};
    c = ((c - size2) / (size2 * .9f));

    int loop = 25;
    float3 p = c;
    float len;
    for (int i = 0; i < loop; i++) {
        //    p = nylander(p, 3) + c;
        p = nylander3(p) + c;
        len = fast_length(p);
        if (len > 2.f) return 255 - loop*10;
        if (len < .3f) return loop*10;

    }
    len = length(p);
    return (short) (255 - (len * 255) / 4);
}

void __attribute__((kernel)) copy(short in, uint32_t x, uint32_t y) {
    rsSetElementAt_short(volume, in, x, y, z);
}
