// -target-api 0 -Wall
#pragma version(1)
#pragma rs java_package_name(struct)

typedef struct MyStruct { int f; } MyStruct;

// No Java array method reflection for kernel with struct input (only Allocation method reflection)
#pragma rs reduce(inStruct) accumulator(inStructAccum) combiner(inStructComb)
static void inStructAccum(int *accum, MyStruct val) { }
static void inStructComb(int *accum, const int *other) { }

// No Java reflection at all for kernel with struct result
#pragma rs reduce(outStruct) accumulator(outStructAccum) outconverter(outStructOut)
static void outStructAccum(int *accum, int val) { }
static void outStructOut(MyStruct *out, const int *accum) { }
