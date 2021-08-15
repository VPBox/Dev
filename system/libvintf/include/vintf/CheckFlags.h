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

#ifndef ANDROID_VINTF_DISABLED_CHECKS_H_
#define ANDROID_VINTF_DISABLED_CHECKS_H_

namespace android {
namespace vintf {

namespace CheckFlags {

// Flags for *::checkCompatibility functions.
class Type {
   public:
#define VINTF_CHECK_FLAGS_FIELD(name, bit)                                                    \
    [[nodiscard]] constexpr Type enable##name() const { return Type(mValue | (1 << bit)); }   \
    [[nodiscard]] constexpr Type disable##name() const { return Type(mValue & ~(1 << bit)); } \
    constexpr bool is##name##Enabled() const { return mValue & (1 << bit); }

    VINTF_CHECK_FLAGS_FIELD(Avb, 0)
    VINTF_CHECK_FLAGS_FIELD(RuntimeInfo, 1)
    VINTF_CHECK_FLAGS_FIELD(Kernel, 2)
#undef VINTF_CHECK_FLAGS_FIELD

    explicit constexpr Type(int32_t value) : mValue(value) {}

   private:
    int32_t mValue;
};

constexpr Type ENABLE_ALL_CHECKS{~0};
constexpr Type DISABLE_ALL_CHECKS{0};
// Disable AVB version check in RuntimeInfo::checkCompatibility
constexpr Type DISABLE_AVB_CHECK = ENABLE_ALL_CHECKS.disableAvb();
// Disable RuntimeInfo <-> Framework Matrix check. This implies DISABLE_AVB_CHECK.
constexpr Type DISABLE_RUNTIME_INFO = ENABLE_ALL_CHECKS.disableRuntimeInfo();

// Default flag if no flag is provided.
constexpr Type DEFAULT = DISABLE_AVB_CHECK;

// tests
static_assert(ENABLE_ALL_CHECKS.isAvbEnabled(), "");
static_assert(ENABLE_ALL_CHECKS.isRuntimeInfoEnabled(), "");
static_assert(!DISABLE_AVB_CHECK.isAvbEnabled(), "");
static_assert(DISABLE_AVB_CHECK.isRuntimeInfoEnabled(), "");
static_assert(DISABLE_RUNTIME_INFO.isAvbEnabled(), "");
static_assert(!DISABLE_RUNTIME_INFO.isRuntimeInfoEnabled(), "");

}  // namespace CheckFlags
}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_DISABLED_CHECKS_H_
