#pragma version(1)

#pragma rs java_package_name(com.android.rs.test)

// Same field name (x), mismatching field type (int32_t vs. uint32_t)
struct Inner {
  uint32_t x;
};

struct Outer {
  struct Inner current;
};

extern uint32_t uint32_ret;

extern struct Outer *outer;

void outer_y(void) {
  uint32_ret = outer->current.x;
}

extern struct Inner foo;
