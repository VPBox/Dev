// -Wall -Werror -target-api 0
#pragma version(1)
#pragma rs java_package_name(examples)

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(fz) \
  initializer(fzInit) \
  accumulator(fzAccum) combiner(fzCombine) \
  halter(fzFound)

static void fzInit(int *accumIdx) { *accumIdx = -1; }

static void fzAccum(int *accumIdx,
                    int inVal, int x /* special arg */) {
  if (inVal==0) *accumIdx = x;
}

static void fzCombine(int *accumIdx, const int *accumIdx2) {
  if (*accumIdx2 >= 0) *accumIdx = *accumIdx2;
}

// halter function
static bool fzFound(const int *accumIdx) {
  return *accumIdx >= 0;
}

/////////////////////////////////////////////////////////////////////////

#pragma rs reduce(fz2) \
  initializer(fz2Init) \
  accumulator(fz2Accum) combiner(fz2Combine) \
  halter(fz2Found)

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

// halter function
static bool fz2Found(const int2 *accum) {
  return accum->x >= 0;
}
