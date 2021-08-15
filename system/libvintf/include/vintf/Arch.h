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


#ifndef ANDROID_VINTF_ARCH_H
#define ANDROID_VINTF_ARCH_H

#include <stdint.h>
#include <string>
#include <array>

namespace android {
namespace vintf {

enum class Arch : size_t {
    ARCH_EMPTY = 0,
    ARCH_32,
    ARCH_64,
    ARCH_32_64
};

static const std::array<std::string, 4> gArchStrings = {
    {
        "",
        "32",
        "64",
        "32+64",
    }
};

inline bool has32(Arch arch) {
    return arch == Arch::ARCH_32 || arch == Arch::ARCH_32_64;
}

inline bool has64(Arch arch) {
    return arch == Arch::ARCH_64 || arch == Arch::ARCH_32_64;
}

inline constexpr Arch operator|(Arch lft, Arch rgt) {
    return static_cast<Arch>(static_cast<size_t>(lft) | static_cast<size_t>(rgt));
}
static_assert((Arch::ARCH_32 | Arch::ARCH_64) == Arch::ARCH_32_64, "bad Arch::operator|");

inline Arch& operator|=(Arch& lft, Arch rgt) {
    return (lft = lft | rgt);
}

// Returns true if lft defines all bitnesses in rgt, otherwise false.
inline constexpr bool contains(Arch lft, Arch rgt) {
    return !(~static_cast<size_t>(lft) & static_cast<size_t>(rgt));
}

static_assert(contains(Arch::ARCH_32_64, Arch::ARCH_32_64), "bad contains(Arch, Arch)");
static_assert(contains(Arch::ARCH_32_64, Arch::ARCH_64), "bad contains(Arch, Arch)");
static_assert(contains(Arch::ARCH_32_64, Arch::ARCH_32), "bad contains(Arch, Arch)");
static_assert(contains(Arch::ARCH_32_64, Arch::ARCH_EMPTY), "bad contains(Arch, Arch)");
static_assert(contains(Arch::ARCH_32, Arch::ARCH_EMPTY), "bad contains(Arch, Arch)");
static_assert(contains(Arch::ARCH_64, Arch::ARCH_EMPTY), "bad contains(Arch, Arch)");
static_assert(!contains(Arch::ARCH_32, Arch::ARCH_32_64), "bad contains(Arch, Arch)");
static_assert(!contains(Arch::ARCH_64, Arch::ARCH_32_64), "bad contains(Arch, Arch)");
static_assert(!contains(Arch::ARCH_32, Arch::ARCH_64), "bad contains(Arch, Arch)");
static_assert(!contains(Arch::ARCH_64, Arch::ARCH_32), "bad contains(Arch, Arch)");
static_assert(!contains(Arch::ARCH_EMPTY, Arch::ARCH_32), "bad contains(Arch, Arch)");
static_assert(!contains(Arch::ARCH_EMPTY, Arch::ARCH_64), "bad contains(Arch, Arch)");

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_ARCH_H
