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

#define LOG_TAG "root_canal"

#include "test_environment.h"

#include <base/logging.h>
#include <utils/Log.h>
#include <future>

#include "hci_internals.h"

using ::android::bluetooth::root_canal::TestEnvironment;

constexpr uint16_t kTestPort = 6401;
constexpr uint16_t kHciServerPort = 6402;
constexpr uint16_t kLinkServerPort = 6403;

int main(int argc, char** argv) {
  ALOGI("main");
  uint16_t test_port = kTestPort;
  uint16_t hci_server_port = kHciServerPort;
  uint16_t link_server_port = kLinkServerPort;

  for (int arg = 0; arg < argc; arg++) {
    int port = atoi(argv[arg]);
    ALOGI("%d: %s (%d)", arg, argv[arg], port);
    if (port < 0 || port > 0xffff) {
      ALOGW("%s out of range", argv[arg]);
    } else {
      switch (arg) {
        case 0:  // executable name
          break;
        case 1:
          test_port = port;
          break;
        case 2:
          hci_server_port = port;
          break;
        case 3:
          link_server_port = port;
          break;
        default:
          ALOGW("Ignored option %s", argv[arg]);
      }
    }
  }

  TestEnvironment root_canal(test_port, hci_server_port, link_server_port);
  std::promise<void> barrier;
  std::future<void> barrier_future = barrier.get_future();
  root_canal.initialize(std::move(barrier));
  barrier_future.wait();
  root_canal.close();
}
