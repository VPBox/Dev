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

#include "adapter/bluetooth_test.h"

namespace bttest {

class RFCommTest : public BluetoothTest {
 protected:
  RFCommTest() = default;
  virtual ~RFCommTest() = default;

  // Getter for the RFCOMM socket
  const btsock_interface_t* socket_interface() const {
    return socket_interface_;
  }

  // SetUp initializes the Bluetooth interfaces and the RFCOMM interface
  virtual void SetUp();

  // TearDown cleans up the Bluetooth and RFCOMM interfaces
  virtual void TearDown();

  RawAddress bt_remote_bdaddr_;

  static const bluetooth::Uuid HFP_UUID;

 private:
  const btsock_interface_t* socket_interface_;
};

}  // bttest
