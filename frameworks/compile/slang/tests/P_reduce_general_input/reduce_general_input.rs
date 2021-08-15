// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(input)

// This test case was created by ./gen-input.pl.
// It exercises various legal combinations of inputs and special parameters,
//  so that we can ensure
// (a) We do not choke when compiling them
// (b) We reflect them correctly

// One example struct type
typedef struct MyStruct { float f; double d; } MyStruct;

// Trivial combiner shared by all test cases
static void combiner(int *accum, const int *other) { }

#pragma rs reduce(my_half_0) accumulator(my_half_0_accum) combiner(combiner)
static void my_half_0_accum(int *accum, half in) { }

#pragma rs reduce(my_half_1) accumulator(my_half_1_accum) combiner(combiner)
static void my_half_1_accum(int *accum, half in, rs_kernel_context context) { }

#pragma rs reduce(my_half_2) accumulator(my_half_2_accum) combiner(combiner)
static void my_half_2_accum(int *accum, half in, uint x) { }

#pragma rs reduce(my_half_3) accumulator(my_half_3_accum) combiner(combiner)
static void my_half_3_accum(int *accum, half in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_half_4) accumulator(my_half_4_accum) combiner(combiner)
static void my_half_4_accum(int *accum, half in, uint y) { }

#pragma rs reduce(my_half_5) accumulator(my_half_5_accum) combiner(combiner)
static void my_half_5_accum(int *accum, half in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_half_6) accumulator(my_half_6_accum) combiner(combiner)
static void my_half_6_accum(int *accum, half in, uint x, uint y) { }

#pragma rs reduce(my_half_7) accumulator(my_half_7_accum) combiner(combiner)
static void my_half_7_accum(int *accum, half in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_half_8) accumulator(my_half_8_accum) combiner(combiner)
static void my_half_8_accum(int *accum, half in, uint z) { }

#pragma rs reduce(my_half_9) accumulator(my_half_9_accum) combiner(combiner)
static void my_half_9_accum(int *accum, half in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_half_10) accumulator(my_half_10_accum) combiner(combiner)
static void my_half_10_accum(int *accum, half in, uint x, uint z) { }

#pragma rs reduce(my_half_11) accumulator(my_half_11_accum) combiner(combiner)
static void my_half_11_accum(int *accum, half in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_half_12) accumulator(my_half_12_accum) combiner(combiner)
static void my_half_12_accum(int *accum, half in, uint y, uint z) { }

#pragma rs reduce(my_half_13) accumulator(my_half_13_accum) combiner(combiner)
static void my_half_13_accum(int *accum, half in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_half_14) accumulator(my_half_14_accum) combiner(combiner)
static void my_half_14_accum(int *accum, half in, uint x, uint y, uint z) { }

#pragma rs reduce(my_half_15) accumulator(my_half_15_accum) combiner(combiner)
static void my_half_15_accum(int *accum, half in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_half2_0) accumulator(my_half2_0_accum) combiner(combiner)
static void my_half2_0_accum(int *accum, half2 in) { }

#pragma rs reduce(my_half2_1) accumulator(my_half2_1_accum) combiner(combiner)
static void my_half2_1_accum(int *accum, half2 in, rs_kernel_context context) { }

#pragma rs reduce(my_half2_2) accumulator(my_half2_2_accum) combiner(combiner)
static void my_half2_2_accum(int *accum, half2 in, uint x) { }

#pragma rs reduce(my_half2_3) accumulator(my_half2_3_accum) combiner(combiner)
static void my_half2_3_accum(int *accum, half2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_half2_4) accumulator(my_half2_4_accum) combiner(combiner)
static void my_half2_4_accum(int *accum, half2 in, uint y) { }

#pragma rs reduce(my_half2_5) accumulator(my_half2_5_accum) combiner(combiner)
static void my_half2_5_accum(int *accum, half2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_half2_6) accumulator(my_half2_6_accum) combiner(combiner)
static void my_half2_6_accum(int *accum, half2 in, uint x, uint y) { }

#pragma rs reduce(my_half2_7) accumulator(my_half2_7_accum) combiner(combiner)
static void my_half2_7_accum(int *accum, half2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_half2_8) accumulator(my_half2_8_accum) combiner(combiner)
static void my_half2_8_accum(int *accum, half2 in, uint z) { }

#pragma rs reduce(my_half2_9) accumulator(my_half2_9_accum) combiner(combiner)
static void my_half2_9_accum(int *accum, half2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_half2_10) accumulator(my_half2_10_accum) combiner(combiner)
static void my_half2_10_accum(int *accum, half2 in, uint x, uint z) { }

#pragma rs reduce(my_half2_11) accumulator(my_half2_11_accum) combiner(combiner)
static void my_half2_11_accum(int *accum, half2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_half2_12) accumulator(my_half2_12_accum) combiner(combiner)
static void my_half2_12_accum(int *accum, half2 in, uint y, uint z) { }

#pragma rs reduce(my_half2_13) accumulator(my_half2_13_accum) combiner(combiner)
static void my_half2_13_accum(int *accum, half2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_half2_14) accumulator(my_half2_14_accum) combiner(combiner)
static void my_half2_14_accum(int *accum, half2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_half2_15) accumulator(my_half2_15_accum) combiner(combiner)
static void my_half2_15_accum(int *accum, half2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_half4_0) accumulator(my_half4_0_accum) combiner(combiner)
static void my_half4_0_accum(int *accum, half4 in) { }

#pragma rs reduce(my_half4_1) accumulator(my_half4_1_accum) combiner(combiner)
static void my_half4_1_accum(int *accum, half4 in, rs_kernel_context context) { }

#pragma rs reduce(my_half4_2) accumulator(my_half4_2_accum) combiner(combiner)
static void my_half4_2_accum(int *accum, half4 in, uint x) { }

#pragma rs reduce(my_half4_3) accumulator(my_half4_3_accum) combiner(combiner)
static void my_half4_3_accum(int *accum, half4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_half4_4) accumulator(my_half4_4_accum) combiner(combiner)
static void my_half4_4_accum(int *accum, half4 in, uint y) { }

#pragma rs reduce(my_half4_5) accumulator(my_half4_5_accum) combiner(combiner)
static void my_half4_5_accum(int *accum, half4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_half4_6) accumulator(my_half4_6_accum) combiner(combiner)
static void my_half4_6_accum(int *accum, half4 in, uint x, uint y) { }

#pragma rs reduce(my_half4_7) accumulator(my_half4_7_accum) combiner(combiner)
static void my_half4_7_accum(int *accum, half4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_half4_8) accumulator(my_half4_8_accum) combiner(combiner)
static void my_half4_8_accum(int *accum, half4 in, uint z) { }

#pragma rs reduce(my_half4_9) accumulator(my_half4_9_accum) combiner(combiner)
static void my_half4_9_accum(int *accum, half4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_half4_10) accumulator(my_half4_10_accum) combiner(combiner)
static void my_half4_10_accum(int *accum, half4 in, uint x, uint z) { }

#pragma rs reduce(my_half4_11) accumulator(my_half4_11_accum) combiner(combiner)
static void my_half4_11_accum(int *accum, half4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_half4_12) accumulator(my_half4_12_accum) combiner(combiner)
static void my_half4_12_accum(int *accum, half4 in, uint y, uint z) { }

#pragma rs reduce(my_half4_13) accumulator(my_half4_13_accum) combiner(combiner)
static void my_half4_13_accum(int *accum, half4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_half4_14) accumulator(my_half4_14_accum) combiner(combiner)
static void my_half4_14_accum(int *accum, half4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_half4_15) accumulator(my_half4_15_accum) combiner(combiner)
static void my_half4_15_accum(int *accum, half4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_float_0) accumulator(my_float_0_accum) combiner(combiner)
static void my_float_0_accum(int *accum, float in) { }

#pragma rs reduce(my_float_1) accumulator(my_float_1_accum) combiner(combiner)
static void my_float_1_accum(int *accum, float in, rs_kernel_context context) { }

#pragma rs reduce(my_float_2) accumulator(my_float_2_accum) combiner(combiner)
static void my_float_2_accum(int *accum, float in, uint x) { }

#pragma rs reduce(my_float_3) accumulator(my_float_3_accum) combiner(combiner)
static void my_float_3_accum(int *accum, float in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_float_4) accumulator(my_float_4_accum) combiner(combiner)
static void my_float_4_accum(int *accum, float in, uint y) { }

#pragma rs reduce(my_float_5) accumulator(my_float_5_accum) combiner(combiner)
static void my_float_5_accum(int *accum, float in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_float_6) accumulator(my_float_6_accum) combiner(combiner)
static void my_float_6_accum(int *accum, float in, uint x, uint y) { }

#pragma rs reduce(my_float_7) accumulator(my_float_7_accum) combiner(combiner)
static void my_float_7_accum(int *accum, float in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_float_8) accumulator(my_float_8_accum) combiner(combiner)
static void my_float_8_accum(int *accum, float in, uint z) { }

#pragma rs reduce(my_float_9) accumulator(my_float_9_accum) combiner(combiner)
static void my_float_9_accum(int *accum, float in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_float_10) accumulator(my_float_10_accum) combiner(combiner)
static void my_float_10_accum(int *accum, float in, uint x, uint z) { }

#pragma rs reduce(my_float_11) accumulator(my_float_11_accum) combiner(combiner)
static void my_float_11_accum(int *accum, float in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_float_12) accumulator(my_float_12_accum) combiner(combiner)
static void my_float_12_accum(int *accum, float in, uint y, uint z) { }

#pragma rs reduce(my_float_13) accumulator(my_float_13_accum) combiner(combiner)
static void my_float_13_accum(int *accum, float in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_float_14) accumulator(my_float_14_accum) combiner(combiner)
static void my_float_14_accum(int *accum, float in, uint x, uint y, uint z) { }

#pragma rs reduce(my_float_15) accumulator(my_float_15_accum) combiner(combiner)
static void my_float_15_accum(int *accum, float in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_float2_0) accumulator(my_float2_0_accum) combiner(combiner)
static void my_float2_0_accum(int *accum, float2 in) { }

#pragma rs reduce(my_float2_1) accumulator(my_float2_1_accum) combiner(combiner)
static void my_float2_1_accum(int *accum, float2 in, rs_kernel_context context) { }

#pragma rs reduce(my_float2_2) accumulator(my_float2_2_accum) combiner(combiner)
static void my_float2_2_accum(int *accum, float2 in, uint x) { }

#pragma rs reduce(my_float2_3) accumulator(my_float2_3_accum) combiner(combiner)
static void my_float2_3_accum(int *accum, float2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_float2_4) accumulator(my_float2_4_accum) combiner(combiner)
static void my_float2_4_accum(int *accum, float2 in, uint y) { }

#pragma rs reduce(my_float2_5) accumulator(my_float2_5_accum) combiner(combiner)
static void my_float2_5_accum(int *accum, float2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_float2_6) accumulator(my_float2_6_accum) combiner(combiner)
static void my_float2_6_accum(int *accum, float2 in, uint x, uint y) { }

#pragma rs reduce(my_float2_7) accumulator(my_float2_7_accum) combiner(combiner)
static void my_float2_7_accum(int *accum, float2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_float2_8) accumulator(my_float2_8_accum) combiner(combiner)
static void my_float2_8_accum(int *accum, float2 in, uint z) { }

#pragma rs reduce(my_float2_9) accumulator(my_float2_9_accum) combiner(combiner)
static void my_float2_9_accum(int *accum, float2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_float2_10) accumulator(my_float2_10_accum) combiner(combiner)
static void my_float2_10_accum(int *accum, float2 in, uint x, uint z) { }

#pragma rs reduce(my_float2_11) accumulator(my_float2_11_accum) combiner(combiner)
static void my_float2_11_accum(int *accum, float2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_float2_12) accumulator(my_float2_12_accum) combiner(combiner)
static void my_float2_12_accum(int *accum, float2 in, uint y, uint z) { }

