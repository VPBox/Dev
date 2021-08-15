# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

'''A list of signatures for RS builtin functions and util functions for them.

from __future__ import absolute_import

Function signature syntax is usually C-like, however, fixed values can also be
specified for those functions where the input range is restricted.
Lines in the function table beginning with - are comments.
Also contains utility functions to build an LLDB expression from a single
function line.
'''

import re

# Remove blank and comment lines using a lambda.
FUNC_LIST = filter(lambda line: line.strip()
                                and not line.strip().startswith('-'), '''
- math functions

uchar abs(char v);
uchar2 abs(char2 v);
uchar3 abs(char3 v);
uchar4 abs(char4 v);
uint abs(int v);
uint2 abs(int2 v);
uint3 abs(int3 v);
uint4 abs(int4 v);
ushort abs(short v);
ushort2 abs(short2 v);
ushort3 abs(short3 v);
ushort4 abs(short4 v);

float acos(float v);
float2 acos(float2 v);
float3 acos(float3 v);
float4 acos(float4 v);

float acosh(float v);
float2 acosh(float2 v);
float3 acosh(float3 v);
float4 acosh(float4 v);

float acospi(float v);
float2 acospi(float2 v);
float3 acospi(float3 v);
float4 acospi(float4 v);

float asin(float v);
float2 asin(float2 v);
float3 asin(float3 v);
float4 asin(float4 v);

float asinh(float v);
float2 asinh(float2 v);
float3 asinh(float3 v);
float4 asinh(float4 v);

float asinpi(float v);
float2 asinpi(float2 v);
float3 asinpi(float3 v);
float4 asinpi(float4 v);

float atan(float v);
float2 atan(float2 v);
float3 atan(float3 v);
float4 atan(float4 v);

float atan2(float numerator, float denominator);
float2 atan2(float2 numerator, float2 denominator);
float3 atan2(float3 numerator, float3 denominator);
float4 atan2(float4 numerator, float4 denominator);

float atan2pi(float numerator, float denominator);
float2 atan2pi(float2 numerator, float2 denominator);
float3 atan2pi(float3 numerator, float3 denominator);
float4 atan2pi(float4 numerator, float4 denominator);

float atanh(float v);
float2 atanh(float2 v);
float3 atanh(float3 v);
float4 atanh(float4 v);

float atanpi(float v);
float2 atanpi(float2 v);
float3 atanpi(float3 v);
float4 atanpi(float4 v);

float cbrt(float v);
float2 cbrt(float2 v);
float3 cbrt(float3 v);
float4 cbrt(float4 v);

float ceil(float v);
float2 ceil(float2 v);
float3 ceil(float3 v);
float4 ceil(float4 v);

char clamp(char value, char min_value, char max_value);
char2 clamp(char2 value, char min_value, char max_value);
char2 clamp(char2 value, char2 min_value, char2 max_value);
char3 clamp(char3 value, char min_value, char max_value);
char3 clamp(char3 value, char3 min_value, char3 max_value);
char4 clamp(char4 value, char min_value, char max_value);
char4 clamp(char4 value, char4 min_value, char4 max_value);
float clamp(float value, float min_value, float max_value);
float2 clamp(float2 value, float min_value, float max_value);
float2 clamp(float2 value, float2 min_value, float2 max_value);
float3 clamp(float3 value, float min_value, float max_value);
float3 clamp(float3 value, float3 min_value, float3 max_value);
float4 clamp(float4 value, float min_value, float max_value);
float4 clamp(float4 value, float4 min_value, float4 max_value);
int clamp(int value, int min_value, int max_value);
int2 clamp(int2 value, int min_value, int max_value);
int2 clamp(int2 value, int2 min_value, int2 max_value);
int3 clamp(int3 value, int min_value, int max_value);
int3 clamp(int3 value, int3 min_value, int3 max_value);
int4 clamp(int4 value, int min_value, int max_value);
int4 clamp(int4 value, int4 min_value, int4 max_value);
long clamp(long value, long min_value, long max_value);
long2 clamp(long2 value, long min_value, long max_value);
long2 clamp(long2 value, long2 min_value, long2 max_value);
long3 clamp(long3 value, long min_value, long max_value);
long3 clamp(long3 value, long3 min_value, long3 max_value);
long4 clamp(long4 value, long min_value, long max_value);
long4 clamp(long4 value, long4 min_value, long4 max_value);
short clamp(short value, short min_value, short max_value);
short2 clamp(short2 value, short min_value, short max_value);
short2 clamp(short2 value, short2 min_value, short2 max_value);
short3 clamp(short3 value, short min_value, short max_value);
short3 clamp(short3 value, short3 min_value, short3 max_value);
short4 clamp(short4 value, short min_value, short max_value);
short4 clamp(short4 value, short4 min_value, short4 max_value);
uchar clamp(uchar value, uchar min_value, uchar max_value);
uchar2 clamp(uchar2 value, uchar min_value, uchar max_value);
uchar2 clamp(uchar2 value, uchar2 min_value, uchar2 max_value);
uchar3 clamp(uchar3 value, uchar min_value, uchar max_value);
uchar3 clamp(uchar3 value, uchar3 min_value, uchar3 max_value);
uchar4 clamp(uchar4 value, uchar min_value, uchar max_value);
uchar4 clamp(uchar4 value, uchar4 min_value, uchar4 max_value);
uint clamp(uint value, uint min_value, uint max_value);
uint2 clamp(uint2 value, uint min_value, uint max_value);
uint2 clamp(uint2 value, uint2 min_value, uint2 max_value);
uint3 clamp(uint3 value, uint min_value, uint max_value);
uint3 clamp(uint3 value, uint3 min_value, uint3 max_value);
uint4 clamp(uint4 value, uint min_value, uint max_value);
uint4 clamp(uint4 value, uint4 min_value, uint4 max_value);
ulong clamp(ulong value, ulong min_value, ulong max_value);
ulong2 clamp(ulong2 value, ulong min_value, ulong max_value);
ulong2 clamp(ulong2 value, ulong2 min_value, ulong2 max_value);
ulong3 clamp(ulong3 value, ulong min_value, ulong max_value);
ulong3 clamp(ulong3 value, ulong3 min_value, ulong3 max_value);
ulong4 clamp(ulong4 value, ulong min_value, ulong max_value);
ulong4 clamp(ulong4 value, ulong4 min_value, ulong4 max_value);
ushort clamp(ushort value, ushort min_value, ushort max_value);
ushort2 clamp(ushort2 value, ushort min_value, ushort max_value);
ushort2 clamp(ushort2 value, ushort2 min_value, ushort2 max_value);
ushort3 clamp(ushort3 value, ushort min_value, ushort max_value);
ushort3 clamp(ushort3 value, ushort3 min_value, ushort3 max_value);
ushort4 clamp(ushort4 value, ushort min_value, ushort max_value);
ushort4 clamp(ushort4 value, ushort4 min_value, ushort4 max_value);

char clz(char value);
char2 clz(char2 value);
char3 clz(char3 value);
char4 clz(char4 value);
int clz(int value);
int2 clz(int2 value);
int3 clz(int3 value);
int4 clz(int4 value);
short clz(short value);
short2 clz(short2 value);
short3 clz(short3 value);
short4 clz(short4 value);
uchar clz(uchar value);
uchar2 clz(uchar2 value);
uchar3 clz(uchar3 value);
uchar4 clz(uchar4 value);
uint clz(uint value);
uint2 clz(uint2 value);
uint3 clz(uint3 value);
uint4 clz(uint4 value);
ushort clz(ushort value);
ushort2 clz(ushort2 value);
ushort3 clz(ushort3 value);
ushort4 clz(ushort4 value);

float copysign(float magnitude_value, float sign_value);
float2 copysign(float2 magnitude_value, float2 sign_value);
float3 copysign(float3 magnitude_value, float3 sign_value);
float4 copysign(float4 magnitude_value, float4 sign_value);

float cos(float v);
float2 cos(float2 v);
float3 cos(float3 v);
float4 cos(float4 v);

float cosh(float v);
float2 cosh(float2 v);
float3 cosh(float3 v);
float4 cosh(float4 v);

float cospi(float v);
float2 cospi(float2 v);
float3 cospi(float3 v);
float4 cospi(float4 v);

float erf(float v);
float2 erf(float2 v);
float3 erf(float3 v);
float4 erf(float4 v);

float erfc(float v);
float2 erfc(float2 v);
float3 erfc(float3 v);
float4 erfc(float4 v);

float exp(float v);
float2 exp(float2 v);
float3 exp(float3 v);
float4 exp(float4 v);

float exp10(float v);
float2 exp10(float2 v);
float3 exp10(float3 v);
float4 exp10(float4 v);

float exp2(float v);
float2 exp2(float2 v);
float3 exp2(float3 v);
float4 exp2(float4 v);

float expm1(float v);
float2 expm1(float2 v);
float3 expm1(float3 v);
float4 expm1(float4 v);

float fabs(float v);
float2 fabs(float2 v);
float3 fabs(float3 v);
float4 fabs(float4 v);

float fdim(float a, float b);
float2 fdim(float2 a, float2 b);
float3 fdim(float3 a, float3 b);
float4 fdim(float4 a, float4 b);

float floor(float v);
float2 floor(float2 v);
float3 floor(float3 v);
float4 floor(float4 v);

float fma(float multiplicand1, float multiplicand2, float offset);
float2 fma(float2 multiplicand1, float2 multiplicand2, float2 offset);
float3 fma(float3 multiplicand1, float3 multiplicand2, float3 offset);
float4 fma(float4 multiplicand1, float4 multiplicand2, float4 offset);

float fmax(float a, float b);
float2 fmax(float2 a, float b);
float2 fmax(float2 a, float2 b);
float3 fmax(float3 a, float b);
float3 fmax(float3 a, float3 b);
float4 fmax(float4 a, float b);
float4 fmax(float4 a, float4 b);

float fmin(float a, float b);
float2 fmin(float2 a, float b);
float2 fmin(float2 a, float2 b);
float3 fmin(float3 a, float b);
float3 fmin(float3 a, float3 b);
float4 fmin(float4 a, float b);
float4 fmin(float4 a, float4 b);

float fmod(float numerator, float denominator);
float2 fmod(float2 numerator, float2 denominator);
float3 fmod(float3 numerator, float3 denominator);
float4 fmod(float4 numerator, float4 denominator);

float fract(float v, float* floor);
float2 fract(float2 v, float2* floor);
float3 fract(float3 v, float3* floor);
float4 fract(float4 v, float4* floor);

float frexp(float v, int* exponent);
float2 frexp(float2 v, int2* exponent);
float3 frexp(float3 v, int3* exponent);
float4 frexp(float4 v, int4* exponent);

float half_recip(float v);
float2 half_recip(float2 v);
float3 half_recip(float3 v);
float4 half_recip(float4 v);

float half_rsqrt(float v);
float2 half_rsqrt(float2 v);
float3 half_rsqrt(float3 v);
float4 half_rsqrt(float4 v);

float half_sqrt(float v);
float2 half_sqrt(float2 v);
float3 half_sqrt(float3 v);
float4 half_sqrt(float4 v);

float hypot(float a, float b);
float2 hypot(float2 a, float2 b);
float3 hypot(float3 a, float3 b);
float4 hypot(float4 a, float4 b);

int ilogb(float v);
int2 ilogb(float2 v);
int3 ilogb(float3 v);
int4 ilogb(float4 v);

float ldexp(float mantissa, int exponent);
float2 ldexp(float2 mantissa, int exponent);
float2 ldexp(float2 mantissa, int2 exponent);
float3 ldexp(float3 mantissa, int exponent);
float3 ldexp(float3 mantissa, int3 exponent);
float4 ldexp(float4 mantissa, int exponent);
float4 ldexp(float4 mantissa, int4 exponent);

float lgamma(float v);
float lgamma(float v, int* sign_of_gamma);
float2 lgamma(float2 v);
float2 lgamma(float2 v, int2* sign_of_gamma);
float3 lgamma(float3 v);
float3 lgamma(float3 v, int3* sign_of_gamma);
float4 lgamma(float4 v);
float4 lgamma(float4 v, int4* sign_of_gamma);

float log(float v);
float2 log(float2 v);
float3 log(float3 v);
float4 log(float4 v);

float log10(float v);
float2 log10(float2 v);
float3 log10(float3 v);
float4 log10(float4 v);

float log1p(float v);
float2 log1p(float2 v);
float3 log1p(float3 v);
float4 log1p(float4 v);

float log2(float v);
float2 log2(float2 v);
float3 log2(float3 v);
float4 log2(float4 v);

float logb(float v);
float2 logb(float2 v);
float3 logb(float3 v);
float4 logb(float4 v);

float mad(float multiplicand1, float multiplicand2, float offset);
float2 mad(float2 multiplicand1, float2 multiplicand2, float2 offset);
float3 mad(float3 multiplicand1, float3 multiplicand2, float3 offset);
float4 mad(float4 multiplicand1, float4 multiplicand2, float4 offset);

char max(char a, char b);
char2 max(char2 a, char2 b);
char3 max(char3 a, char3 b);
char4 max(char4 a, char4 b);
float max(float a, float b);
float2 max(float2 a, float2 b);
float3 max(float3 a, float3 b);
float4 max(float4 a, float4 b);
int max(int a, int b);
int2 max(int2 a, int2 b);
int3 max(int3 a, int3 b);
int4 max(int4 a, int4 b);
long max(long a, long b);
long2 max(long2 a, long2 b);
long3 max(long3 a, long3 b);
long4 max(long4 a, long4 b);
short max(short a, short b);
short2 max(short2 a, short2 b);
short3 max(short3 a, short3 b);
short4 max(short4 a, short4 b);
uchar max(uchar a, uchar b);
uchar2 max(uchar2 a, uchar2 b);
uchar3 max(uchar3 a, uchar3 b);
uchar4 max(uchar4 a, uchar4 b);
uint max(uint a, uint b);
uint2 max(uint2 a, uint2 b);
uint3 max(uint3 a, uint3 b);
uint4 max(uint4 a, uint4 b);
ulong max(ulong a, ulong b);
ulong2 max(ulong2 a, ulong2 b);
ulong3 max(ulong3 a, ulong3 b);
ulong4 max(ulong4 a, ulong4 b);
ushort max(ushort a, ushort b);
ushort2 max(ushort2 a, ushort2 b);
ushort3 max(ushort3 a, ushort3 b);
ushort4 max(ushort4 a, ushort4 b);

char min(char a, char b);
char2 min(char2 a, char2 b);
char3 min(char3 a, char3 b);
char4 min(char4 a, char4 b);
float min(float a, float b);
float2 min(float2 a, float2 b);
float3 min(float3 a, float3 b);
float4 min(float4 a, float4 b);
int min(int a, int b);
int2 min(int2 a, int2 b);
int3 min(int3 a, int3 b);
int4 min(int4 a, int4 b);
long min(long a, long b);
long2 min(long2 a, long2 b);
long3 min(long3 a, long3 b);
long4 min(long4 a, long4 b);
short min(short a, short b);
short2 min(short2 a, short2 b);
short3 min(short3 a, short3 b);
short4 min(short4 a, short4 b);
uchar min(uchar a, uchar b);
uchar2 min(uchar2 a, uchar2 b);
uchar3 min(uchar3 a, uchar3 b);
uchar4 min(uchar4 a, uchar4 b);
uint min(uint a, uint b);
uint2 min(uint2 a, uint2 b);
uint3 min(uint3 a, uint3 b);
uint4 min(uint4 a, uint4 b);
ulong min(ulong a, ulong b);
ulong2 min(ulong2 a, ulong2 b);
ulong3 min(ulong3 a, ulong3 b);
ulong4 min(ulong4 a, ulong4 b);
ushort min(ushort a, ushort b);
ushort2 min(ushort2 a, ushort2 b);
ushort3 min(ushort3 a, ushort3 b);
ushort4 min(ushort4 a, ushort4 b);

float mix(float start, float stop, float fraction);
float2 mix(float2 start, float2 stop, float fraction);
float2 mix(float2 start, float2 stop, float2 fraction);
float3 mix(float3 start, float3 stop, float fraction);
float3 mix(float3 start, float3 stop, float3 fraction);
float4 mix(float4 start, float4 stop, float fraction);
float4 mix(float4 start, float4 stop, float4 fraction);

float modf(float v, float* integral_part);
float2 modf(float2 v, float2* integral_part);
float3 modf(float3 v, float3* integral_part);
float4 modf(float4 v, float4* integral_part);

float nan(uint v);

float native_acos(float v);
float2 native_acos(float2 v);
float3 native_acos(float3 v);
float4 native_acos(float4 v);

float native_acosh(float v);
float2 native_acosh(float2 v);
float3 native_acosh(float3 v);
float4 native_acosh(float4 v);

float native_acospi(float v);
float2 native_acospi(float2 v);
float3 native_acospi(float3 v);
float4 native_acospi(float4 v);

float native_asin(float v);
float2 native_asin(float2 v);
float3 native_asin(float3 v);
float4 native_asin(float4 v);

float native_asinh(float v);
float2 native_asinh(float2 v);
float3 native_asinh(float3 v);
float4 native_asinh(float4 v);

float native_asinpi(float v);
float2 native_asinpi(float2 v);
float3 native_asinpi(float3 v);
float4 native_asinpi(float4 v);

float native_atan(float v);
float2 native_atan(float2 v);
float3 native_atan(float3 v);
float4 native_atan(float4 v);

float native_atan2(float numerator, float denominator);
float2 native_atan2(float2 numerator, float2 denominator);
float3 native_atan2(float3 numerator, float3 denominator);
float4 native_atan2(float4 numerator, float4 denominator);

float native_atan2pi(float numerator, float denominator);
float2 native_atan2pi(float2 numerator, float2 denominator);
float3 native_atan2pi(float3 numerator, float3 denominator);
float4 native_atan2pi(float4 numerator, float4 denominator);

float native_atanh(float v);
float2 native_atanh(float2 v);
float3 native_atanh(float3 v);
float4 native_atanh(float4 v);

float native_atanpi(float v);
float2 native_atanpi(float2 v);
float3 native_atanpi(float3 v);
float4 native_atanpi(float4 v);

float native_cbrt(float v);
float2 native_cbrt(float2 v);
float3 native_cbrt(float3 v);
float4 native_cbrt(float4 v);

float native_cos(float v);
float2 native_cos(float2 v);
float3 native_cos(float3 v);
float4 native_cos(float4 v);

float native_cosh(float v);
float2 native_cosh(float2 v);
float3 native_cosh(float3 v);
float4 native_cosh(float4 v);

float native_cospi(float v);
float2 native_cospi(float2 v);
float3 native_cospi(float3 v);
float4 native_cospi(float4 v);

float native_divide(float left_vector, float right_vector);
float2 native_divide(float2 left_vector, float2 right_vector);
float3 native_divide(float3 left_vector, float3 right_vector);
float4 native_divide(float4 left_vector, float4 right_vector);

float native_exp(float v);
float2 native_exp(float2 v);
float3 native_exp(float3 v);
float4 native_exp(float4 v);

float native_exp10(float v);
float2 native_exp10(float2 v);
float3 native_exp10(float3 v);
float4 native_exp10(float4 v);

float native_exp2(float v);
float2 native_exp2(float2 v);
float3 native_exp2(float3 v);
float4 native_exp2(float4 v);

float native_expm1(float v);
float2 native_expm1(float2 v);
float3 native_expm1(float3 v);
float4 native_expm1(float4 v);

float native_hypot(float a, float b);
float2 native_hypot(float2 a, float2 b);
float3 native_hypot(float3 a, float3 b);
float4 native_hypot(float4 a, float4 b);

float native_log(float v);
float2 native_log(float2 v);
float3 native_log(float3 v);
float4 native_log(float4 v);

float native_log10(float v);
float2 native_log10(float2 v);
float3 native_log10(float3 v);
float4 native_log10(float4 v);

float native_log1p(float v);
float2 native_log1p(float2 v);
float3 native_log1p(float3 v);
float4 native_log1p(float4 v);

float native_log2(float v);
float2 native_log2(float2 v);
float3 native_log2(float3 v);
float4 native_log2(float4 v);

float native_powr(float base, float exponent);
float2 native_powr(float2 base, float2 exponent);
float3 native_powr(float3 base, float3 exponent);
float4 native_powr(float4 base, float4 exponent);

float native_recip(float v);
float2 native_recip(float2 v);
float3 native_recip(float3 v);
float4 native_recip(float4 v);

float native_rootn(float v, int n);
float2 native_rootn(float2 v, int2 n);
float3 native_rootn(float3 v, int3 n);
float4 native_rootn(float4 v, int4 n);

float native_rsqrt(float v);
float2 native_rsqrt(float2 v);
float3 native_rsqrt(float3 v);
float4 native_rsqrt(float4 v);

float native_sin(float v);
float2 native_sin(float2 v);
float3 native_sin(float3 v);
float4 native_sin(float4 v);

float native_sincos(float v, float* cos);
float2 native_sincos(float2 v, float2* cos);
float3 native_sincos(float3 v, float3* cos);
float4 native_sincos(float4 v, float4* cos);

float native_sinh(float v);
float2 native_sinh(float2 v);
float3 native_sinh(float3 v);
float4 native_sinh(float4 v);

float native_sinpi(float v);
float2 native_sinpi(float2 v);
float3 native_sinpi(float3 v);
float4 native_sinpi(float4 v);

float native_sqrt(float v);
float2 native_sqrt(float2 v);
float3 native_sqrt(float3 v);
float4 native_sqrt(float4 v);

float native_tan(float v);
float2 native_tan(float2 v);
float3 native_tan(float3 v);
float4 native_tan(float4 v);

float native_tanh(float v);
float2 native_tanh(float2 v);
float3 native_tanh(float3 v);
float4 native_tanh(float4 v);

float native_tanpi(float v);
float2 native_tanpi(float2 v);
float3 native_tanpi(float3 v);
float4 native_tanpi(float4 v);

float nextafter(float v, float target);
float2 nextafter(float2 v, float2 target);
float3 nextafter(float3 v, float3 target);
float4 nextafter(float4 v, float4 target);

float pow(float base, float exponent);
float2 pow(float2 base, float2 exponent);
float3 pow(float3 base, float3 exponent);
float4 pow(float4 base, float4 exponent);

float pown(float base, int exponent);
float2 pown(float2 base, int2 exponent);
float3 pown(float3 base, int3 exponent);
float4 pown(float4 base, int4 exponent);

float powr(float base, float exponent);
float2 powr(float2 base, float2 exponent);
float3 powr(float3 base, float3 exponent);
float4 powr(float4 base, float4 exponent);

float radians(float v);
float2 radians(float2 v);
float3 radians(float3 v);
float4 radians(float4 v);

float remainder(float numerator, float denominator);
float2 remainder(float2 numerator, float2 denominator);
float3 remainder(float3 numerator, float3 denominator);
float4 remainder(float4 numerator, float4 denominator);

float remquo(float numerator, float denominator, int* quotient);
float2 remquo(float2 numerator, float2 denominator, int2* quotient);
float3 remquo(float3 numerator, float3 denominator, int3* quotient);
float4 remquo(float4 numerator, float4 denominator, int4* quotient);

float rint(float v);
float2 rint(float2 v);
float3 rint(float3 v);
float4 rint(float4 v);

float rootn(float v, int n);
float2 rootn(float2 v, int2 n);
float3 rootn(float3 v, int3 n);
float4 rootn(float4 v, int4 n);

float round(float v);
float2 round(float2 v);
float3 round(float3 v);
float4 round(float4 v);

char rsClamp(char amount, char low, char high);
int rsClamp(int amount, int low, int high);
short rsClamp(short amount, short low, short high);
uchar rsClamp(uchar amount, uchar low, uchar high);
uint rsClamp(uint amount, uint low, uint high);
ushort rsClamp(ushort amount, ushort low, ushort high);

float rsFrac(float v);

float rsRand(float max_value);
float rsRand(float min_value, float max_value);
int rsRand(int max_value);
int rsRand(int min_value, int max_value);

float rsqrt(float v);
float2 rsqrt(float2 v);
float3 rsqrt(float3 v);
float4 rsqrt(float4 v);

float sign(float v);
float2 sign(float2 v);
float3 sign(float3 v);
float4 sign(float4 v);

float sin(float v);
float2 sin(float2 v);
float3 sin(float3 v);
float4 sin(float4 v);

float sincos(float v, float* cos);
float2 sincos(float2 v, float2* cos);
float3 sincos(float3 v, float3* cos);
float4 sincos(float4 v, float4* cos);

float sinh(float v);
float2 sinh(float2 v);
float3 sinh(float3 v);
float4 sinh(float4 v);

float sinpi(float v);
float2 sinpi(float2 v);
float3 sinpi(float3 v);
float4 sinpi(float4 v);

float sqrt(float v);
float2 sqrt(float2 v);
float3 sqrt(float3 v);
float4 sqrt(float4 v);

float step(float edge, float v);
float2 step(float edge, float2 v);
float2 step(float2 edge, float v);
float2 step(float2 edge, float2 v);
float3 step(float edge, float3 v);
float3 step(float3 edge, float v);
float3 step(float3 edge, float3 v);
float4 step(float edge, float4 v);
float4 step(float4 edge, float v);
float4 step(float4 edge, float4 v);

float tan(float v);
float2 tan(float2 v);
float3 tan(float3 v);
float4 tan(float4 v);

float tanh(float v);
float2 tanh(float2 v);
float3 tanh(float3 v);
float4 tanh(float4 v);

float tanpi(float v);
float2 tanpi(float2 v);
float3 tanpi(float3 v);
float4 tanpi(float4 v);

float tgamma(float v);
float2 tgamma(float2 v);
float3 tgamma(float3 v);
float4 tgamma(float4 v);

float trunc(float v);
float2 trunc(float2 v);
float3 trunc(float3 v);
float4 trunc(float4 v);

uchar4 rsPackColorTo8888(float r, float g, float b);
uchar4 rsPackColorTo8888(float r, float g, float b, float a);
uchar4 rsPackColorTo8888(float3 color);
uchar4 rsPackColorTo8888(float4 color);
float4 rsUnpackColor8888(uchar4 c);

float4 rsYuvToRGBA_float4(uchar y, uchar u, uchar v);
uchar4 rsYuvToRGBA_uchar4(uchar y, uchar u, uchar v);

- vector functions

float3 cross(float3 left_vector, float3 right_vector);
float4 cross(float4 left_vector, float4 right_vector);

float distance(float  left_vector, float  right_vector);
float distance(float2 left_vector, float2 right_vector);
float distance(float3 left_vector, float3 right_vector);
float distance(float4 left_vector, float4 right_vector);

float dot(float  left_vector, float  right_vector);
float dot(float2 left_vector, float2 right_vector);
float dot(float3 left_vector, float3 right_vector);
float dot(float4 left_vector, float4 right_vector);

float fast_distance(float  left_vector, float  right_vector);
float fast_distance(float2 left_vector, float2 right_vector);
float fast_distance(float3 left_vector, float3 right_vector);
float fast_distance(float4 left_vector, float4 right_vector);

float fast_length(float  v);
float fast_length(float2 v);
float fast_length(float3 v);
float fast_length(float4 v);

float  fast_normalize(float  v);
float2 fast_normalize(float2 v);
float3 fast_normalize(float3 v);
float4 fast_normalize(float4 v);

float length(float  v);
float length(float2 v);
float length(float3 v);
float length(float4 v);

float native_distance(float  left_vector, float  right_vector);
float native_distance(float2 left_vector, float2 right_vector);
float native_distance(float3 left_vector, float3 right_vector);
float native_distance(float4 left_vector, float4 right_vector);

float native_length(float  v);
float native_length(float2 v);
float native_length(float3 v);
float native_length(float4 v);

float  native_normalize(float  v);
float2 native_normalize(float2 v);
float3 native_normalize(float3 v);
float4 native_normalize(float4 v);

float  normalize(float  v);
float2 normalize(float2 v);
float3 normalize(float3 v);
float4 normalize(float4 v);

- conversion functions

char2 convert_char2(char2 v);
char2 convert_char2(double2 v);
char2 convert_char2(float2 v);
char2 convert_char2(int2 v);
char2 convert_char2(long2 v);
char2 convert_char2(short2 v);
char2 convert_char2(uchar2 v);
char2 convert_char2(uint2 v);
char2 convert_char2(ulong2 v);
char2 convert_char2(ushort2 v);

char3 convert_char3(char3 v);
char3 convert_char3(double3 v);
char3 convert_char3(float3 v);
char3 convert_char3(int3 v);
char3 convert_char3(long3 v);
char3 convert_char3(short3 v);
char3 convert_char3(uchar3 v);
char3 convert_char3(uint3 v);
char3 convert_char3(ulong3 v);
char3 convert_char3(ushort3 v);

char4 convert_char4(char4 v);
char4 convert_char4(double4 v);
char4 convert_char4(float4 v);
char4 convert_char4(int4 v);
char4 convert_char4(long4 v);
char4 convert_char4(short4 v);
char4 convert_char4(uchar4 v);
char4 convert_char4(uint4 v);
char4 convert_char4(ulong4 v);
char4 convert_char4(ushort4 v);

double2 convert_double2(char2 v);
double2 convert_double2(double2 v);
double2 convert_double2(float2 v);
double2 convert_double2(int2 v);
double2 convert_double2(long2 v);
double2 convert_double2(short2 v);
double2 convert_double2(uchar2 v);
double2 convert_double2(uint2 v);
double2 convert_double2(ulong2 v);
double2 convert_double2(ushort2 v);

double3 convert_double3(char3 v);
double3 convert_double3(double3 v);
double3 convert_double3(float3 v);
double3 convert_double3(int3 v);
double3 convert_double3(long3 v);
double3 convert_double3(short3 v);
double3 convert_double3(uchar3 v);
double3 convert_double3(uint3 v);
double3 convert_double3(ulong3 v);
double3 convert_double3(ushort3 v);

double4 convert_double4(char4 v);
double4 convert_double4(double4 v);
double4 convert_double4(float4 v);
double4 convert_double4(int4 v);
double4 convert_double4(long4 v);
double4 convert_double4(short4 v);
double4 convert_double4(uchar4 v);
double4 convert_double4(uint4 v);
double4 convert_double4(ulong4 v);
double4 convert_double4(ushort4 v);

float2 convert_float2(char2 v);
float2 convert_float2(double2 v);
float2 convert_float2(float2 v);
float2 convert_float2(int2 v);
float2 convert_float2(long2 v);
float2 convert_float2(short2 v);
float2 convert_float2(uchar2 v);
float2 convert_float2(uint2 v);
float2 convert_float2(ulong2 v);
float2 convert_float2(ushort2 v);

float3 convert_float3(char3 v);
float3 convert_float3(double3 v);
float3 convert_float3(float3 v);
float3 convert_float3(int3 v);
float3 convert_float3(long3 v);
float3 convert_float3(short3 v);
float3 convert_float3(uchar3 v);
float3 convert_float3(uint3 v);
float3 convert_float3(ulong3 v);
float3 convert_float3(ushort3 v);

float4 convert_float4(char4 v);
float4 convert_float4(double4 v);
float4 convert_float4(float4 v);
float4 convert_float4(int4 v);
float4 convert_float4(long4 v);
float4 convert_float4(short4 v);
float4 convert_float4(uchar4 v);
float4 convert_float4(uint4 v);
float4 convert_float4(ulong4 v);
float4 convert_float4(ushort4 v);

int2 convert_int2(char2 v);
int2 convert_int2(double2 v);
int2 convert_int2(float2 v);
int2 convert_int2(int2 v);
int2 convert_int2(long2 v);
int2 convert_int2(short2 v);
int2 convert_int2(uchar2 v);
int2 convert_int2(uint2 v);
int2 convert_int2(ulong2 v);
int2 convert_int2(ushort2 v);

int3 convert_int3(char3 v);
int3 convert_int3(double3 v);
int3 convert_int3(float3 v);
int3 convert_int3(int3 v);
int3 convert_int3(long3 v);
int3 convert_int3(short3 v);
int3 convert_int3(uchar3 v);
int3 convert_int3(uint3 v);
int3 convert_int3(ulong3 v);
int3 convert_int3(ushort3 v);

int4 convert_int4(char4 v);
int4 convert_int4(double4 v);
int4 convert_int4(float4 v);
int4 convert_int4(int4 v);
int4 convert_int4(long4 v);
int4 convert_int4(short4 v);
int4 convert_int4(uchar4 v);
int4 convert_int4(uint4 v);
int4 convert_int4(ulong4 v);
int4 convert_int4(ushort4 v);

long2 convert_long2(char2 v);
long2 convert_long2(double2 v);
long2 convert_long2(float2 v);
long2 convert_long2(int2 v);
long2 convert_long2(long2 v);
long2 convert_long2(short2 v);
long2 convert_long2(uchar2 v);
long2 convert_long2(uint2 v);
long2 convert_long2(ulong2 v);
long2 convert_long2(ushort2 v);

long3 convert_long3(char3 v);
long3 convert_long3(double3 v);
long3 convert_long3(float3 v);
long3 convert_long3(int3 v);
long3 convert_long3(long3 v);
long3 convert_long3(short3 v);
long3 convert_long3(uchar3 v);
long3 convert_long3(uint3 v);
long3 convert_long3(ulong3 v);
long3 convert_long3(ushort3 v);

long4 convert_long4(char4 v);
long4 convert_long4(double4 v);
long4 convert_long4(float4 v);
long4 convert_long4(int4 v);
long4 convert_long4(long4 v);
long4 convert_long4(short4 v);
long4 convert_long4(uchar4 v);
long4 convert_long4(uint4 v);
long4 convert_long4(ulong4 v);
long4 convert_long4(ushort4 v);

short2 convert_short2(char2 v);
short2 convert_short2(double2 v);
short2 convert_short2(float2 v);
short2 convert_short2(int2 v);
short2 convert_short2(long2 v);
short2 convert_short2(short2 v);
short2 convert_short2(uchar2 v);
short2 convert_short2(uint2 v);
short2 convert_short2(ulong2 v);
short2 convert_short2(ushort2 v);

short3 convert_short3(char3 v);
short3 convert_short3(double3 v);
short3 convert_short3(float3 v);
short3 convert_short3(int3 v);
short3 convert_short3(long3 v);
short3 convert_short3(short3 v);
short3 convert_short3(uchar3 v);
short3 convert_short3(uint3 v);
short3 convert_short3(ulong3 v);
short3 convert_short3(ushort3 v);

short4 convert_short4(char4 v);
short4 convert_short4(double4 v);
short4 convert_short4(float4 v);
short4 convert_short4(int4 v);
short4 convert_short4(long4 v);
short4 convert_short4(short4 v);
short4 convert_short4(uchar4 v);
short4 convert_short4(uint4 v);
short4 convert_short4(ulong4 v);
short4 convert_short4(ushort4 v);

uchar2 convert_uchar2(char2 v);
uchar2 convert_uchar2(double2 v);
uchar2 convert_uchar2(float2 v);
uchar2 convert_uchar2(int2 v);
uchar2 convert_uchar2(long2 v);
uchar2 convert_uchar2(short2 v);
uchar2 convert_uchar2(uchar2 v);
uchar2 convert_uchar2(uint2 v);
uchar2 convert_uchar2(ulong2 v);
uchar2 convert_uchar2(ushort2 v);

uchar3 convert_uchar3(char3 v);
uchar3 convert_uchar3(double3 v);
uchar3 convert_uchar3(float3 v);
uchar3 convert_uchar3(int3 v);
uchar3 convert_uchar3(long3 v);
uchar3 convert_uchar3(short3 v);
uchar3 convert_uchar3(uchar3 v);
uchar3 convert_uchar3(uint3 v);
uchar3 convert_uchar3(ulong3 v);
uchar3 convert_uchar3(ushort3 v);

uchar4 convert_uchar4(char4 v);
uchar4 convert_uchar4(double4 v);
uchar4 convert_uchar4(float4 v);
uchar4 convert_uchar4(int4 v);
uchar4 convert_uchar4(long4 v);
uchar4 convert_uchar4(short4 v);
uchar4 convert_uchar4(uchar4 v);
uchar4 convert_uchar4(uint4 v);
uchar4 convert_uchar4(ulong4 v);
uchar4 convert_uchar4(ushort4 v);

uint2 convert_uint2(char2 v);
uint2 convert_uint2(double2 v);
uint2 convert_uint2(float2 v);
uint2 convert_uint2(int2 v);
uint2 convert_uint2(long2 v);
uint2 convert_uint2(short2 v);
uint2 convert_uint2(uchar2 v);
uint2 convert_uint2(uint2 v);
uint2 convert_uint2(ulong2 v);
uint2 convert_uint2(ushort2 v);

uint3 convert_uint3(char3 v);
uint3 convert_uint3(double3 v);
uint3 convert_uint3(float3 v);
uint3 convert_uint3(int3 v);
uint3 convert_uint3(long3 v);
uint3 convert_uint3(short3 v);
uint3 convert_uint3(uchar3 v);
uint3 convert_uint3(uint3 v);
uint3 convert_uint3(ulong3 v);
uint3 convert_uint3(ushort3 v);

uint4 convert_uint4(char4 v);
uint4 convert_uint4(double4 v);
uint4 convert_uint4(float4 v);
uint4 convert_uint4(int4 v);
uint4 convert_uint4(long4 v);
uint4 convert_uint4(short4 v);
uint4 convert_uint4(uchar4 v);
uint4 convert_uint4(uint4 v);
uint4 convert_uint4(ulong4 v);
uint4 convert_uint4(ushort4 v);

ulong2 convert_ulong2(char2 v);
ulong2 convert_ulong2(double2 v);
ulong2 convert_ulong2(float2 v);
ulong2 convert_ulong2(int2 v);
ulong2 convert_ulong2(long2 v);
ulong2 convert_ulong2(short2 v);
ulong2 convert_ulong2(uchar2 v);
ulong2 convert_ulong2(uint2 v);
ulong2 convert_ulong2(ulong2 v);
ulong2 convert_ulong2(ushort2 v);

ulong3 convert_ulong3(char3 v);
ulong3 convert_ulong3(double3 v);
ulong3 convert_ulong3(float3 v);
ulong3 convert_ulong3(int3 v);
ulong3 convert_ulong3(long3 v);
ulong3 convert_ulong3(short3 v);
ulong3 convert_ulong3(uchar3 v);
ulong3 convert_ulong3(uint3 v);
ulong3 convert_ulong3(ulong3 v);
ulong3 convert_ulong3(ushort3 v);

ulong4 convert_ulong4(char4 v);
ulong4 convert_ulong4(double4 v);
ulong4 convert_ulong4(float4 v);
ulong4 convert_ulong4(int4 v);
ulong4 convert_ulong4(long4 v);
ulong4 convert_ulong4(short4 v);
ulong4 convert_ulong4(uchar4 v);
ulong4 convert_ulong4(uint4 v);
ulong4 convert_ulong4(ulong4 v);
ulong4 convert_ulong4(ushort4 v);

ushort2 convert_ushort2(char2 v);
ushort2 convert_ushort2(double2 v);
ushort2 convert_ushort2(float2 v);
ushort2 convert_ushort2(int2 v);
ushort2 convert_ushort2(long2 v);
ushort2 convert_ushort2(short2 v);
ushort2 convert_ushort2(uchar2 v);
ushort2 convert_ushort2(uint2 v);
ushort2 convert_ushort2(ulong2 v);
ushort2 convert_ushort2(ushort2 v);

ushort3 convert_ushort3(char3 v);
ushort3 convert_ushort3(double3 v);
ushort3 convert_ushort3(float3 v);
ushort3 convert_ushort3(int3 v);
ushort3 convert_ushort3(long3 v);
ushort3 convert_ushort3(short3 v);
ushort3 convert_ushort3(uchar3 v);
ushort3 convert_ushort3(uint3 v);
ushort3 convert_ushort3(ulong3 v);
ushort3 convert_ushort3(ushort3 v);

ushort4 convert_ushort4(char4 v);
ushort4 convert_ushort4(double4 v);
ushort4 convert_ushort4(float4 v);
ushort4 convert_ushort4(int4 v);
ushort4 convert_ushort4(long4 v);
ushort4 convert_ushort4(short4 v);
ushort4 convert_ushort4(uchar4 v);
ushort4 convert_ushort4(uint4 v);
ushort4 convert_ushort4(ulong4 v);
ushort4 convert_ushort4(ushort4 v);

uchar4 rsPackColorTo8888(float r, float g, float b);
uchar4 rsPackColorTo8888(float r, float g, float b, float a);
uchar4 rsPackColorTo8888(float3 color);
uchar4 rsPackColorTo8888(float4 color);

float4 rsUnpackColor8888(uchar4 c);

float4 rsYuvToRGBA_float4(uchar y, uchar u, uchar v);

uchar4 rsYuvToRGBA_uchar4(uchar y, uchar u, uchar v);

- matrix functions, some of these are not supported yet

-bool rsMatrixInverse(rs_matrix4x4* m);

-bool rsMatrixInverseTranspose(rs_matrix4x4* m);

-void rsMatrixLoad(rs_matrix2x2* destination, float* array);
-void rsMatrixLoad(rs_matrix2x2* destination, rs_matrix2x2* source);
-void rsMatrixLoad(rs_matrix3x3* destination, float* array);
-void rsMatrixLoad(rs_matrix3x3* destination, rs_matrix3x3* source);
-void rsMatrixLoad(rs_matrix4x4* destination, float* array);
-void rsMatrixLoad(rs_matrix4x4* destination, rs_matrix2x2* source);
-void rsMatrixLoad(rs_matrix4x4* destination, rs_matrix3x3* source);
-void rsMatrixLoad(rs_matrix4x4* destination, rs_matrix4x4* source);

-void rsMatrixLoadFrustum(rs_matrix4x4* m, float left, float right, float bottom, float top, float near, float far);

-void rsMatrixLoadIdentity(rs_matrix2x2* m);
-void rsMatrixLoadIdentity(rs_matrix3x3* m);
-void rsMatrixLoadIdentity(rs_matrix4x4* m);

-void rsMatrixLoadMultiply(rs_matrix2x2* m, rs_matrix2x2* lhs, rs_matrix2x2* rhs);
-void rsMatrixLoadMultiply(rs_matrix3x3* m, rs_matrix3x3* lhs, rs_matrix3x3* rhs);
-void rsMatrixLoadMultiply(rs_matrix4x4* m, rs_matrix4x4* lhs, rs_matrix4x4* rhs);

-void rsMatrixLoadOrtho(rs_matrix4x4* m, float left, float right, float bottom, float top, float near, float far);

-void rsMatrixLoadPerspective(rs_matrix4x4* m, float fovy, float aspect, float near, float far);

-void rsMatrixLoadRotate(rs_matrix4x4* m, float rot, float x, float y, float z);

-void rsMatrixLoadScale(rs_matrix4x4* m, float x, float y, float z);

-void rsMatrixLoadTranslate(rs_matrix4x4* m, float x, float y, float z);

float2 rsMatrixMultiply(rs_matrix2x2* m, float2 in);
float3 rsMatrixMultiply(rs_matrix3x3* m, float2 in);
float3 rsMatrixMultiply(rs_matrix3x3* m, float3 in);
float4 rsMatrixMultiply(rs_matrix4x4* m, float2 in);
float4 rsMatrixMultiply(rs_matrix4x4* m, float3 in);
float4 rsMatrixMultiply(rs_matrix4x4* m, float4 in);
void rsMatrixMultiply(rs_matrix2x2* m, rs_matrix2x2* rhs);
void rsMatrixMultiply(rs_matrix3x3* m, rs_matrix3x3* rhs);
void rsMatrixMultiply(rs_matrix4x4* m, rs_matrix4x4* rhs);

-void rsMatrixRotate(rs_matrix4x4* m, float rot, float x, float y, float z);

-void rsMatrixScale(rs_matrix4x4* m, float x, float y, float z);

void rsMatrixSet(rs_matrix2x2* m, 0, 1, float v);
void rsMatrixSet(rs_matrix3x3* m, 2, 0, float v);
void rsMatrixSet(rs_matrix4x4* m, 1, 3, float v);

-void rsMatrixTranslate(rs_matrix4x4* m, float x, float y, float z);

-void rsMatrixTranspose(rs_matrix2x2* m);
-void rsMatrixTranspose(rs_matrix3x3* m);
-void rsMatrixTranspose(rs_matrix4x4* m);

- quaternion functions

void rsQuaternionAdd(rs_quaternion* q, rs_quaternion* rhs);

void rsQuaternionConjugate(rs_quaternion* q);

float rsQuaternionDot(rs_quaternion* q0, rs_quaternion* q1);

void rsQuaternionGetMatrixUnit(rs_matrix4x4* m, rs_quaternion* q);

void rsQuaternionLoadRotate(rs_quaternion* q, float rot, float x, float y, float z);

void rsQuaternionLoadRotateUnit(rs_quaternion* q, float rot, float x, float y, float z);

void rsQuaternionMultiply(rs_quaternion* q, rs_quaternion* rhs);
void rsQuaternionMultiply(rs_quaternion* q, float scalar);

void rsQuaternionNormalize(rs_quaternion* q);

void rsQuaternionSet(rs_quaternion* q, rs_quaternion* rhs);
void rsQuaternionSet(rs_quaternion* q, float w, float x, float y, float z);

void rsQuaternionSlerp(rs_quaternion* q, rs_quaternion* q0, rs_quaternion* q1, float t);

- allocation data access functions, this is a subset, since we don't have all types of allocations

-void rsAllocationCopy1DRange(allocation_1D_global, uint dstOff, uint dstMip, uint count, allocation_1D_global2, uint srcOff, uint srcMip);

-void rsAllocationCopy2DRange(allocation_2D_global, uint dstXoff, uint dstYoff, uint dstMip, rs_allocation_cubemap_face dstFace, uint width, uint height, allocation_2D_global2, uint srcXoff, uint srcYoff, uint srcMip, rs_allocation_cubemap_face srcFace);

int2 rsAllocationVLoadX_int2(allocation_1D_global, 0);
int2 rsAllocationVLoadX_int2(allocation_2D_global, 24, 25);
int2 rsAllocationVLoadX_int2(allocation_3D_global, 0, 1, 0);
int3 rsAllocationVLoadX_int3(allocation_1D_global, 1);
int3 rsAllocationVLoadX_int3(allocation_2D_global, 27, 28);
int3 rsAllocationVLoadX_int3(allocation_3D_global, 1, 0, 1);
int4 rsAllocationVLoadX_int4(allocation_1D_global, 0);
int4 rsAllocationVLoadX_int4(allocation_2D_global, 29, 30);
int4 rsAllocationVLoadX_int4(allocation_3D_global, 0, 1, 0);

void rsAllocationVStoreX_int2(allocation_1D_global, int2 val, 2);
void rsAllocationVStoreX_int2(allocation_2D_global, int2 val, 6, 7);
void rsAllocationVStoreX_int2(allocation_3D_global, int2 val, 0, 1, 0);
void rsAllocationVStoreX_int3(allocation_1D_global, int3 val, 1);
void rsAllocationVStoreX_int3(allocation_2D_global, int3 val, 12, 13);
void rsAllocationVStoreX_int3(allocation_3D_global, int3 val, 1, 0, 1);
void rsAllocationVStoreX_int4(allocation_1D_global, int4 val, 0);
void rsAllocationVStoreX_int4(allocation_2D_global, int4 val, 18, 19);
void rsAllocationVStoreX_int4(allocation_3D_global, int4 val, 0, 1, 0);

void* rsGetElementAt(allocation_1D_global, 0);
void* rsGetElementAt(allocation_2D_global, 20, 21);
void* rsGetElementAt(allocation_3D_global, 1, 0, 1);
int rsGetElementAt_int(allocation_1D_global, 1);
int rsGetElementAt_int(allocation_2D_global, 22, 23);
int rsGetElementAt_int(allocation_3D_global, 0, 1, 0);

uchar rsGetElementAtYuv_uchar_U(allocation_YUV_2D_global, 0, 1);

uchar rsGetElementAtYuv_uchar_V(allocation_YUV_2D_global, 2, 3);

uchar rsGetElementAtYuv_uchar_Y(allocation_YUV_2D_global, 0, 1);

float4 rsSample(allocation_1D_global, rs_sampler s, float location);
float4 rsSample(allocation_1D_global, rs_sampler s, float location, float lod);
float4 rsSample(allocation_1D_global, rs_sampler s, float2 location);
float4 rsSample(allocation_1D_global, rs_sampler s, float2 location, float lod);

void rsSetElementAt(allocation_1D_global, int* ptr, 2);
void rsSetElementAt(allocation_2D_global, int* ptr, 24, 25);
void rsSetElementAt_int(allocation_1D_global, int val, 0);
void rsSetElementAt_int(allocation_2D_global, int val, 26, 27);
void rsSetElementAt_int(allocation_3D_global, int val, 1, 0, 1);
'''.splitlines())


