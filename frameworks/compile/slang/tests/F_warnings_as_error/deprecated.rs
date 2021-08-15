// -Werror
#pragma version(1)
#pragma rs java_package_name(foo)

char out;
rs_allocation al;

static void func1() {
  int a = func0(3);
}

static int func2(char in) {
  int t = 4;
  //return func1(in + 3);
}
