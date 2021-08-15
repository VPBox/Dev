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

#ifndef ANDROID_VOLD_APP_FUSE_UTIL_H_
#define ANDROID_VOLD_APP_FUSE_UTIL_H_

#include <android-base/unique_fd.h>

#define DEBUG_APPFUSE 0

namespace android {
namespace vold {

int MountAppFuse(uid_t uid, int mountId, android::base::unique_fd* device_fd);

int UnmountAppFuse(uid_t uid, int mountId);

int OpenAppFuseFile(uid_t uid, int mountId, int fileId, int flags);

}  // namespace vold
}  // namespace android

#endif  // ANDROID_VOLD_APP_FUSE_UTIL_H_
