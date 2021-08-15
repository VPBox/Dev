// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(result)

// This test case was created by ./gen-result.pl.
// It exercises all legal Java-reflectable result types, so that we can ensure
// (a) We do not choke when compiling them
// (b) We reflect them correctly

// One example struct type
typedef struct MyStruct { float f; double d; } MyStruct;

#pragma rs reduce(my_half) accumulator(my_half_accum) combiner(my_half_comb)
static void my_half_accum(half *accum, half val) { }
static void my_half_comb(half *accum, const half *other) { }

#pragma rs reduce(my_half2) accumulator(my_half2_accum) combiner(my_half2_comb)
static void my_half2_accum(half2 *accum, half2 val) { }
static void my_half2_comb(half2 *accum, const half2 *other) { }

#pragma rs reduce(my_half4) accumulator(my_half4_accum) combiner(my_half4_comb)
static void my_half4_accum(half4 *accum, half4 val) { }
static void my_half4_comb(half4 *accum, const half4 *other) { }

#pragma rs reduce(my_array_half) accumulator(my_array_half_accum) combiner(my_array_half_comb)
typedef half array_half[7];
static void my_array_half_accum(array_half *accum, half val) { }
static void my_array_half_comb(array_half *accum, const array_half *other) { }

#pragma rs reduce(my_array_half2) accumulator(my_array_half2_accum) combiner(my_array_half2_comb)
typedef half2 array_half2[7];
static void my_array_half2_accum(array_half2 *accum, half2 val) { }
static void my_array_half2_comb(array_half2 *accum, const array_half2 *other) { }

#pragma rs reduce(my_array_half4) accumulator(my_array_half4_accum) combiner(my_array_half4_comb)
typedef half4 array_half4[7];
static void my_array_half4_accum(array_half4 *accum, half4 val) { }
static void my_array_half4_comb(array_half4 *accum, const array_half4 *other) { }

#pragma rs reduce(my_float) accumulator(my_float_accum) combiner(my_float_comb)
static void my_float_accum(float *accum, float val) { }
static void my_float_comb(float *accum, const float *other) { }

#pragma rs reduce(my_float2) accumulator(my_float2_accum) combiner(my_float2_comb)
static void my_float2_accum(float2 *accum, float2 val) { }
static void my_float2_comb(float2 *accum, const float2 *other) { }

#pragma rs reduce(my_float4) accumulator(my_float4_accum) combiner(my_float4_comb)
static void my_float4_accum(float4 *accum, float4 val) { }
static void my_float4_comb(float4 *accum, const float4 *other) { }

#pragma rs reduce(my_array_float) accumulator(my_array_float_accum) combiner(my_array_float_comb)
typedef float array_float[7];
static void my_array_float_accum(array_float *accum, float val) { }
static void my_array_float_comb(array_float *accum, const array_float *other) { }

#pragma rs reduce(my_array_float2) accumulator(my_array_float2_accum) combiner(my_array_float2_comb)
typedef float2 array_float2[7];
static void my_array_float2_accum(array_float2 *accum, float2 val) { }
static void my_array_float2_comb(array_float2 *accum, const array_float2 *other) { }

#pragma rs reduce(my_array_float4) accumulator(my_array_float4_accum) combiner(my_array_float4_comb)
typedef float4 array_float4[7];
static void my_array_float4_accum(array_float4 *accum, float4 val) { }
static void my_array_float4_comb(array_float4 *accum, const array_float4 *other) { }

#pragma rs reduce(my_double) accumulator(my_double_accum) combiner(my_double_comb)
static void my_double_accum(double *accum, double val) { }
static void my_double_comb(double *accum, const double *other) { }

#pragma rs reduce(my_double2) accumulator(my_double2_accum) combiner(my_double2_comb)
static void my_double2_accum(double2 *accum, double2 val) { }
static void my_double2_comb(double2 *accum, const double2 *other) { }

