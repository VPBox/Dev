/*
 * Copyright 2019 The Android Open Source Project
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

#include <gmock/gmock.h>

namespace {

// Checks for a region match
MATCHER_P(RegionEq, expected, "") {
    std::string buf;
    buf.append("Regions are not equal\n");
    expected.dump(buf, "expected region");
    arg.dump(buf, "actual region");
    *result_listener << buf;

    size_t expectedRectCount = 0;
    android::Rect const* expectedRects = expected.getArray(&expectedRectCount);
    size_t actualRectCount = 0;
    android::Rect const* actualRects = arg.getArray(&actualRectCount);

    if (expectedRectCount != actualRectCount) return false;
    for (size_t i = 0; i < expectedRectCount; i++) {
        if (expectedRects[i] != actualRects[i]) return false;
    }
    return true;
}

} // namespace
