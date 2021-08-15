/*
 * Copyright (C) 2012 The Android Open Source Project
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

#pragma once

__BEGIN_DECLS

#define BTSOCK_FLAG_ENCRYPT 1
#define BTSOCK_FLAG_AUTH (1 << 1)
#define BTSOCK_FLAG_NO_SDP (1 << 2)
#define BTSOCK_FLAG_AUTH_MITM (1 << 3)
#define BTSOCK_FLAG_AUTH_16_DIGIT (1 << 4)
#define BTSOCK_FLAG_LE_COC (1 << 5)

typedef enum {
  BTSOCK_RFCOMM = 1,
  BTSOCK_SCO = 2,
  BTSOCK_L2CAP = 3,
  BTSOCK_L2CAP_LE = 4
} btsock_type_t;

/** Represents the standard BT SOCKET interface. */
typedef struct {
  short size;
  RawAddress bd_addr;
  int channel;
  int status;

  // The writer must make writes using a buffer of this maximum size
  // to avoid loosing data. (L2CAP only)
  unsigned short max_tx_packet_size;

  // The reader must read using a buffer of at least this size to avoid
  // loosing data. (L2CAP only)
  unsigned short max_rx_packet_size;
} __attribute__((packed)) sock_connect_signal_t;

typedef struct {
  /** set to size of this struct*/
  size_t size;

  /**
   * Listen to a RFCOMM UUID or channel. It returns the socket fd from which
   * btsock_connect_signal can be read out when a remote device connected.
   * If neither a UUID nor a channel is provided, a channel will be allocated
   * and a service record can be created providing the channel number to
   * create_sdp_record(...) in bt_sdp.
   * The callingUid is the UID of the application which is requesting the
   * socket. This is used for traffic accounting purposes.
   */
  bt_status_t (*listen)(btsock_type_t type, const char* service_name,
                        const bluetooth::Uuid* service_uuid, int channel,
                        int* sock_fd, int flags, int callingUid);

  /**
   * Connect to a RFCOMM UUID channel of remote device, It returns the socket fd
   * from which the btsock_connect_signal and a new socket fd to be accepted can
   * be read out when connected. The callingUid is the UID of the application
   * which is requesting the socket. This is used for traffic accounting
   * purposes.
   */
  bt_status_t (*connect)(const RawAddress* bd_addr, btsock_type_t type,
                         const bluetooth::Uuid* uuid, int channel, int* sock_fd,
                         int flags, int callingUid);

  /**
   * Set the LE Data Length value to this connected peer to the
   * maximum supported by this BT controller. This command
   * suggests to the BT controller to set its maximum transmission
   * packet size.
   */
  void (*request_max_tx_data_length)(const RawAddress& bd_addr);

} btsock_interface_t;

__END_DECLS
