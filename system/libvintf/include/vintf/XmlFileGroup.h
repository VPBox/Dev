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

#ifndef ANDROID_VINTF_XML_FILE_GROUP_H
#define ANDROID_VINTF_XML_FILE_GROUP_H

#include <map>
#include <type_traits>

#include "MapValueIterator.h"
#include "XmlFile.h"

namespace android {
namespace vintf {

// A XmlFileGroup is a wrapped multimap from name to T, where T
// must be a subclass of XmlFile.
template <typename T>
struct XmlFileGroup {
    static_assert(std::is_base_of<XmlFile, T>::value, "T must be a subclass of XmlFile");

   private:
    using map = std::multimap<std::string, T>;
    using const_range = std::pair<typename map::const_iterator, typename map::const_iterator>;

   public:
    virtual ~XmlFileGroup() {}

    bool addXmlFile(T&& t) {
        if (!shouldAddXmlFile(t)) {
            return false;
        }
        std::string name = t.name();
        mXmlFiles.emplace(std::move(name), std::move(t));
        return true;
    }

    virtual bool shouldAddXmlFile(const T&) const { return true; }

    const_range getXmlFiles(const std::string& key) const { return mXmlFiles.equal_range(key); }

    // Return an iterable to all T objects. Call it as follows:
    // for (const auto& e : vm.getXmlFiles()) { }
    ConstMultiMapValueIterable<std::string, T> getXmlFiles() const {
        return ConstMultiMapValueIterable<std::string, T>(mXmlFiles);
    }

    bool addAllXmlFiles(XmlFileGroup* other, std::string* error) {
        for (auto& pair : other->mXmlFiles) {
            if (!addXmlFile(std::move(pair.second))) {
                if (error) {
                    *error = "XML File \"" + pair.first + "\" has a conflict.";
                }
                return false;
            }
        }
        other->mXmlFiles.clear();
        return true;
    }

   protected:
    map mXmlFiles;
};

}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_XML_FILE_GROUP_H
