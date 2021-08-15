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

#include <general_test/nanoapp_info_events_test_performer.h>

#include <general_test/nanoapp_info.h>

#include <shared/nano_endian.h>

namespace general_test {

NanoAppInfoEventsTestPerformer::NanoAppInfoEventsTestPerformer()
    : Test(CHRE_API_VERSION_1_1) {
}

void NanoAppInfoEventsTestPerformer::setUp(uint32_t /* messageSize */,
                                           const void * /* message */) {
  NanoappInfo info;
  info.sendToHost();
}

void NanoAppInfoEventsTestPerformer::handleEvent(uint32_t senderInstanceId,
                                                 uint16_t eventType,
                                                 const void *eventdata) {
  // Do nothing
}

} // namespace general_test
