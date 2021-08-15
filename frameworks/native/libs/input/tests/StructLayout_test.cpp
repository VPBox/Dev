/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <input/InputTransport.h>
#include <input/Input.h>

namespace android {

#define CHECK_OFFSET(type, member, expected_offset) \
  static_assert((offsetof(type, member) == (expected_offset)), "")

struct Foo {
  uint32_t dummy;
  PointerCoords coords;
};

void TestPointerCoordsAlignment() {
  CHECK_OFFSET(Foo, coords, 8);
}

void TestInputMessageAlignment() {
  CHECK_OFFSET(InputMessage, body, 8);

  CHECK_OFFSET(InputMessage::Body::Key, seq, 0);
  CHECK_OFFSET(InputMessage::Body::Key, eventTime, 8);
  CHECK_OFFSET(InputMessage::Body::Key, deviceId, 16);
  CHECK_OFFSET(InputMessage::Body::Key, source, 20);
  CHECK_OFFSET(InputMessage::Body::Key, displayId, 24);
  CHECK_OFFSET(InputMessage::Body::Key, action, 28);
  CHECK_OFFSET(InputMessage::Body::Key, flags, 32);
  CHECK_OFFSET(InputMessage::Body::Key, keyCode, 36);
  CHECK_OFFSET(InputMessage::Body::Key, scanCode, 40);
  CHECK_OFFSET(InputMessage::Body::Key, metaState, 44);
  CHECK_OFFSET(InputMessage::Body::Key, repeatCount, 48);
  CHECK_OFFSET(InputMessage::Body::Key, downTime, 56);

  CHECK_OFFSET(InputMessage::Body::Motion, seq, 0);
  CHECK_OFFSET(InputMessage::Body::Motion, eventTime, 8);
  CHECK_OFFSET(InputMessage::Body::Motion, deviceId, 16);
  CHECK_OFFSET(InputMessage::Body::Motion, source, 20);
  CHECK_OFFSET(InputMessage::Body::Motion, displayId, 24);
  CHECK_OFFSET(InputMessage::Body::Motion, action, 28);
  CHECK_OFFSET(InputMessage::Body::Motion, actionButton, 32);
  CHECK_OFFSET(InputMessage::Body::Motion, flags, 36);
  CHECK_OFFSET(InputMessage::Body::Motion, metaState, 40);
  CHECK_OFFSET(InputMessage::Body::Motion, buttonState, 44);
  CHECK_OFFSET(InputMessage::Body::Motion, classification, 48);
  CHECK_OFFSET(InputMessage::Body::Motion, edgeFlags, 52);
  CHECK_OFFSET(InputMessage::Body::Motion, downTime, 56);
  CHECK_OFFSET(InputMessage::Body::Motion, xOffset, 64);
  CHECK_OFFSET(InputMessage::Body::Motion, yOffset, 68);
  CHECK_OFFSET(InputMessage::Body::Motion, xPrecision, 72);
  CHECK_OFFSET(InputMessage::Body::Motion, yPrecision, 76);
  CHECK_OFFSET(InputMessage::Body::Motion, pointerCount, 80);
  CHECK_OFFSET(InputMessage::Body::Motion, pointers, 88);

  CHECK_OFFSET(InputMessage::Body::Finished, seq, 0);
  CHECK_OFFSET(InputMessage::Body::Finished, handled, 4);
}

} // namespace android
