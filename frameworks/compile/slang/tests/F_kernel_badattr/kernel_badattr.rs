#pragma version(1)
#pragma rs java_package_name(foo)

int __attribute__((kernel("unimplemented"))) kernel(int arg) {
  return 0;
}

int __attribute__((kernel(7))) kernel2(int arg) {
  return 0;
}

int __attribute__((kernel("reduce", 1))) kernel3(int arg) {
  return 0;
}

int __attribute__((kernel("reduce"))) kernel4(int arg) {
  return 0;
}
