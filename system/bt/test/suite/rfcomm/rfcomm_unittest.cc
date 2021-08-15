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
#include "rfcomm/rfcomm_test.h"

#include <sys/socket.h>
#include <unistd.h>

namespace {
static const char HANDSHAKE_COMMAND[] = "AT+BRSF=29\r";
}  // namespace

namespace bttest {

TEST_F(RFCommTest, RfcommConnectPairedDevice) {
  int fd = -1;
  int error = 0;
  size_t len = 0;

  error = socket_interface()->connect(&bt_remote_bdaddr_, BTSOCK_RFCOMM,
                                      &HFP_UUID, 0, &fd, 0, getuid());
  EXPECT_TRUE(error == BT_STATUS_SUCCESS) << "Error creating RFCOMM socket: "
                                          << error;
  EXPECT_TRUE(fd != -1) << "Error creating RFCOMM socket: invalid fd";

  int channel;
  sock_connect_signal_t signal;
  len = read(fd, &channel, sizeof(channel));
  EXPECT_TRUE(len == sizeof(channel))
      << "Channel not read from RFCOMM socket. Bytes read: " << len;
  len = read(fd, &signal, sizeof(signal));
  EXPECT_TRUE(len == sizeof(signal))
      << "Connection signal not read from RFCOMM socket. Bytes read: " << len;

  EXPECT_TRUE(signal.bd_addr == bt_remote_bdaddr_)
      << "Connected to a different bdaddr than expected.";
  EXPECT_TRUE(channel == signal.channel)
      << "Inconsistent channels returned: " << channel << " and "
      << signal.channel;

  len = write(fd, HANDSHAKE_COMMAND, sizeof(HANDSHAKE_COMMAND));
  EXPECT_TRUE(len == sizeof(HANDSHAKE_COMMAND))
      << "Unable to send HFP handshake. Bytes written: " << len;

  char response[1024];
  len = read(fd, response, sizeof(response));
  EXPECT_TRUE(len > 0) << "Read " << len << " bytes";

  close(fd);
}

TEST_F(RFCommTest, RfcommRepeatedConnectPairedDevice) {
  static const int max_iterations = 128;
  int channel_fail = 0, signal_fail = 0, handshake_fail = 0, read_fail = 0;

  for (int i = 0; i < max_iterations; ++i) {
    int fd = -1;
    int error = 0;
    size_t len = 0;

    error = socket_interface()->connect(&bt_remote_bdaddr_, BTSOCK_RFCOMM,
                                        &HFP_UUID, 0, &fd, 0, getuid());
    ASSERT_TRUE(error == BT_STATUS_SUCCESS) << "Error creating RFCOMM socket: "
                                            << error;
    ASSERT_TRUE(fd != -1) << "Error creating RFCOMM socket: invalid fd";

    int channel;
    sock_connect_signal_t signal;
    len = read(fd, &channel, sizeof(channel));
    if (len != sizeof(channel)) {
      ADD_FAILURE() << "Channel not read from RFCOMM socket. Bytes read: "
                    << len << ", Sizeof channel: " << sizeof(channel);
      channel_fail++;
    }

    len = read(fd, &signal, sizeof(signal));
    if (len != sizeof(signal)) {
      ADD_FAILURE()
          << "Connection signal not read from RFCOMM socket. Bytes read: "
          << len;
      signal_fail++;
    }

    EXPECT_TRUE(signal.bd_addr == bt_remote_bdaddr_)
        << "Connected to a different bdaddr than expected.";
    EXPECT_TRUE(channel == signal.channel)
        << "Inconsistent channels returned: " << channel << " and "
        << signal.channel;
    len = write(fd, HANDSHAKE_COMMAND, sizeof(HANDSHAKE_COMMAND));
    if (len != sizeof(HANDSHAKE_COMMAND)) {
      ADD_FAILURE() << "Unable to send HFP handshake. Bytes written: " << len;
      handshake_fail++;
    }

    char response[1024];
    len = read(fd, response, sizeof(response));
    if (len <= 0) {
      ADD_FAILURE() << "Read " << len << " bytes";
      read_fail++;
    }

    close(fd);
  }

  if (channel_fail > 0 || signal_fail > 0 || handshake_fail > 0 ||
      read_fail > 0) {
    ADD_FAILURE() << "Number of channel read fails: " << channel_fail << "\n"
                  << "Number of signal read fails: " << signal_fail << "\n"
                  << "Number of handshake send fails: " << handshake_fail
                  << "\n"
                  << "Number of read response fails: " << read_fail;
  }
}

}  // bttest
