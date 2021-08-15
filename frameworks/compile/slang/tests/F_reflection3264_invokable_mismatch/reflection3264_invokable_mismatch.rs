#pragma version(1)
#pragma rs java_package_name(foo)

#ifdef __LP64__
void a(float f) { }
#else
void a(double f, float g) { }
#endif

void b(int j, size_t k, int l, ssize_t m) { }

#ifdef __LP64__
void c64() { }
#else
void c32() { }
#endif

// the errors after this point should go unreported, because of the
// invokable name mismatch above (c32 versus c64)

#ifdef __LP64__
void z(float f) { }
#else
void z(double f, float g) { }
#endif

void y(int j, size_t k, int l, ssize_t m) { }

#ifdef __LP64__
void x64() { }
#else
void x32() { }
#endif
