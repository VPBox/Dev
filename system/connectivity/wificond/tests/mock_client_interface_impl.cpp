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

#include "wificond/tests/mock_client_interface_impl.h"

#include <vector>

#include <linux/if_ether.h>

#include <wifi_system/interface_tool.h>

#include "wificond/net/netlink_utils.h"
#include "wificond/scanning/scan_utils.h"

namespace android {
namespace wificond {

const char kTestInterfaceName[] = "testwifi0";
const std::array<uint8_t, ETH_ALEN> kTestInterfaceMacAddress = {0x10, 0x20, 0xfe, 0xae, 0x2d, 0xc2};
const uint32_t kTestInterfaceIndex = 42;
const uint32_t kTestWiphyIndex = 2;

MockClientInterfaceImpl::MockClientInterfaceImpl(
      android::wifi_system::InterfaceTool* interface_tool,
      NetlinkUtils* netlink_utils,
      ScanUtils* scan_utils)
    : ClientInterfaceImpl(
        kTestWiphyIndex,
        kTestInterfaceName,
        kTestInterfaceIndex,
        std::array<uint8_t, ETH_ALEN>(kTestInterfaceMacAddress),
        interface_tool,
        netlink_utils,
        scan_utils) {}

}  // namespace wificond
}  // namespace android
