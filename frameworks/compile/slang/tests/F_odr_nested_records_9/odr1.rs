#pragma version(1)

#pragma rs java_package_name(com.android.rs.test)

// Struct-of-array-of-struct-of-array-of-struct-of-array-of-unsigned
// Mismatching Inner field types
struct Inner {
  unsigned x[5];
};

struct Middle {
  struct Inner deeper[3];
};

struct Outer {
  struct Middle current[2];
};

extern uint32_t uint32_ret;

extern struct Outer *outer;

void outer_y(void) {
  uint32_ret = outer->current[1].deeper[1].x[3];
}

