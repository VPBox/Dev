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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYATTESTATIONAPPLICATIONID_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYATTESTATIONAPPLICATIONID_H_

#include <memory>
#include <vector>

#include <binder/Parcelable.h>

#include "KeyAttestationPackageInfo.h"

namespace android {
namespace security {
namespace keymaster {

class KeyAttestationApplicationId : public Parcelable {
  public:
    typedef SharedNullableIterator<const KeyAttestationPackageInfo, std::vector>
        ConstKeyAttestationPackageInfoIterator;
    typedef std::vector<std::unique_ptr<KeyAttestationPackageInfo>> PackageInfoVector;
    KeyAttestationApplicationId();
    // Following c'tors are for initializing instances containing test data.
    explicit KeyAttestationApplicationId(std::unique_ptr<KeyAttestationPackageInfo> package);
    explicit KeyAttestationApplicationId(PackageInfoVector packages);

    status_t writeToParcel(Parcel*) const override;
    status_t readFromParcel(const Parcel* parcel) override;

    ConstKeyAttestationPackageInfoIterator pinfos_begin() const {
        return ConstKeyAttestationPackageInfoIterator(packageInfos_);
    }
    ConstKeyAttestationPackageInfoIterator pinfos_end() const {
        return ConstKeyAttestationPackageInfoIterator();
    }

  private:
    std::shared_ptr<std::vector<std::unique_ptr<KeyAttestationPackageInfo>>> packageInfos_;
};

}  // namespace keymaster
}  // namespace security
}  // namespace android

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYATTESTATIONAPPLICATIONID_H_
