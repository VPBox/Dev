/******************************************************************************
 *
 *  Copyright 2017 Google, Inc.
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

#define LOG_TAG "bt_hci"

#include "hci_layer.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <base/location.h>
#include <base/logging.h>
#include "buffer_allocator.h"
#include "osi/include/log.h"

#include <android/hardware/bluetooth/1.0/IBluetoothHci.h>
#include <android/hardware/bluetooth/1.0/IBluetoothHciCallbacks.h>
#include <android/hardware/bluetooth/1.0/types.h>
#include <hwbinder/ProcessState.h>

#define LOG_PATH "/data/misc/bluetooth/logs/firmware_events.log"
#define LAST_LOG_PATH "/data/misc/bluetooth/logs/firmware_events.log.last"

using ::android::hardware::hidl_death_recipient;
using ::android::hardware::hidl_vec;
using ::android::hardware::ProcessState;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::bluetooth::V1_0::HciPacket;
using ::android::hardware::bluetooth::V1_0::IBluetoothHci;
using ::android::hardware::bluetooth::V1_0::IBluetoothHciCallbacks;
using ::android::hardware::bluetooth::V1_0::Status;

extern void initialization_complete();
extern void hci_event_received(const base::Location& from_here, BT_HDR* packet);
extern void acl_event_received(BT_HDR* packet);
extern void sco_data_received(BT_HDR* packet);
extern void hal_service_died();

android::sp<IBluetoothHci> btHci;

class BluetoothHciDeathRecipient : public hidl_death_recipient {
 public:
  virtual void serviceDied(uint64_t /*cookie*/, const android::wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
    LOG_ERROR(LOG_TAG, "Bluetooth HAL service died!");
    hal_service_died();
  }
};
android::sp<BluetoothHciDeathRecipient> bluetoothHciDeathRecipient = new BluetoothHciDeathRecipient();

class BluetoothHciCallbacks : public IBluetoothHciCallbacks {
 public:
  BluetoothHciCallbacks() {
    buffer_allocator = buffer_allocator_get_interface();
  }

  BT_HDR* WrapPacketAndCopy(uint16_t event, const hidl_vec<uint8_t>& data) {
    size_t packet_size = data.size() + BT_HDR_SIZE;
    BT_HDR* packet =
        reinterpret_cast<BT_HDR*>(buffer_allocator->alloc(packet_size));
    packet->offset = 0;
    packet->len = data.size();
    packet->layer_specific = 0;
    packet->event = event;
    // TODO(eisenbach): Avoid copy here; if BT_HDR->data can be ensured to
    // be the only way the data is accessed, a pointer could be passed here...
    memcpy(packet->data, data.data(), data.size());
    return packet;
  }

  Return<void> initializationComplete(Status status) {
    CHECK(status == Status::SUCCESS);
    initialization_complete();
    return Void();
  }

  Return<void> hciEventReceived(const hidl_vec<uint8_t>& event) {
    BT_HDR* packet = WrapPacketAndCopy(MSG_HC_TO_STACK_HCI_EVT, event);
    hci_event_received(FROM_HERE, packet);
    return Void();
  }

  Return<void> aclDataReceived(const hidl_vec<uint8_t>& data) {
    BT_HDR* packet = WrapPacketAndCopy(MSG_HC_TO_STACK_HCI_ACL, data);
    acl_event_received(packet);
    return Void();
  }

  Return<void> scoDataReceived(const hidl_vec<uint8_t>& data) {
    BT_HDR* packet = WrapPacketAndCopy(MSG_HC_TO_STACK_HCI_SCO, data);
    sco_data_received(packet);
    return Void();
  }

  const allocator_t* buffer_allocator;
};

void hci_initialize() {
  LOG_INFO(LOG_TAG, "%s", __func__);

  btHci = IBluetoothHci::getService();
  // If android.hardware.bluetooth* is not found, Bluetooth can not continue.
  CHECK(btHci != nullptr);
  auto death_link = btHci->linkToDeath(bluetoothHciDeathRecipient, 0);
  if (!death_link.isOk()) {
    LOG_ERROR(LOG_TAG, "%s: Unable to set the death recipient for the Bluetooth HAL", __func__);
    abort();
  }
  LOG_INFO(LOG_TAG, "%s: IBluetoothHci::getService() returned %p (%s)",
           __func__, btHci.get(), (btHci->isRemote() ? "remote" : "local"));

  // Block allows allocation of a variable that might be bypassed by goto.
  {
    android::sp<IBluetoothHciCallbacks> callbacks = new BluetoothHciCallbacks();
    btHci->initialize(callbacks);
  }
}

void hci_close() {
  if (btHci != nullptr) {
    auto death_unlink = btHci->unlinkToDeath(bluetoothHciDeathRecipient);
    if (!death_unlink.isOk()) {
      LOG_ERROR(LOG_TAG, "%s: Error unlinking death recipient from the Bluetooth HAL", __func__);
    }
  }
  btHci->close();
  btHci = nullptr;
}

void hci_transmit(BT_HDR* packet) {
  HciPacket data;
  data.setToExternal(packet->data + packet->offset, packet->len);

  uint16_t event = packet->event & MSG_EVT_MASK;
  switch (event & MSG_EVT_MASK) {
    case MSG_STACK_TO_HC_HCI_CMD:
      btHci->sendHciCommand(data);
      break;
    case MSG_STACK_TO_HC_HCI_ACL:
      btHci->sendAclData(data);
      break;
    case MSG_STACK_TO_HC_HCI_SCO:
      btHci->sendScoData(data);
      break;
    default:
      LOG_ERROR(LOG_TAG, "Unknown packet type (%d)", event);
      break;
  }
}

int hci_open_firmware_log_file() {
  if (rename(LOG_PATH, LAST_LOG_PATH) == -1 && errno != ENOENT) {
    LOG_ERROR(LOG_TAG, "%s unable to rename '%s' to '%s': %s", __func__,
              LOG_PATH, LAST_LOG_PATH, strerror(errno));
  }

  mode_t prevmask = umask(0);
  int logfile_fd = open(LOG_PATH, O_WRONLY | O_CREAT | O_TRUNC,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  umask(prevmask);
  if (logfile_fd == INVALID_FD) {
    LOG_ERROR(LOG_TAG, "%s unable to open '%s': %s", __func__, LOG_PATH,
              strerror(errno));
  }

  return logfile_fd;
}

void hci_close_firmware_log_file(int fd) {
  if (fd != INVALID_FD) close(fd);
}

void hci_log_firmware_debug_packet(int fd, BT_HDR* packet) {
  TEMP_FAILURE_RETRY(write(fd, packet->data, packet->len));
}
