/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_statistics_tests"
#include <audio_utils/Statistics.h>

#include <random>
#include <stdio.h>
#include <gtest/gtest.h>

// create uniform distribution
template <typename T, typename V>
static void initUniform(V& data, T rangeMin, T rangeMax) {
    const size_t count = data.capacity();
    std::minstd_rand gen(count);
    std::uniform_real_distribution<T> dis(rangeMin, rangeMax);

    // for_each works for scalars
    for (auto& datum : data) {
        android::audio_utils::for_each(datum, [&](T &value) { return value = dis(gen);});
    }
}

// create gaussian distribution
template <typename T, typename V>
static void initNormal(V& data, T mean, T stddev) {
    const size_t count = data.capacity();
    std::minstd_rand gen(count);

    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> dis{mean, stddev};

    // for_each works for scalars
    for (auto& datum : data) {
        android::audio_utils::for_each(datum, [&](T &value) { return value = dis(gen);});
    }
}

// Used to create compile-time reference constants for variance testing.
template <typename T>
class ConstexprStatistics {
public:
    template <size_t N>
    explicit constexpr ConstexprStatistics(const T (&a)[N])
        : mN{N}
        , mMax{android::audio_utils::max(a)}
        , mMin{android::audio_utils::min(a)}
        , mMean{android::audio_utils::sum(a) / mN}
        , mM2{android::audio_utils::sumSqDiff(a, mMean)}
        , mPopVariance{mM2 / mN}
        , mPopStdDev{android::audio_utils::sqrt_constexpr(mPopVariance)}
        , mVariance{mM2 / (mN - 1)}
        , mStdDev{android::audio_utils::sqrt_constexpr(mVariance)}
    { }

    constexpr int64_t getN() const { return mN; }
    constexpr T getMin() const { return mMin; }
    constexpr T getMax() const { return mMax; }
    constexpr double getWeight() const { return (double)mN; }
    constexpr double getMean() const { return mMean; }
    constexpr double getVariance() const { return mVariance; }
    constexpr double getStdDev() const { return mStdDev; }
    constexpr double getPopVariance() const { return mPopVariance; }
    constexpr double getPopStdDev() const { return mPopStdDev; }

private:
    const size_t mN;
    const T mMax;
    const T mMin;
    const double mMean;
    const double mM2;
    const double mPopVariance;
    const double mPopStdDev;
    const double mVariance;
    const double mStdDev;
};

class StatisticsTest : public testing::TestWithParam<const char *>
{
};

// find power of 2 that is small enough that it doesn't add to 1. due to finite mantissa.
template <typename T>
constexpr T smallp2() {
    T smallOne{};
    for (smallOne = T{1.}; smallOne + T{1.} > T{1.}; smallOne *= T(0.5));
    return smallOne;
}

// Our near expectation is 16x the bit that doesn't fit the mantissa.
// this works so long as we add values close in exponent with each other
// realizing that errors accumulate as the sqrt of N (random walk, lln, etc).
#define TEST_EXPECT_NEAR(e, v) \
    EXPECT_NEAR((e), (v), abs((e) * std::numeric_limits<decltype(e)>::epsilon() * 8))

#define PRINT_AND_EXPECT_EQ(expected, expr) { \
    auto value = (expr); \
    printf("(%s): %s\n", #expr, std::to_string(value).c_str()); \
    if ((expected) == (expected)) { EXPECT_EQ((expected), (value)); } \
    EXPECT_EQ((expected) != (expected), (value) != (value)); /* nan check */\
}

#define PRINT_AND_EXPECT_NEAR(expected, expr) { \
    auto ref = (expected); \
    auto value = (expr); \
    printf("(%s): %s\n", #expr, std::to_string(value).c_str()); \
    TEST_EXPECT_NEAR(ref, value); \
}

template <typename T, typename S>
static void verify(const T &stat, const S &refstat) {
    EXPECT_EQ(refstat.getN(), stat.getN());
    EXPECT_EQ(refstat.getMin(), stat.getMin());
    EXPECT_EQ(refstat.getMax(), stat.getMax());
    TEST_EXPECT_NEAR(refstat.getWeight(), stat.getWeight());
    TEST_EXPECT_NEAR(refstat.getMean(), stat.getMean());
    TEST_EXPECT_NEAR(refstat.getVariance(), stat.getVariance());
    TEST_EXPECT_NEAR(refstat.getStdDev(), stat.getStdDev());
    TEST_EXPECT_NEAR(refstat.getPopVariance(), stat.getPopVariance());
    TEST_EXPECT_NEAR(refstat.getPopStdDev(), stat.getPopStdDev());
}

