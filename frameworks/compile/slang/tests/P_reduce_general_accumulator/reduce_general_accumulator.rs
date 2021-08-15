// -target-api 0 -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(accumulator)

// This test case was created by ./gen-accumulator.pl.
// It exercises various legal accumulator types, so that we can ensure
// (a) We do not choke when compiling them
// (b) They do not inhibit reflection

// One example enum type
typedef enum { E1, E2, E3 } MyEnum;

// Example pointer types
typedef float (*MyPtrArray)[3][11];
typedef int (*MyPtrFn)(double);
typedef struct Incomplete *MyPtrIncomplete;
typedef volatile short *MyPtrShort;
typedef const void *MyPtrVoid;

// One example struct type
typedef struct { float f; double d; char *c; } MyStruct;

// One example union type
typedef union { int i; long l; int a[5]; } MyUnion;

// One example of a more-complicated type
typedef struct { MyEnum e1, e2; MyPtrIncomplete p; MyStruct s; MyUnion u; } MyBlob;

#pragma rs reduce(my_half) accumulator(my_half_accum) combiner(my_half_comb) outconverter(my_half_out)
static void my_half_accum(half *accum, int val) { }
static void my_half_comb(half *accum, const half *other) { }
static void my_half_out(int *out, const half *accum) { }

#pragma rs reduce(my_half2) accumulator(my_half2_accum) combiner(my_half2_comb) outconverter(my_half2_out)
static void my_half2_accum(half2 *accum, int val) { }
static void my_half2_comb(half2 *accum, const half2 *other) { }
static void my_half2_out(int *out, const half2 *accum) { }

#pragma rs reduce(my_half4) accumulator(my_half4_accum) combiner(my_half4_comb) outconverter(my_half4_out)
static void my_half4_accum(half4 *accum, int val) { }
static void my_half4_comb(half4 *accum, const half4 *other) { }
static void my_half4_out(int *out, const half4 *accum) { }

#pragma rs reduce(my_array_half) accumulator(my_array_half_accum) combiner(my_array_half_comb) outconverter(my_array_half_out)
typedef half array_half[7];
static void my_array_half_accum(array_half *accum, int val) { }
static void my_array_half_comb(array_half *accum, const array_half *other) { }
static void my_array_half_out(int *out, const array_half *accum) { }

#pragma rs reduce(my_array_half2) accumulator(my_array_half2_accum) combiner(my_array_half2_comb) outconverter(my_array_half2_out)
typedef half2 array_half2[7];
static void my_array_half2_accum(array_half2 *accum, int val) { }
static void my_array_half2_comb(array_half2 *accum, const array_half2 *other) { }
static void my_array_half2_out(int *out, const array_half2 *accum) { }

#pragma rs reduce(my_array_half4) accumulator(my_array_half4_accum) combiner(my_array_half4_comb) outconverter(my_array_half4_out)
typedef half4 array_half4[7];
static void my_array_half4_accum(array_half4 *accum, int val) { }
static void my_array_half4_comb(array_half4 *accum, const array_half4 *other) { }
static void my_array_half4_out(int *out, const array_half4 *accum) { }

#pragma rs reduce(my_float) accumulator(my_float_accum) combiner(my_float_comb) outconverter(my_float_out)
static void my_float_accum(float *accum, int val) { }
static void my_float_comb(float *accum, const float *other) { }
static void my_float_out(int *out, const float *accum) { }

#pragma rs reduce(my_float2) accumulator(my_float2_accum) combiner(my_float2_comb) outconverter(my_float2_out)
static void my_float2_accum(float2 *accum, int val) { }
static void my_float2_comb(float2 *accum, const float2 *other) { }
static void my_float2_out(int *out, const float2 *accum) { }

#pragma rs reduce(my_float4) accumulator(my_float4_accum) combiner(my_float4_comb) outconverter(my_float4_out)
static void my_float4_accum(float4 *accum, int val) { }
static void my_float4_comb(float4 *accum, const float4 *other) { }
static void my_float4_out(int *out, const float4 *accum) { }

#pragma rs reduce(my_array_float) accumulator(my_array_float_accum) combiner(my_array_float_comb) outconverter(my_array_float_out)
typedef float array_float[7];
static void my_array_float_accum(array_float *accum, int val) { }
static void my_array_float_comb(array_float *accum, const array_float *other) { }
static void my_array_float_out(int *out, const array_float *accum) { }

#pragma rs reduce(my_array_float2) accumulator(my_array_float2_accum) combiner(my_array_float2_comb) outconverter(my_array_float2_out)
typedef float2 array_float2[7];
static void my_array_float2_accum(array_float2 *accum, int val) { }
static void my_array_float2_comb(array_float2 *accum, const array_float2 *other) { }
static void my_array_float2_out(int *out, const array_float2 *accum) { }

#pragma rs reduce(my_array_float4) accumulator(my_array_float4_accum) combiner(my_array_float4_comb) outconverter(my_array_float4_out)
typedef float4 array_float4[7];
static void my_array_float4_accum(array_float4 *accum, int val) { }
static void my_array_float4_comb(array_float4 *accum, const array_float4 *other) { }
static void my_array_float4_out(int *out, const array_float4 *accum) { }

