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

#ifndef WIFICOND_TESTS_MOCK_I_SEND_MGMT_FRAME_EVENT_H_
#define WIFICOND_TESTS_MOCK_I_SEND_MGMT_FRAME_EVENT_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "android/net/wifi/ISendMgmtFrameEvent.h"

namespace android {
namespace wificond {

class MockISendMgmtFrameEvent
    : public ::android::net::wifi::ISendMgmtFrameEvent {
 public:
  virtual ~MockISendMgmtFrameEvent() override = default;

  MOCK_METHOD0(onAsBinder, ::android::IBinder*());
  MOCK_METHOD1(OnAck, ::android::binder::Status(int elapsed_time_ms));
  MOCK_METHOD1(OnFailure, ::android::binder::Status(int reason));
};

}
}

#endif // WIFICOND_TESTS_MOCK_I_SEND_MGMT_FRAME_EVENT_H_
