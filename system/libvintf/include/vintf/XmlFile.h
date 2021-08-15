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

#ifndef ANDROID_VINTF_XML_FILE_H
#define ANDROID_VINTF_XML_FILE_H

#include <string>

#include "Version.h"
#include "VersionRange.h"
#include "XmlSchemaFormat.h"

namespace android {
namespace vintf {

struct XmlFile {
   public:
    inline const std::string& name() const { return mName; }
    inline const std::string& overriddenPath() const { return mOverriddenPath; }

   protected:
    std::string mName;
    std::string mOverriddenPath;
};

// An <xmlfile> entry in matrix
struct MatrixXmlFile : public XmlFile {
    inline bool optional() const { return mOptional; }
    inline XmlSchemaFormat format() const { return mFormat; }
    inline const VersionRange& versionRange() const { return mVersionRange; }
    bool operator==(const MatrixXmlFile& other) const;

   private:
    friend struct CompatibilityMatrix;
    friend struct MatrixXmlFileConverter;
    friend struct LibVintfTest;
    bool mOptional;
    XmlSchemaFormat mFormat;
    VersionRange mVersionRange;
};

// An <xmlfile> entry in manifest
struct ManifestXmlFile : public XmlFile {
    inline const Version& version() const { return mVersion; }
    bool operator==(const ManifestXmlFile& other) const;

   private:
    friend struct ManifestXmlFileConverter;
    friend struct LibVintfTest;
    Version mVersion;
};

}  // namespace vintf
}  // namespace android
#endif  // ANDROID_VINTF_XML_FILE_H
