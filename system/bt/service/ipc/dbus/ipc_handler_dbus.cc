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

#include "service/ipc/dbus/ipc_handler_dbus.h"

#include <base/bind.h>
#include <base/threading/thread_task_runner_handle.h>
#include <dbus/bus.h>
#include "service/daemon.h"
#include "service/ipc/dbus/bluetooth_adapter.h"

using dbus::Bus;

namespace ipc {

IPCHandlerDBus::IPCHandlerDBus(bluetooth::Adapter* adapter,
                               IPCManager::Delegate* delegate)
    : IPCHandler(adapter, delegate) {}

IPCHandlerDBus::~IPCHandlerDBus() {}

bool IPCHandlerDBus::Run() {
  LOG(INFO) << __func__;

  dbus_thread_ = new base::Thread("D-Bus Thread");
  base::Thread::Options thread_options;
  thread_options.message_loop_type = base::MessageLoop::TYPE_IO;
  dbus_thread_->StartWithOptions(thread_options);

  dbus_thread_->task_runner()->PostTask(
      FROM_HERE, base::Bind(&IPCHandlerDBus::InitDbus, base::Unretained(this)));

  return true;
}

void IPCHandlerDBus::InitDbus() {
  LOG(INFO) << __func__;

  Bus::Options bus_options;
  bus_options.bus_type = Bus::SYSTEM;
  bus_options.connection_type = Bus::PRIVATE;
  bus_options.dbus_task_runner = base::ThreadTaskRunnerHandle::Get();

  scoped_refptr<Bus> bus_ = new Bus(bus_options);

  ipc::dbus::BluetoothAdapter* bluetooth_adapter =
      new ipc::dbus::BluetoothAdapter(bus_, adapter());

  LOG(INFO) << __func__ << ": all services added";
}

void IPCHandlerDBus::Stop() { dbus_thread_->Stop(); }

}  // namespace ipc
