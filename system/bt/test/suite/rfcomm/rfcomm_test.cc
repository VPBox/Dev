/******************************************************************************
 *
 *  Copyright 2016 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include "rfcomm/rfcomm_test.h"
#include "adapter/bluetooth_test.h"

using bluetooth::Uuid;

namespace bttest {

const Uuid RFCommTest::HFP_UUID = Uuid::From16Bit(0x111E);

void RFCommTest::SetUp() {
  BluetoothTest::SetUp();

  ASSERT_EQ(bt_interface()->enable(), BT_STATUS_SUCCESS);
  semaphore_wait(adapter_state_changed_callback_sem_);
  ASSERT_TRUE(GetState() == BT_STATE_ON);
  socket_interface_ =
      (const btsock_interface_t*)bt_interface()->get_profile_interface(
          BT_PROFILE_SOCKETS_ID);
  ASSERT_NE(socket_interface_, nullptr);

  // Find a bonded device that supports HFP
  bt_remote_bdaddr_ = RawAddress::kEmpty;

  bt_property_t* bonded_devices_prop =
      GetProperty(BT_PROPERTY_ADAPTER_BONDED_DEVICES);
  RawAddress* devices = (RawAddress*)bonded_devices_prop->val;
  int num_bonded_devices = bonded_devices_prop->len / sizeof(RawAddress);

  for (int i = 0; i < num_bonded_devices && bt_remote_bdaddr_.IsEmpty(); i++) {
    ClearSemaphore(remote_device_properties_callback_sem_);
    bt_interface()->get_remote_device_property(&devices[i], BT_PROPERTY_UUIDS);
    semaphore_wait(remote_device_properties_callback_sem_);

    bt_property_t* uuid_prop =
        GetRemoteDeviceProperty(&devices[i], BT_PROPERTY_UUIDS);
    if (uuid_prop == nullptr) continue;
    Uuid* uuids = reinterpret_cast<Uuid*>(uuid_prop->val);
    int num_uuids = uuid_prop->len / sizeof(Uuid);

    for (int j = 0; j < num_uuids; j++) {
      if (!memcmp(uuids + j, &HFP_UUID, sizeof(Uuid))) {
        bt_remote_bdaddr_ = *(devices + i);
        break;
      }
    }
  }

  ASSERT_FALSE(bt_remote_bdaddr_.IsEmpty())
      << "Could not find paired device that supports HFP";
}

void RFCommTest::TearDown() {
  socket_interface_ = NULL;

  ASSERT_EQ(bt_interface()->disable(), BT_STATUS_SUCCESS);
  semaphore_wait(adapter_state_changed_callback_sem_);

  BluetoothTest::TearDown();
}

}  // bttest
