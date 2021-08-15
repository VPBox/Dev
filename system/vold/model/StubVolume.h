/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef ANDROID_VOLD_STUB_VOLUME_H
#define ANDROID_VOLD_STUB_VOLUME_H

#include "VolumeBase.h"

namespace android {
namespace vold {

/*
 * A vold representation of volumes managed from outside Android (e.g., ARC++).
 *
 * Used for the case when events such that mounting and unmounting are
 * actually handled from outside vold, and vold only need to keep track on those
 * vents instead of talking to kernel directly.
 */
class StubVolume : public VolumeBase {
  public:
    StubVolume(int id, const std::string& sourcePath, const std::string& mountPath,
               const std::string& fsType, const std::string& fsUuid, const std::string& fsLabel);
    virtual ~StubVolume();

  protected:
    status_t doCreate() override;
    status_t doDestroy() override;
    status_t doMount() override;
    status_t doUnmount() override;
    status_t doFormat(const std::string& fsType) override;

  private:
    const std::string mSourcePath;
    const std::string mMountPath;
    const std::string mFsType;
    const std::string mFsUuid;
    const std::string mFsLabel;

    DISALLOW_COPY_AND_ASSIGN(StubVolume);
};

}  // namespace vold
}  // namespace android

#endif