TYPE_MAP = {
    'void' : '',
    'char' : r'\((signed )?char\)',
    'uchar' : r'\(uchar\)',
    'short' : r'\(short\)',
    'ushort' : r'\(ushort\)',
    'int' : r'\(int\)',
    'uint' : r'\(uint\)',
    'long' : r'\((long )?long\)',
    'ulong' : r'\(ulong\)',
    'float' : r'\(float\)',
    'float2' : r'\(float2\)',
    'float3' : r'\(float3\)',
    'float4' : r'\(float4\)'
    }


def _build_arg(token):
    '''Given a C argument construct an lldb expression for the argument.

    Given a token, which represents a single argument of a C function
    declaration, construct an lldb expression for the argument.

    Args:
        token: A string representing a single argument to a function. This
               can be either [type][name] (e.g. int arg) or [value] (e.g. 5).

    Returns:
        The string that is the lldb expression for that argument, e.g.
        int_global or 5.
    '''
    if len(token.split()) == 1:
        # This is a fixed constant. Just take that as output.
        return token

    result = token.split()[0]

    # Remove the rs_ prefix, because our globals' names don't have them
    if result[:3] == 'rs_':
        result = result[3:]

    # If the function expects a pointer, take the address of the global
    if result[-1] == '*':
        result = '&' + result
        result = result[:-1]

    result += '_global'
    return result


def build_expr(line):
    '''Build an lldb expression given a function prototype.

    Given a function declaration, this function will construct an lldb
    expression to call it.

    Args:
        line: A string representing a function declaration.

    Returns:
        The string that is the lldb expression.
    '''
    tokens = re.findall(r"[^(),;]+", line)
    assert len(tokens) > 0
    ret_name = tokens[0].split()
    ret = ret_name[0]
    name = ret_name[1]
    expr = 'expr {0}('.format(name)

    first = True
    for tok in tokens[1:]:
        if not first:
            expr += ', '
        expr += _build_arg(tok)
        first = False

    expr += ')'
    return ret, expr
