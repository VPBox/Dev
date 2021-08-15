/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <cstddef>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include <audio_utils/Statistics.h>

template <typename T>
static void initUniform(std::vector<T> &data, T rangeMin, T rangeMax) {
    const size_t count = data.capacity();
    std::minstd_rand gen(count);
    std::uniform_real_distribution<T> dis(rangeMin, rangeMax);
    for (auto &datum : data) {
        datum = dis(gen);
    }
}

template <typename Stats>
static void BM_MeanVariance(benchmark::State& state, int iterlimit, int alphalimit) {
    const float alpha = 1. - alphalimit * std::numeric_limits<float>::epsilon();
    Stats stat(alpha);
    using T = decltype(stat.getMin());
    constexpr size_t count = 1 << 20; // exactly one "mega" samples from the distribution.
    constexpr T range = 1.;
    std::vector<T> data(count);
    initUniform(data, -range, range);

    // Run the test
    int iters = 0;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(data.data());
        for (const auto &datum : data) {
            stat.add(datum);
        }
        benchmark::ClobberMemory();
        if (++iters % iterlimit == 0) {
            printf("%d>  alpha:%f  mean:%.17g  variance:%.17g\n",
                    iters, alpha, (double)stat.getMean(), (double)stat.getPopVariance());
            stat.reset();
        }
    }
    state.SetComplexityN(count);
}


// Test case:
// Do we work correctly within the capacity of float statistics when alpha == 1?
//
// 1 << 23 samples is the mantissa limited capacity of float statistics if alpha == 1.
static constexpr int float_iterlimit = 8;
// alphalimit of 0 means alpha exactly equals one.
static constexpr int alpha_equals_one_alphalimit = 0;

// benchmark running float
static void BM_MeanVariance_float_float_float(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::Statistics<float, float, float>>(state,
        float_iterlimit, alpha_equals_one_alphalimit);
}

BENCHMARK(BM_MeanVariance_float_float_float);

// benchmark reference float
static void BM_RefMeanVariance_float_float(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::ReferenceStatistics<float, float>>(state,
        float_iterlimit, alpha_equals_one_alphalimit);
}

BENCHMARK(BM_RefMeanVariance_float_float);

// benchmark running double
static auto BM_MeanVariance_float_double_double(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::Statistics<float, double, double>>(state,
        float_iterlimit, alpha_equals_one_alphalimit);
}

BENCHMARK(BM_MeanVariance_float_double_double);

// benchmark reference double
static auto BM_RefMeanVariance_float_double(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::ReferenceStatistics<float, double>>(state,
        float_iterlimit, alpha_equals_one_alphalimit);
}

BENCHMARK(BM_RefMeanVariance_float_double);

// benchmark running float + kahan
static auto BM_MeanVariance_float_float_Kahan(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::Statistics<float, float,
        android::audio_utils::KahanSum<float>>>(state,
            float_iterlimit, alpha_equals_one_alphalimit);
}

BENCHMARK(BM_MeanVariance_float_float_Kahan);

// benchmark running float + Neumaier
static auto BM_MeanVariance_float_float_Neumaier(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::Statistics<float, float,
        android::audio_utils::NeumaierSum<float>>>(state,
            float_iterlimit, alpha_equals_one_alphalimit);
}

BENCHMARK(BM_MeanVariance_float_float_Neumaier);

// Test case:
// Do we work correctly for very large N statistics when alpha is 1 - 32 * epsilon?
// This simulates long term statistics collection, where the alpha weighted windowing
// permits us to exceed 1 << 23 samples reliably.
//
// 1 << 25 samples exceeds the mantissa limited capacity of float statistics if alpha == 1...
static constexpr int float_overflow_iterlimit = 32;
// but we use an alphalimit of 32, means 1. - (alphalimit * epsilon) approx = 0.999996.
// This should allow statistics collection indefinitely.
static constexpr int alpha_safe_upperbound_iterlimit = 32;

// benchmark running float at alpha
static auto BM_MeanVariance_float_float_float_alpha(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::Statistics<float, float, float>>(state,
        float_overflow_iterlimit, alpha_safe_upperbound_iterlimit);
}

BENCHMARK(BM_MeanVariance_float_float_float_alpha);

// benchmark running double
static auto BM_MeanVariance_float_double_double_alpha(benchmark::State &state) {
    BM_MeanVariance<android::audio_utils::Statistics<float, double, double>>(state,
        float_overflow_iterlimit, alpha_safe_upperbound_iterlimit);
}

BENCHMARK(BM_MeanVariance_float_double_double_alpha);

BENCHMARK_MAIN();
