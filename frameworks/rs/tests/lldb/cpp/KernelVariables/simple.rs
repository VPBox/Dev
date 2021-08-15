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

#pragma version(1)
#pragma rs java_package_name(com.android.rs.cppkernelvariables)

char char_global = 12;
uchar uchar_global = 234;
short short_global = -321;
ushort ushort_global = 432;
int int_global = 1234;
uint uint_global = 2345;
float float_global = 4.5f;
long long_global = -77777;
ulong ulong_global = 8888;
double double_global = -456.5f;

char2 char2_global = {11, -22};
uchar2 uchar2_global = {33, 44};
short2 short2_global = {-555, 666};
ushort2 ushort2_global = {777, 888};
int2 int2_global = {999, -1111};
uint2 uint2_global = {2222, 3333};
float2 float2_global = {4.5f, -5.0f};
long2 long2_global = {-4444, 5555};
ulong2 ulong2_global = {6666, 7777};
double2 double2_global = {88.5f, -99.0f};

char3 char3_global = {11, -22, -33};
uchar3 uchar3_global = {33, 44, 55};
short3 short3_global = {-555, 666, 777};
ushort3 ushort3_global = {777, 888, 999};
int3 int3_global = {999, -1111, 2222};
uint3 uint3_global = {2222, 3333, 4444};
float3 float3_global = {4.5f, -5.0f, -6.5f};
long3 long3_global = {-4444, 5555, 6666};
ulong3 ulong3_global = {6666, 7777, 8888};
double3 double3_global = {88.5f, -99.0f, 111.5f};

char4 char4_global = {55, 11, -22, -33};
uchar4 uchar4_global = {222, 33, 44, 55};
short4 short4_global = {-444, -555, 666, 777};
ushort4 ushort4_global = {666, 777, 888, 999};
int4 int4_global = {888, 999, -1111, 2222};
uint4 uint4_global = {1111, 2222, 3333, 4444};
float4 float4_global = {3.0f, 4.5f, -5.0f, -6.5f};
long4 long4_global = {-3333, -4444, 5555, 6666};
ulong4 ulong4_global = {5555, 6666, 7777, 8888};
double4 double4_global = {-77.0f, 88.5f, -99.0f, 111.5f};

rs_matrix2x2 matrix2x2_global;
rs_matrix3x3 matrix3x3_global;
rs_matrix4x4 matrix4x4_global;

rs_quaternion quaternion_global;

rs_allocation allocation_1D_global;
rs_allocation allocation_1D_global2;
rs_allocation allocation_2D_global;
rs_allocation allocation_2D_global2;
rs_allocation allocation_3D_global;
rs_allocation allocation_YUV_2D_global;

rs_allocation_cubemap_face cubemap_face_global;
rs_sampler sampler_global;

uchar4 __attribute__((kernel)) kernel(uchar4 in)
{
    char char_local = 'a';
    uchar uchar_local = 'b';
    short short_local = -321;
    ushort ushort_local = 432;
    int int_local = 1234;
    uint uint_local = 2345;
    float float_local = 4.5f;
    long long_local = -77777;
    ulong ulong_local = 8888;
    double double_local = -456.5f;

    char2 char2_local = {-11, -22};
    uchar2 uchar2_local = {33, 44};
    short2 short2_local = {-555, 666};
    ushort2 ushort2_local = {777, 888};
    int2 int2_local = {999, -1111};
    uint2 uint2_local = {2222, 3333};
    float2 float2_local = {4.5f, -5.0f};
    long2 long2_local = {-4444, 5555};
    ulong2 ulong2_local = {6666, 7777};
    double2 double2_local = {88.5f, -99.0f};

    char3 char3_local = {11, -22, -33};
    uchar3 uchar3_local = {33, 44, 55};
    short3 short3_local = {-555, 666, 777};
    ushort3 ushort3_local = {777, 888, 999};
    int3 int3_local = {999, -1111, 2222};
    uint3 uint3_local = {2222, 3333, 4444};
    float3 float3_local = {4.5f, -5.0f, -6.5f};
    long3 long3_local = {-4444, 5555, 6666};
    ulong3 ulong3_local = {6666, 7777, 8888};
    double3 double3_local = {88.5f, -99.0f, 111.5f};

    char4 char4_local = {55, 11, -22, -33};
    uchar4 uchar4_local = {22, 33, 44, 55};
    short4 short4_local = {-444, -555, 666, 777};
    ushort4 ushort4_local = {666, 777, 888, 999};
    int4 int4_local = {888, 999, -1111, 2222};
    uint4 uint4_local = {1111, 2222, 3333, 4444};
    float4 float4_local = {3.0f, 4.5f, -5.0f, -6.5f};
    long4 long4_local = {-3333, -4444, 5555, 6666};
    ulong4 ulong4_local = {5555, 6666, 7777, 8888};
    double4 double4_local = {-77.0f, 88.5f, -99.0f, 111.5f};

    rs_matrix2x2 matrix2x2_local = {{1., 2.5,
                                     3., 4.5}};
    rs_matrix3x3 matrix3x3_local = {{5., 6.5, 7.,
                                     8.5, 9., 1.5,
                                     2., 3.5, 4.}};
    rs_matrix4x4 matrix4x4_local = {{5.5, 6., 7.5, 8.,
                                     9., 1.5, 2., 3.5,
                                     4.5, 5.5, 6.5, 7.,
                                     8., 9.5, 1.5, 2.5}};

    matrix2x2_global = matrix2x2_local;
    matrix3x3_global = matrix3x3_local;
    matrix4x4_global = matrix4x4_local;

    rsQuaternionSet(&quaternion_global, 3.0, 4.5, 5.5, 6.0);

    rs_quaternion quaternion_local;
    rsQuaternionSet(&quaternion_local, 7.5, 8.0, 9.0, 0.5);

    char char_combined = char_local + (char)uchar_local + char2_local.x +
        (char)uchar2_local.x + char3_local.x - (char)uchar3_local.x +
        char4_local.x + (char)uchar4_local.x;

    short short_combined = short_local + (short)ushort_local + short2_local.x +
        (short)ushort2_local.x + short3_local.x + (short)ushort3_local.x +
        short4_local.x + (short)ushort4_local.x;

    int int_combined = int_local + (int)uint_local + int2_local.x +
        (int)uint2_local.x + int3_local.x + (int)uint3_local.x + int4_local.x +
        (int)uint4_local.x;

    float float_combined = float_local + float2_local.x + float3_local.x +
        float4_local.x;

    long long_combined = long_local + (long)ulong_local + long2_local.x +
        (long)ulong2_local.x + long3_local.x + (long)ulong3_local.x +
        long4_local.x + (long)ulong4_local.x;

    double double_combined = double_local + double2_local.x + double3_local.x +
        double4_local.x;

    char_global = char_combined;
    short_global = short_combined;
    int_global = int_combined;
    float_global = float_combined;
    long_global = long_combined;
    double_global = double_combined;

    uchar4 result = {1,2,3,4};
    return result;
}

float use_constants_global;

void setup(void)
{
  use_constants_global =
      M_1_PI +
      M_2_PI +
      M_2_PIl +
      M_2_SQRTPI +
      M_E +
      M_LN10 +
      M_LN2 +
      M_LOG10E +
      M_LOG2E +
      M_PI +
      M_PI_2 +
      M_PI_4 +
      M_SQRT1_2 +
      M_SQRT2;
}
