/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <gtest/gtest.h>

#include "netdutils/ThreadUtil.h"

namespace android {
namespace netdutils {

namespace {

class NoopRun {
  public:
    NoopRun() { instanceNum++; }
    ~NoopRun() { instanceNum--; }

    void run() {}

    static bool waitForAllReleased(int timeoutMs) {
        constexpr int intervalMs = 20;
        int limit = timeoutMs / intervalMs;
        for (int i = 1; i < limit; i++) {
            if (instanceNum == 0) {
                return true;
            }
            usleep(intervalMs * 1000);
        }
        return false;
    }

    // To track how many instances are alive.
    static std::atomic<int> instanceNum;
};

std::atomic<int> NoopRun::instanceNum;

}  // namespace

TEST(ThreadUtilTest, objectReleased) {
    NoopRun::instanceNum = 0;
    NoopRun* obj = new NoopRun();
    EXPECT_EQ(1, NoopRun::instanceNum);
    threadLaunch(obj);

    // Wait for the object released along with the thread exited.
    EXPECT_TRUE(NoopRun::waitForAllReleased(1000));
    EXPECT_EQ(0, NoopRun::instanceNum);
}

}  // namespace netdutils
}  // namespace android
