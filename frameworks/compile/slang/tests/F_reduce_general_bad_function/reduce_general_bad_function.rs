// -Wall
#pragma version(1)
#pragma rs java_package_name(fn)

// Various ways particular reduction consituent functions can fail semantic checks.
// Also see reduce_general_bad_halter.rs for halter function semantic checks.
// Also see reduce_general_bad_accumulator.rs for accumulator data type checks.

// trivial accumulator for use multiple times later
static void AccumInt(int *accum, int val) { }

/////////////////////////////////////////////////////////////////////////////
// initializer
/////////////////////////////////////////////////////////////////////////////

// initializer must take exactly 1 parameter
#pragma rs reduce(init0) initializer(init0) accumulator(AccumInt)
static void init0() { }

// initializer must take exactly 1 parameter
#pragma rs reduce(init2) initializer(init2) accumulator(AccumInt)
static void init2(int *a, int *b) { }

// initializer cannot take special parameter
#pragma rs reduce(init_special1) initializer(init_special1) accumulator(AccumInt)
static void init_special1(int *x) { }

// initializer must take exactly 1 parameter
#pragma rs reduce(init2_special1) initializer(init2_special1) accumulator(AccumInt)
static void init2_special1(int *a, int *x) { }

// initializer must take exactly 1 parameter
#pragma rs reduce(init2_special0) initializer(init2_special0) accumulator(AccumInt)
static void init2_special0(int *x, int *a) { }

// initializer must take pointer parameter
#pragma rs reduce(init_noptr) initializer(init_noptr) accumulator(AccumInt)
static void init_noptr(int a) { }

// initializer may be overloadable . . .
#pragma rs reduce(init_over) initializer(init_over) accumulator(AccumInt)
static __attribute__((overloadable)) void init_over(int *accum) { }

// . . . but initializer must not have duplicate definitions
#pragma rs reduce(init_dup) initializer(init_dup) accumulator(AccumInt)
static __attribute__((overloadable)) void init_dup(int *accum) { }
static __attribute__((overloadable)) void init_dup(unsigned int *accum) { }

// initializer must be present
#pragma rs reduce(init_missing) initializer(init_missing) accumulator(AccumInt)

// initializer must be static
#pragma rs reduce(init_nonstatic) initializer(init_nonstatic) accumulator(AccumInt)
void init_nonstatic(int *accum) { }

// initializer must return void
#pragma rs reduce(init_nonvoid) initializer(init_nonvoid) accumulator(AccumInt)
static int init_nonvoid(int *accum) { return 0; }

// initializer parameter must point to non-const
#pragma rs reduce(init_const) initializer(init_const) accumulator(AccumInt)
static void init_const(const int *accum) { }

// . . . but initializer parameter VALUE can be const
#pragma rs reduce(init_valconst) initializer(init_valconst) accumulator(AccumInt)
static void init_valconst(int *const accum) { }

/////////////////////////////////////////////////////////////////////////////
// accumulator
/////////////////////////////////////////////////////////////////////////////

// accumulator must take at least 2 parameters
#pragma rs reduce(accum0) accumulator(accum0)
static void accum0() { }

// accumulator must take at least 2 parameters
#pragma rs reduce(accum1) accumulator(accum1)
static void accum1(int a) { }

// accumulator first parameter must be of pointer type
#pragma rs reduce(accum_noptr) accumulator(accum_noptr)
static void accum_noptr(int accum, int val) { }

// accumulator and initializer must take pointers to same type
#pragma rs reduce(accum_vs_init) initializer(avi_init) accumulator(avi_accum)
static void avi_init(int *accum) { }
static void avi_accum(double *accum, int val) { }

// accumulator must have at least 1 input
#pragma rs reduce(accum_special) accumulator(accum_special)
static void accum_special(int *accum, int x) { }

// accumulator input must not be a pointer . . .
#pragma rs reduce(accum_ptr) accumulator(accum_ptr) combiner(comb_ptr)
static void accum_ptr(int *accum, char *val) { }
static void comb_ptr(int *accum, const int *other) { }

// . . . including a pointer from array decay
#pragma rs reduce(accum_arr) accumulator(accum_arr) combiner(comb_arr)
typedef char CharArray[10];
static void accum_arr(int *accum, CharArray val) { }
static void comb_arr(int *accum, const int *other) { }

// accumulator input must not contain object type
#pragma rs reduce(accum_obj) accumulator(accum_obj) combiner(comb_obj)
static void accum_obj(int *accum, int val, rs_element elem) { }
static void comb_obj(int *accum, const int *other) { }

// accumulator may be overloadable . . .
#pragma rs reduce(accum_over) accumulator(accum_over)
static __attribute__((overloadable)) void accum_over(int *accum, int val) { }

// . . . but accumulator must not have duplicate definitions
#pragma rs reduce(accum_dup) accumulator(accum_dup)
static __attribute__((overloadable)) void accum_dup(int *accum, int val) { }
static __attribute__((overloadable)) void accum_dup(uint *accum, uint val) { }

// accumulator must be present
#pragma rs reduce(accum_missing) accumulator(accum_missing)

// accumulator must be static
#pragma rs reduce(accum_nonstatic) accumulator(accum_nonstatic)
void accum_nonstatic(int *accum, int val) { }

// accumulator must return void
#pragma rs reduce(accum_nonvoid) accumulator(accum_nonvoid)
static int accum_nonvoid(int *accum, int val) { return 0; }

// first accumulator parameter must point to non-const . . .
#pragma rs reduce(accum_const) accumulator(accum_const)
static void accum_const(const int *accum, int val) { }