#pragma rs reduce(my_float2_13) accumulator(my_float2_13_accum) combiner(combiner)
static void my_float2_13_accum(int *accum, float2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_float2_14) accumulator(my_float2_14_accum) combiner(combiner)
static void my_float2_14_accum(int *accum, float2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_float2_15) accumulator(my_float2_15_accum) combiner(combiner)
static void my_float2_15_accum(int *accum, float2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_float4_0) accumulator(my_float4_0_accum) combiner(combiner)
static void my_float4_0_accum(int *accum, float4 in) { }

#pragma rs reduce(my_float4_1) accumulator(my_float4_1_accum) combiner(combiner)
static void my_float4_1_accum(int *accum, float4 in, rs_kernel_context context) { }

#pragma rs reduce(my_float4_2) accumulator(my_float4_2_accum) combiner(combiner)
static void my_float4_2_accum(int *accum, float4 in, uint x) { }

#pragma rs reduce(my_float4_3) accumulator(my_float4_3_accum) combiner(combiner)
static void my_float4_3_accum(int *accum, float4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_float4_4) accumulator(my_float4_4_accum) combiner(combiner)
static void my_float4_4_accum(int *accum, float4 in, uint y) { }

#pragma rs reduce(my_float4_5) accumulator(my_float4_5_accum) combiner(combiner)
static void my_float4_5_accum(int *accum, float4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_float4_6) accumulator(my_float4_6_accum) combiner(combiner)
static void my_float4_6_accum(int *accum, float4 in, uint x, uint y) { }

#pragma rs reduce(my_float4_7) accumulator(my_float4_7_accum) combiner(combiner)
static void my_float4_7_accum(int *accum, float4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_float4_8) accumulator(my_float4_8_accum) combiner(combiner)
static void my_float4_8_accum(int *accum, float4 in, uint z) { }

#pragma rs reduce(my_float4_9) accumulator(my_float4_9_accum) combiner(combiner)
static void my_float4_9_accum(int *accum, float4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_float4_10) accumulator(my_float4_10_accum) combiner(combiner)
static void my_float4_10_accum(int *accum, float4 in, uint x, uint z) { }

#pragma rs reduce(my_float4_11) accumulator(my_float4_11_accum) combiner(combiner)
static void my_float4_11_accum(int *accum, float4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_float4_12) accumulator(my_float4_12_accum) combiner(combiner)
static void my_float4_12_accum(int *accum, float4 in, uint y, uint z) { }

#pragma rs reduce(my_float4_13) accumulator(my_float4_13_accum) combiner(combiner)
static void my_float4_13_accum(int *accum, float4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_float4_14) accumulator(my_float4_14_accum) combiner(combiner)
static void my_float4_14_accum(int *accum, float4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_float4_15) accumulator(my_float4_15_accum) combiner(combiner)
static void my_float4_15_accum(int *accum, float4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_double_0) accumulator(my_double_0_accum) combiner(combiner)
static void my_double_0_accum(int *accum, double in) { }

#pragma rs reduce(my_double_1) accumulator(my_double_1_accum) combiner(combiner)
static void my_double_1_accum(int *accum, double in, rs_kernel_context context) { }

#pragma rs reduce(my_double_2) accumulator(my_double_2_accum) combiner(combiner)
static void my_double_2_accum(int *accum, double in, uint x) { }

#pragma rs reduce(my_double_3) accumulator(my_double_3_accum) combiner(combiner)
static void my_double_3_accum(int *accum, double in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_double_4) accumulator(my_double_4_accum) combiner(combiner)
static void my_double_4_accum(int *accum, double in, uint y) { }

#pragma rs reduce(my_double_5) accumulator(my_double_5_accum) combiner(combiner)
static void my_double_5_accum(int *accum, double in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_double_6) accumulator(my_double_6_accum) combiner(combiner)
static void my_double_6_accum(int *accum, double in, uint x, uint y) { }

#pragma rs reduce(my_double_7) accumulator(my_double_7_accum) combiner(combiner)
static void my_double_7_accum(int *accum, double in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_double_8) accumulator(my_double_8_accum) combiner(combiner)
static void my_double_8_accum(int *accum, double in, uint z) { }

#pragma rs reduce(my_double_9) accumulator(my_double_9_accum) combiner(combiner)
static void my_double_9_accum(int *accum, double in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_double_10) accumulator(my_double_10_accum) combiner(combiner)
static void my_double_10_accum(int *accum, double in, uint x, uint z) { }

#pragma rs reduce(my_double_11) accumulator(my_double_11_accum) combiner(combiner)
static void my_double_11_accum(int *accum, double in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_double_12) accumulator(my_double_12_accum) combiner(combiner)
static void my_double_12_accum(int *accum, double in, uint y, uint z) { }

#pragma rs reduce(my_double_13) accumulator(my_double_13_accum) combiner(combiner)
static void my_double_13_accum(int *accum, double in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_double_14) accumulator(my_double_14_accum) combiner(combiner)
static void my_double_14_accum(int *accum, double in, uint x, uint y, uint z) { }

#pragma rs reduce(my_double_15) accumulator(my_double_15_accum) combiner(combiner)
static void my_double_15_accum(int *accum, double in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_double2_0) accumulator(my_double2_0_accum) combiner(combiner)
static void my_double2_0_accum(int *accum, double2 in) { }

#pragma rs reduce(my_double2_1) accumulator(my_double2_1_accum) combiner(combiner)
static void my_double2_1_accum(int *accum, double2 in, rs_kernel_context context) { }

#pragma rs reduce(my_double2_2) accumulator(my_double2_2_accum) combiner(combiner)
static void my_double2_2_accum(int *accum, double2 in, uint x) { }

#pragma rs reduce(my_double2_3) accumulator(my_double2_3_accum) combiner(combiner)
static void my_double2_3_accum(int *accum, double2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_double2_4) accumulator(my_double2_4_accum) combiner(combiner)
static void my_double2_4_accum(int *accum, double2 in, uint y) { }

#pragma rs reduce(my_double2_5) accumulator(my_double2_5_accum) combiner(combiner)
static void my_double2_5_accum(int *accum, double2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_double2_6) accumulator(my_double2_6_accum) combiner(combiner)
static void my_double2_6_accum(int *accum, double2 in, uint x, uint y) { }

#pragma rs reduce(my_double2_7) accumulator(my_double2_7_accum) combiner(combiner)
static void my_double2_7_accum(int *accum, double2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_double2_8) accumulator(my_double2_8_accum) combiner(combiner)
static void my_double2_8_accum(int *accum, double2 in, uint z) { }

#pragma rs reduce(my_double2_9) accumulator(my_double2_9_accum) combiner(combiner)
static void my_double2_9_accum(int *accum, double2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_double2_10) accumulator(my_double2_10_accum) combiner(combiner)
static void my_double2_10_accum(int *accum, double2 in, uint x, uint z) { }

#pragma rs reduce(my_double2_11) accumulator(my_double2_11_accum) combiner(combiner)
static void my_double2_11_accum(int *accum, double2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_double2_12) accumulator(my_double2_12_accum) combiner(combiner)
static void my_double2_12_accum(int *accum, double2 in, uint y, uint z) { }

#pragma rs reduce(my_double2_13) accumulator(my_double2_13_accum) combiner(combiner)
static void my_double2_13_accum(int *accum, double2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_double2_14) accumulator(my_double2_14_accum) combiner(combiner)
static void my_double2_14_accum(int *accum, double2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_double2_15) accumulator(my_double2_15_accum) combiner(combiner)
static void my_double2_15_accum(int *accum, double2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_double4_0) accumulator(my_double4_0_accum) combiner(combiner)
static void my_double4_0_accum(int *accum, double4 in) { }

#pragma rs reduce(my_double4_1) accumulator(my_double4_1_accum) combiner(combiner)
static void my_double4_1_accum(int *accum, double4 in, rs_kernel_context context) { }

#pragma rs reduce(my_double4_2) accumulator(my_double4_2_accum) combiner(combiner)
static void my_double4_2_accum(int *accum, double4 in, uint x) { }

#pragma rs reduce(my_double4_3) accumulator(my_double4_3_accum) combiner(combiner)
static void my_double4_3_accum(int *accum, double4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_double4_4) accumulator(my_double4_4_accum) combiner(combiner)
static void my_double4_4_accum(int *accum, double4 in, uint y) { }

#pragma rs reduce(my_double4_5) accumulator(my_double4_5_accum) combiner(combiner)
static void my_double4_5_accum(int *accum, double4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_double4_6) accumulator(my_double4_6_accum) combiner(combiner)
static void my_double4_6_accum(int *accum, double4 in, uint x, uint y) { }

#pragma rs reduce(my_double4_7) accumulator(my_double4_7_accum) combiner(combiner)
static void my_double4_7_accum(int *accum, double4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_double4_8) accumulator(my_double4_8_accum) combiner(combiner)
static void my_double4_8_accum(int *accum, double4 in, uint z) { }

#pragma rs reduce(my_double4_9) accumulator(my_double4_9_accum) combiner(combiner)
static void my_double4_9_accum(int *accum, double4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_double4_10) accumulator(my_double4_10_accum) combiner(combiner)
static void my_double4_10_accum(int *accum, double4 in, uint x, uint z) { }

#pragma rs reduce(my_double4_11) accumulator(my_double4_11_accum) combiner(combiner)
static void my_double4_11_accum(int *accum, double4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_double4_12) accumulator(my_double4_12_accum) combiner(combiner)
static void my_double4_12_accum(int *accum, double4 in, uint y, uint z) { }

#pragma rs reduce(my_double4_13) accumulator(my_double4_13_accum) combiner(combiner)
static void my_double4_13_accum(int *accum, double4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_double4_14) accumulator(my_double4_14_accum) combiner(combiner)
static void my_double4_14_accum(int *accum, double4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_double4_15) accumulator(my_double4_15_accum) combiner(combiner)
static void my_double4_15_accum(int *accum, double4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_char_0) accumulator(my_char_0_accum) combiner(combiner)
static void my_char_0_accum(int *accum, char in) { }

#pragma rs reduce(my_char_1) accumulator(my_char_1_accum) combiner(combiner)
static void my_char_1_accum(int *accum, char in, rs_kernel_context context) { }

#pragma rs reduce(my_char_2) accumulator(my_char_2_accum) combiner(combiner)
static void my_char_2_accum(int *accum, char in, uint x) { }

#pragma rs reduce(my_char_3) accumulator(my_char_3_accum) combiner(combiner)
static void my_char_3_accum(int *accum, char in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_char_4) accumulator(my_char_4_accum) combiner(combiner)
static void my_char_4_accum(int *accum, char in, uint y) { }

#pragma rs reduce(my_char_5) accumulator(my_char_5_accum) combiner(combiner)
static void my_char_5_accum(int *accum, char in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_char_6) accumulator(my_char_6_accum) combiner(combiner)
static void my_char_6_accum(int *accum, char in, uint x, uint y) { }

#pragma rs reduce(my_char_7) accumulator(my_char_7_accum) combiner(combiner)
static void my_char_7_accum(int *accum, char in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_char_8) accumulator(my_char_8_accum) combiner(combiner)
static void my_char_8_accum(int *accum, char in, uint z) { }

#pragma rs reduce(my_char_9) accumulator(my_char_9_accum) combiner(combiner)
static void my_char_9_accum(int *accum, char in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_char_10) accumulator(my_char_10_accum) combiner(combiner)
static void my_char_10_accum(int *accum, char in, uint x, uint z) { }

#pragma rs reduce(my_char_11) accumulator(my_char_11_accum) combiner(combiner)
static void my_char_11_accum(int *accum, char in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_char_12) accumulator(my_char_12_accum) combiner(combiner)
static void my_char_12_accum(int *accum, char in, uint y, uint z) { }

#pragma rs reduce(my_char_13) accumulator(my_char_13_accum) combiner(combiner)
static void my_char_13_accum(int *accum, char in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_char_14) accumulator(my_char_14_accum) combiner(combiner)
static void my_char_14_accum(int *accum, char in, uint x, uint y, uint z) { }

#pragma rs reduce(my_char_15) accumulator(my_char_15_accum) combiner(combiner)
static void my_char_15_accum(int *accum, char in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_char2_0) accumulator(my_char2_0_accum) combiner(combiner)
static void my_char2_0_accum(int *accum, char2 in) { }

