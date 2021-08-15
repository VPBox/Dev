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

#pragma once

namespace android {
namespace vintf {

class PropertyFetcher {
   public:
    virtual ~PropertyFetcher() = default;
    virtual std::string getProperty(const std::string& key,
                                    const std::string& defaultValue = "") const = 0;
    virtual uint64_t getUintProperty(const std::string& key, uint64_t defaultValue,
                                     uint64_t max = UINT64_MAX) const = 0;
    virtual bool getBoolProperty(const std::string& key, bool defaultValue) const = 0;
};

}  // namespace vintf
}  // namespace android
