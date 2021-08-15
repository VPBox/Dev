// -target-api 0
#pragma version(1)
#pragma rs java_package_name(com.example.foo)

void oldFoo(const int* a, int *b) {
  *b = *a;
}

int RS_KERNEL foo(int a) {
  return a;
}

void testStart(rs_allocation in, rs_allocation out) {
  rsForEach(foo, in, out);
}
