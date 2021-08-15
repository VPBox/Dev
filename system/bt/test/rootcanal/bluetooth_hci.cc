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

#define LOG_TAG "android.hardware.bluetooth@1.0.sim"

#include "bluetooth_hci.h"

#include <base/logging.h>
#include <cutils/properties.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <utils/Log.h>

#include "hci_internals.h"

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace sim {

using android::hardware::hidl_vec;
using test_vendor_lib::AsyncTaskId;
using test_vendor_lib::DualModeController;
using test_vendor_lib::TaskCallback;

namespace {

bool BtTestConsoleEnabled() {
  // Assume enabled by default.
  return property_get_bool("bt.rootcanal_test_console", true);
}

}  // namespace

class BluetoothDeathRecipient : public hidl_death_recipient {
 public:
  BluetoothDeathRecipient(const sp<IBluetoothHci> hci) : mHci(hci) {}

  virtual void serviceDied(uint64_t /* cookie */, const wp<::android::hidl::base::V1_0::IBase>& /* who */) {
    ALOGE("BluetoothDeathRecipient::serviceDied - Bluetooth service died");
    has_died_ = true;
    mHci->close();
  }
  sp<IBluetoothHci> mHci;
  bool getHasDied() const {
    return has_died_;
  }
  void setHasDied(bool has_died) {
    has_died_ = has_died;
  }

 private:
  bool has_died_;
};

BluetoothHci::BluetoothHci() : death_recipient_(new BluetoothDeathRecipient(this)) {}

Return<void> BluetoothHci::initialize(const sp<IBluetoothHciCallbacks>& cb) {
  ALOGI("%s", __func__);

  if (cb == nullptr) {
    ALOGE("cb == nullptr! -> Unable to call initializationComplete(ERR)");
    return Void();
  }

  death_recipient_->setHasDied(false);
  cb->linkToDeath(death_recipient_, 0);

  test_channel_transport_.RegisterCommandHandler([this](const std::string& name, const std::vector<std::string>& args) {
    async_manager_.ExecAsync(std::chrono::milliseconds(0),
                             [this, name, args]() { test_channel_.HandleCommand(name, args); });
  });

  controller_ = std::make_shared<DualModeController>();

  controller_->Initialize({"dmc", "3C:5A:B4:01:02:03"});

  controller_->RegisterEventChannel([cb](std::shared_ptr<std::vector<uint8_t>> packet) {
    hidl_vec<uint8_t> hci_event(packet->begin(), packet->end());
    cb->hciEventReceived(hci_event);
  });

  controller_->RegisterAclChannel([cb](std::shared_ptr<std::vector<uint8_t>> packet) {
    hidl_vec<uint8_t> acl_packet(packet->begin(), packet->end());
    cb->aclDataReceived(acl_packet);
  });

  controller_->RegisterScoChannel([cb](std::shared_ptr<std::vector<uint8_t>> packet) {
    hidl_vec<uint8_t> sco_packet(packet->begin(), packet->end());
    cb->aclDataReceived(sco_packet);
  });

  controller_->RegisterTaskScheduler([this](std::chrono::milliseconds delay, const TaskCallback& task) {
    return async_manager_.ExecAsync(delay, task);
  });

  controller_->RegisterPeriodicTaskScheduler(
      [this](std::chrono::milliseconds delay, std::chrono::milliseconds period, const TaskCallback& task) {
        return async_manager_.ExecAsyncPeriodically(delay, period, task);
      });

  controller_->RegisterTaskCancel([this](AsyncTaskId task) { async_manager_.CancelAsyncTask(task); });

  test_model_.Reset();
  // Add the controller as a device in the model.
  test_model_.Add(controller_);

  // Send responses to logcat if the test channel is not configured.
  test_channel_.RegisterSendResponse([](const std::string& response) {
    ALOGI("No test channel yet: %s", response.c_str());
  });

  if (BtTestConsoleEnabled()) {
    SetUpTestChannel(6111);
    SetUpHciServer(6211,
                   [this](int fd) { test_model_.IncomingHciConnection(fd); });
    SetUpLinkLayerServer(
        6311, [this](int fd) { test_model_.IncomingLinkLayerConnection(fd); });
  }

  // Add some default devices for easier debugging
  test_channel_.AddDefaults();

  // This should be configurable in the future.
  ALOGI("Adding Beacons so the scan list is not empty.");
  test_channel_.Add({"beacon", "be:ac:10:00:00:01", "1000"});
  test_channel_.AddDeviceToPhy({"1", "0"});
  test_channel_.Add({"beacon", "be:ac:10:00:00:02", "1000"});
  test_channel_.AddDeviceToPhy({"2", "0"});
  test_channel_.SetTimerPeriod({"1000"});

  unlink_cb_ = [cb](sp<BluetoothDeathRecipient>& death_recipient) {
    if (death_recipient->getHasDied())
      ALOGI("Skipping unlink call, service died.");
    else
      cb->unlinkToDeath(death_recipient);
  };

  cb->initializationComplete(Status::SUCCESS);
  return Void();
}

Return<void> BluetoothHci::close() {
  ALOGI("%s", __func__);
  return Void();
}

Return<void> BluetoothHci::sendHciCommand(const hidl_vec<uint8_t>& packet) {
  async_manager_.ExecAsync(std::chrono::milliseconds(0), [this, packet]() {
    std::shared_ptr<std::vector<uint8_t>> packet_copy =
        std::shared_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(packet));
    controller_->HandleCommand(packet_copy);
  });
  return Void();
}

