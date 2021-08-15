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

#include <android-base/stringprintf.h>
#include <benchmark/benchmark.h>

#include "bpf/BpfMap.h"
#include "bpf/BpfUtils.h"

constexpr uint32_t TEST_MAP_SIZE = 10000;

using android::base::StringPrintf;
using android::bpf::BpfMap;

class BpfBenchMark : public ::benchmark::Fixture {
  public:
    BpfBenchMark() : mBpfTestMap(BPF_MAP_TYPE_HASH, TEST_MAP_SIZE, BPF_F_NO_PREALLOC) {}
    BpfMap<uint32_t, uint32_t> mBpfTestMap;
};

BENCHMARK_DEFINE_F(BpfBenchMark, MapWriteNewEntry)(benchmark::State& state) {
    for (auto _ : state) {
        expectOk(mBpfTestMap.writeValue(state.range(0), state.range(0), BPF_NOEXIST));
    }
}

BENCHMARK_DEFINE_F(BpfBenchMark, MapUpdateEntry)(benchmark::State& state) {
    for (uint32_t i = 0; i < TEST_MAP_SIZE; i++) {
        expectOk(mBpfTestMap.writeValue(i, i, BPF_NOEXIST));
    }
    for (auto _ : state) {
        expectOk(mBpfTestMap.writeValue(state.range(0), state.range(0) + 1, BPF_EXIST));
    }
}

BENCHMARK_DEFINE_F(BpfBenchMark, MapDeleteAddEntry)(benchmark::State& state) {
    for (uint32_t i = 0; i < TEST_MAP_SIZE; i++) {
        expectOk(mBpfTestMap.writeValue(i, i, BPF_NOEXIST));
    }
    for (auto _ : state) {
        expectOk(mBpfTestMap.deleteValue(state.range(0)));
        expectOk(mBpfTestMap.writeValue(state.range(0), state.range(0) + 1, BPF_NOEXIST));
    }
}

BENCHMARK_DEFINE_F(BpfBenchMark, WaitForRcu)(benchmark::State& state) {
    for (auto _ : state) {
        int ret = android::bpf::synchronizeKernelRCU();
        if (ret) {
            state.SkipWithError(
                    StringPrintf("synchronizeKernelRCU() failed with errno=%d", -ret).c_str());
        }
    }
}

BENCHMARK_REGISTER_F(BpfBenchMark, MapUpdateEntry)->Arg(1);
BENCHMARK_REGISTER_F(BpfBenchMark, MapWriteNewEntry)->Arg(1);
BENCHMARK_REGISTER_F(BpfBenchMark, MapDeleteAddEntry)->Arg(1);
BENCHMARK_REGISTER_F(BpfBenchMark, WaitForRcu)->Arg(1);
BENCHMARK_MAIN();
