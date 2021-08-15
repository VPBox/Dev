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

#ifndef WIFICOND_TESTS_MOCK_OFFLOAD_SCAN_CALLBACK_INTERFACE_IMPL_H__
#define WIFICOND_TESTS_MOCK_OFFLOAD_SCAN_CALLBACK_INTERFACE_IMPL_H__

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wificond/scanning/offload_scan_callback_interface_impl.h"

namespace android {
namespace wificond {

class ScannerImpl;

class MockOffloadScanCallbackInterfaceImpl
    : public OffloadScanCallbackInterfaceImpl {
 public:
  MockOffloadScanCallbackInterfaceImpl(ScannerImpl*);
  ~MockOffloadScanCallbackInterfaceImpl() override = default;

  MOCK_METHOD0(OnOffloadScanResult, void());
  MOCK_METHOD1(OnOffloadError,
               void(OffloadScanCallbackInterface::AsyncErrorReason));
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TESTS_MOCK_OFFLOAD_SCAN_CALLBACK_INTERFACE_IMPL_H__