// Test against fixed reference

TEST(StatisticsTest, high_precision_sums)
{
    static const double simple[] = { 1., 2., 3. };

    double rssum = android::audio_utils::sum<double, double>(simple);
    PRINT_AND_EXPECT_EQ(6., rssum);
    double kssum =
        android::audio_utils::sum<double, android::audio_utils::KahanSum<double>>(simple);
    PRINT_AND_EXPECT_EQ(6., kssum);
    double nmsum =
        android::audio_utils::sum<double, android::audio_utils::NeumaierSum<double>>(simple);
    PRINT_AND_EXPECT_EQ(6., nmsum);

    double rs{};
    android::audio_utils::KahanSum<double> ks{};
    android::audio_utils::NeumaierSum<double> ns{};

    // add 1.
    rs += 1.;
    ks += 1.;
    ns += 1.;

    static constexpr double smallOne = std::numeric_limits<double>::epsilon() * 0.5;
    // add lots of small values
    static const int loop = 1000;
    for (int i = 0; i < loop; ++i) {
        rs += smallOne;
        ks += smallOne;
        ns += smallOne;
    }

    // remove 1.
    rs += -1.;
    ks += -1.;
    ns += -1.;

    const double totalAdded = smallOne * loop;
    printf("totalAdded: %lg\n", totalAdded);
    PRINT_AND_EXPECT_EQ(0., rs);            // normal count fails
    PRINT_AND_EXPECT_EQ(totalAdded, ks);    // kahan succeeds
    PRINT_AND_EXPECT_EQ(totalAdded, ns);    // neumaier succeeds

    // test case where kahan fails and neumaier method succeeds.
    static const double tricky[] = { 1e100, 1., -1e100 };

    rssum = android::audio_utils::sum<double, double>(tricky);
    PRINT_AND_EXPECT_EQ(0., rssum);
    kssum = android::audio_utils::sum<double, android::audio_utils::KahanSum<double>>(tricky);
    PRINT_AND_EXPECT_EQ(0., kssum);
    nmsum = android::audio_utils::sum<double, android::audio_utils::NeumaierSum<double>>(tricky);
    PRINT_AND_EXPECT_EQ(1., nmsum);
}

TEST(StatisticsTest, minmax_bounds)
{
    // range based min and max use iterator forms of min and max.

    static constexpr double one[] = { 1. };

    PRINT_AND_EXPECT_EQ(std::numeric_limits<double>::infinity(),
            android::audio_utils::min(&one[0], &one[0]));

    PRINT_AND_EXPECT_EQ(-std::numeric_limits<double>::infinity(),
            android::audio_utils::max(&one[0], &one[0]));

    static constexpr int un[] = { 1 };

    PRINT_AND_EXPECT_EQ(std::numeric_limits<int>::max(),
            android::audio_utils::min(&un[0], &un[0]));

    PRINT_AND_EXPECT_EQ(std::numeric_limits<int>::min(),
            android::audio_utils::max(&un[0], &un[0]));

    double nanarray[] = { nan(""), nan(""), nan("") };

    PRINT_AND_EXPECT_EQ(std::numeric_limits<double>::infinity(),
            android::audio_utils::min(nanarray));

    PRINT_AND_EXPECT_EQ(-std::numeric_limits<double>::infinity(),
            android::audio_utils::max(nanarray));

    android::audio_utils::Statistics<double> s(nanarray);

    PRINT_AND_EXPECT_EQ(std::numeric_limits<double>::infinity(),
           s.getMin());

    PRINT_AND_EXPECT_EQ(-std::numeric_limits<double>::infinity(),
            s.getMax());
}

/*
TEST(StatisticsTest, sqrt_convergence)
{
    union {
        int i;
        float f;
    } u;

    for (int i = 0; i < INT_MAX; ++i) {
        u.i = i;
        const float f = u.f;
        if (!android::audio_utils::isnan(f)) {
            const float sf = android::audio_utils::sqrt(f);
            if ((i & (1 << 16) - 1) == 0) {
                printf("i: %d  f:%f  sf:%f\n", i, f, sf);
            }
        }
    }
}
*/