#pragma rs reduce(my_char2_1) accumulator(my_char2_1_accum) combiner(combiner)
static void my_char2_1_accum(int *accum, char2 in, rs_kernel_context context) { }

#pragma rs reduce(my_char2_2) accumulator(my_char2_2_accum) combiner(combiner)
static void my_char2_2_accum(int *accum, char2 in, uint x) { }

#pragma rs reduce(my_char2_3) accumulator(my_char2_3_accum) combiner(combiner)
static void my_char2_3_accum(int *accum, char2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_char2_4) accumulator(my_char2_4_accum) combiner(combiner)
static void my_char2_4_accum(int *accum, char2 in, uint y) { }

#pragma rs reduce(my_char2_5) accumulator(my_char2_5_accum) combiner(combiner)
static void my_char2_5_accum(int *accum, char2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_char2_6) accumulator(my_char2_6_accum) combiner(combiner)
static void my_char2_6_accum(int *accum, char2 in, uint x, uint y) { }

#pragma rs reduce(my_char2_7) accumulator(my_char2_7_accum) combiner(combiner)
static void my_char2_7_accum(int *accum, char2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_char2_8) accumulator(my_char2_8_accum) combiner(combiner)
static void my_char2_8_accum(int *accum, char2 in, uint z) { }

#pragma rs reduce(my_char2_9) accumulator(my_char2_9_accum) combiner(combiner)
static void my_char2_9_accum(int *accum, char2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_char2_10) accumulator(my_char2_10_accum) combiner(combiner)
static void my_char2_10_accum(int *accum, char2 in, uint x, uint z) { }

#pragma rs reduce(my_char2_11) accumulator(my_char2_11_accum) combiner(combiner)
static void my_char2_11_accum(int *accum, char2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_char2_12) accumulator(my_char2_12_accum) combiner(combiner)
static void my_char2_12_accum(int *accum, char2 in, uint y, uint z) { }

#pragma rs reduce(my_char2_13) accumulator(my_char2_13_accum) combiner(combiner)
static void my_char2_13_accum(int *accum, char2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_char2_14) accumulator(my_char2_14_accum) combiner(combiner)
static void my_char2_14_accum(int *accum, char2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_char2_15) accumulator(my_char2_15_accum) combiner(combiner)
static void my_char2_15_accum(int *accum, char2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_char4_0) accumulator(my_char4_0_accum) combiner(combiner)
static void my_char4_0_accum(int *accum, char4 in) { }

#pragma rs reduce(my_char4_1) accumulator(my_char4_1_accum) combiner(combiner)
static void my_char4_1_accum(int *accum, char4 in, rs_kernel_context context) { }

#pragma rs reduce(my_char4_2) accumulator(my_char4_2_accum) combiner(combiner)
static void my_char4_2_accum(int *accum, char4 in, uint x) { }

#pragma rs reduce(my_char4_3) accumulator(my_char4_3_accum) combiner(combiner)
static void my_char4_3_accum(int *accum, char4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_char4_4) accumulator(my_char4_4_accum) combiner(combiner)
static void my_char4_4_accum(int *accum, char4 in, uint y) { }

#pragma rs reduce(my_char4_5) accumulator(my_char4_5_accum) combiner(combiner)
static void my_char4_5_accum(int *accum, char4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_char4_6) accumulator(my_char4_6_accum) combiner(combiner)
static void my_char4_6_accum(int *accum, char4 in, uint x, uint y) { }

#pragma rs reduce(my_char4_7) accumulator(my_char4_7_accum) combiner(combiner)
static void my_char4_7_accum(int *accum, char4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_char4_8) accumulator(my_char4_8_accum) combiner(combiner)
static void my_char4_8_accum(int *accum, char4 in, uint z) { }

#pragma rs reduce(my_char4_9) accumulator(my_char4_9_accum) combiner(combiner)
static void my_char4_9_accum(int *accum, char4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_char4_10) accumulator(my_char4_10_accum) combiner(combiner)
static void my_char4_10_accum(int *accum, char4 in, uint x, uint z) { }

#pragma rs reduce(my_char4_11) accumulator(my_char4_11_accum) combiner(combiner)
static void my_char4_11_accum(int *accum, char4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_char4_12) accumulator(my_char4_12_accum) combiner(combiner)
static void my_char4_12_accum(int *accum, char4 in, uint y, uint z) { }

#pragma rs reduce(my_char4_13) accumulator(my_char4_13_accum) combiner(combiner)
static void my_char4_13_accum(int *accum, char4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_char4_14) accumulator(my_char4_14_accum) combiner(combiner)
static void my_char4_14_accum(int *accum, char4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_char4_15) accumulator(my_char4_15_accum) combiner(combiner)
static void my_char4_15_accum(int *accum, char4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_short_0) accumulator(my_short_0_accum) combiner(combiner)
static void my_short_0_accum(int *accum, short in) { }

#pragma rs reduce(my_short_1) accumulator(my_short_1_accum) combiner(combiner)
static void my_short_1_accum(int *accum, short in, rs_kernel_context context) { }

#pragma rs reduce(my_short_2) accumulator(my_short_2_accum) combiner(combiner)
static void my_short_2_accum(int *accum, short in, uint x) { }

#pragma rs reduce(my_short_3) accumulator(my_short_3_accum) combiner(combiner)
static void my_short_3_accum(int *accum, short in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_short_4) accumulator(my_short_4_accum) combiner(combiner)
static void my_short_4_accum(int *accum, short in, uint y) { }

#pragma rs reduce(my_short_5) accumulator(my_short_5_accum) combiner(combiner)
static void my_short_5_accum(int *accum, short in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_short_6) accumulator(my_short_6_accum) combiner(combiner)
static void my_short_6_accum(int *accum, short in, uint x, uint y) { }

#pragma rs reduce(my_short_7) accumulator(my_short_7_accum) combiner(combiner)
static void my_short_7_accum(int *accum, short in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_short_8) accumulator(my_short_8_accum) combiner(combiner)
static void my_short_8_accum(int *accum, short in, uint z) { }

#pragma rs reduce(my_short_9) accumulator(my_short_9_accum) combiner(combiner)
static void my_short_9_accum(int *accum, short in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_short_10) accumulator(my_short_10_accum) combiner(combiner)
static void my_short_10_accum(int *accum, short in, uint x, uint z) { }

#pragma rs reduce(my_short_11) accumulator(my_short_11_accum) combiner(combiner)
static void my_short_11_accum(int *accum, short in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_short_12) accumulator(my_short_12_accum) combiner(combiner)
static void my_short_12_accum(int *accum, short in, uint y, uint z) { }

#pragma rs reduce(my_short_13) accumulator(my_short_13_accum) combiner(combiner)
static void my_short_13_accum(int *accum, short in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_short_14) accumulator(my_short_14_accum) combiner(combiner)
static void my_short_14_accum(int *accum, short in, uint x, uint y, uint z) { }

#pragma rs reduce(my_short_15) accumulator(my_short_15_accum) combiner(combiner)
static void my_short_15_accum(int *accum, short in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_short2_0) accumulator(my_short2_0_accum) combiner(combiner)
static void my_short2_0_accum(int *accum, short2 in) { }

#pragma rs reduce(my_short2_1) accumulator(my_short2_1_accum) combiner(combiner)
static void my_short2_1_accum(int *accum, short2 in, rs_kernel_context context) { }

#pragma rs reduce(my_short2_2) accumulator(my_short2_2_accum) combiner(combiner)
static void my_short2_2_accum(int *accum, short2 in, uint x) { }

#pragma rs reduce(my_short2_3) accumulator(my_short2_3_accum) combiner(combiner)
static void my_short2_3_accum(int *accum, short2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_short2_4) accumulator(my_short2_4_accum) combiner(combiner)
static void my_short2_4_accum(int *accum, short2 in, uint y) { }

#pragma rs reduce(my_short2_5) accumulator(my_short2_5_accum) combiner(combiner)
static void my_short2_5_accum(int *accum, short2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_short2_6) accumulator(my_short2_6_accum) combiner(combiner)
static void my_short2_6_accum(int *accum, short2 in, uint x, uint y) { }

#pragma rs reduce(my_short2_7) accumulator(my_short2_7_accum) combiner(combiner)
static void my_short2_7_accum(int *accum, short2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_short2_8) accumulator(my_short2_8_accum) combiner(combiner)
static void my_short2_8_accum(int *accum, short2 in, uint z) { }

#pragma rs reduce(my_short2_9) accumulator(my_short2_9_accum) combiner(combiner)
static void my_short2_9_accum(int *accum, short2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_short2_10) accumulator(my_short2_10_accum) combiner(combiner)
static void my_short2_10_accum(int *accum, short2 in, uint x, uint z) { }

#pragma rs reduce(my_short2_11) accumulator(my_short2_11_accum) combiner(combiner)
static void my_short2_11_accum(int *accum, short2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_short2_12) accumulator(my_short2_12_accum) combiner(combiner)
static void my_short2_12_accum(int *accum, short2 in, uint y, uint z) { }

#pragma rs reduce(my_short2_13) accumulator(my_short2_13_accum) combiner(combiner)
static void my_short2_13_accum(int *accum, short2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_short2_14) accumulator(my_short2_14_accum) combiner(combiner)
static void my_short2_14_accum(int *accum, short2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_short2_15) accumulator(my_short2_15_accum) combiner(combiner)
static void my_short2_15_accum(int *accum, short2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_short4_0) accumulator(my_short4_0_accum) combiner(combiner)
static void my_short4_0_accum(int *accum, short4 in) { }

#pragma rs reduce(my_short4_1) accumulator(my_short4_1_accum) combiner(combiner)
static void my_short4_1_accum(int *accum, short4 in, rs_kernel_context context) { }

#pragma rs reduce(my_short4_2) accumulator(my_short4_2_accum) combiner(combiner)
static void my_short4_2_accum(int *accum, short4 in, uint x) { }

#pragma rs reduce(my_short4_3) accumulator(my_short4_3_accum) combiner(combiner)
static void my_short4_3_accum(int *accum, short4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_short4_4) accumulator(my_short4_4_accum) combiner(combiner)
static void my_short4_4_accum(int *accum, short4 in, uint y) { }

#pragma rs reduce(my_short4_5) accumulator(my_short4_5_accum) combiner(combiner)
static void my_short4_5_accum(int *accum, short4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_short4_6) accumulator(my_short4_6_accum) combiner(combiner)
static void my_short4_6_accum(int *accum, short4 in, uint x, uint y) { }

#pragma rs reduce(my_short4_7) accumulator(my_short4_7_accum) combiner(combiner)
static void my_short4_7_accum(int *accum, short4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_short4_8) accumulator(my_short4_8_accum) combiner(combiner)
static void my_short4_8_accum(int *accum, short4 in, uint z) { }

#pragma rs reduce(my_short4_9) accumulator(my_short4_9_accum) combiner(combiner)
static void my_short4_9_accum(int *accum, short4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_short4_10) accumulator(my_short4_10_accum) combiner(combiner)
static void my_short4_10_accum(int *accum, short4 in, uint x, uint z) { }

#pragma rs reduce(my_short4_11) accumulator(my_short4_11_accum) combiner(combiner)
static void my_short4_11_accum(int *accum, short4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_short4_12) accumulator(my_short4_12_accum) combiner(combiner)
static void my_short4_12_accum(int *accum, short4 in, uint y, uint z) { }

#pragma rs reduce(my_short4_13) accumulator(my_short4_13_accum) combiner(combiner)
static void my_short4_13_accum(int *accum, short4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_short4_14) accumulator(my_short4_14_accum) combiner(combiner)
static void my_short4_14_accum(int *accum, short4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_short4_15) accumulator(my_short4_15_accum) combiner(combiner)
static void my_short4_15_accum(int *accum, short4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_int_0) accumulator(my_int_0_accum) combiner(combiner)
static void my_int_0_accum(int *accum, int in) { }

