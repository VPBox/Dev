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

#include "CpuExecutor.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <omp.h>
#include <random>
#include <thread>
#include <unistd.h>
#include <vector>

namespace {

class OpenmpSettingsTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        const int blocktimeInitial = kmp_get_blocktime();
        ASSERT_EQ(blocktimeInitial, kOpenmpDefaultBlockTime);
    }
    virtual void TearDown() override {
        const int blocktimeRestored = kmp_get_blocktime();
        ASSERT_EQ(blocktimeRestored, kOpenmpDefaultBlockTime);
    }
    static const int kOpenmpDefaultBlockTime;
    static const int kPreferredBlockTime;
};

const int OpenmpSettingsTest::kOpenmpDefaultBlockTime = 200;
const int OpenmpSettingsTest::kPreferredBlockTime = 20;

using ::android::nn::ScopedOpenmpSettings;

TEST_F(OpenmpSettingsTest, TestkPreferredBlockTime) {
    ScopedOpenmpSettings s;
    const int blocktimeSet = kmp_get_blocktime();
    ASSERT_EQ(blocktimeSet, kPreferredBlockTime);
}

TEST_F(OpenmpSettingsTest, Test2) {
    ScopedOpenmpSettings s1;
    const int blocktimeSet1 = kmp_get_blocktime();
    ASSERT_EQ(blocktimeSet1, kPreferredBlockTime);

    ScopedOpenmpSettings s2;
    const int blocktimeSet2 = kmp_get_blocktime();
    ASSERT_EQ(blocktimeSet2, kPreferredBlockTime);
}

TEST_F(OpenmpSettingsTest, TestThreaded) {
    // Threaded test to validate that each thread gets its own settings.
    std::vector<std::thread> threads;
    std::mt19937 randGen;
    std::uniform_int_distribution<> rand(1, 20);
    for (int i = 0; i < 10; i++) {
        const int sleepFor = rand(randGen);
        threads.push_back(std::thread([sleepFor]() {
            const int blocktimeInitial = kmp_get_blocktime();
            // kmp_get_blocktime() in a new thread returns 0 instead of 200
            // about 10% of time on-device
            ASSERT_TRUE(blocktimeInitial == 0 || blocktimeInitial == kOpenmpDefaultBlockTime);

            ScopedOpenmpSettings s;

            const int blocktimeSet1 = kmp_get_blocktime();
            ASSERT_EQ(blocktimeSet1, kPreferredBlockTime);

            usleep(sleepFor);

            const int blocktimeSet2 = kmp_get_blocktime();
            ASSERT_EQ(blocktimeSet2, kPreferredBlockTime);
        }));
    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& t) {
        t.join();
    });
}

}  // end namespace
