/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <unistd.h>
#include <sys/capability.h>

#include <csignal>
#include <memory>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <cutils/properties.h>
#include <hidl/HidlTransportSupport.h>
#include <libminijail.h>
#include <utils/String16.h>
#include <wifi_system/interface_tool.h>

#include "wificond/ipc_constants.h"
#include "wificond/looper_backed_event_loop.h"
#include "wificond/net/netlink_manager.h"
#include "wificond/net/netlink_utils.h"
#include "wificond/scanning/scan_utils.h"
#include "wificond/server.h"

using android::net::wifi::IWificond;
using android::wifi_system::InterfaceTool;
using android::wificond::ipc_constants::kServiceName;
using std::unique_ptr;

namespace {

class ScopedSignalHandler final {
 public:
  ScopedSignalHandler(android::wificond::LooperBackedEventLoop* event_loop) {
    if (s_event_loop_ != nullptr) {
      LOG(FATAL) << "Only instantiate one signal handler per process!";
    }
    s_event_loop_ = event_loop;
    std::signal(SIGINT, &ScopedSignalHandler::LeaveLoop);
    std::signal(SIGTERM, &ScopedSignalHandler::LeaveLoop);
  }

  ~ScopedSignalHandler() {
    std::signal(SIGINT, SIG_DFL);
    std::signal(SIGTERM, SIG_DFL);
    s_event_loop_ = nullptr;
  }

 private:
  static android::wificond::LooperBackedEventLoop* s_event_loop_;
  static void LeaveLoop(int signal) {
    if (s_event_loop_ != nullptr) {
      s_event_loop_->TriggerExit();
    }
  }

  DISALLOW_COPY_AND_ASSIGN(ScopedSignalHandler);
};

android::wificond::LooperBackedEventLoop*
    ScopedSignalHandler::s_event_loop_ = nullptr;


// Setup our interface to the Binder driver or die trying.
int SetupBinderOrCrash() {
  int binder_fd = -1;
  android::ProcessState::self()->setThreadPoolMaxThreadCount(0);
  android::IPCThreadState::self()->disableBackgroundScheduling(true);
  int err = android::IPCThreadState::self()->setupPolling(&binder_fd);
  CHECK_EQ(err, 0) << "Error setting up binder polling: " << strerror(-err);
  CHECK_GE(binder_fd, 0) << "Invalid binder FD: " << binder_fd;
  return binder_fd;
}

// Setup our interface to the hw Binder driver or die trying.
int SetupHwBinderOrCrash() {
  android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);
  int binder_fd  = android::hardware::setupTransportPolling();
  CHECK_GE(binder_fd, 0) << "Invalid hw binder FD: " << binder_fd;
  return binder_fd;
}

void RegisterServiceOrCrash(const android::sp<android::IBinder>& service) {
  android::sp<android::IServiceManager> sm = android::defaultServiceManager();
  CHECK_EQ(sm != NULL, true) << "Could not obtain IServiceManager";

  CHECK_EQ(sm->addService(android::String16(kServiceName), service),
           android::NO_ERROR);
}

}  // namespace

void OnBinderReadReady(int fd) {
  android::IPCThreadState::self()->handlePolledCommands();
}

void OnHwBinderReadReady(int fd) {
  android::hardware::handleTransportPoll(fd);
}

int main(int argc, char** argv) {
  android::base::InitLogging(argv, android::base::LogdLogger(android::base::SYSTEM));
  LOG(INFO) << "wificond is starting up...";

  unique_ptr<android::wificond::LooperBackedEventLoop> event_dispatcher(
      new android::wificond::LooperBackedEventLoop());
  ScopedSignalHandler scoped_signal_handler(event_dispatcher.get());

  int binder_fd = SetupBinderOrCrash();
  CHECK(event_dispatcher->WatchFileDescriptor(
      binder_fd,
      android::wificond::EventLoop::kModeInput,
      &OnBinderReadReady)) << "Failed to watch binder FD";

  int hw_binder_fd = SetupHwBinderOrCrash();
  CHECK(event_dispatcher->WatchFileDescriptor(
      hw_binder_fd, android::wificond::EventLoop::kModeInput,
      &OnHwBinderReadReady)) << "Failed to watch Hw Binder FD";

  android::wificond::NetlinkManager netlink_manager(event_dispatcher.get());
  if (!netlink_manager.Start()) {
    LOG(ERROR) << "Failed to start netlink manager";
  }
  android::wificond::NetlinkUtils netlink_utils(&netlink_manager);
  android::wificond::ScanUtils scan_utils(&netlink_manager);

  unique_ptr<android::wificond::Server> server(new android::wificond::Server(
      unique_ptr<InterfaceTool>(new InterfaceTool),
      &netlink_utils,
      &scan_utils));
  RegisterServiceOrCrash(server.get());

  event_dispatcher->Poll();
  LOG(INFO) << "wificond is about to exit";
  return 0;
}