#pragma rs reduce(my_int_1) accumulator(my_int_1_accum) combiner(combiner)
static void my_int_1_accum(int *accum, int in, rs_kernel_context context) { }

#pragma rs reduce(my_int_2) accumulator(my_int_2_accum) combiner(combiner)
static void my_int_2_accum(int *accum, int in, uint x) { }

#pragma rs reduce(my_int_3) accumulator(my_int_3_accum) combiner(combiner)
static void my_int_3_accum(int *accum, int in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_int_4) accumulator(my_int_4_accum) combiner(combiner)
static void my_int_4_accum(int *accum, int in, uint y) { }

#pragma rs reduce(my_int_5) accumulator(my_int_5_accum) combiner(combiner)
static void my_int_5_accum(int *accum, int in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_int_6) accumulator(my_int_6_accum) combiner(combiner)
static void my_int_6_accum(int *accum, int in, uint x, uint y) { }

#pragma rs reduce(my_int_7) accumulator(my_int_7_accum) combiner(combiner)
static void my_int_7_accum(int *accum, int in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_int_8) accumulator(my_int_8_accum) combiner(combiner)
static void my_int_8_accum(int *accum, int in, uint z) { }

#pragma rs reduce(my_int_9) accumulator(my_int_9_accum) combiner(combiner)
static void my_int_9_accum(int *accum, int in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_int_10) accumulator(my_int_10_accum) combiner(combiner)
static void my_int_10_accum(int *accum, int in, uint x, uint z) { }

#pragma rs reduce(my_int_11) accumulator(my_int_11_accum) combiner(combiner)
static void my_int_11_accum(int *accum, int in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_int_12) accumulator(my_int_12_accum) combiner(combiner)
static void my_int_12_accum(int *accum, int in, uint y, uint z) { }

#pragma rs reduce(my_int_13) accumulator(my_int_13_accum) combiner(combiner)
static void my_int_13_accum(int *accum, int in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_int_14) accumulator(my_int_14_accum) combiner(combiner)
static void my_int_14_accum(int *accum, int in, uint x, uint y, uint z) { }

#pragma rs reduce(my_int_15) accumulator(my_int_15_accum) combiner(combiner)
static void my_int_15_accum(int *accum, int in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_int2_0) accumulator(my_int2_0_accum) combiner(combiner)
static void my_int2_0_accum(int *accum, int2 in) { }

#pragma rs reduce(my_int2_1) accumulator(my_int2_1_accum) combiner(combiner)
static void my_int2_1_accum(int *accum, int2 in, rs_kernel_context context) { }

#pragma rs reduce(my_int2_2) accumulator(my_int2_2_accum) combiner(combiner)
static void my_int2_2_accum(int *accum, int2 in, uint x) { }

#pragma rs reduce(my_int2_3) accumulator(my_int2_3_accum) combiner(combiner)
static void my_int2_3_accum(int *accum, int2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_int2_4) accumulator(my_int2_4_accum) combiner(combiner)
static void my_int2_4_accum(int *accum, int2 in, uint y) { }

#pragma rs reduce(my_int2_5) accumulator(my_int2_5_accum) combiner(combiner)
static void my_int2_5_accum(int *accum, int2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_int2_6) accumulator(my_int2_6_accum) combiner(combiner)
static void my_int2_6_accum(int *accum, int2 in, uint x, uint y) { }

#pragma rs reduce(my_int2_7) accumulator(my_int2_7_accum) combiner(combiner)
static void my_int2_7_accum(int *accum, int2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_int2_8) accumulator(my_int2_8_accum) combiner(combiner)
static void my_int2_8_accum(int *accum, int2 in, uint z) { }

#pragma rs reduce(my_int2_9) accumulator(my_int2_9_accum) combiner(combiner)
static void my_int2_9_accum(int *accum, int2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_int2_10) accumulator(my_int2_10_accum) combiner(combiner)
static void my_int2_10_accum(int *accum, int2 in, uint x, uint z) { }

#pragma rs reduce(my_int2_11) accumulator(my_int2_11_accum) combiner(combiner)
static void my_int2_11_accum(int *accum, int2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_int2_12) accumulator(my_int2_12_accum) combiner(combiner)
static void my_int2_12_accum(int *accum, int2 in, uint y, uint z) { }

#pragma rs reduce(my_int2_13) accumulator(my_int2_13_accum) combiner(combiner)
static void my_int2_13_accum(int *accum, int2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_int2_14) accumulator(my_int2_14_accum) combiner(combiner)
static void my_int2_14_accum(int *accum, int2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_int2_15) accumulator(my_int2_15_accum) combiner(combiner)
static void my_int2_15_accum(int *accum, int2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_int4_0) accumulator(my_int4_0_accum) combiner(combiner)
static void my_int4_0_accum(int *accum, int4 in) { }

#pragma rs reduce(my_int4_1) accumulator(my_int4_1_accum) combiner(combiner)
static void my_int4_1_accum(int *accum, int4 in, rs_kernel_context context) { }

#pragma rs reduce(my_int4_2) accumulator(my_int4_2_accum) combiner(combiner)
static void my_int4_2_accum(int *accum, int4 in, uint x) { }

#pragma rs reduce(my_int4_3) accumulator(my_int4_3_accum) combiner(combiner)
static void my_int4_3_accum(int *accum, int4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_int4_4) accumulator(my_int4_4_accum) combiner(combiner)
static void my_int4_4_accum(int *accum, int4 in, uint y) { }

#pragma rs reduce(my_int4_5) accumulator(my_int4_5_accum) combiner(combiner)
static void my_int4_5_accum(int *accum, int4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_int4_6) accumulator(my_int4_6_accum) combiner(combiner)
static void my_int4_6_accum(int *accum, int4 in, uint x, uint y) { }

#pragma rs reduce(my_int4_7) accumulator(my_int4_7_accum) combiner(combiner)
static void my_int4_7_accum(int *accum, int4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_int4_8) accumulator(my_int4_8_accum) combiner(combiner)
static void my_int4_8_accum(int *accum, int4 in, uint z) { }

#pragma rs reduce(my_int4_9) accumulator(my_int4_9_accum) combiner(combiner)
static void my_int4_9_accum(int *accum, int4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_int4_10) accumulator(my_int4_10_accum) combiner(combiner)
static void my_int4_10_accum(int *accum, int4 in, uint x, uint z) { }

#pragma rs reduce(my_int4_11) accumulator(my_int4_11_accum) combiner(combiner)
static void my_int4_11_accum(int *accum, int4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_int4_12) accumulator(my_int4_12_accum) combiner(combiner)
static void my_int4_12_accum(int *accum, int4 in, uint y, uint z) { }

#pragma rs reduce(my_int4_13) accumulator(my_int4_13_accum) combiner(combiner)
static void my_int4_13_accum(int *accum, int4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_int4_14) accumulator(my_int4_14_accum) combiner(combiner)
static void my_int4_14_accum(int *accum, int4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_int4_15) accumulator(my_int4_15_accum) combiner(combiner)
static void my_int4_15_accum(int *accum, int4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_long_0) accumulator(my_long_0_accum) combiner(combiner)
static void my_long_0_accum(int *accum, long in) { }

#pragma rs reduce(my_long_1) accumulator(my_long_1_accum) combiner(combiner)
static void my_long_1_accum(int *accum, long in, rs_kernel_context context) { }

#pragma rs reduce(my_long_2) accumulator(my_long_2_accum) combiner(combiner)
static void my_long_2_accum(int *accum, long in, uint x) { }

#pragma rs reduce(my_long_3) accumulator(my_long_3_accum) combiner(combiner)
static void my_long_3_accum(int *accum, long in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_long_4) accumulator(my_long_4_accum) combiner(combiner)
static void my_long_4_accum(int *accum, long in, uint y) { }

#pragma rs reduce(my_long_5) accumulator(my_long_5_accum) combiner(combiner)
static void my_long_5_accum(int *accum, long in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_long_6) accumulator(my_long_6_accum) combiner(combiner)
static void my_long_6_accum(int *accum, long in, uint x, uint y) { }

#pragma rs reduce(my_long_7) accumulator(my_long_7_accum) combiner(combiner)
static void my_long_7_accum(int *accum, long in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_long_8) accumulator(my_long_8_accum) combiner(combiner)
static void my_long_8_accum(int *accum, long in, uint z) { }

#pragma rs reduce(my_long_9) accumulator(my_long_9_accum) combiner(combiner)
static void my_long_9_accum(int *accum, long in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_long_10) accumulator(my_long_10_accum) combiner(combiner)
static void my_long_10_accum(int *accum, long in, uint x, uint z) { }

#pragma rs reduce(my_long_11) accumulator(my_long_11_accum) combiner(combiner)
static void my_long_11_accum(int *accum, long in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_long_12) accumulator(my_long_12_accum) combiner(combiner)
static void my_long_12_accum(int *accum, long in, uint y, uint z) { }

#pragma rs reduce(my_long_13) accumulator(my_long_13_accum) combiner(combiner)
static void my_long_13_accum(int *accum, long in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_long_14) accumulator(my_long_14_accum) combiner(combiner)
static void my_long_14_accum(int *accum, long in, uint x, uint y, uint z) { }

#pragma rs reduce(my_long_15) accumulator(my_long_15_accum) combiner(combiner)
static void my_long_15_accum(int *accum, long in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_long2_0) accumulator(my_long2_0_accum) combiner(combiner)
static void my_long2_0_accum(int *accum, long2 in) { }

#pragma rs reduce(my_long2_1) accumulator(my_long2_1_accum) combiner(combiner)
static void my_long2_1_accum(int *accum, long2 in, rs_kernel_context context) { }

#pragma rs reduce(my_long2_2) accumulator(my_long2_2_accum) combiner(combiner)
static void my_long2_2_accum(int *accum, long2 in, uint x) { }

#pragma rs reduce(my_long2_3) accumulator(my_long2_3_accum) combiner(combiner)
static void my_long2_3_accum(int *accum, long2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_long2_4) accumulator(my_long2_4_accum) combiner(combiner)
static void my_long2_4_accum(int *accum, long2 in, uint y) { }

#pragma rs reduce(my_long2_5) accumulator(my_long2_5_accum) combiner(combiner)
static void my_long2_5_accum(int *accum, long2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_long2_6) accumulator(my_long2_6_accum) combiner(combiner)
static void my_long2_6_accum(int *accum, long2 in, uint x, uint y) { }

#pragma rs reduce(my_long2_7) accumulator(my_long2_7_accum) combiner(combiner)
static void my_long2_7_accum(int *accum, long2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_long2_8) accumulator(my_long2_8_accum) combiner(combiner)
static void my_long2_8_accum(int *accum, long2 in, uint z) { }

#pragma rs reduce(my_long2_9) accumulator(my_long2_9_accum) combiner(combiner)
static void my_long2_9_accum(int *accum, long2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_long2_10) accumulator(my_long2_10_accum) combiner(combiner)
static void my_long2_10_accum(int *accum, long2 in, uint x, uint z) { }

#pragma rs reduce(my_long2_11) accumulator(my_long2_11_accum) combiner(combiner)
static void my_long2_11_accum(int *accum, long2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_long2_12) accumulator(my_long2_12_accum) combiner(combiner)
static void my_long2_12_accum(int *accum, long2 in, uint y, uint z) { }

#pragma rs reduce(my_long2_13) accumulator(my_long2_13_accum) combiner(combiner)
static void my_long2_13_accum(int *accum, long2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_long2_14) accumulator(my_long2_14_accum) combiner(combiner)
static void my_long2_14_accum(int *accum, long2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_long2_15) accumulator(my_long2_15_accum) combiner(combiner)
static void my_long2_15_accum(int *accum, long2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_long4_0) accumulator(my_long4_0_accum) combiner(combiner)
static void my_long4_0_accum(int *accum, long4 in) { }

#pragma rs reduce(my_long4_1) accumulator(my_long4_1_accum) combiner(combiner)
static void my_long4_1_accum(int *accum, long4 in, rs_kernel_context context) { }

#pragma rs reduce(my_long4_2) accumulator(my_long4_2_accum) combiner(combiner)
static void my_long4_2_accum(int *accum, long4 in, uint x) { }

