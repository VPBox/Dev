/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>

#include <gtest/gtest.h>

#include "wificond/looper_backed_event_loop.h"
#include "wificond/net/netlink_manager.h"

namespace android {
namespace wificond {

class NetlinkManagerTest : public ::testing::Test {
 protected:
  std::unique_ptr<LooperBackedEventLoop> event_loop_;

  virtual void SetUp() {
    event_loop_.reset(new LooperBackedEventLoop());
  }
};

TEST_F(NetlinkManagerTest, CanSetupNetlinkManagerInARowTest) {
  NetlinkManager netlink_manager(event_loop_.get());
  EXPECT_TRUE(netlink_manager.Start());
  EXPECT_TRUE(netlink_manager.Start());
  EXPECT_TRUE(netlink_manager.Start());
  EXPECT_TRUE(netlink_manager.Start());
}

}  // namespace wificond
}  // namespace android
