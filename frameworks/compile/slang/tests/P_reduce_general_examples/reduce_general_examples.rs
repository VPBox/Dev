// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(examples)

// Same as reduce_general_examples_backward.rs, except this test case
// places the pragmas before the functions (forward reference), and
// the other test case places the pragmas after the functions
// (backward reference).

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(addint) \
  accumulator(aiAccum)

static void aiAccum(int *accum, int val) { *accum += val; }

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(mpyint) \
  initializer(mpyInit) accumulator(mpyAccum)

static void mpyInit(int *accum) { *accum = 1; }

static void mpyAccum(int *accum, int val) { *accum *= val; }

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(dp) \
  accumulator(dpAccum) combiner(dpSum)

static void dpAccum(float *accum, float in1, float in2) {
  *accum += in1*in2;
}

// combiner function
static void dpSum(float *accum, const float *val) { *accum += *val; }

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(findMinAndMax) \
  initializer(fMMInit) accumulator(fMMAccumulator) \
  combiner(fMMCombiner) outconverter(fMMOutConverter)

// this is just a compilation test
#define INFINITY 0.0f

typedef struct {
  float val;
  int idx;
} IndexedVal;

typedef struct {
  IndexedVal min, max;
} MinAndMax;

static void fMMInit(MinAndMax *accum) {
  static const MinAndMax r = { { INFINITY, -1 }, { -INFINITY, -1 } };
  *accum = r;
}

static void fMMAccumulator(MinAndMax *accum, float in, int x) {
  IndexedVal me;
  me.val = in;
  me.idx = x;

  if (me.val < accum->min.val)
    accum->min = me;
  if (me.val > accum->max.val)
    accum->max = me;
}

static void fMMCombiner(MinAndMax *accum,
                        const MinAndMax *val) {
  fMMAccumulator(accum, val->min.val, val->min.idx);
  fMMAccumulator(accum, val->max.val, val->max.idx);
}

static void fMMOutConverter(int2 *result,
                            const MinAndMax *val) {
  result->x = val->min.idx;
  result->y = val->max.idx;
}

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(fz) \
  initializer(fzInit) \
  accumulator(fzAccum) combiner(fzCombine)

static void fzInit(int *accumIdx) { *accumIdx = -1; }

static void fzAccum(int *accumIdx,
                    int inVal, int x /* special arg */) {
  if (inVal==0) *accumIdx = x;
}

static void fzCombine(int *accumIdx, const int *accumIdx2) {
  if (*accumIdx2 >= 0) *accumIdx = *accumIdx2;
}

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(fz2) \
  initializer(fz2Init) \
  accumulator(fz2Accum) combiner(fz2Combine)

static void fz2Init(int2 *accum) { accum->x = accum->y = -1; }

static void fz2Accum(int2 *accum,
                     int inVal,
                     int x /* special arg */,
                     int y /* special arg */) {
  if (inVal==0) {
    accum->x = x;
    accum->y = y;
  }
}

static void fz2Combine(int2 *accum, const int2 *accum2) {
  if (accum2->x >= 0) *accum = *accum2;
}

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(histogram) \
  accumulator(hsgAccum) combiner(hsgCombine)

#define BUCKETS 256
typedef uint32_t Histogram[BUCKETS];

static void hsgAccum(Histogram *h, uchar in) { ++(*h)[in]; }

static void hsgCombine(Histogram *accum, const Histogram *addend) {
  for (int i = 0; i < BUCKETS; ++i)
    (*accum)[i] += (*addend)[i];
}

#pragma rs reduce(mode) \
  accumulator(hsgAccum) combiner(hsgCombine) \
  outconverter(modeOutConvert)

static void modeOutConvert(int2 *result, const Histogram *h) {
  uint32_t mode = 0;
  for (int i = 1; i < BUCKETS; ++i)
    if ((*h)[i] > (*h)[mode]) mode = i;
  result->x = mode;
  result->y = (*h)[mode];
}
