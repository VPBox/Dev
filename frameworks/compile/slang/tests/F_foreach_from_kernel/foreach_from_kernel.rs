// -target-api 0
#pragma version(1)
#pragma rs java_package_name(com.example.foo)

int __attribute__((kernel)) foo(int a) {
  return a;
}

rs_allocation g_in;
rs_allocation g_out;

int __attribute__((kernel)) goo(int a) {
  rsForEach(foo, g_in, g_out);
  return a;
}

void bar() {
  rsForEach(foo, g_in, g_out);
}


