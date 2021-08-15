#pragma version(1)
#pragma rs java_package_name(foo)

int RS_KERNEL root(uint32_t ain) {
  return 0;
}

void RS_KERNEL in_only(uint32_t ain) {
}

int RS_KERNEL out_only() {
  return 0;
}

int RS_KERNEL everything(uint32_t ain, uint32_t x, uint32_t y) {
  return 0;
}