// . . . but accumulator parameter VALUES can be const
#pragma rs reduce(accum_valconst) accumulator(accum_valconst)
static void accum_valconst(int *const accum, const int val) { }

/////////////////////////////////////////////////////////////////////////////
// combiner
/////////////////////////////////////////////////////////////////////////////

// combiner must take exactly 2 parameters
#pragma rs reduce(comb0) accumulator(AccumInt) combiner(comb0)
static void comb0() { }

// combiner must take exactly 2 parameters
#pragma rs reduce(comb1) accumulator(AccumInt) combiner(comb1)
static void comb1(int *accum) { }

// combiner must take exactly 2 parameters
#pragma rs reduce(comb3) accumulator(AccumInt) combiner(comb3)
static void comb3(int *accum, const int *otherA, int otherB) { }

// combiner and accumulator must take pointers to same type
#pragma rs reduce(comb_vs_accum) accumulator(cva_accum) combiner(cva_comb)
static void cva_accum(int *accum, char val) { }
static void cva_comb(unsigned *accum, const unsigned *other) { }

// accumulator must have 1 input and no specials to omit combiner
#pragma rs reduce(accum_2in) accumulator(accum_2in)
static void accum_2in(int *accum, int in1, int in2) { }

// accumulator must have 1 input and no specials to omit combiner
#pragma rs reduce(accum_special_1in) accumulator(accum_special_1in)
static void accum_special_1in(int *accum, int in1, int x) { }

// accumulator parameter types must agree to omit combiner
#pragma rs reduce(accum_types) accumulator(accum_types)
static void accum_types(int *accum, unsigned val) { }

// combiner may be overloadable . . .
#pragma rs reduce(comb_over) accumulator(AccumInt) combiner(comb_over)
static __attribute__((overloadable)) void comb_over(int *accum, const int *other) { }

// . . . but combiner must not have duplicate definitions
#pragma rs reduce(comb_dup) accumulator(AccumInt) combiner(comb_dup)
static __attribute__((overloadable)) void comb_dup(int *accum, const int *other) { }
static __attribute__((overloadable)) void comb_dup(uint *accum, const uint *other) { }

// combiner must be present
#pragma rs reduce(comb_missing) accumulator(AccumInt) combiner(comb_missing)

// combiner must be static
#pragma rs reduce(comb_nonstatic) accumulator(AccumInt) combiner(comb_nonstatic)
void comb_nonstatic(int *accum, const int *other) { }

// combiner must return void
#pragma rs reduce(comb_nonvoid) accumulator(AccumInt) combiner(comb_nonvoid)
static int comb_nonvoid(int *accum, const int *other) { return 0; }

// first combiner parameter must point to non-const and second to const . . .
#pragma rs reduce(comb_const) accumulator(AccumInt) combiner(comb_const)
static void comb_const(const int *accum, int *other) { }

// . . . but combiner parameter VALUES can be const
#pragma rs reduce(comb_valconst) accumulator(AccumInt) combiner(comb_valconst)
static void comb_valconst(int *const accum, const int *other) { }

/////////////////////////////////////////////////////////////////////////////
// outconverter
/////////////////////////////////////////////////////////////////////////////

// outconverter must take exactly 2 parameters
#pragma rs reduce(out0) accumulator(AccumInt) outconverter(out0)
static void out0() { }

// outconverter must take exactly 2 parameters
#pragma rs reduce(out1) accumulator(AccumInt) outconverter(out1)
static void out1(char *out) { }

// outconverter must take exactly 2 parameters
#pragma rs reduce(out3) accumulator(AccumInt) outconverter(out3)
static void out3(char *out, const int *val, int foo) { }

// outconverter cannot take special parameter
#pragma rs reduce(out_special) accumulator(AccumInt) outconverter(out_special)
static void out_special(char *out, const int *y) { }

// outconverter must take pointer
#pragma rs reduce(out_ptr1) accumulator(AccumInt) outconverter(out_ptr1)
static void out_ptr1(int out, const int *val) { }

// outconverter must take pointer
#pragma rs reduce(out_ptr2) accumulator(AccumInt) outconverter(out_ptr2)
static void out_ptr2(int *out, const int val) { }

// outconverter and accumulator must take pointers to same type
#pragma rs reduce(out_vs_accum) accumulator(AccumInt) outconverter(ova_out)
static void ova_out(int *out, const double *val) { }

// outconverter may be overloadable . . .
#pragma rs reduce(out_over) accumulator(AccumInt) outconverter(out_over)
static __attribute__((overloadable)) void out_over(double *accum, const int *val) { }

// . . . but outconverter must not have duplicate definitions
#pragma rs reduce(out_dup) accumulator(AccumInt) outconverter(out_dup)
static __attribute__((overloadable)) void out_dup(double *out, const int *val) { }
static __attribute__((overloadable)) void out_dup(float *out, const int *val) { }

// ouconverter must be present
#pragma rs reduce(out_missing) accumulator(AccumInt) outconverter(out_missing)

// outconverter must be static
#pragma rs reduce(out_nonstatic) accumulator(AccumInt) outconverter(out_nonstatic)
void out_nonstatic(int *out, const int *val) { }

// outconverter must return void
#pragma rs reduce(out_nonvoid) accumulator(AccumInt) outconverter(out_nonvoid)
static int out_nonvoid(int *out, const int *val) { return 0; }

// first outconverter parameter must point to non-const and second to const . . .
#pragma rs reduce(out_const) accumulator(AccumInt) outconverter(out_const)
static void out_const(const int *out, int *val) { }

// . . . but outconverter parameter VALUES can be const
#pragma rs reduce(out_valconst) accumulator(AccumInt) outconverter(out_valconst)
static void out_valconst(int *const out, const int *val) { }
