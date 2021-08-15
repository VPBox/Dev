// -Wall
#pragma version(1)
#pragma rs java_package_name(accum)

// Several checks for bad accumulator DATA TYPE (not bad accumulator FUNCTION)

/////////////////////////////////////////////////////////////////////////////
// accumulator cannot be incomplete array
/////////////////////////////////////////////////////////////////////////////

typedef int IntA[]; // incomplete (unknown element count)

#pragma rs reduce(redIntA) accumulator(AccumIntA) combiner(CombIntA)
static void AccumIntA(IntA *accum, int val) { }
static void CombIntA(IntA *accum, const IntA *val) { }

// outconverter does not help
#pragma rs reduce(redIntAOut) accumulator(AccumIntA) combiner(CombIntA) outconverter(OutIntA)
static void OutIntA(int *out, const IntA *val) { }

/////////////////////////////////////////////////////////////////////////////
// accumulator cannot be incomplete struct
/////////////////////////////////////////////////////////////////////////////

struct Incomplete;

#pragma rs reduce(redStructIncomplete) accumulator(AccumStructIncomplete) combiner(CombStructIncomplete)
static void AccumStructIncomplete(struct Incomplete *accum, int val) { }
static void CombStructIncomplete(struct Incomplete *accum, const struct Incomplete *other) { }

// outconverter does not help
#pragma rs reduce(redStructIncompleteOut) accumulator(AccumStructIncomplete) combiner(CombStructIncomplete) \
  outconverter(OutStructIncomplete)
static void OutStructIncomplete(int *out, const struct Incomplete *val) { }

/////////////////////////////////////////////////////////////////////////////
// accumulator cannot be function
/////////////////////////////////////////////////////////////////////////////

typedef void Fn(void);

#pragma rs reduce(redFn) accumulator(AccumFn) combiner(CombineFn)
static void AccumFn(Fn *accum, int val) { }
static void CombineFn(Fn *accum, const Fn *other) { }

// outconverter does not help
#pragma rs reduce(redFnOut) accumulator(AccumFn) combiner(CombineFn) outconverter(OutFn)
static void OutFn(int *out, const Fn *val) { }

/////////////////////////////////////////////////////////////////////////////
// accumulator cannot be object
/////////////////////////////////////////////////////////////////////////////

#pragma rs reduce(redObj) accumulator(AccumObj) combiner(CombineObj)
static void AccumObj(rs_element *accum, int val) { }
static void CombineObj(rs_element *accum, const rs_element *other) { }

// outconverter does not help
#pragma rs reduce(redObjOut) accumulator(AccumObj) combiner(CombineObj) outconverter(OutObj)
static void OutObj(int *out, const rs_element *val) { }

