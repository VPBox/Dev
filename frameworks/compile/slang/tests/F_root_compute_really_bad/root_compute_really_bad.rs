#pragma version(1)
#pragma rs java_package_name(foo)

int root(int ain, int aout, int usrData, float x1, double y1,
         uint32_t y, uint32_t x,
         uint32_t extra1, uint32_t extra2) {
    return 10;
}

void root2(int* ain, const int* aout, int* usrData, const float* x1,
           const double* y1, uint32_t y) {
}

int RS_KERNEL
    root_kernel(int ain, int* aout, int usrData, float x1, double y1,
                uint32_t y, uint32_t x, uint32_t extra1, uint32_t extra2) {
  return 10;
}
