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

#ifndef ANDROID_VINTF_SERIALIZE_FLAGS_H
#define ANDROID_VINTF_SERIALIZE_FLAGS_H

#include <stdint.h>

namespace android {
namespace vintf {

namespace SerializeFlags {

class Type {
   public:
    explicit constexpr Type(uint32_t value) : mValue(value) {}

#define VINTF_SERIALIZE_FLAGS_FIELD(name, bit)                                  \
    constexpr Type enable##name() const { return Type(mValue | (1 << bit)); }   \
    constexpr Type disable##name() const { return Type(mValue & ~(1 << bit)); } \
    constexpr bool is##name##Enabled() const { return mValue & (1 << bit); }

    VINTF_SERIALIZE_FLAGS_FIELD(Hals, 0)
    VINTF_SERIALIZE_FLAGS_FIELD(Avb, 1)
    VINTF_SERIALIZE_FLAGS_FIELD(Sepolicy, 2)
    VINTF_SERIALIZE_FLAGS_FIELD(Vndk, 3)
    VINTF_SERIALIZE_FLAGS_FIELD(Kernel, 4)
    VINTF_SERIALIZE_FLAGS_FIELD(XmlFiles, 5)
    VINTF_SERIALIZE_FLAGS_FIELD(Ssdk, 6)
    VINTF_SERIALIZE_FLAGS_FIELD(Fqname, 7)
    VINTF_SERIALIZE_FLAGS_FIELD(KernelConfigs, 8)
    VINTF_SERIALIZE_FLAGS_FIELD(KernelMinorRevision, 9)
    VINTF_SERIALIZE_FLAGS_FIELD(MetaVersion, 10)
    VINTF_SERIALIZE_FLAGS_FIELD(SchemaType, 11)

#undef VINTF_SERIALIZE_FLAGS_FIELD

   private:
    uint32_t mValue;
};

constexpr Type EVERYTHING = Type(~0);
constexpr Type NO_HALS = EVERYTHING.disableHals();
constexpr Type NO_AVB = EVERYTHING.disableAvb();
constexpr Type NO_SEPOLICY = EVERYTHING.disableSepolicy();
constexpr Type NO_VNDK = EVERYTHING.disableVndk();
constexpr Type NO_KERNEL = EVERYTHING.disableKernel();
constexpr Type NO_XMLFILES = EVERYTHING.disableXmlFiles();
constexpr Type NO_SSDK = EVERYTHING.disableSsdk();
constexpr Type NO_FQNAME = EVERYTHING.disableFqname();
constexpr Type NO_KERNEL_CONFIGS = EVERYTHING.disableKernelConfigs();
constexpr Type NO_KERNEL_MINOR_REVISION = EVERYTHING.disableKernelMinorRevision();

constexpr Type NO_TAGS = Type(0).enableMetaVersion().enableSchemaType();
constexpr Type HALS_ONLY = NO_TAGS.enableHals().enableFqname();  // <hal> with <fqname>
constexpr Type XMLFILES_ONLY = NO_TAGS.enableXmlFiles();
constexpr Type SEPOLICY_ONLY = NO_TAGS.enableSepolicy();
constexpr Type VNDK_ONLY = NO_TAGS.enableVndk();
constexpr Type HALS_NO_FQNAME = NO_TAGS.enableHals();  // <hal> without <fqname>
constexpr Type SSDK_ONLY = NO_TAGS.enableSsdk();

// tests
static_assert(EVERYTHING.isHalsEnabled(), "");
static_assert(EVERYTHING.isMetaVersionEnabled(), "");
static_assert(!NO_HALS.isHalsEnabled(), "");
static_assert(NO_HALS.isAvbEnabled(), "");
static_assert(NO_HALS.isMetaVersionEnabled(), "");
static_assert(!NO_TAGS.isHalsEnabled(), "");
static_assert(NO_TAGS.isMetaVersionEnabled(), "");
static_assert(HALS_ONLY.isHalsEnabled(), "");
static_assert(!HALS_ONLY.isAvbEnabled(), "");
static_assert(HALS_ONLY.isMetaVersionEnabled(), "");

}  // namespace SerializeFlags
}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_SERIALIZE_FLAGS_H
