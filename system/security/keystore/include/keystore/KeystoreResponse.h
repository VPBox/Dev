// Copyright 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef KEYSTORE_INCLUDE_KEYSTORE_RESPONSE_H_
#define KEYSTORE_INCLUDE_KEYSTORE_RESPONSE_H_

#include <binder/Parcel.h>
#include <binder/Parcelable.h>
#include <utils/String8.h>

#include "keystore_return_types.h"

namespace android {
namespace security {
namespace keystore {

// struct for holding response code and optionally an error message for keystore
// AIDL callbacks
struct KeystoreResponse : public ::android::Parcelable {
  public:
    KeystoreResponse() = default;
    explicit KeystoreResponse(const int response_code, const String16& error_msg)
        : response_code_(response_code), error_msg_(std::make_unique<String16>(error_msg)) {}
    explicit KeystoreResponse(const int response_code)
        : response_code_(response_code), error_msg_() {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeystoreResponse(const ::keystore::KeyStoreServiceReturnCode& rc)
        : response_code_(rc.getErrorCode()), error_msg_() {}
    KeystoreResponse(const KeystoreResponse& other)
        : response_code_(other.response_code_), error_msg_() {
        if (other.error_msg_) {
            error_msg_ = std::make_unique<String16>(*other.error_msg_);
        }
    }
    KeystoreResponse(KeystoreResponse&& other) = default;

    status_t readFromParcel(const Parcel* in) override;
    status_t writeToParcel(Parcel* out) const override;

    int response_code() const { return response_code_; }
    const String16* error_msg() const { return error_msg_.get(); }

  private:
    int response_code_;
    std::unique_ptr<String16> error_msg_;
};

}  // namespace keystore
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_RESPONSE_H_