#pragma rs reduce(my_long4_3) accumulator(my_long4_3_accum) combiner(combiner)
static void my_long4_3_accum(int *accum, long4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_long4_4) accumulator(my_long4_4_accum) combiner(combiner)
static void my_long4_4_accum(int *accum, long4 in, uint y) { }

#pragma rs reduce(my_long4_5) accumulator(my_long4_5_accum) combiner(combiner)
static void my_long4_5_accum(int *accum, long4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_long4_6) accumulator(my_long4_6_accum) combiner(combiner)
static void my_long4_6_accum(int *accum, long4 in, uint x, uint y) { }

#pragma rs reduce(my_long4_7) accumulator(my_long4_7_accum) combiner(combiner)
static void my_long4_7_accum(int *accum, long4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_long4_8) accumulator(my_long4_8_accum) combiner(combiner)
static void my_long4_8_accum(int *accum, long4 in, uint z) { }

#pragma rs reduce(my_long4_9) accumulator(my_long4_9_accum) combiner(combiner)
static void my_long4_9_accum(int *accum, long4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_long4_10) accumulator(my_long4_10_accum) combiner(combiner)
static void my_long4_10_accum(int *accum, long4 in, uint x, uint z) { }

#pragma rs reduce(my_long4_11) accumulator(my_long4_11_accum) combiner(combiner)
static void my_long4_11_accum(int *accum, long4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_long4_12) accumulator(my_long4_12_accum) combiner(combiner)
static void my_long4_12_accum(int *accum, long4 in, uint y, uint z) { }

#pragma rs reduce(my_long4_13) accumulator(my_long4_13_accum) combiner(combiner)
static void my_long4_13_accum(int *accum, long4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_long4_14) accumulator(my_long4_14_accum) combiner(combiner)
static void my_long4_14_accum(int *accum, long4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_long4_15) accumulator(my_long4_15_accum) combiner(combiner)
static void my_long4_15_accum(int *accum, long4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_uchar_0) accumulator(my_uchar_0_accum) combiner(combiner)
static void my_uchar_0_accum(int *accum, uchar in) { }

#pragma rs reduce(my_uchar_1) accumulator(my_uchar_1_accum) combiner(combiner)
static void my_uchar_1_accum(int *accum, uchar in, rs_kernel_context context) { }

#pragma rs reduce(my_uchar_2) accumulator(my_uchar_2_accum) combiner(combiner)
static void my_uchar_2_accum(int *accum, uchar in, uint x) { }

#pragma rs reduce(my_uchar_3) accumulator(my_uchar_3_accum) combiner(combiner)
static void my_uchar_3_accum(int *accum, uchar in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_uchar_4) accumulator(my_uchar_4_accum) combiner(combiner)
static void my_uchar_4_accum(int *accum, uchar in, uint y) { }

#pragma rs reduce(my_uchar_5) accumulator(my_uchar_5_accum) combiner(combiner)
static void my_uchar_5_accum(int *accum, uchar in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_uchar_6) accumulator(my_uchar_6_accum) combiner(combiner)
static void my_uchar_6_accum(int *accum, uchar in, uint x, uint y) { }

#pragma rs reduce(my_uchar_7) accumulator(my_uchar_7_accum) combiner(combiner)
static void my_uchar_7_accum(int *accum, uchar in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_uchar_8) accumulator(my_uchar_8_accum) combiner(combiner)
static void my_uchar_8_accum(int *accum, uchar in, uint z) { }

#pragma rs reduce(my_uchar_9) accumulator(my_uchar_9_accum) combiner(combiner)
static void my_uchar_9_accum(int *accum, uchar in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_uchar_10) accumulator(my_uchar_10_accum) combiner(combiner)
static void my_uchar_10_accum(int *accum, uchar in, uint x, uint z) { }

#pragma rs reduce(my_uchar_11) accumulator(my_uchar_11_accum) combiner(combiner)
static void my_uchar_11_accum(int *accum, uchar in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_uchar_12) accumulator(my_uchar_12_accum) combiner(combiner)
static void my_uchar_12_accum(int *accum, uchar in, uint y, uint z) { }

#pragma rs reduce(my_uchar_13) accumulator(my_uchar_13_accum) combiner(combiner)
static void my_uchar_13_accum(int *accum, uchar in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_uchar_14) accumulator(my_uchar_14_accum) combiner(combiner)
static void my_uchar_14_accum(int *accum, uchar in, uint x, uint y, uint z) { }

#pragma rs reduce(my_uchar_15) accumulator(my_uchar_15_accum) combiner(combiner)
static void my_uchar_15_accum(int *accum, uchar in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_uchar2_0) accumulator(my_uchar2_0_accum) combiner(combiner)
static void my_uchar2_0_accum(int *accum, uchar2 in) { }

#pragma rs reduce(my_uchar2_1) accumulator(my_uchar2_1_accum) combiner(combiner)
static void my_uchar2_1_accum(int *accum, uchar2 in, rs_kernel_context context) { }

#pragma rs reduce(my_uchar2_2) accumulator(my_uchar2_2_accum) combiner(combiner)
static void my_uchar2_2_accum(int *accum, uchar2 in, uint x) { }

#pragma rs reduce(my_uchar2_3) accumulator(my_uchar2_3_accum) combiner(combiner)
static void my_uchar2_3_accum(int *accum, uchar2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_uchar2_4) accumulator(my_uchar2_4_accum) combiner(combiner)
static void my_uchar2_4_accum(int *accum, uchar2 in, uint y) { }

#pragma rs reduce(my_uchar2_5) accumulator(my_uchar2_5_accum) combiner(combiner)
static void my_uchar2_5_accum(int *accum, uchar2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_uchar2_6) accumulator(my_uchar2_6_accum) combiner(combiner)
static void my_uchar2_6_accum(int *accum, uchar2 in, uint x, uint y) { }

#pragma rs reduce(my_uchar2_7) accumulator(my_uchar2_7_accum) combiner(combiner)
static void my_uchar2_7_accum(int *accum, uchar2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_uchar2_8) accumulator(my_uchar2_8_accum) combiner(combiner)
static void my_uchar2_8_accum(int *accum, uchar2 in, uint z) { }

#pragma rs reduce(my_uchar2_9) accumulator(my_uchar2_9_accum) combiner(combiner)
static void my_uchar2_9_accum(int *accum, uchar2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_uchar2_10) accumulator(my_uchar2_10_accum) combiner(combiner)
static void my_uchar2_10_accum(int *accum, uchar2 in, uint x, uint z) { }

#pragma rs reduce(my_uchar2_11) accumulator(my_uchar2_11_accum) combiner(combiner)
static void my_uchar2_11_accum(int *accum, uchar2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_uchar2_12) accumulator(my_uchar2_12_accum) combiner(combiner)
static void my_uchar2_12_accum(int *accum, uchar2 in, uint y, uint z) { }

#pragma rs reduce(my_uchar2_13) accumulator(my_uchar2_13_accum) combiner(combiner)
static void my_uchar2_13_accum(int *accum, uchar2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_uchar2_14) accumulator(my_uchar2_14_accum) combiner(combiner)
static void my_uchar2_14_accum(int *accum, uchar2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_uchar2_15) accumulator(my_uchar2_15_accum) combiner(combiner)
static void my_uchar2_15_accum(int *accum, uchar2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_uchar4_0) accumulator(my_uchar4_0_accum) combiner(combiner)
static void my_uchar4_0_accum(int *accum, uchar4 in) { }

#pragma rs reduce(my_uchar4_1) accumulator(my_uchar4_1_accum) combiner(combiner)
static void my_uchar4_1_accum(int *accum, uchar4 in, rs_kernel_context context) { }

#pragma rs reduce(my_uchar4_2) accumulator(my_uchar4_2_accum) combiner(combiner)
static void my_uchar4_2_accum(int *accum, uchar4 in, uint x) { }

#pragma rs reduce(my_uchar4_3) accumulator(my_uchar4_3_accum) combiner(combiner)
static void my_uchar4_3_accum(int *accum, uchar4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_uchar4_4) accumulator(my_uchar4_4_accum) combiner(combiner)
static void my_uchar4_4_accum(int *accum, uchar4 in, uint y) { }

#pragma rs reduce(my_uchar4_5) accumulator(my_uchar4_5_accum) combiner(combiner)
static void my_uchar4_5_accum(int *accum, uchar4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_uchar4_6) accumulator(my_uchar4_6_accum) combiner(combiner)
static void my_uchar4_6_accum(int *accum, uchar4 in, uint x, uint y) { }

#pragma rs reduce(my_uchar4_7) accumulator(my_uchar4_7_accum) combiner(combiner)
static void my_uchar4_7_accum(int *accum, uchar4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_uchar4_8) accumulator(my_uchar4_8_accum) combiner(combiner)
static void my_uchar4_8_accum(int *accum, uchar4 in, uint z) { }

#pragma rs reduce(my_uchar4_9) accumulator(my_uchar4_9_accum) combiner(combiner)
static void my_uchar4_9_accum(int *accum, uchar4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_uchar4_10) accumulator(my_uchar4_10_accum) combiner(combiner)
static void my_uchar4_10_accum(int *accum, uchar4 in, uint x, uint z) { }

#pragma rs reduce(my_uchar4_11) accumulator(my_uchar4_11_accum) combiner(combiner)
static void my_uchar4_11_accum(int *accum, uchar4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_uchar4_12) accumulator(my_uchar4_12_accum) combiner(combiner)
static void my_uchar4_12_accum(int *accum, uchar4 in, uint y, uint z) { }

#pragma rs reduce(my_uchar4_13) accumulator(my_uchar4_13_accum) combiner(combiner)
static void my_uchar4_13_accum(int *accum, uchar4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_uchar4_14) accumulator(my_uchar4_14_accum) combiner(combiner)
static void my_uchar4_14_accum(int *accum, uchar4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_uchar4_15) accumulator(my_uchar4_15_accum) combiner(combiner)
static void my_uchar4_15_accum(int *accum, uchar4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_ushort_0) accumulator(my_ushort_0_accum) combiner(combiner)
static void my_ushort_0_accum(int *accum, ushort in) { }

#pragma rs reduce(my_ushort_1) accumulator(my_ushort_1_accum) combiner(combiner)
static void my_ushort_1_accum(int *accum, ushort in, rs_kernel_context context) { }

#pragma rs reduce(my_ushort_2) accumulator(my_ushort_2_accum) combiner(combiner)
static void my_ushort_2_accum(int *accum, ushort in, uint x) { }

#pragma rs reduce(my_ushort_3) accumulator(my_ushort_3_accum) combiner(combiner)
static void my_ushort_3_accum(int *accum, ushort in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_ushort_4) accumulator(my_ushort_4_accum) combiner(combiner)
static void my_ushort_4_accum(int *accum, ushort in, uint y) { }

#pragma rs reduce(my_ushort_5) accumulator(my_ushort_5_accum) combiner(combiner)
static void my_ushort_5_accum(int *accum, ushort in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_ushort_6) accumulator(my_ushort_6_accum) combiner(combiner)
static void my_ushort_6_accum(int *accum, ushort in, uint x, uint y) { }

#pragma rs reduce(my_ushort_7) accumulator(my_ushort_7_accum) combiner(combiner)
static void my_ushort_7_accum(int *accum, ushort in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_ushort_8) accumulator(my_ushort_8_accum) combiner(combiner)
static void my_ushort_8_accum(int *accum, ushort in, uint z) { }

#pragma rs reduce(my_ushort_9) accumulator(my_ushort_9_accum) combiner(combiner)
static void my_ushort_9_accum(int *accum, ushort in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_ushort_10) accumulator(my_ushort_10_accum) combiner(combiner)
static void my_ushort_10_accum(int *accum, ushort in, uint x, uint z) { }

#pragma rs reduce(my_ushort_11) accumulator(my_ushort_11_accum) combiner(combiner)
static void my_ushort_11_accum(int *accum, ushort in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_ushort_12) accumulator(my_ushort_12_accum) combiner(combiner)
static void my_ushort_12_accum(int *accum, ushort in, uint y, uint z) { }

