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

#ifndef ANDROID_APEXD_APEXD_LOOP_H_
#define ANDROID_APEXD_APEXD_LOOP_H_

#include "status_or.h"

#include <android-base/unique_fd.h>

#include <functional>
#include <string>

namespace android {
namespace apex {
namespace loop {

using android::base::unique_fd;

struct LoopbackDeviceUniqueFd {
  unique_fd device_fd;
  std::string name;

  LoopbackDeviceUniqueFd() {}
  LoopbackDeviceUniqueFd(unique_fd&& fd, const std::string& name)
      : device_fd(std::move(fd)), name(name) {}

  LoopbackDeviceUniqueFd(LoopbackDeviceUniqueFd&& fd) noexcept
      : device_fd(std::move(fd.device_fd)), name(fd.name) {}
  LoopbackDeviceUniqueFd& operator=(LoopbackDeviceUniqueFd&& other) noexcept {
    MaybeCloseBad();
    device_fd = std::move(other.device_fd);
    name = std::move(other.name);
    return *this;
  }

  ~LoopbackDeviceUniqueFd() { MaybeCloseBad(); }

  void MaybeCloseBad();

  void CloseGood() { device_fd.reset(-1); }

  int get() { return device_fd.get(); }
};

Status configureReadAhead(const std::string& device_path);

Status preAllocateLoopDevices(size_t num);

StatusOr<LoopbackDeviceUniqueFd> createLoopDevice(const std::string& target,
                                                  const int32_t imageOffset,
                                                  const size_t imageSize);

using DestroyLoopFn =
    std::function<void(const std::string&, const std::string&)>;
void DestroyLoopDevice(const std::string& path, const DestroyLoopFn& extra);

}  // namespace loop
}  // namespace apex
}  // namespace android

#endif  // ANDROID_APEXD_APEXD_LOOP_H_
