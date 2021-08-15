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

#ifndef ANDROID_VOLD_OBB_VOLUME_H
#define ANDROID_VOLD_OBB_VOLUME_H

#include "VolumeBase.h"

#include <cutils/multiuser.h>

namespace android {
namespace vold {

/*
 * OBB container.
 */
class ObbVolume : public VolumeBase {
  public:
    ObbVolume(int id, const std::string& sourcePath, const std::string& sourceKey, gid_t ownerGid);
    virtual ~ObbVolume();

  protected:
    status_t doCreate() override;
    status_t doDestroy() override;
    status_t doMount() override;
    status_t doUnmount() override;

  private:
    std::string mSourcePath;
    std::string mSourceKey;
    gid_t mOwnerGid;

    std::string mLoopPath;
    std::string mDmPath;
    std::string mMountPath;

    DISALLOW_COPY_AND_ASSIGN(ObbVolume);
};

}  // namespace vold
}  // namespace android

#endif
