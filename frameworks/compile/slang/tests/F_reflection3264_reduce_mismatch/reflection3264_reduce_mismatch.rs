#pragma version(1)
#pragma rs java_package_name(foo)

struct S { int f; } ss;

// accumulator types need not match; function names need not match
#ifdef __LP64__
#pragma rs reduce(aa) accumulator(aaAccum) combiner(aaCombAlpha) outconverter(aaOut)
#else
#pragma rs reduce(aa) accumulator(aaAccum) combiner(aaCombBeta) outconverter(aaOut)
#endif
static void aaAccum(size_t *a, int v) { }
static void
#ifdef __LP64__
aaCombAlpha
#else
aaCombBeta
#endif
(size_t *a, const size_t *other) { }
static void aaOut(int *out, const size_t *a) { }

// which functions are present need not match; special arguments need not match
#ifdef __LP64__
#pragma rs reduce(bb) accumulator(bbAccum) combiner(bbComb) outconverter(bbOut)
static void bbOut(int *out, const int *accum) { }
#else
#pragma rs reduce(bb) accumulator(bbAccum) combiner(bbComb)
#endif
static void bbAccum(int *a, int v,
#ifdef __LP64__
                    int x,
#endif
                    int y) { }
static void bbComb(int *a, const int *b) { }

// result type must match; ordinary argument types must match
#pragma rs reduce(cc) accumulator(ccAccum) combiner(ccComb) outconverter(ccOut)
static void ccAccum(int *accum, char i, size_t j, short k, size_t l) { }
static void ccComb(int *accum, const int *other) { }
static void ccOut(size_t *out, const int *accum) { }

// accumulators must have same number of inputs
#pragma rs reduce(dd) accumulator(ddAccum) combiner(ddComb)
#ifdef __LP64__
static void ddAccum(int *accum, char c) { }
#else
static void ddAccum(int *accum, short a, float b) { }
#endif
static void ddComb(int *accum, const int *other) { }

// here, 32-bit kernel, 64-bit kernel, or both are not reflected in Java
#pragma rs reduce(ee) accumulator(eeAccum) combiner(eeComb) outconverter(eeOut)
#pragma rs reduce(ff) accumulator(ffAccum) combiner(ffComb) outconverter(ffOut)
#pragma rs reduce(gg) accumulator(ggAccum) outconverter(ggOut)
#ifdef __LP64__
#define EETYPE struct S
#define FFTYPE int
#define FFINEXTRA int j
#else
#define EETYPE int
#define FFTYPE struct S
#define FFINEXTRA int j, int k
#endif
static void eeAccum(int *a, int v, size_t k) { }
static void eeComb(int *a, const int *other) { }
static void eeOut(EETYPE *out, const int *a) { }
static void ffAccum(int *a, int v, FFINEXTRA) { }
static void ffComb(int *a, const int *other) { }
static void ffOut(FFTYPE *out, const int *a) { }
static void ggAccum(size_t *a, size_t v) { }
static void ggOut(struct S *out, const size_t *a) { }
