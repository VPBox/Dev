// -Wall
#pragma version(1)
#pragma rs java_package_name(export)

// trivial accumulator for use multiple times later
static void AccumInt(int *accum, int val) { }

// NOTE: union and bitfield errors are only emitted once per type, so
//       we have multiple types here to ensure that we issue errors
//       in multiple contexts.

// NOTE: accumulator data type is restricted in certain ways regardless
//       of result type; we do not test that here.  This is covered
//       in the test case reduce_general_bad_accumulator.rs.

/////////////////////////////////////////////////////////////////////////

typedef unsigned int UIntA916[9][16];

// result type cannot be multidimensional array
#pragma rs reduce(redUInt2dA) accumulator(AccumUInt2dA) combiner(CombUInt2dA)
static void AccumUInt2dA(UIntA916 *accum, int val) { }
static void CombUInt2dA(UIntA916 *accum, const UIntA916 *val) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redUInt2dAOutFix) accumulator(AccumUInt2dA) combiner(CombUInt2dA) \
  outconverter(OutUInt2dAFix)
static void OutUInt2dAFix(uint *out, const UIntA916 *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redUInt2dAOutBreak) accumulator(AccumInt) outconverter(OutUInt2dABreak)
static void OutUInt2dABreak(UIntA916 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

union UnionShortDouble {
  short s;
  double d;
};

union UnionCharInt {
  char c;
  int i;
};

union UnionLongFloat {
  long l;
  float f;
};

// result type cannot be union
#pragma rs reduce(redUnionShortDouble) accumulator(AccumUnionShortDouble) \
  combiner(CombineUnionShortDouble)
static void AccumUnionShortDouble(union UnionShortDouble *accum, short s, double d) { }
static void CombineUnionShortDouble(union UnionShortDouble *accum,
                                    const union UnionShortDouble *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redUnionCharIntOutFix) accumulator(AccumUnionCharInt) \
  combiner(CombineUnionCharInt) outconverter(OutUnionCharIntFix)
static void AccumUnionCharInt(union UnionCharInt *accum, short s, double d) { }
static void CombineUnionCharInt(union UnionCharInt *accum, const union UnionCharInt *other) { }
static void OutUnionCharIntFix(int *out, const union UnionCharInt *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redUnionLongFloatOutBreak) accumulator(AccumInt) \
  outconverter(OutUnionLongFloatBreak)
static void OutUnionLongFloatBreak(union UnionLongFloat *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

union UnionIntFloat {
  short s;
  double d;
};
typedef union UnionIntFloat UnionIntFloatArray[10];

union UnionDoubleFloat {
  double d;
  float f;
};
typedef union UnionDoubleFloat UnionDoubleFloatArray[10];

union UnionCharLong {
  char c;
  long l;
};
typedef union UnionCharLong UnionCharLongArray[10];

// cannot export array of union
#pragma rs reduce(redUnionIntFloatArray) accumulator(AccumUnionIntFloatArray) \
  combiner(CombineUnionIntFloatArray)
static void AccumUnionIntFloatArray(UnionIntFloatArray *accum, int val) { }
static void CombineUnionIntFloatArray(UnionIntFloatArray *accum,
                                      const UnionIntFloatArray *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redUnionDoubleFloatArrayOutFix) accumulator(AccumUnionDoubleFloatArray) \
  combiner(CombineUnionDoubleFloatArray) outconverter(OutUnionDoubleFloatArrayFix)
static void AccumUnionDoubleFloatArray(UnionDoubleFloatArray *accum, short s, double d) { }
static void CombineUnionDoubleFloatArray(UnionDoubleFloatArray *accum,
                                           const UnionDoubleFloatArray *other) { }
static void OutUnionDoubleFloatArrayFix(int *out, const UnionDoubleFloatArray *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redUnionCharLongArrayOutBreak) accumulator(AccumInt) \
  outconverter(OutUnionCharLongArrayBreak)
static void OutUnionCharLongArrayBreak(UnionCharLongArray *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct Bits1 {
  int b:1;
};

struct Bits11 {
  int c:1;
  int d:1;
};

struct Bits111 {
  int e:1;
  int f:1;
  int g:1;
};

// cannot export struct with bitfield
#pragma rs reduce(redBits) accumulator(AccumBits) combiner(CombineBits)
static void AccumBits(struct Bits1 *accum, int val) { }
static void CombineBits(struct Bits1 *accum, const struct Bits1 *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redBitsOutFix) accumulator(AccumBitsOutFix) combiner(CombineBitsOutFix) \
  outconverter(OutBitsFix)
static void AccumBitsOutFix(struct Bits11 *accum, int val) { }
static void CombineBitsOutFix(struct Bits11 *accum, const struct Bits11 *other) { }
static void OutBitsFix(int *out, const struct Bits11 *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redBitsOutBreak) accumulator(AccumInt) outconverter(OutBitsBreak)
static void OutBitsBreak(struct Bits111 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct Bits2 {
  int b:2;
};
typedef struct Bits2 Bits2Array[10];

struct Bits22 {
  int c:2;
  int d:2;
};
typedef struct Bits22 Bits22Array[10];

struct Bits222 {
  int e:2;
  int f:2;
  int g:2;
};
typedef struct Bits222 Bits222Array[10];

// cannot export array of struct with bitfield
#pragma rs reduce(redBitsArray) accumulator(AccumBitsArray) combiner(CombineBitsArray)
static void AccumBitsArray(Bits2Array *accum, int val) { }
static void CombineBitsArray(Bits2Array *accum, const Bits2Array *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redBitsArrayOutFix) accumulator(AccumBitsArrayOutFix) \
  combiner(CombineBitsArrayOutFix) outconverter(OutBitsArrayFix)
static void AccumBitsArrayOutFix(Bits22Array *accum, int val) { }
static void CombineBitsArrayOutFix(Bits22Array *accum, const Bits22Array *other) { }
static void OutBitsArrayFix(int *out, const Bits22Array *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redBitsArrayOutBreak) accumulator(AccumInt) outconverter(OutBitsArrayBreak)
static void OutBitsArrayBreak(Bits222Array *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

// result type cannot be matrix
#pragma rs reduce(redMat) accumulator(AccumMat)
static void AccumMat(rs_matrix2x2 *accum, rs_matrix2x2 val) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redMatOutFix) accumulator(AccumMat) outconverter(OutMatFix)
static void OutMatFix(int *out, const rs_matrix2x2 *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redMatOutBreak) accumulator(AccumInt) outconverter(OutMatBreak)
static void OutMatBreak(rs_matrix2x2 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

typedef rs_matrix2x2 ArrayMatrix2x2[10];

// result type cannot be array of matrix
#pragma rs reduce(redMatArray) accumulator(AccumMatArray) combiner(CombineMatArray)
static void AccumMatArray(ArrayMatrix2x2 *accum, rs_matrix2x2 val) { }
static void CombineMatArray(ArrayMatrix2x2 *accum, const ArrayMatrix2x2 *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redMatArrayOutFix) accumulator(AccumMatArray) combiner(CombineMatArray) \
  outconverter(OutMatArrayFix)
static void OutMatArrayFix(int *out, const ArrayMatrix2x2 *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redMatArrayOutBreak) accumulator(AccumInt) outconverter(OutMatArrayBreak)
static void OutMatArrayBreak(ArrayMatrix2x2 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

// result type cannot be object type
#pragma rs reduce(redAlloc) accumulator(AccumInt) outconverter(OutAlloc)
static void OutAlloc(rs_allocation *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

typedef rs_allocation AllocArray[10];

// result type cannot be array of object type
#pragma rs reduce(redAllocArray) accumulator(AccumAllocArray) outconverter(OutAllocArray)
static void AccumAllocArray(int *accum, int val) { }
static void OutAllocArray(AllocArray *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct StructObj1 {
  int f;
  rs_element e;
};

// result type cannot be struct containing object type
#pragma rs reduce(redStructObj) accumulator(AccumStructObj) outconverter(OutStructObj)
static void AccumStructObj(int *accum, int val) { }
static void OutStructObj(struct StructObj1 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct StructObj2 {
  rs_type t;
  float f;
};
typedef struct StructObj2 StructObj2Array[10];

// result type cannot be array of struct containing object type
#pragma rs reduce(redStructObjArray) accumulator(AccumStructObjArray) \
  outconverter(OutStructObjArray)
static void AccumStructObjArray(int *accum, int val) { }
static void OutStructObjArray(StructObj2Array *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

typedef char *Ptr;

// result type cannot be pointer
#pragma rs reduce(redPtr) accumulator(AccumPtr) combiner(CombPtr)
static void AccumPtr(Ptr *accum, char val) { }
static void CombPtr(Ptr *accum, const Ptr *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redPtrOutFix) accumulator(AccumPtr) combiner(CombPtr) \
  outconverter(OutPtrFix)
static void OutPtrFix(int *out, const Ptr *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redPtrOutBreak) accumulator(AccumInt) outconverter(OutPtrBreak)
static void OutPtrBreak(Ptr *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

typedef Ptr PtrArray7[7];

// cannot export array of pointer
#pragma rs reduce(redPtrArray7) accumulator(AccumPtrArray7) combiner(CombPtrArray7)
static void AccumPtrArray7(PtrArray7 *accum, int val) { }
static void CombPtrArray7(PtrArray7 *accum, const PtrArray7 *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redPtrArray7OutFix) accumulator(AccumPtrArray7) combiner(CombPtrArray7) \
  outconverter(OutPtrArray7Fix)
static void OutPtrArray7Fix(int *out, const PtrArray7 *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redPtrArray7OutBreak) accumulator(AccumInt) outconverter(OutPtrArray7Break)
static void OutPtrArray7Break(PtrArray7 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct StructPtr1 {
  Ptr p;
  double d;
};

struct StructPtr11 {
  Ptr p;
  double d;
  int i;
};

struct StructPtr111 {
  Ptr p;
  double d;
  int i;
  float f;
};

// cannot export struct containing pointer
#pragma rs reduce(redStructPtr) accumulator(AccumStructPtr) combiner(CombStructPtr)
static void AccumStructPtr(struct StructPtr1 *accum, int val) { }
static void CombStructPtr(struct StructPtr1 *accum, const struct StructPtr1 *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redStructPtrOutFix) accumulator(AccumStructPtrOutFix) \
  combiner(CombStructPtrOutFix) outconverter(OutStructPtrFix)
static void AccumStructPtrOutFix(struct StructPtr11 *accum, int val) { }
static void CombStructPtrOutFix(struct StructPtr11 *accum, const struct StructPtr11 *other) { }
static void OutStructPtrFix(int *out, const struct StructPtr11 *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redStructPtrOutBreak) accumulator(AccumInt) outconverter(OutStructPtrBreak)
static void OutStructPtrBreak(struct StructPtr111 *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct StructPtr2 {
  float f;
  Ptr p;
};
typedef struct StructPtr2 StructPtr2Array[7];

struct StructPtr22 {
  float f;
  Ptr p;
  char c;
};
typedef struct StructPtr22 StructPtr22Array[7];

struct StructPtr222 {
  float f;
  Ptr p;
  char c;
  double d;
};
typedef struct StructPtr222 StructPtr222Array[7];

// cannot export struct containing pointer
#pragma rs reduce(redStructPtrArray) accumulator(AccumStructPtrArray) combiner(CombStructPtrArray)
static void AccumStructPtrArray(StructPtr2Array *accum, int val) { }
static void CombStructPtrArray(StructPtr2Array *accum, const StructPtr2Array *other) { }

// problem is result type, not accumulator type, so outconverter can fix this
#pragma rs reduce(redStructPtrArrayOutFix) accumulator(AccumStructPtrArrayOutFix) \
  combiner(CombStructPtrArrayOutFix) outconverter(OutStructPtrArrayFix)
static void AccumStructPtrArrayOutFix(StructPtr22Array *accum, int val) { }
static void CombStructPtrArrayOutFix(StructPtr22Array *accum, const StructPtr22Array *other) { }
static void OutStructPtrArrayFix(int *out, const StructPtr22Array *val) { }

// problem is result type, not accumulator type, so outconverter can break this
#pragma rs reduce(redStructPtrArrayOutBreak) accumulator(AccumInt) \
  outconverter(OutStructPtrArrayBreak)
static void OutStructPtrArrayBreak(StructPtr22Array *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

typedef void Fn(void);

// cannot export function type
//
// NOTE: Not suitable as accumulator data type, regardless of whether
//       we have outconverter; that's tested in reduce_general_bad_accumulator.rs.
#pragma rs reduce(redFn) accumulator(AccumInt) outconverter(OutFn)
static void OutFn(Fn *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

struct Incomplete;

// result type cannot be incomplete type
//
// NOTE: Not suitable as accumulator data type, regardless of whether
//       we have outconverter; that's tested in reduce_general_bad_accumulator.rs.
#pragma rs reduce(redStructIncomplete) accumulator(AccumInt) outconverter(OutStructIncomplete)
static void OutStructIncomplete(struct Incomplete *out, const int *val) { }

/////////////////////////////////////////////////////////////////////////

typedef int IntA[]; // incomplete (unknown element count)

// result type cannot be incomplete type
//
// NOTE: Not suitable as accumulator data type, regardless of whether
//       we have outconverter; that's tested in reduce_general_bad_accumulator.rs.
#pragma rs reduce(redArrayIncomplete) accumulator(AccumInt) outconverter(OutArrayIncomplete)
static void OutArrayIncomplete(IntA *out, const int *val) { }
