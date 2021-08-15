#pragma version(1)
#pragma rs java_package_name(foo)

void root_old(const int *ain, int *aout, int x, int y) {
  *aout = *ain + x + y;
}

int RS_KERNEL root_new(int in, int x, int y) {
  return in + x + y;
}
