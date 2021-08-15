#include <gtest/gtest.h>

#include <private/dvr/numeric.h>

using TestTypes = ::testing::Types<float, double, int>;

using android::dvr::RandomInRange;

template <typename T>
class NumericTest : public ::testing::TestWithParam<T> {
 public:
  using FT = T;
};

TYPED_TEST_CASE(NumericTest, TestTypes);

TYPED_TEST(NumericTest, RandomInRange) {
  using FT = typename TestFixture::FT;

  const int kNumTrials = 50;
  const FT kLowRange = static_cast<FT>(-100);
  const FT kHighRange = static_cast<FT>(100);

  for (int i = 0; i < kNumTrials; ++i) {
    FT value = RandomInRange(kLowRange, kHighRange);

    EXPECT_LE(kLowRange, value);
    EXPECT_GE(kHighRange, value);
  }
}

TEST(RandomInRange, TestIntVersion) {
  // This checks specifically that the function does not always give the lo
  // value (this was previously a bug)

  const int kNumTrials = 50;
  const int kLowRange = -100;
  const int kHighRange = 100;

  for (int i = 0; i < kNumTrials; ++i) {
    int value = RandomInRange(kLowRange, kHighRange);

    if (value != kLowRange) {
      SUCCEED();
      return;
    }
  }

  FAIL() << "Did not produce a value other than the range minimum for "
         << "integers.";
}

TEST(RandomInRange, TestVectorVersion) {
  Eigen::Vector3d lo(-3.0, -4.0, -5.0);
  Eigen::Vector3d hi(5.0, 4.0, 3.0);

  const int kNumTrials = 50;

  for (int i = 0; i < kNumTrials; ++i) {
    Eigen::Vector3d result = RandomInRange(lo, hi);

    for (int j = 0; j < 3; ++j) {
      EXPECT_LE(lo[j], result[j]);
      EXPECT_GE(hi[j], result[j]);
    }
  }
}
