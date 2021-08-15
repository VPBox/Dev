#pragma version(1)

#pragma rs java_package_name(com.android.rs.test)

// Different field type - no "Inner" in odr2.rs
struct Outer {
  int current[2];
};

extern uint32_t uint32_ret;

extern struct Outer *outer;

void outer_y(void) {
  uint32_ret = outer->current[1];
}

