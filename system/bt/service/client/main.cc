//
//  Copyright 2015 Google, Inc.
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

#include <iostream>
#include <string>

#include <base/at_exit.h>
#include <base/command_line.h>
#include <base/logging.h>
#include <base/macros.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#include <android/bluetooth/BnBluetoothCallback.h>
#include <android/bluetooth/BnBluetoothGattClientCallback.h>
#include <android/bluetooth/BnBluetoothLeAdvertiserCallback.h>
#include <android/bluetooth/BnBluetoothLeScannerCallback.h>
#include <android/bluetooth/BnBluetoothLowEnergyCallback.h>
#include <android/bluetooth/IBluetooth.h>
#include <android/bluetooth/IBluetoothGattClient.h>
#include <android/bluetooth/IBluetoothLeAdvertiser.h>
#include <android/bluetooth/IBluetoothLeScanner.h>
#include <android/bluetooth/IBluetoothLowEnergy.h>
#include <bluetooth/adapter_state.h>
#include <bluetooth/low_energy_constants.h>
#include <bluetooth/scan_filter.h>
#include <bluetooth/scan_settings.h>
#include <bluetooth/uuid.h>

using namespace std;

using android::sp;
using android::String8;
using android::String16;
using android::binder::Status;
using android::OK;
using android::getService;

using android::bluetooth::IBluetooth;
using android::bluetooth::IBluetoothGattClient;
using android::bluetooth::IBluetoothLeAdvertiser;
using android::bluetooth::IBluetoothLeScanner;
using android::bluetooth::IBluetoothLowEnergy;

namespace {

#define COLOR_OFF "\x1B[0m"
#define COLOR_RED "\x1B[0;91m"
#define COLOR_GREEN "\x1B[0;92m"
#define COLOR_YELLOW "\x1B[0;93m"
#define COLOR_BLUE "\x1B[0;94m"
#define COLOR_MAGENTA "\x1B[0;95m"
#define COLOR_BOLDGRAY "\x1B[1;30m"
#define COLOR_BOLDWHITE "\x1B[1;37m"
#define COLOR_BOLDYELLOW "\x1B[1;93m"
#define CLEAR_LINE "\x1B[2K"

#define CHECK_ARGS_COUNT(args, op, num, msg) \
  if (!((args).size() op num)) {             \
    PrintError(msg);                         \
    return;                                  \
  }
#define CHECK_NO_ARGS(args) \
  CHECK_ARGS_COUNT(args, ==, 0, "Expected no arguments")

// TODO(armansito): Clean up this code. Right now everything is in this
// monolithic file. We should organize this into different classes for command
// handling, console output/printing, callback handling, etc.
// (See http://b/23387611)

// Used to synchronize the printing of the command-line prompt and incoming
// Binder callbacks.
std::atomic_bool showing_prompt(false);

// The registered IBluetoothLowEnergy client handle. If |ble_registering| is
// true then an operation to register the client is in progress.
std::atomic_bool ble_registering(false);
std::atomic_int ble_client_id(0);

// The registered IBluetoothLeAdvertiser handle. If |ble_advertiser_registering|
// is true then an operation to register the advertiser is in progress.
const int invalid_advertiser_id = -1;
std::atomic_bool ble_advertiser_registering(false);
std::atomic_int ble_advertiser_id(invalid_advertiser_id);

// The registered IBluetoothLeScanner handle. If |ble_scanner_registering| is
// true then an operation to register the scanner is in progress.
std::atomic_bool ble_scanner_registering(false);
std::atomic_int ble_scanner_id(0);

// The registered IBluetoothGattClient client handle. If |gatt_registering| is
// true then an operation to register the client is in progress.
std::atomic_bool gatt_registering(false);
std::atomic_int gatt_client_id(0);

// True if we should dump the scan record bytes for incoming scan results.
std::atomic_bool dump_scan_record(false);

// True if the remote process has died and we should exit.
std::atomic_bool should_exit(false);

std::string kServiceName = "bluetooth-service";

void PrintPrompt() { cout << COLOR_BLUE "[FCLI] " COLOR_OFF << flush; }

void PrintError(const string& message) {
  cout << COLOR_RED << message << COLOR_OFF << endl;
}

void PrintOpStatus(const std::string& op, bool status) {
  cout << COLOR_BOLDWHITE << op << " status: " COLOR_OFF
       << (status ? (COLOR_GREEN "success") : (COLOR_RED "failure"))
       << COLOR_OFF << endl;
}

inline void BeginAsyncOut() {
  if (showing_prompt.load()) cout << CLEAR_LINE << "\r";
}

inline void EndAsyncOut() {
  std::flush(cout);
  if (showing_prompt.load())
    PrintPrompt();
  else
    cout << endl;
}

class CLIBluetoothCallback : public android::bluetooth::BnBluetoothCallback {
 public:
  CLIBluetoothCallback() = default;
  ~CLIBluetoothCallback() override = default;

