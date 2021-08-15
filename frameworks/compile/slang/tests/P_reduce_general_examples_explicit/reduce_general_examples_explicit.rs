// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(examples)

// These are alternative versions of some of the test cases from
// reduce_general_examples.rs, except that this test case contains
// explicit definitions for functions that were omitted from the other
// test case.

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(addint_init) \
  initializer(aiiInit) accumulator(aiiAccum)

static void aiiInit(int *accum) { *accum = 0; } // unnecessary

static void aiiAccum(int *accum, int val) { *accum += val; }

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(addint_comb) \
  accumulator(aicAccum) combiner(aicComb)

static void aicAccum(int *accum, int val) { *accum += val; }

static void aicComb(int *accum, const int *other) { *accum += *other; } // unnecessary

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(addint_init_comb) \
  initializer(aiicInit) accumulator(aiicAccum) combiner(aiicComb)

static void aiicInit(int *accum) { *accum = 0; } // unnecessary

static void aiicAccum(int *accum, int val) { *accum += val; }

static void aiicComb(int *accum, const int *other) { *accum += *other; } // unnecessary

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(dp_init) \
  initializer(dpiInit) accumulator(dpiAccum) combiner(dpiSum)

static void dpiInit(float *accum) { *accum = 0.0f; } // unnecessary

static void dpiAccum(float *accum, float in1, float in2) {
  *accum += in1*in2;
}

// combiner function
static void dpiSum(float *accum, const float *val) { *accum += *val; }

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(histogram_init) \
  initializer(hsgiInit) accumulator(hsgiAccum) combiner(hsgiCombine)

#define BUCKETS 256
typedef uint32_t Histogram[BUCKETS];

static void hsgiInit(Histogram *h) {
  for (int i = 0; i < BUCKETS; ++i)
    (*h)[i] = 0;
}

static void hsgiAccum(Histogram *h, uchar in) { ++(*h)[in]; }

static void hsgiCombine(Histogram *accum, const Histogram *addend) {
  for (int i = 0; i < BUCKETS; ++i)
    (*accum)[i] += (*addend)[i];
}
