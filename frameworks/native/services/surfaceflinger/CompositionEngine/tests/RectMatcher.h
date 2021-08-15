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

#include <string>

#include <android-base/stringprintf.h>
#include <gmock/gmock.h>

namespace {

using android::base::StringAppendF;
using Rect = android::Rect;

void dumpRect(const Rect& rect, std::string& result, const char* name) {
    StringAppendF(&result, "%s (%d %d %d %d) ", name, rect.left, rect.top, rect.right, rect.bottom);
}

// Checks for a region match
MATCHER_P(RectEq, expected, "") {
    std::string buf;
    buf.append("Rects are not equal\n");
    dumpRect(expected, buf, "expected rect");
    dumpRect(arg, buf, "actual rect");
    *result_listener << buf;

    return (expected.left == arg.left) && (expected.top == arg.top) &&
            (expected.right == arg.right) && (expected.bottom == arg.bottom);
}

} // namespace