Return<void> BluetoothHci::sendAclData(const hidl_vec<uint8_t>& packet) {
  async_manager_.ExecAsync(std::chrono::milliseconds(0), [this, packet]() {
    std::shared_ptr<std::vector<uint8_t>> packet_copy =
        std::shared_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(packet));
    controller_->HandleAcl(packet_copy);
  });
  return Void();
}

Return<void> BluetoothHci::sendScoData(const hidl_vec<uint8_t>& packet) {
  async_manager_.ExecAsync(std::chrono::milliseconds(0), [this, packet]() {
    std::shared_ptr<std::vector<uint8_t>> packet_copy =
        std::shared_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(packet));
    controller_->HandleSco(packet_copy);
  });
  return Void();
}

void BluetoothHci::SetUpHciServer(int port, const std::function<void(int)>& connection_callback) {
  int socket_fd = remote_hci_transport_.SetUp(port);

  test_channel_.RegisterSendResponse(
      [](const std::string& response) { ALOGI("No HCI Response channel: %s", response.c_str()); });

  if (socket_fd == -1) {
    ALOGE("Remote HCI channel SetUp(%d) failed.", port);
    return;
  }

  async_manager_.WatchFdForNonBlockingReads(socket_fd, [this, connection_callback](int socket_fd) {
    int conn_fd = remote_hci_transport_.Accept(socket_fd);
    if (conn_fd < 0) {
      ALOGE("Error watching remote HCI channel fd.");
      return;
    }
    int flags = fcntl(conn_fd, F_GETFL, NULL);
    int ret;
    ret = fcntl(conn_fd, F_SETFL, flags | O_NONBLOCK);
    CHECK(ret != -1) << "Error setting O_NONBLOCK " << strerror(errno);

    connection_callback(conn_fd);
  });
}

void BluetoothHci::SetUpLinkLayerServer(int port, const std::function<void(int)>& connection_callback) {
  int socket_fd = remote_link_layer_transport_.SetUp(port);

  test_channel_.RegisterSendResponse(
      [](const std::string& response) { ALOGI("No LinkLayer Response channel: %s", response.c_str()); });

  if (socket_fd == -1) {
    ALOGE("Remote LinkLayer channel SetUp(%d) failed.", port);
    return;
  }

  async_manager_.WatchFdForNonBlockingReads(socket_fd, [this, connection_callback](int socket_fd) {
    int conn_fd = remote_link_layer_transport_.Accept(socket_fd);
    if (conn_fd < 0) {
      ALOGE("Error watching remote LinkLayer channel fd.");
      return;
    }
    int flags = fcntl(conn_fd, F_GETFL, NULL);
    int ret = fcntl(conn_fd, F_SETFL, flags | O_NONBLOCK);
    CHECK(ret != -1) << "Error setting O_NONBLOCK " << strerror(errno);

    connection_callback(conn_fd);
  });
}

int BluetoothHci::ConnectToRemoteServer(const std::string& server, int port) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 1) {
    ALOGI("socket() call failed: %s", strerror(errno));
    return -1;
  }

  struct hostent* host;
  host = gethostbyname(server.c_str());
  if (host == NULL) {
    ALOGI("gethostbyname() failed for %s: %s", server.c_str(), strerror(errno));
    return -1;
  }

  struct sockaddr_in serv_addr;
  memset((void*)&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  int result = connect(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (result < 0) {
    ALOGI("connect() failed for %s@%d: %s", server.c_str(), port, strerror(errno));
    return -1;
  }

  int flags = fcntl(socket_fd, F_GETFL, NULL);
  int ret = fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
  CHECK(ret != -1) << "Error setting O_NONBLOCK " << strerror(errno);

  return socket_fd;
}

void BluetoothHci::SetUpTestChannel(int port) {
  int socket_fd = test_channel_transport_.SetUp(port);

  test_channel_.RegisterSendResponse(
      [](const std::string& response) { ALOGI("No test channel: %s", response.c_str()); });

  if (socket_fd == -1) {
    ALOGE("Test channel SetUp(%d) failed.", port);
    return;
  }

  ALOGI("Test channel SetUp() successful");
  async_manager_.WatchFdForNonBlockingReads(socket_fd, [this](int socket_fd) {
    int conn_fd = test_channel_transport_.Accept(socket_fd);
    if (conn_fd < 0) {
      ALOGE("Error watching test channel fd.");
      return;
    }
    ALOGI("Test channel connection accepted.");
    test_channel_.RegisterSendResponse(
        [this, conn_fd](const std::string& response) { test_channel_transport_.SendResponse(conn_fd, response); });

    async_manager_.WatchFdForNonBlockingReads(conn_fd, [this](int conn_fd) {
      test_channel_transport_.OnCommandReady(conn_fd,
                                             [this, conn_fd]() { async_manager_.StopWatchingFileDescriptor(conn_fd); });
    });
  });
}

/* Fallback to shared library if there is no service. */
IBluetoothHci* HIDL_FETCH_IBluetoothHci(const char* /* name */) {
  return new BluetoothHci();
}

}  // namespace sim
}  // namespace V1_0
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