  // IBluetoothCallback overrides:
  Status OnBluetoothStateChange(int32_t prev_state,
                                int32_t new_state) override {
    BeginAsyncOut();
    cout << COLOR_BOLDWHITE "Adapter state changed: " COLOR_OFF << COLOR_MAGENTA
         << AdapterStateToString(bluetooth::AdapterState(prev_state))
         << COLOR_OFF << COLOR_BOLDWHITE " -> " COLOR_OFF << COLOR_BOLDYELLOW
         << AdapterStateToString(bluetooth::AdapterState(new_state))
         << COLOR_OFF;
    EndAsyncOut();

    return Status::ok();
  }

  Status OnSspRequest(const String16& device_address,
                      const String16& device_name, int32_t cod,
                      int32_t pairing_variant, int32_t pass_key) override {
    // no-op
    return Status::ok();
  }

  Status OnGetBondedDevices(
      int32_t status,
      const ::std::vector<String16>& device_addresses) override {
    BeginAsyncOut();
    std::cout << "Bonded devices:\n";
    for (const auto& device_address : device_addresses) {
      std::cout << "    " << device_address << "\n";
    }
    EndAsyncOut();
    return Status::ok();
  }

  Status OnBondStateChanged(int32_t status, const String16& device_address,
                            int32_t state) override {
    BeginAsyncOut();
    std::cout << COLOR_BOLDWHITE "Device address: " << COLOR_BOLDYELLOW "["
              << device_address << " bond state: " << state << " ] "
              << COLOR_BOLDWHITE "- status: "
              << (status == 0 ? "SUCCESS" : "FAIL") << COLOR_OFF;
    EndAsyncOut();
    return Status::ok();
  }

  Status OnGetRemoteDeviceProperties(
      int32_t status, const String16& device_address,
      const android::bluetooth::BluetoothRemoteDeviceProps& props) override {
    // no-op
    return Status::ok();
  }

  Status OnDeviceFound(
      const android::bluetooth::BluetoothRemoteDeviceProps& props) override {
    // no-op
    return Status::ok();
  }

  Status OnDeviceConnectionStateChanged(const String16& device_address,
                                        bool connected) override {
    // no-op
    return Status::ok();
  }

