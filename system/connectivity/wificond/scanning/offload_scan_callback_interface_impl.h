/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef WIFICOND_SCANNER_OFFLOAD_SCAN_CALLBACK_INTERFACE_IML_H_
#define WIFICOND_SCANNER_OFFLOAD_SCAN_CALLBACK_INTERFACE_IML_H_

#include "wificond/scanning/offload_scan_callback_interface.h"
#include "wificond/scanning/scanner_impl.h"

namespace android {
namespace wificond {

class ScannerImpl;

class OffloadScanCallbackInterfaceImpl
    : public android::wificond::OffloadScanCallbackInterface {
 public:
  OffloadScanCallbackInterfaceImpl(ScannerImpl* parent);
  ~OffloadScanCallbackInterfaceImpl() override;

  void OnOffloadScanResult() override;
  void OnOffloadError(OffloadScanCallbackInterface::AsyncErrorReason) override;

 private:
  ScannerImpl* scanner_impl_;
};

}  // namespace wificond
}  // namespace android

#endif  //  WIFICOND_SCANNER_OFFLOAD_SCAN_CALLBACK_INTERFACE_IML_H_
