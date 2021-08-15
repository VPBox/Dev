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

#include "StubVolume.h"

#include <android-base/logging.h>
#include <android-base/stringprintf.h>

using android::base::StringPrintf;

namespace android {
namespace vold {

StubVolume::StubVolume(int id, const std::string& sourcePath, const std::string& mountPath,
                       const std::string& fsType, const std::string& fsUuid,
                       const std::string& fsLabel)
    : VolumeBase(Type::kStub),
      mSourcePath(sourcePath),
      mMountPath(mountPath),
      mFsType(fsType),
      mFsUuid(fsUuid),
      mFsLabel(fsLabel) {
    setId(StringPrintf("stub:%d", id));
}

StubVolume::~StubVolume() {}

status_t StubVolume::doCreate() {
    return OK;
}

status_t StubVolume::doDestroy() {
    return OK;
}

status_t StubVolume::doMount() {
    auto listener = getListener();
    if (listener) listener->onVolumeMetadataChanged(getId(), mFsType, mFsUuid, mFsLabel);
    setInternalPath(mSourcePath);
    setPath(mMountPath);
    return OK;
}

status_t StubVolume::doUnmount() {
    return OK;
}

// TODO: return error instead.
status_t StubVolume::doFormat(const std::string& fsType) {
    return OK;
}

}  // namespace vold
}  // namespace android
