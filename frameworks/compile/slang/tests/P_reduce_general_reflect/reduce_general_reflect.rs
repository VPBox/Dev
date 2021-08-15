// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(reflect)

// A few interesting reflection cases

//////////////////////////////////////////////////////////////////

// A given result type should only be reflected once

#pragma rs reduce(d1) accumulator(d1_accum) outconverter(d1_out)
static void d1_accum(float *accum, float val) { }
static void d1_out(double *out, const float *val) { }

#pragma rs reduce(d2) accumulator(d2_accum)
static void d2_accum(double *accum, double val) { }

//////////////////////////////////////////////////////////////////

// Arrays of different sizes are reflected separately

#pragma rs reduce(a3) accumulator(a3_accum) combiner(a3_comb)
typedef int IntA3[3];
static void a3_accum(IntA3 *accum, int val) { }
static void a3_comb(IntA3 *accum, const IntA3 *other) { }

#pragma rs reduce(a7) accumulator(a7_accum) combiner(a7_comb)
typedef int IntA7[7];
static void a7_accum(IntA7 *accum, int val) { }
static void a7_comb(IntA7 *accum, const IntA7 *other) { }