#pragma rs reduce(my_double4) accumulator(my_double4_accum) combiner(my_double4_comb)
static void my_double4_accum(double4 *accum, double4 val) { }
static void my_double4_comb(double4 *accum, const double4 *other) { }

#pragma rs reduce(my_array_double) accumulator(my_array_double_accum) combiner(my_array_double_comb)
typedef double array_double[7];
static void my_array_double_accum(array_double *accum, double val) { }
static void my_array_double_comb(array_double *accum, const array_double *other) { }

#pragma rs reduce(my_array_double2) accumulator(my_array_double2_accum) combiner(my_array_double2_comb)
typedef double2 array_double2[7];
static void my_array_double2_accum(array_double2 *accum, double2 val) { }
static void my_array_double2_comb(array_double2 *accum, const array_double2 *other) { }

#pragma rs reduce(my_array_double4) accumulator(my_array_double4_accum) combiner(my_array_double4_comb)
typedef double4 array_double4[7];
static void my_array_double4_accum(array_double4 *accum, double4 val) { }
static void my_array_double4_comb(array_double4 *accum, const array_double4 *other) { }

#pragma rs reduce(my_char) accumulator(my_char_accum) combiner(my_char_comb)
static void my_char_accum(char *accum, char val) { }
static void my_char_comb(char *accum, const char *other) { }

#pragma rs reduce(my_char2) accumulator(my_char2_accum) combiner(my_char2_comb)
static void my_char2_accum(char2 *accum, char2 val) { }
static void my_char2_comb(char2 *accum, const char2 *other) { }

#pragma rs reduce(my_char4) accumulator(my_char4_accum) combiner(my_char4_comb)
static void my_char4_accum(char4 *accum, char4 val) { }
static void my_char4_comb(char4 *accum, const char4 *other) { }

#pragma rs reduce(my_array_char) accumulator(my_array_char_accum) combiner(my_array_char_comb)
typedef char array_char[7];
static void my_array_char_accum(array_char *accum, char val) { }
static void my_array_char_comb(array_char *accum, const array_char *other) { }

#pragma rs reduce(my_array_char2) accumulator(my_array_char2_accum) combiner(my_array_char2_comb)
typedef char2 array_char2[7];
static void my_array_char2_accum(array_char2 *accum, char2 val) { }
static void my_array_char2_comb(array_char2 *accum, const array_char2 *other) { }

#pragma rs reduce(my_array_char4) accumulator(my_array_char4_accum) combiner(my_array_char4_comb)
typedef char4 array_char4[7];
static void my_array_char4_accum(array_char4 *accum, char4 val) { }
static void my_array_char4_comb(array_char4 *accum, const array_char4 *other) { }

#pragma rs reduce(my_short) accumulator(my_short_accum) combiner(my_short_comb)
static void my_short_accum(short *accum, short val) { }
static void my_short_comb(short *accum, const short *other) { }

#pragma rs reduce(my_short2) accumulator(my_short2_accum) combiner(my_short2_comb)
static void my_short2_accum(short2 *accum, short2 val) { }
static void my_short2_comb(short2 *accum, const short2 *other) { }

#pragma rs reduce(my_short4) accumulator(my_short4_accum) combiner(my_short4_comb)
static void my_short4_accum(short4 *accum, short4 val) { }
static void my_short4_comb(short4 *accum, const short4 *other) { }

#pragma rs reduce(my_array_short) accumulator(my_array_short_accum) combiner(my_array_short_comb)
typedef short array_short[7];
static void my_array_short_accum(array_short *accum, short val) { }
static void my_array_short_comb(array_short *accum, const array_short *other) { }

#pragma rs reduce(my_array_short2) accumulator(my_array_short2_accum) combiner(my_array_short2_comb)
typedef short2 array_short2[7];
static void my_array_short2_accum(array_short2 *accum, short2 val) { }
static void my_array_short2_comb(array_short2 *accum, const array_short2 *other) { }

