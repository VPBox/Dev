// -target-api 23
#pragma version(1)
#pragma rs java_package_name(foo)

int RS_KERNEL good(int in, rs_kernel_context context, uint32_t x) {
  return 0;
}

int RS_KERNEL bad(int in, uint32_t x, rs_kernel_context context) {
  return 0;
}
