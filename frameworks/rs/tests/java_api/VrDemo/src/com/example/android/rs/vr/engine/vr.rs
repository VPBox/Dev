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
#define JITTER_LENGTH 3457
float jitter[JITTER_LENGTH];

float3 s;
float3 dx;
float3 dy;
float3 dz;
float zoomFactor;
rs_matrix4x4 matrix4;
rs_matrix3x3 matrix3;
uchar4 base_color;
static float3 mLight;
static float rscale;

// material color
rs_allocation opacity;
rs_allocation color_map;

static void fillJitter() {
    for (int i = 0; i < JITTER_LENGTH; i++) {
        jitter[i] = (rsRand(rscale)+rsRand(rscale)+rsRand(rscale))/3;
    }
}

void setup_vectors() {
    s = rsMatrixMultiply(&matrix4, (float3) {0.5f, 0.5f, 0.5f}).xyz;
    dx = rsMatrixMultiply(&matrix3, (float3) {1.f, 0.f, 0.f});
    dy = rsMatrixMultiply(&matrix3, (float3) {0.f, 1.f, 0.f});
    dz = rsMatrixMultiply(&matrix3, (float3) {0.f, 0.f, 1.f});
    zoomFactor = dz.x * dz.x + dz.y * dz.y + dz.z * dz.z;
    zoomFactor /= dx.x * dx.x + dx.y * dx.y + dx.z * dx.z;
    base_color.r = 0;
    base_color.g = 0;
    base_color.b = 0;
    base_color.a = 255;

    float3 mLightRelitvePos = (float3) {0.f, 0.7071f, -0.7071f}; // light relitve to camera
    mLight = mLightRelitvePos.x + dx + mLightRelitvePos.y * dy + mLightRelitvePos.z * dz;
    mLight = normalize(mLight);
    rscale = 1.5f/length(dz);
    fillJitter();
}

static float triLinear(float v_0_0_0, float v_0_0_1, float v_0_1_0, float v_0_1_1,
        float v_1_0_0, float v_1_0_1, float v_1_1_0, float v_1_1_1,
        float3 delta) {
    float v_0_0 = mix(v_0_0_0, v_0_0_1, delta.x);
    float v_0_1 = mix(v_0_1_0, v_0_1_1, delta.x);
    float v_1_0 = mix(v_1_0_0, v_1_0_1, delta.x);
    float v_1_1 = mix(v_1_1_0, v_1_1_1, delta.x);
    float v_0 = mix(v_0_0, v_0_1, delta.y);
    float v_1 = mix(v_1_0, v_1_1, delta.y);
    float v = mix(v_0, v_1, delta.z);
    return v;
}

rs_allocation bricks;
rs_allocation brick_index;
int brick_dimx;
int brick_dimy;
int brick_dimz;

static int isInBrick(int3 p) {
    int bx = p.x >> 5;
    int by = p.y >> 5;
    int bz = p.z >> 5;
    int brickno = bx + (by + brick_dimy * bz) * brick_dimx;

    brickno *= 32 * 32;
    int off = brickno + (p.z & 0x1F) * 32 + (p.y & 0x1F);
    uint slice = rsGetElementAt_uint(bricks, off);
    return slice & (1 << (p.x & 0x1F));
}

rs_allocation volume;
rs_allocation zbuff;
bool debug = true;

