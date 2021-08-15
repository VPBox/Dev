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

#include <general_test/nanoapp_info_by_instance_id_test.h>

#include <general_test/running_info.h>

#include <shared/send_message.h>
#include <shared/nano_endian.h>
#include <shared/nano_string.h>

#include <chre.h>

namespace general_test {

NanoappInfoByInstanceIdTest::NanoappInfoByInstanceIdTest()
    : Test(CHRE_API_VERSION_1_1) {
}

void NanoappInfoByInstanceIdTest::setUp(uint32_t /* messageSize */,
                                        const void * /* message */) {
  nanoapp_testing::sendMessageToHost(nanoapp_testing::MessageType::kContinue);
}

void NanoappInfoByInstanceIdTest::handleEvent(uint32_t senderInstanceId,
                                              uint16_t eventType,
                                              const void *eventData) {
  if (senderInstanceId == CHRE_INSTANCE_ID) {
    uint32_t appVersion;
    const void *message =
        getMessageDataFromHostEvent(senderInstanceId, eventType,
                                    eventData,
                                    nanoapp_testing::MessageType::kContinue,
                                    sizeof(&appVersion));

    nanoapp_testing::memcpy(&appVersion, message, sizeof(appVersion));
    appVersion = nanoapp_testing::littleEndianToHost(appVersion);

    RunningInfo runningInfo;

    if (runningInfo.queryByInstanceId() && runningInfo.validate(appVersion)) {
      nanoapp_testing::sendSuccessToHost();
    }
  }
}

} // namespace general_test
