/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <audio_utils/primitives.h>

static void BM_MemcpyToFloatFromFloatWithClamping(benchmark::State& state) {
    const size_t count = state.range(0);
    const float srcMax = state.range(1);
    const float absMax = 1.413;

    std::vector<float> src(count);
    std::vector<float> dst(count);
    std::vector<float> expected(count);

    // Initialize src buffer with deterministic pseudo-random values
    std::minstd_rand gen(count);
    std::uniform_real_distribution<> dis(-srcMax, srcMax);
    for (size_t i = 0; i < count; i++) {
        src[i] = dis(gen);
        expected[i] = fmin(absMax, fmax(-absMax, src[i]));
    }

    // Run the test
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(src.data());
        benchmark::DoNotOptimize(dst.data());
        memcpy_to_float_from_float_with_clamping(dst.data(), src.data(), count, 1.413);
        benchmark::ClobberMemory();
    }

    if (expected != dst) {
        state.SkipWithError("Incorrect clamping!");
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_MemcpyToFloatFromFloatWithClamping)->RangeMultiplier(2)->Ranges({{10, 8<<12}, {1, 2}});

static void BM_MemcpyFloat(benchmark::State& state) {
    const size_t count = state.range(0);

    std::vector<float> src(count);
    std::vector<float> dst(count);

    // Initialize src buffer with deterministic pseudo-random values
    std::minstd_rand gen(count);
    std::uniform_real_distribution<> dis;
    for (size_t i = 0; i < count; i++) {
        src[i] = dis(gen);
    }

    // Run the test
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(src.data());
        benchmark::DoNotOptimize(dst.data());
        memcpy(dst.data(), src.data(), count * sizeof(float));
        benchmark::ClobberMemory();
    }

    if (src != dst) {
        state.SkipWithError("Incorrect memcpy!");
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_MemcpyFloat)->RangeMultiplier(2)->Ranges({{10, 8<<12}});

static void BM_MemcpyToFloatFromI16(benchmark::State& state) {
    const size_t count = state.range(0);

    std::vector<int16_t> src(count);
    std::vector<float> dst(count);

    // Initialize src buffer with deterministic pseudo-random values
    std::minstd_rand gen(count);
    std::uniform_int_distribution<> dis(INT16_MIN, INT16_MAX);
    for (size_t i = 0; i < count; i++) {
        src[i] = dis(gen);
    }

    // Run the test
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(src.data());
        benchmark::DoNotOptimize(dst.data());
        memcpy_to_float_from_i16(dst.data(), src.data(), count);
        benchmark::ClobberMemory();
    }

    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_MemcpyToFloatFromI16)->RangeMultiplier(2)->Ranges({{10, 8<<12}});


static void BM_MemcpyToI16FromFloat(benchmark::State& state) {
    const size_t count = state.range(0);

    std::vector<float> src(count);
    std::vector<int16_t> dst(count);

    // Initialize src buffer with deterministic pseudo-random values
    std::minstd_rand gen(count);
    std::uniform_real_distribution<> dis;
    for (size_t i = 0; i < count; i++) {
        src[i] = dis(gen);
    }

    // Run the test
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(src.data());
        benchmark::DoNotOptimize(dst.data());
        memcpy_to_i16_from_float(dst.data(), src.data(), count);
        benchmark::ClobberMemory();
    }

    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_MemcpyToI16FromFloat)->RangeMultiplier(2)->Ranges({{10, 8<<12}});

BENCHMARK_MAIN();
