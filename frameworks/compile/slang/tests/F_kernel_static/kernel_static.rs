#pragma version(1)
#pragma rs java_package_name(foo)

static int gi;

static void not_a_kernel(int i) {
    static int j;
    int k;
    j = i;
}

int RS_KERNEL root(uint32_t ain) {
  static const int ci;
  static int i;
  return 0;
}

static int RS_KERNEL static_kernel() {
  return 0;
}
