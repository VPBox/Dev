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

#include "h4_protocol.h"

#define LOG_TAG "android.hardware.bluetooth-hci-h4"
#include <android-base/logging.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <utils/Log.h>

namespace android {
namespace hardware {
namespace bluetooth {
namespace hci {

size_t H4Protocol::Send(uint8_t type, const uint8_t* data, size_t length) {
    /* For HCI communication over USB dongle, multiple write results in
     * response timeout as driver expect type + data at once to process
     * the command, so using "writev"(for atomicity) here.
     */
    struct iovec iov[2];
    ssize_t ret = 0;
    iov[0].iov_base = &type;
    iov[0].iov_len = sizeof(type);
    iov[1].iov_base = (void *)data;
    iov[1].iov_len = length;
    while (1) {
        ret = TEMP_FAILURE_RETRY(writev(uart_fd_, iov, 2));
        if (ret == -1) {
            if (errno == EAGAIN) {
                ALOGE("%s error writing to UART (%s)", __func__, strerror(errno));
                continue;
            }
        } else if (ret == 0) {
            // Nothing written :(
            ALOGE("%s zero bytes written - something went wrong...", __func__);
            break;
        }
        break;
    }
    return ret;
}

void H4Protocol::OnPacketReady() {
  switch (hci_packet_type_) {
    case HCI_PACKET_TYPE_EVENT:
      event_cb_(hci_packetizer_.GetPacket());
      break;
    case HCI_PACKET_TYPE_ACL_DATA:
      acl_cb_(hci_packetizer_.GetPacket());
      break;
    case HCI_PACKET_TYPE_SCO_DATA:
      sco_cb_(hci_packetizer_.GetPacket());
      break;
    default: {
      bool bad_packet_type = true;
      CHECK(!bad_packet_type);
    }
  }
  // Get ready for the next type byte.
  hci_packet_type_ = HCI_PACKET_TYPE_UNKNOWN;
}

void H4Protocol::OnDataReady(int fd) {
    if (hci_packet_type_ == HCI_PACKET_TYPE_UNKNOWN) {
        /**
         * read full buffer. ACL max length is 2 bytes, and SCO max length is 2
         * byte. so taking 64K as buffer length.
         * Question : Why to read in single chunk rather than multiple reads,
         * which can give parameter length arriving in response ?
         * Answer: The multiple reads does not work with BT USB dongle. At least
         * with Bluetooth 2.0 supported USB dongle. After first read, either
         * firmware/kernel (do not know who is responsible - inputs ??) driver
         * discard the whole message and successive read results in forever
         * blocking loop. - Is there any other way to make it work with multiple
         * reads, do not know yet (it can eliminate need of this function) ?
         * Reading in single shot gives expected response.
         */
        const size_t max_plen = 64*1024;
        hidl_vec<uint8_t> tpkt;
        tpkt.resize(max_plen);
        size_t bytes_read = TEMP_FAILURE_RETRY(read(fd, tpkt.data(), max_plen));
        hci_packet_type_ = static_cast<HciPacketType>(tpkt.data()[0]);
        hci_packetizer_.CbHciPacket(tpkt.data()+1, bytes_read-1);
    }
}

}  // namespace hci
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