  Status OnScanEnableChanged(bool scan_enabled) override {
    // no-op
    return Status::ok();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CLIBluetoothCallback);
};

class CLIBluetoothLowEnergyCallback
    : public android::bluetooth::BnBluetoothLowEnergyCallback {
 public:
  CLIBluetoothLowEnergyCallback() = default;
  ~CLIBluetoothLowEnergyCallback() override = default;

  // IBluetoothLowEnergyCallback overrides:
  Status OnClientRegistered(int status, int client_id) override {
    BeginAsyncOut();
    if (status != bluetooth::BLE_STATUS_SUCCESS) {
      PrintError("Failed to register BLE client");
    } else {
      ble_client_id = client_id;
      cout << COLOR_BOLDWHITE "Registered BLE client with ID: " COLOR_OFF
           << COLOR_GREEN << client_id << COLOR_OFF;
    }
    EndAsyncOut();

    ble_registering = false;
    return Status::ok();
  }

  Status OnConnectionState(int status, int client_id, const String16& address,
                           bool connected) override {
    BeginAsyncOut();
    cout << COLOR_BOLDWHITE "Connection state: " << COLOR_BOLDYELLOW "["
         << address << " connected: " << (connected ? "true" : "false") << " ] "
         << COLOR_BOLDWHITE "- status: " << status
         << COLOR_BOLDWHITE " - client_id: " << client_id << COLOR_OFF;
    EndAsyncOut();
    return Status::ok();
  }

  Status OnMtuChanged(int status, const String16& address, int mtu) override {
    BeginAsyncOut();
    cout << COLOR_BOLDWHITE "MTU changed: " << COLOR_BOLDYELLOW "[" << address
         << " ] " << COLOR_BOLDWHITE " - status: " << status
         << COLOR_BOLDWHITE " - mtu: " << mtu << COLOR_OFF;
    EndAsyncOut();
    return Status::ok();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CLIBluetoothLowEnergyCallback);
};

class CLIBluetoothLeAdvertiserCallback
    : public android::bluetooth::BnBluetoothLeAdvertiserCallback {
 public:
  CLIBluetoothLeAdvertiserCallback() = default;
  ~CLIBluetoothLeAdvertiserCallback() override = default;

  // IBluetoothLowEnergyCallback overrides:
  Status OnAdvertiserRegistered(int status, int advertiser_id) override {
    BeginAsyncOut();
    if (status != bluetooth::BLE_STATUS_SUCCESS) {
      PrintError("Failed to register BLE advertiser");
    } else {
      ble_advertiser_id = advertiser_id;
      cout << COLOR_BOLDWHITE "Registered BLE advertiser with ID: " COLOR_OFF
           << COLOR_GREEN << advertiser_id << COLOR_OFF;
    }
    EndAsyncOut();

    ble_advertiser_registering = false;
    return Status::ok();
  }

  Status OnMultiAdvertiseCallback(
      int status, bool is_start,
      const android::bluetooth::AdvertiseSettings& /* settings */) {
    BeginAsyncOut();
    std::string op = is_start ? "start" : "stop";

    PrintOpStatus("Advertising " + op, status == bluetooth::BLE_STATUS_SUCCESS);
    EndAsyncOut();
    return Status::ok();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CLIBluetoothLeAdvertiserCallback);
};

class CLIBluetoothLeScannerCallback
    : public android::bluetooth::BnBluetoothLeScannerCallback {
 public:
  CLIBluetoothLeScannerCallback() = default;
  ~CLIBluetoothLeScannerCallback() override = default;

  // IBluetoothLowEnergyCallback overrides:
  Status OnScannerRegistered(int status, int scanner_id) override {
    BeginAsyncOut();
    if (status != bluetooth::BLE_STATUS_SUCCESS) {
      PrintError("Failed to register BLE client");
    } else {
      ble_scanner_id = scanner_id;
      cout << COLOR_BOLDWHITE "Registered BLE client with ID: " COLOR_OFF
           << COLOR_GREEN << scanner_id << COLOR_OFF;
    }
    EndAsyncOut();

    ble_scanner_registering = false;
    return Status::ok();
  }

  Status OnScanResult(
      const android::bluetooth::ScanResult& scan_result) override {
    BeginAsyncOut();
    cout << COLOR_BOLDWHITE "Scan result: " << COLOR_BOLDYELLOW "["
         << scan_result.device_address() << "] "
         << COLOR_BOLDWHITE "- RSSI: " << scan_result.rssi() << COLOR_OFF;

    if (dump_scan_record) {
      cout << " - Record: "
           << base::HexEncode(scan_result.scan_record().data(),
                              scan_result.scan_record().size());
    }
    EndAsyncOut();
    return Status::ok();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CLIBluetoothLeScannerCallback);
};

class CLIGattClientCallback
    : public android::bluetooth::BnBluetoothGattClientCallback {
 public:
  CLIGattClientCallback() = default;
  ~CLIGattClientCallback() override = default;

  // IBluetoothGattClientCallback overrides:
  Status OnClientRegistered(int status, int client_id) override {
    BeginAsyncOut();
    if (status != bluetooth::BLE_STATUS_SUCCESS) {
      PrintError("Failed to register GATT client");
    } else {
      gatt_client_id = client_id;
      cout << COLOR_BOLDWHITE "Registered GATT client with ID: " COLOR_OFF
           << COLOR_GREEN << client_id << COLOR_OFF;
    }
    EndAsyncOut();

    gatt_registering = false;
    return Status::ok();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CLIGattClientCallback);
};

void PrintCommandStatus(bool status) { PrintOpStatus("Command", status); }

void PrintFieldAndValue(const string& field, const string& value) {
  cout << COLOR_BOLDWHITE << field << ": " << COLOR_BOLDYELLOW << value
       << COLOR_OFF << endl;
}

void PrintFieldAndBoolValue(const string& field, bool value) {
  PrintFieldAndValue(field, (value ? "true" : "false"));
}

void HandleDisable(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);
  bool status;
  bt_iface->Disable(&status);
  PrintCommandStatus(status);
}

void HandleEnable(IBluetooth* bt_iface, const vector<string>& args) {
  bool status;
  bt_iface->Enable(&status);
  PrintCommandStatus(status);
}

void HandleGetState(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  int32_t st;
  bt_iface->GetState(&st);
  bluetooth::AdapterState state = static_cast<bluetooth::AdapterState>(st);
  PrintFieldAndValue("Adapter state", bluetooth::AdapterStateToString(state));
}

