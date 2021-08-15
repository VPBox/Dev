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

#include <set>
#include <string>

#include "HalInterface.h"

namespace android {
namespace vintf {

bool operator==(const HalInterface& lft, const HalInterface& rgt) {
    if (lft.mName != rgt.mName) return false;
    if (lft.mInstances != rgt.mInstances) return false;
    return true;
}

bool HalInterface::forEachInstance(
    const std::function<bool(const std::string&, const std::string&, bool isRegex)>& func) const {
    for (const auto& instance : mInstances) {
        if (!func(mName, instance, false /* isRegex */)) {
            return false;
        }
    }
    for (const auto& instance : mRegexes) {
        if (!func(mName, instance, true /* isRegex */)) {
            return false;
        }
    }
    return true;
}

bool HalInterface::hasAnyInstance() const {
    bool found = false;
    forEachInstance([&found](const auto&, const auto&, bool) {
        found = true;
        return false;  // break;
    });
    return found;
}

bool HalInterface::insertInstance(const std::string& instanceOrPattern, bool isRegex) {
    if (isRegex) {
        return mRegexes.insert(instanceOrPattern).second;
    } else {
        return mInstances.insert(instanceOrPattern).second;
    }
}

bool HalInterface::removeInstance(const std::string& instanceOrPattern, bool isRegex) {
    if (isRegex) {
        return mRegexes.erase(instanceOrPattern) > 0;
    } else {
        return mInstances.erase(instanceOrPattern) > 0;
    }
}

} // namespace vintf
} // namespace android
