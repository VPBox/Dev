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

#include "wificond/scanning/offload_scan_callback_interface_impl.h"

#include <android-base/logging.h>


namespace android {
namespace wificond {

OffloadScanCallbackInterfaceImpl::OffloadScanCallbackInterfaceImpl(
    ScannerImpl* parent)
    : scanner_impl_(parent) {}

OffloadScanCallbackInterfaceImpl::~OffloadScanCallbackInterfaceImpl() {}

void OffloadScanCallbackInterfaceImpl::OnOffloadScanResult() {
  scanner_impl_->OnOffloadScanResult();
}

void OffloadScanCallbackInterfaceImpl::OnOffloadError(
    OffloadScanCallbackInterface::AsyncErrorReason error_code) {
  scanner_impl_->OnOffloadError(error_code);
}

}  // namespace wificond
}  // namespace android
