/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "shared.rsh"

// Has a subset of the kernels from reduce.rs.
//
// This test case places the pragmas after the functions (backward
// reference), and the other test case places the pragmas before the
// functions (forward reference).

float negInf, posInf;

/////////////////////////////////////////////////////////////////////////

static void aiAccum(int *accum, int val) { *accum += val; }

#pragma rs reduce(addint) \
  accumulator(aiAccum)

/////////////////////////////////////////////////////////////////////////

typedef struct {
  float val;
  int idx;
} IndexedVal;

typedef struct {
  IndexedVal min, max;
} MinAndMax;

static void fMMInit(MinAndMax *accum) {
  accum->min.val = posInf;
  accum->min.idx = -1;
  accum->max.val = negInf;
  accum->max.idx = -1;
}

static void fMMAccumulator(MinAndMax *accum, float in, int x) {
  IndexedVal me;
  me.val = in;
  me.idx = x;

  if (me.val <= accum->min.val)
    accum->min = me;
  if (me.val >= accum->max.val)
    accum->max = me;
}

static void fMMCombiner(MinAndMax *accum,
                        const MinAndMax *val) {
  if ((accum->min.idx < 0) || (val->min.val < accum->min.val))
    accum->min = val->min;
  if ((accum->max.idx < 0) || (val->max.val > accum->max.val))
    accum->max = val->max;
}

static void fMMOutConverter(int2 *result,
                            const MinAndMax *val) {
  result->x = val->min.idx;
  result->y = val->max.idx;
}

#pragma rs reduce(findMinAndMax) \
  initializer(fMMInit) accumulator(fMMAccumulator) \
  combiner(fMMCombiner) outconverter(fMMOutConverter)

/////////////////////////////////////////////////////////////////////////

static void fzInit(int *accumIdx) { *accumIdx = -1; }

static void fzAccum(int *accumIdx,
                    int inVal, int x /* special arg */) {
  if (inVal==0) *accumIdx = x;
}

static void fzCombine(int *accumIdx, const int *accumIdx2) {
  if (*accumIdx2 >= 0) *accumIdx = *accumIdx2;
}

#pragma rs reduce(fz) \
  initializer(fzInit) \
  accumulator(fzAccum) combiner(fzCombine)

/////////////////////////////////////////////////////////////////////////

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

#pragma rs reduce(fz2) \
  initializer(fz2Init) \
  accumulator(fz2Accum) combiner(fz2Combine)

/////////////////////////////////////////////////////////////////////////

static void fz3Init(int3 *accum) { accum->x = accum->y = accum->z = -1; }

static void fz3Accum(int3 *accum,
                     int inVal,
                     int x /* special arg */,
                     int y /* special arg */,
                     int z /* special arg */) {
  if (inVal==0) {
    accum->x = x;
    accum->y = y;
    accum->z = z;
  }
}

static void fz3Combine(int3 *accum, const int3 *accum2) {
  if (accum2->x >= 0) *accum = *accum2;
}

#pragma rs reduce(fz3) \
  initializer(fz3Init) \
  accumulator(fz3Accum) combiner(fz3Combine)

/////////////////////////////////////////////////////////////////////////

#define BUCKETS 256
typedef uint32_t Histogram[BUCKETS];

static void hsgAccum(Histogram *h, uchar in) { ++(*h)[in]; }

static void hsgCombine(Histogram *accum, const Histogram *addend) {
  for (int i = 0; i < BUCKETS; ++i)
    (*accum)[i] += (*addend)[i];
}

#pragma rs reduce(histogram) \
  accumulator(hsgAccum) combiner(hsgCombine)

static void modeOutConvert(int2 *result, const Histogram *h) {
  uint32_t mode = 0;
  for (int i = 1; i < BUCKETS; ++i)
    if ((*h)[i] > (*h)[mode]) mode = i;
  result->x = mode;
  result->y = (*h)[mode];
}

#pragma rs reduce(mode) \
  accumulator(hsgAccum) combiner(hsgCombine) \
  outconverter(modeOutConvert)
