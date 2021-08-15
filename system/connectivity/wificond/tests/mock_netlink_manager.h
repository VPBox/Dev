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

#ifndef WIFICOND_TEST_MOCK_NETLINK_MANAGER_H_
#define WIFICOND_TEST_MOCK_NETLINK_MANAGER_H_

#include <vector>

#include <gmock/gmock.h>

#include "wificond/net/netlink_manager.h"
#include "wificond/net/nl80211_packet.h"

namespace android {
namespace wificond {

class MockNetlinkManager : public NetlinkManager {
 public:
  MockNetlinkManager();
  ~MockNetlinkManager() override = default;

  MOCK_METHOD0(GetSequenceNumber, uint32_t());
  MOCK_METHOD0(GetFamilyId, uint16_t());
  MOCK_METHOD0(Start, bool());
  MOCK_CONST_METHOD0(IsStarted, bool());
  MOCK_METHOD2(SendMessageAndGetResponses,
      bool(const NL80211Packet&, std::vector<std::unique_ptr<const NL80211Packet>>*));
  MOCK_METHOD2(RegisterHandlerAndSendMessage,
      bool(const NL80211Packet&, std::function<void(std::unique_ptr<const NL80211Packet>)>));
};  // class MockNetlinkManager

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TEST_MOCK_NETLINK_MANAGER_H