uchar4 __attribute__ ((kernel)) draw_z_buffer(float2 in, uint32_t x, uint32_t y) {
    uchar4 out = base_color;

    float zsuface = in.x + .5f;
    float zstart = zsuface;
    float zend = in.y - 2.f;//0.5f;
    float zlen = zend - zstart;

    if (zstart == FLOAT_MAX || zlen < 0) {
        return out;
    }

    float3 p = s + x * dx + y * dy + dz * zstart;
    float zb = zend;
    int izlen = (int) zlen;
    float light = 1;
    float4 total_color = (float4) {0.f, 0.f, 0.f, 0.f};

    if (false) { // show the walls only used for debuging
        int3 ip = convert_int3(p);
        ip = clamp(ip, 4, 500);
        short pix = rsGetElementAt_short(volume, ip.x, ip.y, ip.z);

        int intensity = (((short) pix) & 0xFFFF);
        //   intensity = clamp(intensity,0,400);
        uchar4 color = rsGetElementAt_uchar4(color_map, intensity * 2);
        // int op = rsGetElementAt_uchar(opacity, intensity);

        out.r = color.r;
        out.g = color.g;
        out.b = color.b;
        out.a = 255;
        return out;
    }
    {
        int3 ip = convert_int3(p);

        if (isInBrick(ip)) { // isInBrick(ip)) {

            float3 delta = p - convert_float3(ip);
            // TODO switch to rsAllocationVLoadX_short2
            // short2 tmps = rsAllocationVLoadX_short2(volume, ip.x + 0, ip.y + 0, ip.z + 0);
            //  float2 tmp =   convert_float2(tmps);
            float v_0_0_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 0, ip.z + 0);
            float v_0_0_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 0, ip.z + 0);

            float v_0_1_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 0);
            float v_0_1_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 0);
            float v_1_0_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 0, ip.z + 1);
            float v_1_0_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 0, ip.z + 1);
            float v_1_1_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 1);
            float v_1_1_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 1);
            float v = triLinear(v_0_0_0, v_0_0_1, v_0_1_0, v_0_1_1,
                    v_1_0_0, v_1_0_1, v_1_1_0, v_1_1_1,
                    delta);
            int intensity = (((short) v) & 0xFFFF);
            uchar op = rsGetElementAt_uchar(opacity, intensity);

            if (op != 0) { // near the surface "broken"

                float sdx = rsGetElementAt_float2(zbuff, max(0, (int) x - 1), y).x - in.x;
                float sdy = rsGetElementAt_float2(zbuff, x, max(0, (int) y - 1)).x - in.x;
                float dot_prod = sqrt(1 / (1 + (sdy * sdy + sdx * sdx) * zoomFactor));
                float opf = op  * (1/255.f);
                uchar4 color = rsGetElementAt_uchar4(color_map, intensity * 2);
                uchar4 mat = rsGetElementAt_uchar4(color_map, intensity * 2 + 1);
                float4 fcolor = convert_float4(color);;

                float ambient = mat.x * (1/255.f);
                // float specular = mat.y * (1/255.f);
                float diffuse = mat.z * (1/255.f);
                float lop = (ambient + diffuse * dot_prod) * light * opf;
                light -= opf;
                total_color += fcolor * lop;
                zb = zstart;

            }
        }
    }
    p += dz * jitter[(x+(y<<11))%JITTER_LENGTH];

    if (light > 0) {
        for (int k = 0; k < izlen - 1; k++) {

            int3 ip = convert_int3(p);
            if (isInBrick(ip)) {
                float3 delta = p - convert_float3(ip);

                float v_0_0_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 0, ip.z + 0);
                float v_0_0_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 0, ip.z + 0);
                float v_0_1_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 0);
                float v_0_1_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 0);
                float v_1_0_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 0, ip.z + 1);
                float v_1_0_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 0, ip.z + 1);
                float v_1_1_0 = (float) rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 1);
                float v_1_1_1 = (float) rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 1);
                float v = triLinear(v_0_0_0, v_0_0_1, v_0_1_0, v_0_1_1,
                        v_1_0_0, v_1_0_1, v_1_1_0, v_1_1_1,
                        delta);
                int intensity = (((short) v) & 0xFFFF);
                uchar op = rsGetElementAt_uchar(opacity, intensity);

                if (op != 0) {

                    float3 v;
                    float3 vn;

                    float v_0_0_2 = rsGetElementAt_short(volume, ip.x + 2, ip.y + 0, ip.z + 0);
                    float v_0_1_2 = rsGetElementAt_short(volume, ip.x + 2, ip.y + 1, ip.z + 0);
                    float v_1_0_2 = rsGetElementAt_short(volume, ip.x + 2, ip.y + 0, ip.z + 1);
                    float v_1_1_2 = rsGetElementAt_short(volume, ip.x + 2, ip.y + 1, ip.z + 1);
                    v.x = triLinear(v_0_0_1, v_0_0_2, v_0_1_1, v_0_1_2,
                            v_1_0_1, v_1_0_2, v_1_1_1, v_1_1_2,
                            delta);

                    float v_0_0_n = rsGetElementAt_short(volume, ip.x - 1, ip.y + 0, ip.z + 0);
                    float v_0_1_n = rsGetElementAt_short(volume, ip.x - 1, ip.y + 1, ip.z + 0);
                    float v_1_0_n = rsGetElementAt_short(volume, ip.x - 1, ip.y + 0, ip.z + 1);
                    float v_1_1_n = rsGetElementAt_short(volume, ip.x - 1, ip.y + 1, ip.z + 1);
                    vn.x = triLinear(v_0_0_n, v_0_0_0, v_0_1_n, v_0_1_0,
                            v_1_0_n, v_1_0_0, v_1_1_n, v_1_1_0,
                            delta);

                    float v_0_2_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 2, ip.z + 0);
                    float v_0_2_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 2, ip.z + 0);
                    float v_1_2_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 2, ip.z + 1);
                    float v_1_2_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 2, ip.z + 1);
                    v.y = triLinear(v_0_1_0, v_0_1_1, v_0_2_0, v_0_2_1,
                            v_1_1_0, v_1_1_1, v_1_2_0, v_1_2_1,
                            delta);

                    float v_0_n_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 0);
                    float v_0_n_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 0);
                    float v_1_n_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 1);
                    float v_1_n_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 1);
                    vn.y = triLinear(v_0_n_0, v_0_n_1, v_0_0_0, v_0_0_1,
                            v_1_n_0, v_1_n_1, v_1_0_0, v_1_0_1,
                            delta);

                    float v_n_0_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 0, ip.z - 1);
                    float v_n_0_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 0, ip.z - 1);
                    float v_n_1_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z - 1);
                    float v_n_1_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z - 1);
                    vn.z = triLinear(v_n_0_0, v_n_0_1, v_n_1_0, v_n_1_1,
                            v_0_0_0, v_0_0_1, v_0_1_0, v_0_1_1,
                            delta);

                    float v_2_0_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 0, ip.z + 2);
                    float v_2_0_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 0, ip.z + 2);
                    float v_2_1_0 = rsGetElementAt_short(volume, ip.x + 0, ip.y + 1, ip.z + 2);
                    float v_2_1_1 = rsGetElementAt_short(volume, ip.x + 1, ip.y + 1, ip.z + 2);
                    v.z = triLinear(v_1_0_0, v_1_0_1, v_1_1_0, v_1_1_1,
                            v_2_0_0, v_2_0_1, v_2_1_0, v_2_1_1,
                            delta);

                    float3 dv = normalize(v - vn);
                    float dot_prod = dot(dv, dz);
                    float opf = op / 255.f;

                    uchar4 color = rsGetElementAt_uchar4(color_map, intensity * 2);
                    uchar4 mat = rsGetElementAt_uchar4(color_map, intensity * 2 + 1);
                    float4 fcolor = convert_float4(color);;

                    // float3 mLight = (float3) {0,-1,0};
                    float3 normal = dv;
                    float3 r = 2 * normal * dot(mLight, normal) - mLight;
                    float spec = dot(r, dz);

                    // Eye point in this space is in the direction (0,0,-1)
                    // Spec * Math.pow(R_z , P) lets use power == 2 (cheap)

                    float ambient = mat.x * (1/255.f); // ambient
                    float specular = mat.y * (1/255.f); // specular not used right now
                    float diffuse = mat.z  * (1/255.f);// diffuse
                    float lop = (ambient + diffuse * dot_prod + specular * pow(spec, 10)) * light * opf;
                    light -= opf;
                    total_color += fcolor * lop;

                    zb = zstart + k;
                    if (light <= 0) {
                        break;
                    }
                }
            }

            p += dz;
        }
    }

    out = convert_uchar4(clamp(total_color, 0.f, 255.f));
    out.a = 0xFF;

    return out;
}
