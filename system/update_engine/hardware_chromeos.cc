//
// Copyright (C) 2013 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/hardware_chromeos.h"

#include <utility>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <brillo/key_value_store.h>
#include <debugd/dbus-constants.h>
#include <vboot/crossystem.h>

extern "C" {
#include "vboot/vboot_host.h"
}

#include "update_engine/common/constants.h"
#include "update_engine/common/hardware.h"
#include "update_engine/common/hwid_override.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/subprocess.h"
#include "update_engine/common/utils.h"
#include "update_engine/dbus_connection.h"

using std::string;
using std::vector;

namespace {

const char kOOBECompletedMarker[] = "/home/chronos/.oobe_completed";

// The stateful directory used by update_engine to store powerwash-safe files.
// The files stored here must be whitelisted in the powerwash scripts.
const char kPowerwashSafeDirectory[] =
    "/mnt/stateful_partition/unencrypted/preserve";

// The powerwash_count marker file contains the number of times the device was
// powerwashed. This value is incremented by the clobber-state script when
// a powerwash is performed.
const char kPowerwashCountMarker[] = "powerwash_count";

// The name of the marker file used to trigger powerwash when post-install
// completes successfully so that the device is powerwashed on next reboot.
const char kPowerwashMarkerFile[] =
    "/mnt/stateful_partition/factory_install_reset";

// The contents of the powerwash marker file for the non-rollback case.
const char kPowerwashCommand[] = "safe fast keepimg reason=update_engine\n";

// The contents of the powerwas marker file for the rollback case.
const char kRollbackPowerwashCommand[] =
    "safe fast keepimg rollback reason=update_engine\n";

// UpdateManager config path.
const char* kConfigFilePath = "/etc/update_manager.conf";

// UpdateManager config options:
const char* kConfigOptsIsOOBEEnabled = "is_oobe_enabled";

const char* kActivePingKey = "first_active_omaha_ping_sent";

}  // namespace

