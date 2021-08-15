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

#ifndef AIDL_TESTS_SIMPLE_PARCELABLE_H_
#define AIDL_TESTS_SIMPLE_PARCELABLE_H_

#include <cstdint>
#include <string>

#include <binder/Parcelable.h>
#include <utils/Errors.h>
#include <utils/String16.h>

namespace android {
namespace aidl {
namespace tests {

class SimpleParcelable : public Parcelable {
 public:
  SimpleParcelable() = default;
  SimpleParcelable(const std::string& name, int32_t number);
  virtual ~SimpleParcelable() = default;

  // Write |this| parcelable to the given |parcel|.  Keep in mind that
  // implementations of writeToParcel must be manually kept in sync
  // with readFromParcel and the Java equivalent versions of these methods.
  //
  // Returns android::OK on success and an appropriate error otherwise.
  status_t writeToParcel(Parcel* parcel) const override;

  // Read data from the given |parcel| into |this|.  After readFromParcel
  // completes, |this| should have equivalent state to the object that
  // wrote itself to the parcel.
  //
  // Returns android::OK on success and an appropriate error otherwise.
  status_t readFromParcel(const Parcel* parcel) override;

  std::string toString() const;

  friend bool operator==(const SimpleParcelable& lhs,
                         const SimpleParcelable& rhs) {
    return (lhs.name_ == rhs.name_) && (lhs.number_ == rhs.number_);
  }
  friend bool operator!=(const SimpleParcelable& lhs,
                         const SimpleParcelable& rhs) {
    return !(lhs == rhs);
  }

 private:
  String16 name_;
  int32_t number_ = 0;
};  // class SimpleParcelable

}  // namespace tests
}  // namespace aidl
}  // namespace android

#endif  // AIDL_TESTS_SIMPLE_PARCELABLE_H_
