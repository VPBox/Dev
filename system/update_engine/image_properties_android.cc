//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/image_properties.h"

#include <fcntl.h>

#include <string>

#include <android-base/properties.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <bootloader_message/bootloader_message.h>
#include <brillo/osrelease_reader.h>
#include <brillo/strings/string_utils.h>

#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/common/utils.h"
#include "update_engine/system_state.h"

using android::base::GetProperty;
using std::string;

namespace chromeos_update_engine {

namespace {

// Build time properties name used in Android Things.
const char kProductId[] = "product_id";
const char kProductVersion[] = "product_version";
const char kSystemId[] = "system_id";
const char kSystemVersion[] = "system_version";

// The path to the product_components file which stores the version of each
// components in OEM partition.
const char kProductComponentsPath[] = "/oem/os-release.d/product_components";

// Prefs used to store the powerwash settings.
const char kPrefsImgPropPowerwashAllowed[] = "img-prop-powerwash-allowed";

// System properties that identifies the "board".
const char kPropProductName[] = "ro.product.name";
const char kPropBuildFingerprint[] = "ro.build.fingerprint";
const char kPropBuildType[] = "ro.build.type";

// Default channel from factory.prop
const char kPropDefaultChannel[] = "ro.update.default_channel";

// A prefix added to the path, used for testing.
const char* root_prefix = nullptr;

string GetStringWithDefault(const brillo::OsReleaseReader& osrelease,
                            const string& key,
                            const string& default_value) {
  string result;
  if (osrelease.GetString(key, &result))
    return result;
  LOG(INFO) << "Cannot load ImageProperty " << key << ", using default value "
            << default_value;
  return default_value;
}

// Open misc partition for read or write and output the fd in |out_fd|.
bool OpenMisc(bool write, int* out_fd) {
  string misc_device;
  int flags = write ? O_WRONLY | O_SYNC : O_RDONLY;
  if (root_prefix) {
    // Use a file for unittest and create one if doesn't exist.
    misc_device = base::FilePath(root_prefix).Append("misc").value();
    if (write)
      flags |= O_CREAT;
  } else {
    string err;
    misc_device = get_bootloader_message_blk_device(&err);
    if (misc_device.empty()) {
      LOG(ERROR) << "Unable to get misc block device: " << err;
      return false;
    }
  }

  int fd = HANDLE_EINTR(open(misc_device.c_str(), flags, 0600));
  if (fd < 0) {
    PLOG(ERROR) << "Opening misc failed";
    return false;
  }
  *out_fd = fd;
  return true;
}

// The offset and size of the channel field in misc partition.
constexpr size_t kChannelOffset =
    BOOTLOADER_MESSAGE_OFFSET_IN_MISC +
    offsetof(bootloader_message_ab, update_channel);
constexpr size_t kChannelSize = sizeof(bootloader_message_ab::update_channel);

// Read channel from misc partition to |out_channel|, return false if unable to
// read misc or no channel is set in misc.
bool ReadChannelFromMisc(string* out_channel) {
  int fd;
  TEST_AND_RETURN_FALSE(OpenMisc(false, &fd));
  ScopedFdCloser fd_closer(&fd);
  char channel[kChannelSize] = {0};
  ssize_t bytes_read = 0;
  if (!utils::PReadAll(
          fd, channel, kChannelSize - 1, kChannelOffset, &bytes_read) ||
      bytes_read != kChannelSize - 1) {
    PLOG(ERROR) << "Reading update channel from misc failed";
    return false;
  }
  if (channel[0] == '\0') {
    LOG(INFO) << "No channel set in misc.";
    return false;
  }
  if (!base::EndsWith(channel, "-channel", base::CompareCase::SENSITIVE)) {
    LOG(ERROR) << "Channel " << channel << " doesn't end with -channel.";
    return false;
  }
  out_channel->assign(channel);
  return true;
}

// Write |in_channel| to misc partition, return false if failed to write.
bool WriteChannelToMisc(const string& in_channel) {
  int fd;
  TEST_AND_RETURN_FALSE(OpenMisc(true, &fd));
  ScopedFdCloser fd_closer(&fd);
  if (in_channel.size() >= kChannelSize) {
    LOG(ERROR) << "Channel name is too long: " << in_channel
               << ", the maximum length is " << kChannelSize - 1;
    return false;
  }
  char channel[kChannelSize] = {0};
  memcpy(channel, in_channel.data(), in_channel.size());
  if (!utils::PWriteAll(fd, channel, kChannelSize, kChannelOffset)) {
    PLOG(ERROR) << "Writing update channel to misc failed";
    return false;
  }
  return true;
}

string GetTargetChannel() {
  string channel;
  if (!ReadChannelFromMisc(&channel))
    channel = GetProperty(kPropDefaultChannel, "stable-channel");
  return channel;
}
}  // namespace

namespace test {
void SetImagePropertiesRootPrefix(const char* test_root_prefix) {
  root_prefix = test_root_prefix;
}
}  // namespace test

ImageProperties LoadImageProperties(SystemState* system_state) {
  ImageProperties result;

  brillo::OsReleaseReader osrelease;
  if (root_prefix)
    osrelease.LoadTestingOnly(base::FilePath(root_prefix));
  else
    osrelease.Load();
  result.product_id =
      GetStringWithDefault(osrelease, kProductId, "invalid-product");
  result.system_id = GetStringWithDefault(
      osrelease, kSystemId, "developer-boards:brillo-starter-board");
  // Update the system id to match the prefix of product id for testing.
  string prefix, not_used, system_id;
  if (brillo::string_utils::SplitAtFirst(
          result.product_id, ":", &prefix, &not_used, false) &&
      brillo::string_utils::SplitAtFirst(
          result.system_id, ":", &not_used, &system_id, false)) {
    result.system_id = prefix + ":" + system_id;
  }
  result.canary_product_id = result.product_id;
  result.version = GetStringWithDefault(osrelease, kProductVersion, "0.0.0.0");
  result.system_version =
      GetStringWithDefault(osrelease, kSystemVersion, "0.0.0.0");
  // Can't read it with OsReleaseReader because it has multiple lines.
  utils::ReadFile(kProductComponentsPath, &result.product_components);

  result.board = GetProperty(kPropProductName, "brillo");
  result.build_fingerprint = GetProperty(kPropBuildFingerprint, "none");
  result.build_type = GetProperty(kPropBuildType, "");

  // Android doesn't have channel information in system image, we try to read
  // the channel of current slot from prefs and then fallback to use the
  // persisted target channel as current channel.
  string current_channel_key =
      kPrefsChannelOnSlotPrefix +
      std::to_string(system_state->boot_control()->GetCurrentSlot());
  string current_channel;
  if (!system_state->prefs()->Exists(current_channel_key) ||
      !system_state->prefs()->GetString(current_channel_key, &current_channel))
    current_channel = GetTargetChannel();
  result.current_channel = current_channel;
  result.allow_arbitrary_channels = true;

  // Brillo only supports the official omaha URL.
  result.omaha_url = constants::kOmahaDefaultProductionURL;

  return result;
}

MutableImageProperties LoadMutableImageProperties(SystemState* system_state) {
  MutableImageProperties result;
  result.target_channel = GetTargetChannel();
  if (!system_state->prefs()->GetBoolean(kPrefsImgPropPowerwashAllowed,
                                         &result.is_powerwash_allowed)) {
    result.is_powerwash_allowed = false;
  }
  return result;
}

bool StoreMutableImageProperties(SystemState* system_state,
                                 const MutableImageProperties& properties) {
  bool ret = true;
  if (!WriteChannelToMisc(properties.target_channel))
    ret = false;
  if (!system_state->prefs()->SetBoolean(kPrefsImgPropPowerwashAllowed,
                                         properties.is_powerwash_allowed))
    ret = false;
  return ret;
}

void LogImageProperties() {
  // TODO(*): Implement this.
}

}  // namespace chromeos_update_engine
