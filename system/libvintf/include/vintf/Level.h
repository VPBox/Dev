/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef ANDROID_VINTF_LEVEL_H
#define ANDROID_VINTF_LEVEL_H

#include <stdint.h>

namespace android {
namespace vintf {

// Manifest and Compatibility Matrix Level, a.k.a FCM Version, is a number assigned to each
// manifest / matrix.
// - For manifest, the FCM Version that it implements
// - For matrix, the single FCM Version that this matrix file details.
// This is not a strong-typed enum because Level can be any integer value. Listed are some
// special values.
enum Level : size_t {
    // Non-Treble devices.
    LEGACY = 0,
    // Actual values starts from 1. Following are some historic values for convenience.
    O = 1,
    O_MR1 = 2,
    // For older manifests and compatibility matrices, "level" is not specified.
    UNSPECIFIED = SIZE_MAX,
};

}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_LEVEL_H
