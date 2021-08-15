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

#include <gmock/gmock.h>

namespace {

// Check for a transform match
MATCHER_P(TransformEq, expected, "") {
    std::string buf;
    buf.append("Transforms are not equal\n");
    expected.dump(buf, "expected transform");
    arg.dump(buf, "actual transform");
    *result_listener << buf;

    const float TOLERANCE = 1e-3f;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (std::fabs(expected[i][j] - arg[i][j]) > TOLERANCE) {
                return false;
            }
        }
    }

    return true;
}

} // namespace
