//
// Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <string>

namespace android {
namespace gsi {

static constexpr char kGsiServiceName[] = "gsiservice";

static constexpr char kGsiBootedIndicatorFile[] = "/metadata/gsi/dsu/booted";

static constexpr char kGsiBootedProp[] = "ro.gsid.image_running";

static constexpr int kMaxBootAttempts = 1;

// Returns true if the currently running system image is a live GSI.
bool IsGsiRunning();

// Return true if a GSI is installed (but not necessarily running).
bool IsGsiInstalled();

// Set the GSI as no longer bootable. This effectively removes the GSI. If no
// GSI was bootable, false is returned.
bool UninstallGsi();

// Set the GSI as no longer bootable, without removing its installed files.
bool DisableGsi();

// Returns true if init should attempt to boot into a live GSI image, false
// otherwise. If true, then the path to the liblp metadata file is set. If
// false, an error message is set instead.
//
// This is only called by first-stage init.
bool CanBootIntoGsi(std::string* metadata_file, std::string* error);

// Called by first-stage init to indicate that we're about to boot into a
// GSI.
bool MarkSystemAsGsi();

}  // namespace gsi
}  // namespace android