#pragma rs reduce(my_double) accumulator(my_double_accum) combiner(my_double_comb) outconverter(my_double_out)
static void my_double_accum(double *accum, int val) { }
static void my_double_comb(double *accum, const double *other) { }
static void my_double_out(int *out, const double *accum) { }

#pragma rs reduce(my_double2) accumulator(my_double2_accum) combiner(my_double2_comb) outconverter(my_double2_out)
static void my_double2_accum(double2 *accum, int val) { }
static void my_double2_comb(double2 *accum, const double2 *other) { }
static void my_double2_out(int *out, const double2 *accum) { }

#pragma rs reduce(my_double4) accumulator(my_double4_accum) combiner(my_double4_comb) outconverter(my_double4_out)
static void my_double4_accum(double4 *accum, int val) { }
static void my_double4_comb(double4 *accum, const double4 *other) { }
static void my_double4_out(int *out, const double4 *accum) { }

#pragma rs reduce(my_array_double) accumulator(my_array_double_accum) combiner(my_array_double_comb) outconverter(my_array_double_out)
typedef double array_double[7];
static void my_array_double_accum(array_double *accum, int val) { }
static void my_array_double_comb(array_double *accum, const array_double *other) { }
static void my_array_double_out(int *out, const array_double *accum) { }

#pragma rs reduce(my_array_double2) accumulator(my_array_double2_accum) combiner(my_array_double2_comb) outconverter(my_array_double2_out)
typedef double2 array_double2[7];
static void my_array_double2_accum(array_double2 *accum, int val) { }
static void my_array_double2_comb(array_double2 *accum, const array_double2 *other) { }
static void my_array_double2_out(int *out, const array_double2 *accum) { }

#pragma rs reduce(my_array_double4) accumulator(my_array_double4_accum) combiner(my_array_double4_comb) outconverter(my_array_double4_out)
typedef double4 array_double4[7];
static void my_array_double4_accum(array_double4 *accum, int val) { }
static void my_array_double4_comb(array_double4 *accum, const array_double4 *other) { }
static void my_array_double4_out(int *out, const array_double4 *accum) { }

#pragma rs reduce(my_char) accumulator(my_char_accum) combiner(my_char_comb) outconverter(my_char_out)
static void my_char_accum(char *accum, int val) { }
static void my_char_comb(char *accum, const char *other) { }
static void my_char_out(int *out, const char *accum) { }

#pragma rs reduce(my_char2) accumulator(my_char2_accum) combiner(my_char2_comb) outconverter(my_char2_out)
static void my_char2_accum(char2 *accum, int val) { }
static void my_char2_comb(char2 *accum, const char2 *other) { }
static void my_char2_out(int *out, const char2 *accum) { }

#pragma rs reduce(my_char4) accumulator(my_char4_accum) combiner(my_char4_comb) outconverter(my_char4_out)
static void my_char4_accum(char4 *accum, int val) { }
static void my_char4_comb(char4 *accum, const char4 *other) { }
static void my_char4_out(int *out, const char4 *accum) { }

#pragma rs reduce(my_array_char) accumulator(my_array_char_accum) combiner(my_array_char_comb) outconverter(my_array_char_out)
typedef char array_char[7];
static void my_array_char_accum(array_char *accum, int val) { }
static void my_array_char_comb(array_char *accum, const array_char *other) { }
static void my_array_char_out(int *out, const array_char *accum) { }

#pragma rs reduce(my_array_char2) accumulator(my_array_char2_accum) combiner(my_array_char2_comb) outconverter(my_array_char2_out)
typedef char2 array_char2[7];
static void my_array_char2_accum(array_char2 *accum, int val) { }
static void my_array_char2_comb(array_char2 *accum, const array_char2 *other) { }
static void my_array_char2_out(int *out, const array_char2 *accum) { }

#pragma rs reduce(my_array_char4) accumulator(my_array_char4_accum) combiner(my_array_char4_comb) outconverter(my_array_char4_out)
typedef char4 array_char4[7];
static void my_array_char4_accum(array_char4 *accum, int val) { }
static void my_array_char4_comb(array_char4 *accum, const array_char4 *other) { }
static void my_array_char4_out(int *out, const array_char4 *accum) { }

#pragma rs reduce(my_short) accumulator(my_short_accum) combiner(my_short_comb) outconverter(my_short_out)
static void my_short_accum(short *accum, int val) { }
static void my_short_comb(short *accum, const short *other) { }
static void my_short_out(int *out, const short *accum) { }

#pragma rs reduce(my_short2) accumulator(my_short2_accum) combiner(my_short2_comb) outconverter(my_short2_out)
static void my_short2_accum(short2 *accum, int val) { }
static void my_short2_comb(short2 *accum, const short2 *other) { }
static void my_short2_out(int *out, const short2 *accum) { }

#pragma rs reduce(my_short4) accumulator(my_short4_accum) combiner(my_short4_comb) outconverter(my_short4_out)
static void my_short4_accum(short4 *accum, int val) { }
static void my_short4_comb(short4 *accum, const short4 *other) { }
static void my_short4_out(int *out, const short4 *accum) { }

#pragma rs reduce(my_array_short) accumulator(my_array_short_accum) combiner(my_array_short_comb) outconverter(my_array_short_out)
typedef short array_short[7];
static void my_array_short_accum(array_short *accum, int val) { }
static void my_array_short_comb(array_short *accum, const array_short *other) { }
static void my_array_short_out(int *out, const array_short *accum) { }

