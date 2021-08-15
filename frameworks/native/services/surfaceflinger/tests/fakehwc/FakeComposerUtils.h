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

#pragma once

#include "FakeComposerClient.h"

#include <gui/SurfaceComposerClient.h>

#include <hardware/hwcomposer_defs.h>

#include <log/log.h>

#include <gtest/gtest.h>

// clang-format off
// Note: This needs to reside in the global namespace for the GTest to use it
inline ::std::ostream& operator<<(::std::ostream& os, const hwc_rect_t& rect) {
    return os << "(" << rect.left << ","
              << rect.top << ","
              << rect.right << ","
              << rect.bottom << ")";
}

inline ::std::ostream& operator<<(::std::ostream& os, const hwc_frect_t& rect) {
    return os << "(" << rect.left << ","
              << rect.top << ","
              << rect.right << ","
              << rect.bottom << ")";
}
// clang-format on

namespace sftest {

class RenderState;

// clang-format off
inline bool operator==(const hwc_rect_t& a, const hwc_rect_t& b) {
    return a.top == b.top &&
            a.left == b.left &&
            a.bottom == b.bottom &&
            a.right == b.right;
}

inline bool operator==(const hwc_frect_t& a, const hwc_frect_t& b) {
    return a.top == b.top &&
            a.left == b.left &&
            a.bottom == b.bottom &&
            a.right == b.right;
}
// clang-format on

inline bool operator!=(const hwc_rect_t& a, const hwc_rect_t& b) {
    return !(a == b);
}

inline bool operator!=(const hwc_frect_t& a, const hwc_frect_t& b) {
    return !(a == b);
}

::testing::AssertionResult rectsAreSame(const RenderState& ref, const RenderState& val);
::testing::AssertionResult framesAreSame(const std::vector<RenderState>& ref,
                                         const std::vector<RenderState>& val);

void startSurfaceFlinger();
void stopSurfaceFlinger();

class FakeHwcEnvironment : public ::testing::Environment {
public:
    virtual ~FakeHwcEnvironment() {}
    void SetUp() override;
    void TearDown() override;
};

/*
 * All surface state changes are supposed to happen inside a global
 * transaction. TransactionScope object at the beginning of
 * scope automates the process. The resulting scope gives a visual cue
 * on the span of the transaction as well.
 *
 * Closing the transaction is synchronous, i.e., it waits for
 * SurfaceFlinger to composite one frame. Now, the FakeComposerClient
 * is built to explicitly request vsyncs one at the time. A delayed
 * request must be made before closing the transaction or the test
 * thread stalls until SurfaceFlinger does an emergency vsync by
 * itself. TransactionScope encapsulates this vsync magic.
 */
class TransactionScope : public android::SurfaceComposerClient::Transaction {
public:
    explicit TransactionScope(FakeComposerClient& composer) : Transaction(), mComposer(composer) {}

    ~TransactionScope() {
        int frameCount = mComposer.getFrameCount();
        mComposer.runVSyncAfter(1ms);
        LOG_ALWAYS_FATAL_IF(android::NO_ERROR != apply());
        // Make sure that exactly one frame has been rendered.
        mComposer.waitUntilFrame(frameCount + 1);
        LOG_ALWAYS_FATAL_IF(frameCount + 1 != mComposer.getFrameCount(),
                            "Unexpected frame advance. Delta: %d",
                            mComposer.getFrameCount() - frameCount);
    }

    FakeComposerClient& mComposer;
};

} // namespace sftest
