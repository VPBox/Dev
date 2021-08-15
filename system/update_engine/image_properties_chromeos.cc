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

#include <string>
#include <vector>

#include <base/files/file_util.h>
#include <base/logging.h>
#include <brillo/key_value_store.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/utils.h"
#include "update_engine/system_state.h"

namespace {

const char kLsbRelease[] = "/etc/lsb-release";

const char kLsbReleaseAppIdKey[] = "CHROMEOS_RELEASE_APPID";
const char kLsbReleaseAutoUpdateServerKey[] = "CHROMEOS_AUSERVER";
const char kLsbReleaseBoardAppIdKey[] = "CHROMEOS_BOARD_APPID";
const char kLsbReleaseBoardKey[] = "CHROMEOS_RELEASE_BOARD";
const char kLsbReleaseCanaryAppIdKey[] = "CHROMEOS_CANARY_APPID";
const char kLsbReleaseIsPowerwashAllowedKey[] = "CHROMEOS_IS_POWERWASH_ALLOWED";
const char kLsbReleaseUpdateChannelKey[] = "CHROMEOS_RELEASE_TRACK";
const char kLsbReleaseVersionKey[] = "CHROMEOS_RELEASE_VERSION";

const char kDefaultAppId[] = "{87efface-864d-49a5-9bb3-4b050a7c227a}";

// A prefix added to the path, used for testing.
const char* root_prefix = nullptr;

std::string GetStringWithDefault(const brillo::KeyValueStore& store,
                                 const std::string& key,
                                 const std::string& default_value) {
  std::string result;
  if (store.GetString(key, &result))
    return result;
  LOG(INFO) << "Cannot load ImageProperty " << key << ", using default value "
            << default_value;
  return default_value;
}

enum class LsbReleaseSource {
  kSystem,
  kStateful,
};

// Loads the lsb-release properties into the key-value |store| reading the file
// from either the system image or the stateful partition as specified by
// |source|. The loaded values are added to the store, possibly overriding
// existing values.
void LoadLsbRelease(LsbReleaseSource source, brillo::KeyValueStore* store) {
  std::string path;
  if (root_prefix)
    path = root_prefix;
  if (source == LsbReleaseSource::kStateful)
    path += chromeos_update_engine::kStatefulPartition;
  store->Load(base::FilePath(path + kLsbRelease));
}

}  // namespace

namespace chromeos_update_engine {

namespace test {
void SetImagePropertiesRootPrefix(const char* test_root_prefix) {
  root_prefix = test_root_prefix;
}
}  // namespace test

ImageProperties LoadImageProperties(SystemState* system_state) {
  ImageProperties result;

  brillo::KeyValueStore lsb_release;
  LoadLsbRelease(LsbReleaseSource::kSystem, &lsb_release);
  result.current_channel = GetStringWithDefault(
      lsb_release, kLsbReleaseUpdateChannelKey, "stable-channel");

  // In dev-mode and unofficial build we can override the image properties set
  // in the system image with the ones from the stateful partition, except the
  // channel of the current image.
  HardwareInterface* const hardware = system_state->hardware();
  if (!hardware->IsOfficialBuild() || !hardware->IsNormalBootMode())
    LoadLsbRelease(LsbReleaseSource::kStateful, &lsb_release);

  // The release_app_id is used as the default appid, but can be override by
  // the board appid in the general case or the canary appid for the canary
  // channel only.
  std::string release_app_id =
      GetStringWithDefault(lsb_release, kLsbReleaseAppIdKey, kDefaultAppId);

  result.product_id = GetStringWithDefault(
      lsb_release, kLsbReleaseBoardAppIdKey, release_app_id);
  result.canary_product_id = GetStringWithDefault(
      lsb_release, kLsbReleaseCanaryAppIdKey, release_app_id);
  result.board = GetStringWithDefault(lsb_release, kLsbReleaseBoardKey, "");
  result.version = GetStringWithDefault(lsb_release, kLsbReleaseVersionKey, "");
  result.omaha_url =
      GetStringWithDefault(lsb_release,
                           kLsbReleaseAutoUpdateServerKey,
                           constants::kOmahaDefaultProductionURL);
  // Build fingerprint not used in Chrome OS.
  result.build_fingerprint = "";
  result.allow_arbitrary_channels = false;

  return result;
}

MutableImageProperties LoadMutableImageProperties(SystemState* system_state) {
  MutableImageProperties result;
  brillo::KeyValueStore lsb_release;
  LoadLsbRelease(LsbReleaseSource::kSystem, &lsb_release);
  LoadLsbRelease(LsbReleaseSource::kStateful, &lsb_release);
  result.target_channel = GetStringWithDefault(
      lsb_release, kLsbReleaseUpdateChannelKey, "stable-channel");
  if (!lsb_release.GetBoolean(kLsbReleaseIsPowerwashAllowedKey,
                              &result.is_powerwash_allowed))
    result.is_powerwash_allowed = false;
  return result;
}

bool StoreMutableImageProperties(SystemState* system_state,
                                 const MutableImageProperties& properties) {
  brillo::KeyValueStore lsb_release;
  LoadLsbRelease(LsbReleaseSource::kStateful, &lsb_release);
  lsb_release.SetString(kLsbReleaseUpdateChannelKey, properties.target_channel);
  lsb_release.SetBoolean(kLsbReleaseIsPowerwashAllowedKey,
                         properties.is_powerwash_allowed);

  std::string root_prefix_str = root_prefix ? root_prefix : "";
  base::FilePath path(root_prefix_str + kStatefulPartition + kLsbRelease);
  if (!base::DirectoryExists(path.DirName()))
    base::CreateDirectory(path.DirName());
  return lsb_release.Save(path);
}

void LogImageProperties() {
  std::string lsb_release;
  if (utils::ReadFile(kLsbRelease, &lsb_release)) {
    LOG(INFO) << "lsb-release inside the old rootfs:\n" << lsb_release;
  }

  std::string stateful_lsb_release;
  if (utils::ReadFile(std::string(kStatefulPartition) + kLsbRelease,
                      &stateful_lsb_release)) {
    LOG(INFO) << "stateful lsb-release:\n" << stateful_lsb_release;
  }
}

}  // namespace chromeos_update_engine
