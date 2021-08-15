#ifndef LIBS_VR_LIBDVRCOMMON_INCLUDE_PRIVATE_DVR_TEST_TEST_MACROS_H_
#define LIBS_VR_LIBDVRCOMMON_INCLUDE_PRIVATE_DVR_TEST_TEST_MACROS_H_

#include <gtest/gtest.h>

#include <cmath>

#include <private/dvr/numeric.h>

namespace android {
namespace dvr {

template <int N, typename A, typename B, typename T>
::testing::AssertionResult CmpArrayLikeFloatEq(
    const char* expectedStr, const char* actualStr, const char* toleranceStr,
    const A& expected, const B& actual, const T& tolerance) {
  for (int i = 0; i < N; ++i) {
    if (!IsEqual(expected[i], actual[i], tolerance)) {
      return ::testing::AssertionFailure()
             << "\"" << expectedStr << "\" and \"" << actualStr
             << "\" differ at element " << i << " by at least " << tolerance
             << " : "
             << " Expected \"" << expected[i] << "\", was \"" << actual[i]
             << "\".";
    }
  }

  return ::testing::AssertionSuccess();
}

template <int N, typename A, typename B, typename T>
::testing::AssertionResult CmpMatrixLikeFloatEq(
    const char* expectedStr, const char* actualStr, const char* toleranceStr,
    const A& expected, const B& actual, const T& tolerance) {
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (!IsEqual(expected(r, c), actual(r, c), tolerance)) {
        return ::testing::AssertionFailure()
               << "\"" << expectedStr << "\" and \"" << actualStr
               << "\" differ at (" << r << "," << c << ")"
               << " by at least " << tolerance << " : "
               << " Expected \"" << expected(r, c) << "\", was \""
               << actual(r, c) << "\".";
      }
    }
  }

  return ::testing::AssertionSuccess();
}

template <int N, typename A, typename B, typename T>
::testing::AssertionResult CmpArrayLikeFloatNe(
    const char* expectedStr, const char* actualStr, const char* toleranceStr,
    const A& expected, const B& actual, const T& tolerance) {
  for (int i = 0; i < N; ++i) {
    if (!IsEqual(expected[i], actual[i], tolerance)) {
      return ::testing::AssertionSuccess();
    }
  }

  ::testing::Message message;
  message << "Expected \"" << expectedStr
          << "\" to differ from provided value \"" << actualStr
          << "\" by at least " << tolerance << ".";

  return ::testing::AssertionFailure(message);
}

template <int N, typename A, typename B, typename T>
::testing::AssertionResult CmpMatrixLikeFloatNe(
    const char* expectedStr, const char* actualStr, const char* toleranceStr,
    const A& expected, const B& actual, const T& tolerance) {
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (!IsEqual(expected(r, c), actual(r, c), tolerance)) {
        return ::testing::AssertionSuccess();
      }
    }
  }

  ::testing::Message message;
  message << "Expected \"" << expectedStr
          << "\" to differ from provided value \"" << actualStr
          << "\" by at least " << tolerance << ".";

  return ::testing::AssertionFailure(message);
}

}  // namespace dvr
}  // namespace android

#define EXPECT_VEC3_NEAR(expected, actual, tol)                               \
  EXPECT_PRED_FORMAT3(android::dvr::CmpArrayLikeFloatEq<3>, expected, actual, \
                      tol)

#define EXPECT_VEC3_NOT_NEAR(expected, actual, tol)                           \
  EXPECT_PRED_FORMAT3(android::dvr::CmpArrayLikeFloatNe<3>, expected, actual, \
                      tol)

#define EXPECT_QUAT_NEAR(expected, actual, tol)                                \
  EXPECT_PRED_FORMAT3(android::dvr::CmpArrayLikeFloatEq<3>, expected.coeffs(), \
                      actual.coeffs(), tol)

#define EXPECT_QUAT_NOT_NEAR(expected, actual, tol)                            \
  EXPECT_PRED_FORMAT3(android::dvr::CmpArrayLikeFloatNe<3>, expected.coeffs(), \
                      actual.coeffs(), tol)

#define EXPECT_MAT4_NEAR(expected, actual, tol)                                \
  EXPECT_PRED_FORMAT3(android::dvr::CmpMatrixLikeFloatEq<4>, expected, actual, \
                      tol)

#define EXPECT_MAT4_NOT_NEAR(expected, actual, tol)                            \
  EXPECT_PRED_FORMAT3(android::dvr::CmpMatrixLikeFloatNe<4>, expected, actual, \
                      tol)

#define EXPECT_MAT3_NEAR(expected, actual, tol) \
  EXPECT_PRED_FORMAT3(android::dvr              \
                      : CmpMatrixLikeFloatEq<3>, expected, actual, tol)

#define EXPECT_MAT3_NOT_NEAR(expected, actual, tol)                            \
  EXPECT_PRED_FORMAT3(android::dvr::CmpMatrixLikeFloatNe<3>, expected, actual, \
                      tol)

#endif  // LIBS_VR_LIBDVRCOMMON_INCLUDE_PRIVATE_DVR_TEST_TEST_MACROS_H_
