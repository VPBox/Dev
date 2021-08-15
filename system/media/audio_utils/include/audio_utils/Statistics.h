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

#ifndef ANDROID_AUDIO_UTILS_STATISTICS_H
#define ANDROID_AUDIO_UTILS_STATISTICS_H

#ifdef __cplusplus

#include "variadic_utils.h"

// variadic_utils already contains stl headers; in addition:
#include <deque> // for ReferenceStatistics implementation
#include <sstream>

namespace android {
namespace audio_utils {

/**
 * Compensated summation is used to accumulate a sequence of floating point
 * values, with "compensation" information to help preserve precision lost
 * due to catastrophic cancellation, e.g. (BIG) + (SMALL) - (BIG) = 0.
 *
 * We provide two forms of compensated summation:
 * the Kahan variant (which has better properties if the sum is generally
 * larger than the data added; and the Neumaier variant which is better if
 * the sum or delta may alternatively be larger.
 *
 * https://en.wikipedia.org/wiki/Kahan_summation_algorithm
 *
 * Alternative approaches include divide-and-conquer summation
 * which provides increased accuracy with log n stack depth (recursion).
 *
 * https://en.wikipedia.org/wiki/Pairwise_summation
 */

template <typename T>
struct KahanSum {
    T mSum{};
    T mCorrection{}; // negative low order bits of mSum.

    constexpr KahanSum<T>() = default;

    explicit constexpr KahanSum<T>(const T& value)
        : mSum{value}
    { }

    // takes T not KahanSum<T>
    friend constexpr KahanSum<T> operator+(KahanSum<T> lhs, const T& rhs) {
        const T y = rhs - lhs.mCorrection;
        const T t = lhs.mSum + y;

#ifdef __FAST_MATH__
#warning "fast math enabled, could optimize out KahanSum correction"
#endif

        lhs.mCorrection = (t - lhs.mSum) - y; // compiler, please do not optimize with /fp:fast
        lhs.mSum = t;
        return lhs;
    }

    constexpr KahanSum<T>& operator+=(const T& rhs) { // takes T not KahanSum<T>
        *this = *this + rhs;
        return *this;
    }

    constexpr operator T() const {
        return mSum;
    }

    constexpr void reset() {
        mSum = {};
        mCorrection = {};
    }
};

// A more robust version of Kahan summation for input greater than sum.
// TODO: investigate variants that reincorporate mCorrection bits into mSum if possible.
template <typename T>
struct NeumaierSum {
    T mSum{};
    T mCorrection{}; // low order bits of mSum.

    constexpr NeumaierSum<T>() = default;

    explicit constexpr NeumaierSum<T>(const T& value)
        : mSum{value}
    { }

    friend constexpr NeumaierSum<T> operator+(NeumaierSum<T> lhs, const T& rhs) {
        const T t = lhs.mSum + rhs;

        if (const_abs(lhs.mSum) >= const_abs(rhs)) {
            lhs.mCorrection += (lhs.mSum - t) + rhs;
        } else {
            lhs.mCorrection += (rhs - t) + lhs.mSum;
        }
        lhs.mSum = t;
        return lhs;
    }

    constexpr NeumaierSum<T>& operator+=(const T& rhs) { // takes T not NeumaierSum<T>
        *this = *this + rhs;
        return *this;
    }

    static constexpr T const_abs(T x) {
        return x < T{} ? -x : x;
    }

    constexpr operator T() const {
        return mSum + mCorrection;
    }

    constexpr void reset() {
        mSum = {};
        mCorrection = {};
    }
};

//-------------------------------------------------------------------
// Constants and limits

template <typename T, typename T2=void>  struct StatisticsConstants;

template <typename T>
struct StatisticsConstants<T, std::enable_if_t<std::is_arithmetic<T>::value>> {
    // value closest to negative infinity for type T
    static constexpr T negativeInfinity() {
        return std::numeric_limits<T>::has_infinity ?
                -std::numeric_limits<T>::infinity() : std::numeric_limits<T>::min();
    };

    static constexpr T mNegativeInfinity = negativeInfinity();

    // value closest to positive infinity for type T
    static constexpr T positiveInfinity() {
        return std::numeric_limits<T>::has_infinity ?
                std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();
    }

