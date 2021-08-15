//
//  Copyright 2016 The Android Open Source Project
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
#pragma once

#include <base/threading/thread.h>
#include "service/ipc/ipc_handler.h"
#include "service/ipc/ipc_manager.h"

namespace base {
class SingleThreadTaskRunner;
}  // namespace base

namespace ipc {

// Implements a DBus based IPCHandler
class IPCHandlerDBus : public IPCHandler {
 public:
  IPCHandlerDBus(bluetooth::Adapter* adapter, IPCManager::Delegate* delegate);
  ~IPCHandlerDBus() override;

  void InitDbus();

  // IPCHandler overrides:
  bool Run() override;
  void Stop() override;

 private:
  base::Thread* dbus_thread_;

  IPCHandlerDBus() = default;

  DISALLOW_COPY_AND_ASSIGN(IPCHandlerDBus);
};

}  // namespace ipc
