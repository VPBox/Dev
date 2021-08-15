// -target-api 22
#pragma version(1)
#pragma rs java_package_name(foo)

char out;
rs_allocation al;

void foo(char in) {
  out = rsClamp(in, (char) 1, (char) 4);
  al = rsGetAllocation((void*) 33);
}
