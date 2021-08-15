// RUN: %Slang %s

// RUN: %rs-filecheck-wrapper %s --check-prefix=CHECK-LL
//     Check that the data types are properly padded:
// CHECK-LL: %struct.NoBitfield{{(\.[0-9]+)?}} = type { i32, [4 x i8], i64, float, [4 x i8] }
// CHECK-LL: %struct.Bitfield{{(\.[0-9]+)?}} = type { i32, [4 x i8], i64, i8 }
//     Check that only NoBitfield is exported:
// CHECK-LL: !\23rs_export_type = !{![[NODE:[0-9]+]]}
// CHECK-LL: ![[NODE]] = !{!"NoBitfield"}

// RUN: %scriptc-filecheck-wrapper --lang=Java --type=NoBitfield --check-prefix=CHECK-JAVA-STRUCT %s
// CHECK-JAVA-STRUCT:      public static Element createElement(RenderScript rs) {
// CHECK-JAVA-STRUCT-NEXT:     Element.Builder eb = new Element.Builder(rs);
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.I32(rs), "I");
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_1");
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.I64(rs), "L");
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.F32(rs), "F");
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_2");
// CHECK-JAVA-STRUCT-NEXT:     return eb.create();
// CHECK-JAVA-STRUCT-NEXT: }

#pragma version(1)
#pragma rs java_package_name(foo)

// There is a C99 rule (under "Structure and union members") that
// reads "One special guarantee is made in order to simplify the use
// of unions: if a union contains several structures that share a
// common initial sequence, and if the union object currently contains
// one of these structures, it is permitted to inspect the common
// initial part of any of them anywhere that a declaration of the
// completed type of the union is visible. Two structures share a
// common initial sequence if corresponding members have compatible
// types (and, for bit-fields, the same widths) for a sequence of one
// or more initial members."
//
// We want to ensure that the common initial sequences of exported
// and non-exported types have the same layout.

// An exported type (because we declare a global variable of this type)
struct NoBitfield {
    int I;
    // expect 4 bytes of padding here
    long L;
    float F;
    // expect 4 bytes of padding here
};

struct NoBitfield junk;  // just to make this an exported type

// A non-exported type that shares a common initial sequence with NoBitfield
struct Bitfield {
    int I;
    // expect 4 bytes of padding here
    long L;
    uint U:3;
};

union CommonInitialSequence {
    struct NoBitfield nbf;
    struct   Bitfield  bf;
};

static union CommonInitialSequence U, V;

static struct NoBitfield *nbf;
static struct   Bitfield * bf;

// Note: Sets through the exported type (NoBitfield)
void setUnion(long argL, int argI) {
    nbf->L = argL;
    nbf->I = argI;
}

bool failed = false;

// Note: Tests through the non-exported type (Bitfield)
void testUnion(long argL, int argI) {
    failed |= ((bf->I != argI) || (bf->L != argL));
}

// Note: Prevent compiler from optimizing setUnion()/testUnion()
//       to convert indirect accesses through nbf/bf into direct
//       accesses through U or V.
void choose(int i) {
    if (i) {
        nbf = &U.nbf;
         bf = &U. bf;
    } else {
        nbf = &V.nbf;
         bf = &V. bf;
    }
}
