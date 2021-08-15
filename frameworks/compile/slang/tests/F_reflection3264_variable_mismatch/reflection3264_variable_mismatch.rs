#pragma version(1)
#pragma rs java_package_name(foo)

struct Foo { int f; } foo;
struct Bar { int f; } bar;

size_t a;

int b[sizeof(void*)];

rs_allocation c;

#ifdef __LP64__
const
#endif
int d = 0;

#ifndef __LP64__
const
#endif
int e = 0;

#ifdef __LP64__
struct Foo f;
#else
struct Bar f;
#endif

size_t g[10];

#ifdef __LP64__
int h64;
#else
int h32;
#endif

// the errors after this point should go unreported, because of the
// variable name mismatch above (h32 versus h64)

size_t z;

int y[sizeof(void*)];

rs_allocation x;

#ifdef __LP64__
const
#endif
int w = 0;

#ifndef __LP64__
const
#endif
int v = 0;

#ifdef __LP64__
struct Foo u;
#else
struct Bar u;
#endif

#ifdef __LP64__
int t64;
#else
int t32;
#endif