    static constexpr T mPositiveInfinity = positiveInfinity();
};

// specialize for tuple and pair
template <typename T>
struct StatisticsConstants<T, std::enable_if_t<!std::is_arithmetic<T>::value>> {
private:
    template <std::size_t... I >
    static constexpr auto negativeInfinity(std::index_sequence<I...>) {
       return T{StatisticsConstants<
               typename std::tuple_element<I, T>::type>::mNegativeInfinity...};
    }
    template <std::size_t... I >
    static constexpr auto positiveInfinity(std::index_sequence<I...>) {
       return T{StatisticsConstants<
               typename std::tuple_element<I, T>::type>::mPositiveInfinity...};
    }
public:
    static constexpr auto negativeInfinity() {
       return negativeInfinity(std::make_index_sequence<std::tuple_size<T>::value>());
    }
    static constexpr auto mNegativeInfinity =
        negativeInfinity(std::make_index_sequence<std::tuple_size<T>::value>());
    static constexpr auto positiveInfinity() {
       return positiveInfinity(std::make_index_sequence<std::tuple_size<T>::value>());
    }
    static constexpr auto mPositiveInfinity =
        positiveInfinity(std::make_index_sequence<std::tuple_size<T>::value>());
};

/**
 * Statistics provides a running weighted average, variance, and standard deviation of
 * a sample stream. It is more numerically stable for floating point computation than a
 * naive sum of values, sum of values squared.
 *
 * The weighting is like an IIR filter, with the most recent sample weighted as 1, and decaying
 * by alpha (between 0 and 1).  With alpha == 1. this is rectangular weighting, reducing to
 * Welford's algorithm.
 *
 * The IIR filter weighting emphasizes more recent samples, has low overhead updates,
 * constant storage, and constant computation (per update or variance read).
 *
 * This is a variant of the weighted mean and variance algorithms described here:
 * https://en.wikipedia.org/wiki/Moving_average
 * https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm
 * https://en.wikipedia.org/wiki/Weighted_arithmetic_mean
 *
 * weight = sum_{i=1}^n \alpha^{n-i}
 * mean = 1/weight * sum_{i=1}^n \alpha^{n-i}x_i
 * var = 1/weight * sum_{i=1}^n alpha^{n-i}(x_i- mean)^2
 *
 * The Statistics class is safe to call from a SCHED_FIFO thread with the exception of
 * the toString() method, which uses std::stringstream to format data for printing.
 *
 * Long term data accumulation and constant alpha:
 * If the alpha weight is 1 (or not specified) then statistics objects with float
 * summation types (D, S) should NOT add more than the mantissa-bits elements
 * without reset to prevent variance increases due to weight precision underflow.
 * This is 1 << 23 elements for float and 1 << 52 elements for double.
 *
 * Setting alpha less than 1 avoids this underflow problem.
 * Alpha < 1 - (epsilon * 32), where epsilon is std::numeric_limits<D>::epsilon()
 * is recommended for continuously running statistics (alpha <= 0.999996
 * for float summation precision).
 *
 * Alpha may also change on-the-fly, based on the reliability of
 * new information.  In that case, alpha may be set temporarily greater
 * than 1.
 *
 * https://en.wikipedia.org/wiki/Weighted_arithmetic_mean#Reliability_weights_2
 *
 * Statistics may also be collected on variadic "vector" object instead of
 * scalars, where the variance may be computed as an inner product radial squared
 * distance from the mean; or as an outer product where the variance returned
 * is a covariance matrix.
 *
 * TODO:
 * 1) Alternative versions of Kahan/Neumaier sum that better preserve precision.
 * 2) Add binary math ops to corrected sum classes for better precision in lieu of long double.
 * 3) Add Cholesky decomposition to ensure positive definite covariance matrices if
 *    the input is a variadic object.
 */

/**
 * Mean may have catastrophic cancellation of positive and negative sample values,
 * so we use Kahan summation in the algorithms below (or substitute "D" if not needed).
 *
 * https://en.wikipedia.org/wiki/Kahan_summation_algorithm
 */

template <
    typename T,               // input data type
    typename D = double,      // output mean data type
    typename S = KahanSum<D>, // compensated mean summation type, if any
    typename A = double,      // weight type
    typename D2 = double,     // output variance "D^2" type
    typename PRODUCT = std::multiplies<D> // how the output variance is computed
    >
class Statistics {
public:
    /** alpha is the weight (if alpha == 1. we use a rectangular window) */
    explicit constexpr Statistics(A alpha = A(1.))
        : mAlpha(alpha)
    { }

    template <size_t N>
    explicit constexpr Statistics(const T (&a)[N], A alpha = A(1.))
        : mAlpha(alpha)
    {
        for (const auto &data : a) {
            add(data);
        }
    }

