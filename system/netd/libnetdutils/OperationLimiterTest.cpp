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

#include "netdutils/OperationLimiter.h"

#include <gtest/gtest-spi.h>

namespace android {
namespace netdutils {

TEST(OperationLimiter, limits) {
    OperationLimiter<int> limiter(3);

    EXPECT_TRUE(limiter.start(42));
    EXPECT_TRUE(limiter.start(42));
    EXPECT_TRUE(limiter.start(42));

    // Limit reached... calling any number of times should have no effect.
    EXPECT_FALSE(limiter.start(42));
    EXPECT_FALSE(limiter.start(42));
    EXPECT_FALSE(limiter.start(42));

    // Finishing a single operations is enough for starting a new one...
    limiter.finish(42);
    EXPECT_TRUE(limiter.start(42));

    // ...but not two!
    EXPECT_FALSE(limiter.start(42));

    // Different ids should still have quota...
    EXPECT_TRUE(limiter.start(666));
    limiter.finish(666);

    // Finish all pending operations
    limiter.finish(42);
    limiter.finish(42);
    limiter.finish(42);
}

TEST(OperationLimiter, finishWithoutStart) {
    OperationLimiter<int> limiter(1);

    // Will output a LOG(FATAL_WITHOUT_ABORT), but we have no way to probe this.
    limiter.finish(42);

    // This will ensure that the finish() above didn't set a negative value.
    EXPECT_TRUE(limiter.start(42));
    EXPECT_FALSE(limiter.start(42));
}

TEST(OperationLimiter, destroyWithActiveOperations) {
    // The death message doesn't seem to be captured on Android.
    EXPECT_DEBUG_DEATH({
        OperationLimiter<int> limiter(3);
        limiter.start(42);
    }, "" /* "active operations */);
}

}  // namespace netdutils
}  // namespace android
