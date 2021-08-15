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

#include "update_engine/common/platform_constants.h"

namespace chromeos_update_engine {
namespace constants {

const char kOmahaDefaultProductionURL[] =
    "https://clients2.google.com/service/update2/brillo";
const char kOmahaDefaultAUTestURL[] =
    "https://clients2.google.com/service/update2/brillo";
const char kOmahaUpdaterID[] = "Brillo";
const char kOmahaPlatformName[] = "Brillo";
const char kUpdatePayloadPublicKeyPath[] =
    "/etc/update_engine/update-payload-key.pub.pem";
const char kCACertificatesPath[] = "/system/etc/security/cacerts_google";
// No deadline file API support on Android.
const char kOmahaResponseDeadlineFile[] = "";
const char kNonVolatileDirectory[] = "/data/misc/update_engine";
const char kPostinstallMountOptions[] =
    "context=u:object_r:postinstall_file:s0";

}  // namespace constants
}  // namespace chromeos_update_engine
