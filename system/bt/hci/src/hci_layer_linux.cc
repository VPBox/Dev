/**********************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
 *  Copyright 2015 Intel Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 *  implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 **********************************************************************/
#include <base/bind.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/threading/thread.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include <sys/ioctl.h>
#include <sys/socket.h>

#include "buffer_allocator.h"
#include "hci_internals.h"
#include "hci_layer.h"
#include "osi/include/compat.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "osi/include/properties.h"

using base::Thread;

#define BTPROTO_HCI 1
#define HCI_CHANNEL_USER 1
#define HCI_CHANNEL_CONTROL 3
#define HCI_DEV_NONE 0xffff

#define RFKILL_TYPE_BLUETOOTH 2
#define RFKILL_OP_CHANGE_ALL 3

#define MGMT_OP_INDEX_LIST 0x0003
#define MGMT_EV_INDEX_ADDED 0x0004
#define MGMT_EV_COMMAND_COMP 0x0001
#define MGMT_EV_SIZE_MAX 1024
#define MGMT_EV_POLL_TIMEOUT 3000 /* 3000ms */

struct sockaddr_hci {
  sa_family_t hci_family;
  unsigned short hci_dev;
  unsigned short hci_channel;
};

struct rfkill_event {
  uint32_t idx;
  uint8_t type;
  uint8_t op;
  uint8_t soft, hard;
} __attribute__((packed));

struct mgmt_pkt {
  uint16_t opcode;
  uint16_t index;
  uint16_t len;
  uint8_t data[MGMT_EV_SIZE_MAX];
} __attribute__((packed));

struct mgmt_event_read_index {
  uint16_t cc_opcode;
  uint8_t status;
  uint16_t num_intf;
  uint16_t index[0];
} __attribute__((packed));

enum HciPacketType {
  HCI_PACKET_TYPE_UNKNOWN = 0,
  HCI_PACKET_TYPE_COMMAND = 1,
  HCI_PACKET_TYPE_ACL_DATA = 2,
  HCI_PACKET_TYPE_SCO_DATA = 3,
  HCI_PACKET_TYPE_EVENT = 4
};

extern void initialization_complete();
extern void hci_event_received(const base::Location& from_here, BT_HDR* packet);
extern void acl_event_received(BT_HDR* packet);
extern void sco_data_received(BT_HDR* packet);

static int bt_vendor_fd = -1;
static int hci_interface;
static int rfkill_en;
static int wait_hcidev(void);
static int rfkill(int block);

int reader_thread_ctrl_fd = -1;
Thread* reader_thread = NULL;

void monitor_socket(int ctrl_fd, int fd) {
  const allocator_t* buffer_allocator = buffer_allocator_get_interface();
  const size_t buf_size = 2000;
  uint8_t buf[buf_size];
  ssize_t len = read(fd, buf, buf_size);

  while (len > 0) {
    if (len == buf_size)
      LOG(FATAL) << "This packet filled buffer, if it have continuation we "
                    "don't know how to merge it, increase buffer size!";

    uint8_t type = buf[0];

    size_t packet_size = buf_size + BT_HDR_SIZE;
    BT_HDR* packet =
        reinterpret_cast<BT_HDR*>(buffer_allocator->alloc(packet_size));
    packet->offset = 0;
    packet->layer_specific = 0;
    packet->len = len - 1;
    memcpy(packet->data, buf + 1, len - 1);

    switch (type) {
      case HCI_PACKET_TYPE_COMMAND:
        packet->event = MSG_HC_TO_STACK_HCI_EVT;
        hci_event_received(FROM_HERE, packet);
        break;
      case HCI_PACKET_TYPE_ACL_DATA:
        packet->event = MSG_HC_TO_STACK_HCI_ACL;
        acl_event_received(packet);
        break;
      case HCI_PACKET_TYPE_SCO_DATA:
        packet->event = MSG_HC_TO_STACK_HCI_SCO;
        sco_data_received(packet);
        break;
      case HCI_PACKET_TYPE_EVENT:
        packet->event = MSG_HC_TO_STACK_HCI_EVT;
        hci_event_received(FROM_HERE, packet);
        break;
      default:
        LOG(FATAL) << "Unexpected event type: " << +type;
        break;
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(ctrl_fd, &fds);
    FD_SET(fd, &fds);
    int res = select(std::max(fd, ctrl_fd) + 1, &fds, NULL, NULL, NULL);
    if (res <= 0) LOG(INFO) << "Nothing more to read";

    if (FD_ISSET(ctrl_fd, &fds)) {
      LOG(INFO) << "exitting";
      return;
    }

    len = read(fd, buf, buf_size);
  }
}

/* TODO: should thread the device waiting and return immedialty */
void hci_initialize() {
  LOG(INFO) << __func__;

  char prop_value[PROPERTY_VALUE_MAX];
  osi_property_get("bluetooth.interface", prop_value, "0");

  errno = 0;
  if (memcmp(prop_value, "hci", 3))
    hci_interface = strtol(prop_value, NULL, 10);
  else
    hci_interface = strtol(prop_value + 3, NULL, 10);
  if (errno) hci_interface = 0;

  LOG(INFO) << "Using interface hci" << +hci_interface;

  osi_property_get("bluetooth.rfkill", prop_value, "1");

  rfkill_en = atoi(prop_value);
  if (rfkill_en) {
    rfkill(0);
  }

  int fd = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
  CHECK(fd >= 0) << "socket create error" << strerror(errno);

  bt_vendor_fd = fd;

  if (wait_hcidev()) {
    LOG(FATAL) << "HCI interface hci" << +hci_interface << " not found";
  }

  struct sockaddr_hci addr;
  memset(&addr, 0, sizeof(addr));
  addr.hci_family = AF_BLUETOOTH;
  addr.hci_dev = hci_interface;
  addr.hci_channel = HCI_CHANNEL_USER;
  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    PLOG(FATAL) << "socket bind error";
  }

  int sv[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
    PLOG(FATAL) << "socketpair failed";
  }

  reader_thread_ctrl_fd = sv[0];
  reader_thread = new Thread("hci_sock_reader");
  reader_thread->Start();
  reader_thread->task_runner()->PostTask(
      FROM_HERE, base::Bind(&monitor_socket, sv[1], bt_vendor_fd));

  LOG(INFO) << "HCI device ready";
  initialization_complete();
}

