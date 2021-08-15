//
// Copyright 2016 The Android Open Source Project
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

#define LOG_TAG "android.hardware.bluetooth@1.0-btlinux"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>

#include <utils/Log.h>

#include "bluetooth_hci.h"

#define BTPROTO_HCI 1

#define HCI_CHANNEL_USER 1
#define HCI_CHANNEL_CONTROL 3
#define HCI_DEV_NONE 0xffff

/* reference from <kernel>/include/net/bluetooth/mgmt.h */
#define MGMT_OP_INDEX_LIST 0x0003
#define MGMT_EV_INDEX_ADDED 0x0004
#define MGMT_EV_COMMAND_COMP 0x0001
#define MGMT_EV_SIZE_MAX 1024
#define WRITE_NO_INTR(fn) \
  do {                  \
  } while ((fn) == -1 && errno == EINTR)

struct sockaddr_hci {
    sa_family_t hci_family;
    unsigned short hci_dev;
    unsigned short hci_channel;
};

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

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace btlinux {

int BluetoothHci::openBtHci() {

  ALOGI( "%s", __func__);

  int hci_interface = 0;
  rfkill_state_ = NULL;
  rfKill(1);

  int fd = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
  if (fd < 0) {
    ALOGE( "Bluetooth socket error: %s", strerror(errno));
    return -1;
  }
  bt_soc_fd_ = fd;

  if (waitHciDev(hci_interface)) {
    ALOGE( "HCI interface (%d) not found", hci_interface);
    ::close(fd);
    return -1;
  }
  struct sockaddr_hci addr;
  memset(&addr, 0, sizeof(addr));
  addr.hci_family = AF_BLUETOOTH;
  addr.hci_dev = hci_interface;
  addr.hci_channel = HCI_CHANNEL_USER;
  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    ALOGE( "HCI Channel Control: %s", strerror(errno));
    ::close(fd);
    return -1;
  }
  ALOGI( "HCI device ready");
  return fd;
}

void BluetoothHci::closeBtHci() {
  if (bt_soc_fd_ != -1) {
    ::close(bt_soc_fd_);
    bt_soc_fd_ = -1;
  }
  rfKill(0);
  free(rfkill_state_);
}