namespace chromeos_update_engine {

namespace hardware {

// Factory defined in hardware.h.
std::unique_ptr<HardwareInterface> CreateHardware() {
  std::unique_ptr<HardwareChromeOS> hardware(new HardwareChromeOS());
  hardware->Init();
  return std::move(hardware);
}

}  // namespace hardware

void HardwareChromeOS::Init() {
  LoadConfig("" /* root_prefix */, IsNormalBootMode());
  debugd_proxy_.reset(
      new org::chromium::debugdProxy(DBusConnection::Get()->GetDBus()));
}

bool HardwareChromeOS::IsOfficialBuild() const {
  return VbGetSystemPropertyInt("debug_build") == 0;
}

bool HardwareChromeOS::IsNormalBootMode() const {
  bool dev_mode = VbGetSystemPropertyInt("devsw_boot") != 0;
  return !dev_mode;
}

bool HardwareChromeOS::AreDevFeaturesEnabled() const {
  // Even though the debugd tools are also gated on devmode, checking here can
  // save us a D-Bus call so it's worth doing explicitly.
  if (IsNormalBootMode())
    return false;

  int32_t dev_features = debugd::DEV_FEATURES_DISABLED;
  brillo::ErrorPtr error;
  // Some boards may not include debugd so it's expected that this may fail,
  // in which case we treat it as disabled.
  if (debugd_proxy_ && debugd_proxy_->QueryDevFeatures(&dev_features, &error) &&
      !(dev_features & debugd::DEV_FEATURES_DISABLED)) {
    LOG(INFO) << "Debugd dev tools enabled.";
    return true;
  }
  return false;
}

bool HardwareChromeOS::IsOOBEEnabled() const {
  return is_oobe_enabled_;
}

bool HardwareChromeOS::IsOOBEComplete(base::Time* out_time_of_oobe) const {
  if (!is_oobe_enabled_) {
    LOG(WARNING) << "OOBE is not enabled but IsOOBEComplete() was called";
  }
  struct stat statbuf;
  if (stat(kOOBECompletedMarker, &statbuf) != 0) {
    if (errno != ENOENT) {
      PLOG(ERROR) << "Error getting information about " << kOOBECompletedMarker;
    }
    return false;
  }

  if (out_time_of_oobe != nullptr)
    *out_time_of_oobe = base::Time::FromTimeT(statbuf.st_mtime);
  return true;
}

static string ReadValueFromCrosSystem(const string& key) {
  char value_buffer[VB_MAX_STRING_PROPERTY];

  const char* rv = VbGetSystemPropertyString(
      key.c_str(), value_buffer, sizeof(value_buffer));
  if (rv != nullptr) {
    string return_value(value_buffer);
    base::TrimWhitespaceASCII(return_value, base::TRIM_ALL, &return_value);
    return return_value;
  }

  LOG(ERROR) << "Unable to read crossystem key " << key;
  return "";
}

string HardwareChromeOS::GetHardwareClass() const {
  if (USE_HWID_OVERRIDE) {
    return HwidOverride::Read(base::FilePath("/"));
  }
  return ReadValueFromCrosSystem("hwid");
}

string HardwareChromeOS::GetFirmwareVersion() const {
  return ReadValueFromCrosSystem("fwid");
}

string HardwareChromeOS::GetECVersion() const {
  string input_line;
  int exit_code = 0;
  vector<string> cmd = {"/usr/sbin/mosys", "-k", "ec", "info"};

  bool success = Subprocess::SynchronousExec(cmd, &exit_code, &input_line);
  if (!success || exit_code) {
    LOG(ERROR) << "Unable to read ec info from mosys (" << exit_code << ")";
    return "";
  }

  return utils::ParseECVersion(input_line);
}

int HardwareChromeOS::GetMinKernelKeyVersion() const {
  return VbGetSystemPropertyInt("tpm_kernver");
}

int HardwareChromeOS::GetMaxFirmwareKeyRollforward() const {
  return VbGetSystemPropertyInt("firmware_max_rollforward");
}

bool HardwareChromeOS::SetMaxFirmwareKeyRollforward(
    int firmware_max_rollforward) {
  // Not all devices have this field yet. So first try to read
  // it and if there is an error just fail.
  if (GetMaxFirmwareKeyRollforward() == -1)
    return false;

  return VbSetSystemPropertyInt("firmware_max_rollforward",
                                firmware_max_rollforward) == 0;
}

int HardwareChromeOS::GetMinFirmwareKeyVersion() const {
  return VbGetSystemPropertyInt("tpm_fwver");
}

bool HardwareChromeOS::SetMaxKernelKeyRollforward(int kernel_max_rollforward) {
  return VbSetSystemPropertyInt("kernel_max_rollforward",
                                kernel_max_rollforward) == 0;
}

int HardwareChromeOS::GetPowerwashCount() const {
  int powerwash_count;
  base::FilePath marker_path =
      base::FilePath(kPowerwashSafeDirectory).Append(kPowerwashCountMarker);
  string contents;
  if (!utils::ReadFile(marker_path.value(), &contents))
    return -1;
  base::TrimWhitespaceASCII(contents, base::TRIM_TRAILING, &contents);
  if (!base::StringToInt(contents, &powerwash_count))
    return -1;
  return powerwash_count;
}

bool HardwareChromeOS::SchedulePowerwash(bool is_rollback) {
  const char* powerwash_command =
      is_rollback ? kRollbackPowerwashCommand : kPowerwashCommand;
  bool result = utils::WriteFile(
      kPowerwashMarkerFile, powerwash_command, strlen(powerwash_command));
  if (result) {
    LOG(INFO) << "Created " << kPowerwashMarkerFile
              << " to powerwash on next reboot (is_rollback=" << is_rollback
              << ")";
  } else {
    PLOG(ERROR) << "Error in creating powerwash marker file: "
                << kPowerwashMarkerFile;
  }

  return result;
}

bool HardwareChromeOS::CancelPowerwash() {
  bool result = base::DeleteFile(base::FilePath(kPowerwashMarkerFile), false);

  if (result) {
    LOG(INFO) << "Successfully deleted the powerwash marker file : "
              << kPowerwashMarkerFile;
  } else {
    PLOG(ERROR) << "Could not delete the powerwash marker file : "
                << kPowerwashMarkerFile;
  }

  return result;
}

bool HardwareChromeOS::GetNonVolatileDirectory(base::FilePath* path) const {
  *path = base::FilePath(constants::kNonVolatileDirectory);
  return true;
}

bool HardwareChromeOS::GetPowerwashSafeDirectory(base::FilePath* path) const {
  *path = base::FilePath(kPowerwashSafeDirectory);
  return true;
}

int64_t HardwareChromeOS::GetBuildTimestamp() const {
  // TODO(senj): implement this in Chrome OS.
  return 0;
}

void HardwareChromeOS::LoadConfig(const string& root_prefix, bool normal_mode) {
  brillo::KeyValueStore store;

  if (normal_mode) {
    store.Load(base::FilePath(root_prefix + kConfigFilePath));
  } else {
    if (store.Load(base::FilePath(root_prefix + kStatefulPartition +
                                  kConfigFilePath))) {
      LOG(INFO) << "UpdateManager Config loaded from stateful partition.";
    } else {
      store.Load(base::FilePath(root_prefix + kConfigFilePath));
    }
  }

  if (!store.GetBoolean(kConfigOptsIsOOBEEnabled, &is_oobe_enabled_))
    is_oobe_enabled_ = true;  // Default value.
}

bool HardwareChromeOS::GetFirstActiveOmahaPingSent() const {
  int exit_code = 0;
  string active_ping_str;
  vector<string> cmd = {"vpd_get_value", kActivePingKey};
  if (!Subprocess::SynchronousExec(cmd, &exit_code, &active_ping_str) ||
      exit_code) {
    LOG(ERROR) << "Failed to get vpd key for " << kActivePingKey
               << " with exit code: " << exit_code;
    return false;
  }

  base::TrimWhitespaceASCII(active_ping_str, base::TRIM_ALL, &active_ping_str);
  int active_ping;
  if (active_ping_str.empty() ||
      !base::StringToInt(active_ping_str, &active_ping)) {
    LOG(INFO) << "Failed to parse active_ping value: " << active_ping_str;
    return false;
  }
  return static_cast<bool>(active_ping);
}

bool HardwareChromeOS::SetFirstActiveOmahaPingSent() {
  int exit_code = 0;
  string output;
  vector<string> vpd_set_cmd = {
      "vpd", "-i", "RW_VPD", "-s", string(kActivePingKey) + "=1"};
  if (!Subprocess::SynchronousExec(vpd_set_cmd, &exit_code, &output) ||
      exit_code) {
    LOG(ERROR) << "Failed to set vpd key for " << kActivePingKey
               << " with exit code: " << exit_code << " with error: " << output;
    return false;
  }

  vector<string> vpd_dump_cmd = {"dump_vpd_log", "--force"};
  if (!Subprocess::SynchronousExec(vpd_dump_cmd, &exit_code, &output) ||
      exit_code) {
    LOG(ERROR) << "Failed to cache " << kActivePingKey << " using dump_vpd_log"
               << " with exit code: " << exit_code << " with error: " << output;
    return false;
  }
  return true;
}

}  // namespace chromeos_update_engine