    constexpr void setAlpha(A alpha) {
        mAlpha = alpha;
    }

    constexpr void add(const T &value) {
        // Note: fastest implementation uses fmin fminf but would not be constexpr

        mMax = audio_utils::max(mMax, value); // order important: reject NaN
        mMin = audio_utils::min(mMin, value); // order important: reject NaN
        ++mN;
        const D delta = value - mMean;
        /* if (mAlpha == 1.) we have Welford's algorithm
            ++mN;
            mMean += delta / mN;
            mM2 += delta * (value - mMean);

            Note delta * (value - mMean) should be non-negative.
        */
        mWeight = A(1.) + mAlpha * mWeight;
        mWeight2 = A(1.) + mAlpha * mAlpha * mWeight2;
        D meanDelta = delta / mWeight;
        mMean += meanDelta;
        mM2 = mAlpha * mM2 + PRODUCT()(delta, (value - mMean));

        /*
           Alternate variant related to:
           http://mathworld.wolfram.com/SampleVarianceComputation.html

           const double sweight = mAlpha * mWeight;
           mWeight = 1. + sweight;
           const double dmean = delta / mWeight;
           mMean += dmean;
           mM2 = mAlpha * mM2 + mWeight * sweight * dmean * dmean;

           The update is slightly different than Welford's algorithm
           showing a by-construction non-negative update to M2.
        */
    }

    constexpr int64_t getN() const {
        return mN;
    }

    constexpr void reset() {
        mMin = StatisticsConstants<T>::positiveInfinity();
        mMax = StatisticsConstants<T>::negativeInfinity();
        mN = 0;
        mWeight = {};
        mWeight2 = {};
        mMean = {};
        mM2 = {};
    }

    constexpr A getWeight() const {
        return mWeight;
    }

    constexpr D getMean() const {
        return mMean;
    }

    constexpr D2 getVariance() const {
        if (mN < 2) {
            // must have 2 samples for sample variance.
            return {};
        } else {
            return mM2 / getSampleWeight();
        }
    }

    constexpr D2 getPopVariance() const {
        if (mN < 1) {
            return {};
        } else {
            return mM2 / mWeight;
        }
    }

    // explicitly use sqrt_constexpr if you need a constexpr version
    D2 getStdDev() const {
        return android::audio_utils::sqrt(getVariance());
    }

    D2 getPopStdDev() const {
        return android::audio_utils::sqrt(getPopVariance());
    }

    constexpr T getMin() const {
        return mMin;
    }

    constexpr T getMax() const {
        return mMax;
    }

    std::string toString() const {
        const int64_t N = getN();
        if (N == 0) return "unavail";

        std::stringstream ss;
        ss << "ave=" << getMean();
        if (N > 1) {
            // we use the sample standard deviation (not entirely unbiased,
            // though the sample variance is unbiased).
            ss << " std=" << getStdDev();
        }
        ss << " min=" << getMin();
        ss << " max=" << getMax();
        return ss.str();
    }

private:
    A mAlpha;
    T mMin{StatisticsConstants<T>::positiveInfinity()};
    T mMax{StatisticsConstants<T>::negativeInfinity()};

    int64_t mN = 0;  // running count of samples.
    A mWeight{};     // sum of weights.
    A mWeight2{};    // sum of weights squared.
    S mMean{};       // running mean.
    D2 mM2{};         // running unnormalized variance.

    // Reliability correction for unbiasing variance, since mean is estimated
    // from same sample stream as variance.
    // if mAlpha == 1 this is mWeight - 1;
    //
    // TODO: consider exposing the correction factor.
    constexpr A getSampleWeight() const {
        // if mAlpha is constant then the mWeight2 member variable is not
        // needed, one can use instead:
        // return (mWeight - D(1.)) * D(2.) / (D(1.) + mAlpha);

        return mWeight - mWeight2 / mWeight;
    }
};

/**
 * ReferenceStatistics is a naive implementation of the weighted running variance,
 * which consumes more space and is slower than Statistics.  It is provided for
 * comparison and testing purposes.  Do not call from a SCHED_FIFO thread!
 *
 * Note: Common code not combined for implementation clarity.
 *       We don't invoke Kahan summation or other tricks.
 */
template <
    typename T, // input data type
    typename D = double // output mean/variance data type
    >
class ReferenceStatistics {
public:
    /** alpha is the weight (alpha == 1. is rectangular window) */
    explicit ReferenceStatistics(D alpha = D(1.))
        : mAlpha(alpha)
    { }

