/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef ANDROID_VOLD_VOLUME_BASE_H
#define ANDROID_VOLD_VOLUME_BASE_H

#include "Utils.h"
#include "android/os/IVoldListener.h"

#include <cutils/multiuser.h>
#include <utils/Errors.h>

#include <sys/types.h>
#include <list>
#include <string>

static constexpr userid_t USER_UNKNOWN = ((userid_t)-1);

namespace android {
namespace vold {

/*
 * Representation of a mounted volume ready for presentation.
 *
 * Various subclasses handle the different mounting prerequisites, such as
 * encryption details, etc.  Volumes can also be "stacked" above other
 * volumes to help communicate dependencies.  For example, an ASEC volume
 * can be stacked on a vfat volume.
 *
 * Mounted volumes can be asked to manage bind mounts to present themselves
 * to specific users on the device.
 *
 * When an unmount is requested, the volume recursively unmounts any stacked
 * volumes and removes any bind mounts before finally unmounting itself.
 */
class VolumeBase {
  public:
    virtual ~VolumeBase();

    enum class Type {
        kPublic = 0,
        kPrivate,
        kEmulated,
        kAsec,
        kObb,
        kStub,
    };

    enum MountFlags {
        /* Flag that volume is primary external storage */
        kPrimary = 1 << 0,
        /* Flag that volume is visible to normal apps */
        kVisible = 1 << 1,
    };

    enum class State {
        kUnmounted = 0,
        kChecking,
        kMounted,
        kMountedReadOnly,
        kFormatting,
        kEjecting,
        kUnmountable,
        kRemoved,
        kBadRemoval,
    };

    const std::string& getId() const { return mId; }
    const std::string& getDiskId() const { return mDiskId; }
    const std::string& getPartGuid() const { return mPartGuid; }
    Type getType() const { return mType; }
    int getMountFlags() const { return mMountFlags; }
    userid_t getMountUserId() const { return mMountUserId; }
    State getState() const { return mState; }
    const std::string& getPath() const { return mPath; }
    const std::string& getInternalPath() const { return mInternalPath; }

    status_t setDiskId(const std::string& diskId);
    status_t setPartGuid(const std::string& partGuid);
    status_t setMountFlags(int mountFlags);
    status_t setMountUserId(userid_t mountUserId);
    status_t setSilent(bool silent);

    void addVolume(const std::shared_ptr<VolumeBase>& volume);
    void removeVolume(const std::shared_ptr<VolumeBase>& volume);

    std::shared_ptr<VolumeBase> findVolume(const std::string& id);

    bool isEmulated() { return mType == Type::kEmulated; }

    status_t create();
    status_t destroy();
    status_t mount();
    status_t unmount();
    status_t format(const std::string& fsType);

    std::ostream& operator<<(std::ostream& stream) const;

  protected:
    explicit VolumeBase(Type type);

    virtual status_t doCreate();
    virtual status_t doDestroy();
    virtual status_t doMount() = 0;
    virtual status_t doUnmount() = 0;
    virtual status_t doFormat(const std::string& fsType);

    status_t setId(const std::string& id);
    status_t setPath(const std::string& path);
    status_t setInternalPath(const std::string& internalPath);

    android::sp<android::os::IVoldListener> getListener() const;

  private:
    /* ID that uniquely references volume while alive */
    std::string mId;
    /* ID that uniquely references parent disk while alive */
    std::string mDiskId;
    /* Partition GUID of this volume */
    std::string mPartGuid;
    /* Volume type */
    Type mType;
    /* Flags used when mounting this volume */
    int mMountFlags;
    /* User that owns this volume, otherwise -1 */
    userid_t mMountUserId;
    /* Flag indicating object is created */
    bool mCreated;
    /* Current state of volume */
    State mState;
    /* Path to mounted volume */
    std::string mPath;
    /* Path to internal backing storage */
    std::string mInternalPath;
    /* Flag indicating that volume should emit no events */
    bool mSilent;

    /* Volumes stacked on top of this volume */
    std::list<std::shared_ptr<VolumeBase>> mVolumes;

    void setState(State state);

    DISALLOW_COPY_AND_ASSIGN(VolumeBase);
};

}  // namespace vold
}  // namespace android

#endif
