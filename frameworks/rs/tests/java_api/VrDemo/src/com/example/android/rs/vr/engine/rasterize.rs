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

#define FLOAT_MAX  3.4028234E30f

float vert[3 * 8];
int index[3 * 12];
int width;
int height;

static float zoff[12];
static float2 slope[12];

static float2 p1[12];
static float2 p2[12];
static float2 p3[12];

      float2 d12[12];
        float2 d23[12];
        float2 d31[12];
static int total;

static void
triangleSetup(float3 f1, float3 f2, float3 f3) {
    if (((f1.x - f2.x) * (f3.y - f2.y) - (f1.y - f2.y) * (f3.x - f2.x)) < 0) {
        float3 tmp = f1;
        f1 = f2;
        f2 = tmp;
    }

    // using maxmima
    // string(solve([x1*dx+y1*dy+zoff=z1,x2*dx+y2*dy+zoff=z2,x3*dx+y3*dy+zoff=z3],[dx,dy,zoff]));
    double d = (f1.x * (f3.y - f2.y) - f2.x * f3.y + f3.x * f2.y + (f2.x - f3.x)
            * f1.y);

    if (d == 0) {
        return;
    }

    slope[total].x = (-(f1.y * (f3.z - f2.z) - f2.y * f3.z + f3.y * f2.z + (f2.y - f3.y) * f1.z)
            / d);
    slope[total].y = ((f1.x * (f3.z - f2.z) - f2.x * f3.z + f3.x * f2.z + (f2.x - f3.x) * f1.z)
            / d);
    zoff[total] = ((f1.x * (f3.y * f2.z - f2.y * f3.z) + f1.y * (f2.x * f3.z - f3.x * f2.z) +
            (f3.x * f2.y - f2.x * f3.y) * f1.z) / d);

    p1[total] = f1.xy;
    p2[total] = f2.xy;
    p3[total] = f3.xy;
    d12[total] = p1[total] - p2[total];
    d23[total] = p2[total] - p3[total];
    d31[total] = p3[total] - p1[total];
    total++;
}

void setup_triangles(int w, int h) {
    width = w;
    height = h;
    total = 0;
    //   rsDebug("RRRRR >>>>>> setup_triangles ", w, h);
    float3 f1;
    float3 f2;
    float3 f3;
    for (int i = 0; i < 3 * 12; i += 3) {
        int p1 = index[i];
        int p2 = index[i + 1];
        int p3 = index[i + 2];
        f1.x = vert[p1];
        f1.y = vert[p1 + 1];
        f1.z = vert[p1 + 2];
        f2.x = vert[p2];
        f2.y = vert[p2 + 1];
        f2.z = vert[p2 + 2];
        f3.x = vert[p3];
        f3.y = vert[p3 + 1];
        f3.z = vert[p3 + 2];
        triangleSetup(f1, f2, f3);
    }
}


float2 __attribute__ ((kernel)) render_z(uint32_t x, uint32_t y) {
    float2 out = (float2) {FLOAT_MAX,-FLOAT_MAX};
    float2 loc;
    loc.x = x;
    loc.y = y;
    for (int i = 0; i < total; i++) {
        float2 d1 = loc - p1[i];
        float2 d2 = loc - p2[i];
        float2 d3 = loc - p3[i];

        float test1 = (d12[i].x) * (d1.y) - (d12[i].y) * (d1.x);
        float test2 = (d23[i].x) * (d2.y) - (d23[i].y) * (d2.x);
        float test3 = (d31[i].x) * (d3.y) - (d31[i].y) * (d3.x);
        // float test = edge(0 , test1) * edge(0 , test2)* edge(0 , test3);

        if (test1 >= 0 &&
                test2 >= 0 &&
                test3 >= 0) {
            float2 delta = slope[i] * loc;
            float z = zoff[i] + delta.x + delta.y;

            out.x = min(z, out.x);
            out.y = max(z, out.y);
        }
    }
    return out;
}


rs_allocation z_range_buff;
float min_z = 1500;
float max_z = 2300;

void getMinMax() {

    float tmp_min = FLOAT_MAX;
    float tmp_max = -FLOAT_MAX;;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float2 v = rsGetElementAt_float2(z_range_buff, x, y);
            if (v.x == FLOAT_MAX) {
                continue;
            }
            tmp_min = min(tmp_min, v.x);
            tmp_max = max(tmp_max, v.x);
        }
    }
    min_z = tmp_min;
    max_z = tmp_max;
    rsDebug("RRRRR >>>>>> getMinMax ", min_z, max_z);
}

uchar4 __attribute__ ((kernel)) draw_z_buffer(float2 in) {

    if (in.x != FLOAT_MAX) {
        uchar v = (uchar)(255 * (in.x - min_z) / (max_z - min_z));
        uchar4 out;
        out.r = v;
        out.g = v;
        out.b = v;
        out.a = 255;
        return out;
    }

    uchar4 out;
    out.r = 0x44;
    out.g = 0x44;
    out.b = 0x99;
    out.a = 255;
    return out;
}