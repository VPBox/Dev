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

#ifndef ANDROID_VOLD_PRIVATE_VOLUME_H
#define ANDROID_VOLD_PRIVATE_VOLUME_H

#include "VolumeBase.h"

#include <cutils/multiuser.h>

namespace android {
namespace vold {

/*
 * Private storage provided by an encrypted partition.
 *
 * Given a raw block device, it knows how to wrap it in dm-crypt and
 * format as ext4/f2fs.  EmulatedVolume can be stacked above it.
 *
 * This volume is designed to behave much like the internal /data
 * partition, both in layout and function.  For example, apps and
 * private app data can be safely stored on this volume because the
 * keys are tightly tied to this device.
 */
class PrivateVolume : public VolumeBase {
  public:
    PrivateVolume(dev_t device, const std::string& keyRaw);
    virtual ~PrivateVolume();
    const std::string& getFsType() const { return mFsType; };
    const std::string& getRawDevPath() const { return mRawDevPath; };
    const std::string& getRawDmDevPath() const { return mDmDevPath; };

  protected:
    status_t doCreate() override;
    status_t doDestroy() override;
    status_t doMount() override;
    status_t doUnmount() override;
    status_t doFormat(const std::string& fsType) override;

    status_t readMetadata();

  private:
    /* Kernel device of raw, encrypted partition */
    dev_t mRawDevice;
    /* Path to raw, encrypted block device */
    std::string mRawDevPath;
    /* Path to decrypted block device */
    std::string mDmDevPath;
    /* Path where decrypted device is mounted */
    std::string mPath;

    /* Encryption key as raw bytes */
    std::string mKeyRaw;

    /* Filesystem type */
    std::string mFsType;
    /* Filesystem UUID */
    std::string mFsUuid;
    /* User-visible filesystem label */
    std::string mFsLabel;

    DISALLOW_COPY_AND_ASSIGN(PrivateVolume);
};

}  // namespace vold
}  // namespace android

#endif
