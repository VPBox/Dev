#pragma version(1)
#pragma rs java_package_name(foo)

static void accum(int *a, int v) { }

#pragma rs reduce(aa) accumulator(accum)
#pragma rs reduce(bb) accumulator(accum)

#ifndef __LP64__
#pragma rs reduce(cc) accumulator(accum)
#pragma rs reduce(dd) accumulator(accum)
#endif
