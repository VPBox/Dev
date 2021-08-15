#pragma version(1)

#pragma rs java_package_name(com.android.rs.test)

struct Inner_2 {
  int i;
};

// Mismatching field types in Middle
struct Middle {
  struct Inner_2 i1;
};

struct Outer {
  struct Middle m;
};

extern int ret;

extern struct Outer *outer;

void outer_y(void) {
  ret = outer->m.i1.i;
}