TEST(StatisticsTest, minmax_simple_array)
{
    static constexpr double ary[] = { -1.5, 1.5, -2.5, 2.5 };

    PRINT_AND_EXPECT_EQ(-2.5, android::audio_utils::min(ary));

    PRINT_AND_EXPECT_EQ(2.5, android::audio_utils::max(ary));

    static constexpr int ray[] = { -1, 1, -2, 2 };

    PRINT_AND_EXPECT_EQ(-2, android::audio_utils::min(ray));

    PRINT_AND_EXPECT_EQ(2, android::audio_utils::max(ray));
}

TEST(StatisticsTest, sqrt)
{
    // check doubles
    PRINT_AND_EXPECT_EQ(std::numeric_limits<double>::infinity(),
            android::audio_utils::sqrt(std::numeric_limits<double>::infinity()));

    PRINT_AND_EXPECT_EQ(std::nan(""),
            android::audio_utils::sqrt(-std::numeric_limits<double>::infinity()));

    PRINT_AND_EXPECT_NEAR(sqrt(std::numeric_limits<double>::epsilon()),
            android::audio_utils::sqrt(std::numeric_limits<double>::epsilon()));

    PRINT_AND_EXPECT_EQ(3.,
            android::audio_utils::sqrt(9.));

    PRINT_AND_EXPECT_EQ(0.,
            android::audio_utils::sqrt(0.));

    PRINT_AND_EXPECT_EQ(std::nan(""),
            android::audio_utils::sqrt(-1.));

    PRINT_AND_EXPECT_EQ(std::nan(""),
            android::audio_utils::sqrt(std::nan("")));

    // check floats
    PRINT_AND_EXPECT_EQ(std::numeric_limits<float>::infinity(),
            android::audio_utils::sqrt(std::numeric_limits<float>::infinity()));

    PRINT_AND_EXPECT_EQ(std::nanf(""),
            android::audio_utils::sqrt(-std::numeric_limits<float>::infinity()));

    PRINT_AND_EXPECT_NEAR(sqrtf(std::numeric_limits<float>::epsilon()),
            android::audio_utils::sqrt(std::numeric_limits<float>::epsilon()));

    PRINT_AND_EXPECT_EQ(2.f,
            android::audio_utils::sqrt(4.f));

    PRINT_AND_EXPECT_EQ(0.f,
            android::audio_utils::sqrt(0.f));

    PRINT_AND_EXPECT_EQ(std::nanf(""),
            android::audio_utils::sqrt(-1.f));

    PRINT_AND_EXPECT_EQ(std::nanf(""),
            android::audio_utils::sqrt(std::nanf("")));
}

TEST(StatisticsTest, stat_reference)
{
    // fixed reference compile time constants.
    static constexpr double data[] = {0.1, -0.1, 0.2, -0.3};
    static constexpr ConstexprStatistics<double> rstat(data); // use alpha = 1.
    static constexpr android::audio_utils::Statistics<double> stat{data};

    verify(stat, rstat);
}

TEST(StatisticsTest, stat_variable_alpha)
{
    constexpr size_t TEST_SIZE = 1 << 20;
    std::vector<double> data(TEST_SIZE);
    std::vector<double> alpha(TEST_SIZE);

    initUniform(data, -1., 1.);
    initUniform(alpha, .95, .99);

    android::audio_utils::ReferenceStatistics<double> rstat;
    android::audio_utils::Statistics<double> stat;

    static_assert(std::is_trivially_copyable<decltype(stat)>::value,
        "basic statistics must be trivially copyable");

    for (size_t i = 0; i < TEST_SIZE; ++i) {
        rstat.setAlpha(alpha[i]);
        rstat.add(data[i]);

        stat.setAlpha(alpha[i]);
        stat.add(data[i]);
    }

    printf("statistics: %s\n", stat.toString().c_str());
    printf("ref statistics: %s\n", rstat.toString().c_str());
    verify(stat, rstat);
}

