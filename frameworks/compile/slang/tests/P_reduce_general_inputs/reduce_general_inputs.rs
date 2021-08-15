// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(inputs)

// This test case was created by ./gen-inputs.pl.
// It exercises various legal combinations of inputs and special parameters,
// so that we can ensure
// (a) We do not choke when compiling them
// (b) We reflect them correctly

// One example struct type
typedef struct MyStruct { float f; double d; } MyStruct;

// Trivial combiner shared by all test cases
static void combiner(int *accum, const int *other) { }

#pragma rs reduce(my_half_half_0) accumulator(my_half_half_0_accum) combiner(combiner)
static void my_half_half_0_accum(int *accum, half a, half b) { }

#pragma rs reduce(my_half_half_1) accumulator(my_half_half_1_accum) combiner(combiner)
static void my_half_half_1_accum(int *accum, half a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_half_half2_0) accumulator(my_half_half2_0_accum) combiner(combiner)
static void my_half_half2_0_accum(int *accum, half a, half2 b) { }

#pragma rs reduce(my_half_half2_1) accumulator(my_half_half2_1_accum) combiner(combiner)
static void my_half_half2_1_accum(int *accum, half a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_half4_0) accumulator(my_half_half4_0_accum) combiner(combiner)
static void my_half_half4_0_accum(int *accum, half a, half4 b) { }

#pragma rs reduce(my_half_half4_1) accumulator(my_half_half4_1_accum) combiner(combiner)
static void my_half_half4_1_accum(int *accum, half a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_float_0) accumulator(my_half_float_0_accum) combiner(combiner)
static void my_half_float_0_accum(int *accum, half a, float b) { }

#pragma rs reduce(my_half_float_1) accumulator(my_half_float_1_accum) combiner(combiner)
static void my_half_float_1_accum(int *accum, half a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_half_float2_0) accumulator(my_half_float2_0_accum) combiner(combiner)
static void my_half_float2_0_accum(int *accum, half a, float2 b) { }

#pragma rs reduce(my_half_float2_1) accumulator(my_half_float2_1_accum) combiner(combiner)
static void my_half_float2_1_accum(int *accum, half a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_float4_0) accumulator(my_half_float4_0_accum) combiner(combiner)
static void my_half_float4_0_accum(int *accum, half a, float4 b) { }

#pragma rs reduce(my_half_float4_1) accumulator(my_half_float4_1_accum) combiner(combiner)
static void my_half_float4_1_accum(int *accum, half a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_char_0) accumulator(my_half_char_0_accum) combiner(combiner)
static void my_half_char_0_accum(int *accum, half a, char b) { }

#pragma rs reduce(my_half_char_1) accumulator(my_half_char_1_accum) combiner(combiner)
static void my_half_char_1_accum(int *accum, half a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_half_char2_0) accumulator(my_half_char2_0_accum) combiner(combiner)
static void my_half_char2_0_accum(int *accum, half a, char2 b) { }

#pragma rs reduce(my_half_char2_1) accumulator(my_half_char2_1_accum) combiner(combiner)
static void my_half_char2_1_accum(int *accum, half a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_char4_0) accumulator(my_half_char4_0_accum) combiner(combiner)
static void my_half_char4_0_accum(int *accum, half a, char4 b) { }

#pragma rs reduce(my_half_char4_1) accumulator(my_half_char4_1_accum) combiner(combiner)
static void my_half_char4_1_accum(int *accum, half a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_short_0) accumulator(my_half_short_0_accum) combiner(combiner)
static void my_half_short_0_accum(int *accum, half a, short b) { }

#pragma rs reduce(my_half_short_1) accumulator(my_half_short_1_accum) combiner(combiner)
static void my_half_short_1_accum(int *accum, half a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_half_short2_0) accumulator(my_half_short2_0_accum) combiner(combiner)
static void my_half_short2_0_accum(int *accum, half a, short2 b) { }

#pragma rs reduce(my_half_short2_1) accumulator(my_half_short2_1_accum) combiner(combiner)
static void my_half_short2_1_accum(int *accum, half a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_short4_0) accumulator(my_half_short4_0_accum) combiner(combiner)
static void my_half_short4_0_accum(int *accum, half a, short4 b) { }

#pragma rs reduce(my_half_short4_1) accumulator(my_half_short4_1_accum) combiner(combiner)
static void my_half_short4_1_accum(int *accum, half a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_uchar_0) accumulator(my_half_uchar_0_accum) combiner(combiner)
static void my_half_uchar_0_accum(int *accum, half a, uchar b) { }

#pragma rs reduce(my_half_uchar_1) accumulator(my_half_uchar_1_accum) combiner(combiner)
static void my_half_uchar_1_accum(int *accum, half a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_half_uchar2_0) accumulator(my_half_uchar2_0_accum) combiner(combiner)
static void my_half_uchar2_0_accum(int *accum, half a, uchar2 b) { }

#pragma rs reduce(my_half_uchar2_1) accumulator(my_half_uchar2_1_accum) combiner(combiner)
static void my_half_uchar2_1_accum(int *accum, half a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_uchar4_0) accumulator(my_half_uchar4_0_accum) combiner(combiner)
static void my_half_uchar4_0_accum(int *accum, half a, uchar4 b) { }

#pragma rs reduce(my_half_uchar4_1) accumulator(my_half_uchar4_1_accum) combiner(combiner)
static void my_half_uchar4_1_accum(int *accum, half a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_ushort_0) accumulator(my_half_ushort_0_accum) combiner(combiner)
static void my_half_ushort_0_accum(int *accum, half a, ushort b) { }

#pragma rs reduce(my_half_ushort_1) accumulator(my_half_ushort_1_accum) combiner(combiner)
static void my_half_ushort_1_accum(int *accum, half a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_half_ushort2_0) accumulator(my_half_ushort2_0_accum) combiner(combiner)
static void my_half_ushort2_0_accum(int *accum, half a, ushort2 b) { }

#pragma rs reduce(my_half_ushort2_1) accumulator(my_half_ushort2_1_accum) combiner(combiner)
static void my_half_ushort2_1_accum(int *accum, half a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_ushort4_0) accumulator(my_half_ushort4_0_accum) combiner(combiner)
static void my_half_ushort4_0_accum(int *accum, half a, ushort4 b) { }

#pragma rs reduce(my_half_ushort4_1) accumulator(my_half_ushort4_1_accum) combiner(combiner)
static void my_half_ushort4_1_accum(int *accum, half a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_bool_0) accumulator(my_half_bool_0_accum) combiner(combiner)
static void my_half_bool_0_accum(int *accum, half a, bool b) { }

#pragma rs reduce(my_half_bool_1) accumulator(my_half_bool_1_accum) combiner(combiner)
static void my_half_bool_1_accum(int *accum, half a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_half_rs_matrix2x2_0) accumulator(my_half_rs_matrix2x2_0_accum) combiner(combiner)
static void my_half_rs_matrix2x2_0_accum(int *accum, half a, rs_matrix2x2 b) { }

#pragma rs reduce(my_half_rs_matrix2x2_1) accumulator(my_half_rs_matrix2x2_1_accum) combiner(combiner)
static void my_half_rs_matrix2x2_1_accum(int *accum, half a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half_MyStruct_0) accumulator(my_half_MyStruct_0_accum) combiner(combiner)
static void my_half_MyStruct_0_accum(int *accum, half a, MyStruct b) { }

#pragma rs reduce(my_half_MyStruct_1) accumulator(my_half_MyStruct_1_accum) combiner(combiner)
static void my_half_MyStruct_1_accum(int *accum, half a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_half_0) accumulator(my_half2_half_0_accum) combiner(combiner)
static void my_half2_half_0_accum(int *accum, half2 a, half b) { }

#pragma rs reduce(my_half2_half_1) accumulator(my_half2_half_1_accum) combiner(combiner)
static void my_half2_half_1_accum(int *accum, half2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_half2_0) accumulator(my_half2_half2_0_accum) combiner(combiner)
static void my_half2_half2_0_accum(int *accum, half2 a, half2 b) { }

#pragma rs reduce(my_half2_half2_1) accumulator(my_half2_half2_1_accum) combiner(combiner)
static void my_half2_half2_1_accum(int *accum, half2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_half4_0) accumulator(my_half2_half4_0_accum) combiner(combiner)
static void my_half2_half4_0_accum(int *accum, half2 a, half4 b) { }

#pragma rs reduce(my_half2_half4_1) accumulator(my_half2_half4_1_accum) combiner(combiner)
static void my_half2_half4_1_accum(int *accum, half2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_float_0) accumulator(my_half2_float_0_accum) combiner(combiner)
static void my_half2_float_0_accum(int *accum, half2 a, float b) { }

#pragma rs reduce(my_half2_float_1) accumulator(my_half2_float_1_accum) combiner(combiner)
static void my_half2_float_1_accum(int *accum, half2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_float2_0) accumulator(my_half2_float2_0_accum) combiner(combiner)
static void my_half2_float2_0_accum(int *accum, half2 a, float2 b) { }

#pragma rs reduce(my_half2_float2_1) accumulator(my_half2_float2_1_accum) combiner(combiner)
static void my_half2_float2_1_accum(int *accum, half2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_float4_0) accumulator(my_half2_float4_0_accum) combiner(combiner)
static void my_half2_float4_0_accum(int *accum, half2 a, float4 b) { }

#pragma rs reduce(my_half2_float4_1) accumulator(my_half2_float4_1_accum) combiner(combiner)
static void my_half2_float4_1_accum(int *accum, half2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_char_0) accumulator(my_half2_char_0_accum) combiner(combiner)
static void my_half2_char_0_accum(int *accum, half2 a, char b) { }

#pragma rs reduce(my_half2_char_1) accumulator(my_half2_char_1_accum) combiner(combiner)
static void my_half2_char_1_accum(int *accum, half2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_char2_0) accumulator(my_half2_char2_0_accum) combiner(combiner)
static void my_half2_char2_0_accum(int *accum, half2 a, char2 b) { }

#pragma rs reduce(my_half2_char2_1) accumulator(my_half2_char2_1_accum) combiner(combiner)
static void my_half2_char2_1_accum(int *accum, half2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_char4_0) accumulator(my_half2_char4_0_accum) combiner(combiner)
static void my_half2_char4_0_accum(int *accum, half2 a, char4 b) { }

#pragma rs reduce(my_half2_char4_1) accumulator(my_half2_char4_1_accum) combiner(combiner)
static void my_half2_char4_1_accum(int *accum, half2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_short_0) accumulator(my_half2_short_0_accum) combiner(combiner)
static void my_half2_short_0_accum(int *accum, half2 a, short b) { }

#pragma rs reduce(my_half2_short_1) accumulator(my_half2_short_1_accum) combiner(combiner)
static void my_half2_short_1_accum(int *accum, half2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_short2_0) accumulator(my_half2_short2_0_accum) combiner(combiner)
static void my_half2_short2_0_accum(int *accum, half2 a, short2 b) { }

#pragma rs reduce(my_half2_short2_1) accumulator(my_half2_short2_1_accum) combiner(combiner)
static void my_half2_short2_1_accum(int *accum, half2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_short4_0) accumulator(my_half2_short4_0_accum) combiner(combiner)
static void my_half2_short4_0_accum(int *accum, half2 a, short4 b) { }

#pragma rs reduce(my_half2_short4_1) accumulator(my_half2_short4_1_accum) combiner(combiner)
static void my_half2_short4_1_accum(int *accum, half2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_uchar_0) accumulator(my_half2_uchar_0_accum) combiner(combiner)
static void my_half2_uchar_0_accum(int *accum, half2 a, uchar b) { }

#pragma rs reduce(my_half2_uchar_1) accumulator(my_half2_uchar_1_accum) combiner(combiner)
static void my_half2_uchar_1_accum(int *accum, half2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_uchar2_0) accumulator(my_half2_uchar2_0_accum) combiner(combiner)
static void my_half2_uchar2_0_accum(int *accum, half2 a, uchar2 b) { }

#pragma rs reduce(my_half2_uchar2_1) accumulator(my_half2_uchar2_1_accum) combiner(combiner)
static void my_half2_uchar2_1_accum(int *accum, half2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_uchar4_0) accumulator(my_half2_uchar4_0_accum) combiner(combiner)
static void my_half2_uchar4_0_accum(int *accum, half2 a, uchar4 b) { }

#pragma rs reduce(my_half2_uchar4_1) accumulator(my_half2_uchar4_1_accum) combiner(combiner)
static void my_half2_uchar4_1_accum(int *accum, half2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_ushort_0) accumulator(my_half2_ushort_0_accum) combiner(combiner)
static void my_half2_ushort_0_accum(int *accum, half2 a, ushort b) { }

#pragma rs reduce(my_half2_ushort_1) accumulator(my_half2_ushort_1_accum) combiner(combiner)
static void my_half2_ushort_1_accum(int *accum, half2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_ushort2_0) accumulator(my_half2_ushort2_0_accum) combiner(combiner)
static void my_half2_ushort2_0_accum(int *accum, half2 a, ushort2 b) { }

#pragma rs reduce(my_half2_ushort2_1) accumulator(my_half2_ushort2_1_accum) combiner(combiner)
static void my_half2_ushort2_1_accum(int *accum, half2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_ushort4_0) accumulator(my_half2_ushort4_0_accum) combiner(combiner)
static void my_half2_ushort4_0_accum(int *accum, half2 a, ushort4 b) { }

#pragma rs reduce(my_half2_ushort4_1) accumulator(my_half2_ushort4_1_accum) combiner(combiner)
static void my_half2_ushort4_1_accum(int *accum, half2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_bool_0) accumulator(my_half2_bool_0_accum) combiner(combiner)
static void my_half2_bool_0_accum(int *accum, half2 a, bool b) { }

