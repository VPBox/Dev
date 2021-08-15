/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "tests/simple_parcelable.h"

#include <android-base/stringprintf.h>
#include <binder/Parcel.h>
#include <utils/String8.h>

using android::base::StringPrintf;

namespace android {
namespace aidl {
namespace tests {

SimpleParcelable::SimpleParcelable(const std::string& name, int32_t number)
    : name_(name.c_str(), name.length()),
      number_(number) {}

status_t SimpleParcelable::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeString16(name_);
  if (status != OK) {
    return status;
  }
  status = parcel->writeInt32(number_);
  return status;
}

status_t SimpleParcelable::readFromParcel(const Parcel* parcel) {
  status_t status = parcel->readString16(&name_);
  if (status != OK) {
    return status;
  }
  return parcel->readInt32(&number_);
}

std::string SimpleParcelable::toString() const {
    return StringPrintf("%s(%d)", String8(name_).string(), number_);
}

}  // namespace tests
}  // namespace aidl
}  // namespace android
