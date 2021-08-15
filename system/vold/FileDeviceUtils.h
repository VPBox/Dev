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

#ifndef ANDROID_VOLD_FILEDEVICEUTILS_H
#define ANDROID_VOLD_FILEDEVICEUTILS_H

#include <linux/fiemap.h>
#include <string>

namespace android {
namespace vold {

// Given a file path, look for the corresponding block device in /proc/mount
std::string BlockDeviceForPath(const std::string& path);

// Read the file's FIEMAP
std::unique_ptr<struct fiemap> PathFiemap(const std::string& path, uint32_t extent_count);

}  // namespace vold
}  // namespace android

#endif
