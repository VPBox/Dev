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
#ifndef ANDROID_WIFICOND_OFFLOAD_SCAN_CALLBACK_INTERFACE_H
#define ANDROID_WIFICOND_OFFLOAD_SCAN_CALLBACK_INTERFACE_H

#include <vector>

#include "wificond/scanning/scan_result.h"

namespace android {
namespace wificond {

class OffloadScanCallbackInterface {
 public:
  enum AsyncErrorReason {
    BINDER_DEATH = 0,
    REMOTE_FAILURE,
  };

  virtual ~OffloadScanCallbackInterface() {}

  virtual void OnOffloadScanResult() = 0;
  virtual void OnOffloadError(AsyncErrorReason) = 0;
};

}  // namespace wificond
}  // namespace android

#endif  // ANDROID_WIFICOND_OFFLOAD_SCAN_CALLBACK_INTERFACE_H
