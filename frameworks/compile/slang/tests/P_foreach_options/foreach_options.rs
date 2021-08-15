// -target-api 0
#pragma version(1)
#pragma rs java_package_name(com.example.foo)

int RS_KERNEL foo(int a) {
  return a;
}

void testStart(rs_allocation in, rs_allocation out) {
  rs_script_call_t opts = {0};
  opts.xStart=0;
  opts.xEnd = 100;
  rsForEach(foo, in, out);
}
