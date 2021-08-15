#pragma version(1)
#pragma rs java_package_name(foo)

// disagree on RS_KERNEL

#ifdef __LP64__
void aa(int *aout) { }
#else
void RS_KERNEL aa(double in) { }
#endif

#ifdef __LP64__
int RS_KERNEL bb(float fin, double din) { return 0; }
#else
void bb(const long *ain) { }
#endif

// two different non-void* output types

void cc(size_t *out) { }

size_t RS_KERNEL dd() { return 0; }

// one non-void* output type, one void* output type

#ifdef __LP64__
void ee(const int *in, short *out) { }
void ff(const int *in, void *out) { }
#else
void ee(const int *in, void *out) { }
void ff(const int *in, short *out) { }
#endif

// one non-void* output type, one no-output

#ifdef __LP64__
void gg(const int *in, int *out) { }
void hh(const int *in) { }
int RS_KERNEL ii(int v) { return 0; }
void RS_KERNEL jj(int v) { }
#else
void gg(const int *in) { }
void hh(const int *in, int *out) { }
void RS_KERNEL ii(int v) { }
int RS_KERNEL jj(int v) { return 0; }
#endif

// one void* output type, one no-output

#ifdef __LP64__
void kk(const int *in, void *out) { }
void ll(const int *in) { }
#else
void kk(const int *in) { }
void ll(const int *in, void *out) { }
#endif

// disagree on input count

void mm(
#ifdef __LP64__
const int *in,
#endif
  double *out) { }

void RS_KERNEL nn(
#ifdef __LP64__
int in0,
#endif
  size_t v) { }

// disagree on param count

void oo(const size_t *in, size_t *out
#ifdef __LP64__
            , const char *d
#endif
            ) { }

// disagree on special parameters

void pp(const int *in,
#ifdef __LP64__
          int x,
#endif
          int y) { }

void RS_KERNEL qq(int in0, long in1,
#ifdef __LP64__
int x
#else
int y
#endif
                      ) { }

// disagree on input types

void rr(const size_t *in) { }

void ss(
#ifdef __LP64__
const void *in
#else
const char *in
#endif
) { }

void tt(
#ifdef __LP64__
const short *in
#else
const void *in
#endif
) { }

struct S { int f; } s;
struct T { int f; } t;
#ifdef __LP64__
#define QQTYPE struct T
#else
#define QQTYPE struct S
#endif

void RS_KERNEL uu(int a, size_t b, char c, QQTYPE d) { }

// disagree on param types

void vv(const int *a, const size_t *b) { }

// ... note that today, a void* param is ignored for reflection
#ifdef __LP64__
void ww(const int *a, const void *b) { }
void xx(const int *a, const float *b) { }
void yy(const int *a, const void *b) { }
void zz(const int *a) { }
#else
void ww(const int *a, const float *b) { }
void xx(const int *a, const void *b) { }
void yy(const int *a) { }
void zz(const int *a, const void *b) { }
#endif