void HandleIsEnabled(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);
  bool enabled;
  bt_iface->IsEnabled(&enabled);
  PrintFieldAndBoolValue("Adapter enabled", enabled);
}

void HandleGetLocalAddress(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);
  String16 address;
  bt_iface->GetAddress(&address);
  PrintFieldAndValue("Adapter address", std::string(String8(address).string()));
}

void HandleSetLocalName(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_ARGS_COUNT(args, >=, 1, "No name was given");

  std::string name;
  for (const auto& arg : args) name += arg + " ";

  base::TrimWhitespaceASCII(name, base::TRIM_TRAILING, &name);

  bool status;
  bt_iface->SetName(String16(String8(name.c_str())), &status);
  PrintCommandStatus(status);
}

void HandleGetLocalName(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);
  String16 name;
  bt_iface->GetName(&name);
  PrintFieldAndValue("Adapter name", std::string(String8(name).string()));
}

void HandleAdapterInfo(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  cout << COLOR_BOLDWHITE "Adapter Properties: " COLOR_OFF << endl;

  String16 address;
  bt_iface->GetAddress(&address);
  PrintFieldAndValue("\tAddress", std::string(String8(address).string()));

  int adapter_state;
  bt_iface->GetState(&adapter_state);
  PrintFieldAndValue("\tState",
                     bluetooth::AdapterStateToString(
                         static_cast<bluetooth::AdapterState>(adapter_state)));

  String16 name;
  bt_iface->GetName(&name);
  PrintFieldAndValue("\tName", std::string(String8(name).string()));

  bool multi_adv;
  bt_iface->IsMultiAdvertisementSupported(&multi_adv);
  PrintFieldAndBoolValue("\tMulti-Adv. supported", multi_adv);
}

void HandleSupportsMultiAdv(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  bool multi_adv;
  bt_iface->IsMultiAdvertisementSupported(&multi_adv);
  PrintFieldAndBoolValue("Multi-advertisement support", multi_adv);
}

void HandleRegisterBLEAdvertiser(IBluetooth* bt_iface,
                                 const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (ble_advertiser_registering.load()) {
    PrintError("In progress");
    return;
  }

  if (ble_advertiser_id.load() != invalid_advertiser_id) {
    PrintError("Already registered");
    return;
  }

  sp<IBluetoothLeAdvertiser> ble_advertiser_iface;
  bt_iface->GetLeAdvertiserInterface(&ble_advertiser_iface);
  if (!ble_advertiser_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Le Advertiser interface");
    return;
  }

  bool status;
  ble_advertiser_iface->RegisterAdvertiser(
      new CLIBluetoothLeAdvertiserCallback(), &status);
  ble_advertiser_registering = status;
  PrintCommandStatus(status);
}

