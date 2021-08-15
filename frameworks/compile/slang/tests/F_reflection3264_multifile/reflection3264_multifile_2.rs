#pragma version(1)
#pragma rs java_package_name(foo)

short v;

short w;

void f2(int a) { }

// error: invokable is only present for 32-bit targets
#ifndef __LP64__
void g() { }
#endif

// error: 'in2' has different types for 32-bit and 64-bit targets
void RS_KERNEL m(int in1, size_t in2) { }
