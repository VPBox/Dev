// -target-api 0
#pragma version(1)
#pragma rs java_package_name(com.example.foo)

int RS_KERNEL foo(int a, int b) {
  return a + b;
}

void testStart(rs_allocation in, rs_allocation out) {
  rsForEach(foo, in, in, out);
}
