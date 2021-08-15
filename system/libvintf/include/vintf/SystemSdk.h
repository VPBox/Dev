/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef ANDROID_VINTF_SYSTEM_SDK_H
#define ANDROID_VINTF_SYSTEM_SDK_H

#include <stdint.h>

#include <set>
#include <string>

namespace android {
namespace vintf {

// System SDK versions provided for vendor apps.
class SystemSdk {
   public:
    SystemSdk() = default;
    SystemSdk(std::set<std::string>&& versions) : mVersions(std::move(versions)) {}
    SystemSdk(const std::set<std::string>& versions) : mVersions(versions) {}
    const std::set<std::string>& versions() const { return mVersions; }
    bool empty() const { return versions().empty(); }

    bool operator==(const SystemSdk& other) const;
    // return {v : v in this and not in other}
    SystemSdk removeVersions(const SystemSdk& other) const;

    // Move all from "other" to "this".
    void addAll(SystemSdk* other);

   private:
    friend class AssembleVintfImpl;
    friend struct SystemSdkConverter;
    std::set<std::string> mVersions;
};

}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_SYSTEM_SDK_H
