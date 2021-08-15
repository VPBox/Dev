#ifndef UTILS_RSH
#define UTILS_RSH

#define RS_MSG_TEST_PASSED 100
#define RS_MSG_TEST_FAILED 101

#define DEFINE_CHECK(T)                                                 \
  static int __attribute__((overloadable))                              \
  checkEq(T expected, T actual) {                                       \
    return (expected == actual);                                        \
  }

#define DEFINE_CHECK2(T)                                                \
  static int __attribute__((overloadable))                              \
  checkEq(T##2 expected, T##2 actual) {                                 \
    return (expected.x == actual.x && expected.y == actual.y);          \
  }

#define DEFINE_CHECK4(T)                                                \
  static int __attribute__((overloadable))                              \
  checkEq(T##4 expected, T##4 actual) {                                 \
    return (expected.x == actual.x &&                                   \
            expected.y == actual.y &&                                   \
            expected.z == actual.z &&                                   \
            expected.w == actual.w);                                    \
  }

#define DEFINE_GET(T)                           \
  static T __attribute__((overloadable))        \
  get_##T(rs_allocation a, int x) {             \
    return rsGetElementAt_##T(a, x);            \
  }

#define HANDLE_TYPE(T)                                   \
  DEFINE_GET(T)                                          \
  DEFINE_CHECK(T)                                        \
  DEFINE_TEST(T)                                         \
  DEFINE_GET(T##2)                                       \
  DEFINE_CHECK2(T)                                       \
  DEFINE_TEST(T##2)                                      \
  DEFINE_GET(T##4)                                       \
  DEFINE_CHECK4(T)                                       \
  DEFINE_TEST(T##4)                                      \

#endif  // UTILS_RSH
