#pragma version(1)
#pragma rs java_package_name(foo)

void aa(const int *in) { }

void RS_KERNEL bb(int in) { }

#ifdef __LP64__

void cc(const int *in) { }

void RS_KERNEL dd(int in) { }

#endif