TEST(StatisticsTest, stat_vector)
{
    // for operator overloading...
    using namespace android::audio_utils;

    using data_t = std::tuple<double, double>;
    using covariance_t = std::tuple<double, double, double, double>;
    using covariance_ut_t = std::tuple<double, double, double>;

    constexpr size_t TEST_SIZE = 1 << 20;
    std::vector<data_t> data(TEST_SIZE);
    // std::vector<double> alpha(TEST_SIZE);

    initUniform(data, -1., 1.);

    std::cout << "sample data[0]: " << data[0] << "\n";

    Statistics<data_t, data_t, data_t, double, double, innerProduct_scalar<data_t>> stat;
    Statistics<data_t, data_t, data_t, double,
            covariance_t, outerProduct_tuple<data_t>> stat_outer;
    Statistics<data_t, data_t, data_t, double,
            covariance_ut_t, outerProduct_UT_tuple<data_t>> stat_outer_ut;

    using pair_t = std::pair<double, double>;
    std::vector<pair_t> pairs(TEST_SIZE);
    initUniform(pairs, -1., 1.);
    Statistics<pair_t, pair_t, pair_t, double, double, innerProduct_scalar<pair_t>> stat_pair;

    using array_t = std::array<double, 2>;
    using array_covariance_ut_t = std::array<double, 3>;
    std::vector<array_t> arrays(TEST_SIZE);
    initUniform(arrays, -1., 1.);
    Statistics<array_t, array_t, array_t, double,
               double, innerProduct_scalar<array_t>> stat_array;
    Statistics<array_t, array_t, array_t, double,
               array_covariance_ut_t, outerProduct_UT_array<array_t>> stat_array_ut;

    for (size_t i = 0; i < TEST_SIZE; ++i) {
        stat.add(data[i]);
        stat_outer.add(data[i]);
        stat_outer_ut.add(data[i]);
        stat_pair.add(pairs[i]);
        stat_array.add(arrays[i]);
        stat_array_ut.add(arrays[i]);
    }

#if 0
    // these aren't trivially copyable
    static_assert(std::is_trivially_copyable<decltype(stat)>::value,
        "tuple based inner product not trivially copyable");
    static_assert(std::is_trivially_copyable<decltype(stat_outer)>::value,
        "tuple based outer product not trivially copyable");
    static_assert(std::is_trivially_copyable<decltype(stat_outer_ut)>::value,
        "tuple based outer product not trivially copyable");
#endif
    static_assert(std::is_trivially_copyable<decltype(stat_array)>::value,
        "array based inner product not trivially copyable");
    static_assert(std::is_trivially_copyable<decltype(stat_array_ut)>::value,
        "array based inner product not trivially copyable");

    // inner product variance should be same as outer product diagonal sum
    const double variance = stat.getPopVariance();
    EXPECT_NEAR(variance,
        std::get<0>(stat_outer.getPopVariance()) +
        std::get<3>(stat_outer.getPopVariance()),
        variance * std::numeric_limits<double>::epsilon() * 128);

    // outer product covariance should be identical
    PRINT_AND_EXPECT_NEAR(std::get<1>(stat_outer.getPopVariance()),
        std::get<2>(stat_outer.getPopVariance()));

    // upper triangular computation should be identical to outer product
    PRINT_AND_EXPECT_NEAR(std::get<0>(stat_outer.getPopVariance()),
        std::get<0>(stat_outer_ut.getPopVariance()));
    PRINT_AND_EXPECT_NEAR(std::get<1>(stat_outer.getPopVariance()),
        std::get<1>(stat_outer_ut.getPopVariance()));
    PRINT_AND_EXPECT_NEAR(std::get<3>(stat_outer.getPopVariance()),
        std::get<2>(stat_outer_ut.getPopVariance()));

    PRINT_AND_EXPECT_EQ(variance, stat_pair.getPopVariance());

    EXPECT_TRUE(equivalent(stat_array_ut.getPopVariance(), stat_outer_ut.getPopVariance()));

    printf("statistics_inner: %s\n", stat.toString().c_str());
    printf("statistics_outer: %s\n", stat_outer.toString().c_str());
    printf("statistics_outer_ut: %s\n", stat_outer_ut.toString().c_str());
}

