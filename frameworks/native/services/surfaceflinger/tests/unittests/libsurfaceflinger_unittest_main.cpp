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

#include <gtest/gtest.h>

#include <sched.h>

#include <processgroup/sched_policy.h>
#include <system/graphics.h>

#include "libsurfaceflinger_unittest_main.h"

// ------------------------------------------------------------------------
// To pass extra command line arguments to the Google Test executable from
// atest, you have to use this somewhat verbose syntax:
//
// clang-format off
//
//     atest libsurfaceflinger_unittest -- --module-arg libsurfaceflinger_unittest:native-test-flag:<--flag>[:<value>]
//
// For example:
//
//     atest libsurfaceflinger_unittest -- --module-arg libsurfaceflinger_unittest:native-test-flag:--no-slow
//
// clang-format on
//  ------------------------------------------------------------------------

// Set to true if "--no-slow" is passed to the test.
bool g_noSlowTests = false;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // The SurfaceFlinger implementation assumes that threads resume
    // execution as quickly as possible once they become unblocked.
    // (These same calls are made in main_surfaceflinger.cpp)
    setpriority(PRIO_PROCESS, 0, HAL_PRIORITY_URGENT_DISPLAY);
    set_sched_policy(0, SP_FOREGROUND);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--no-slow") == 0) {
            g_noSlowTests = true;
        }
    }

    return RUN_ALL_TESTS();
}