#pragma rs reduce(my_array_short4) accumulator(my_array_short4_accum) combiner(my_array_short4_comb)
typedef short4 array_short4[7];
static void my_array_short4_accum(array_short4 *accum, short4 val) { }
static void my_array_short4_comb(array_short4 *accum, const array_short4 *other) { }

#pragma rs reduce(my_int) accumulator(my_int_accum) combiner(my_int_comb)
static void my_int_accum(int *accum, int val) { }
static void my_int_comb(int *accum, const int *other) { }

#pragma rs reduce(my_int2) accumulator(my_int2_accum) combiner(my_int2_comb)
static void my_int2_accum(int2 *accum, int2 val) { }
static void my_int2_comb(int2 *accum, const int2 *other) { }

#pragma rs reduce(my_int4) accumulator(my_int4_accum) combiner(my_int4_comb)
static void my_int4_accum(int4 *accum, int4 val) { }
static void my_int4_comb(int4 *accum, const int4 *other) { }

#pragma rs reduce(my_array_int) accumulator(my_array_int_accum) combiner(my_array_int_comb)
typedef int array_int[7];
static void my_array_int_accum(array_int *accum, int val) { }
static void my_array_int_comb(array_int *accum, const array_int *other) { }

#pragma rs reduce(my_array_int2) accumulator(my_array_int2_accum) combiner(my_array_int2_comb)
typedef int2 array_int2[7];
static void my_array_int2_accum(array_int2 *accum, int2 val) { }
static void my_array_int2_comb(array_int2 *accum, const array_int2 *other) { }

#pragma rs reduce(my_array_int4) accumulator(my_array_int4_accum) combiner(my_array_int4_comb)
typedef int4 array_int4[7];
static void my_array_int4_accum(array_int4 *accum, int4 val) { }
static void my_array_int4_comb(array_int4 *accum, const array_int4 *other) { }

#pragma rs reduce(my_long) accumulator(my_long_accum) combiner(my_long_comb)
static void my_long_accum(long *accum, long val) { }
static void my_long_comb(long *accum, const long *other) { }

#pragma rs reduce(my_long2) accumulator(my_long2_accum) combiner(my_long2_comb)
static void my_long2_accum(long2 *accum, long2 val) { }
static void my_long2_comb(long2 *accum, const long2 *other) { }

#pragma rs reduce(my_long4) accumulator(my_long4_accum) combiner(my_long4_comb)
static void my_long4_accum(long4 *accum, long4 val) { }
static void my_long4_comb(long4 *accum, const long4 *other) { }

#pragma rs reduce(my_array_long) accumulator(my_array_long_accum) combiner(my_array_long_comb)
typedef long array_long[7];
static void my_array_long_accum(array_long *accum, long val) { }
static void my_array_long_comb(array_long *accum, const array_long *other) { }

#pragma rs reduce(my_array_long2) accumulator(my_array_long2_accum) combiner(my_array_long2_comb)
typedef long2 array_long2[7];
static void my_array_long2_accum(array_long2 *accum, long2 val) { }
static void my_array_long2_comb(array_long2 *accum, const array_long2 *other) { }

#pragma rs reduce(my_array_long4) accumulator(my_array_long4_accum) combiner(my_array_long4_comb)
typedef long4 array_long4[7];
static void my_array_long4_accum(array_long4 *accum, long4 val) { }
static void my_array_long4_comb(array_long4 *accum, const array_long4 *other) { }

#pragma rs reduce(my_uchar) accumulator(my_uchar_accum) combiner(my_uchar_comb)
static void my_uchar_accum(uchar *accum, uchar val) { }
static void my_uchar_comb(uchar *accum, const uchar *other) { }

#pragma rs reduce(my_uchar2) accumulator(my_uchar2_accum) combiner(my_uchar2_comb)
static void my_uchar2_accum(uchar2 *accum, uchar2 val) { }
static void my_uchar2_comb(uchar2 *accum, const uchar2 *other) { }