    constexpr void setAlpha(D alpha) {
        mAlpha = alpha;
    }

    // For independent testing, have intentionally slightly different behavior
    // of min and max than Statistics with respect to Nan.
    constexpr void add(const T &value) {
        if (getN() == 0) {
            mMax = value;
            mMin = value;
        } else if (value > mMax) {
            mMax = value;
        } else if (value < mMin) {
            mMin = value;
        }

        mData.push_front(value);
        mAlphaList.push_front(mAlpha);
    }

    int64_t getN() const {
        return mData.size();
    }

    void reset() {
        mMin = {};
        mMax = {};
        mData.clear();
        mAlphaList.clear();
    }

    D getWeight() const {
        D weight{};
        D alpha_i(1.);
        for (size_t i = 0; i < mData.size(); ++i) {
            weight += alpha_i;
            alpha_i *= mAlphaList[i];
        }
        return weight;
    }

    D getWeight2() const {
        D weight2{};
        D alpha2_i(1.);
        for (size_t i = 0; i < mData.size(); ++i) {
            weight2 += alpha2_i;
            alpha2_i *= mAlphaList[i] * mAlphaList[i];
        }
        return weight2;
    }

    D getMean() const {
        D wsum{};
        D alpha_i(1.);
        for (size_t i = 0; i < mData.size(); ++i) {
            wsum += alpha_i * mData[i];
            alpha_i *= mAlphaList[i];
        }
        return wsum / getWeight();
    }

    // Should always return a positive value.
    D getVariance() const {
        return getUnweightedVariance() / (getWeight() - getWeight2() / getWeight());
    }

    // Should always return a positive value.
    D getPopVariance() const {
        return getUnweightedVariance() / getWeight();
    }

    D getStdDev() const {
        return sqrt(getVariance());
    }

    D getPopStdDev() const {
        return sqrt(getPopVariance());
    }

    T getMin() const {
        return mMin;
    }

    T getMax() const {
        return mMax;
    }

    std::string toString() const {
        const auto N = getN();
        if (N == 0) return "unavail";

        std::stringstream ss;
        ss << "ave=" << getMean();
        if (N > 1) {
            // we use the sample standard deviation (not entirely unbiased,
            // though the sample variance is unbiased).
            ss << " std=" << getStdDev();
        }
        ss << " min=" << getMin();
        ss << " max=" << getMax();
        return ss.str();
    }

private:
    T mMin{};
    T mMax{};

    D mAlpha;                 // current alpha value
    std::deque<T> mData;      // store all the data for exact summation, mData[0] most recent.
    std::deque<D> mAlphaList; // alpha value for the data added.

