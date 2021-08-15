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

#ifndef ANDROID_VINTF_UTILS_H
#define ANDROID_VINTF_UTILS_H

#include <memory>
#include <mutex>

#include <utils/Errors.h>
#include <vintf/FileSystem.h>
#include <vintf/PropertyFetcher.h>
#include <vintf/RuntimeInfo.h>
#include <vintf/parse_xml.h>

namespace android {
namespace vintf {
namespace details {

template <typename T>
status_t fetchAllInformation(const FileSystem* fileSystem, const std::string& path,
                             const XmlConverter<T>& converter, T* outObject, std::string* error) {
    std::string info;
    status_t result = fileSystem->fetch(path, &info, error);

    if (result != OK) {
        return result;
    }

    bool success = converter(outObject, info, error);
    if (!success) {
        if (error) {
            *error = "Illformed file: " + path + ": " + *error;
        }
        return BAD_VALUE;
    }
    return OK;
}

// TODO(b/70628538): Do not infer from Shipping API level.
inline Level convertFromApiLevel(size_t apiLevel) {
    if (apiLevel < 26) {
        return Level::LEGACY;
    } else if (apiLevel == 26) {
        return Level::O;
    } else if (apiLevel == 27) {
        return Level::O_MR1;
    } else {
        return Level::UNSPECIFIED;
    }
}

class PropertyFetcherImpl : public PropertyFetcher {
   public:
    virtual std::string getProperty(const std::string& key,
                                    const std::string& defaultValue = "") const;
    virtual uint64_t getUintProperty(const std::string& key, uint64_t defaultValue,
                                     uint64_t max = UINT64_MAX) const;
    virtual bool getBoolProperty(const std::string& key, bool defaultValue) const;
};

class PropertyFetcherNoOp : public PropertyFetcher {
   public:
    virtual std::string getProperty(const std::string& key,
                                    const std::string& defaultValue = "") const override;
    virtual uint64_t getUintProperty(const std::string& key, uint64_t defaultValue,
                                     uint64_t max = UINT64_MAX) const override;
    virtual bool getBoolProperty(const std::string& key, bool defaultValue) const override;
};

// Merge src into dst.
// postcondition (if successful): *src == empty.
template <typename T>
static bool mergeField(T* dst, T* src, const T& empty = T{}) {
    if (*dst == *src) {
        *src = empty;
        return true;  // no conflict
    }
    if (*src == empty) {
        return true;
    }
    if (*dst == empty) {
        *dst = std::move(*src);
        *src = empty;
        return true;
    }
    return false;
}

}  // namespace details
}  // namespace vintf
}  // namespace android



#endif
