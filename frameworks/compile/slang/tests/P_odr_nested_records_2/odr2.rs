#pragma version(1)

#pragma rs java_package_name(com.android.rs.test)

typedef struct Inner {
  uint32_t x;
} Inner2;

struct Outer {
  // Different typedef'd type name than the one in odr1.rs
  // but the actual underlying type names agree
  Inner2 current[2];
};

extern uint32_t uint32_ret;

extern struct Outer *outer;

void outer_y(void) {
  uint32_ret = outer->current[1].x;
}

