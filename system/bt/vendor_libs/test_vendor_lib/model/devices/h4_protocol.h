//
// Copyright 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include "hci_protocol.h"
#include "include/hci.h"

namespace test_vendor_lib {
namespace hci {

class H4Protocol : public HciProtocol {
 public:
  H4Protocol(int fd, PacketReadCallback command_cb, PacketReadCallback event_cb, PacketReadCallback acl_cb,
             PacketReadCallback sco_cb);

  size_t Send(uint8_t type, const uint8_t* data, size_t length);

  void OnPacketReady();

  void OnDataReady(int fd);

 private:
  int uart_fd_;

  PacketReadCallback command_cb_;
  PacketReadCallback event_cb_;
  PacketReadCallback acl_cb_;
  PacketReadCallback sco_cb_;

  hci::PacketType hci_packet_type_{hci::PacketType::UNKNOWN};
  hci::HciPacketizer hci_packetizer_;
};

}  // namespace hci
}  // namespace test_vendor_lib
