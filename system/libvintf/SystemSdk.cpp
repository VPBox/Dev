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

#include "SystemSdk.h"

#include <algorithm>

namespace android {
namespace vintf {

SystemSdk SystemSdk::removeVersions(const SystemSdk& other) const {
    SystemSdk ret;
    std::set_difference(versions().begin(), versions().end(), other.versions().begin(),
                        other.versions().end(), std::inserter(ret.mVersions, ret.mVersions.end()));
    return ret;
}

bool SystemSdk::operator==(const SystemSdk& other) const {
    return versions() == other.versions();
}

void SystemSdk::addAll(SystemSdk* other) {
    mVersions.insert(other->mVersions.begin(), other->mVersions.end());
    other->mVersions.clear();
}

}  // namespace vintf
}  // namespace android
