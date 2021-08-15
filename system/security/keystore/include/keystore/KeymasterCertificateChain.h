// Copyright 2017 The Android Open Source Project
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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYMASTERCERTIFICATECHAIN_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYMASTERCERTIFICATECHAIN_H_

#include <binder/Parcelable.h>
#include <keystore/keymaster_types.h>

namespace android {
namespace security {
namespace keymaster {

// struct for serializing keymaster_cert_chain_t's
struct KeymasterCertificateChain : public ::android::Parcelable {
    KeymasterCertificateChain(){};
    explicit KeymasterCertificateChain(hardware::hidl_vec<hardware::hidl_vec<uint8_t>> other)
        : chain(std::move(other)) {}

    status_t readFromParcel(const Parcel* in) override;
    status_t writeToParcel(Parcel* out) const override;

  private:
    // The structure is only used as output and doesn't have getter.
    hardware::hidl_vec<hardware::hidl_vec<uint8_t>> chain;
};

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYMASTERCERTIFICATECHAIN_H_
