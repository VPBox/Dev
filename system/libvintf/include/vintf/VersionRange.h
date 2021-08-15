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


#ifndef ANDROID_VINTF_VERSION_RANGE_H
#define ANDROID_VINTF_VERSION_RANGE_H

#include <stdint.h>
#include <string>
#include <tuple>
#include <utility>

#include "Version.h"

namespace android {
namespace vintf {

// A version range with the same major version, e.g. 2.3-7
struct VersionRange {
    VersionRange() : VersionRange(0u, 0u, 0u) {}
    VersionRange(size_t mjV, size_t miV) : VersionRange(mjV, miV, miV) {}
    VersionRange(size_t mjV, size_t miM, size_t mxM)
            : majorVer(mjV), minMinor(miM), maxMinor(mxM) {}
    inline Version minVer() const { return Version(majorVer, minMinor); }
    inline Version maxVer() const { return Version(majorVer, maxMinor); }
    inline bool isSingleVersion() const { return minMinor == maxMinor; }

    inline bool operator==(const VersionRange &other) const {
        return majorVer == other.majorVer
            && minMinor == other.minMinor
            && maxMinor == other.maxMinor;
    }

    inline bool contains(const Version &ver) const {
        return minVer() <= ver && ver <= maxVer();
    }

    // If this == 2.3-7,
    //     ver == 2.2: false
    //     ver == 2.3: true
    //     ver == 2.7: true
    //     ver == 2.8: false
    inline bool supportedBy(const Version &ver) const {
        return majorVer == ver.majorVer && minMinor <= ver.minorVer;
    }

    // If a.overlaps(b) then b.overlaps(a).
    // 1.2-4 and 2.2-4: false
    // 1.2-4 and 1.4-5: true
    // 1.2-4 and 1.0-1: false
    inline bool overlaps(const VersionRange& other) const {
        return majorVer == other.majorVer && minMinor <= other.maxMinor &&
               other.minMinor <= maxMinor;
    }

    size_t majorVer;
    size_t minMinor;
    size_t maxMinor;
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_VERSION_RANGE_H
