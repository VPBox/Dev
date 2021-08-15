// Copyright 2016 The Android Open Source Project
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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_SIGNATURE_H_
#define KEYSTORE_INCLUDE_KEYSTORE_SIGNATURE_H_

#include <vector>

#include <binder/Parcelable.h>

namespace android {
namespace content {
namespace pm {

class Signature : public Parcelable {
  public:
    Signature() = default;
    // Intended for initializing instances containing test data.
    explicit Signature(std::vector<uint8_t> signature_data);

    status_t writeToParcel(Parcel*) const override;
    status_t readFromParcel(const Parcel* parcel) override;

    const std::vector<uint8_t>& data() const & { return sig_data_; }
    std::vector<uint8_t>& data() & { return sig_data_; }
    std::vector<uint8_t>&& data() && { return std::move(sig_data_); }

  private:
    std::vector<uint8_t> sig_data_;
};

}  // namespace pm
}  // namespace content
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_SIGNATURE_H_
