// -target-api 0
#pragma version(1)
#pragma rs java_package_name(foo)

int RS_KERNEL good(int in0, int in1, int in2, int in3, int in4, int in5, int in6, int in7) {
  return 0;
}

int RS_KERNEL bad(int in0, int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8) {
  return 0;
}
