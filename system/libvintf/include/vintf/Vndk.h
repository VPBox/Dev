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


#ifndef ANDROID_VINTF_VNDK_H
#define ANDROID_VINTF_VNDK_H

#include <set>
#include <string>

namespace android {
namespace vintf {

// deprecated. Kept here for libvintf backwards compatibility.
struct [[deprecated]] VndkVersionRange {

    VndkVersionRange() : VndkVersionRange(0u, 0u, 0u) {}
    VndkVersionRange(size_t s, size_t v, size_t p)
        : VndkVersionRange(s, v, p, p) {}
    VndkVersionRange(size_t s, size_t v, size_t pi, size_t pa)
        : sdk(s), vndk(v), patchMin(pi), patchMax(pa) {}

    inline bool isSingleVersion() const { return patchMin == patchMax; }

    size_t sdk;
    size_t vndk;
    size_t patchMin;
    size_t patchMax;
};

// deprecated. Kept here for libvintf backwards compatibility.
struct [[deprecated]] Vndk {

    const VndkVersionRange &versionRange() const { return mVersionRange; }
    const std::set<std::string> &libraries() const { return mLibraries; }

private:
    friend struct VndkConverter;
    friend struct HalManifestConverter;
    friend struct LibVintfTest;
    friend struct HalManifest;
    friend struct CompatibilityMatrix;
    friend bool operator==(const Vndk &, const Vndk &);
    VndkVersionRange mVersionRange;
    std::set<std::string> mLibraries;
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
inline bool operator==(const VndkVersionRange &lft, const VndkVersionRange &rgt) {
    return lft.sdk == rgt.sdk && lft.vndk == rgt.vndk &&
           lft.patchMin == rgt.patchMin && lft.patchMax == rgt.patchMax;
}

inline bool operator==(const Vndk &lft, const Vndk &rgt) {
    return lft.mVersionRange == rgt.mVersionRange &&
           lft.mLibraries == rgt.mLibraries;
}
#pragma clang diagnostic pop

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_VNDK_H
