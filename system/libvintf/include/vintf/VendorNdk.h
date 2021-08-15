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

#ifndef ANDROID_VINTF_VENDOR_NDK_H
#define ANDROID_VINTF_VENDOR_NDK_H

#include <set>
#include <string>

namespace android {
namespace vintf {

class VendorNdk {
   public:
    VendorNdk() = default;
    VendorNdk(std::string&& version) : mVersion(std::move(version)) {}
    VendorNdk(std::string&& version, std::set<std::string>&& libs)
        : mVersion(std::move(version)), mLibraries(std::move(libs)) {}
    VendorNdk(const std::string& version) : mVersion(version) {}
    VendorNdk(const std::string& version, const std::set<std::string>& libs)
        : mVersion(version), mLibraries(libs) {}
    bool operator==(const VendorNdk& other) const { return version() == other.version(); }
    const std::string& version() const { return mVersion; }
    const std::set<std::string>& libraries() const { return mLibraries; }

   private:
    friend struct VendorNdkConverter;
    std::string mVersion;
    std::set<std::string> mLibraries;
};

}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_VENDOR_NDK_H
