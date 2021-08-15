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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYATTESTATIONPACKAGEINFO_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYATTESTATIONPACKAGEINFO_H_

#include <stdint.h>

#include <memory>
#include <vector>

#include <binder/Parcelable.h>

#include "Signature.h"
#include "utils.h"

namespace android {
namespace security {
namespace keymaster {

class KeyAttestationPackageInfo : public Parcelable {
  public:
    typedef SharedNullableIterator<const content::pm::Signature, std::vector>
        ConstSignatureIterator;
    typedef std::vector<std::unique_ptr<content::pm::Signature>>
        SignaturesVector;
    typedef std::shared_ptr<SignaturesVector> SharedSignaturesVector;

    KeyAttestationPackageInfo(const String16& packageName, int64_t versionCode,
                              SharedSignaturesVector signatures);
    KeyAttestationPackageInfo();

    status_t writeToParcel(Parcel*) const override;
    status_t readFromParcel(const Parcel* parcel) override;

    const std::unique_ptr<String16>& package_name() const { return packageName_; }
    int64_t version_code() const { return versionCode_; }

    ConstSignatureIterator sigs_begin() const { return ConstSignatureIterator(signatures_); }
    ConstSignatureIterator sigs_end() const { return ConstSignatureIterator(); }

  private:
    std::unique_ptr<String16> packageName_;
    int64_t versionCode_;
    SharedSignaturesVector signatures_;
};

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYATTESTATIONPACKAGEINFO_H_
