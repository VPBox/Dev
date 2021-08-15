/*
 * stringCopyright (C) 2017 The Android Open Source Project
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

#include "KernelConfigTypedValue.h"

#include "parse_string.h"

#include <android-base/logging.h>

namespace android {
namespace vintf {

// static
const KernelConfigTypedValue KernelConfigTypedValue::gMissingConfig{Tristate::NO};

KernelConfigTypedValue::KernelConfigTypedValue()
        : KernelConfigTypedValue("") {
}

KernelConfigTypedValue::KernelConfigTypedValue(std::string &&s){
    mType = KernelConfigType::STRING;
    std::swap(mStringValue, s);
}

KernelConfigTypedValue::KernelConfigTypedValue(KernelConfigIntValue v){
    mType = KernelConfigType::INTEGER;
    mIntegerValue = v;
}

KernelConfigTypedValue::KernelConfigTypedValue(KernelConfigRangeValue &&v){
    mType = KernelConfigType::RANGE;
    std::swap(mRangeValue, v);
}

KernelConfigTypedValue::KernelConfigTypedValue(Tristate t){
    mType = KernelConfigType::TRISTATE;
    mTristateValue = t;
}

bool KernelConfigTypedValue::operator==(const KernelConfigTypedValue &other) const {
    if (mType != other.mType) {
        return false;
    }
    switch(mType) {
        case KernelConfigType::STRING:
            return mStringValue == other.mStringValue;
        case KernelConfigType::INTEGER:
            return mIntegerValue == other.mIntegerValue;
        case KernelConfigType::RANGE:
            return mRangeValue == other.mRangeValue;
        case KernelConfigType::TRISTATE:
            return mTristateValue == other.mTristateValue;
    }
}


bool KernelConfigTypedValue::matchValue(const std::string &s) const {
    switch(mType) {
        case KernelConfigType::STRING:
            return ("\"" + mStringValue + "\"") == s;
        case KernelConfigType::INTEGER: {
            KernelConfigIntValue iv;
            return parseKernelConfigInt(s, &iv) && iv == mIntegerValue;
        }
        case KernelConfigType::RANGE: {
            KernelConfigRangeValue range;
            return parseRange(s, &range) && range == mRangeValue;
        }
        case KernelConfigType::TRISTATE: {
            Tristate tristate;
            return parse(s, &tristate) && tristate == mTristateValue;
        }
    }
}

} // namespace vintf
} // namespace android