#pragma rs reduce(my_array_short2) accumulator(my_array_short2_accum) combiner(my_array_short2_comb) outconverter(my_array_short2_out)
typedef short2 array_short2[7];
static void my_array_short2_accum(array_short2 *accum, int val) { }
static void my_array_short2_comb(array_short2 *accum, const array_short2 *other) { }
static void my_array_short2_out(int *out, const array_short2 *accum) { }

#pragma rs reduce(my_array_short4) accumulator(my_array_short4_accum) combiner(my_array_short4_comb) outconverter(my_array_short4_out)
typedef short4 array_short4[7];
static void my_array_short4_accum(array_short4 *accum, int val) { }
static void my_array_short4_comb(array_short4 *accum, const array_short4 *other) { }
static void my_array_short4_out(int *out, const array_short4 *accum) { }

#pragma rs reduce(my_int) accumulator(my_int_accum) combiner(my_int_comb) outconverter(my_int_out)
static void my_int_accum(int *accum, int val) { }
static void my_int_comb(int *accum, const int *other) { }
static void my_int_out(int *out, const int *accum) { }

#pragma rs reduce(my_int2) accumulator(my_int2_accum) combiner(my_int2_comb) outconverter(my_int2_out)
static void my_int2_accum(int2 *accum, int val) { }
static void my_int2_comb(int2 *accum, const int2 *other) { }
static void my_int2_out(int *out, const int2 *accum) { }

#pragma rs reduce(my_int4) accumulator(my_int4_accum) combiner(my_int4_comb) outconverter(my_int4_out)
static void my_int4_accum(int4 *accum, int val) { }
static void my_int4_comb(int4 *accum, const int4 *other) { }
static void my_int4_out(int *out, const int4 *accum) { }

#pragma rs reduce(my_array_int) accumulator(my_array_int_accum) combiner(my_array_int_comb) outconverter(my_array_int_out)
typedef int array_int[7];
static void my_array_int_accum(array_int *accum, int val) { }
static void my_array_int_comb(array_int *accum, const array_int *other) { }
static void my_array_int_out(int *out, const array_int *accum) { }

#pragma rs reduce(my_array_int2) accumulator(my_array_int2_accum) combiner(my_array_int2_comb) outconverter(my_array_int2_out)
typedef int2 array_int2[7];
static void my_array_int2_accum(array_int2 *accum, int val) { }
static void my_array_int2_comb(array_int2 *accum, const array_int2 *other) { }
static void my_array_int2_out(int *out, const array_int2 *accum) { }

#pragma rs reduce(my_array_int4) accumulator(my_array_int4_accum) combiner(my_array_int4_comb) outconverter(my_array_int4_out)
typedef int4 array_int4[7];
static void my_array_int4_accum(array_int4 *accum, int val) { }
static void my_array_int4_comb(array_int4 *accum, const array_int4 *other) { }
static void my_array_int4_out(int *out, const array_int4 *accum) { }

#pragma rs reduce(my_long) accumulator(my_long_accum) combiner(my_long_comb) outconverter(my_long_out)
static void my_long_accum(long *accum, int val) { }
static void my_long_comb(long *accum, const long *other) { }
static void my_long_out(int *out, const long *accum) { }

#pragma rs reduce(my_long2) accumulator(my_long2_accum) combiner(my_long2_comb) outconverter(my_long2_out)
static void my_long2_accum(long2 *accum, int val) { }
static void my_long2_comb(long2 *accum, const long2 *other) { }
static void my_long2_out(int *out, const long2 *accum) { }

#pragma rs reduce(my_long4) accumulator(my_long4_accum) combiner(my_long4_comb) outconverter(my_long4_out)
static void my_long4_accum(long4 *accum, int val) { }
static void my_long4_comb(long4 *accum, const long4 *other) { }
static void my_long4_out(int *out, const long4 *accum) { }

#pragma rs reduce(my_array_long) accumulator(my_array_long_accum) combiner(my_array_long_comb) outconverter(my_array_long_out)
typedef long array_long[7];
static void my_array_long_accum(array_long *accum, int val) { }
static void my_array_long_comb(array_long *accum, const array_long *other) { }
static void my_array_long_out(int *out, const array_long *accum) { }

#pragma rs reduce(my_array_long2) accumulator(my_array_long2_accum) combiner(my_array_long2_comb) outconverter(my_array_long2_out)
typedef long2 array_long2[7];
static void my_array_long2_accum(array_long2 *accum, int val) { }
static void my_array_long2_comb(array_long2 *accum, const array_long2 *other) { }
static void my_array_long2_out(int *out, const array_long2 *accum) { }

#pragma rs reduce(my_array_long4) accumulator(my_array_long4_accum) combiner(my_array_long4_comb) outconverter(my_array_long4_out)
typedef long4 array_long4[7];
static void my_array_long4_accum(array_long4 *accum, int val) { }
static void my_array_long4_comb(array_long4 *accum, const array_long4 *other) { }
static void my_array_long4_out(int *out, const array_long4 *accum) { }

#pragma rs reduce(my_uchar) accumulator(my_uchar_accum) combiner(my_uchar_comb) outconverter(my_uchar_out)
static void my_uchar_accum(uchar *accum, int val) { }
static void my_uchar_comb(uchar *accum, const uchar *other) { }
static void my_uchar_out(int *out, const uchar *accum) { }

