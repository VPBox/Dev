// -target-api 23
#pragma version(1)
#pragma rs java_package_name(foo)

int RS_KERNEL good1(int in, rs_kernel_context context) {
  return 0;
}

int RS_KERNEL good2(int in, const rs_kernel_context context) {
  return 0;
}

int RS_KERNEL bad3(int in, const rs_kernel_context *context) {
  return 0;
}
