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


#ifndef ANDROID_VINTF_VERSION_H
#define ANDROID_VINTF_VERSION_H

#include <stdint.h>
#include <string>
#include <utility>

namespace android {
namespace vintf {

struct Version {

    constexpr Version() : Version(0u, 0u) {}
    constexpr Version(size_t mj, size_t mi) : majorVer(mj), minorVer(mi) {}
    constexpr Version(const std::pair<size_t, size_t>& pair)
        : majorVer(pair.first), minorVer(pair.second) {}

    size_t majorVer;
    size_t minorVer;

    inline bool operator==(const Version &other) const {
        return majorVer == other.majorVer && minorVer == other.minorVer;
    }
    inline bool operator!=(const Version &other) const {
        return !((*this) == other);
    }
    inline bool operator<(const Version &other) const {
        if (majorVer < other.majorVer)
            return true;
        if (majorVer > other.majorVer)
            return false;
        return minorVer < other.minorVer;
    }
    inline bool operator>(const Version &other) const {
        return other < (*this);
    }
    inline bool operator<=(const Version &other) const {
        return !((*this) > other);
    }
    inline bool operator>=(const Version &other) const {
        return !((*this) < other);
    }
    // Version(2, 1).minorAtLeast(Version(1, 0)) == false
    // Version(2, 1).minorAtLeast(Version(2, 0)) == true
    // Version(2, 1).minorAtLeast(Version(2, 1)) == true
    // Version(2, 1).minorAtLeast(Version(2, 2)) == false
    inline bool minorAtLeast(const Version& other) const {
        return majorVer == other.majorVer && minorVer >= other.minorVer;
    }
};

struct KernelVersion {

    constexpr KernelVersion() : KernelVersion(0u, 0u, 0u) {}
    constexpr KernelVersion(size_t v, size_t mj, size_t mi) :
            version(v), majorRev(mj), minorRev(mi) {}

    size_t version;
    size_t majorRev;
    size_t minorRev;

    inline bool operator==(const KernelVersion &other) const {
        return version == other.version
            && majorRev == other.majorRev
            && minorRev == other.minorRev;
    }
    inline bool operator!=(const KernelVersion &other) const {
        return !((*this) == other);
    }

    inline bool operator<(const KernelVersion& other) const {
        if (version < other.version) return true;
        if (version > other.version) return false;
        if (majorRev < other.majorRev) return true;
        if (majorRev > other.majorRev) return false;
        return minorRev < other.minorRev;
    }
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_VERSION_H