#pragma rs reduce(my_uchar2) accumulator(my_uchar2_accum) combiner(my_uchar2_comb) outconverter(my_uchar2_out)
static void my_uchar2_accum(uchar2 *accum, int val) { }
static void my_uchar2_comb(uchar2 *accum, const uchar2 *other) { }
static void my_uchar2_out(int *out, const uchar2 *accum) { }

#pragma rs reduce(my_uchar4) accumulator(my_uchar4_accum) combiner(my_uchar4_comb) outconverter(my_uchar4_out)
static void my_uchar4_accum(uchar4 *accum, int val) { }
static void my_uchar4_comb(uchar4 *accum, const uchar4 *other) { }
static void my_uchar4_out(int *out, const uchar4 *accum) { }

#pragma rs reduce(my_array_uchar) accumulator(my_array_uchar_accum) combiner(my_array_uchar_comb) outconverter(my_array_uchar_out)
typedef uchar array_uchar[7];
static void my_array_uchar_accum(array_uchar *accum, int val) { }
static void my_array_uchar_comb(array_uchar *accum, const array_uchar *other) { }
static void my_array_uchar_out(int *out, const array_uchar *accum) { }

#pragma rs reduce(my_array_uchar2) accumulator(my_array_uchar2_accum) combiner(my_array_uchar2_comb) outconverter(my_array_uchar2_out)
typedef uchar2 array_uchar2[7];
static void my_array_uchar2_accum(array_uchar2 *accum, int val) { }
static void my_array_uchar2_comb(array_uchar2 *accum, const array_uchar2 *other) { }
static void my_array_uchar2_out(int *out, const array_uchar2 *accum) { }

#pragma rs reduce(my_array_uchar4) accumulator(my_array_uchar4_accum) combiner(my_array_uchar4_comb) outconverter(my_array_uchar4_out)
typedef uchar4 array_uchar4[7];
static void my_array_uchar4_accum(array_uchar4 *accum, int val) { }
static void my_array_uchar4_comb(array_uchar4 *accum, const array_uchar4 *other) { }
static void my_array_uchar4_out(int *out, const array_uchar4 *accum) { }

#pragma rs reduce(my_ushort) accumulator(my_ushort_accum) combiner(my_ushort_comb) outconverter(my_ushort_out)
static void my_ushort_accum(ushort *accum, int val) { }
static void my_ushort_comb(ushort *accum, const ushort *other) { }
static void my_ushort_out(int *out, const ushort *accum) { }

#pragma rs reduce(my_ushort2) accumulator(my_ushort2_accum) combiner(my_ushort2_comb) outconverter(my_ushort2_out)
static void my_ushort2_accum(ushort2 *accum, int val) { }
static void my_ushort2_comb(ushort2 *accum, const ushort2 *other) { }
static void my_ushort2_out(int *out, const ushort2 *accum) { }

#pragma rs reduce(my_ushort4) accumulator(my_ushort4_accum) combiner(my_ushort4_comb) outconverter(my_ushort4_out)
static void my_ushort4_accum(ushort4 *accum, int val) { }
static void my_ushort4_comb(ushort4 *accum, const ushort4 *other) { }
static void my_ushort4_out(int *out, const ushort4 *accum) { }

#pragma rs reduce(my_array_ushort) accumulator(my_array_ushort_accum) combiner(my_array_ushort_comb) outconverter(my_array_ushort_out)
typedef ushort array_ushort[7];
static void my_array_ushort_accum(array_ushort *accum, int val) { }
static void my_array_ushort_comb(array_ushort *accum, const array_ushort *other) { }
static void my_array_ushort_out(int *out, const array_ushort *accum) { }

#pragma rs reduce(my_array_ushort2) accumulator(my_array_ushort2_accum) combiner(my_array_ushort2_comb) outconverter(my_array_ushort2_out)
typedef ushort2 array_ushort2[7];
static void my_array_ushort2_accum(array_ushort2 *accum, int val) { }
static void my_array_ushort2_comb(array_ushort2 *accum, const array_ushort2 *other) { }
static void my_array_ushort2_out(int *out, const array_ushort2 *accum) { }

#pragma rs reduce(my_array_ushort4) accumulator(my_array_ushort4_accum) combiner(my_array_ushort4_comb) outconverter(my_array_ushort4_out)
typedef ushort4 array_ushort4[7];
static void my_array_ushort4_accum(array_ushort4 *accum, int val) { }
static void my_array_ushort4_comb(array_ushort4 *accum, const array_ushort4 *other) { }
static void my_array_ushort4_out(int *out, const array_ushort4 *accum) { }

#pragma rs reduce(my_uint) accumulator(my_uint_accum) combiner(my_uint_comb) outconverter(my_uint_out)
static void my_uint_accum(uint *accum, int val) { }
static void my_uint_comb(uint *accum, const uint *other) { }
static void my_uint_out(int *out, const uint *accum) { }

#pragma rs reduce(my_uint2) accumulator(my_uint2_accum) combiner(my_uint2_comb) outconverter(my_uint2_out)
static void my_uint2_accum(uint2 *accum, int val) { }
static void my_uint2_comb(uint2 *accum, const uint2 *other) { }
static void my_uint2_out(int *out, const uint2 *accum) { }

