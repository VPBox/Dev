/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "chre/core/event_loop_manager.h"
#include "chre/core/init.h"
#include "chre/core/static_nanoapps.h"
#include "chre/platform/android/host_link.h"
#include "chre/platform/shared/platform_log.h"
#include "chre_host/host_protocol_host.h"

#include <csignal>
#include <thread>

using android::chre::HostProtocolHost;
using chre::EventLoopManagerSingleton;

namespace {

void onMessageReceivedFromClient(uint16_t clientId, void *data, size_t length) {
  if (!HostProtocolHost::mutateHostClientId(data, length, clientId)) {
    LOGE("Couldn't set host client ID in message container!");
  } else {
    LOGD("Delivering message from host (size %zu)", length);
    if (!chre::handleMessageFromHost(data, length)) {
      LOGE("Failed to decode message from host");
    }
  }
}

}

int main(int argc, char **argv) {
  // Initilize CHRE.
  chre::init();
  chre::loadStaticNanoapps();

  // Initialize the socket server.
  chre::SocketServerSingleton::init();

  // Setup the socket server for communications with the HAL.
  std::thread socketServerThread([&]() {
    chre::SocketServerSingleton::get()->run(
        "chre", true, onMessageReceivedFromClient);
    EventLoopManagerSingleton::get()->getEventLoop().stop();
  });

  EventLoopManagerSingleton::get()->getEventLoop().run();

  chre::deinit();
  return 0;
}