    D getUnweightedVariance() const {
        const D mean = getMean();
        D wsum{};
        D alpha_i(1.);
        for (size_t i = 0; i < mData.size(); ++i) {
            const D diff = mData[i] - mean;
            wsum += alpha_i * diff * diff;
            alpha_i *= mAlphaList[i];
        }
        return wsum;
    }
};

/**
 * Least squares fitting of a 2D straight line based on the covariance matrix.
 *
 * See formula from:
 * http://mathworld.wolfram.com/LeastSquaresFitting.html
 *
 * y = a + b*x
 *
 * returns a: y intercept
 *         b: slope
 *         r2: correlation coefficient (1.0 means great fit, 0.0 means no fit.)
 *
 * For better numerical stability, it is suggested to use the slope b only:
 * as the least squares fit line intersects the mean.
 *
 * (y - mean_y) = b * (x - mean_x).
 *
 */
template <typename T>
constexpr void computeYLineFromStatistics(
        T &a, T& b, T &r2,
        const T& mean_x,
        const T& mean_y,
        const T& var_x,
        const T& cov_xy,
        const T& var_y) {

    // Dimensionally r2 is unitless.  If there is no correlation
    // then r2 is clearly 0 as cov_xy == 0.  If x and y are identical up to a scale
    // and shift, then r2 is 1.
    r2 = cov_xy * cov_xy / (var_x * var_y);

    // The least squares solution to the overconstrained matrix equation requires
    // the pseudo-inverse. In 2D, the best-fit slope is the mean removed
    // (via covariance and variance) dy/dx derived from the joint expectation
    // (this is also dimensionally correct).
    b = cov_xy / var_x;

    // The best fit line goes through the mean, and can be used to find the y intercept.
    a = mean_y - b * mean_x;
}

/**
 * LinearLeastSquaresFit<> class is derived from the Statistics<> class, with a 2 element array.
 * Arrays are preferred over tuples or pairs because copy assignment is constexpr and
 * arrays are trivially copyable.
 */
template <typename T>
class LinearLeastSquaresFit : public
    Statistics<std::array<T, 2>, // input
               std::array<T, 2>, // mean data output
               std::array<T, 2>, // compensated mean sum
               T,                // weight type
               std::array<T, 3>, // covariance_ut
               audio_utils::outerProduct_UT_array<std::array<T, 2>>>
{
public:
    constexpr explicit LinearLeastSquaresFit(const T &alpha = T(1.))
        : Statistics<std::array<T, 2>,
             std::array<T, 2>,
             std::array<T, 2>,
             T,
             std::array<T, 3>, // covariance_ut
             audio_utils::outerProduct_UT_array<std::array<T, 2>>>(alpha) { }

    /* Note: base class method: add(value)

    constexpr void add(const std::array<T, 2>& value);

       use:
          add({1., 2.});
       or
          add(to_array(myTuple));
    */

    /**
     * y = a + b*x
     *
     * returns a: y intercept
     *         b: y slope (dy / dx)
     *         r2: correlation coefficient (1.0 means great fit, 0.0 means no fit.)
     */
    constexpr void computeYLine(T &a, T &b, T &r2) const {
        computeYLineFromStatistics(a, b, r2,
                std::get<0>(this->getMean()), /* mean_x */
                std::get<1>(this->getMean()), /* mean_y */
                std::get<0>(this->getPopVariance()), /* var_x */
                std::get<1>(this->getPopVariance()), /* cov_xy */
                std::get<2>(this->getPopVariance())); /* var_y */
    }

    /**
     * x = a + b*y
     *
     * returns a: x intercept
     *         b: x slope (dx / dy)
     *         r2: correlation coefficient (1.0 means great fit, 0.0 means no fit.)
     */
    constexpr void computeXLine(T &a, T &b, T &r2) const {
        // reverse x and y for X line computation
        computeYLineFromStatistics(a, b, r2,
                std::get<1>(this->getMean()), /* mean_x */
                std::get<0>(this->getMean()), /* mean_y */
                std::get<2>(this->getPopVariance()), /* var_x */
                std::get<1>(this->getPopVariance()), /* cov_xy */
                std::get<0>(this->getPopVariance())); /* var_y */
    }

    /**
     * this returns the estimate of y from a given x
     */
    constexpr T getYFromX(const T &x) const {
        const T var_x = std::get<0>(this->getPopVariance());
        const T cov_xy = std::get<1>(this->getPopVariance());
        const T b = cov_xy / var_x;  // dy / dx

        const T mean_x = std::get<0>(this->getMean());
        const T mean_y = std::get<1>(this->getMean());
        return /* y = */ b * (x - mean_x) + mean_y;
    }

    /**
     * this returns the estimate of x from a given y
     */
    constexpr T getXFromY(const T &y) const {
        const T cov_xy = std::get<1>(this->getPopVariance());
        const T var_y = std::get<2>(this->getPopVariance());
        const T b = cov_xy / var_y;  // dx / dy

        const T mean_x = std::get<0>(this->getMean());
        const T mean_y = std::get<1>(this->getMean());
        return /* x = */ b * (y - mean_y) + mean_x;
    }

    constexpr T getR2() const {
        const T var_x = std::get<0>(this->getPopVariance());
        const T cov_xy = std::get<1>(this->getPopVariance());
        const T var_y = std::get<2>(this->getPopVariance());
        return cov_xy * cov_xy / (var_x * var_y);
    }
};

/**
 * constexpr statistics functions of form:
 * algorithm(forward_iterator begin, forward_iterator end)
 *
 * These check that the input looks like an iterator, but doesn't
 * check if __is_forward_iterator<>.
 *
 * divide-and-conquer pairwise summation forms will require
 * __is_random_access_iterator<>.
 */

// returns max of elements, or if no elements negative infinity.
template <typename T,
          std::enable_if_t<is_iterator<T>::value, int> = 0>
constexpr auto max(T begin, T end) {
    using S = std::remove_cv_t<std::remove_reference_t<
            decltype(*begin)>>;
    S maxValue = StatisticsConstants<S>::mNegativeInfinity;
    for (auto it = begin; it != end; ++it) {
        maxValue = std::max(maxValue, *it);
    }
    return maxValue;
}

// returns min of elements, or if no elements positive infinity.
template <typename T,
          std::enable_if_t<is_iterator<T>::value, int> = 0>
constexpr auto min(T begin, T end) {
    using S = std::remove_cv_t<std::remove_reference_t<
            decltype(*begin)>>;
    S minValue = StatisticsConstants<S>::mPositiveInfinity;
    for (auto it = begin; it != end; ++it) {
        minValue = std::min(minValue, *it);
    }
    return minValue;
}

template <typename D = double, typename S = KahanSum<D>, typename T,
          std::enable_if_t<is_iterator<T>::value, int> = 0>
constexpr auto sum(T begin, T end) {
    S sum{};
    for (auto it = begin; it != end; ++it) {
        sum += D(*it);
    }
    return sum;
}

template <typename D = double, typename S = KahanSum<D>, typename T,
          std::enable_if_t<is_iterator<T>::value, int> = 0>
constexpr auto sumSqDiff(T begin, T end, D x = {}) {
    S sum{};
    for (auto it = begin; it != end; ++it) {
        const D diff = *it - x;
        sum += diff * diff;
    }
    return sum;
}

// Form: algorithm(array[]), where array size is known to the compiler.
template <typename T, size_t N>
constexpr T max(const T (&a)[N]) {
    return max(&a[0], &a[N]);
}

template <typename T, size_t N>
constexpr T min(const T (&a)[N]) {
    return min(&a[0], &a[N]);
}

template <typename D = double, typename S = KahanSum<D>, typename T, size_t N>
constexpr D sum(const T (&a)[N]) {
    return sum<D, S>(&a[0], &a[N]);
}

template <typename D = double, typename S = KahanSum<D>, typename T, size_t N>
constexpr D sumSqDiff(const T (&a)[N], D x = {}) {
    return sumSqDiff<D, S>(&a[0], &a[N], x);
}

// TODO: remove when std::isnan is constexpr
template <typename T>
constexpr T isnan(T x) {
    return __builtin_isnan(x);
}

// constexpr sqrt computed by the Babylonian (Newton's) method.
// Please use math libraries for non-constexpr cases.
// TODO: remove when there is some std::sqrt which is constexpr.
//
// https://en.wikipedia.org/wiki/Methods_of_computing_square_roots

// watch out using the unchecked version, use the checked version below.
template <typename T>
constexpr T sqrt_constexpr_unchecked(T x, T prev) {
    static_assert(std::is_floating_point<T>::value, "must be floating point type");
    const T next = T(0.5) * (prev + x / prev);
    return next == prev ? next : sqrt_constexpr_unchecked(x, next);
}

// checked sqrt
template <typename T>
constexpr T sqrt_constexpr(T x) {
    static_assert(std::is_floating_point<T>::value, "must be floating point type");
    if (x < T{}) { // negative values return nan
        return std::numeric_limits<T>::quiet_NaN();
    } else if (isnan(x)
            || x == std::numeric_limits<T>::infinity()
            || x == T{}) {
        return x;
    } else { // good to go.
        return sqrt_constexpr_unchecked(x, T(1.));
    }
}

} // namespace audio_utils
} // namespace android

