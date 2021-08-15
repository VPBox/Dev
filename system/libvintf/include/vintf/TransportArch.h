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


#ifndef ANDROID_VINTF_TRANSPORT_ARCH_H
#define ANDROID_VINTF_TRANSPORT_ARCH_H

#include "Arch.h"
#include "Transport.h"

namespace android {
namespace vintf {

struct TransportArch {
    Transport transport = Transport::EMPTY;
    Arch arch = Arch::ARCH_EMPTY;

    TransportArch() = default;
    TransportArch(Transport t, Arch a) : transport(t), arch(a) {}

    inline bool operator==(const TransportArch& other) const {
        return transport == other.transport && arch == other.arch;
    }
    inline bool operator<(const TransportArch& other) const {
        if (transport < other.transport) return true;
        if (transport > other.transport) return false;
        return arch < other.arch;
    }

   private:
    friend struct TransportArchConverter;
    friend struct ManifestHalConverter;
    friend struct ManifestHal;
    friend bool parse(const std::string &s, TransportArch *ta);
    bool empty() const;
    // Valid combinations:
    // <transport arch="32">passthrough</transport>
    // <transport arch="64">passthrough</transport>
    // <transport arch="32+64">passthrough</transport>
    // <transport>hwbinder</transport>
    // Element doesn't exist
    bool isValid() const;
};


} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_TRANSPORT_ARCH_H
