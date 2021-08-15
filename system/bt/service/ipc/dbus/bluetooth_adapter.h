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

#include "service/adapter.h"

#include <base/memory/ref_counted.h>
#include <dbus/bus.h>
#include <dbus/exported_object.h>
#include <dbus/message.h>
#include <dbus/object_path.h>
#include <dbus/property.h>

using ::dbus::Bus;
using ::dbus::ExportedObject;
using ::dbus::MethodCall;

namespace ipc {
namespace dbus {

class BluetoothAdapter {
 public:
  explicit BluetoothAdapter(scoped_refptr<Bus> bus,
                            bluetooth::Adapter* adapter);
  virtual ~BluetoothAdapter();

  void Enable(MethodCall* method_call,
              ExportedObject::ResponseSender response_sender);

  void Disable(MethodCall* method_call,
               ExportedObject::ResponseSender response_sender);

  void Introspect(MethodCall* method_call,
                  ExportedObject::ResponseSender response_sender);

 private:
  ExportedObject* exported_object_;
  bluetooth::Adapter* adapter_;
};

}  // namespace dbus
}  // namespace ipc
