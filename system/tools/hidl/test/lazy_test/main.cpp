/*
 * Copyright (C) 2019, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>

#include <unistd.h>

#include <gtest/gtest.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <hidl/HidlTransportUtils.h>
#include <hwbinder/IPCThreadState.h>

using ::android::sp;
using ::android::hardware::IPCThreadState;
using ::android::hidl::base::V1_0::IBase;

static std::string gDescriptor;
static std::string gInstance;

sp<IBase> getHal() {
    return ::android::hardware::details::getRawServiceInternal(gDescriptor, gInstance,
                                                               true /*retry*/, false /*getStub*/);
}

static constexpr size_t NUM_IMMEDIATE_GET_UNGETS = 100;
TEST(LazyHidl, GetUnget) {
    for (size_t i = 0; i < NUM_IMMEDIATE_GET_UNGETS; i++) {
        IPCThreadState::self()->flushCommands();
        sp<IBase> hal = getHal();
        ASSERT_NE(nullptr, hal.get());
        EXPECT_TRUE(hal->ping().isOk());
    }
}

static std::vector<size_t> waitTimes(size_t numTimes, size_t maxWait) {
    std::vector<size_t> times(numTimes);
    for (size_t i = 0; i < numTimes; i++) {
        times[i] = (size_t)(rand() % (maxWait + 1));
    }
    return times;
}

static void testWithTimes(const std::vector<size_t>& waitTimes) {
    std::cout << "Note runtime expected from sleeps: "
              << std::accumulate(waitTimes.begin(), waitTimes.end(), 0) << " second(s)."
              << std::endl;

    for (size_t sleepTime : waitTimes) {
        IPCThreadState::self()->flushCommands();
        std::cout << "Thread waiting " << sleepTime << " while not holding HAL." << std::endl;
        sleep(sleepTime);
        sp<IBase> hal = getHal();
        ASSERT_NE(nullptr, hal.get());
        ASSERT_TRUE(hal->ping().isOk());
    }
}

static constexpr size_t NUM_TIMES_GET_UNGET = 5;
static constexpr size_t MAX_WAITING_DURATION = 10;
static constexpr size_t NUM_CONCURRENT_THREADS = 5;
TEST(LazyHidl, GetWithWaitConcurrent) {
    std::vector<std::vector<size_t>> threadWaitTimes(NUM_CONCURRENT_THREADS);

    for (size_t i = 0; i < threadWaitTimes.size(); i++) {
        threadWaitTimes[i] = waitTimes(NUM_TIMES_GET_UNGET, MAX_WAITING_DURATION);
    }

    std::vector<std::thread> threads(NUM_CONCURRENT_THREADS);
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i] = std::thread(testWithTimes, threadWaitTimes[i]);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    srand(time(nullptr));

    if (argc != 3) {
        std::cerr << "Usage: lazy_test fqname instance" << std::endl;
        return 1;
    }

    gDescriptor = argv[1];
    gInstance = argv[2];

    return RUN_ALL_TESTS();
}