void hci_close() {
  LOG(INFO) << __func__;

  if (bt_vendor_fd != -1) {
    close(bt_vendor_fd);
    bt_vendor_fd = -1;
  }

  if (reader_thread_ctrl_fd != -1) {
    uint8_t msg[] = {1};
    send(reader_thread_ctrl_fd, msg, sizeof(msg), 0);
    reader_thread_ctrl_fd = -1;
  }

  if (reader_thread != NULL) {
    reader_thread->Stop();
    delete reader_thread;
    reader_thread = NULL;
  }

  rfkill(1);
}

void hci_transmit(BT_HDR* packet) {
  uint8_t type = 0;

  CHECK(bt_vendor_fd != -1);

  uint16_t event = packet->event & MSG_EVT_MASK;
  switch (event & MSG_EVT_MASK) {
    case MSG_STACK_TO_HC_HCI_CMD:
      type = 1;
      break;
    case MSG_STACK_TO_HC_HCI_ACL:
      type = 2;
      break;
    case MSG_STACK_TO_HC_HCI_SCO:
      type = 3;
      break;
    default:
      LOG(FATAL) << "Unknown packet type " << event;
      break;
  }

  uint8_t* addr = packet->data + packet->offset - 1;
  uint8_t store = *addr;
  *addr = type;
  size_t ret = write(bt_vendor_fd, addr, packet->len + 1);

  *(addr) = store;

  if (ret != packet->len + 1) LOG(ERROR) << "Should have send whole packet";

  if (ret == -1) PLOG(FATAL) << "write failed";
}

static int wait_hcidev(void) {
  struct sockaddr_hci addr;
  struct pollfd fds[1];
  struct mgmt_pkt ev;
  int fd;
  int ret = 0;

  LOG(INFO) << __func__;

  fd = socket(PF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
  if (fd < 0) {
    PLOG(ERROR) << "Bluetooth socket error";
    return -1;
  }

  memset(&addr, 0, sizeof(addr));
  addr.hci_family = AF_BLUETOOTH;
  addr.hci_dev = HCI_DEV_NONE;
  addr.hci_channel = HCI_CHANNEL_CONTROL;

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    PLOG(ERROR) << "HCI Channel Control";
    close(fd);
    return -1;
  }

  fds[0].fd = fd;
  fds[0].events = POLLIN;

  /* Read Controller Index List Command */
  ev.opcode = MGMT_OP_INDEX_LIST;
  ev.index = HCI_DEV_NONE;
  ev.len = 0;

  ssize_t wrote;
  OSI_NO_INTR(wrote = write(fd, &ev, 6));
  if (wrote != 6) {
    PLOG(ERROR) << "Unable to write mgmt command";
    ret = -1;
    goto end;
  }

  while (1) {
    int n;
    OSI_NO_INTR(n = poll(fds, 1, MGMT_EV_POLL_TIMEOUT));
    if (n == -1) {
      PLOG(ERROR) << "Poll error";
      ret = -1;
      break;
    } else if (n == 0) {
      LOG(ERROR) << "Timeout, no HCI device detected";
      ret = -1;
      break;
    }

    if (fds[0].revents & POLLIN) {
      OSI_NO_INTR(n = read(fd, &ev, sizeof(struct mgmt_pkt)));
      if (n < 0) {
        PLOG(ERROR) << "Error reading control channel";
        ret = -1;
        break;
      }

      if (ev.opcode == MGMT_EV_INDEX_ADDED && ev.index == hci_interface) {
        goto end;
      } else if (ev.opcode == MGMT_EV_COMMAND_COMP) {
        struct mgmt_event_read_index* cc;
        int i;

        cc = (struct mgmt_event_read_index*)ev.data;

        if (cc->cc_opcode != MGMT_OP_INDEX_LIST || cc->status != 0) continue;

        for (i = 0; i < cc->num_intf; i++) {
          if (cc->index[i] == hci_interface) goto end;
        }
      }
    }
  }

end:
  close(fd);
  return ret;
}

static int rfkill(int block) {
  struct rfkill_event event;
  int fd;

  LOG(INFO) << __func__;

  fd = open("/dev/rfkill", O_WRONLY);
  if (fd < 0) {
    LOG(ERROR) << "Unable to open /dev/rfkill";
    return -1;
  }

  memset(&event, 0, sizeof(struct rfkill_event));
  event.op = RFKILL_OP_CHANGE_ALL;
  event.type = RFKILL_TYPE_BLUETOOTH;
  event.hard = block;
  event.soft = block;

  ssize_t len;
  OSI_NO_INTR(len = write(fd, &event, sizeof(event)));
  if (len < 0) {
    LOG(ERROR) << "Failed to change rfkill state";
    close(fd);
    return 1;
  }

  close(fd);
  return 0;
}

int hci_open_firmware_log_file() { return INVALID_FD; }

void hci_close_firmware_log_file(int fd) {}

void hci_log_firmware_debug_packet(int fd, BT_HDR* packet) {}
