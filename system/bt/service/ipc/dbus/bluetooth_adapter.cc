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

#include "service/ipc/dbus/bluetooth_adapter.h"
#include <base/files/file_util.h>
#include <base/logging.h>
#include "service/hal/bluetooth_interface.h"

using ::dbus::Bus;
using ::dbus::ExportedObject;
using ::dbus::MethodCall;
using ::dbus::MessageWriter;
using ::dbus::Response;
using ::dbus::ObjectPath;
using ::dbus::ErrorResponse;

namespace {

const std::string kBluetoothAdapterInterface = "org.fluoride.BluetoothAdapter";
const std::string kEnable = "Enable";
const std::string kDisable = "Disable";
const std::string kBluetoothAdapter = "org.fluoride.BluetoothAdapter";
const std::string kBluetoothAdapterPath = "/org/fluoride/BluetoothAdapter";

// TODO(jpawlowski): right now xml interface files are in service/ipc/dbus/
// folder.  Make a script to move them into /usr/share/dbus-1/interfaces
const char kBindingsPath[] =
    "/usr/share/dbus-1/interfaces/org.fluoride.BluetoothAdapter.xml";
const char kDBusIntrospectMethod[] = "Introspect";

}  // namespace

namespace ipc {
namespace dbus {

BluetoothAdapter::BluetoothAdapter(scoped_refptr<Bus> bus,
                                   bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  exported_object_ = bus->GetExportedObject(ObjectPath(kBluetoothAdapterPath));

  CHECK(exported_object_->ExportMethodAndBlock(
      kBluetoothAdapterInterface, kEnable,
      base::Bind(&BluetoothAdapter::Enable, base::Unretained(this))));

  CHECK(exported_object_->ExportMethodAndBlock(
      kBluetoothAdapterInterface, kDisable,
      base::Bind(&BluetoothAdapter::Disable, base::Unretained(this))));

  CHECK(exported_object_->ExportMethodAndBlock(
      DBUS_INTERFACE_INTROSPECTABLE, kDBusIntrospectMethod,
      base::Bind(&BluetoothAdapter::Introspect, base::Unretained(this))));

  CHECK(bus->RequestOwnershipAndBlock(kBluetoothAdapter, Bus::REQUIRE_PRIMARY))
      << "Unable to take ownership of " << kBluetoothAdapter
      << ". Make sure you have proper busconfig file "
         "/etc/dbus-1/system.d/org.fluoride.conf";
}

void BluetoothAdapter::Enable(MethodCall* method_call,
                              ExportedObject::ResponseSender response_sender) {
  VLOG(1) << __func__;
  adapter_->Enable(false);
  response_sender.Run(Response::FromMethodCall(method_call));
}

void BluetoothAdapter::Disable(MethodCall* method_call,
                               ExportedObject::ResponseSender response_sender) {
  VLOG(1) << __func__;
  adapter_->Disable();
  response_sender.Run(Response::FromMethodCall(method_call));
}

void BluetoothAdapter::Introspect(
    MethodCall* method_call, ExportedObject::ResponseSender response_sender) {
  VLOG(1) << __func__;

  std::string output;
  if (!base::ReadFileToString(base::FilePath(kBindingsPath), &output)) {
    PLOG(ERROR) << "Can't read XML bindings from disk:";
    response_sender.Run(ErrorResponse::FromMethodCall(
        method_call, "Can't read XML bindings from disk.", ""));
  }
  std::unique_ptr<Response> response(Response::FromMethodCall(method_call));
  MessageWriter writer(response.get());
  writer.AppendString(output);

  response_sender.Run(std::move(response));
}

BluetoothAdapter::~BluetoothAdapter() {}

}  // namespace dbus
}  // namespace ipc
