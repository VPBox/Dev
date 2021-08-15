// -Wall -target-api 0
#pragma version(1)
#pragma rs java_package_name(fn)

// Various ways halter can fail semantic checks.
// Also see reduce_general_bad_function.rs for other constituent function semantic checks.
// Also see reduce_general_bad_accumulator.rs for accumulator data type checks.

// trivial accumulator for use multiple times later
static void AccumInt(int *accum, int val) { }

/////////////////////////////////////////////////////////////////////////////
// halter
/////////////////////////////////////////////////////////////////////////////

// halter must return bool
#pragma rs reduce(halt_void) accumulator(AccumInt) halter(halt_void)
static void halt_void(const int *accum) { }

// halter must return bool
#pragma rs reduce(halt_int) accumulator(AccumInt) halter(halt_int)
static int halt_int(const int *accum) { return 0; }

// halter must take exactly 1 parameter
#pragma rs reduce(halt0) accumulator(AccumInt) halter(halt0)
static bool halt0() { return false; }

// halter must take exactly 1 parameter
#pragma rs reduce(halt2) accumulator(AccumInt) halter(halt2)
static bool halt2(const int *accum, const int *accum2) { return false; }

// halter cannot take special parameter
#pragma rs reduce(halt_special) accumulator(AccumInt) halter(halt_special)
static bool halt_special(const int *context) { return false; }

// halter and accumulator must take pointers to same type
#pragma rs reduce(halt_vs_accum) accumulator(AccumInt) halter(hva_halt)
static bool hva_halt(const unsigned *accum) { return false; }

// halter may be overloadable . . .
#pragma rs reduce(halt_over) accumulator(AccumInt) halter(halt_over)
static __attribute__((overloadable)) bool halt_over(const int *accum) { return false; }

// . . . but halter must not have duplicate definitions
#pragma rs reduce(halt_dup) accumulator(AccumInt) halter(halt_dup)
static __attribute__((overloadable)) bool halt_dup(const int *accum) { return false; }
static __attribute__((overloadable)) bool halt_dup(const unsigned int *accum) { return false; }

// halter must be present
#pragma rs reduce(halt_missing) accumulator(AccumInt) halter(halt_missing)

// halter must be static
#pragma rs reduce(halt_nonstatic) accumulator(AccumInt) halter(halt_nonstatic)
bool halt_nonstatic(const int *accum) { return false; }

// halter parameter must point to const
#pragma rs reduce(halt_const) accumulator(AccumInt) halter(halt_const)
static bool halt_const(int *accum) { return false; }