#endif // __cplusplus

/** \cond */
 __BEGIN_DECLS
/** \endcond */

/** Simple stats structure for low overhead statistics gathering.
 * Designed to be accessed by C (with no functional getters).
 * Zero initialize {} to clear or reset.
 */
typedef struct {
   int64_t n;
   double min;
   double max;
   double last;
   double mean;
} simple_stats_t;

/** logs new value to the simple_stats_t */
static inline void simple_stats_log(simple_stats_t *stats, double value) {
    if (++stats->n == 1) {
        stats->min = stats->max = stats->last = stats->mean = value;
    } else {
        stats->last = value;
        if (value < stats->min) {
            stats->min = value;
        } else if (value > stats->max) {
            stats->max = value;
        }
        // Welford's algorithm for mean
        const double delta = value - stats->mean;
        stats->mean += delta / stats->n;
    }
}

/** dumps statistics to a string, returns the length of string excluding null termination. */
static inline size_t simple_stats_to_string(simple_stats_t *stats, char *buffer, size_t size) {
    if (size == 0) {
        return 0;
    } else if (stats->n == 0) {
        return snprintf(buffer, size, "none");
    } else {
        return snprintf(buffer, size, "(mean: %lf  min: %lf  max: %lf  last: %lf  n: %lld)",
                stats->mean, stats->min, stats->max, stats->last, (long long)stats->n);
    }
}

/** \cond */
__END_DECLS
/** \endcond */

#endif // !ANDROID_AUDIO_UTILS_STATISTICS_H