#pragma rs reduce(my_uchar4) accumulator(my_uchar4_accum) combiner(my_uchar4_comb)
static void my_uchar4_accum(uchar4 *accum, uchar4 val) { }
static void my_uchar4_comb(uchar4 *accum, const uchar4 *other) { }

#pragma rs reduce(my_array_uchar) accumulator(my_array_uchar_accum) combiner(my_array_uchar_comb)
typedef uchar array_uchar[7];
static void my_array_uchar_accum(array_uchar *accum, uchar val) { }
static void my_array_uchar_comb(array_uchar *accum, const array_uchar *other) { }

#pragma rs reduce(my_array_uchar2) accumulator(my_array_uchar2_accum) combiner(my_array_uchar2_comb)
typedef uchar2 array_uchar2[7];
static void my_array_uchar2_accum(array_uchar2 *accum, uchar2 val) { }
static void my_array_uchar2_comb(array_uchar2 *accum, const array_uchar2 *other) { }

#pragma rs reduce(my_array_uchar4) accumulator(my_array_uchar4_accum) combiner(my_array_uchar4_comb)
typedef uchar4 array_uchar4[7];
static void my_array_uchar4_accum(array_uchar4 *accum, uchar4 val) { }
static void my_array_uchar4_comb(array_uchar4 *accum, const array_uchar4 *other) { }

#pragma rs reduce(my_ushort) accumulator(my_ushort_accum) combiner(my_ushort_comb)
static void my_ushort_accum(ushort *accum, ushort val) { }
static void my_ushort_comb(ushort *accum, const ushort *other) { }

#pragma rs reduce(my_ushort2) accumulator(my_ushort2_accum) combiner(my_ushort2_comb)
static void my_ushort2_accum(ushort2 *accum, ushort2 val) { }
static void my_ushort2_comb(ushort2 *accum, const ushort2 *other) { }

#pragma rs reduce(my_ushort4) accumulator(my_ushort4_accum) combiner(my_ushort4_comb)
static void my_ushort4_accum(ushort4 *accum, ushort4 val) { }
static void my_ushort4_comb(ushort4 *accum, const ushort4 *other) { }

#pragma rs reduce(my_array_ushort) accumulator(my_array_ushort_accum) combiner(my_array_ushort_comb)
typedef ushort array_ushort[7];
static void my_array_ushort_accum(array_ushort *accum, ushort val) { }
static void my_array_ushort_comb(array_ushort *accum, const array_ushort *other) { }

#pragma rs reduce(my_array_ushort2) accumulator(my_array_ushort2_accum) combiner(my_array_ushort2_comb)
typedef ushort2 array_ushort2[7];
static void my_array_ushort2_accum(array_ushort2 *accum, ushort2 val) { }
static void my_array_ushort2_comb(array_ushort2 *accum, const array_ushort2 *other) { }

#pragma rs reduce(my_array_ushort4) accumulator(my_array_ushort4_accum) combiner(my_array_ushort4_comb)
typedef ushort4 array_ushort4[7];
static void my_array_ushort4_accum(array_ushort4 *accum, ushort4 val) { }
static void my_array_ushort4_comb(array_ushort4 *accum, const array_ushort4 *other) { }

#pragma rs reduce(my_uint) accumulator(my_uint_accum) combiner(my_uint_comb)
static void my_uint_accum(uint *accum, uint val) { }
static void my_uint_comb(uint *accum, const uint *other) { }

#pragma rs reduce(my_uint2) accumulator(my_uint2_accum) combiner(my_uint2_comb)
static void my_uint2_accum(uint2 *accum, uint2 val) { }
static void my_uint2_comb(uint2 *accum, const uint2 *other) { }

#pragma rs reduce(my_uint4) accumulator(my_uint4_accum) combiner(my_uint4_comb)
static void my_uint4_accum(uint4 *accum, uint4 val) { }
static void my_uint4_comb(uint4 *accum, const uint4 *other) { }