#pragma rs reduce(my_uint4) accumulator(my_uint4_accum) combiner(my_uint4_comb) outconverter(my_uint4_out)
static void my_uint4_accum(uint4 *accum, int val) { }
static void my_uint4_comb(uint4 *accum, const uint4 *other) { }
static void my_uint4_out(int *out, const uint4 *accum) { }

#pragma rs reduce(my_array_uint) accumulator(my_array_uint_accum) combiner(my_array_uint_comb) outconverter(my_array_uint_out)
typedef uint array_uint[7];
static void my_array_uint_accum(array_uint *accum, int val) { }
static void my_array_uint_comb(array_uint *accum, const array_uint *other) { }
static void my_array_uint_out(int *out, const array_uint *accum) { }

#pragma rs reduce(my_array_uint2) accumulator(my_array_uint2_accum) combiner(my_array_uint2_comb) outconverter(my_array_uint2_out)
typedef uint2 array_uint2[7];
static void my_array_uint2_accum(array_uint2 *accum, int val) { }
static void my_array_uint2_comb(array_uint2 *accum, const array_uint2 *other) { }
static void my_array_uint2_out(int *out, const array_uint2 *accum) { }

#pragma rs reduce(my_array_uint4) accumulator(my_array_uint4_accum) combiner(my_array_uint4_comb) outconverter(my_array_uint4_out)
typedef uint4 array_uint4[7];
static void my_array_uint4_accum(array_uint4 *accum, int val) { }
static void my_array_uint4_comb(array_uint4 *accum, const array_uint4 *other) { }
static void my_array_uint4_out(int *out, const array_uint4 *accum) { }

#pragma rs reduce(my_ulong) accumulator(my_ulong_accum) combiner(my_ulong_comb) outconverter(my_ulong_out)
static void my_ulong_accum(ulong *accum, int val) { }
static void my_ulong_comb(ulong *accum, const ulong *other) { }
static void my_ulong_out(int *out, const ulong *accum) { }

#pragma rs reduce(my_ulong2) accumulator(my_ulong2_accum) combiner(my_ulong2_comb) outconverter(my_ulong2_out)
static void my_ulong2_accum(ulong2 *accum, int val) { }
static void my_ulong2_comb(ulong2 *accum, const ulong2 *other) { }
static void my_ulong2_out(int *out, const ulong2 *accum) { }

#pragma rs reduce(my_ulong4) accumulator(my_ulong4_accum) combiner(my_ulong4_comb) outconverter(my_ulong4_out)
static void my_ulong4_accum(ulong4 *accum, int val) { }
static void my_ulong4_comb(ulong4 *accum, const ulong4 *other) { }
static void my_ulong4_out(int *out, const ulong4 *accum) { }

#pragma rs reduce(my_array_ulong) accumulator(my_array_ulong_accum) combiner(my_array_ulong_comb) outconverter(my_array_ulong_out)
typedef ulong array_ulong[7];
static void my_array_ulong_accum(array_ulong *accum, int val) { }
static void my_array_ulong_comb(array_ulong *accum, const array_ulong *other) { }
static void my_array_ulong_out(int *out, const array_ulong *accum) { }

#pragma rs reduce(my_array_ulong2) accumulator(my_array_ulong2_accum) combiner(my_array_ulong2_comb) outconverter(my_array_ulong2_out)
typedef ulong2 array_ulong2[7];
static void my_array_ulong2_accum(array_ulong2 *accum, int val) { }
static void my_array_ulong2_comb(array_ulong2 *accum, const array_ulong2 *other) { }
static void my_array_ulong2_out(int *out, const array_ulong2 *accum) { }

#pragma rs reduce(my_array_ulong4) accumulator(my_array_ulong4_accum) combiner(my_array_ulong4_comb) outconverter(my_array_ulong4_out)
typedef ulong4 array_ulong4[7];
static void my_array_ulong4_accum(array_ulong4 *accum, int val) { }
static void my_array_ulong4_comb(array_ulong4 *accum, const array_ulong4 *other) { }
static void my_array_ulong4_out(int *out, const array_ulong4 *accum) { }

#pragma rs reduce(my_bool) accumulator(my_bool_accum) combiner(my_bool_comb) outconverter(my_bool_out)
static void my_bool_accum(bool *accum, int val) { }
static void my_bool_comb(bool *accum, const bool *other) { }
static void my_bool_out(int *out, const bool *accum) { }

#pragma rs reduce(my_array_bool) accumulator(my_array_bool_accum) combiner(my_array_bool_comb) outconverter(my_array_bool_out)
typedef bool array_bool[7];
static void my_array_bool_accum(array_bool *accum, int val) { }
static void my_array_bool_comb(array_bool *accum, const array_bool *other) { }
static void my_array_bool_out(int *out, const array_bool *accum) { }

#pragma rs reduce(my_rs_matrix2x2) accumulator(my_rs_matrix2x2_accum) combiner(my_rs_matrix2x2_comb) outconverter(my_rs_matrix2x2_out)
static void my_rs_matrix2x2_accum(rs_matrix2x2 *accum, int val) { }
static void my_rs_matrix2x2_comb(rs_matrix2x2 *accum, const rs_matrix2x2 *other) { }
static void my_rs_matrix2x2_out(int *out, const rs_matrix2x2 *accum) { }

