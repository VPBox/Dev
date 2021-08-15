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

#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

#include <memory>
#include <string>

#include "android-base/macros.h"

#include "wifilogd/command_processor.h"
#include "wifilogd/os.h"

namespace android {
namespace wifilogd {

// The main event loop for wifilogd.
class MainLoop {
 public:
  explicit MainLoop(const std::string& socket_name);
  MainLoop(const std::string& socket_name, std::unique_ptr<Os> os,
           std::unique_ptr<CommandProcessor> command_processor);

  // Runs one iteration of the loop.
  void RunOnce();

 private:
  void ProcessError(Os::Errno err);

  std::unique_ptr<Os> os_;
  std::unique_ptr<CommandProcessor> command_processor_;
  // We use an int, rather than a unique_fd, because the file
  // descriptor's lifetime is managed by init. (init creates
  // the socket before forking our process.)
  int sock_fd_;

  DISALLOW_COPY_AND_ASSIGN(MainLoop);
};

}  // namespace wifilogd
}  // namespace android

#endif  // MAIN_LOOP_H_
