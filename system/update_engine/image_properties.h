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

// This module abstracts the properties tied to the current running image. These
// properties are meant to be constant during the life of this daemon, but can
// be modified in dev-move or non-official builds.

#ifndef UPDATE_ENGINE_IMAGE_PROPERTIES_H_
#define UPDATE_ENGINE_IMAGE_PROPERTIES_H_

#include <string>

namespace chromeos_update_engine {

class SystemState;

// The read-only system properties of the running image.
struct ImageProperties {
  // The product id of the image used for all channels, except canary.
  std::string product_id;
  // The canary-channel product id.
  std::string canary_product_id;
  // The system id for the Android Things SoM, empty for Chrome OS.
  std::string system_id;

  // The product version of this image.
  std::string version;
  // The system version of this image.
  std::string system_version;

  // The version of all product components in key values pairs.
  std::string product_components;

  // A unique string that identifies this build. Normally a combination of the
  // the version, signing keys and build target.
  std::string build_fingerprint;

  // The Android build type, should be either 'user', 'userdebug' or 'eng'.
  // It's empty string on other platform.
  std::string build_type;

  // The board name this image was built for.
  std::string board;

  // The release channel this image was obtained from.
  std::string current_channel;

  // Whether we allow arbitrary channels instead of just the ones listed in
  // kChannelsByStability.
  bool allow_arbitrary_channels = false;

  // The Omaha URL this image should get updates from.
  std::string omaha_url;
};

// The mutable image properties are read-write image properties, initialized
// with values from the image but can be modified by storing them in the
// stateful partition.
struct MutableImageProperties {
  // The release channel we are tracking.
  std::string target_channel;

  // Whether powerwash is allowed when downloading an update for the selected
  // target_channel.
  bool is_powerwash_allowed{false};
};

// Loads all the image properties from the running system. In case of error
// loading any of these properties from the read-only system image a default
// value may be returned instead.
ImageProperties LoadImageProperties(SystemState* system_state);

// Loads the mutable image properties from the stateful partition if found or
// the system image otherwise.
MutableImageProperties LoadMutableImageProperties(SystemState* system_state);

// Stores the mutable image properties in the stateful partition. Returns
// whether the operation succeeded.
bool StoreMutableImageProperties(SystemState* system_state,
                                 const MutableImageProperties& properties);

// Logs the image properties.
void LogImageProperties();

// Sets the root_prefix used to load files from during unittests to
// |test_root_prefix|. Passing a nullptr value resets it to the default.
namespace test {
void SetImagePropertiesRootPrefix(const char* test_root_prefix);
}  // namespace test

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_IMAGE_PROPERTIES_H_
