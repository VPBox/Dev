#pragma version(1)
#pragma rs java_package_name(foo)

// There are multiple name mismatches (cc*, dd*) but only the first is
// reported.  This reporting doesn't prevent errors for kernels with
// matched names from being reported.

void aa(const size_t *in) { }

#ifdef __LP64__
void cc64(const int *in) { }
void dd64(const int *in) { }
#else
void cc32(const float *in) { }
void dd32(const float *in) { }
#endif

void RS_KERNEL bb(size_t in) { }

// extra kernel goes unreported because of cc* name mismatch
#ifdef __LP64__
void e(const int *in) { }
#endif