void HandleUnregisterBLEAdvertiser(IBluetooth* bt_iface,
                                   const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (ble_advertiser_id.load() == invalid_advertiser_id) {
    PrintError("Not registered");
    return;
  }

  sp<IBluetoothLeAdvertiser> ble_advertiser_iface;
  bt_iface->GetLeAdvertiserInterface(&ble_advertiser_iface);
  if (!ble_advertiser_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  ble_advertiser_iface->UnregisterAdvertiser(ble_advertiser_id.load());
  ble_advertiser_id = invalid_advertiser_id;
  PrintCommandStatus(true);
}

void HandleRegisterBLE(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (ble_registering.load()) {
    PrintError("In progress");
    return;
  }

  if (ble_client_id.load()) {
    PrintError("Already registered");
    return;
  }

  sp<IBluetoothLowEnergy> ble_iface;
  bt_iface->GetLowEnergyInterface(&ble_iface);
  if (!ble_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  bool status;
  ble_iface->RegisterClient(new CLIBluetoothLowEnergyCallback(), &status);
  ble_registering = status;
  PrintCommandStatus(status);
}

void HandleUnregisterBLE(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (!ble_client_id.load()) {
    PrintError("Not registered");
    return;
  }

  sp<IBluetoothLowEnergy> ble_iface;
  bt_iface->GetLowEnergyInterface(&ble_iface);
  if (!ble_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  ble_iface->UnregisterClient(ble_client_id.load());
  ble_client_id = 0;
  PrintCommandStatus(true);
}

void HandleUnregisterAllBLE(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  sp<IBluetoothLowEnergy> ble_iface;
  bt_iface->GetLowEnergyInterface(&ble_iface);
  if (!ble_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  ble_iface->UnregisterAll();
  PrintCommandStatus(true);
}

void HandleRegisterGATT(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (gatt_registering.load()) {
    PrintError("In progress");
    return;
  }

  if (gatt_client_id.load()) {
    PrintError("Already registered");
    return;
  }

  sp<IBluetoothGattClient> gatt_iface;
  bt_iface->GetGattClientInterface(&gatt_iface);
  if (!gatt_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth GATT Client interface");
    return;
  }

  bool status;
  gatt_iface->RegisterClient(new CLIGattClientCallback(), &status);
  gatt_registering = status;
  PrintCommandStatus(status);
}

void HandleUnregisterGATT(IBluetooth* bt_iface, const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (!gatt_client_id.load()) {
    PrintError("Not registered");
    return;
  }

  sp<IBluetoothGattClient> gatt_iface;
  bt_iface->GetGattClientInterface(&gatt_iface);
  if (!gatt_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth GATT Client interface");
    return;
  }

  gatt_iface->UnregisterClient(gatt_client_id.load());
  gatt_client_id = 0;
  PrintCommandStatus(true);
}

void HandleStartAdv(IBluetooth* bt_iface, const vector<string>& args) {
  bool include_name = false;
  bool include_tx_power = false;
  bool connectable = false;
  bool set_manufacturer_data = false;
  bool set_uuid = false;
  bluetooth::Uuid uuid;

  for (auto iter = args.begin(); iter != args.end(); ++iter) {
    const std::string& arg = *iter;
    if (arg == "-n")
      include_name = true;
    else if (arg == "-t")
      include_tx_power = true;
    else if (arg == "-c")
      connectable = true;
    else if (arg == "-m")
      set_manufacturer_data = true;
    else if (arg == "-u") {
      // This flag has a single argument.
      ++iter;
      if (iter == args.end()) {
        PrintError("Expected a Uuid after -u");
        return;
      }

      std::string uuid_str = *iter;
      bool is_valid = false;
      uuid = bluetooth::Uuid::FromString(uuid_str, &is_valid);
      if (!is_valid) {
        PrintError("Invalid Uuid: " + uuid_str);
        return;
      }

      set_uuid = true;
    } else if (arg == "-h") {
      static const char kUsage[] =
          "Usage: start-adv [flags]\n"
          "\n"
          "Flags:\n"
          "\t-n\tInclude device name\n"
          "\t-t\tInclude TX power\n"
          "\t-c\tSend connectable adv. packets (default is non-connectable)\n"
          "\t-m\tInclude random manufacturer data\n"
          "\t-h\tShow this help message\n";
      cout << kUsage << endl;
      return;
    } else {
      PrintError("Unrecognized option: " + arg);
      return;
    }
  }

  if (ble_advertiser_id.load() == invalid_advertiser_id) {
    PrintError("BLE advertiser not registered");
    return;
  }

  sp<IBluetoothLeAdvertiser> ble_advertiser_iface;
  bt_iface->GetLeAdvertiserInterface(&ble_advertiser_iface);
  if (!ble_advertiser_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Le Advertiser interface");
    return;
  }

  std::vector<uint8_t> data;
  if (set_manufacturer_data) {
    data = {{0x07, bluetooth::kEIRTypeManufacturerSpecificData, 0xe0, 0x00, 'T',
             'e', 's', 't'}};
  }

  if (set_uuid) {
    // Determine the type and length bytes.
    int uuid_size = uuid.GetShortestRepresentationSize();
    uint8_t type;
    if (uuid_size == bluetooth::Uuid::kNumBytes128)
      type = bluetooth::kEIRTypeComplete128BitUuids;
    else if (uuid_size == bluetooth::Uuid::kNumBytes32)
      type = bluetooth::kEIRTypeComplete32BitUuids;
    else if (uuid_size == bluetooth::Uuid::kNumBytes16)
      type = bluetooth::kEIRTypeComplete16BitUuids;
    else
      NOTREACHED() << "Unexpected size: " << uuid_size;

    data.push_back(uuid_size + 1);
    data.push_back(type);

    auto uuid_bytes = uuid.To128BitLE();
    int index = (uuid_size == 16) ? 0 : 12;
    data.insert(data.end(), uuid_bytes.data() + index,
                uuid_bytes.data() + index + uuid_size);
  }

  base::TimeDelta timeout;

  bluetooth::AdvertiseSettings settings(
      bluetooth::AdvertiseSettings::MODE_LOW_POWER, timeout,
      bluetooth::AdvertiseSettings::TX_POWER_LEVEL_MEDIUM, connectable);

  if (include_tx_power) {
    data.push_back(0x02);
    data.push_back(bluetooth::kEIRTypeTxPower);
    data.push_back(0x00);
  }

  bluetooth::AdvertiseData adv_data(data);

  if (include_name) {
    String16 name_param;
    bt_iface->GetName(&name_param);
    std::string name(String8(name_param).string());
    data.push_back(name.length() + 1);
    data.push_back(bluetooth::kEIRTypeCompleteLocalName);
    data.insert(data.begin(), name.c_str(), name.c_str() + name.length());
  }

  bluetooth::AdvertiseData scan_rsp;

  bool status;
  ble_advertiser_iface->StartMultiAdvertising(
      ble_advertiser_id.load(), adv_data, scan_rsp, settings, &status);
  PrintCommandStatus(status);
}

void HandleStopAdv(IBluetooth* bt_iface, const vector<string>& args) {
  if (ble_advertiser_id.load() == invalid_advertiser_id) {
    PrintError("BLE advertiser not registered");
    return;
  }

  sp<IBluetoothLeAdvertiser> ble_advertiser_iface;
  bt_iface->GetLeAdvertiserInterface(&ble_advertiser_iface);
  if (!ble_advertiser_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  bool status;
  ble_advertiser_iface->StopMultiAdvertising(ble_advertiser_id.load(), &status);
  PrintCommandStatus(status);
}

void HandleConnect(IBluetooth* bt_iface, const vector<string>& args) {
  string address;

  if (args.size() != 1) {
    PrintError("Expected MAC address as only argument");
    return;
  }

  address = args[0];

  if (!ble_client_id.load()) {
    PrintError("BLE not registered");
    return;
  }

  sp<IBluetoothLowEnergy> ble_iface;
  bt_iface->GetLowEnergyInterface(&ble_iface);
  if (!ble_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  bool status;
  ble_iface->Connect(ble_client_id.load(),
                     String16(address.c_str(), address.length()),
                     false /*  is_direct */, &status);

  PrintCommandStatus(status);
}

void HandleDisconnect(IBluetooth* bt_iface, const vector<string>& args) {
  string address;

  if (args.size() != 1) {
    PrintError("Expected MAC address as only argument");
    return;
  }

  address = args[0];

  if (!ble_client_id.load()) {
    PrintError("BLE not registered");
    return;
  }

  sp<IBluetoothLowEnergy> ble_iface;
  bt_iface->GetLowEnergyInterface(&ble_iface);
  if (!ble_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  bool status;
  ble_iface->Disconnect(ble_client_id.load(),
                        String16(address.c_str(), address.length()), &status);
  PrintCommandStatus(status);
}

void HandleSetMtu(IBluetooth* bt_iface, const vector<string>& args) {
  string address;
  int mtu;

  if (args.size() != 2) {
    PrintError("Usage: set-mtu [address] [mtu]");
    return;
  }

  address = args[0];
  mtu = std::stoi(args[1]);

  if (mtu < 23) {
    PrintError("MTU must be 23 or larger");
    return;
  }

  if (!ble_client_id.load()) {
    PrintError("BLE not registered");
    return;
  }

  sp<IBluetoothLowEnergy> ble_iface;
  bt_iface->GetLowEnergyInterface(&ble_iface);
  if (!ble_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth Low Energy interface");
    return;
  }

  bool status;
  ble_iface->SetMtu(ble_client_id.load(),
                    String16(address.c_str(), address.length()), mtu, &status);
  PrintCommandStatus(status);
}

void HandleRegisterBLEScanner(IBluetooth* bt_iface,
                              const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (ble_scanner_registering.load()) {
    PrintError("In progress");
    return;
  }

  if (ble_scanner_id.load()) {
    PrintError("Already registered");
    return;
  }

  sp<IBluetoothLeScanner> ble_scanner_iface;
  bt_iface->GetLeScannerInterface(&ble_scanner_iface);
  if (!ble_scanner_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth LE Scanner interface");
    return;
  }

  bool status;
  ble_scanner_iface->RegisterScanner(new CLIBluetoothLeScannerCallback(),
                                     &status);
  ble_scanner_registering = status;
  PrintCommandStatus(status);
}

void HandleUnregisterBLEScanner(IBluetooth* bt_iface,
                                const vector<string>& args) {
  CHECK_NO_ARGS(args);

  if (!ble_scanner_id.load()) {
    PrintError("Not registered");
    return;
  }

  sp<IBluetoothLeScanner> ble_scanner_iface;
  bt_iface->GetLeScannerInterface(&ble_scanner_iface);
  if (!ble_scanner_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth LE scanner interface");
    return;
  }

  ble_scanner_iface->UnregisterScanner(ble_scanner_id.load());
  ble_scanner_id = 0;
  PrintCommandStatus(true);
}

void HandleStartLeScan(IBluetooth* bt_iface, const vector<string>& args) {
  if (!ble_client_id.load()) {
    PrintError("BLE not registered");
    return;
  }

  for (const auto& arg : args) {
    if (arg == "-d") {
      dump_scan_record = true;
    } else if (arg == "-h") {
      static const char kUsage[] =
          "Usage: start-le-scan [flags]\n"
          "\n"
          "Flags:\n"
          "\t-d\tDump scan record\n"
          "\t-h\tShow this help message\n";
      cout << kUsage << endl;
      return;
    }
  }

  sp<IBluetoothLeScanner> ble_scanner_iface;
  bt_iface->GetLeScannerInterface(&ble_scanner_iface);
  if (!ble_scanner_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth LE scanner interface");
    return;
  }

  bluetooth::ScanSettings settings;
  std::vector<android::bluetooth::ScanFilter> filters;

  bool status;
  ble_scanner_iface->StartScan(ble_scanner_id.load(), settings, filters,
                               &status);
  PrintCommandStatus(status);
}

void HandleStopLeScan(IBluetooth* bt_iface, const vector<string>& args) {
  if (!ble_client_id.load()) {
    PrintError("BLE not registered");
    return;
  }

  sp<IBluetoothLeScanner> ble_scanner_iface;
  bt_iface->GetLeScannerInterface(&ble_scanner_iface);
  if (!ble_scanner_iface.get()) {
    PrintError("Failed to obtain handle to Bluetooth LE scanner interface");
    return;
  }

  bool status;
  ble_scanner_iface->StopScan(ble_scanner_id.load(), &status);
  PrintCommandStatus(status);
}

void HandleHelp(IBluetooth* bt_iface, const vector<string>& args);

struct {
  string command;
  void (*func)(IBluetooth*, const vector<string>& args);
  string help;
} kCommandMap[] = {
    {"help", HandleHelp, "\t\t\tDisplay this message"},
    {"disable", HandleDisable, "\t\t\tDisable Bluetooth"},
    {"enable", HandleEnable, "\t\t\tEnable Bluetooth (-h for options)"},
    {"get-state", HandleGetState, "\t\tGet the current adapter state"},
    {"is-enabled", HandleIsEnabled, "\t\tReturn if Bluetooth is enabled"},
    {"get-local-address", HandleGetLocalAddress,
     "\tGet the local adapter address"},
    {"set-local-name", HandleSetLocalName, "\t\tSet the local adapter name"},
    {"get-local-name", HandleGetLocalName, "\t\tGet the local adapter name"},
    {"adapter-info", HandleAdapterInfo, "\t\tPrint adapter properties"},
    {"supports-multi-adv", HandleSupportsMultiAdv,
     "\tWhether multi-advertisement is currently supported"},
    {"register-le-advertiser", HandleRegisterBLEAdvertiser,
     "\t\tRegister with the Bluetooth Low Energy Advertiser interface"},
    {"unregister-le-advertiser", HandleUnregisterBLEAdvertiser,
     "\t\tUnregister from the Bluetooth LE Advertiser interface"},
    {"register-ble", HandleRegisterBLE,
     "\t\tRegister with the Bluetooth Low Energy interface"},
    {"unregister-ble", HandleUnregisterBLE,
     "\t\tUnregister from the Bluetooth Low Energy interface"},
    {"unregister-all-ble", HandleUnregisterAllBLE,
     "\tUnregister all clients from the Bluetooth Low Energy interface"},
    {"register-gatt", HandleRegisterGATT,
     "\t\tRegister with the Bluetooth GATT Client interface"},
    {"unregister-gatt", HandleUnregisterGATT,
     "\t\tUnregister from the Bluetooth GATT Client interface"},
    {"connect-le", HandleConnect, "\t\tConnect to LE device (-h for options)"},
    {"disconnect-le", HandleDisconnect,
     "\t\tDisconnect LE device (-h for options)"},
    {"set-mtu", HandleSetMtu, "\t\tSet MTU (-h for options)"},
    {"start-adv", HandleStartAdv, "\t\tStart advertising (-h for options)"},
    {"stop-adv", HandleStopAdv, "\t\tStop advertising"},
    {"register-le-scanner", HandleRegisterBLEScanner,
     "\t\tRegister with the Bluetooth Low Energy scanner interface"},
    {"unregister-le-scanner", HandleUnregisterBLEScanner,
     "\t\tUnregister from the Bluetooth LE scanner interface"},
    {"start-le-scan", HandleStartLeScan,
     "\t\tStart LE device scan (-h for options)"},
    {"stop-le-scan", HandleStopLeScan, "\t\tStop LE device scan"},
    {},
};

void HandleHelp(IBluetooth* /* bt_iface */, const vector<string>& /* args */) {
  cout << endl;
  for (int i = 0; kCommandMap[i].func; i++)
    cout << "\t" << kCommandMap[i].command << kCommandMap[i].help << endl;
  cout << endl;
}

const char kExecuteLong[] = "exec";
const char kExecuteShort[] = "e";

bool ExecuteCommand(const sp<IBluetooth>& bt_iface, std::string& command) {
  vector<string> args = base::SplitString(command, " ", base::TRIM_WHITESPACE,
                                          base::SPLIT_WANT_ALL);

  if (args.empty()) return true;

  // The first argument is the command while the remaining are what we pass to
  // the handler functions.
  command = args[0];
  args.erase(args.begin());

  for (int i = 0; kCommandMap[i].func; i++) {
    if (command == kCommandMap[i].command) {
      kCommandMap[i].func(bt_iface.get(), args);
      return true;
    }
  }

  cout << "Unrecognized command: " << command << endl;
  return false;
}

}  // namespace

class BluetoothDeathRecipient : public android::IBinder::DeathRecipient {
 public:
  BluetoothDeathRecipient() = default;
  ~BluetoothDeathRecipient() override = default;

  // android::IBinder::DeathRecipient override:
  void binderDied(const android::wp<android::IBinder>& /* who */) override {
    BeginAsyncOut();
    cout << COLOR_BOLDWHITE "The Bluetooth daemon has died" COLOR_OFF << endl;
    cout << "\nPress 'ENTER' to exit.";
    EndAsyncOut();

    android::IPCThreadState::self()->stopProcess();
    should_exit = true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothDeathRecipient);
};

int main(int argc, char* argv[]) {
  base::AtExitManager exit_manager;
  base::CommandLine::Init(argc, argv);
  logging::LoggingSettings log_settings;

  if (!logging::InitLogging(log_settings)) {
    LOG(ERROR) << "Failed to set up logging";
    return EXIT_FAILURE;
  }

  sp<IBluetooth> bt_iface;
  status_t status = getService(String16(kServiceName.c_str()), &bt_iface);
  if (status != OK) {
    LOG(ERROR) << "Failed to get service binder: '" << kServiceName
               << "' status=" << status;
    return EXIT_FAILURE;
  }

  sp<BluetoothDeathRecipient> dr(new BluetoothDeathRecipient());
  if (android::IInterface::asBinder(bt_iface.get())->linkToDeath(dr) !=
      android::NO_ERROR) {
    LOG(ERROR) << "Failed to register DeathRecipient for IBluetooth";
    return EXIT_FAILURE;
  }

  // Initialize the Binder process thread pool. We have to set this up,
  // otherwise, incoming callbacks from IBluetoothCallback will block the main
  // thread (in other words, we have to do this as we are a "Binder server").
  android::ProcessState::self()->startThreadPool();

  // Register Adapter state-change callback
  sp<CLIBluetoothCallback> callback = new CLIBluetoothCallback();
  bt_iface->RegisterCallback(callback);

  cout << COLOR_BOLDWHITE << "Fluoride Command-Line Interface\n"
       << COLOR_OFF << endl
       << "Type \"help\" to see possible commands.\n"
       << endl;

  string command;

  // Add commands from the command line, if they exist.
  auto command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(kExecuteLong)) {
    command += command_line->GetSwitchValueASCII(kExecuteLong);
  }

  if (command_line->HasSwitch(kExecuteShort)) {
    if (!command.empty()) command += " ; ";
    command += command_line->GetSwitchValueASCII(kExecuteShort);
  }

  while (true) {
    vector<string> commands = base::SplitString(
        command, ";", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    for (string command : commands) {
      if (!ExecuteCommand(bt_iface, command)) break;
    }

    commands.clear();

    PrintPrompt();

    showing_prompt = true;
    auto& istream = getline(cin, command);
    showing_prompt = false;

    if (istream.eof() || should_exit.load()) {
      cout << "\nExiting" << endl;
      return EXIT_SUCCESS;
    }

    if (!istream.good()) {
      LOG(ERROR) << "An error occured while reading input";
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