#pragma rs reduce(my_array_rs_matrix2x2) accumulator(my_array_rs_matrix2x2_accum) combiner(my_array_rs_matrix2x2_comb) outconverter(my_array_rs_matrix2x2_out)
typedef rs_matrix2x2 array_rs_matrix2x2[7];
static void my_array_rs_matrix2x2_accum(array_rs_matrix2x2 *accum, int val) { }
static void my_array_rs_matrix2x2_comb(array_rs_matrix2x2 *accum, const array_rs_matrix2x2 *other) { }
static void my_array_rs_matrix2x2_out(int *out, const array_rs_matrix2x2 *accum) { }

#pragma rs reduce(my_rs_matrix3x3) accumulator(my_rs_matrix3x3_accum) combiner(my_rs_matrix3x3_comb) outconverter(my_rs_matrix3x3_out)
static void my_rs_matrix3x3_accum(rs_matrix3x3 *accum, int val) { }
static void my_rs_matrix3x3_comb(rs_matrix3x3 *accum, const rs_matrix3x3 *other) { }
static void my_rs_matrix3x3_out(int *out, const rs_matrix3x3 *accum) { }

#pragma rs reduce(my_array_rs_matrix3x3) accumulator(my_array_rs_matrix3x3_accum) combiner(my_array_rs_matrix3x3_comb) outconverter(my_array_rs_matrix3x3_out)
typedef rs_matrix3x3 array_rs_matrix3x3[7];
static void my_array_rs_matrix3x3_accum(array_rs_matrix3x3 *accum, int val) { }
static void my_array_rs_matrix3x3_comb(array_rs_matrix3x3 *accum, const array_rs_matrix3x3 *other) { }
static void my_array_rs_matrix3x3_out(int *out, const array_rs_matrix3x3 *accum) { }

#pragma rs reduce(my_rs_matrix4x4) accumulator(my_rs_matrix4x4_accum) combiner(my_rs_matrix4x4_comb) outconverter(my_rs_matrix4x4_out)
static void my_rs_matrix4x4_accum(rs_matrix4x4 *accum, int val) { }
static void my_rs_matrix4x4_comb(rs_matrix4x4 *accum, const rs_matrix4x4 *other) { }
static void my_rs_matrix4x4_out(int *out, const rs_matrix4x4 *accum) { }

#pragma rs reduce(my_array_rs_matrix4x4) accumulator(my_array_rs_matrix4x4_accum) combiner(my_array_rs_matrix4x4_comb) outconverter(my_array_rs_matrix4x4_out)
typedef rs_matrix4x4 array_rs_matrix4x4[7];
static void my_array_rs_matrix4x4_accum(array_rs_matrix4x4 *accum, int val) { }
static void my_array_rs_matrix4x4_comb(array_rs_matrix4x4 *accum, const array_rs_matrix4x4 *other) { }
static void my_array_rs_matrix4x4_out(int *out, const array_rs_matrix4x4 *accum) { }

#pragma rs reduce(my_rs_for_each_strategy_t) accumulator(my_rs_for_each_strategy_t_accum) combiner(my_rs_for_each_strategy_t_comb) outconverter(my_rs_for_each_strategy_t_out)
static void my_rs_for_each_strategy_t_accum(rs_for_each_strategy_t *accum, int val) { }
static void my_rs_for_each_strategy_t_comb(rs_for_each_strategy_t *accum, const rs_for_each_strategy_t *other) { }
static void my_rs_for_each_strategy_t_out(int *out, const rs_for_each_strategy_t *accum) { }

#pragma rs reduce(my_array_rs_for_each_strategy_t) accumulator(my_array_rs_for_each_strategy_t_accum) combiner(my_array_rs_for_each_strategy_t_comb) outconverter(my_array_rs_for_each_strategy_t_out)
typedef rs_for_each_strategy_t array_rs_for_each_strategy_t[7];
static void my_array_rs_for_each_strategy_t_accum(array_rs_for_each_strategy_t *accum, int val) { }
static void my_array_rs_for_each_strategy_t_comb(array_rs_for_each_strategy_t *accum, const array_rs_for_each_strategy_t *other) { }
static void my_array_rs_for_each_strategy_t_out(int *out, const array_rs_for_each_strategy_t *accum) { }

#pragma rs reduce(my_rs_script_call_t) accumulator(my_rs_script_call_t_accum) combiner(my_rs_script_call_t_comb) outconverter(my_rs_script_call_t_out)
static void my_rs_script_call_t_accum(rs_script_call_t *accum, int val) { }
static void my_rs_script_call_t_comb(rs_script_call_t *accum, const rs_script_call_t *other) { }
static void my_rs_script_call_t_out(int *out, const rs_script_call_t *accum) { }

#pragma rs reduce(my_array_rs_script_call_t) accumulator(my_array_rs_script_call_t_accum) combiner(my_array_rs_script_call_t_comb) outconverter(my_array_rs_script_call_t_out)
typedef rs_script_call_t array_rs_script_call_t[7];
static void my_array_rs_script_call_t_accum(array_rs_script_call_t *accum, int val) { }
static void my_array_rs_script_call_t_comb(array_rs_script_call_t *accum, const array_rs_script_call_t *other) { }
static void my_array_rs_script_call_t_out(int *out, const array_rs_script_call_t *accum) { }