#pragma rs reduce(my_array_uint) accumulator(my_array_uint_accum) combiner(my_array_uint_comb)
typedef uint array_uint[7];
static void my_array_uint_accum(array_uint *accum, uint val) { }
static void my_array_uint_comb(array_uint *accum, const array_uint *other) { }

#pragma rs reduce(my_array_uint2) accumulator(my_array_uint2_accum) combiner(my_array_uint2_comb)
typedef uint2 array_uint2[7];
static void my_array_uint2_accum(array_uint2 *accum, uint2 val) { }
static void my_array_uint2_comb(array_uint2 *accum, const array_uint2 *other) { }

#pragma rs reduce(my_array_uint4) accumulator(my_array_uint4_accum) combiner(my_array_uint4_comb)
typedef uint4 array_uint4[7];
static void my_array_uint4_accum(array_uint4 *accum, uint4 val) { }
static void my_array_uint4_comb(array_uint4 *accum, const array_uint4 *other) { }

#pragma rs reduce(my_ulong) accumulator(my_ulong_accum) combiner(my_ulong_comb)
static void my_ulong_accum(ulong *accum, ulong val) { }
static void my_ulong_comb(ulong *accum, const ulong *other) { }

#pragma rs reduce(my_ulong2) accumulator(my_ulong2_accum) combiner(my_ulong2_comb)
static void my_ulong2_accum(ulong2 *accum, ulong2 val) { }
static void my_ulong2_comb(ulong2 *accum, const ulong2 *other) { }

#pragma rs reduce(my_ulong4) accumulator(my_ulong4_accum) combiner(my_ulong4_comb)
static void my_ulong4_accum(ulong4 *accum, ulong4 val) { }
static void my_ulong4_comb(ulong4 *accum, const ulong4 *other) { }

#pragma rs reduce(my_array_ulong) accumulator(my_array_ulong_accum) combiner(my_array_ulong_comb)
typedef ulong array_ulong[7];
static void my_array_ulong_accum(array_ulong *accum, ulong val) { }
static void my_array_ulong_comb(array_ulong *accum, const array_ulong *other) { }

#pragma rs reduce(my_array_ulong2) accumulator(my_array_ulong2_accum) combiner(my_array_ulong2_comb)
typedef ulong2 array_ulong2[7];
static void my_array_ulong2_accum(array_ulong2 *accum, ulong2 val) { }
static void my_array_ulong2_comb(array_ulong2 *accum, const array_ulong2 *other) { }

#pragma rs reduce(my_array_ulong4) accumulator(my_array_ulong4_accum) combiner(my_array_ulong4_comb)
typedef ulong4 array_ulong4[7];
static void my_array_ulong4_accum(array_ulong4 *accum, ulong4 val) { }
static void my_array_ulong4_comb(array_ulong4 *accum, const array_ulong4 *other) { }

#pragma rs reduce(my_bool) accumulator(my_bool_accum) combiner(my_bool_comb)
static void my_bool_accum(bool *accum, bool val) { }
static void my_bool_comb(bool *accum, const bool *other) { }

#pragma rs reduce(my_array_bool) accumulator(my_array_bool_accum) combiner(my_array_bool_comb)
typedef bool array_bool[7];
static void my_array_bool_accum(array_bool *accum, bool val) { }
static void my_array_bool_comb(array_bool *accum, const array_bool *other) { }

#pragma rs reduce(my_MyStruct) accumulator(my_MyStruct_accum) combiner(my_MyStruct_comb)
static void my_MyStruct_accum(MyStruct *accum, MyStruct val) { }
static void my_MyStruct_comb(MyStruct *accum, const MyStruct *other) { }

#pragma rs reduce(my_array_MyStruct) accumulator(my_array_MyStruct_accum) combiner(my_array_MyStruct_comb)
typedef MyStruct array_MyStruct[7];
static void my_array_MyStruct_accum(array_MyStruct *accum, MyStruct val) { }
static void my_array_MyStruct_comb(array_MyStruct *accum, const array_MyStruct *other) { }