#pragma rs reduce(my_half2_bool_1) accumulator(my_half2_bool_1_accum) combiner(combiner)
static void my_half2_bool_1_accum(int *accum, half2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_rs_matrix2x2_0) accumulator(my_half2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_half2_rs_matrix2x2_0_accum(int *accum, half2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_half2_rs_matrix2x2_1) accumulator(my_half2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_half2_rs_matrix2x2_1_accum(int *accum, half2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half2_MyStruct_0) accumulator(my_half2_MyStruct_0_accum) combiner(combiner)
static void my_half2_MyStruct_0_accum(int *accum, half2 a, MyStruct b) { }

#pragma rs reduce(my_half2_MyStruct_1) accumulator(my_half2_MyStruct_1_accum) combiner(combiner)
static void my_half2_MyStruct_1_accum(int *accum, half2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_half_0) accumulator(my_half4_half_0_accum) combiner(combiner)
static void my_half4_half_0_accum(int *accum, half4 a, half b) { }

#pragma rs reduce(my_half4_half_1) accumulator(my_half4_half_1_accum) combiner(combiner)
static void my_half4_half_1_accum(int *accum, half4 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_half2_0) accumulator(my_half4_half2_0_accum) combiner(combiner)
static void my_half4_half2_0_accum(int *accum, half4 a, half2 b) { }

#pragma rs reduce(my_half4_half2_1) accumulator(my_half4_half2_1_accum) combiner(combiner)
static void my_half4_half2_1_accum(int *accum, half4 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_half4_0) accumulator(my_half4_half4_0_accum) combiner(combiner)
static void my_half4_half4_0_accum(int *accum, half4 a, half4 b) { }

#pragma rs reduce(my_half4_half4_1) accumulator(my_half4_half4_1_accum) combiner(combiner)
static void my_half4_half4_1_accum(int *accum, half4 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_float_0) accumulator(my_half4_float_0_accum) combiner(combiner)
static void my_half4_float_0_accum(int *accum, half4 a, float b) { }

#pragma rs reduce(my_half4_float_1) accumulator(my_half4_float_1_accum) combiner(combiner)
static void my_half4_float_1_accum(int *accum, half4 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_float2_0) accumulator(my_half4_float2_0_accum) combiner(combiner)
static void my_half4_float2_0_accum(int *accum, half4 a, float2 b) { }

#pragma rs reduce(my_half4_float2_1) accumulator(my_half4_float2_1_accum) combiner(combiner)
static void my_half4_float2_1_accum(int *accum, half4 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_float4_0) accumulator(my_half4_float4_0_accum) combiner(combiner)
static void my_half4_float4_0_accum(int *accum, half4 a, float4 b) { }

#pragma rs reduce(my_half4_float4_1) accumulator(my_half4_float4_1_accum) combiner(combiner)
static void my_half4_float4_1_accum(int *accum, half4 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_char_0) accumulator(my_half4_char_0_accum) combiner(combiner)
static void my_half4_char_0_accum(int *accum, half4 a, char b) { }

#pragma rs reduce(my_half4_char_1) accumulator(my_half4_char_1_accum) combiner(combiner)
static void my_half4_char_1_accum(int *accum, half4 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_char2_0) accumulator(my_half4_char2_0_accum) combiner(combiner)
static void my_half4_char2_0_accum(int *accum, half4 a, char2 b) { }

#pragma rs reduce(my_half4_char2_1) accumulator(my_half4_char2_1_accum) combiner(combiner)
static void my_half4_char2_1_accum(int *accum, half4 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_char4_0) accumulator(my_half4_char4_0_accum) combiner(combiner)
static void my_half4_char4_0_accum(int *accum, half4 a, char4 b) { }

#pragma rs reduce(my_half4_char4_1) accumulator(my_half4_char4_1_accum) combiner(combiner)
static void my_half4_char4_1_accum(int *accum, half4 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_short_0) accumulator(my_half4_short_0_accum) combiner(combiner)
static void my_half4_short_0_accum(int *accum, half4 a, short b) { }

#pragma rs reduce(my_half4_short_1) accumulator(my_half4_short_1_accum) combiner(combiner)
static void my_half4_short_1_accum(int *accum, half4 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_short2_0) accumulator(my_half4_short2_0_accum) combiner(combiner)
static void my_half4_short2_0_accum(int *accum, half4 a, short2 b) { }

#pragma rs reduce(my_half4_short2_1) accumulator(my_half4_short2_1_accum) combiner(combiner)
static void my_half4_short2_1_accum(int *accum, half4 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_short4_0) accumulator(my_half4_short4_0_accum) combiner(combiner)
static void my_half4_short4_0_accum(int *accum, half4 a, short4 b) { }

#pragma rs reduce(my_half4_short4_1) accumulator(my_half4_short4_1_accum) combiner(combiner)
static void my_half4_short4_1_accum(int *accum, half4 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_uchar_0) accumulator(my_half4_uchar_0_accum) combiner(combiner)
static void my_half4_uchar_0_accum(int *accum, half4 a, uchar b) { }

#pragma rs reduce(my_half4_uchar_1) accumulator(my_half4_uchar_1_accum) combiner(combiner)
static void my_half4_uchar_1_accum(int *accum, half4 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_uchar2_0) accumulator(my_half4_uchar2_0_accum) combiner(combiner)
static void my_half4_uchar2_0_accum(int *accum, half4 a, uchar2 b) { }

#pragma rs reduce(my_half4_uchar2_1) accumulator(my_half4_uchar2_1_accum) combiner(combiner)
static void my_half4_uchar2_1_accum(int *accum, half4 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_uchar4_0) accumulator(my_half4_uchar4_0_accum) combiner(combiner)
static void my_half4_uchar4_0_accum(int *accum, half4 a, uchar4 b) { }

#pragma rs reduce(my_half4_uchar4_1) accumulator(my_half4_uchar4_1_accum) combiner(combiner)
static void my_half4_uchar4_1_accum(int *accum, half4 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_ushort_0) accumulator(my_half4_ushort_0_accum) combiner(combiner)
static void my_half4_ushort_0_accum(int *accum, half4 a, ushort b) { }

#pragma rs reduce(my_half4_ushort_1) accumulator(my_half4_ushort_1_accum) combiner(combiner)
static void my_half4_ushort_1_accum(int *accum, half4 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_ushort2_0) accumulator(my_half4_ushort2_0_accum) combiner(combiner)
static void my_half4_ushort2_0_accum(int *accum, half4 a, ushort2 b) { }

#pragma rs reduce(my_half4_ushort2_1) accumulator(my_half4_ushort2_1_accum) combiner(combiner)
static void my_half4_ushort2_1_accum(int *accum, half4 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_ushort4_0) accumulator(my_half4_ushort4_0_accum) combiner(combiner)
static void my_half4_ushort4_0_accum(int *accum, half4 a, ushort4 b) { }

#pragma rs reduce(my_half4_ushort4_1) accumulator(my_half4_ushort4_1_accum) combiner(combiner)
static void my_half4_ushort4_1_accum(int *accum, half4 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_bool_0) accumulator(my_half4_bool_0_accum) combiner(combiner)
static void my_half4_bool_0_accum(int *accum, half4 a, bool b) { }

#pragma rs reduce(my_half4_bool_1) accumulator(my_half4_bool_1_accum) combiner(combiner)
static void my_half4_bool_1_accum(int *accum, half4 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_rs_matrix2x2_0) accumulator(my_half4_rs_matrix2x2_0_accum) combiner(combiner)
static void my_half4_rs_matrix2x2_0_accum(int *accum, half4 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_half4_rs_matrix2x2_1) accumulator(my_half4_rs_matrix2x2_1_accum) combiner(combiner)
static void my_half4_rs_matrix2x2_1_accum(int *accum, half4 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_half4_MyStruct_0) accumulator(my_half4_MyStruct_0_accum) combiner(combiner)
static void my_half4_MyStruct_0_accum(int *accum, half4 a, MyStruct b) { }

#pragma rs reduce(my_half4_MyStruct_1) accumulator(my_half4_MyStruct_1_accum) combiner(combiner)
static void my_half4_MyStruct_1_accum(int *accum, half4 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_float_half_0) accumulator(my_float_half_0_accum) combiner(combiner)
static void my_float_half_0_accum(int *accum, float a, half b) { }

#pragma rs reduce(my_float_half_1) accumulator(my_float_half_1_accum) combiner(combiner)
static void my_float_half_1_accum(int *accum, float a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_float_half2_0) accumulator(my_float_half2_0_accum) combiner(combiner)
static void my_float_half2_0_accum(int *accum, float a, half2 b) { }

#pragma rs reduce(my_float_half2_1) accumulator(my_float_half2_1_accum) combiner(combiner)
static void my_float_half2_1_accum(int *accum, float a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_half4_0) accumulator(my_float_half4_0_accum) combiner(combiner)
static void my_float_half4_0_accum(int *accum, float a, half4 b) { }

#pragma rs reduce(my_float_half4_1) accumulator(my_float_half4_1_accum) combiner(combiner)
static void my_float_half4_1_accum(int *accum, float a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_float_0) accumulator(my_float_float_0_accum) combiner(combiner)
static void my_float_float_0_accum(int *accum, float a, float b) { }

#pragma rs reduce(my_float_float_1) accumulator(my_float_float_1_accum) combiner(combiner)
static void my_float_float_1_accum(int *accum, float a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_float_float2_0) accumulator(my_float_float2_0_accum) combiner(combiner)
static void my_float_float2_0_accum(int *accum, float a, float2 b) { }

#pragma rs reduce(my_float_float2_1) accumulator(my_float_float2_1_accum) combiner(combiner)
static void my_float_float2_1_accum(int *accum, float a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_float4_0) accumulator(my_float_float4_0_accum) combiner(combiner)
static void my_float_float4_0_accum(int *accum, float a, float4 b) { }

#pragma rs reduce(my_float_float4_1) accumulator(my_float_float4_1_accum) combiner(combiner)
static void my_float_float4_1_accum(int *accum, float a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_char_0) accumulator(my_float_char_0_accum) combiner(combiner)
static void my_float_char_0_accum(int *accum, float a, char b) { }

#pragma rs reduce(my_float_char_1) accumulator(my_float_char_1_accum) combiner(combiner)
static void my_float_char_1_accum(int *accum, float a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_float_char2_0) accumulator(my_float_char2_0_accum) combiner(combiner)
static void my_float_char2_0_accum(int *accum, float a, char2 b) { }

#pragma rs reduce(my_float_char2_1) accumulator(my_float_char2_1_accum) combiner(combiner)
static void my_float_char2_1_accum(int *accum, float a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_char4_0) accumulator(my_float_char4_0_accum) combiner(combiner)
static void my_float_char4_0_accum(int *accum, float a, char4 b) { }

#pragma rs reduce(my_float_char4_1) accumulator(my_float_char4_1_accum) combiner(combiner)
static void my_float_char4_1_accum(int *accum, float a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_short_0) accumulator(my_float_short_0_accum) combiner(combiner)
static void my_float_short_0_accum(int *accum, float a, short b) { }

#pragma rs reduce(my_float_short_1) accumulator(my_float_short_1_accum) combiner(combiner)
static void my_float_short_1_accum(int *accum, float a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_float_short2_0) accumulator(my_float_short2_0_accum) combiner(combiner)
static void my_float_short2_0_accum(int *accum, float a, short2 b) { }

#pragma rs reduce(my_float_short2_1) accumulator(my_float_short2_1_accum) combiner(combiner)
static void my_float_short2_1_accum(int *accum, float a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_short4_0) accumulator(my_float_short4_0_accum) combiner(combiner)
static void my_float_short4_0_accum(int *accum, float a, short4 b) { }

#pragma rs reduce(my_float_short4_1) accumulator(my_float_short4_1_accum) combiner(combiner)
static void my_float_short4_1_accum(int *accum, float a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_uchar_0) accumulator(my_float_uchar_0_accum) combiner(combiner)
static void my_float_uchar_0_accum(int *accum, float a, uchar b) { }

#pragma rs reduce(my_float_uchar_1) accumulator(my_float_uchar_1_accum) combiner(combiner)
static void my_float_uchar_1_accum(int *accum, float a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_float_uchar2_0) accumulator(my_float_uchar2_0_accum) combiner(combiner)
static void my_float_uchar2_0_accum(int *accum, float a, uchar2 b) { }

#pragma rs reduce(my_float_uchar2_1) accumulator(my_float_uchar2_1_accum) combiner(combiner)
static void my_float_uchar2_1_accum(int *accum, float a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_uchar4_0) accumulator(my_float_uchar4_0_accum) combiner(combiner)
static void my_float_uchar4_0_accum(int *accum, float a, uchar4 b) { }

#pragma rs reduce(my_float_uchar4_1) accumulator(my_float_uchar4_1_accum) combiner(combiner)
static void my_float_uchar4_1_accum(int *accum, float a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_ushort_0) accumulator(my_float_ushort_0_accum) combiner(combiner)
static void my_float_ushort_0_accum(int *accum, float a, ushort b) { }

#pragma rs reduce(my_float_ushort_1) accumulator(my_float_ushort_1_accum) combiner(combiner)
static void my_float_ushort_1_accum(int *accum, float a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_float_ushort2_0) accumulator(my_float_ushort2_0_accum) combiner(combiner)
static void my_float_ushort2_0_accum(int *accum, float a, ushort2 b) { }

#pragma rs reduce(my_float_ushort2_1) accumulator(my_float_ushort2_1_accum) combiner(combiner)
static void my_float_ushort2_1_accum(int *accum, float a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_ushort4_0) accumulator(my_float_ushort4_0_accum) combiner(combiner)
static void my_float_ushort4_0_accum(int *accum, float a, ushort4 b) { }

#pragma rs reduce(my_float_ushort4_1) accumulator(my_float_ushort4_1_accum) combiner(combiner)
static void my_float_ushort4_1_accum(int *accum, float a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_bool_0) accumulator(my_float_bool_0_accum) combiner(combiner)
static void my_float_bool_0_accum(int *accum, float a, bool b) { }

#pragma rs reduce(my_float_bool_1) accumulator(my_float_bool_1_accum) combiner(combiner)
static void my_float_bool_1_accum(int *accum, float a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_float_rs_matrix2x2_0) accumulator(my_float_rs_matrix2x2_0_accum) combiner(combiner)
static void my_float_rs_matrix2x2_0_accum(int *accum, float a, rs_matrix2x2 b) { }

#pragma rs reduce(my_float_rs_matrix2x2_1) accumulator(my_float_rs_matrix2x2_1_accum) combiner(combiner)
static void my_float_rs_matrix2x2_1_accum(int *accum, float a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float_MyStruct_0) accumulator(my_float_MyStruct_0_accum) combiner(combiner)
static void my_float_MyStruct_0_accum(int *accum, float a, MyStruct b) { }

#pragma rs reduce(my_float_MyStruct_1) accumulator(my_float_MyStruct_1_accum) combiner(combiner)
static void my_float_MyStruct_1_accum(int *accum, float a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_half_0) accumulator(my_float2_half_0_accum) combiner(combiner)
static void my_float2_half_0_accum(int *accum, float2 a, half b) { }

#pragma rs reduce(my_float2_half_1) accumulator(my_float2_half_1_accum) combiner(combiner)
static void my_float2_half_1_accum(int *accum, float2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_half2_0) accumulator(my_float2_half2_0_accum) combiner(combiner)
static void my_float2_half2_0_accum(int *accum, float2 a, half2 b) { }

#pragma rs reduce(my_float2_half2_1) accumulator(my_float2_half2_1_accum) combiner(combiner)
static void my_float2_half2_1_accum(int *accum, float2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_half4_0) accumulator(my_float2_half4_0_accum) combiner(combiner)
static void my_float2_half4_0_accum(int *accum, float2 a, half4 b) { }

#pragma rs reduce(my_float2_half4_1) accumulator(my_float2_half4_1_accum) combiner(combiner)
static void my_float2_half4_1_accum(int *accum, float2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_float_0) accumulator(my_float2_float_0_accum) combiner(combiner)
static void my_float2_float_0_accum(int *accum, float2 a, float b) { }

#pragma rs reduce(my_float2_float_1) accumulator(my_float2_float_1_accum) combiner(combiner)
static void my_float2_float_1_accum(int *accum, float2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_float2_0) accumulator(my_float2_float2_0_accum) combiner(combiner)
static void my_float2_float2_0_accum(int *accum, float2 a, float2 b) { }

#pragma rs reduce(my_float2_float2_1) accumulator(my_float2_float2_1_accum) combiner(combiner)
static void my_float2_float2_1_accum(int *accum, float2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_float4_0) accumulator(my_float2_float4_0_accum) combiner(combiner)
static void my_float2_float4_0_accum(int *accum, float2 a, float4 b) { }

#pragma rs reduce(my_float2_float4_1) accumulator(my_float2_float4_1_accum) combiner(combiner)
static void my_float2_float4_1_accum(int *accum, float2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_char_0) accumulator(my_float2_char_0_accum) combiner(combiner)
static void my_float2_char_0_accum(int *accum, float2 a, char b) { }

#pragma rs reduce(my_float2_char_1) accumulator(my_float2_char_1_accum) combiner(combiner)
static void my_float2_char_1_accum(int *accum, float2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_char2_0) accumulator(my_float2_char2_0_accum) combiner(combiner)
static void my_float2_char2_0_accum(int *accum, float2 a, char2 b) { }

#pragma rs reduce(my_float2_char2_1) accumulator(my_float2_char2_1_accum) combiner(combiner)
static void my_float2_char2_1_accum(int *accum, float2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_char4_0) accumulator(my_float2_char4_0_accum) combiner(combiner)
static void my_float2_char4_0_accum(int *accum, float2 a, char4 b) { }

#pragma rs reduce(my_float2_char4_1) accumulator(my_float2_char4_1_accum) combiner(combiner)
static void my_float2_char4_1_accum(int *accum, float2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_short_0) accumulator(my_float2_short_0_accum) combiner(combiner)
static void my_float2_short_0_accum(int *accum, float2 a, short b) { }

#pragma rs reduce(my_float2_short_1) accumulator(my_float2_short_1_accum) combiner(combiner)
static void my_float2_short_1_accum(int *accum, float2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_short2_0) accumulator(my_float2_short2_0_accum) combiner(combiner)
static void my_float2_short2_0_accum(int *accum, float2 a, short2 b) { }

#pragma rs reduce(my_float2_short2_1) accumulator(my_float2_short2_1_accum) combiner(combiner)
static void my_float2_short2_1_accum(int *accum, float2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_short4_0) accumulator(my_float2_short4_0_accum) combiner(combiner)
static void my_float2_short4_0_accum(int *accum, float2 a, short4 b) { }

#pragma rs reduce(my_float2_short4_1) accumulator(my_float2_short4_1_accum) combiner(combiner)
static void my_float2_short4_1_accum(int *accum, float2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_uchar_0) accumulator(my_float2_uchar_0_accum) combiner(combiner)
static void my_float2_uchar_0_accum(int *accum, float2 a, uchar b) { }

#pragma rs reduce(my_float2_uchar_1) accumulator(my_float2_uchar_1_accum) combiner(combiner)
static void my_float2_uchar_1_accum(int *accum, float2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_uchar2_0) accumulator(my_float2_uchar2_0_accum) combiner(combiner)
static void my_float2_uchar2_0_accum(int *accum, float2 a, uchar2 b) { }

#pragma rs reduce(my_float2_uchar2_1) accumulator(my_float2_uchar2_1_accum) combiner(combiner)
static void my_float2_uchar2_1_accum(int *accum, float2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_uchar4_0) accumulator(my_float2_uchar4_0_accum) combiner(combiner)
static void my_float2_uchar4_0_accum(int *accum, float2 a, uchar4 b) { }

#pragma rs reduce(my_float2_uchar4_1) accumulator(my_float2_uchar4_1_accum) combiner(combiner)
static void my_float2_uchar4_1_accum(int *accum, float2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_ushort_0) accumulator(my_float2_ushort_0_accum) combiner(combiner)
static void my_float2_ushort_0_accum(int *accum, float2 a, ushort b) { }

#pragma rs reduce(my_float2_ushort_1) accumulator(my_float2_ushort_1_accum) combiner(combiner)
static void my_float2_ushort_1_accum(int *accum, float2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_ushort2_0) accumulator(my_float2_ushort2_0_accum) combiner(combiner)
static void my_float2_ushort2_0_accum(int *accum, float2 a, ushort2 b) { }

#pragma rs reduce(my_float2_ushort2_1) accumulator(my_float2_ushort2_1_accum) combiner(combiner)
static void my_float2_ushort2_1_accum(int *accum, float2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_ushort4_0) accumulator(my_float2_ushort4_0_accum) combiner(combiner)
static void my_float2_ushort4_0_accum(int *accum, float2 a, ushort4 b) { }

#pragma rs reduce(my_float2_ushort4_1) accumulator(my_float2_ushort4_1_accum) combiner(combiner)
static void my_float2_ushort4_1_accum(int *accum, float2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_bool_0) accumulator(my_float2_bool_0_accum) combiner(combiner)
static void my_float2_bool_0_accum(int *accum, float2 a, bool b) { }

#pragma rs reduce(my_float2_bool_1) accumulator(my_float2_bool_1_accum) combiner(combiner)
static void my_float2_bool_1_accum(int *accum, float2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_rs_matrix2x2_0) accumulator(my_float2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_float2_rs_matrix2x2_0_accum(int *accum, float2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_float2_rs_matrix2x2_1) accumulator(my_float2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_float2_rs_matrix2x2_1_accum(int *accum, float2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float2_MyStruct_0) accumulator(my_float2_MyStruct_0_accum) combiner(combiner)
static void my_float2_MyStruct_0_accum(int *accum, float2 a, MyStruct b) { }

#pragma rs reduce(my_float2_MyStruct_1) accumulator(my_float2_MyStruct_1_accum) combiner(combiner)
static void my_float2_MyStruct_1_accum(int *accum, float2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_half_0) accumulator(my_float4_half_0_accum) combiner(combiner)
static void my_float4_half_0_accum(int *accum, float4 a, half b) { }

#pragma rs reduce(my_float4_half_1) accumulator(my_float4_half_1_accum) combiner(combiner)
static void my_float4_half_1_accum(int *accum, float4 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_half2_0) accumulator(my_float4_half2_0_accum) combiner(combiner)
static void my_float4_half2_0_accum(int *accum, float4 a, half2 b) { }

#pragma rs reduce(my_float4_half2_1) accumulator(my_float4_half2_1_accum) combiner(combiner)
static void my_float4_half2_1_accum(int *accum, float4 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_half4_0) accumulator(my_float4_half4_0_accum) combiner(combiner)
static void my_float4_half4_0_accum(int *accum, float4 a, half4 b) { }

#pragma rs reduce(my_float4_half4_1) accumulator(my_float4_half4_1_accum) combiner(combiner)
static void my_float4_half4_1_accum(int *accum, float4 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_float_0) accumulator(my_float4_float_0_accum) combiner(combiner)
static void my_float4_float_0_accum(int *accum, float4 a, float b) { }

#pragma rs reduce(my_float4_float_1) accumulator(my_float4_float_1_accum) combiner(combiner)
static void my_float4_float_1_accum(int *accum, float4 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_float2_0) accumulator(my_float4_float2_0_accum) combiner(combiner)
static void my_float4_float2_0_accum(int *accum, float4 a, float2 b) { }

#pragma rs reduce(my_float4_float2_1) accumulator(my_float4_float2_1_accum) combiner(combiner)
static void my_float4_float2_1_accum(int *accum, float4 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_float4_0) accumulator(my_float4_float4_0_accum) combiner(combiner)
static void my_float4_float4_0_accum(int *accum, float4 a, float4 b) { }

#pragma rs reduce(my_float4_float4_1) accumulator(my_float4_float4_1_accum) combiner(combiner)
static void my_float4_float4_1_accum(int *accum, float4 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_char_0) accumulator(my_float4_char_0_accum) combiner(combiner)
static void my_float4_char_0_accum(int *accum, float4 a, char b) { }

#pragma rs reduce(my_float4_char_1) accumulator(my_float4_char_1_accum) combiner(combiner)
static void my_float4_char_1_accum(int *accum, float4 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_char2_0) accumulator(my_float4_char2_0_accum) combiner(combiner)
static void my_float4_char2_0_accum(int *accum, float4 a, char2 b) { }

#pragma rs reduce(my_float4_char2_1) accumulator(my_float4_char2_1_accum) combiner(combiner)
static void my_float4_char2_1_accum(int *accum, float4 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_char4_0) accumulator(my_float4_char4_0_accum) combiner(combiner)
static void my_float4_char4_0_accum(int *accum, float4 a, char4 b) { }

#pragma rs reduce(my_float4_char4_1) accumulator(my_float4_char4_1_accum) combiner(combiner)
static void my_float4_char4_1_accum(int *accum, float4 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_short_0) accumulator(my_float4_short_0_accum) combiner(combiner)
static void my_float4_short_0_accum(int *accum, float4 a, short b) { }

#pragma rs reduce(my_float4_short_1) accumulator(my_float4_short_1_accum) combiner(combiner)
static void my_float4_short_1_accum(int *accum, float4 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_short2_0) accumulator(my_float4_short2_0_accum) combiner(combiner)
static void my_float4_short2_0_accum(int *accum, float4 a, short2 b) { }

#pragma rs reduce(my_float4_short2_1) accumulator(my_float4_short2_1_accum) combiner(combiner)
static void my_float4_short2_1_accum(int *accum, float4 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_short4_0) accumulator(my_float4_short4_0_accum) combiner(combiner)
static void my_float4_short4_0_accum(int *accum, float4 a, short4 b) { }

#pragma rs reduce(my_float4_short4_1) accumulator(my_float4_short4_1_accum) combiner(combiner)
static void my_float4_short4_1_accum(int *accum, float4 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_uchar_0) accumulator(my_float4_uchar_0_accum) combiner(combiner)
static void my_float4_uchar_0_accum(int *accum, float4 a, uchar b) { }

#pragma rs reduce(my_float4_uchar_1) accumulator(my_float4_uchar_1_accum) combiner(combiner)
static void my_float4_uchar_1_accum(int *accum, float4 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_uchar2_0) accumulator(my_float4_uchar2_0_accum) combiner(combiner)
static void my_float4_uchar2_0_accum(int *accum, float4 a, uchar2 b) { }

#pragma rs reduce(my_float4_uchar2_1) accumulator(my_float4_uchar2_1_accum) combiner(combiner)
static void my_float4_uchar2_1_accum(int *accum, float4 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_uchar4_0) accumulator(my_float4_uchar4_0_accum) combiner(combiner)
static void my_float4_uchar4_0_accum(int *accum, float4 a, uchar4 b) { }

#pragma rs reduce(my_float4_uchar4_1) accumulator(my_float4_uchar4_1_accum) combiner(combiner)
static void my_float4_uchar4_1_accum(int *accum, float4 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_ushort_0) accumulator(my_float4_ushort_0_accum) combiner(combiner)
static void my_float4_ushort_0_accum(int *accum, float4 a, ushort b) { }

#pragma rs reduce(my_float4_ushort_1) accumulator(my_float4_ushort_1_accum) combiner(combiner)
static void my_float4_ushort_1_accum(int *accum, float4 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_ushort2_0) accumulator(my_float4_ushort2_0_accum) combiner(combiner)
static void my_float4_ushort2_0_accum(int *accum, float4 a, ushort2 b) { }

#pragma rs reduce(my_float4_ushort2_1) accumulator(my_float4_ushort2_1_accum) combiner(combiner)
static void my_float4_ushort2_1_accum(int *accum, float4 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_ushort4_0) accumulator(my_float4_ushort4_0_accum) combiner(combiner)
static void my_float4_ushort4_0_accum(int *accum, float4 a, ushort4 b) { }

#pragma rs reduce(my_float4_ushort4_1) accumulator(my_float4_ushort4_1_accum) combiner(combiner)
static void my_float4_ushort4_1_accum(int *accum, float4 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_bool_0) accumulator(my_float4_bool_0_accum) combiner(combiner)
static void my_float4_bool_0_accum(int *accum, float4 a, bool b) { }

#pragma rs reduce(my_float4_bool_1) accumulator(my_float4_bool_1_accum) combiner(combiner)
static void my_float4_bool_1_accum(int *accum, float4 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_rs_matrix2x2_0) accumulator(my_float4_rs_matrix2x2_0_accum) combiner(combiner)
static void my_float4_rs_matrix2x2_0_accum(int *accum, float4 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_float4_rs_matrix2x2_1) accumulator(my_float4_rs_matrix2x2_1_accum) combiner(combiner)
static void my_float4_rs_matrix2x2_1_accum(int *accum, float4 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_float4_MyStruct_0) accumulator(my_float4_MyStruct_0_accum) combiner(combiner)
static void my_float4_MyStruct_0_accum(int *accum, float4 a, MyStruct b) { }

#pragma rs reduce(my_float4_MyStruct_1) accumulator(my_float4_MyStruct_1_accum) combiner(combiner)
static void my_float4_MyStruct_1_accum(int *accum, float4 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_char_half_0) accumulator(my_char_half_0_accum) combiner(combiner)
static void my_char_half_0_accum(int *accum, char a, half b) { }

#pragma rs reduce(my_char_half_1) accumulator(my_char_half_1_accum) combiner(combiner)
static void my_char_half_1_accum(int *accum, char a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_char_half2_0) accumulator(my_char_half2_0_accum) combiner(combiner)
static void my_char_half2_0_accum(int *accum, char a, half2 b) { }

#pragma rs reduce(my_char_half2_1) accumulator(my_char_half2_1_accum) combiner(combiner)
static void my_char_half2_1_accum(int *accum, char a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_half4_0) accumulator(my_char_half4_0_accum) combiner(combiner)
static void my_char_half4_0_accum(int *accum, char a, half4 b) { }

#pragma rs reduce(my_char_half4_1) accumulator(my_char_half4_1_accum) combiner(combiner)
static void my_char_half4_1_accum(int *accum, char a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_float_0) accumulator(my_char_float_0_accum) combiner(combiner)
static void my_char_float_0_accum(int *accum, char a, float b) { }

#pragma rs reduce(my_char_float_1) accumulator(my_char_float_1_accum) combiner(combiner)
static void my_char_float_1_accum(int *accum, char a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_char_float2_0) accumulator(my_char_float2_0_accum) combiner(combiner)
static void my_char_float2_0_accum(int *accum, char a, float2 b) { }

#pragma rs reduce(my_char_float2_1) accumulator(my_char_float2_1_accum) combiner(combiner)
static void my_char_float2_1_accum(int *accum, char a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_float4_0) accumulator(my_char_float4_0_accum) combiner(combiner)
static void my_char_float4_0_accum(int *accum, char a, float4 b) { }

#pragma rs reduce(my_char_float4_1) accumulator(my_char_float4_1_accum) combiner(combiner)
static void my_char_float4_1_accum(int *accum, char a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_char_0) accumulator(my_char_char_0_accum) combiner(combiner)
static void my_char_char_0_accum(int *accum, char a, char b) { }

#pragma rs reduce(my_char_char_1) accumulator(my_char_char_1_accum) combiner(combiner)
static void my_char_char_1_accum(int *accum, char a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_char_char2_0) accumulator(my_char_char2_0_accum) combiner(combiner)
static void my_char_char2_0_accum(int *accum, char a, char2 b) { }

#pragma rs reduce(my_char_char2_1) accumulator(my_char_char2_1_accum) combiner(combiner)
static void my_char_char2_1_accum(int *accum, char a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_char4_0) accumulator(my_char_char4_0_accum) combiner(combiner)
static void my_char_char4_0_accum(int *accum, char a, char4 b) { }

#pragma rs reduce(my_char_char4_1) accumulator(my_char_char4_1_accum) combiner(combiner)
static void my_char_char4_1_accum(int *accum, char a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_short_0) accumulator(my_char_short_0_accum) combiner(combiner)
static void my_char_short_0_accum(int *accum, char a, short b) { }

#pragma rs reduce(my_char_short_1) accumulator(my_char_short_1_accum) combiner(combiner)
static void my_char_short_1_accum(int *accum, char a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_char_short2_0) accumulator(my_char_short2_0_accum) combiner(combiner)
static void my_char_short2_0_accum(int *accum, char a, short2 b) { }

#pragma rs reduce(my_char_short2_1) accumulator(my_char_short2_1_accum) combiner(combiner)
static void my_char_short2_1_accum(int *accum, char a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_short4_0) accumulator(my_char_short4_0_accum) combiner(combiner)
static void my_char_short4_0_accum(int *accum, char a, short4 b) { }

#pragma rs reduce(my_char_short4_1) accumulator(my_char_short4_1_accum) combiner(combiner)
static void my_char_short4_1_accum(int *accum, char a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_uchar_0) accumulator(my_char_uchar_0_accum) combiner(combiner)
static void my_char_uchar_0_accum(int *accum, char a, uchar b) { }

#pragma rs reduce(my_char_uchar_1) accumulator(my_char_uchar_1_accum) combiner(combiner)
static void my_char_uchar_1_accum(int *accum, char a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_char_uchar2_0) accumulator(my_char_uchar2_0_accum) combiner(combiner)
static void my_char_uchar2_0_accum(int *accum, char a, uchar2 b) { }

#pragma rs reduce(my_char_uchar2_1) accumulator(my_char_uchar2_1_accum) combiner(combiner)
static void my_char_uchar2_1_accum(int *accum, char a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_uchar4_0) accumulator(my_char_uchar4_0_accum) combiner(combiner)
static void my_char_uchar4_0_accum(int *accum, char a, uchar4 b) { }

#pragma rs reduce(my_char_uchar4_1) accumulator(my_char_uchar4_1_accum) combiner(combiner)
static void my_char_uchar4_1_accum(int *accum, char a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_ushort_0) accumulator(my_char_ushort_0_accum) combiner(combiner)
static void my_char_ushort_0_accum(int *accum, char a, ushort b) { }

#pragma rs reduce(my_char_ushort_1) accumulator(my_char_ushort_1_accum) combiner(combiner)
static void my_char_ushort_1_accum(int *accum, char a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_char_ushort2_0) accumulator(my_char_ushort2_0_accum) combiner(combiner)
static void my_char_ushort2_0_accum(int *accum, char a, ushort2 b) { }

#pragma rs reduce(my_char_ushort2_1) accumulator(my_char_ushort2_1_accum) combiner(combiner)
static void my_char_ushort2_1_accum(int *accum, char a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_ushort4_0) accumulator(my_char_ushort4_0_accum) combiner(combiner)
static void my_char_ushort4_0_accum(int *accum, char a, ushort4 b) { }

#pragma rs reduce(my_char_ushort4_1) accumulator(my_char_ushort4_1_accum) combiner(combiner)
static void my_char_ushort4_1_accum(int *accum, char a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_bool_0) accumulator(my_char_bool_0_accum) combiner(combiner)
static void my_char_bool_0_accum(int *accum, char a, bool b) { }

#pragma rs reduce(my_char_bool_1) accumulator(my_char_bool_1_accum) combiner(combiner)
static void my_char_bool_1_accum(int *accum, char a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_char_rs_matrix2x2_0) accumulator(my_char_rs_matrix2x2_0_accum) combiner(combiner)
static void my_char_rs_matrix2x2_0_accum(int *accum, char a, rs_matrix2x2 b) { }

#pragma rs reduce(my_char_rs_matrix2x2_1) accumulator(my_char_rs_matrix2x2_1_accum) combiner(combiner)
static void my_char_rs_matrix2x2_1_accum(int *accum, char a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char_MyStruct_0) accumulator(my_char_MyStruct_0_accum) combiner(combiner)
static void my_char_MyStruct_0_accum(int *accum, char a, MyStruct b) { }

#pragma rs reduce(my_char_MyStruct_1) accumulator(my_char_MyStruct_1_accum) combiner(combiner)
static void my_char_MyStruct_1_accum(int *accum, char a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_half_0) accumulator(my_char2_half_0_accum) combiner(combiner)
static void my_char2_half_0_accum(int *accum, char2 a, half b) { }

#pragma rs reduce(my_char2_half_1) accumulator(my_char2_half_1_accum) combiner(combiner)
static void my_char2_half_1_accum(int *accum, char2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_half2_0) accumulator(my_char2_half2_0_accum) combiner(combiner)
static void my_char2_half2_0_accum(int *accum, char2 a, half2 b) { }

#pragma rs reduce(my_char2_half2_1) accumulator(my_char2_half2_1_accum) combiner(combiner)
static void my_char2_half2_1_accum(int *accum, char2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_half4_0) accumulator(my_char2_half4_0_accum) combiner(combiner)
static void my_char2_half4_0_accum(int *accum, char2 a, half4 b) { }

#pragma rs reduce(my_char2_half4_1) accumulator(my_char2_half4_1_accum) combiner(combiner)
static void my_char2_half4_1_accum(int *accum, char2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_float_0) accumulator(my_char2_float_0_accum) combiner(combiner)
static void my_char2_float_0_accum(int *accum, char2 a, float b) { }

#pragma rs reduce(my_char2_float_1) accumulator(my_char2_float_1_accum) combiner(combiner)
static void my_char2_float_1_accum(int *accum, char2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_float2_0) accumulator(my_char2_float2_0_accum) combiner(combiner)
static void my_char2_float2_0_accum(int *accum, char2 a, float2 b) { }

#pragma rs reduce(my_char2_float2_1) accumulator(my_char2_float2_1_accum) combiner(combiner)
static void my_char2_float2_1_accum(int *accum, char2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_float4_0) accumulator(my_char2_float4_0_accum) combiner(combiner)
static void my_char2_float4_0_accum(int *accum, char2 a, float4 b) { }

#pragma rs reduce(my_char2_float4_1) accumulator(my_char2_float4_1_accum) combiner(combiner)
static void my_char2_float4_1_accum(int *accum, char2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_char_0) accumulator(my_char2_char_0_accum) combiner(combiner)
static void my_char2_char_0_accum(int *accum, char2 a, char b) { }

#pragma rs reduce(my_char2_char_1) accumulator(my_char2_char_1_accum) combiner(combiner)
static void my_char2_char_1_accum(int *accum, char2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_char2_0) accumulator(my_char2_char2_0_accum) combiner(combiner)
static void my_char2_char2_0_accum(int *accum, char2 a, char2 b) { }

#pragma rs reduce(my_char2_char2_1) accumulator(my_char2_char2_1_accum) combiner(combiner)
static void my_char2_char2_1_accum(int *accum, char2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_char4_0) accumulator(my_char2_char4_0_accum) combiner(combiner)
static void my_char2_char4_0_accum(int *accum, char2 a, char4 b) { }

#pragma rs reduce(my_char2_char4_1) accumulator(my_char2_char4_1_accum) combiner(combiner)
static void my_char2_char4_1_accum(int *accum, char2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_short_0) accumulator(my_char2_short_0_accum) combiner(combiner)
static void my_char2_short_0_accum(int *accum, char2 a, short b) { }

#pragma rs reduce(my_char2_short_1) accumulator(my_char2_short_1_accum) combiner(combiner)
static void my_char2_short_1_accum(int *accum, char2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_short2_0) accumulator(my_char2_short2_0_accum) combiner(combiner)
static void my_char2_short2_0_accum(int *accum, char2 a, short2 b) { }

#pragma rs reduce(my_char2_short2_1) accumulator(my_char2_short2_1_accum) combiner(combiner)
static void my_char2_short2_1_accum(int *accum, char2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_short4_0) accumulator(my_char2_short4_0_accum) combiner(combiner)
static void my_char2_short4_0_accum(int *accum, char2 a, short4 b) { }

#pragma rs reduce(my_char2_short4_1) accumulator(my_char2_short4_1_accum) combiner(combiner)
static void my_char2_short4_1_accum(int *accum, char2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_uchar_0) accumulator(my_char2_uchar_0_accum) combiner(combiner)
static void my_char2_uchar_0_accum(int *accum, char2 a, uchar b) { }

#pragma rs reduce(my_char2_uchar_1) accumulator(my_char2_uchar_1_accum) combiner(combiner)
static void my_char2_uchar_1_accum(int *accum, char2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_uchar2_0) accumulator(my_char2_uchar2_0_accum) combiner(combiner)
static void my_char2_uchar2_0_accum(int *accum, char2 a, uchar2 b) { }

#pragma rs reduce(my_char2_uchar2_1) accumulator(my_char2_uchar2_1_accum) combiner(combiner)
static void my_char2_uchar2_1_accum(int *accum, char2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_uchar4_0) accumulator(my_char2_uchar4_0_accum) combiner(combiner)
static void my_char2_uchar4_0_accum(int *accum, char2 a, uchar4 b) { }

#pragma rs reduce(my_char2_uchar4_1) accumulator(my_char2_uchar4_1_accum) combiner(combiner)
static void my_char2_uchar4_1_accum(int *accum, char2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_ushort_0) accumulator(my_char2_ushort_0_accum) combiner(combiner)
static void my_char2_ushort_0_accum(int *accum, char2 a, ushort b) { }

#pragma rs reduce(my_char2_ushort_1) accumulator(my_char2_ushort_1_accum) combiner(combiner)
static void my_char2_ushort_1_accum(int *accum, char2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_ushort2_0) accumulator(my_char2_ushort2_0_accum) combiner(combiner)
static void my_char2_ushort2_0_accum(int *accum, char2 a, ushort2 b) { }

#pragma rs reduce(my_char2_ushort2_1) accumulator(my_char2_ushort2_1_accum) combiner(combiner)
static void my_char2_ushort2_1_accum(int *accum, char2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_ushort4_0) accumulator(my_char2_ushort4_0_accum) combiner(combiner)
static void my_char2_ushort4_0_accum(int *accum, char2 a, ushort4 b) { }

#pragma rs reduce(my_char2_ushort4_1) accumulator(my_char2_ushort4_1_accum) combiner(combiner)
static void my_char2_ushort4_1_accum(int *accum, char2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_bool_0) accumulator(my_char2_bool_0_accum) combiner(combiner)
static void my_char2_bool_0_accum(int *accum, char2 a, bool b) { }

#pragma rs reduce(my_char2_bool_1) accumulator(my_char2_bool_1_accum) combiner(combiner)
static void my_char2_bool_1_accum(int *accum, char2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_rs_matrix2x2_0) accumulator(my_char2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_char2_rs_matrix2x2_0_accum(int *accum, char2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_char2_rs_matrix2x2_1) accumulator(my_char2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_char2_rs_matrix2x2_1_accum(int *accum, char2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char2_MyStruct_0) accumulator(my_char2_MyStruct_0_accum) combiner(combiner)
static void my_char2_MyStruct_0_accum(int *accum, char2 a, MyStruct b) { }

#pragma rs reduce(my_char2_MyStruct_1) accumulator(my_char2_MyStruct_1_accum) combiner(combiner)
static void my_char2_MyStruct_1_accum(int *accum, char2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_half_0) accumulator(my_char4_half_0_accum) combiner(combiner)
static void my_char4_half_0_accum(int *accum, char4 a, half b) { }

#pragma rs reduce(my_char4_half_1) accumulator(my_char4_half_1_accum) combiner(combiner)
static void my_char4_half_1_accum(int *accum, char4 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_half2_0) accumulator(my_char4_half2_0_accum) combiner(combiner)
static void my_char4_half2_0_accum(int *accum, char4 a, half2 b) { }

#pragma rs reduce(my_char4_half2_1) accumulator(my_char4_half2_1_accum) combiner(combiner)
static void my_char4_half2_1_accum(int *accum, char4 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_half4_0) accumulator(my_char4_half4_0_accum) combiner(combiner)
static void my_char4_half4_0_accum(int *accum, char4 a, half4 b) { }

#pragma rs reduce(my_char4_half4_1) accumulator(my_char4_half4_1_accum) combiner(combiner)
static void my_char4_half4_1_accum(int *accum, char4 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_float_0) accumulator(my_char4_float_0_accum) combiner(combiner)
static void my_char4_float_0_accum(int *accum, char4 a, float b) { }

#pragma rs reduce(my_char4_float_1) accumulator(my_char4_float_1_accum) combiner(combiner)
static void my_char4_float_1_accum(int *accum, char4 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_float2_0) accumulator(my_char4_float2_0_accum) combiner(combiner)
static void my_char4_float2_0_accum(int *accum, char4 a, float2 b) { }

#pragma rs reduce(my_char4_float2_1) accumulator(my_char4_float2_1_accum) combiner(combiner)
static void my_char4_float2_1_accum(int *accum, char4 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_float4_0) accumulator(my_char4_float4_0_accum) combiner(combiner)
static void my_char4_float4_0_accum(int *accum, char4 a, float4 b) { }

#pragma rs reduce(my_char4_float4_1) accumulator(my_char4_float4_1_accum) combiner(combiner)
static void my_char4_float4_1_accum(int *accum, char4 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_char_0) accumulator(my_char4_char_0_accum) combiner(combiner)
static void my_char4_char_0_accum(int *accum, char4 a, char b) { }

#pragma rs reduce(my_char4_char_1) accumulator(my_char4_char_1_accum) combiner(combiner)
static void my_char4_char_1_accum(int *accum, char4 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_char2_0) accumulator(my_char4_char2_0_accum) combiner(combiner)
static void my_char4_char2_0_accum(int *accum, char4 a, char2 b) { }

#pragma rs reduce(my_char4_char2_1) accumulator(my_char4_char2_1_accum) combiner(combiner)
static void my_char4_char2_1_accum(int *accum, char4 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_char4_0) accumulator(my_char4_char4_0_accum) combiner(combiner)
static void my_char4_char4_0_accum(int *accum, char4 a, char4 b) { }

#pragma rs reduce(my_char4_char4_1) accumulator(my_char4_char4_1_accum) combiner(combiner)
static void my_char4_char4_1_accum(int *accum, char4 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_short_0) accumulator(my_char4_short_0_accum) combiner(combiner)
static void my_char4_short_0_accum(int *accum, char4 a, short b) { }

#pragma rs reduce(my_char4_short_1) accumulator(my_char4_short_1_accum) combiner(combiner)
static void my_char4_short_1_accum(int *accum, char4 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_short2_0) accumulator(my_char4_short2_0_accum) combiner(combiner)
static void my_char4_short2_0_accum(int *accum, char4 a, short2 b) { }

#pragma rs reduce(my_char4_short2_1) accumulator(my_char4_short2_1_accum) combiner(combiner)
static void my_char4_short2_1_accum(int *accum, char4 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_short4_0) accumulator(my_char4_short4_0_accum) combiner(combiner)
static void my_char4_short4_0_accum(int *accum, char4 a, short4 b) { }

#pragma rs reduce(my_char4_short4_1) accumulator(my_char4_short4_1_accum) combiner(combiner)
static void my_char4_short4_1_accum(int *accum, char4 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_uchar_0) accumulator(my_char4_uchar_0_accum) combiner(combiner)
static void my_char4_uchar_0_accum(int *accum, char4 a, uchar b) { }

#pragma rs reduce(my_char4_uchar_1) accumulator(my_char4_uchar_1_accum) combiner(combiner)
static void my_char4_uchar_1_accum(int *accum, char4 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_uchar2_0) accumulator(my_char4_uchar2_0_accum) combiner(combiner)
static void my_char4_uchar2_0_accum(int *accum, char4 a, uchar2 b) { }

#pragma rs reduce(my_char4_uchar2_1) accumulator(my_char4_uchar2_1_accum) combiner(combiner)
static void my_char4_uchar2_1_accum(int *accum, char4 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_uchar4_0) accumulator(my_char4_uchar4_0_accum) combiner(combiner)
static void my_char4_uchar4_0_accum(int *accum, char4 a, uchar4 b) { }

#pragma rs reduce(my_char4_uchar4_1) accumulator(my_char4_uchar4_1_accum) combiner(combiner)
static void my_char4_uchar4_1_accum(int *accum, char4 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_ushort_0) accumulator(my_char4_ushort_0_accum) combiner(combiner)
static void my_char4_ushort_0_accum(int *accum, char4 a, ushort b) { }

#pragma rs reduce(my_char4_ushort_1) accumulator(my_char4_ushort_1_accum) combiner(combiner)
static void my_char4_ushort_1_accum(int *accum, char4 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_ushort2_0) accumulator(my_char4_ushort2_0_accum) combiner(combiner)
static void my_char4_ushort2_0_accum(int *accum, char4 a, ushort2 b) { }

#pragma rs reduce(my_char4_ushort2_1) accumulator(my_char4_ushort2_1_accum) combiner(combiner)
static void my_char4_ushort2_1_accum(int *accum, char4 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_ushort4_0) accumulator(my_char4_ushort4_0_accum) combiner(combiner)
static void my_char4_ushort4_0_accum(int *accum, char4 a, ushort4 b) { }

#pragma rs reduce(my_char4_ushort4_1) accumulator(my_char4_ushort4_1_accum) combiner(combiner)
static void my_char4_ushort4_1_accum(int *accum, char4 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_bool_0) accumulator(my_char4_bool_0_accum) combiner(combiner)
static void my_char4_bool_0_accum(int *accum, char4 a, bool b) { }

#pragma rs reduce(my_char4_bool_1) accumulator(my_char4_bool_1_accum) combiner(combiner)
static void my_char4_bool_1_accum(int *accum, char4 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_rs_matrix2x2_0) accumulator(my_char4_rs_matrix2x2_0_accum) combiner(combiner)
static void my_char4_rs_matrix2x2_0_accum(int *accum, char4 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_char4_rs_matrix2x2_1) accumulator(my_char4_rs_matrix2x2_1_accum) combiner(combiner)
static void my_char4_rs_matrix2x2_1_accum(int *accum, char4 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_char4_MyStruct_0) accumulator(my_char4_MyStruct_0_accum) combiner(combiner)
static void my_char4_MyStruct_0_accum(int *accum, char4 a, MyStruct b) { }

#pragma rs reduce(my_char4_MyStruct_1) accumulator(my_char4_MyStruct_1_accum) combiner(combiner)
static void my_char4_MyStruct_1_accum(int *accum, char4 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_short_half_0) accumulator(my_short_half_0_accum) combiner(combiner)
static void my_short_half_0_accum(int *accum, short a, half b) { }

#pragma rs reduce(my_short_half_1) accumulator(my_short_half_1_accum) combiner(combiner)
static void my_short_half_1_accum(int *accum, short a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_short_half2_0) accumulator(my_short_half2_0_accum) combiner(combiner)
static void my_short_half2_0_accum(int *accum, short a, half2 b) { }

#pragma rs reduce(my_short_half2_1) accumulator(my_short_half2_1_accum) combiner(combiner)
static void my_short_half2_1_accum(int *accum, short a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_half4_0) accumulator(my_short_half4_0_accum) combiner(combiner)
static void my_short_half4_0_accum(int *accum, short a, half4 b) { }

#pragma rs reduce(my_short_half4_1) accumulator(my_short_half4_1_accum) combiner(combiner)
static void my_short_half4_1_accum(int *accum, short a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_float_0) accumulator(my_short_float_0_accum) combiner(combiner)
static void my_short_float_0_accum(int *accum, short a, float b) { }

#pragma rs reduce(my_short_float_1) accumulator(my_short_float_1_accum) combiner(combiner)
static void my_short_float_1_accum(int *accum, short a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_short_float2_0) accumulator(my_short_float2_0_accum) combiner(combiner)
static void my_short_float2_0_accum(int *accum, short a, float2 b) { }

#pragma rs reduce(my_short_float2_1) accumulator(my_short_float2_1_accum) combiner(combiner)
static void my_short_float2_1_accum(int *accum, short a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_float4_0) accumulator(my_short_float4_0_accum) combiner(combiner)
static void my_short_float4_0_accum(int *accum, short a, float4 b) { }

#pragma rs reduce(my_short_float4_1) accumulator(my_short_float4_1_accum) combiner(combiner)
static void my_short_float4_1_accum(int *accum, short a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_char_0) accumulator(my_short_char_0_accum) combiner(combiner)
static void my_short_char_0_accum(int *accum, short a, char b) { }

#pragma rs reduce(my_short_char_1) accumulator(my_short_char_1_accum) combiner(combiner)
static void my_short_char_1_accum(int *accum, short a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_short_char2_0) accumulator(my_short_char2_0_accum) combiner(combiner)
static void my_short_char2_0_accum(int *accum, short a, char2 b) { }

#pragma rs reduce(my_short_char2_1) accumulator(my_short_char2_1_accum) combiner(combiner)
static void my_short_char2_1_accum(int *accum, short a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_char4_0) accumulator(my_short_char4_0_accum) combiner(combiner)
static void my_short_char4_0_accum(int *accum, short a, char4 b) { }

#pragma rs reduce(my_short_char4_1) accumulator(my_short_char4_1_accum) combiner(combiner)
static void my_short_char4_1_accum(int *accum, short a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_short_0) accumulator(my_short_short_0_accum) combiner(combiner)
static void my_short_short_0_accum(int *accum, short a, short b) { }

#pragma rs reduce(my_short_short_1) accumulator(my_short_short_1_accum) combiner(combiner)
static void my_short_short_1_accum(int *accum, short a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_short_short2_0) accumulator(my_short_short2_0_accum) combiner(combiner)
static void my_short_short2_0_accum(int *accum, short a, short2 b) { }

#pragma rs reduce(my_short_short2_1) accumulator(my_short_short2_1_accum) combiner(combiner)
static void my_short_short2_1_accum(int *accum, short a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_short4_0) accumulator(my_short_short4_0_accum) combiner(combiner)
static void my_short_short4_0_accum(int *accum, short a, short4 b) { }

#pragma rs reduce(my_short_short4_1) accumulator(my_short_short4_1_accum) combiner(combiner)
static void my_short_short4_1_accum(int *accum, short a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_uchar_0) accumulator(my_short_uchar_0_accum) combiner(combiner)
static void my_short_uchar_0_accum(int *accum, short a, uchar b) { }

#pragma rs reduce(my_short_uchar_1) accumulator(my_short_uchar_1_accum) combiner(combiner)
static void my_short_uchar_1_accum(int *accum, short a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_short_uchar2_0) accumulator(my_short_uchar2_0_accum) combiner(combiner)
static void my_short_uchar2_0_accum(int *accum, short a, uchar2 b) { }

#pragma rs reduce(my_short_uchar2_1) accumulator(my_short_uchar2_1_accum) combiner(combiner)
static void my_short_uchar2_1_accum(int *accum, short a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_uchar4_0) accumulator(my_short_uchar4_0_accum) combiner(combiner)
static void my_short_uchar4_0_accum(int *accum, short a, uchar4 b) { }

#pragma rs reduce(my_short_uchar4_1) accumulator(my_short_uchar4_1_accum) combiner(combiner)
static void my_short_uchar4_1_accum(int *accum, short a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_ushort_0) accumulator(my_short_ushort_0_accum) combiner(combiner)
static void my_short_ushort_0_accum(int *accum, short a, ushort b) { }

#pragma rs reduce(my_short_ushort_1) accumulator(my_short_ushort_1_accum) combiner(combiner)
static void my_short_ushort_1_accum(int *accum, short a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_short_ushort2_0) accumulator(my_short_ushort2_0_accum) combiner(combiner)
static void my_short_ushort2_0_accum(int *accum, short a, ushort2 b) { }

#pragma rs reduce(my_short_ushort2_1) accumulator(my_short_ushort2_1_accum) combiner(combiner)
static void my_short_ushort2_1_accum(int *accum, short a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_ushort4_0) accumulator(my_short_ushort4_0_accum) combiner(combiner)
static void my_short_ushort4_0_accum(int *accum, short a, ushort4 b) { }

#pragma rs reduce(my_short_ushort4_1) accumulator(my_short_ushort4_1_accum) combiner(combiner)
static void my_short_ushort4_1_accum(int *accum, short a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_bool_0) accumulator(my_short_bool_0_accum) combiner(combiner)
static void my_short_bool_0_accum(int *accum, short a, bool b) { }

#pragma rs reduce(my_short_bool_1) accumulator(my_short_bool_1_accum) combiner(combiner)
static void my_short_bool_1_accum(int *accum, short a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_short_rs_matrix2x2_0) accumulator(my_short_rs_matrix2x2_0_accum) combiner(combiner)
static void my_short_rs_matrix2x2_0_accum(int *accum, short a, rs_matrix2x2 b) { }

#pragma rs reduce(my_short_rs_matrix2x2_1) accumulator(my_short_rs_matrix2x2_1_accum) combiner(combiner)
static void my_short_rs_matrix2x2_1_accum(int *accum, short a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short_MyStruct_0) accumulator(my_short_MyStruct_0_accum) combiner(combiner)
static void my_short_MyStruct_0_accum(int *accum, short a, MyStruct b) { }

#pragma rs reduce(my_short_MyStruct_1) accumulator(my_short_MyStruct_1_accum) combiner(combiner)
static void my_short_MyStruct_1_accum(int *accum, short a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_half_0) accumulator(my_short2_half_0_accum) combiner(combiner)
static void my_short2_half_0_accum(int *accum, short2 a, half b) { }

#pragma rs reduce(my_short2_half_1) accumulator(my_short2_half_1_accum) combiner(combiner)
static void my_short2_half_1_accum(int *accum, short2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_half2_0) accumulator(my_short2_half2_0_accum) combiner(combiner)
static void my_short2_half2_0_accum(int *accum, short2 a, half2 b) { }

#pragma rs reduce(my_short2_half2_1) accumulator(my_short2_half2_1_accum) combiner(combiner)
static void my_short2_half2_1_accum(int *accum, short2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_half4_0) accumulator(my_short2_half4_0_accum) combiner(combiner)
static void my_short2_half4_0_accum(int *accum, short2 a, half4 b) { }

#pragma rs reduce(my_short2_half4_1) accumulator(my_short2_half4_1_accum) combiner(combiner)
static void my_short2_half4_1_accum(int *accum, short2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_float_0) accumulator(my_short2_float_0_accum) combiner(combiner)
static void my_short2_float_0_accum(int *accum, short2 a, float b) { }

#pragma rs reduce(my_short2_float_1) accumulator(my_short2_float_1_accum) combiner(combiner)
static void my_short2_float_1_accum(int *accum, short2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_float2_0) accumulator(my_short2_float2_0_accum) combiner(combiner)
static void my_short2_float2_0_accum(int *accum, short2 a, float2 b) { }

#pragma rs reduce(my_short2_float2_1) accumulator(my_short2_float2_1_accum) combiner(combiner)
static void my_short2_float2_1_accum(int *accum, short2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_float4_0) accumulator(my_short2_float4_0_accum) combiner(combiner)
static void my_short2_float4_0_accum(int *accum, short2 a, float4 b) { }

#pragma rs reduce(my_short2_float4_1) accumulator(my_short2_float4_1_accum) combiner(combiner)
static void my_short2_float4_1_accum(int *accum, short2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_char_0) accumulator(my_short2_char_0_accum) combiner(combiner)
static void my_short2_char_0_accum(int *accum, short2 a, char b) { }

#pragma rs reduce(my_short2_char_1) accumulator(my_short2_char_1_accum) combiner(combiner)
static void my_short2_char_1_accum(int *accum, short2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_char2_0) accumulator(my_short2_char2_0_accum) combiner(combiner)
static void my_short2_char2_0_accum(int *accum, short2 a, char2 b) { }

#pragma rs reduce(my_short2_char2_1) accumulator(my_short2_char2_1_accum) combiner(combiner)
static void my_short2_char2_1_accum(int *accum, short2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_char4_0) accumulator(my_short2_char4_0_accum) combiner(combiner)
static void my_short2_char4_0_accum(int *accum, short2 a, char4 b) { }

#pragma rs reduce(my_short2_char4_1) accumulator(my_short2_char4_1_accum) combiner(combiner)
static void my_short2_char4_1_accum(int *accum, short2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_short_0) accumulator(my_short2_short_0_accum) combiner(combiner)
static void my_short2_short_0_accum(int *accum, short2 a, short b) { }

#pragma rs reduce(my_short2_short_1) accumulator(my_short2_short_1_accum) combiner(combiner)
static void my_short2_short_1_accum(int *accum, short2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_short2_0) accumulator(my_short2_short2_0_accum) combiner(combiner)
static void my_short2_short2_0_accum(int *accum, short2 a, short2 b) { }

#pragma rs reduce(my_short2_short2_1) accumulator(my_short2_short2_1_accum) combiner(combiner)
static void my_short2_short2_1_accum(int *accum, short2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_short4_0) accumulator(my_short2_short4_0_accum) combiner(combiner)
static void my_short2_short4_0_accum(int *accum, short2 a, short4 b) { }

#pragma rs reduce(my_short2_short4_1) accumulator(my_short2_short4_1_accum) combiner(combiner)
static void my_short2_short4_1_accum(int *accum, short2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_uchar_0) accumulator(my_short2_uchar_0_accum) combiner(combiner)
static void my_short2_uchar_0_accum(int *accum, short2 a, uchar b) { }

#pragma rs reduce(my_short2_uchar_1) accumulator(my_short2_uchar_1_accum) combiner(combiner)
static void my_short2_uchar_1_accum(int *accum, short2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_uchar2_0) accumulator(my_short2_uchar2_0_accum) combiner(combiner)
static void my_short2_uchar2_0_accum(int *accum, short2 a, uchar2 b) { }

#pragma rs reduce(my_short2_uchar2_1) accumulator(my_short2_uchar2_1_accum) combiner(combiner)
static void my_short2_uchar2_1_accum(int *accum, short2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_uchar4_0) accumulator(my_short2_uchar4_0_accum) combiner(combiner)
static void my_short2_uchar4_0_accum(int *accum, short2 a, uchar4 b) { }

#pragma rs reduce(my_short2_uchar4_1) accumulator(my_short2_uchar4_1_accum) combiner(combiner)
static void my_short2_uchar4_1_accum(int *accum, short2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_ushort_0) accumulator(my_short2_ushort_0_accum) combiner(combiner)
static void my_short2_ushort_0_accum(int *accum, short2 a, ushort b) { }

#pragma rs reduce(my_short2_ushort_1) accumulator(my_short2_ushort_1_accum) combiner(combiner)
static void my_short2_ushort_1_accum(int *accum, short2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_ushort2_0) accumulator(my_short2_ushort2_0_accum) combiner(combiner)
static void my_short2_ushort2_0_accum(int *accum, short2 a, ushort2 b) { }

#pragma rs reduce(my_short2_ushort2_1) accumulator(my_short2_ushort2_1_accum) combiner(combiner)
static void my_short2_ushort2_1_accum(int *accum, short2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_ushort4_0) accumulator(my_short2_ushort4_0_accum) combiner(combiner)
static void my_short2_ushort4_0_accum(int *accum, short2 a, ushort4 b) { }

#pragma rs reduce(my_short2_ushort4_1) accumulator(my_short2_ushort4_1_accum) combiner(combiner)
static void my_short2_ushort4_1_accum(int *accum, short2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_bool_0) accumulator(my_short2_bool_0_accum) combiner(combiner)
static void my_short2_bool_0_accum(int *accum, short2 a, bool b) { }

#pragma rs reduce(my_short2_bool_1) accumulator(my_short2_bool_1_accum) combiner(combiner)
static void my_short2_bool_1_accum(int *accum, short2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_rs_matrix2x2_0) accumulator(my_short2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_short2_rs_matrix2x2_0_accum(int *accum, short2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_short2_rs_matrix2x2_1) accumulator(my_short2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_short2_rs_matrix2x2_1_accum(int *accum, short2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short2_MyStruct_0) accumulator(my_short2_MyStruct_0_accum) combiner(combiner)
static void my_short2_MyStruct_0_accum(int *accum, short2 a, MyStruct b) { }

#pragma rs reduce(my_short2_MyStruct_1) accumulator(my_short2_MyStruct_1_accum) combiner(combiner)
static void my_short2_MyStruct_1_accum(int *accum, short2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_half_0) accumulator(my_short4_half_0_accum) combiner(combiner)
static void my_short4_half_0_accum(int *accum, short4 a, half b) { }

#pragma rs reduce(my_short4_half_1) accumulator(my_short4_half_1_accum) combiner(combiner)
static void my_short4_half_1_accum(int *accum, short4 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_half2_0) accumulator(my_short4_half2_0_accum) combiner(combiner)
static void my_short4_half2_0_accum(int *accum, short4 a, half2 b) { }

#pragma rs reduce(my_short4_half2_1) accumulator(my_short4_half2_1_accum) combiner(combiner)
static void my_short4_half2_1_accum(int *accum, short4 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_half4_0) accumulator(my_short4_half4_0_accum) combiner(combiner)
static void my_short4_half4_0_accum(int *accum, short4 a, half4 b) { }

#pragma rs reduce(my_short4_half4_1) accumulator(my_short4_half4_1_accum) combiner(combiner)
static void my_short4_half4_1_accum(int *accum, short4 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_float_0) accumulator(my_short4_float_0_accum) combiner(combiner)
static void my_short4_float_0_accum(int *accum, short4 a, float b) { }

#pragma rs reduce(my_short4_float_1) accumulator(my_short4_float_1_accum) combiner(combiner)
static void my_short4_float_1_accum(int *accum, short4 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_float2_0) accumulator(my_short4_float2_0_accum) combiner(combiner)
static void my_short4_float2_0_accum(int *accum, short4 a, float2 b) { }

#pragma rs reduce(my_short4_float2_1) accumulator(my_short4_float2_1_accum) combiner(combiner)
static void my_short4_float2_1_accum(int *accum, short4 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_float4_0) accumulator(my_short4_float4_0_accum) combiner(combiner)
static void my_short4_float4_0_accum(int *accum, short4 a, float4 b) { }

#pragma rs reduce(my_short4_float4_1) accumulator(my_short4_float4_1_accum) combiner(combiner)
static void my_short4_float4_1_accum(int *accum, short4 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_char_0) accumulator(my_short4_char_0_accum) combiner(combiner)
static void my_short4_char_0_accum(int *accum, short4 a, char b) { }

#pragma rs reduce(my_short4_char_1) accumulator(my_short4_char_1_accum) combiner(combiner)
static void my_short4_char_1_accum(int *accum, short4 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_char2_0) accumulator(my_short4_char2_0_accum) combiner(combiner)
static void my_short4_char2_0_accum(int *accum, short4 a, char2 b) { }

#pragma rs reduce(my_short4_char2_1) accumulator(my_short4_char2_1_accum) combiner(combiner)
static void my_short4_char2_1_accum(int *accum, short4 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_char4_0) accumulator(my_short4_char4_0_accum) combiner(combiner)
static void my_short4_char4_0_accum(int *accum, short4 a, char4 b) { }

#pragma rs reduce(my_short4_char4_1) accumulator(my_short4_char4_1_accum) combiner(combiner)
static void my_short4_char4_1_accum(int *accum, short4 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_short_0) accumulator(my_short4_short_0_accum) combiner(combiner)
static void my_short4_short_0_accum(int *accum, short4 a, short b) { }

#pragma rs reduce(my_short4_short_1) accumulator(my_short4_short_1_accum) combiner(combiner)
static void my_short4_short_1_accum(int *accum, short4 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_short2_0) accumulator(my_short4_short2_0_accum) combiner(combiner)
static void my_short4_short2_0_accum(int *accum, short4 a, short2 b) { }

#pragma rs reduce(my_short4_short2_1) accumulator(my_short4_short2_1_accum) combiner(combiner)
static void my_short4_short2_1_accum(int *accum, short4 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_short4_0) accumulator(my_short4_short4_0_accum) combiner(combiner)
static void my_short4_short4_0_accum(int *accum, short4 a, short4 b) { }

#pragma rs reduce(my_short4_short4_1) accumulator(my_short4_short4_1_accum) combiner(combiner)
static void my_short4_short4_1_accum(int *accum, short4 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_uchar_0) accumulator(my_short4_uchar_0_accum) combiner(combiner)
static void my_short4_uchar_0_accum(int *accum, short4 a, uchar b) { }

#pragma rs reduce(my_short4_uchar_1) accumulator(my_short4_uchar_1_accum) combiner(combiner)
static void my_short4_uchar_1_accum(int *accum, short4 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_uchar2_0) accumulator(my_short4_uchar2_0_accum) combiner(combiner)
static void my_short4_uchar2_0_accum(int *accum, short4 a, uchar2 b) { }

#pragma rs reduce(my_short4_uchar2_1) accumulator(my_short4_uchar2_1_accum) combiner(combiner)
static void my_short4_uchar2_1_accum(int *accum, short4 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_uchar4_0) accumulator(my_short4_uchar4_0_accum) combiner(combiner)
static void my_short4_uchar4_0_accum(int *accum, short4 a, uchar4 b) { }

#pragma rs reduce(my_short4_uchar4_1) accumulator(my_short4_uchar4_1_accum) combiner(combiner)
static void my_short4_uchar4_1_accum(int *accum, short4 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_ushort_0) accumulator(my_short4_ushort_0_accum) combiner(combiner)
static void my_short4_ushort_0_accum(int *accum, short4 a, ushort b) { }

#pragma rs reduce(my_short4_ushort_1) accumulator(my_short4_ushort_1_accum) combiner(combiner)
static void my_short4_ushort_1_accum(int *accum, short4 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_ushort2_0) accumulator(my_short4_ushort2_0_accum) combiner(combiner)
static void my_short4_ushort2_0_accum(int *accum, short4 a, ushort2 b) { }

#pragma rs reduce(my_short4_ushort2_1) accumulator(my_short4_ushort2_1_accum) combiner(combiner)
static void my_short4_ushort2_1_accum(int *accum, short4 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_ushort4_0) accumulator(my_short4_ushort4_0_accum) combiner(combiner)
static void my_short4_ushort4_0_accum(int *accum, short4 a, ushort4 b) { }

#pragma rs reduce(my_short4_ushort4_1) accumulator(my_short4_ushort4_1_accum) combiner(combiner)
static void my_short4_ushort4_1_accum(int *accum, short4 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_bool_0) accumulator(my_short4_bool_0_accum) combiner(combiner)
static void my_short4_bool_0_accum(int *accum, short4 a, bool b) { }

#pragma rs reduce(my_short4_bool_1) accumulator(my_short4_bool_1_accum) combiner(combiner)
static void my_short4_bool_1_accum(int *accum, short4 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_rs_matrix2x2_0) accumulator(my_short4_rs_matrix2x2_0_accum) combiner(combiner)
static void my_short4_rs_matrix2x2_0_accum(int *accum, short4 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_short4_rs_matrix2x2_1) accumulator(my_short4_rs_matrix2x2_1_accum) combiner(combiner)
static void my_short4_rs_matrix2x2_1_accum(int *accum, short4 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_short4_MyStruct_0) accumulator(my_short4_MyStruct_0_accum) combiner(combiner)
static void my_short4_MyStruct_0_accum(int *accum, short4 a, MyStruct b) { }

#pragma rs reduce(my_short4_MyStruct_1) accumulator(my_short4_MyStruct_1_accum) combiner(combiner)
static void my_short4_MyStruct_1_accum(int *accum, short4 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_half_0) accumulator(my_uchar_half_0_accum) combiner(combiner)
static void my_uchar_half_0_accum(int *accum, uchar a, half b) { }

#pragma rs reduce(my_uchar_half_1) accumulator(my_uchar_half_1_accum) combiner(combiner)
static void my_uchar_half_1_accum(int *accum, uchar a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_half2_0) accumulator(my_uchar_half2_0_accum) combiner(combiner)
static void my_uchar_half2_0_accum(int *accum, uchar a, half2 b) { }

#pragma rs reduce(my_uchar_half2_1) accumulator(my_uchar_half2_1_accum) combiner(combiner)
static void my_uchar_half2_1_accum(int *accum, uchar a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_half4_0) accumulator(my_uchar_half4_0_accum) combiner(combiner)
static void my_uchar_half4_0_accum(int *accum, uchar a, half4 b) { }

#pragma rs reduce(my_uchar_half4_1) accumulator(my_uchar_half4_1_accum) combiner(combiner)
static void my_uchar_half4_1_accum(int *accum, uchar a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_float_0) accumulator(my_uchar_float_0_accum) combiner(combiner)
static void my_uchar_float_0_accum(int *accum, uchar a, float b) { }

#pragma rs reduce(my_uchar_float_1) accumulator(my_uchar_float_1_accum) combiner(combiner)
static void my_uchar_float_1_accum(int *accum, uchar a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_float2_0) accumulator(my_uchar_float2_0_accum) combiner(combiner)
static void my_uchar_float2_0_accum(int *accum, uchar a, float2 b) { }

#pragma rs reduce(my_uchar_float2_1) accumulator(my_uchar_float2_1_accum) combiner(combiner)
static void my_uchar_float2_1_accum(int *accum, uchar a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_float4_0) accumulator(my_uchar_float4_0_accum) combiner(combiner)
static void my_uchar_float4_0_accum(int *accum, uchar a, float4 b) { }

#pragma rs reduce(my_uchar_float4_1) accumulator(my_uchar_float4_1_accum) combiner(combiner)
static void my_uchar_float4_1_accum(int *accum, uchar a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_char_0) accumulator(my_uchar_char_0_accum) combiner(combiner)
static void my_uchar_char_0_accum(int *accum, uchar a, char b) { }

#pragma rs reduce(my_uchar_char_1) accumulator(my_uchar_char_1_accum) combiner(combiner)
static void my_uchar_char_1_accum(int *accum, uchar a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_char2_0) accumulator(my_uchar_char2_0_accum) combiner(combiner)
static void my_uchar_char2_0_accum(int *accum, uchar a, char2 b) { }

#pragma rs reduce(my_uchar_char2_1) accumulator(my_uchar_char2_1_accum) combiner(combiner)
static void my_uchar_char2_1_accum(int *accum, uchar a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_char4_0) accumulator(my_uchar_char4_0_accum) combiner(combiner)
static void my_uchar_char4_0_accum(int *accum, uchar a, char4 b) { }

#pragma rs reduce(my_uchar_char4_1) accumulator(my_uchar_char4_1_accum) combiner(combiner)
static void my_uchar_char4_1_accum(int *accum, uchar a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_short_0) accumulator(my_uchar_short_0_accum) combiner(combiner)
static void my_uchar_short_0_accum(int *accum, uchar a, short b) { }

#pragma rs reduce(my_uchar_short_1) accumulator(my_uchar_short_1_accum) combiner(combiner)
static void my_uchar_short_1_accum(int *accum, uchar a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_short2_0) accumulator(my_uchar_short2_0_accum) combiner(combiner)
static void my_uchar_short2_0_accum(int *accum, uchar a, short2 b) { }

#pragma rs reduce(my_uchar_short2_1) accumulator(my_uchar_short2_1_accum) combiner(combiner)
static void my_uchar_short2_1_accum(int *accum, uchar a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_short4_0) accumulator(my_uchar_short4_0_accum) combiner(combiner)
static void my_uchar_short4_0_accum(int *accum, uchar a, short4 b) { }

#pragma rs reduce(my_uchar_short4_1) accumulator(my_uchar_short4_1_accum) combiner(combiner)
static void my_uchar_short4_1_accum(int *accum, uchar a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_uchar_0) accumulator(my_uchar_uchar_0_accum) combiner(combiner)
static void my_uchar_uchar_0_accum(int *accum, uchar a, uchar b) { }

#pragma rs reduce(my_uchar_uchar_1) accumulator(my_uchar_uchar_1_accum) combiner(combiner)
static void my_uchar_uchar_1_accum(int *accum, uchar a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_uchar2_0) accumulator(my_uchar_uchar2_0_accum) combiner(combiner)
static void my_uchar_uchar2_0_accum(int *accum, uchar a, uchar2 b) { }

#pragma rs reduce(my_uchar_uchar2_1) accumulator(my_uchar_uchar2_1_accum) combiner(combiner)
static void my_uchar_uchar2_1_accum(int *accum, uchar a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_uchar4_0) accumulator(my_uchar_uchar4_0_accum) combiner(combiner)
static void my_uchar_uchar4_0_accum(int *accum, uchar a, uchar4 b) { }

#pragma rs reduce(my_uchar_uchar4_1) accumulator(my_uchar_uchar4_1_accum) combiner(combiner)
static void my_uchar_uchar4_1_accum(int *accum, uchar a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_ushort_0) accumulator(my_uchar_ushort_0_accum) combiner(combiner)
static void my_uchar_ushort_0_accum(int *accum, uchar a, ushort b) { }

#pragma rs reduce(my_uchar_ushort_1) accumulator(my_uchar_ushort_1_accum) combiner(combiner)
static void my_uchar_ushort_1_accum(int *accum, uchar a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_ushort2_0) accumulator(my_uchar_ushort2_0_accum) combiner(combiner)
static void my_uchar_ushort2_0_accum(int *accum, uchar a, ushort2 b) { }

#pragma rs reduce(my_uchar_ushort2_1) accumulator(my_uchar_ushort2_1_accum) combiner(combiner)
static void my_uchar_ushort2_1_accum(int *accum, uchar a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_ushort4_0) accumulator(my_uchar_ushort4_0_accum) combiner(combiner)
static void my_uchar_ushort4_0_accum(int *accum, uchar a, ushort4 b) { }

#pragma rs reduce(my_uchar_ushort4_1) accumulator(my_uchar_ushort4_1_accum) combiner(combiner)
static void my_uchar_ushort4_1_accum(int *accum, uchar a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_bool_0) accumulator(my_uchar_bool_0_accum) combiner(combiner)
static void my_uchar_bool_0_accum(int *accum, uchar a, bool b) { }

#pragma rs reduce(my_uchar_bool_1) accumulator(my_uchar_bool_1_accum) combiner(combiner)
static void my_uchar_bool_1_accum(int *accum, uchar a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_rs_matrix2x2_0) accumulator(my_uchar_rs_matrix2x2_0_accum) combiner(combiner)
static void my_uchar_rs_matrix2x2_0_accum(int *accum, uchar a, rs_matrix2x2 b) { }

#pragma rs reduce(my_uchar_rs_matrix2x2_1) accumulator(my_uchar_rs_matrix2x2_1_accum) combiner(combiner)
static void my_uchar_rs_matrix2x2_1_accum(int *accum, uchar a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_MyStruct_0) accumulator(my_uchar_MyStruct_0_accum) combiner(combiner)
static void my_uchar_MyStruct_0_accum(int *accum, uchar a, MyStruct b) { }

#pragma rs reduce(my_uchar_MyStruct_1) accumulator(my_uchar_MyStruct_1_accum) combiner(combiner)
static void my_uchar_MyStruct_1_accum(int *accum, uchar a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_half_0) accumulator(my_uchar2_half_0_accum) combiner(combiner)
static void my_uchar2_half_0_accum(int *accum, uchar2 a, half b) { }

#pragma rs reduce(my_uchar2_half_1) accumulator(my_uchar2_half_1_accum) combiner(combiner)
static void my_uchar2_half_1_accum(int *accum, uchar2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_half2_0) accumulator(my_uchar2_half2_0_accum) combiner(combiner)
static void my_uchar2_half2_0_accum(int *accum, uchar2 a, half2 b) { }

#pragma rs reduce(my_uchar2_half2_1) accumulator(my_uchar2_half2_1_accum) combiner(combiner)
static void my_uchar2_half2_1_accum(int *accum, uchar2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_half4_0) accumulator(my_uchar2_half4_0_accum) combiner(combiner)
static void my_uchar2_half4_0_accum(int *accum, uchar2 a, half4 b) { }

#pragma rs reduce(my_uchar2_half4_1) accumulator(my_uchar2_half4_1_accum) combiner(combiner)
static void my_uchar2_half4_1_accum(int *accum, uchar2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_float_0) accumulator(my_uchar2_float_0_accum) combiner(combiner)
static void my_uchar2_float_0_accum(int *accum, uchar2 a, float b) { }

#pragma rs reduce(my_uchar2_float_1) accumulator(my_uchar2_float_1_accum) combiner(combiner)
static void my_uchar2_float_1_accum(int *accum, uchar2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_float2_0) accumulator(my_uchar2_float2_0_accum) combiner(combiner)
static void my_uchar2_float2_0_accum(int *accum, uchar2 a, float2 b) { }

#pragma rs reduce(my_uchar2_float2_1) accumulator(my_uchar2_float2_1_accum) combiner(combiner)
static void my_uchar2_float2_1_accum(int *accum, uchar2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_float4_0) accumulator(my_uchar2_float4_0_accum) combiner(combiner)
static void my_uchar2_float4_0_accum(int *accum, uchar2 a, float4 b) { }

#pragma rs reduce(my_uchar2_float4_1) accumulator(my_uchar2_float4_1_accum) combiner(combiner)
static void my_uchar2_float4_1_accum(int *accum, uchar2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_char_0) accumulator(my_uchar2_char_0_accum) combiner(combiner)
static void my_uchar2_char_0_accum(int *accum, uchar2 a, char b) { }

#pragma rs reduce(my_uchar2_char_1) accumulator(my_uchar2_char_1_accum) combiner(combiner)
static void my_uchar2_char_1_accum(int *accum, uchar2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_char2_0) accumulator(my_uchar2_char2_0_accum) combiner(combiner)
static void my_uchar2_char2_0_accum(int *accum, uchar2 a, char2 b) { }

#pragma rs reduce(my_uchar2_char2_1) accumulator(my_uchar2_char2_1_accum) combiner(combiner)
static void my_uchar2_char2_1_accum(int *accum, uchar2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_char4_0) accumulator(my_uchar2_char4_0_accum) combiner(combiner)
static void my_uchar2_char4_0_accum(int *accum, uchar2 a, char4 b) { }

#pragma rs reduce(my_uchar2_char4_1) accumulator(my_uchar2_char4_1_accum) combiner(combiner)
static void my_uchar2_char4_1_accum(int *accum, uchar2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_short_0) accumulator(my_uchar2_short_0_accum) combiner(combiner)
static void my_uchar2_short_0_accum(int *accum, uchar2 a, short b) { }

#pragma rs reduce(my_uchar2_short_1) accumulator(my_uchar2_short_1_accum) combiner(combiner)
static void my_uchar2_short_1_accum(int *accum, uchar2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_short2_0) accumulator(my_uchar2_short2_0_accum) combiner(combiner)
static void my_uchar2_short2_0_accum(int *accum, uchar2 a, short2 b) { }

#pragma rs reduce(my_uchar2_short2_1) accumulator(my_uchar2_short2_1_accum) combiner(combiner)
static void my_uchar2_short2_1_accum(int *accum, uchar2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_short4_0) accumulator(my_uchar2_short4_0_accum) combiner(combiner)
static void my_uchar2_short4_0_accum(int *accum, uchar2 a, short4 b) { }

#pragma rs reduce(my_uchar2_short4_1) accumulator(my_uchar2_short4_1_accum) combiner(combiner)
static void my_uchar2_short4_1_accum(int *accum, uchar2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_uchar_0) accumulator(my_uchar2_uchar_0_accum) combiner(combiner)
static void my_uchar2_uchar_0_accum(int *accum, uchar2 a, uchar b) { }

#pragma rs reduce(my_uchar2_uchar_1) accumulator(my_uchar2_uchar_1_accum) combiner(combiner)
static void my_uchar2_uchar_1_accum(int *accum, uchar2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_uchar2_0) accumulator(my_uchar2_uchar2_0_accum) combiner(combiner)
static void my_uchar2_uchar2_0_accum(int *accum, uchar2 a, uchar2 b) { }

#pragma rs reduce(my_uchar2_uchar2_1) accumulator(my_uchar2_uchar2_1_accum) combiner(combiner)
static void my_uchar2_uchar2_1_accum(int *accum, uchar2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_uchar4_0) accumulator(my_uchar2_uchar4_0_accum) combiner(combiner)
static void my_uchar2_uchar4_0_accum(int *accum, uchar2 a, uchar4 b) { }

#pragma rs reduce(my_uchar2_uchar4_1) accumulator(my_uchar2_uchar4_1_accum) combiner(combiner)
static void my_uchar2_uchar4_1_accum(int *accum, uchar2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_ushort_0) accumulator(my_uchar2_ushort_0_accum) combiner(combiner)
static void my_uchar2_ushort_0_accum(int *accum, uchar2 a, ushort b) { }

#pragma rs reduce(my_uchar2_ushort_1) accumulator(my_uchar2_ushort_1_accum) combiner(combiner)
static void my_uchar2_ushort_1_accum(int *accum, uchar2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_ushort2_0) accumulator(my_uchar2_ushort2_0_accum) combiner(combiner)
static void my_uchar2_ushort2_0_accum(int *accum, uchar2 a, ushort2 b) { }

#pragma rs reduce(my_uchar2_ushort2_1) accumulator(my_uchar2_ushort2_1_accum) combiner(combiner)
static void my_uchar2_ushort2_1_accum(int *accum, uchar2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_ushort4_0) accumulator(my_uchar2_ushort4_0_accum) combiner(combiner)
static void my_uchar2_ushort4_0_accum(int *accum, uchar2 a, ushort4 b) { }

#pragma rs reduce(my_uchar2_ushort4_1) accumulator(my_uchar2_ushort4_1_accum) combiner(combiner)
static void my_uchar2_ushort4_1_accum(int *accum, uchar2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_bool_0) accumulator(my_uchar2_bool_0_accum) combiner(combiner)
static void my_uchar2_bool_0_accum(int *accum, uchar2 a, bool b) { }

#pragma rs reduce(my_uchar2_bool_1) accumulator(my_uchar2_bool_1_accum) combiner(combiner)
static void my_uchar2_bool_1_accum(int *accum, uchar2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_rs_matrix2x2_0) accumulator(my_uchar2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_uchar2_rs_matrix2x2_0_accum(int *accum, uchar2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_uchar2_rs_matrix2x2_1) accumulator(my_uchar2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_uchar2_rs_matrix2x2_1_accum(int *accum, uchar2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_MyStruct_0) accumulator(my_uchar2_MyStruct_0_accum) combiner(combiner)
static void my_uchar2_MyStruct_0_accum(int *accum, uchar2 a, MyStruct b) { }

#pragma rs reduce(my_uchar2_MyStruct_1) accumulator(my_uchar2_MyStruct_1_accum) combiner(combiner)
static void my_uchar2_MyStruct_1_accum(int *accum, uchar2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_half_0) accumulator(my_uchar4_half_0_accum) combiner(combiner)
static void my_uchar4_half_0_accum(int *accum, uchar4 a, half b) { }

#pragma rs reduce(my_uchar4_half_1) accumulator(my_uchar4_half_1_accum) combiner(combiner)
static void my_uchar4_half_1_accum(int *accum, uchar4 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_half2_0) accumulator(my_uchar4_half2_0_accum) combiner(combiner)
static void my_uchar4_half2_0_accum(int *accum, uchar4 a, half2 b) { }

#pragma rs reduce(my_uchar4_half2_1) accumulator(my_uchar4_half2_1_accum) combiner(combiner)
static void my_uchar4_half2_1_accum(int *accum, uchar4 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_half4_0) accumulator(my_uchar4_half4_0_accum) combiner(combiner)
static void my_uchar4_half4_0_accum(int *accum, uchar4 a, half4 b) { }

#pragma rs reduce(my_uchar4_half4_1) accumulator(my_uchar4_half4_1_accum) combiner(combiner)
static void my_uchar4_half4_1_accum(int *accum, uchar4 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_float_0) accumulator(my_uchar4_float_0_accum) combiner(combiner)
static void my_uchar4_float_0_accum(int *accum, uchar4 a, float b) { }

#pragma rs reduce(my_uchar4_float_1) accumulator(my_uchar4_float_1_accum) combiner(combiner)
static void my_uchar4_float_1_accum(int *accum, uchar4 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_float2_0) accumulator(my_uchar4_float2_0_accum) combiner(combiner)
static void my_uchar4_float2_0_accum(int *accum, uchar4 a, float2 b) { }

#pragma rs reduce(my_uchar4_float2_1) accumulator(my_uchar4_float2_1_accum) combiner(combiner)
static void my_uchar4_float2_1_accum(int *accum, uchar4 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_float4_0) accumulator(my_uchar4_float4_0_accum) combiner(combiner)
static void my_uchar4_float4_0_accum(int *accum, uchar4 a, float4 b) { }

#pragma rs reduce(my_uchar4_float4_1) accumulator(my_uchar4_float4_1_accum) combiner(combiner)
static void my_uchar4_float4_1_accum(int *accum, uchar4 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_char_0) accumulator(my_uchar4_char_0_accum) combiner(combiner)
static void my_uchar4_char_0_accum(int *accum, uchar4 a, char b) { }

#pragma rs reduce(my_uchar4_char_1) accumulator(my_uchar4_char_1_accum) combiner(combiner)
static void my_uchar4_char_1_accum(int *accum, uchar4 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_char2_0) accumulator(my_uchar4_char2_0_accum) combiner(combiner)
static void my_uchar4_char2_0_accum(int *accum, uchar4 a, char2 b) { }

#pragma rs reduce(my_uchar4_char2_1) accumulator(my_uchar4_char2_1_accum) combiner(combiner)
static void my_uchar4_char2_1_accum(int *accum, uchar4 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_char4_0) accumulator(my_uchar4_char4_0_accum) combiner(combiner)
static void my_uchar4_char4_0_accum(int *accum, uchar4 a, char4 b) { }

#pragma rs reduce(my_uchar4_char4_1) accumulator(my_uchar4_char4_1_accum) combiner(combiner)
static void my_uchar4_char4_1_accum(int *accum, uchar4 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_short_0) accumulator(my_uchar4_short_0_accum) combiner(combiner)
static void my_uchar4_short_0_accum(int *accum, uchar4 a, short b) { }

#pragma rs reduce(my_uchar4_short_1) accumulator(my_uchar4_short_1_accum) combiner(combiner)
static void my_uchar4_short_1_accum(int *accum, uchar4 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_short2_0) accumulator(my_uchar4_short2_0_accum) combiner(combiner)
static void my_uchar4_short2_0_accum(int *accum, uchar4 a, short2 b) { }

#pragma rs reduce(my_uchar4_short2_1) accumulator(my_uchar4_short2_1_accum) combiner(combiner)
static void my_uchar4_short2_1_accum(int *accum, uchar4 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_short4_0) accumulator(my_uchar4_short4_0_accum) combiner(combiner)
static void my_uchar4_short4_0_accum(int *accum, uchar4 a, short4 b) { }

#pragma rs reduce(my_uchar4_short4_1) accumulator(my_uchar4_short4_1_accum) combiner(combiner)
static void my_uchar4_short4_1_accum(int *accum, uchar4 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_uchar_0) accumulator(my_uchar4_uchar_0_accum) combiner(combiner)
static void my_uchar4_uchar_0_accum(int *accum, uchar4 a, uchar b) { }

#pragma rs reduce(my_uchar4_uchar_1) accumulator(my_uchar4_uchar_1_accum) combiner(combiner)
static void my_uchar4_uchar_1_accum(int *accum, uchar4 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_uchar2_0) accumulator(my_uchar4_uchar2_0_accum) combiner(combiner)
static void my_uchar4_uchar2_0_accum(int *accum, uchar4 a, uchar2 b) { }

#pragma rs reduce(my_uchar4_uchar2_1) accumulator(my_uchar4_uchar2_1_accum) combiner(combiner)
static void my_uchar4_uchar2_1_accum(int *accum, uchar4 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_uchar4_0) accumulator(my_uchar4_uchar4_0_accum) combiner(combiner)
static void my_uchar4_uchar4_0_accum(int *accum, uchar4 a, uchar4 b) { }

#pragma rs reduce(my_uchar4_uchar4_1) accumulator(my_uchar4_uchar4_1_accum) combiner(combiner)
static void my_uchar4_uchar4_1_accum(int *accum, uchar4 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_ushort_0) accumulator(my_uchar4_ushort_0_accum) combiner(combiner)
static void my_uchar4_ushort_0_accum(int *accum, uchar4 a, ushort b) { }

#pragma rs reduce(my_uchar4_ushort_1) accumulator(my_uchar4_ushort_1_accum) combiner(combiner)
static void my_uchar4_ushort_1_accum(int *accum, uchar4 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_ushort2_0) accumulator(my_uchar4_ushort2_0_accum) combiner(combiner)
static void my_uchar4_ushort2_0_accum(int *accum, uchar4 a, ushort2 b) { }

#pragma rs reduce(my_uchar4_ushort2_1) accumulator(my_uchar4_ushort2_1_accum) combiner(combiner)
static void my_uchar4_ushort2_1_accum(int *accum, uchar4 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_ushort4_0) accumulator(my_uchar4_ushort4_0_accum) combiner(combiner)
static void my_uchar4_ushort4_0_accum(int *accum, uchar4 a, ushort4 b) { }

#pragma rs reduce(my_uchar4_ushort4_1) accumulator(my_uchar4_ushort4_1_accum) combiner(combiner)
static void my_uchar4_ushort4_1_accum(int *accum, uchar4 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_bool_0) accumulator(my_uchar4_bool_0_accum) combiner(combiner)
static void my_uchar4_bool_0_accum(int *accum, uchar4 a, bool b) { }

#pragma rs reduce(my_uchar4_bool_1) accumulator(my_uchar4_bool_1_accum) combiner(combiner)
static void my_uchar4_bool_1_accum(int *accum, uchar4 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_rs_matrix2x2_0) accumulator(my_uchar4_rs_matrix2x2_0_accum) combiner(combiner)
static void my_uchar4_rs_matrix2x2_0_accum(int *accum, uchar4 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_uchar4_rs_matrix2x2_1) accumulator(my_uchar4_rs_matrix2x2_1_accum) combiner(combiner)
static void my_uchar4_rs_matrix2x2_1_accum(int *accum, uchar4 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_MyStruct_0) accumulator(my_uchar4_MyStruct_0_accum) combiner(combiner)
static void my_uchar4_MyStruct_0_accum(int *accum, uchar4 a, MyStruct b) { }

#pragma rs reduce(my_uchar4_MyStruct_1) accumulator(my_uchar4_MyStruct_1_accum) combiner(combiner)
static void my_uchar4_MyStruct_1_accum(int *accum, uchar4 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_half_0) accumulator(my_ushort_half_0_accum) combiner(combiner)
static void my_ushort_half_0_accum(int *accum, ushort a, half b) { }

#pragma rs reduce(my_ushort_half_1) accumulator(my_ushort_half_1_accum) combiner(combiner)
static void my_ushort_half_1_accum(int *accum, ushort a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_half2_0) accumulator(my_ushort_half2_0_accum) combiner(combiner)
static void my_ushort_half2_0_accum(int *accum, ushort a, half2 b) { }

#pragma rs reduce(my_ushort_half2_1) accumulator(my_ushort_half2_1_accum) combiner(combiner)
static void my_ushort_half2_1_accum(int *accum, ushort a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_half4_0) accumulator(my_ushort_half4_0_accum) combiner(combiner)
static void my_ushort_half4_0_accum(int *accum, ushort a, half4 b) { }

#pragma rs reduce(my_ushort_half4_1) accumulator(my_ushort_half4_1_accum) combiner(combiner)
static void my_ushort_half4_1_accum(int *accum, ushort a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_float_0) accumulator(my_ushort_float_0_accum) combiner(combiner)
static void my_ushort_float_0_accum(int *accum, ushort a, float b) { }

#pragma rs reduce(my_ushort_float_1) accumulator(my_ushort_float_1_accum) combiner(combiner)
static void my_ushort_float_1_accum(int *accum, ushort a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_float2_0) accumulator(my_ushort_float2_0_accum) combiner(combiner)
static void my_ushort_float2_0_accum(int *accum, ushort a, float2 b) { }

#pragma rs reduce(my_ushort_float2_1) accumulator(my_ushort_float2_1_accum) combiner(combiner)
static void my_ushort_float2_1_accum(int *accum, ushort a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_float4_0) accumulator(my_ushort_float4_0_accum) combiner(combiner)
static void my_ushort_float4_0_accum(int *accum, ushort a, float4 b) { }

#pragma rs reduce(my_ushort_float4_1) accumulator(my_ushort_float4_1_accum) combiner(combiner)
static void my_ushort_float4_1_accum(int *accum, ushort a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_char_0) accumulator(my_ushort_char_0_accum) combiner(combiner)
static void my_ushort_char_0_accum(int *accum, ushort a, char b) { }

#pragma rs reduce(my_ushort_char_1) accumulator(my_ushort_char_1_accum) combiner(combiner)
static void my_ushort_char_1_accum(int *accum, ushort a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_char2_0) accumulator(my_ushort_char2_0_accum) combiner(combiner)
static void my_ushort_char2_0_accum(int *accum, ushort a, char2 b) { }

#pragma rs reduce(my_ushort_char2_1) accumulator(my_ushort_char2_1_accum) combiner(combiner)
static void my_ushort_char2_1_accum(int *accum, ushort a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_char4_0) accumulator(my_ushort_char4_0_accum) combiner(combiner)
static void my_ushort_char4_0_accum(int *accum, ushort a, char4 b) { }

#pragma rs reduce(my_ushort_char4_1) accumulator(my_ushort_char4_1_accum) combiner(combiner)
static void my_ushort_char4_1_accum(int *accum, ushort a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_short_0) accumulator(my_ushort_short_0_accum) combiner(combiner)
static void my_ushort_short_0_accum(int *accum, ushort a, short b) { }

#pragma rs reduce(my_ushort_short_1) accumulator(my_ushort_short_1_accum) combiner(combiner)
static void my_ushort_short_1_accum(int *accum, ushort a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_short2_0) accumulator(my_ushort_short2_0_accum) combiner(combiner)
static void my_ushort_short2_0_accum(int *accum, ushort a, short2 b) { }

#pragma rs reduce(my_ushort_short2_1) accumulator(my_ushort_short2_1_accum) combiner(combiner)
static void my_ushort_short2_1_accum(int *accum, ushort a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_short4_0) accumulator(my_ushort_short4_0_accum) combiner(combiner)
static void my_ushort_short4_0_accum(int *accum, ushort a, short4 b) { }

#pragma rs reduce(my_ushort_short4_1) accumulator(my_ushort_short4_1_accum) combiner(combiner)
static void my_ushort_short4_1_accum(int *accum, ushort a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_uchar_0) accumulator(my_ushort_uchar_0_accum) combiner(combiner)
static void my_ushort_uchar_0_accum(int *accum, ushort a, uchar b) { }

#pragma rs reduce(my_ushort_uchar_1) accumulator(my_ushort_uchar_1_accum) combiner(combiner)
static void my_ushort_uchar_1_accum(int *accum, ushort a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_uchar2_0) accumulator(my_ushort_uchar2_0_accum) combiner(combiner)
static void my_ushort_uchar2_0_accum(int *accum, ushort a, uchar2 b) { }

#pragma rs reduce(my_ushort_uchar2_1) accumulator(my_ushort_uchar2_1_accum) combiner(combiner)
static void my_ushort_uchar2_1_accum(int *accum, ushort a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_uchar4_0) accumulator(my_ushort_uchar4_0_accum) combiner(combiner)
static void my_ushort_uchar4_0_accum(int *accum, ushort a, uchar4 b) { }

#pragma rs reduce(my_ushort_uchar4_1) accumulator(my_ushort_uchar4_1_accum) combiner(combiner)
static void my_ushort_uchar4_1_accum(int *accum, ushort a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_ushort_0) accumulator(my_ushort_ushort_0_accum) combiner(combiner)
static void my_ushort_ushort_0_accum(int *accum, ushort a, ushort b) { }

#pragma rs reduce(my_ushort_ushort_1) accumulator(my_ushort_ushort_1_accum) combiner(combiner)
static void my_ushort_ushort_1_accum(int *accum, ushort a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_ushort2_0) accumulator(my_ushort_ushort2_0_accum) combiner(combiner)
static void my_ushort_ushort2_0_accum(int *accum, ushort a, ushort2 b) { }

#pragma rs reduce(my_ushort_ushort2_1) accumulator(my_ushort_ushort2_1_accum) combiner(combiner)
static void my_ushort_ushort2_1_accum(int *accum, ushort a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_ushort4_0) accumulator(my_ushort_ushort4_0_accum) combiner(combiner)
static void my_ushort_ushort4_0_accum(int *accum, ushort a, ushort4 b) { }

#pragma rs reduce(my_ushort_ushort4_1) accumulator(my_ushort_ushort4_1_accum) combiner(combiner)
static void my_ushort_ushort4_1_accum(int *accum, ushort a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_bool_0) accumulator(my_ushort_bool_0_accum) combiner(combiner)
static void my_ushort_bool_0_accum(int *accum, ushort a, bool b) { }

#pragma rs reduce(my_ushort_bool_1) accumulator(my_ushort_bool_1_accum) combiner(combiner)
static void my_ushort_bool_1_accum(int *accum, ushort a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_rs_matrix2x2_0) accumulator(my_ushort_rs_matrix2x2_0_accum) combiner(combiner)
static void my_ushort_rs_matrix2x2_0_accum(int *accum, ushort a, rs_matrix2x2 b) { }

#pragma rs reduce(my_ushort_rs_matrix2x2_1) accumulator(my_ushort_rs_matrix2x2_1_accum) combiner(combiner)
static void my_ushort_rs_matrix2x2_1_accum(int *accum, ushort a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_MyStruct_0) accumulator(my_ushort_MyStruct_0_accum) combiner(combiner)
static void my_ushort_MyStruct_0_accum(int *accum, ushort a, MyStruct b) { }

#pragma rs reduce(my_ushort_MyStruct_1) accumulator(my_ushort_MyStruct_1_accum) combiner(combiner)
static void my_ushort_MyStruct_1_accum(int *accum, ushort a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_half_0) accumulator(my_ushort2_half_0_accum) combiner(combiner)
static void my_ushort2_half_0_accum(int *accum, ushort2 a, half b) { }

#pragma rs reduce(my_ushort2_half_1) accumulator(my_ushort2_half_1_accum) combiner(combiner)
static void my_ushort2_half_1_accum(int *accum, ushort2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_half2_0) accumulator(my_ushort2_half2_0_accum) combiner(combiner)
static void my_ushort2_half2_0_accum(int *accum, ushort2 a, half2 b) { }

#pragma rs reduce(my_ushort2_half2_1) accumulator(my_ushort2_half2_1_accum) combiner(combiner)
static void my_ushort2_half2_1_accum(int *accum, ushort2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_half4_0) accumulator(my_ushort2_half4_0_accum) combiner(combiner)
static void my_ushort2_half4_0_accum(int *accum, ushort2 a, half4 b) { }

#pragma rs reduce(my_ushort2_half4_1) accumulator(my_ushort2_half4_1_accum) combiner(combiner)
static void my_ushort2_half4_1_accum(int *accum, ushort2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_float_0) accumulator(my_ushort2_float_0_accum) combiner(combiner)
static void my_ushort2_float_0_accum(int *accum, ushort2 a, float b) { }

#pragma rs reduce(my_ushort2_float_1) accumulator(my_ushort2_float_1_accum) combiner(combiner)
static void my_ushort2_float_1_accum(int *accum, ushort2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_float2_0) accumulator(my_ushort2_float2_0_accum) combiner(combiner)
static void my_ushort2_float2_0_accum(int *accum, ushort2 a, float2 b) { }

#pragma rs reduce(my_ushort2_float2_1) accumulator(my_ushort2_float2_1_accum) combiner(combiner)
static void my_ushort2_float2_1_accum(int *accum, ushort2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_float4_0) accumulator(my_ushort2_float4_0_accum) combiner(combiner)
static void my_ushort2_float4_0_accum(int *accum, ushort2 a, float4 b) { }

#pragma rs reduce(my_ushort2_float4_1) accumulator(my_ushort2_float4_1_accum) combiner(combiner)
static void my_ushort2_float4_1_accum(int *accum, ushort2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_char_0) accumulator(my_ushort2_char_0_accum) combiner(combiner)
static void my_ushort2_char_0_accum(int *accum, ushort2 a, char b) { }

#pragma rs reduce(my_ushort2_char_1) accumulator(my_ushort2_char_1_accum) combiner(combiner)
static void my_ushort2_char_1_accum(int *accum, ushort2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_char2_0) accumulator(my_ushort2_char2_0_accum) combiner(combiner)
static void my_ushort2_char2_0_accum(int *accum, ushort2 a, char2 b) { }

#pragma rs reduce(my_ushort2_char2_1) accumulator(my_ushort2_char2_1_accum) combiner(combiner)
static void my_ushort2_char2_1_accum(int *accum, ushort2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_char4_0) accumulator(my_ushort2_char4_0_accum) combiner(combiner)
static void my_ushort2_char4_0_accum(int *accum, ushort2 a, char4 b) { }

#pragma rs reduce(my_ushort2_char4_1) accumulator(my_ushort2_char4_1_accum) combiner(combiner)
static void my_ushort2_char4_1_accum(int *accum, ushort2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_short_0) accumulator(my_ushort2_short_0_accum) combiner(combiner)
static void my_ushort2_short_0_accum(int *accum, ushort2 a, short b) { }

#pragma rs reduce(my_ushort2_short_1) accumulator(my_ushort2_short_1_accum) combiner(combiner)
static void my_ushort2_short_1_accum(int *accum, ushort2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_short2_0) accumulator(my_ushort2_short2_0_accum) combiner(combiner)
static void my_ushort2_short2_0_accum(int *accum, ushort2 a, short2 b) { }

#pragma rs reduce(my_ushort2_short2_1) accumulator(my_ushort2_short2_1_accum) combiner(combiner)
static void my_ushort2_short2_1_accum(int *accum, ushort2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_short4_0) accumulator(my_ushort2_short4_0_accum) combiner(combiner)
static void my_ushort2_short4_0_accum(int *accum, ushort2 a, short4 b) { }

#pragma rs reduce(my_ushort2_short4_1) accumulator(my_ushort2_short4_1_accum) combiner(combiner)
static void my_ushort2_short4_1_accum(int *accum, ushort2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_uchar_0) accumulator(my_ushort2_uchar_0_accum) combiner(combiner)
static void my_ushort2_uchar_0_accum(int *accum, ushort2 a, uchar b) { }

#pragma rs reduce(my_ushort2_uchar_1) accumulator(my_ushort2_uchar_1_accum) combiner(combiner)
static void my_ushort2_uchar_1_accum(int *accum, ushort2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_uchar2_0) accumulator(my_ushort2_uchar2_0_accum) combiner(combiner)
static void my_ushort2_uchar2_0_accum(int *accum, ushort2 a, uchar2 b) { }

#pragma rs reduce(my_ushort2_uchar2_1) accumulator(my_ushort2_uchar2_1_accum) combiner(combiner)
static void my_ushort2_uchar2_1_accum(int *accum, ushort2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_uchar4_0) accumulator(my_ushort2_uchar4_0_accum) combiner(combiner)
static void my_ushort2_uchar4_0_accum(int *accum, ushort2 a, uchar4 b) { }

#pragma rs reduce(my_ushort2_uchar4_1) accumulator(my_ushort2_uchar4_1_accum) combiner(combiner)
static void my_ushort2_uchar4_1_accum(int *accum, ushort2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_ushort_0) accumulator(my_ushort2_ushort_0_accum) combiner(combiner)
static void my_ushort2_ushort_0_accum(int *accum, ushort2 a, ushort b) { }

#pragma rs reduce(my_ushort2_ushort_1) accumulator(my_ushort2_ushort_1_accum) combiner(combiner)
static void my_ushort2_ushort_1_accum(int *accum, ushort2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_ushort2_0) accumulator(my_ushort2_ushort2_0_accum) combiner(combiner)
static void my_ushort2_ushort2_0_accum(int *accum, ushort2 a, ushort2 b) { }

#pragma rs reduce(my_ushort2_ushort2_1) accumulator(my_ushort2_ushort2_1_accum) combiner(combiner)
static void my_ushort2_ushort2_1_accum(int *accum, ushort2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_ushort4_0) accumulator(my_ushort2_ushort4_0_accum) combiner(combiner)
static void my_ushort2_ushort4_0_accum(int *accum, ushort2 a, ushort4 b) { }

#pragma rs reduce(my_ushort2_ushort4_1) accumulator(my_ushort2_ushort4_1_accum) combiner(combiner)
static void my_ushort2_ushort4_1_accum(int *accum, ushort2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_bool_0) accumulator(my_ushort2_bool_0_accum) combiner(combiner)
static void my_ushort2_bool_0_accum(int *accum, ushort2 a, bool b) { }

#pragma rs reduce(my_ushort2_bool_1) accumulator(my_ushort2_bool_1_accum) combiner(combiner)
static void my_ushort2_bool_1_accum(int *accum, ushort2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_rs_matrix2x2_0) accumulator(my_ushort2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_ushort2_rs_matrix2x2_0_accum(int *accum, ushort2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_ushort2_rs_matrix2x2_1) accumulator(my_ushort2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_ushort2_rs_matrix2x2_1_accum(int *accum, ushort2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_MyStruct_0) accumulator(my_ushort2_MyStruct_0_accum) combiner(combiner)
static void my_ushort2_MyStruct_0_accum(int *accum, ushort2 a, MyStruct b) { }

#pragma rs reduce(my_ushort2_MyStruct_1) accumulator(my_ushort2_MyStruct_1_accum) combiner(combiner)
static void my_ushort2_MyStruct_1_accum(int *accum, ushort2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_half_0) accumulator(my_ushort4_half_0_accum) combiner(combiner)
static void my_ushort4_half_0_accum(int *accum, ushort4 a, half b) { }

#pragma rs reduce(my_ushort4_half_1) accumulator(my_ushort4_half_1_accum) combiner(combiner)
static void my_ushort4_half_1_accum(int *accum, ushort4 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_half2_0) accumulator(my_ushort4_half2_0_accum) combiner(combiner)
static void my_ushort4_half2_0_accum(int *accum, ushort4 a, half2 b) { }

#pragma rs reduce(my_ushort4_half2_1) accumulator(my_ushort4_half2_1_accum) combiner(combiner)
static void my_ushort4_half2_1_accum(int *accum, ushort4 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_half4_0) accumulator(my_ushort4_half4_0_accum) combiner(combiner)
static void my_ushort4_half4_0_accum(int *accum, ushort4 a, half4 b) { }

#pragma rs reduce(my_ushort4_half4_1) accumulator(my_ushort4_half4_1_accum) combiner(combiner)
static void my_ushort4_half4_1_accum(int *accum, ushort4 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_float_0) accumulator(my_ushort4_float_0_accum) combiner(combiner)
static void my_ushort4_float_0_accum(int *accum, ushort4 a, float b) { }

#pragma rs reduce(my_ushort4_float_1) accumulator(my_ushort4_float_1_accum) combiner(combiner)
static void my_ushort4_float_1_accum(int *accum, ushort4 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_float2_0) accumulator(my_ushort4_float2_0_accum) combiner(combiner)
static void my_ushort4_float2_0_accum(int *accum, ushort4 a, float2 b) { }

#pragma rs reduce(my_ushort4_float2_1) accumulator(my_ushort4_float2_1_accum) combiner(combiner)
static void my_ushort4_float2_1_accum(int *accum, ushort4 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_float4_0) accumulator(my_ushort4_float4_0_accum) combiner(combiner)
static void my_ushort4_float4_0_accum(int *accum, ushort4 a, float4 b) { }

#pragma rs reduce(my_ushort4_float4_1) accumulator(my_ushort4_float4_1_accum) combiner(combiner)
static void my_ushort4_float4_1_accum(int *accum, ushort4 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_char_0) accumulator(my_ushort4_char_0_accum) combiner(combiner)
static void my_ushort4_char_0_accum(int *accum, ushort4 a, char b) { }

#pragma rs reduce(my_ushort4_char_1) accumulator(my_ushort4_char_1_accum) combiner(combiner)
static void my_ushort4_char_1_accum(int *accum, ushort4 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_char2_0) accumulator(my_ushort4_char2_0_accum) combiner(combiner)
static void my_ushort4_char2_0_accum(int *accum, ushort4 a, char2 b) { }

#pragma rs reduce(my_ushort4_char2_1) accumulator(my_ushort4_char2_1_accum) combiner(combiner)
static void my_ushort4_char2_1_accum(int *accum, ushort4 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_char4_0) accumulator(my_ushort4_char4_0_accum) combiner(combiner)
static void my_ushort4_char4_0_accum(int *accum, ushort4 a, char4 b) { }

#pragma rs reduce(my_ushort4_char4_1) accumulator(my_ushort4_char4_1_accum) combiner(combiner)
static void my_ushort4_char4_1_accum(int *accum, ushort4 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_short_0) accumulator(my_ushort4_short_0_accum) combiner(combiner)
static void my_ushort4_short_0_accum(int *accum, ushort4 a, short b) { }

#pragma rs reduce(my_ushort4_short_1) accumulator(my_ushort4_short_1_accum) combiner(combiner)
static void my_ushort4_short_1_accum(int *accum, ushort4 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_short2_0) accumulator(my_ushort4_short2_0_accum) combiner(combiner)
static void my_ushort4_short2_0_accum(int *accum, ushort4 a, short2 b) { }

#pragma rs reduce(my_ushort4_short2_1) accumulator(my_ushort4_short2_1_accum) combiner(combiner)
static void my_ushort4_short2_1_accum(int *accum, ushort4 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_short4_0) accumulator(my_ushort4_short4_0_accum) combiner(combiner)
static void my_ushort4_short4_0_accum(int *accum, ushort4 a, short4 b) { }

#pragma rs reduce(my_ushort4_short4_1) accumulator(my_ushort4_short4_1_accum) combiner(combiner)
static void my_ushort4_short4_1_accum(int *accum, ushort4 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_uchar_0) accumulator(my_ushort4_uchar_0_accum) combiner(combiner)
static void my_ushort4_uchar_0_accum(int *accum, ushort4 a, uchar b) { }

#pragma rs reduce(my_ushort4_uchar_1) accumulator(my_ushort4_uchar_1_accum) combiner(combiner)
static void my_ushort4_uchar_1_accum(int *accum, ushort4 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_uchar2_0) accumulator(my_ushort4_uchar2_0_accum) combiner(combiner)
static void my_ushort4_uchar2_0_accum(int *accum, ushort4 a, uchar2 b) { }

#pragma rs reduce(my_ushort4_uchar2_1) accumulator(my_ushort4_uchar2_1_accum) combiner(combiner)
static void my_ushort4_uchar2_1_accum(int *accum, ushort4 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_uchar4_0) accumulator(my_ushort4_uchar4_0_accum) combiner(combiner)
static void my_ushort4_uchar4_0_accum(int *accum, ushort4 a, uchar4 b) { }

#pragma rs reduce(my_ushort4_uchar4_1) accumulator(my_ushort4_uchar4_1_accum) combiner(combiner)
static void my_ushort4_uchar4_1_accum(int *accum, ushort4 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_ushort_0) accumulator(my_ushort4_ushort_0_accum) combiner(combiner)
static void my_ushort4_ushort_0_accum(int *accum, ushort4 a, ushort b) { }

#pragma rs reduce(my_ushort4_ushort_1) accumulator(my_ushort4_ushort_1_accum) combiner(combiner)
static void my_ushort4_ushort_1_accum(int *accum, ushort4 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_ushort2_0) accumulator(my_ushort4_ushort2_0_accum) combiner(combiner)
static void my_ushort4_ushort2_0_accum(int *accum, ushort4 a, ushort2 b) { }

#pragma rs reduce(my_ushort4_ushort2_1) accumulator(my_ushort4_ushort2_1_accum) combiner(combiner)
static void my_ushort4_ushort2_1_accum(int *accum, ushort4 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_ushort4_0) accumulator(my_ushort4_ushort4_0_accum) combiner(combiner)
static void my_ushort4_ushort4_0_accum(int *accum, ushort4 a, ushort4 b) { }

#pragma rs reduce(my_ushort4_ushort4_1) accumulator(my_ushort4_ushort4_1_accum) combiner(combiner)
static void my_ushort4_ushort4_1_accum(int *accum, ushort4 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_bool_0) accumulator(my_ushort4_bool_0_accum) combiner(combiner)
static void my_ushort4_bool_0_accum(int *accum, ushort4 a, bool b) { }

#pragma rs reduce(my_ushort4_bool_1) accumulator(my_ushort4_bool_1_accum) combiner(combiner)
static void my_ushort4_bool_1_accum(int *accum, ushort4 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_rs_matrix2x2_0) accumulator(my_ushort4_rs_matrix2x2_0_accum) combiner(combiner)
static void my_ushort4_rs_matrix2x2_0_accum(int *accum, ushort4 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_ushort4_rs_matrix2x2_1) accumulator(my_ushort4_rs_matrix2x2_1_accum) combiner(combiner)
static void my_ushort4_rs_matrix2x2_1_accum(int *accum, ushort4 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_MyStruct_0) accumulator(my_ushort4_MyStruct_0_accum) combiner(combiner)
static void my_ushort4_MyStruct_0_accum(int *accum, ushort4 a, MyStruct b) { }

#pragma rs reduce(my_ushort4_MyStruct_1) accumulator(my_ushort4_MyStruct_1_accum) combiner(combiner)
static void my_ushort4_MyStruct_1_accum(int *accum, ushort4 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_half_0) accumulator(my_bool_half_0_accum) combiner(combiner)
static void my_bool_half_0_accum(int *accum, bool a, half b) { }

#pragma rs reduce(my_bool_half_1) accumulator(my_bool_half_1_accum) combiner(combiner)
static void my_bool_half_1_accum(int *accum, bool a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_half2_0) accumulator(my_bool_half2_0_accum) combiner(combiner)
static void my_bool_half2_0_accum(int *accum, bool a, half2 b) { }

#pragma rs reduce(my_bool_half2_1) accumulator(my_bool_half2_1_accum) combiner(combiner)
static void my_bool_half2_1_accum(int *accum, bool a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_half4_0) accumulator(my_bool_half4_0_accum) combiner(combiner)
static void my_bool_half4_0_accum(int *accum, bool a, half4 b) { }

#pragma rs reduce(my_bool_half4_1) accumulator(my_bool_half4_1_accum) combiner(combiner)
static void my_bool_half4_1_accum(int *accum, bool a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_float_0) accumulator(my_bool_float_0_accum) combiner(combiner)
static void my_bool_float_0_accum(int *accum, bool a, float b) { }

#pragma rs reduce(my_bool_float_1) accumulator(my_bool_float_1_accum) combiner(combiner)
static void my_bool_float_1_accum(int *accum, bool a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_float2_0) accumulator(my_bool_float2_0_accum) combiner(combiner)
static void my_bool_float2_0_accum(int *accum, bool a, float2 b) { }

#pragma rs reduce(my_bool_float2_1) accumulator(my_bool_float2_1_accum) combiner(combiner)
static void my_bool_float2_1_accum(int *accum, bool a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_float4_0) accumulator(my_bool_float4_0_accum) combiner(combiner)
static void my_bool_float4_0_accum(int *accum, bool a, float4 b) { }

#pragma rs reduce(my_bool_float4_1) accumulator(my_bool_float4_1_accum) combiner(combiner)
static void my_bool_float4_1_accum(int *accum, bool a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_char_0) accumulator(my_bool_char_0_accum) combiner(combiner)
static void my_bool_char_0_accum(int *accum, bool a, char b) { }

#pragma rs reduce(my_bool_char_1) accumulator(my_bool_char_1_accum) combiner(combiner)
static void my_bool_char_1_accum(int *accum, bool a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_char2_0) accumulator(my_bool_char2_0_accum) combiner(combiner)
static void my_bool_char2_0_accum(int *accum, bool a, char2 b) { }

#pragma rs reduce(my_bool_char2_1) accumulator(my_bool_char2_1_accum) combiner(combiner)
static void my_bool_char2_1_accum(int *accum, bool a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_char4_0) accumulator(my_bool_char4_0_accum) combiner(combiner)
static void my_bool_char4_0_accum(int *accum, bool a, char4 b) { }

#pragma rs reduce(my_bool_char4_1) accumulator(my_bool_char4_1_accum) combiner(combiner)
static void my_bool_char4_1_accum(int *accum, bool a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_short_0) accumulator(my_bool_short_0_accum) combiner(combiner)
static void my_bool_short_0_accum(int *accum, bool a, short b) { }

#pragma rs reduce(my_bool_short_1) accumulator(my_bool_short_1_accum) combiner(combiner)
static void my_bool_short_1_accum(int *accum, bool a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_short2_0) accumulator(my_bool_short2_0_accum) combiner(combiner)
static void my_bool_short2_0_accum(int *accum, bool a, short2 b) { }

#pragma rs reduce(my_bool_short2_1) accumulator(my_bool_short2_1_accum) combiner(combiner)
static void my_bool_short2_1_accum(int *accum, bool a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_short4_0) accumulator(my_bool_short4_0_accum) combiner(combiner)
static void my_bool_short4_0_accum(int *accum, bool a, short4 b) { }

#pragma rs reduce(my_bool_short4_1) accumulator(my_bool_short4_1_accum) combiner(combiner)
static void my_bool_short4_1_accum(int *accum, bool a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_uchar_0) accumulator(my_bool_uchar_0_accum) combiner(combiner)
static void my_bool_uchar_0_accum(int *accum, bool a, uchar b) { }

#pragma rs reduce(my_bool_uchar_1) accumulator(my_bool_uchar_1_accum) combiner(combiner)
static void my_bool_uchar_1_accum(int *accum, bool a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_uchar2_0) accumulator(my_bool_uchar2_0_accum) combiner(combiner)
static void my_bool_uchar2_0_accum(int *accum, bool a, uchar2 b) { }

#pragma rs reduce(my_bool_uchar2_1) accumulator(my_bool_uchar2_1_accum) combiner(combiner)
static void my_bool_uchar2_1_accum(int *accum, bool a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_uchar4_0) accumulator(my_bool_uchar4_0_accum) combiner(combiner)
static void my_bool_uchar4_0_accum(int *accum, bool a, uchar4 b) { }

#pragma rs reduce(my_bool_uchar4_1) accumulator(my_bool_uchar4_1_accum) combiner(combiner)
static void my_bool_uchar4_1_accum(int *accum, bool a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_ushort_0) accumulator(my_bool_ushort_0_accum) combiner(combiner)
static void my_bool_ushort_0_accum(int *accum, bool a, ushort b) { }

#pragma rs reduce(my_bool_ushort_1) accumulator(my_bool_ushort_1_accum) combiner(combiner)
static void my_bool_ushort_1_accum(int *accum, bool a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_ushort2_0) accumulator(my_bool_ushort2_0_accum) combiner(combiner)
static void my_bool_ushort2_0_accum(int *accum, bool a, ushort2 b) { }

#pragma rs reduce(my_bool_ushort2_1) accumulator(my_bool_ushort2_1_accum) combiner(combiner)
static void my_bool_ushort2_1_accum(int *accum, bool a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_ushort4_0) accumulator(my_bool_ushort4_0_accum) combiner(combiner)
static void my_bool_ushort4_0_accum(int *accum, bool a, ushort4 b) { }

#pragma rs reduce(my_bool_ushort4_1) accumulator(my_bool_ushort4_1_accum) combiner(combiner)
static void my_bool_ushort4_1_accum(int *accum, bool a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_bool_0) accumulator(my_bool_bool_0_accum) combiner(combiner)
static void my_bool_bool_0_accum(int *accum, bool a, bool b) { }

#pragma rs reduce(my_bool_bool_1) accumulator(my_bool_bool_1_accum) combiner(combiner)
static void my_bool_bool_1_accum(int *accum, bool a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_rs_matrix2x2_0) accumulator(my_bool_rs_matrix2x2_0_accum) combiner(combiner)
static void my_bool_rs_matrix2x2_0_accum(int *accum, bool a, rs_matrix2x2 b) { }

#pragma rs reduce(my_bool_rs_matrix2x2_1) accumulator(my_bool_rs_matrix2x2_1_accum) combiner(combiner)
static void my_bool_rs_matrix2x2_1_accum(int *accum, bool a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_bool_MyStruct_0) accumulator(my_bool_MyStruct_0_accum) combiner(combiner)
static void my_bool_MyStruct_0_accum(int *accum, bool a, MyStruct b) { }

#pragma rs reduce(my_bool_MyStruct_1) accumulator(my_bool_MyStruct_1_accum) combiner(combiner)
static void my_bool_MyStruct_1_accum(int *accum, bool a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_half_0) accumulator(my_rs_matrix2x2_half_0_accum) combiner(combiner)
static void my_rs_matrix2x2_half_0_accum(int *accum, rs_matrix2x2 a, half b) { }

#pragma rs reduce(my_rs_matrix2x2_half_1) accumulator(my_rs_matrix2x2_half_1_accum) combiner(combiner)
static void my_rs_matrix2x2_half_1_accum(int *accum, rs_matrix2x2 a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_half2_0) accumulator(my_rs_matrix2x2_half2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_half2_0_accum(int *accum, rs_matrix2x2 a, half2 b) { }

#pragma rs reduce(my_rs_matrix2x2_half2_1) accumulator(my_rs_matrix2x2_half2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_half2_1_accum(int *accum, rs_matrix2x2 a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_half4_0) accumulator(my_rs_matrix2x2_half4_0_accum) combiner(combiner)
static void my_rs_matrix2x2_half4_0_accum(int *accum, rs_matrix2x2 a, half4 b) { }

#pragma rs reduce(my_rs_matrix2x2_half4_1) accumulator(my_rs_matrix2x2_half4_1_accum) combiner(combiner)
static void my_rs_matrix2x2_half4_1_accum(int *accum, rs_matrix2x2 a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_float_0) accumulator(my_rs_matrix2x2_float_0_accum) combiner(combiner)
static void my_rs_matrix2x2_float_0_accum(int *accum, rs_matrix2x2 a, float b) { }

#pragma rs reduce(my_rs_matrix2x2_float_1) accumulator(my_rs_matrix2x2_float_1_accum) combiner(combiner)
static void my_rs_matrix2x2_float_1_accum(int *accum, rs_matrix2x2 a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_float2_0) accumulator(my_rs_matrix2x2_float2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_float2_0_accum(int *accum, rs_matrix2x2 a, float2 b) { }

#pragma rs reduce(my_rs_matrix2x2_float2_1) accumulator(my_rs_matrix2x2_float2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_float2_1_accum(int *accum, rs_matrix2x2 a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_float4_0) accumulator(my_rs_matrix2x2_float4_0_accum) combiner(combiner)
static void my_rs_matrix2x2_float4_0_accum(int *accum, rs_matrix2x2 a, float4 b) { }

#pragma rs reduce(my_rs_matrix2x2_float4_1) accumulator(my_rs_matrix2x2_float4_1_accum) combiner(combiner)
static void my_rs_matrix2x2_float4_1_accum(int *accum, rs_matrix2x2 a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_char_0) accumulator(my_rs_matrix2x2_char_0_accum) combiner(combiner)
static void my_rs_matrix2x2_char_0_accum(int *accum, rs_matrix2x2 a, char b) { }

#pragma rs reduce(my_rs_matrix2x2_char_1) accumulator(my_rs_matrix2x2_char_1_accum) combiner(combiner)
static void my_rs_matrix2x2_char_1_accum(int *accum, rs_matrix2x2 a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_char2_0) accumulator(my_rs_matrix2x2_char2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_char2_0_accum(int *accum, rs_matrix2x2 a, char2 b) { }

#pragma rs reduce(my_rs_matrix2x2_char2_1) accumulator(my_rs_matrix2x2_char2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_char2_1_accum(int *accum, rs_matrix2x2 a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_char4_0) accumulator(my_rs_matrix2x2_char4_0_accum) combiner(combiner)
static void my_rs_matrix2x2_char4_0_accum(int *accum, rs_matrix2x2 a, char4 b) { }

#pragma rs reduce(my_rs_matrix2x2_char4_1) accumulator(my_rs_matrix2x2_char4_1_accum) combiner(combiner)
static void my_rs_matrix2x2_char4_1_accum(int *accum, rs_matrix2x2 a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_short_0) accumulator(my_rs_matrix2x2_short_0_accum) combiner(combiner)
static void my_rs_matrix2x2_short_0_accum(int *accum, rs_matrix2x2 a, short b) { }

#pragma rs reduce(my_rs_matrix2x2_short_1) accumulator(my_rs_matrix2x2_short_1_accum) combiner(combiner)
static void my_rs_matrix2x2_short_1_accum(int *accum, rs_matrix2x2 a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_short2_0) accumulator(my_rs_matrix2x2_short2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_short2_0_accum(int *accum, rs_matrix2x2 a, short2 b) { }

#pragma rs reduce(my_rs_matrix2x2_short2_1) accumulator(my_rs_matrix2x2_short2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_short2_1_accum(int *accum, rs_matrix2x2 a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_short4_0) accumulator(my_rs_matrix2x2_short4_0_accum) combiner(combiner)
static void my_rs_matrix2x2_short4_0_accum(int *accum, rs_matrix2x2 a, short4 b) { }

#pragma rs reduce(my_rs_matrix2x2_short4_1) accumulator(my_rs_matrix2x2_short4_1_accum) combiner(combiner)
static void my_rs_matrix2x2_short4_1_accum(int *accum, rs_matrix2x2 a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_uchar_0) accumulator(my_rs_matrix2x2_uchar_0_accum) combiner(combiner)
static void my_rs_matrix2x2_uchar_0_accum(int *accum, rs_matrix2x2 a, uchar b) { }

#pragma rs reduce(my_rs_matrix2x2_uchar_1) accumulator(my_rs_matrix2x2_uchar_1_accum) combiner(combiner)
static void my_rs_matrix2x2_uchar_1_accum(int *accum, rs_matrix2x2 a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_uchar2_0) accumulator(my_rs_matrix2x2_uchar2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_uchar2_0_accum(int *accum, rs_matrix2x2 a, uchar2 b) { }

#pragma rs reduce(my_rs_matrix2x2_uchar2_1) accumulator(my_rs_matrix2x2_uchar2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_uchar2_1_accum(int *accum, rs_matrix2x2 a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_uchar4_0) accumulator(my_rs_matrix2x2_uchar4_0_accum) combiner(combiner)
static void my_rs_matrix2x2_uchar4_0_accum(int *accum, rs_matrix2x2 a, uchar4 b) { }

#pragma rs reduce(my_rs_matrix2x2_uchar4_1) accumulator(my_rs_matrix2x2_uchar4_1_accum) combiner(combiner)
static void my_rs_matrix2x2_uchar4_1_accum(int *accum, rs_matrix2x2 a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_ushort_0) accumulator(my_rs_matrix2x2_ushort_0_accum) combiner(combiner)
static void my_rs_matrix2x2_ushort_0_accum(int *accum, rs_matrix2x2 a, ushort b) { }

#pragma rs reduce(my_rs_matrix2x2_ushort_1) accumulator(my_rs_matrix2x2_ushort_1_accum) combiner(combiner)
static void my_rs_matrix2x2_ushort_1_accum(int *accum, rs_matrix2x2 a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_ushort2_0) accumulator(my_rs_matrix2x2_ushort2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_ushort2_0_accum(int *accum, rs_matrix2x2 a, ushort2 b) { }

#pragma rs reduce(my_rs_matrix2x2_ushort2_1) accumulator(my_rs_matrix2x2_ushort2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_ushort2_1_accum(int *accum, rs_matrix2x2 a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_ushort4_0) accumulator(my_rs_matrix2x2_ushort4_0_accum) combiner(combiner)
static void my_rs_matrix2x2_ushort4_0_accum(int *accum, rs_matrix2x2 a, ushort4 b) { }

#pragma rs reduce(my_rs_matrix2x2_ushort4_1) accumulator(my_rs_matrix2x2_ushort4_1_accum) combiner(combiner)
static void my_rs_matrix2x2_ushort4_1_accum(int *accum, rs_matrix2x2 a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_bool_0) accumulator(my_rs_matrix2x2_bool_0_accum) combiner(combiner)
static void my_rs_matrix2x2_bool_0_accum(int *accum, rs_matrix2x2 a, bool b) { }

#pragma rs reduce(my_rs_matrix2x2_bool_1) accumulator(my_rs_matrix2x2_bool_1_accum) combiner(combiner)
static void my_rs_matrix2x2_bool_1_accum(int *accum, rs_matrix2x2 a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_rs_matrix2x2_0) accumulator(my_rs_matrix2x2_rs_matrix2x2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_rs_matrix2x2_0_accum(int *accum, rs_matrix2x2 a, rs_matrix2x2 b) { }

#pragma rs reduce(my_rs_matrix2x2_rs_matrix2x2_1) accumulator(my_rs_matrix2x2_rs_matrix2x2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_rs_matrix2x2_1_accum(int *accum, rs_matrix2x2 a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_MyStruct_0) accumulator(my_rs_matrix2x2_MyStruct_0_accum) combiner(combiner)
static void my_rs_matrix2x2_MyStruct_0_accum(int *accum, rs_matrix2x2 a, MyStruct b) { }

#pragma rs reduce(my_rs_matrix2x2_MyStruct_1) accumulator(my_rs_matrix2x2_MyStruct_1_accum) combiner(combiner)
static void my_rs_matrix2x2_MyStruct_1_accum(int *accum, rs_matrix2x2 a, MyStruct b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_half_0) accumulator(my_MyStruct_half_0_accum) combiner(combiner)
static void my_MyStruct_half_0_accum(int *accum, MyStruct a, half b) { }

#pragma rs reduce(my_MyStruct_half_1) accumulator(my_MyStruct_half_1_accum) combiner(combiner)
static void my_MyStruct_half_1_accum(int *accum, MyStruct a, half b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_half2_0) accumulator(my_MyStruct_half2_0_accum) combiner(combiner)
static void my_MyStruct_half2_0_accum(int *accum, MyStruct a, half2 b) { }

#pragma rs reduce(my_MyStruct_half2_1) accumulator(my_MyStruct_half2_1_accum) combiner(combiner)
static void my_MyStruct_half2_1_accum(int *accum, MyStruct a, half2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_half4_0) accumulator(my_MyStruct_half4_0_accum) combiner(combiner)
static void my_MyStruct_half4_0_accum(int *accum, MyStruct a, half4 b) { }

#pragma rs reduce(my_MyStruct_half4_1) accumulator(my_MyStruct_half4_1_accum) combiner(combiner)
static void my_MyStruct_half4_1_accum(int *accum, MyStruct a, half4 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_float_0) accumulator(my_MyStruct_float_0_accum) combiner(combiner)
static void my_MyStruct_float_0_accum(int *accum, MyStruct a, float b) { }

#pragma rs reduce(my_MyStruct_float_1) accumulator(my_MyStruct_float_1_accum) combiner(combiner)
static void my_MyStruct_float_1_accum(int *accum, MyStruct a, float b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_float2_0) accumulator(my_MyStruct_float2_0_accum) combiner(combiner)
static void my_MyStruct_float2_0_accum(int *accum, MyStruct a, float2 b) { }

#pragma rs reduce(my_MyStruct_float2_1) accumulator(my_MyStruct_float2_1_accum) combiner(combiner)
static void my_MyStruct_float2_1_accum(int *accum, MyStruct a, float2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_float4_0) accumulator(my_MyStruct_float4_0_accum) combiner(combiner)
static void my_MyStruct_float4_0_accum(int *accum, MyStruct a, float4 b) { }

#pragma rs reduce(my_MyStruct_float4_1) accumulator(my_MyStruct_float4_1_accum) combiner(combiner)
static void my_MyStruct_float4_1_accum(int *accum, MyStruct a, float4 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_char_0) accumulator(my_MyStruct_char_0_accum) combiner(combiner)
static void my_MyStruct_char_0_accum(int *accum, MyStruct a, char b) { }

#pragma rs reduce(my_MyStruct_char_1) accumulator(my_MyStruct_char_1_accum) combiner(combiner)
static void my_MyStruct_char_1_accum(int *accum, MyStruct a, char b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_char2_0) accumulator(my_MyStruct_char2_0_accum) combiner(combiner)
static void my_MyStruct_char2_0_accum(int *accum, MyStruct a, char2 b) { }

#pragma rs reduce(my_MyStruct_char2_1) accumulator(my_MyStruct_char2_1_accum) combiner(combiner)
static void my_MyStruct_char2_1_accum(int *accum, MyStruct a, char2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_char4_0) accumulator(my_MyStruct_char4_0_accum) combiner(combiner)
static void my_MyStruct_char4_0_accum(int *accum, MyStruct a, char4 b) { }

#pragma rs reduce(my_MyStruct_char4_1) accumulator(my_MyStruct_char4_1_accum) combiner(combiner)
static void my_MyStruct_char4_1_accum(int *accum, MyStruct a, char4 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_short_0) accumulator(my_MyStruct_short_0_accum) combiner(combiner)
static void my_MyStruct_short_0_accum(int *accum, MyStruct a, short b) { }

#pragma rs reduce(my_MyStruct_short_1) accumulator(my_MyStruct_short_1_accum) combiner(combiner)
static void my_MyStruct_short_1_accum(int *accum, MyStruct a, short b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_short2_0) accumulator(my_MyStruct_short2_0_accum) combiner(combiner)
static void my_MyStruct_short2_0_accum(int *accum, MyStruct a, short2 b) { }

#pragma rs reduce(my_MyStruct_short2_1) accumulator(my_MyStruct_short2_1_accum) combiner(combiner)
static void my_MyStruct_short2_1_accum(int *accum, MyStruct a, short2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_short4_0) accumulator(my_MyStruct_short4_0_accum) combiner(combiner)
static void my_MyStruct_short4_0_accum(int *accum, MyStruct a, short4 b) { }

#pragma rs reduce(my_MyStruct_short4_1) accumulator(my_MyStruct_short4_1_accum) combiner(combiner)
static void my_MyStruct_short4_1_accum(int *accum, MyStruct a, short4 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_uchar_0) accumulator(my_MyStruct_uchar_0_accum) combiner(combiner)
static void my_MyStruct_uchar_0_accum(int *accum, MyStruct a, uchar b) { }

#pragma rs reduce(my_MyStruct_uchar_1) accumulator(my_MyStruct_uchar_1_accum) combiner(combiner)
static void my_MyStruct_uchar_1_accum(int *accum, MyStruct a, uchar b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_uchar2_0) accumulator(my_MyStruct_uchar2_0_accum) combiner(combiner)
static void my_MyStruct_uchar2_0_accum(int *accum, MyStruct a, uchar2 b) { }

#pragma rs reduce(my_MyStruct_uchar2_1) accumulator(my_MyStruct_uchar2_1_accum) combiner(combiner)
static void my_MyStruct_uchar2_1_accum(int *accum, MyStruct a, uchar2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_uchar4_0) accumulator(my_MyStruct_uchar4_0_accum) combiner(combiner)
static void my_MyStruct_uchar4_0_accum(int *accum, MyStruct a, uchar4 b) { }

#pragma rs reduce(my_MyStruct_uchar4_1) accumulator(my_MyStruct_uchar4_1_accum) combiner(combiner)
static void my_MyStruct_uchar4_1_accum(int *accum, MyStruct a, uchar4 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_ushort_0) accumulator(my_MyStruct_ushort_0_accum) combiner(combiner)
static void my_MyStruct_ushort_0_accum(int *accum, MyStruct a, ushort b) { }

#pragma rs reduce(my_MyStruct_ushort_1) accumulator(my_MyStruct_ushort_1_accum) combiner(combiner)
static void my_MyStruct_ushort_1_accum(int *accum, MyStruct a, ushort b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_ushort2_0) accumulator(my_MyStruct_ushort2_0_accum) combiner(combiner)
static void my_MyStruct_ushort2_0_accum(int *accum, MyStruct a, ushort2 b) { }

#pragma rs reduce(my_MyStruct_ushort2_1) accumulator(my_MyStruct_ushort2_1_accum) combiner(combiner)
static void my_MyStruct_ushort2_1_accum(int *accum, MyStruct a, ushort2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_ushort4_0) accumulator(my_MyStruct_ushort4_0_accum) combiner(combiner)
static void my_MyStruct_ushort4_0_accum(int *accum, MyStruct a, ushort4 b) { }

#pragma rs reduce(my_MyStruct_ushort4_1) accumulator(my_MyStruct_ushort4_1_accum) combiner(combiner)
static void my_MyStruct_ushort4_1_accum(int *accum, MyStruct a, ushort4 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_bool_0) accumulator(my_MyStruct_bool_0_accum) combiner(combiner)
static void my_MyStruct_bool_0_accum(int *accum, MyStruct a, bool b) { }

#pragma rs reduce(my_MyStruct_bool_1) accumulator(my_MyStruct_bool_1_accum) combiner(combiner)
static void my_MyStruct_bool_1_accum(int *accum, MyStruct a, bool b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_rs_matrix2x2_0) accumulator(my_MyStruct_rs_matrix2x2_0_accum) combiner(combiner)
static void my_MyStruct_rs_matrix2x2_0_accum(int *accum, MyStruct a, rs_matrix2x2 b) { }

#pragma rs reduce(my_MyStruct_rs_matrix2x2_1) accumulator(my_MyStruct_rs_matrix2x2_1_accum) combiner(combiner)
static void my_MyStruct_rs_matrix2x2_1_accum(int *accum, MyStruct a, rs_matrix2x2 b, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_MyStruct_0) accumulator(my_MyStruct_MyStruct_0_accum) combiner(combiner)
static void my_MyStruct_MyStruct_0_accum(int *accum, MyStruct a, MyStruct b) { }

#pragma rs reduce(my_MyStruct_MyStruct_1) accumulator(my_MyStruct_MyStruct_1_accum) combiner(combiner)
static void my_MyStruct_MyStruct_1_accum(int *accum, MyStruct a, MyStruct b, rs_kernel_context context) { }
