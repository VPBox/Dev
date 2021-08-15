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

#ifndef ANDROID_VINTF_SEPOLICY_H
#define ANDROID_VINTF_SEPOLICY_H


namespace android {
namespace vintf {

struct KernelSepolicyVersion {
    size_t value;
    inline KernelSepolicyVersion() : KernelSepolicyVersion(0u) {}
    inline KernelSepolicyVersion(size_t v) : value(v) {}
    inline operator size_t() const {
        return value;
    }
};

// Sepolicy section in compatibility matrix.
struct Sepolicy {

    Sepolicy() : Sepolicy(0u, {}) {}
    Sepolicy(KernelSepolicyVersion kernelSepolicyVersion,
            std::vector<VersionRange> &&sepolicyVersions) :
            mKernelSepolicyVersion(kernelSepolicyVersion),
            mSepolicyVersionRanges(std::move(sepolicyVersions)) {}

    inline size_t kernelSepolicyVersion() const { return mKernelSepolicyVersion.value; }
    inline const std::vector<VersionRange> &sepolicyVersions() const {
        return mSepolicyVersionRanges;
    }

   private:
    friend class AssembleVintfImpl;
    friend struct SepolicyConverter;
    KernelSepolicyVersion mKernelSepolicyVersion;
    std::vector<VersionRange> mSepolicyVersionRanges;
};

inline bool operator==(const Sepolicy &lft, const Sepolicy &rgt) {
    return lft.kernelSepolicyVersion() == rgt.kernelSepolicyVersion() &&
           lft.sepolicyVersions() == rgt.sepolicyVersions();
}

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_SEPOLICY_H
