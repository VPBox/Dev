// -Wall
#pragma version(1)
#pragma rs java_package_name(voidptr)

// void is not a suitable accumulator data type or result type; and
// note that an input cannot be a pointer

// accumulator must not be void
#pragma rs reduce(voidA) initializer(voidA_init) accumulator(voidA_accum) combiner(voidA_comb)
static void voidA_init(void *accum) { }
static void voidA_accum(void *accum, int val) { }
static void voidA_comb(void *accum, const void *other) { }

// input must not be void*
#pragma rs reduce(voidI) accumulator(voidI_accum) combiner(voidI_comb)
static void voidI_accum(int *accum, void *val) { }
static void voidI_comb(int *accum, const int *other) { }

// result type must not be void
#pragma rs reduce(voidO) accumulator(voidO_accum) outconverter(voidO_out)
static void voidO_accum(int *accum, int val) { }
static void voidO_out(void *out, const int *val) { }
