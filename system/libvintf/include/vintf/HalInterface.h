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

#ifndef ANDROID_VINTF_HAL_INTERFACE_H_
#define ANDROID_VINTF_HAL_INTERFACE_H_

#include <functional>
#include <set>
#include <string>

#include "Regex.h"

namespace android {
namespace vintf {

// manifest.hal.interface element / compatibility-matrix.hal.interface element
struct HalInterface {
    HalInterface() = default;
    HalInterface(std::string&& name, std::set<std::string>&& instances)
        : mName(std::move(name)), mInstances(std::move(instances)) {}
    HalInterface(const std::string& name, const std::set<std::string>& instances)
        : mName(name), mInstances(instances) {}

    bool forEachInstance(
        const std::function<bool(const std::string& interface, const std::string& instance,
                                 bool isRegex)>& func) const;
    bool hasAnyInstance() const;

    // Return true if inserted, false otherwise.
    bool insertInstance(const std::string& instanceOrPattern, bool isRegex);

    // Return true if removed, false otherwise.
    bool removeInstance(const std::string& instanceOrPattern, bool isRegex);

    const std::string& name() const { return mName; }

   private:
    friend bool operator==(const HalInterface&, const HalInterface&);
    friend struct HalInterfaceConverter;

    std::string mName;
    std::set<std::string> mInstances;
    std::set<std::string> mRegexes;
};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_HAL_INTERFACE_H_