int BluetoothHci::waitHciDev(int hci_interface) {
  struct sockaddr_hci addr;
  struct pollfd fds[1];
  struct mgmt_pkt ev;
  int fd;
  int ret = 0;

  ALOGI( "%s", __func__);
  fd = socket(PF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
  if (fd < 0) {
    ALOGE( "Bluetooth socket error: %s", strerror(errno));
    return -1;
  }
  memset(&addr, 0, sizeof(addr));
  addr.hci_family = AF_BLUETOOTH;
  addr.hci_dev = HCI_DEV_NONE;
  addr.hci_channel = HCI_CHANNEL_CONTROL;
  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    ALOGE( "HCI Channel Control: %s", strerror(errno));
    ret = -1;
    goto end;
  }

  fds[0].fd = fd;
  fds[0].events = POLLIN;

  /* Read Controller Index List Command */
  ev.opcode = MGMT_OP_INDEX_LIST;
  ev.index = HCI_DEV_NONE;
  ev.len = 0;

  ssize_t wrote;
  WRITE_NO_INTR(wrote = write(fd, &ev, 6));
  if (wrote != 6) {
    ALOGE( "Unable to write mgmt command: %s", strerror(errno));
    ret = -1;
    goto end;
  }
  /* validate mentioned hci interface is present and registered with sock system */
  while (1) {
    int n;
    WRITE_NO_INTR(n = poll(fds, 1, -1));
    if (n == -1) {
      ALOGE( "Poll error: %s", strerror(errno));
      ret = -1;
      break;
    } else if (n == 0) {
      ALOGE( "Timeout, no HCI device detected");
      ret = -1;
      break;
    }

    if (fds[0].revents & POLLIN) {
      WRITE_NO_INTR(n = read(fd, &ev, sizeof(struct mgmt_pkt)));
      if (n < 0) {
        ALOGE( "Error reading control channel: %s",
                  strerror(errno));
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
  ::close(fd);
  return ret;
}

int BluetoothHci::findRfKill() {
    char rfkill_type[64];
    char type[16];
    int fd, size, i;
    for(i = 0; rfkill_state_ == NULL; i++)
    {
        snprintf(rfkill_type, sizeof(rfkill_type), "/sys/class/rfkill/rfkill%d/type", i);
        if ((fd = open(rfkill_type, O_RDONLY)) < 0)
        {
            ALOGE("open(%s) failed: %s (%d)\n", rfkill_type, strerror(errno), errno);
            return -1;
        }

        size = read(fd, &type, sizeof(type));
        ::close(fd);

        if ((size >= 9) && !memcmp(type, "bluetooth", 9))
        {
            ::asprintf(&rfkill_state_, "/sys/class/rfkill/rfkill%d/state", i);
            break;
        }
    }
    return 0;
}

int BluetoothHci::rfKill(int block) {
  int fd;
  char on = (block)?'1':'0';
  if (findRfKill() != 0) return 0;

  fd = open(rfkill_state_, O_WRONLY);
  if (fd < 0) {
    ALOGE( "Unable to open /dev/rfkill");
    return -1;
  }
  ssize_t len;
  WRITE_NO_INTR(len = write(fd, &on, 1));
  if (len < 0) {
    ALOGE( "Failed to change rfkill state");
    ::close(fd);
    return -1;
  }
  ::close(fd);
  return 0;
}

class BluetoothDeathRecipient : public hidl_death_recipient {
 public:
  BluetoothDeathRecipient(const sp<IBluetoothHci> hci) : mHci(hci) {}

  virtual void serviceDied(
      uint64_t /*cookie*/,
      const wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
    ALOGE("BluetoothDeathRecipient::serviceDied - Bluetooth service died");
    has_died_ = true;
    mHci->close();
  }
  sp<IBluetoothHci> mHci;
  bool getHasDied() const { return has_died_; }
  void setHasDied(bool has_died) { has_died_ = has_died; }

 private:
  bool has_died_;
};

BluetoothHci::BluetoothHci()
    : death_recipient_(new BluetoothDeathRecipient(this)) {}

Return<void> BluetoothHci::initialize(
    const ::android::sp<IBluetoothHciCallbacks>& cb) {
  ALOGI("BluetoothHci::initialize()");
  if (cb == nullptr) {
    ALOGE("cb == nullptr! -> Unable to call initializationComplete(ERR)");
    return Void();
  }

  death_recipient_->setHasDied(false);
  cb->linkToDeath(death_recipient_, 0);
  int hci_fd = openBtHci();
  auto hidl_status = cb->initializationComplete(
          hci_fd > 0 ? Status::SUCCESS : Status::INITIALIZATION_ERROR);
  if (!hidl_status.isOk()) {
      ALOGE("VendorInterface -> Unable to call initializationComplete(ERR)");
  }
  hci::H4Protocol* h4_hci = new hci::H4Protocol(
      hci_fd,
      [cb](const hidl_vec<uint8_t>& packet) { cb->hciEventReceived(packet); },
      [cb](const hidl_vec<uint8_t>& packet) { cb->aclDataReceived(packet); },
      [cb](const hidl_vec<uint8_t>& packet) { cb->scoDataReceived(packet); });

  fd_watcher_.WatchFdForNonBlockingReads(
          hci_fd, [h4_hci](int fd) { h4_hci->OnDataReady(fd); });
  hci_handle_ = h4_hci;

  unlink_cb_ = [cb](sp<BluetoothDeathRecipient>& death_recipient) {
    if (death_recipient->getHasDied())
      ALOGI("Skipping unlink call, service died.");
    else
      cb->unlinkToDeath(death_recipient);
  };

  return Void();
}

Return<void> BluetoothHci::close() {
  ALOGI("BluetoothHci::close()");
  unlink_cb_(death_recipient_);
  fd_watcher_.StopWatchingFileDescriptors();

  if (hci_handle_ != nullptr) {
    delete hci_handle_;
    hci_handle_ = nullptr;
  }
  closeBtHci();
  return Void();
}

Return<void> BluetoothHci::sendHciCommand(const hidl_vec<uint8_t>& command) {
  sendDataToController(HCI_DATA_TYPE_COMMAND, command);
  return Void();
}

Return<void> BluetoothHci::sendAclData(const hidl_vec<uint8_t>& data) {
  sendDataToController(HCI_DATA_TYPE_ACL, data);
  return Void();
}

Return<void> BluetoothHci::sendScoData(const hidl_vec<uint8_t>& data) {
  sendDataToController(HCI_DATA_TYPE_SCO, data);
  return Void();
}

void BluetoothHci::sendDataToController(const uint8_t type,
                                        const hidl_vec<uint8_t>& data) {
  hci_handle_->Send(type, data.data(), data.size());
}

IBluetoothHci* HIDL_FETCH_IBluetoothHci(const char* /* name */) {
  return new BluetoothHci();
}

}  // namespace btlinux
}  // namespace V1_0
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
