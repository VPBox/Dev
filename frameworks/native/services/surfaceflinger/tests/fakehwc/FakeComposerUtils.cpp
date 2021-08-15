/*
 * Copyright 2017 The Android Open Source Project
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

#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "FakeHwcUtil"
#include <log/log.h>

#include "FakeComposerUtils.h"
#include "RenderState.h"

#include "SurfaceFlinger.h" // Get the name of the service...

#include <binder/IServiceManager.h>

#include <cutils/properties.h>

#include <iomanip>
#include <thread>

using android::String16;
using android::sp;
using namespace std::chrono_literals;
using namespace sftest;
using std::setw;

namespace sftest {

// clang-format off
inline void printSourceRectAligned(::std::ostream& os, const hwc_frect_t& sourceRect, int align) {
    os << std::fixed << std::setprecision(1) << "("
       << setw(align) << sourceRect.left << setw(0) << ","
       << setw(align) << sourceRect.top << setw(0) << ","
       << setw(align) << sourceRect.right << setw(0) << ","
       << setw(align) << sourceRect.bottom << setw(0) << ")";
}

inline void printDisplayRectAligned(::std::ostream& os, const hwc_rect_t& displayRect, int align) {
    os << "("
       << setw(align) << displayRect.left << setw(0) << ","
       << setw(align) << displayRect.top << setw(0) << ","
       << setw(align) << displayRect.right << setw(0) << ","
       << setw(align) << displayRect.bottom << setw(0) << ")";
}
// clang-format on

inline ::std::ostream& operator<<(::std::ostream& os, const sftest::RenderState& state) {
    printSourceRectAligned(os, state.mSourceCrop, 7);
    os << "->";
    printDisplayRectAligned(os, state.mDisplayFrame, 5);
    return os << " Swaps:" << state.mSwapCount << " Alpha:" << std::setprecision(3)
              << state.mPlaneAlpha << " Xform:" << state.mTransform;
}

// Helper for verifying the parts of the RenderState
template <typename T>
bool valuesMatch(::testing::AssertionResult& message, const T& ref, const T& val,
                 const char* name) {
    if (ref != val) {
        message = message << "Expected " << name << ":" << ref << ", got:" << val << ".";
        return false;
    }
    return true;
}

::testing::AssertionResult rectsAreSame(const RenderState& ref, const RenderState& val) {
    // TODO: Message could start as success and be assigned as failure.
    // Only problem is that utility assumes it to be failure and just adds stuff. Would
    // need still special case the initial failure in the utility?
    // TODO: ... or would it be possible to break this back to gtest primitives?
    ::testing::AssertionResult message = ::testing::AssertionFailure();
    bool passes = true;

    // The work here is mostly about providing good log strings for differences
    passes &= valuesMatch(message, ref.mDisplayFrame, val.mDisplayFrame, "display frame");
    passes &= valuesMatch(message, ref.mPlaneAlpha, val.mPlaneAlpha, "alpha");
    passes &= valuesMatch(message, ref.mSwapCount, val.mSwapCount, "swap count");
    passes &= valuesMatch(message, ref.mSourceCrop, val.mSourceCrop, "source crop");
    // ... add more
    if (passes) {
        return ::testing::AssertionSuccess();
    }
    return message;
}

::testing::AssertionResult framesAreSame(const std::vector<RenderState>& ref,
                                         const std::vector<RenderState>& val) {
    ::testing::AssertionResult message = ::testing::AssertionFailure();
    bool passed = true;
    if (ref.size() != val.size()) {
        message << "Expected " << ref.size() << " rects, got " << val.size() << ".";
        passed = false;
    }
    for (size_t rectIndex = 0; rectIndex < std::min(ref.size(), val.size()); rectIndex++) {
        ::testing::AssertionResult rectResult = rectsAreSame(ref[rectIndex], val[rectIndex]);
        if (rectResult == false) {
            message << "First different rect at " << rectIndex << ": " << rectResult.message();
            passed = false;
            break;
        }
    }

    if (passed) {
        return ::testing::AssertionSuccess();
    } else {
        message << "\nReference:";
        for (auto state = ref.begin(); state != ref.end(); ++state) {
            message << "\n" << *state;
        }
        message << "\nActual:";
        for (auto state = val.begin(); state != val.end(); ++state) {
            message << "\n" << *state;
        }
    }
    return message;
}

void startSurfaceFlinger() {
    ALOGI("Start SurfaceFlinger");
    system("start surfaceflinger");

    sp<android::IServiceManager> sm(android::defaultServiceManager());
    sp<android::IBinder> sf;
    while (sf == nullptr) {
        std::this_thread::sleep_for(10ms);
        sf = sm->checkService(String16(android::SurfaceFlinger::getServiceName()));
    }
    ALOGV("SurfaceFlinger running");
}

void stopSurfaceFlinger() {
    ALOGI("Stop SurfaceFlinger");
    system("stop surfaceflinger");
    sp<android::IServiceManager> sm(android::defaultServiceManager());
    sp<android::IBinder> sf;
    while (sf != nullptr) {
        std::this_thread::sleep_for(10ms);
        sf = sm->checkService(String16(android::SurfaceFlinger::getServiceName()));
    }
    ALOGV("SurfaceFlinger stopped");
}

////////////////////////////////////////////////

void FakeHwcEnvironment::SetUp() {
    ALOGI("Test env setup");
    system("setenforce 0");
    system("stop");
    property_set("debug.sf.nobootanimation", "1");
    {
        char value[PROPERTY_VALUE_MAX];
        property_get("debug.sf.nobootanimation", value, "0");
        LOG_FATAL_IF(atoi(value) != 1, "boot skip not set");
    }
    // TODO: Try registering the mock as the default service instead.
    property_set("debug.sf.hwc_service_name", "mock");
    // This allows the SurfaceFlinger to load a HIDL service not listed in manifest files.
    property_set("debug.sf.treble_testing_override", "true");
}

void FakeHwcEnvironment::TearDown() {
    ALOGI("Test env tear down");
    system("stop");
    // Wait for mock call signaling teardown?
    property_set("debug.sf.nobootanimation", "0");
    property_set("debug.sf.hwc_service_name", "default");
    ALOGI("Test env tear down - done");
}

} // namespace sftest