#pragma rs reduce(my_ushort_13) accumulator(my_ushort_13_accum) combiner(combiner)
static void my_ushort_13_accum(int *accum, ushort in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_ushort_14) accumulator(my_ushort_14_accum) combiner(combiner)
static void my_ushort_14_accum(int *accum, ushort in, uint x, uint y, uint z) { }

#pragma rs reduce(my_ushort_15) accumulator(my_ushort_15_accum) combiner(combiner)
static void my_ushort_15_accum(int *accum, ushort in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_ushort2_0) accumulator(my_ushort2_0_accum) combiner(combiner)
static void my_ushort2_0_accum(int *accum, ushort2 in) { }

#pragma rs reduce(my_ushort2_1) accumulator(my_ushort2_1_accum) combiner(combiner)
static void my_ushort2_1_accum(int *accum, ushort2 in, rs_kernel_context context) { }

#pragma rs reduce(my_ushort2_2) accumulator(my_ushort2_2_accum) combiner(combiner)
static void my_ushort2_2_accum(int *accum, ushort2 in, uint x) { }

#pragma rs reduce(my_ushort2_3) accumulator(my_ushort2_3_accum) combiner(combiner)
static void my_ushort2_3_accum(int *accum, ushort2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_ushort2_4) accumulator(my_ushort2_4_accum) combiner(combiner)
static void my_ushort2_4_accum(int *accum, ushort2 in, uint y) { }

#pragma rs reduce(my_ushort2_5) accumulator(my_ushort2_5_accum) combiner(combiner)
static void my_ushort2_5_accum(int *accum, ushort2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_ushort2_6) accumulator(my_ushort2_6_accum) combiner(combiner)
static void my_ushort2_6_accum(int *accum, ushort2 in, uint x, uint y) { }

#pragma rs reduce(my_ushort2_7) accumulator(my_ushort2_7_accum) combiner(combiner)
static void my_ushort2_7_accum(int *accum, ushort2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_ushort2_8) accumulator(my_ushort2_8_accum) combiner(combiner)
static void my_ushort2_8_accum(int *accum, ushort2 in, uint z) { }

#pragma rs reduce(my_ushort2_9) accumulator(my_ushort2_9_accum) combiner(combiner)
static void my_ushort2_9_accum(int *accum, ushort2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_ushort2_10) accumulator(my_ushort2_10_accum) combiner(combiner)
static void my_ushort2_10_accum(int *accum, ushort2 in, uint x, uint z) { }

#pragma rs reduce(my_ushort2_11) accumulator(my_ushort2_11_accum) combiner(combiner)
static void my_ushort2_11_accum(int *accum, ushort2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_ushort2_12) accumulator(my_ushort2_12_accum) combiner(combiner)
static void my_ushort2_12_accum(int *accum, ushort2 in, uint y, uint z) { }

#pragma rs reduce(my_ushort2_13) accumulator(my_ushort2_13_accum) combiner(combiner)
static void my_ushort2_13_accum(int *accum, ushort2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_ushort2_14) accumulator(my_ushort2_14_accum) combiner(combiner)
static void my_ushort2_14_accum(int *accum, ushort2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_ushort2_15) accumulator(my_ushort2_15_accum) combiner(combiner)
static void my_ushort2_15_accum(int *accum, ushort2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_ushort4_0) accumulator(my_ushort4_0_accum) combiner(combiner)
static void my_ushort4_0_accum(int *accum, ushort4 in) { }

#pragma rs reduce(my_ushort4_1) accumulator(my_ushort4_1_accum) combiner(combiner)
static void my_ushort4_1_accum(int *accum, ushort4 in, rs_kernel_context context) { }

#pragma rs reduce(my_ushort4_2) accumulator(my_ushort4_2_accum) combiner(combiner)
static void my_ushort4_2_accum(int *accum, ushort4 in, uint x) { }

#pragma rs reduce(my_ushort4_3) accumulator(my_ushort4_3_accum) combiner(combiner)
static void my_ushort4_3_accum(int *accum, ushort4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_ushort4_4) accumulator(my_ushort4_4_accum) combiner(combiner)
static void my_ushort4_4_accum(int *accum, ushort4 in, uint y) { }

#pragma rs reduce(my_ushort4_5) accumulator(my_ushort4_5_accum) combiner(combiner)
static void my_ushort4_5_accum(int *accum, ushort4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_ushort4_6) accumulator(my_ushort4_6_accum) combiner(combiner)
static void my_ushort4_6_accum(int *accum, ushort4 in, uint x, uint y) { }

#pragma rs reduce(my_ushort4_7) accumulator(my_ushort4_7_accum) combiner(combiner)
static void my_ushort4_7_accum(int *accum, ushort4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_ushort4_8) accumulator(my_ushort4_8_accum) combiner(combiner)
static void my_ushort4_8_accum(int *accum, ushort4 in, uint z) { }

#pragma rs reduce(my_ushort4_9) accumulator(my_ushort4_9_accum) combiner(combiner)
static void my_ushort4_9_accum(int *accum, ushort4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_ushort4_10) accumulator(my_ushort4_10_accum) combiner(combiner)
static void my_ushort4_10_accum(int *accum, ushort4 in, uint x, uint z) { }

#pragma rs reduce(my_ushort4_11) accumulator(my_ushort4_11_accum) combiner(combiner)
static void my_ushort4_11_accum(int *accum, ushort4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_ushort4_12) accumulator(my_ushort4_12_accum) combiner(combiner)
static void my_ushort4_12_accum(int *accum, ushort4 in, uint y, uint z) { }

#pragma rs reduce(my_ushort4_13) accumulator(my_ushort4_13_accum) combiner(combiner)
static void my_ushort4_13_accum(int *accum, ushort4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_ushort4_14) accumulator(my_ushort4_14_accum) combiner(combiner)
static void my_ushort4_14_accum(int *accum, ushort4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_ushort4_15) accumulator(my_ushort4_15_accum) combiner(combiner)
static void my_ushort4_15_accum(int *accum, ushort4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_uint_0) accumulator(my_uint_0_accum) combiner(combiner)
static void my_uint_0_accum(int *accum, uint in) { }

#pragma rs reduce(my_uint_1) accumulator(my_uint_1_accum) combiner(combiner)
static void my_uint_1_accum(int *accum, uint in, rs_kernel_context context) { }

#pragma rs reduce(my_uint_2) accumulator(my_uint_2_accum) combiner(combiner)
static void my_uint_2_accum(int *accum, uint in, uint x) { }

#pragma rs reduce(my_uint_3) accumulator(my_uint_3_accum) combiner(combiner)
static void my_uint_3_accum(int *accum, uint in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_uint_4) accumulator(my_uint_4_accum) combiner(combiner)
static void my_uint_4_accum(int *accum, uint in, uint y) { }

#pragma rs reduce(my_uint_5) accumulator(my_uint_5_accum) combiner(combiner)
static void my_uint_5_accum(int *accum, uint in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_uint_6) accumulator(my_uint_6_accum) combiner(combiner)
static void my_uint_6_accum(int *accum, uint in, uint x, uint y) { }

#pragma rs reduce(my_uint_7) accumulator(my_uint_7_accum) combiner(combiner)
static void my_uint_7_accum(int *accum, uint in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_uint_8) accumulator(my_uint_8_accum) combiner(combiner)
static void my_uint_8_accum(int *accum, uint in, uint z) { }

#pragma rs reduce(my_uint_9) accumulator(my_uint_9_accum) combiner(combiner)
static void my_uint_9_accum(int *accum, uint in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_uint_10) accumulator(my_uint_10_accum) combiner(combiner)
static void my_uint_10_accum(int *accum, uint in, uint x, uint z) { }

#pragma rs reduce(my_uint_11) accumulator(my_uint_11_accum) combiner(combiner)
static void my_uint_11_accum(int *accum, uint in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_uint_12) accumulator(my_uint_12_accum) combiner(combiner)
static void my_uint_12_accum(int *accum, uint in, uint y, uint z) { }

#pragma rs reduce(my_uint_13) accumulator(my_uint_13_accum) combiner(combiner)
static void my_uint_13_accum(int *accum, uint in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_uint_14) accumulator(my_uint_14_accum) combiner(combiner)
static void my_uint_14_accum(int *accum, uint in, uint x, uint y, uint z) { }

#pragma rs reduce(my_uint_15) accumulator(my_uint_15_accum) combiner(combiner)
static void my_uint_15_accum(int *accum, uint in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_uint2_0) accumulator(my_uint2_0_accum) combiner(combiner)
static void my_uint2_0_accum(int *accum, uint2 in) { }

#pragma rs reduce(my_uint2_1) accumulator(my_uint2_1_accum) combiner(combiner)
static void my_uint2_1_accum(int *accum, uint2 in, rs_kernel_context context) { }

#pragma rs reduce(my_uint2_2) accumulator(my_uint2_2_accum) combiner(combiner)
static void my_uint2_2_accum(int *accum, uint2 in, uint x) { }

#pragma rs reduce(my_uint2_3) accumulator(my_uint2_3_accum) combiner(combiner)
static void my_uint2_3_accum(int *accum, uint2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_uint2_4) accumulator(my_uint2_4_accum) combiner(combiner)
static void my_uint2_4_accum(int *accum, uint2 in, uint y) { }

#pragma rs reduce(my_uint2_5) accumulator(my_uint2_5_accum) combiner(combiner)
static void my_uint2_5_accum(int *accum, uint2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_uint2_6) accumulator(my_uint2_6_accum) combiner(combiner)
static void my_uint2_6_accum(int *accum, uint2 in, uint x, uint y) { }

#pragma rs reduce(my_uint2_7) accumulator(my_uint2_7_accum) combiner(combiner)
static void my_uint2_7_accum(int *accum, uint2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_uint2_8) accumulator(my_uint2_8_accum) combiner(combiner)
static void my_uint2_8_accum(int *accum, uint2 in, uint z) { }

#pragma rs reduce(my_uint2_9) accumulator(my_uint2_9_accum) combiner(combiner)
static void my_uint2_9_accum(int *accum, uint2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_uint2_10) accumulator(my_uint2_10_accum) combiner(combiner)
static void my_uint2_10_accum(int *accum, uint2 in, uint x, uint z) { }

#pragma rs reduce(my_uint2_11) accumulator(my_uint2_11_accum) combiner(combiner)
static void my_uint2_11_accum(int *accum, uint2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_uint2_12) accumulator(my_uint2_12_accum) combiner(combiner)
static void my_uint2_12_accum(int *accum, uint2 in, uint y, uint z) { }

#pragma rs reduce(my_uint2_13) accumulator(my_uint2_13_accum) combiner(combiner)
static void my_uint2_13_accum(int *accum, uint2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_uint2_14) accumulator(my_uint2_14_accum) combiner(combiner)
static void my_uint2_14_accum(int *accum, uint2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_uint2_15) accumulator(my_uint2_15_accum) combiner(combiner)
static void my_uint2_15_accum(int *accum, uint2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_uint4_0) accumulator(my_uint4_0_accum) combiner(combiner)
static void my_uint4_0_accum(int *accum, uint4 in) { }

#pragma rs reduce(my_uint4_1) accumulator(my_uint4_1_accum) combiner(combiner)
static void my_uint4_1_accum(int *accum, uint4 in, rs_kernel_context context) { }

#pragma rs reduce(my_uint4_2) accumulator(my_uint4_2_accum) combiner(combiner)
static void my_uint4_2_accum(int *accum, uint4 in, uint x) { }

#pragma rs reduce(my_uint4_3) accumulator(my_uint4_3_accum) combiner(combiner)
static void my_uint4_3_accum(int *accum, uint4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_uint4_4) accumulator(my_uint4_4_accum) combiner(combiner)
static void my_uint4_4_accum(int *accum, uint4 in, uint y) { }

#pragma rs reduce(my_uint4_5) accumulator(my_uint4_5_accum) combiner(combiner)
static void my_uint4_5_accum(int *accum, uint4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_uint4_6) accumulator(my_uint4_6_accum) combiner(combiner)
static void my_uint4_6_accum(int *accum, uint4 in, uint x, uint y) { }