#pragma rs reduce(my_rs_time_t) accumulator(my_rs_time_t_accum) combiner(my_rs_time_t_comb) outconverter(my_rs_time_t_out)
static void my_rs_time_t_accum(rs_time_t *accum, int val) { }
static void my_rs_time_t_comb(rs_time_t *accum, const rs_time_t *other) { }
static void my_rs_time_t_out(int *out, const rs_time_t *accum) { }

#pragma rs reduce(my_array_rs_time_t) accumulator(my_array_rs_time_t_accum) combiner(my_array_rs_time_t_comb) outconverter(my_array_rs_time_t_out)
typedef rs_time_t array_rs_time_t[7];
static void my_array_rs_time_t_accum(array_rs_time_t *accum, int val) { }
static void my_array_rs_time_t_comb(array_rs_time_t *accum, const array_rs_time_t *other) { }
static void my_array_rs_time_t_out(int *out, const array_rs_time_t *accum) { }

#pragma rs reduce(my_rs_tm) accumulator(my_rs_tm_accum) combiner(my_rs_tm_comb) outconverter(my_rs_tm_out)
static void my_rs_tm_accum(rs_tm *accum, int val) { }
static void my_rs_tm_comb(rs_tm *accum, const rs_tm *other) { }
static void my_rs_tm_out(int *out, const rs_tm *accum) { }

#pragma rs reduce(my_array_rs_tm) accumulator(my_array_rs_tm_accum) combiner(my_array_rs_tm_comb) outconverter(my_array_rs_tm_out)
typedef rs_tm array_rs_tm[7];
static void my_array_rs_tm_accum(array_rs_tm *accum, int val) { }
static void my_array_rs_tm_comb(array_rs_tm *accum, const array_rs_tm *other) { }
static void my_array_rs_tm_out(int *out, const array_rs_tm *accum) { }

#pragma rs reduce(my_MyEnum) accumulator(my_MyEnum_accum) combiner(my_MyEnum_comb) outconverter(my_MyEnum_out)
static void my_MyEnum_accum(MyEnum *accum, int val) { }
static void my_MyEnum_comb(MyEnum *accum, const MyEnum *other) { }
static void my_MyEnum_out(int *out, const MyEnum *accum) { }

#pragma rs reduce(my_array_MyEnum) accumulator(my_array_MyEnum_accum) combiner(my_array_MyEnum_comb) outconverter(my_array_MyEnum_out)
typedef MyEnum array_MyEnum[7];
static void my_array_MyEnum_accum(array_MyEnum *accum, int val) { }
static void my_array_MyEnum_comb(array_MyEnum *accum, const array_MyEnum *other) { }
static void my_array_MyEnum_out(int *out, const array_MyEnum *accum) { }

#pragma rs reduce(my_MyPtrArray) accumulator(my_MyPtrArray_accum) combiner(my_MyPtrArray_comb) outconverter(my_MyPtrArray_out)
static void my_MyPtrArray_accum(MyPtrArray *accum, int val) { }
static void my_MyPtrArray_comb(MyPtrArray *accum, const MyPtrArray *other) { }
static void my_MyPtrArray_out(int *out, const MyPtrArray *accum) { }

#pragma rs reduce(my_array_MyPtrArray) accumulator(my_array_MyPtrArray_accum) combiner(my_array_MyPtrArray_comb) outconverter(my_array_MyPtrArray_out)
typedef MyPtrArray array_MyPtrArray[7];
static void my_array_MyPtrArray_accum(array_MyPtrArray *accum, int val) { }
static void my_array_MyPtrArray_comb(array_MyPtrArray *accum, const array_MyPtrArray *other) { }
static void my_array_MyPtrArray_out(int *out, const array_MyPtrArray *accum) { }

#pragma rs reduce(my_MyPtrFn) accumulator(my_MyPtrFn_accum) combiner(my_MyPtrFn_comb) outconverter(my_MyPtrFn_out)
static void my_MyPtrFn_accum(MyPtrFn *accum, int val) { }
static void my_MyPtrFn_comb(MyPtrFn *accum, const MyPtrFn *other) { }
static void my_MyPtrFn_out(int *out, const MyPtrFn *accum) { }

#pragma rs reduce(my_array_MyPtrFn) accumulator(my_array_MyPtrFn_accum) combiner(my_array_MyPtrFn_comb) outconverter(my_array_MyPtrFn_out)
typedef MyPtrFn array_MyPtrFn[7];
static void my_array_MyPtrFn_accum(array_MyPtrFn *accum, int val) { }
static void my_array_MyPtrFn_comb(array_MyPtrFn *accum, const array_MyPtrFn *other) { }
static void my_array_MyPtrFn_out(int *out, const array_MyPtrFn *accum) { }

#pragma rs reduce(my_MyPtrIncomplete) accumulator(my_MyPtrIncomplete_accum) combiner(my_MyPtrIncomplete_comb) outconverter(my_MyPtrIncomplete_out)
static void my_MyPtrIncomplete_accum(MyPtrIncomplete *accum, int val) { }
static void my_MyPtrIncomplete_comb(MyPtrIncomplete *accum, const MyPtrIncomplete *other) { }
static void my_MyPtrIncomplete_out(int *out, const MyPtrIncomplete *accum) { }

