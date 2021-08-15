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

#ifndef ANDROID_VINTF_MANIFEST_INSTANCE_H
#define ANDROID_VINTF_MANIFEST_INSTANCE_H

#include <string>

#include <hidl-util/FqInstance.h>

#include "HalFormat.h"
#include "TransportArch.h"
#include "Version.h"

namespace android {
namespace vintf {

class ManifestInstance {
   public:
    ManifestInstance();
    ManifestInstance(const ManifestInstance&);
    ManifestInstance(ManifestInstance&&) noexcept;
    ManifestInstance& operator=(const ManifestInstance&);
    ManifestInstance& operator=(ManifestInstance&&) noexcept;

    using VersionType = Version;
    ManifestInstance(FqInstance&& fqInstance, TransportArch&& ta, HalFormat fmt);
    ManifestInstance(const FqInstance& fqInstance, const TransportArch& ta, HalFormat fmt);
    const std::string& package() const;
    Version version() const;
    const std::string& interface() const;
    const std::string& instance() const;
    Transport transport() const;
    Arch arch() const;
    HalFormat format() const;

    bool operator==(const ManifestInstance& other) const;
    bool operator<(const ManifestInstance& other) const;

    // Convenience methods.
    // return package@version::interface/instance
    const FqInstance& getFqInstance() const;
    // return @version::interface/instance
    FqInstance getFqInstanceNoPackage() const;

   private:
    FqInstance mFqInstance;
    TransportArch mTransportArch;
    HalFormat mHalFormat;
};

}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_MANIFEST_INSTANCE_H
