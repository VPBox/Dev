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

#ifndef ANDROID_VINTF_KERNEL_CONFIG_H
#define ANDROID_VINTF_KERNEL_CONFIG_H

#include <string>

#include "KernelConfigType.h"
#include "Tristate.h"

namespace android {
namespace vintf {

using KernelConfigIntValue = int64_t;
using KernelConfigRangeValue = std::pair<uint64_t, uint64_t>;

// compatibility-matrix.kernel.config.value item.
struct KernelConfigTypedValue {

    const static KernelConfigTypedValue gMissingConfig;

    // Construct with empty string value
    KernelConfigTypedValue();

    KernelConfigTypedValue(std::string &&s);
    KernelConfigTypedValue(KernelConfigIntValue v);
    KernelConfigTypedValue(KernelConfigRangeValue &&v);
    KernelConfigTypedValue(Tristate t);

    bool operator==(const KernelConfigTypedValue &other) const;

    bool matchValue(const std::string &) const;

private:
    friend struct KernelConfigTypedValueConverter;
    friend std::ostream &operator<<(std::ostream &os, const KernelConfigTypedValue &kctv);
    friend bool parseKernelConfigValue(const std::string &s, KernelConfigTypedValue *kctv);
    friend bool parseKernelConfigTypedValue(const std::string& s, KernelConfigTypedValue* kctv);

    std::string mStringValue;
    KernelConfigIntValue mIntegerValue;
    KernelConfigRangeValue mRangeValue;
    Tristate mTristateValue;
    KernelConfigType mType;

};

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_KERNEL_CONFIG_H
