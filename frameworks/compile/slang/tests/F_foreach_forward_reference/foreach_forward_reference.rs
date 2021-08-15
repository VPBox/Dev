// -target-api 0
#pragma version(1)
#pragma rs java_package_name(com.example.foo)

void testStart(rs_allocation in, rs_allocation out) {
  rsForEach(goo, in, out);
}

int RS_KERNEL goo(int a) {
  return a;
}