#pragma rs reduce(my_uint4_7) accumulator(my_uint4_7_accum) combiner(combiner)
static void my_uint4_7_accum(int *accum, uint4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_uint4_8) accumulator(my_uint4_8_accum) combiner(combiner)
static void my_uint4_8_accum(int *accum, uint4 in, uint z) { }

#pragma rs reduce(my_uint4_9) accumulator(my_uint4_9_accum) combiner(combiner)
static void my_uint4_9_accum(int *accum, uint4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_uint4_10) accumulator(my_uint4_10_accum) combiner(combiner)
static void my_uint4_10_accum(int *accum, uint4 in, uint x, uint z) { }

#pragma rs reduce(my_uint4_11) accumulator(my_uint4_11_accum) combiner(combiner)
static void my_uint4_11_accum(int *accum, uint4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_uint4_12) accumulator(my_uint4_12_accum) combiner(combiner)
static void my_uint4_12_accum(int *accum, uint4 in, uint y, uint z) { }

#pragma rs reduce(my_uint4_13) accumulator(my_uint4_13_accum) combiner(combiner)
static void my_uint4_13_accum(int *accum, uint4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_uint4_14) accumulator(my_uint4_14_accum) combiner(combiner)
static void my_uint4_14_accum(int *accum, uint4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_uint4_15) accumulator(my_uint4_15_accum) combiner(combiner)
static void my_uint4_15_accum(int *accum, uint4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_ulong_0) accumulator(my_ulong_0_accum) combiner(combiner)
static void my_ulong_0_accum(int *accum, ulong in) { }

#pragma rs reduce(my_ulong_1) accumulator(my_ulong_1_accum) combiner(combiner)
static void my_ulong_1_accum(int *accum, ulong in, rs_kernel_context context) { }

#pragma rs reduce(my_ulong_2) accumulator(my_ulong_2_accum) combiner(combiner)
static void my_ulong_2_accum(int *accum, ulong in, uint x) { }

#pragma rs reduce(my_ulong_3) accumulator(my_ulong_3_accum) combiner(combiner)
static void my_ulong_3_accum(int *accum, ulong in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_ulong_4) accumulator(my_ulong_4_accum) combiner(combiner)
static void my_ulong_4_accum(int *accum, ulong in, uint y) { }

#pragma rs reduce(my_ulong_5) accumulator(my_ulong_5_accum) combiner(combiner)
static void my_ulong_5_accum(int *accum, ulong in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_ulong_6) accumulator(my_ulong_6_accum) combiner(combiner)
static void my_ulong_6_accum(int *accum, ulong in, uint x, uint y) { }

#pragma rs reduce(my_ulong_7) accumulator(my_ulong_7_accum) combiner(combiner)
static void my_ulong_7_accum(int *accum, ulong in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_ulong_8) accumulator(my_ulong_8_accum) combiner(combiner)
static void my_ulong_8_accum(int *accum, ulong in, uint z) { }

#pragma rs reduce(my_ulong_9) accumulator(my_ulong_9_accum) combiner(combiner)
static void my_ulong_9_accum(int *accum, ulong in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_ulong_10) accumulator(my_ulong_10_accum) combiner(combiner)
static void my_ulong_10_accum(int *accum, ulong in, uint x, uint z) { }

#pragma rs reduce(my_ulong_11) accumulator(my_ulong_11_accum) combiner(combiner)
static void my_ulong_11_accum(int *accum, ulong in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_ulong_12) accumulator(my_ulong_12_accum) combiner(combiner)
static void my_ulong_12_accum(int *accum, ulong in, uint y, uint z) { }

#pragma rs reduce(my_ulong_13) accumulator(my_ulong_13_accum) combiner(combiner)
static void my_ulong_13_accum(int *accum, ulong in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_ulong_14) accumulator(my_ulong_14_accum) combiner(combiner)
static void my_ulong_14_accum(int *accum, ulong in, uint x, uint y, uint z) { }

#pragma rs reduce(my_ulong_15) accumulator(my_ulong_15_accum) combiner(combiner)
static void my_ulong_15_accum(int *accum, ulong in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_ulong2_0) accumulator(my_ulong2_0_accum) combiner(combiner)
static void my_ulong2_0_accum(int *accum, ulong2 in) { }

#pragma rs reduce(my_ulong2_1) accumulator(my_ulong2_1_accum) combiner(combiner)
static void my_ulong2_1_accum(int *accum, ulong2 in, rs_kernel_context context) { }

#pragma rs reduce(my_ulong2_2) accumulator(my_ulong2_2_accum) combiner(combiner)
static void my_ulong2_2_accum(int *accum, ulong2 in, uint x) { }

#pragma rs reduce(my_ulong2_3) accumulator(my_ulong2_3_accum) combiner(combiner)
static void my_ulong2_3_accum(int *accum, ulong2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_ulong2_4) accumulator(my_ulong2_4_accum) combiner(combiner)
static void my_ulong2_4_accum(int *accum, ulong2 in, uint y) { }

#pragma rs reduce(my_ulong2_5) accumulator(my_ulong2_5_accum) combiner(combiner)
static void my_ulong2_5_accum(int *accum, ulong2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_ulong2_6) accumulator(my_ulong2_6_accum) combiner(combiner)
static void my_ulong2_6_accum(int *accum, ulong2 in, uint x, uint y) { }

#pragma rs reduce(my_ulong2_7) accumulator(my_ulong2_7_accum) combiner(combiner)
static void my_ulong2_7_accum(int *accum, ulong2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_ulong2_8) accumulator(my_ulong2_8_accum) combiner(combiner)
static void my_ulong2_8_accum(int *accum, ulong2 in, uint z) { }

#pragma rs reduce(my_ulong2_9) accumulator(my_ulong2_9_accum) combiner(combiner)
static void my_ulong2_9_accum(int *accum, ulong2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_ulong2_10) accumulator(my_ulong2_10_accum) combiner(combiner)
static void my_ulong2_10_accum(int *accum, ulong2 in, uint x, uint z) { }

#pragma rs reduce(my_ulong2_11) accumulator(my_ulong2_11_accum) combiner(combiner)
static void my_ulong2_11_accum(int *accum, ulong2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_ulong2_12) accumulator(my_ulong2_12_accum) combiner(combiner)
static void my_ulong2_12_accum(int *accum, ulong2 in, uint y, uint z) { }

#pragma rs reduce(my_ulong2_13) accumulator(my_ulong2_13_accum) combiner(combiner)
static void my_ulong2_13_accum(int *accum, ulong2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_ulong2_14) accumulator(my_ulong2_14_accum) combiner(combiner)
static void my_ulong2_14_accum(int *accum, ulong2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_ulong2_15) accumulator(my_ulong2_15_accum) combiner(combiner)
static void my_ulong2_15_accum(int *accum, ulong2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_ulong4_0) accumulator(my_ulong4_0_accum) combiner(combiner)
static void my_ulong4_0_accum(int *accum, ulong4 in) { }

#pragma rs reduce(my_ulong4_1) accumulator(my_ulong4_1_accum) combiner(combiner)
static void my_ulong4_1_accum(int *accum, ulong4 in, rs_kernel_context context) { }

#pragma rs reduce(my_ulong4_2) accumulator(my_ulong4_2_accum) combiner(combiner)
static void my_ulong4_2_accum(int *accum, ulong4 in, uint x) { }

#pragma rs reduce(my_ulong4_3) accumulator(my_ulong4_3_accum) combiner(combiner)
static void my_ulong4_3_accum(int *accum, ulong4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_ulong4_4) accumulator(my_ulong4_4_accum) combiner(combiner)
static void my_ulong4_4_accum(int *accum, ulong4 in, uint y) { }

#pragma rs reduce(my_ulong4_5) accumulator(my_ulong4_5_accum) combiner(combiner)
static void my_ulong4_5_accum(int *accum, ulong4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_ulong4_6) accumulator(my_ulong4_6_accum) combiner(combiner)
static void my_ulong4_6_accum(int *accum, ulong4 in, uint x, uint y) { }

#pragma rs reduce(my_ulong4_7) accumulator(my_ulong4_7_accum) combiner(combiner)
static void my_ulong4_7_accum(int *accum, ulong4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_ulong4_8) accumulator(my_ulong4_8_accum) combiner(combiner)
static void my_ulong4_8_accum(int *accum, ulong4 in, uint z) { }

#pragma rs reduce(my_ulong4_9) accumulator(my_ulong4_9_accum) combiner(combiner)
static void my_ulong4_9_accum(int *accum, ulong4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_ulong4_10) accumulator(my_ulong4_10_accum) combiner(combiner)
static void my_ulong4_10_accum(int *accum, ulong4 in, uint x, uint z) { }

#pragma rs reduce(my_ulong4_11) accumulator(my_ulong4_11_accum) combiner(combiner)
static void my_ulong4_11_accum(int *accum, ulong4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_ulong4_12) accumulator(my_ulong4_12_accum) combiner(combiner)
static void my_ulong4_12_accum(int *accum, ulong4 in, uint y, uint z) { }

#pragma rs reduce(my_ulong4_13) accumulator(my_ulong4_13_accum) combiner(combiner)
static void my_ulong4_13_accum(int *accum, ulong4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_ulong4_14) accumulator(my_ulong4_14_accum) combiner(combiner)
static void my_ulong4_14_accum(int *accum, ulong4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_ulong4_15) accumulator(my_ulong4_15_accum) combiner(combiner)
static void my_ulong4_15_accum(int *accum, ulong4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_bool_0) accumulator(my_bool_0_accum) combiner(combiner)
static void my_bool_0_accum(int *accum, bool in) { }

#pragma rs reduce(my_bool_1) accumulator(my_bool_1_accum) combiner(combiner)
static void my_bool_1_accum(int *accum, bool in, rs_kernel_context context) { }

#pragma rs reduce(my_bool_2) accumulator(my_bool_2_accum) combiner(combiner)
static void my_bool_2_accum(int *accum, bool in, uint x) { }

#pragma rs reduce(my_bool_3) accumulator(my_bool_3_accum) combiner(combiner)
static void my_bool_3_accum(int *accum, bool in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_bool_4) accumulator(my_bool_4_accum) combiner(combiner)
static void my_bool_4_accum(int *accum, bool in, uint y) { }

#pragma rs reduce(my_bool_5) accumulator(my_bool_5_accum) combiner(combiner)
static void my_bool_5_accum(int *accum, bool in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_bool_6) accumulator(my_bool_6_accum) combiner(combiner)
static void my_bool_6_accum(int *accum, bool in, uint x, uint y) { }

#pragma rs reduce(my_bool_7) accumulator(my_bool_7_accum) combiner(combiner)
static void my_bool_7_accum(int *accum, bool in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_bool_8) accumulator(my_bool_8_accum) combiner(combiner)
static void my_bool_8_accum(int *accum, bool in, uint z) { }

#pragma rs reduce(my_bool_9) accumulator(my_bool_9_accum) combiner(combiner)
static void my_bool_9_accum(int *accum, bool in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_bool_10) accumulator(my_bool_10_accum) combiner(combiner)
static void my_bool_10_accum(int *accum, bool in, uint x, uint z) { }

#pragma rs reduce(my_bool_11) accumulator(my_bool_11_accum) combiner(combiner)
static void my_bool_11_accum(int *accum, bool in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_bool_12) accumulator(my_bool_12_accum) combiner(combiner)
static void my_bool_12_accum(int *accum, bool in, uint y, uint z) { }

#pragma rs reduce(my_bool_13) accumulator(my_bool_13_accum) combiner(combiner)
static void my_bool_13_accum(int *accum, bool in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_bool_14) accumulator(my_bool_14_accum) combiner(combiner)
static void my_bool_14_accum(int *accum, bool in, uint x, uint y, uint z) { }

#pragma rs reduce(my_bool_15) accumulator(my_bool_15_accum) combiner(combiner)
static void my_bool_15_accum(int *accum, bool in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_0) accumulator(my_rs_matrix2x2_0_accum) combiner(combiner)
static void my_rs_matrix2x2_0_accum(int *accum, rs_matrix2x2 in) { }

