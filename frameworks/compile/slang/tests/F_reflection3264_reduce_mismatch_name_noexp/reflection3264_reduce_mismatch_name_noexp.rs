#pragma version(1)
#pragma rs java_package_name(foo)

struct S { int f; };

static void accumGood(struct S *a, struct S v) { }
static void accumBad(size_t *a, size_t v) { }

#ifdef __LP64__
#pragma rs reduce(a64) accumulator(accumGood)
#else
#pragma rs reduce(a32) accumulator(accumGood)
#endif

// the errors after this point should go unreported, because of the
// name mismatch above (a32 versus a64)

#pragma rs reduce(b) accumulator(accumBad)
