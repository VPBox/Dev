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

#ifndef ANDROID_VOLD_EMULATED_VOLUME_H
#define ANDROID_VOLD_EMULATED_VOLUME_H

#include "VolumeBase.h"

#include <cutils/multiuser.h>

namespace android {
namespace vold {

/*
 * Shared storage emulated on top of private storage.
 *
 * Knows how to spawn a FUSE daemon to synthesize permissions.  ObbVolume
 * can be stacked above it.
 *
 * This volume is always multi-user aware, but is only binds itself to
 * users when its primary storage.  This volume should never be presented
 * as secondary storage, since we're strongly encouraging developers to
 * store data local to their app.
 */
class EmulatedVolume : public VolumeBase {
  public:
    explicit EmulatedVolume(const std::string& rawPath);
    EmulatedVolume(const std::string& rawPath, dev_t device, const std::string& fsUuid);
    virtual ~EmulatedVolume();

  protected:
    status_t doMount() override;
    status_t doUnmount() override;

  private:
    std::string mRawPath;
    std::string mLabel;

    std::string mFuseDefault;
    std::string mFuseRead;
    std::string mFuseWrite;
    std::string mFuseFull;

    /* PID of FUSE wrapper */
    pid_t mFusePid;

    DISALLOW_COPY_AND_ASSIGN(EmulatedVolume);
};

}  // namespace vold
}  // namespace android

#endif