#pragma rs reduce(my_rs_matrix2x2_1) accumulator(my_rs_matrix2x2_1_accum) combiner(combiner)
static void my_rs_matrix2x2_1_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix2x2_2) accumulator(my_rs_matrix2x2_2_accum) combiner(combiner)
static void my_rs_matrix2x2_2_accum(int *accum, rs_matrix2x2 in, uint x) { }

#pragma rs reduce(my_rs_matrix2x2_3) accumulator(my_rs_matrix2x2_3_accum) combiner(combiner)
static void my_rs_matrix2x2_3_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_rs_matrix2x2_4) accumulator(my_rs_matrix2x2_4_accum) combiner(combiner)
static void my_rs_matrix2x2_4_accum(int *accum, rs_matrix2x2 in, uint y) { }

#pragma rs reduce(my_rs_matrix2x2_5) accumulator(my_rs_matrix2x2_5_accum) combiner(combiner)
static void my_rs_matrix2x2_5_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_rs_matrix2x2_6) accumulator(my_rs_matrix2x2_6_accum) combiner(combiner)
static void my_rs_matrix2x2_6_accum(int *accum, rs_matrix2x2 in, uint x, uint y) { }

#pragma rs reduce(my_rs_matrix2x2_7) accumulator(my_rs_matrix2x2_7_accum) combiner(combiner)
static void my_rs_matrix2x2_7_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_rs_matrix2x2_8) accumulator(my_rs_matrix2x2_8_accum) combiner(combiner)
static void my_rs_matrix2x2_8_accum(int *accum, rs_matrix2x2 in, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_9) accumulator(my_rs_matrix2x2_9_accum) combiner(combiner)
static void my_rs_matrix2x2_9_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_10) accumulator(my_rs_matrix2x2_10_accum) combiner(combiner)
static void my_rs_matrix2x2_10_accum(int *accum, rs_matrix2x2 in, uint x, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_11) accumulator(my_rs_matrix2x2_11_accum) combiner(combiner)
static void my_rs_matrix2x2_11_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_12) accumulator(my_rs_matrix2x2_12_accum) combiner(combiner)
static void my_rs_matrix2x2_12_accum(int *accum, rs_matrix2x2 in, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_13) accumulator(my_rs_matrix2x2_13_accum) combiner(combiner)
static void my_rs_matrix2x2_13_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_14) accumulator(my_rs_matrix2x2_14_accum) combiner(combiner)
static void my_rs_matrix2x2_14_accum(int *accum, rs_matrix2x2 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix2x2_15) accumulator(my_rs_matrix2x2_15_accum) combiner(combiner)
static void my_rs_matrix2x2_15_accum(int *accum, rs_matrix2x2 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_0) accumulator(my_rs_matrix3x3_0_accum) combiner(combiner)
static void my_rs_matrix3x3_0_accum(int *accum, rs_matrix3x3 in) { }

#pragma rs reduce(my_rs_matrix3x3_1) accumulator(my_rs_matrix3x3_1_accum) combiner(combiner)
static void my_rs_matrix3x3_1_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix3x3_2) accumulator(my_rs_matrix3x3_2_accum) combiner(combiner)
static void my_rs_matrix3x3_2_accum(int *accum, rs_matrix3x3 in, uint x) { }

#pragma rs reduce(my_rs_matrix3x3_3) accumulator(my_rs_matrix3x3_3_accum) combiner(combiner)
static void my_rs_matrix3x3_3_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_rs_matrix3x3_4) accumulator(my_rs_matrix3x3_4_accum) combiner(combiner)
static void my_rs_matrix3x3_4_accum(int *accum, rs_matrix3x3 in, uint y) { }

#pragma rs reduce(my_rs_matrix3x3_5) accumulator(my_rs_matrix3x3_5_accum) combiner(combiner)
static void my_rs_matrix3x3_5_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_rs_matrix3x3_6) accumulator(my_rs_matrix3x3_6_accum) combiner(combiner)
static void my_rs_matrix3x3_6_accum(int *accum, rs_matrix3x3 in, uint x, uint y) { }

#pragma rs reduce(my_rs_matrix3x3_7) accumulator(my_rs_matrix3x3_7_accum) combiner(combiner)
static void my_rs_matrix3x3_7_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_rs_matrix3x3_8) accumulator(my_rs_matrix3x3_8_accum) combiner(combiner)
static void my_rs_matrix3x3_8_accum(int *accum, rs_matrix3x3 in, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_9) accumulator(my_rs_matrix3x3_9_accum) combiner(combiner)
static void my_rs_matrix3x3_9_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_10) accumulator(my_rs_matrix3x3_10_accum) combiner(combiner)
static void my_rs_matrix3x3_10_accum(int *accum, rs_matrix3x3 in, uint x, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_11) accumulator(my_rs_matrix3x3_11_accum) combiner(combiner)
static void my_rs_matrix3x3_11_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_12) accumulator(my_rs_matrix3x3_12_accum) combiner(combiner)
static void my_rs_matrix3x3_12_accum(int *accum, rs_matrix3x3 in, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_13) accumulator(my_rs_matrix3x3_13_accum) combiner(combiner)
static void my_rs_matrix3x3_13_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_14) accumulator(my_rs_matrix3x3_14_accum) combiner(combiner)
static void my_rs_matrix3x3_14_accum(int *accum, rs_matrix3x3 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix3x3_15) accumulator(my_rs_matrix3x3_15_accum) combiner(combiner)
static void my_rs_matrix3x3_15_accum(int *accum, rs_matrix3x3 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_0) accumulator(my_rs_matrix4x4_0_accum) combiner(combiner)
static void my_rs_matrix4x4_0_accum(int *accum, rs_matrix4x4 in) { }

#pragma rs reduce(my_rs_matrix4x4_1) accumulator(my_rs_matrix4x4_1_accum) combiner(combiner)
static void my_rs_matrix4x4_1_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context) { }

#pragma rs reduce(my_rs_matrix4x4_2) accumulator(my_rs_matrix4x4_2_accum) combiner(combiner)
static void my_rs_matrix4x4_2_accum(int *accum, rs_matrix4x4 in, uint x) { }

#pragma rs reduce(my_rs_matrix4x4_3) accumulator(my_rs_matrix4x4_3_accum) combiner(combiner)
static void my_rs_matrix4x4_3_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_rs_matrix4x4_4) accumulator(my_rs_matrix4x4_4_accum) combiner(combiner)
static void my_rs_matrix4x4_4_accum(int *accum, rs_matrix4x4 in, uint y) { }

#pragma rs reduce(my_rs_matrix4x4_5) accumulator(my_rs_matrix4x4_5_accum) combiner(combiner)
static void my_rs_matrix4x4_5_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_rs_matrix4x4_6) accumulator(my_rs_matrix4x4_6_accum) combiner(combiner)
static void my_rs_matrix4x4_6_accum(int *accum, rs_matrix4x4 in, uint x, uint y) { }

#pragma rs reduce(my_rs_matrix4x4_7) accumulator(my_rs_matrix4x4_7_accum) combiner(combiner)
static void my_rs_matrix4x4_7_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_rs_matrix4x4_8) accumulator(my_rs_matrix4x4_8_accum) combiner(combiner)
static void my_rs_matrix4x4_8_accum(int *accum, rs_matrix4x4 in, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_9) accumulator(my_rs_matrix4x4_9_accum) combiner(combiner)
static void my_rs_matrix4x4_9_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_10) accumulator(my_rs_matrix4x4_10_accum) combiner(combiner)
static void my_rs_matrix4x4_10_accum(int *accum, rs_matrix4x4 in, uint x, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_11) accumulator(my_rs_matrix4x4_11_accum) combiner(combiner)
static void my_rs_matrix4x4_11_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_12) accumulator(my_rs_matrix4x4_12_accum) combiner(combiner)
static void my_rs_matrix4x4_12_accum(int *accum, rs_matrix4x4 in, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_13) accumulator(my_rs_matrix4x4_13_accum) combiner(combiner)
static void my_rs_matrix4x4_13_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_14) accumulator(my_rs_matrix4x4_14_accum) combiner(combiner)
static void my_rs_matrix4x4_14_accum(int *accum, rs_matrix4x4 in, uint x, uint y, uint z) { }

#pragma rs reduce(my_rs_matrix4x4_15) accumulator(my_rs_matrix4x4_15_accum) combiner(combiner)
static void my_rs_matrix4x4_15_accum(int *accum, rs_matrix4x4 in, rs_kernel_context context, uint x, uint y, uint z) { }

#pragma rs reduce(my_MyStruct_0) accumulator(my_MyStruct_0_accum) combiner(combiner)
static void my_MyStruct_0_accum(int *accum, MyStruct in) { }

#pragma rs reduce(my_MyStruct_1) accumulator(my_MyStruct_1_accum) combiner(combiner)
static void my_MyStruct_1_accum(int *accum, MyStruct in, rs_kernel_context context) { }

#pragma rs reduce(my_MyStruct_2) accumulator(my_MyStruct_2_accum) combiner(combiner)
static void my_MyStruct_2_accum(int *accum, MyStruct in, uint x) { }

#pragma rs reduce(my_MyStruct_3) accumulator(my_MyStruct_3_accum) combiner(combiner)
static void my_MyStruct_3_accum(int *accum, MyStruct in, rs_kernel_context context, uint x) { }

#pragma rs reduce(my_MyStruct_4) accumulator(my_MyStruct_4_accum) combiner(combiner)
static void my_MyStruct_4_accum(int *accum, MyStruct in, uint y) { }

#pragma rs reduce(my_MyStruct_5) accumulator(my_MyStruct_5_accum) combiner(combiner)
static void my_MyStruct_5_accum(int *accum, MyStruct in, rs_kernel_context context, uint y) { }

#pragma rs reduce(my_MyStruct_6) accumulator(my_MyStruct_6_accum) combiner(combiner)
static void my_MyStruct_6_accum(int *accum, MyStruct in, uint x, uint y) { }

#pragma rs reduce(my_MyStruct_7) accumulator(my_MyStruct_7_accum) combiner(combiner)
static void my_MyStruct_7_accum(int *accum, MyStruct in, rs_kernel_context context, uint x, uint y) { }

#pragma rs reduce(my_MyStruct_8) accumulator(my_MyStruct_8_accum) combiner(combiner)
static void my_MyStruct_8_accum(int *accum, MyStruct in, uint z) { }

#pragma rs reduce(my_MyStruct_9) accumulator(my_MyStruct_9_accum) combiner(combiner)
static void my_MyStruct_9_accum(int *accum, MyStruct in, rs_kernel_context context, uint z) { }

#pragma rs reduce(my_MyStruct_10) accumulator(my_MyStruct_10_accum) combiner(combiner)
static void my_MyStruct_10_accum(int *accum, MyStruct in, uint x, uint z) { }

#pragma rs reduce(my_MyStruct_11) accumulator(my_MyStruct_11_accum) combiner(combiner)
static void my_MyStruct_11_accum(int *accum, MyStruct in, rs_kernel_context context, uint x, uint z) { }

#pragma rs reduce(my_MyStruct_12) accumulator(my_MyStruct_12_accum) combiner(combiner)
static void my_MyStruct_12_accum(int *accum, MyStruct in, uint y, uint z) { }

#pragma rs reduce(my_MyStruct_13) accumulator(my_MyStruct_13_accum) combiner(combiner)
static void my_MyStruct_13_accum(int *accum, MyStruct in, rs_kernel_context context, uint y, uint z) { }

#pragma rs reduce(my_MyStruct_14) accumulator(my_MyStruct_14_accum) combiner(combiner)
static void my_MyStruct_14_accum(int *accum, MyStruct in, uint x, uint y, uint z) { }

#pragma rs reduce(my_MyStruct_15) accumulator(my_MyStruct_15_accum) combiner(combiner)
static void my_MyStruct_15_accum(int *accum, MyStruct in, rs_kernel_context context, uint x, uint y, uint z) { }
