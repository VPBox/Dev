#pragma version(1)
#pragma rs java_package_name(foo)

typedef struct {
  int i;
} myStruct;

/* Test old-style kernel */
void root(const myStruct *in, int *out) {
  *out = in->i;
}

/* Test new-style kernel */
myStruct RS_KERNEL kernel_returning_myStruct(int in) {
  myStruct out = { in };
  return out;
}

int RS_KERNEL kernel_with_myStruct_param(myStruct in) {
  return in.i;
}