#pragma rs reduce(my_array_MyPtrIncomplete) accumulator(my_array_MyPtrIncomplete_accum) combiner(my_array_MyPtrIncomplete_comb) outconverter(my_array_MyPtrIncomplete_out)
typedef MyPtrIncomplete array_MyPtrIncomplete[7];
static void my_array_MyPtrIncomplete_accum(array_MyPtrIncomplete *accum, int val) { }
static void my_array_MyPtrIncomplete_comb(array_MyPtrIncomplete *accum, const array_MyPtrIncomplete *other) { }
static void my_array_MyPtrIncomplete_out(int *out, const array_MyPtrIncomplete *accum) { }

#pragma rs reduce(my_MyPtrShort) accumulator(my_MyPtrShort_accum) combiner(my_MyPtrShort_comb) outconverter(my_MyPtrShort_out)
static void my_MyPtrShort_accum(MyPtrShort *accum, int val) { }
static void my_MyPtrShort_comb(MyPtrShort *accum, const MyPtrShort *other) { }
static void my_MyPtrShort_out(int *out, const MyPtrShort *accum) { }

#pragma rs reduce(my_array_MyPtrShort) accumulator(my_array_MyPtrShort_accum) combiner(my_array_MyPtrShort_comb) outconverter(my_array_MyPtrShort_out)
typedef MyPtrShort array_MyPtrShort[7];
static void my_array_MyPtrShort_accum(array_MyPtrShort *accum, int val) { }
static void my_array_MyPtrShort_comb(array_MyPtrShort *accum, const array_MyPtrShort *other) { }
static void my_array_MyPtrShort_out(int *out, const array_MyPtrShort *accum) { }

#pragma rs reduce(my_MyPtrVoid) accumulator(my_MyPtrVoid_accum) combiner(my_MyPtrVoid_comb) outconverter(my_MyPtrVoid_out)
static void my_MyPtrVoid_accum(MyPtrVoid *accum, int val) { }
static void my_MyPtrVoid_comb(MyPtrVoid *accum, const MyPtrVoid *other) { }
static void my_MyPtrVoid_out(int *out, const MyPtrVoid *accum) { }

#pragma rs reduce(my_array_MyPtrVoid) accumulator(my_array_MyPtrVoid_accum) combiner(my_array_MyPtrVoid_comb) outconverter(my_array_MyPtrVoid_out)
typedef MyPtrVoid array_MyPtrVoid[7];
static void my_array_MyPtrVoid_accum(array_MyPtrVoid *accum, int val) { }
static void my_array_MyPtrVoid_comb(array_MyPtrVoid *accum, const array_MyPtrVoid *other) { }
static void my_array_MyPtrVoid_out(int *out, const array_MyPtrVoid *accum) { }

#pragma rs reduce(my_MyStruct) accumulator(my_MyStruct_accum) combiner(my_MyStruct_comb) outconverter(my_MyStruct_out)
static void my_MyStruct_accum(MyStruct *accum, int val) { }
static void my_MyStruct_comb(MyStruct *accum, const MyStruct *other) { }
static void my_MyStruct_out(int *out, const MyStruct *accum) { }

#pragma rs reduce(my_array_MyStruct) accumulator(my_array_MyStruct_accum) combiner(my_array_MyStruct_comb) outconverter(my_array_MyStruct_out)
typedef MyStruct array_MyStruct[7];
static void my_array_MyStruct_accum(array_MyStruct *accum, int val) { }
static void my_array_MyStruct_comb(array_MyStruct *accum, const array_MyStruct *other) { }
static void my_array_MyStruct_out(int *out, const array_MyStruct *accum) { }

#pragma rs reduce(my_MyUnion) accumulator(my_MyUnion_accum) combiner(my_MyUnion_comb) outconverter(my_MyUnion_out)
static void my_MyUnion_accum(MyUnion *accum, int val) { }
static void my_MyUnion_comb(MyUnion *accum, const MyUnion *other) { }
static void my_MyUnion_out(int *out, const MyUnion *accum) { }

#pragma rs reduce(my_array_MyUnion) accumulator(my_array_MyUnion_accum) combiner(my_array_MyUnion_comb) outconverter(my_array_MyUnion_out)
typedef MyUnion array_MyUnion[7];
static void my_array_MyUnion_accum(array_MyUnion *accum, int val) { }
static void my_array_MyUnion_comb(array_MyUnion *accum, const array_MyUnion *other) { }
static void my_array_MyUnion_out(int *out, const array_MyUnion *accum) { }

#pragma rs reduce(my_MyBlob) accumulator(my_MyBlob_accum) combiner(my_MyBlob_comb) outconverter(my_MyBlob_out)
static void my_MyBlob_accum(MyBlob *accum, int val) { }
static void my_MyBlob_comb(MyBlob *accum, const MyBlob *other) { }
static void my_MyBlob_out(int *out, const MyBlob *accum) { }

#pragma rs reduce(my_array_MyBlob) accumulator(my_array_MyBlob_accum) combiner(my_array_MyBlob_comb) outconverter(my_array_MyBlob_out)
typedef MyBlob array_MyBlob[7];
static void my_array_MyBlob_accum(array_MyBlob *accum, int val) { }
static void my_array_MyBlob_comb(array_MyBlob *accum, const array_MyBlob *other) { }
static void my_array_MyBlob_out(int *out, const array_MyBlob *accum) { }