TEST(StatisticsTest, stat_linearfit)
{
    using namespace android::audio_utils; // for operator overload
    LinearLeastSquaresFit<double> fit;

    static_assert(std::is_trivially_copyable<decltype(fit)>::value,
        "LinearLeastSquaresFit must be trivially copyable");

    using array_t = std::array<double, 2>;
    array_t data{0.0, 1.5};

    for (size_t i = 0; i < 10; ++i) {
        fit.add(data);
        data = data + array_t{0.1, 0.2};
    }

    // check the y line equation
    {
        double a, b, r2;
        fit.computeYLine(a, b, r2);
        printf("y line - a:%lf  b:%lf  r2:%lf\n", a, b, r2);
        PRINT_AND_EXPECT_NEAR(1.5, a); // y intercept
        PRINT_AND_EXPECT_NEAR(2.0, b); // y slope
        PRINT_AND_EXPECT_NEAR(1.0, r2); // correlation coefficient.

        // check same as static variant
        double ac, bc, r2c;
        computeYLineFromStatistics(ac, bc, r2c,
            std::get<0>(fit.getMean()), /* mean_x */
            std::get<1>(fit.getMean()), /* mean_y */
            std::get<0>(fit.getPopVariance()), /* var_x */
            std::get<1>(fit.getPopVariance()), /* cov_xy */
            std::get<2>(fit.getPopVariance())); /* var_y */

        EXPECT_EQ(a, ac);
        EXPECT_EQ(b, bc);
        EXPECT_EQ(r2, r2c);

        TEST_EXPECT_NEAR(1.9, fit.getYFromX(0.2));
        TEST_EXPECT_NEAR(0.2, fit.getXFromY(1.9));
        TEST_EXPECT_NEAR(1.0, fit.getR2());
    }

    // check the x line equation
    {
        double a, b, r2;
        fit.computeXLine(a, b, r2);
        printf("x line - a:%lf  b:%lf  r2:%lf\n", a, b, r2);
        PRINT_AND_EXPECT_NEAR(-0.75, a); // x intercept
        PRINT_AND_EXPECT_NEAR(0.5, b); // x slope
        PRINT_AND_EXPECT_NEAR(1.0, r2); // correlation coefficient.
    }
}

TEST(StatisticsTest, stat_linearfit_noise)
{
    using namespace android::audio_utils; // for operator overload
    using array_t = std::array<double, 2>;
    LinearLeastSquaresFit<double> fit;

    // We use 1000 steps for a linear line going from (0, 0) to (1, 1) as true data for
    // our linear fit.
    constexpr size_t ELEMENTS = 1000;
    array_t incr{1. / ELEMENTS, 1. / ELEMENTS};

    // To simulate additive noise, we use a Gaussian with stddev of 1, and then scale
    // achieve the desired stddev. We precompute our noise here (1000 of them).
    std::vector<array_t> noise(ELEMENTS);
    initNormal(noise, 0. /* mean */, 1. /* stddev */);

    for (int i = 0; i < 30; ++i) {
        // We run through 30 trials, with noise stddev ranging from 0 to 1.
        // The steps increment linearly from 0.001 to 0.01, linearly from 0.01 to 0.1, and
        // linearly again from 0.1 to 1.0.
        // 0.001, 0.002, ... 0.009, 0.01, 0.02, ....0.09, 0.1, 0.2, .... 1.0
        const double stddev = (i <= 10) ? i / 1000. : (i <= 20) ? (i - 9) / 100. : (i - 19) / 10.;
        fit.reset();

        for (size_t j = 0; j < ELEMENTS; ++j) {
            array_t data = j * incr + noise[j] * stddev;
            fit.add(data);
        }

        double a, b, r2;
        fit.computeYLine(a, b, r2);
        printf("stddev: %lf y line - N:%lld a:%lf  b:%lf  r2:%lf\n",
                stddev, (long long) fit.getN(), a, b, r2);
    }
}


TEST_P(StatisticsTest, stat_simple_char)
{
    const char *param = GetParam();

    android::audio_utils::Statistics<char> stat(0.9);
    android::audio_utils::ReferenceStatistics<char> rstat(0.9);

    // feed the string character by character to the statistics collectors.
    for (size_t i = 0; param[i] != '\0'; ++i) {
        stat.add(param[i]);
        rstat.add(param[i]);
    }

    printf("statistics for %s: %s\n", param, stat.toString().c_str());
    printf("ref statistics for %s: %s\n", param, rstat.toString().c_str());
    // verify that the statistics are the same
    verify(stat, rstat);
}

// find the variance of pet names as signed characters.
const char *pets[] = {"cat", "dog", "elephant", "mountain lion"};
INSTANTIATE_TEST_CASE_P(PetNameStatistics, StatisticsTest,
                        ::testing::ValuesIn(pets));

TEST(StatisticsTest, simple_stats)
{
    simple_stats_t ss{};

    for (const double value : { -1., 1., 3.}) {
        simple_stats_log(&ss, value);
    }

    PRINT_AND_EXPECT_EQ(3., ss.last);
    PRINT_AND_EXPECT_EQ(1., ss.mean);
    PRINT_AND_EXPECT_EQ(-1., ss.min);
    PRINT_AND_EXPECT_EQ(3., ss.max);
    PRINT_AND_EXPECT_EQ(3, ss.n);

    char buffer[256];
    simple_stats_to_string(&ss, buffer, sizeof(buffer));
    printf("simple_stats: %s", buffer);
}
