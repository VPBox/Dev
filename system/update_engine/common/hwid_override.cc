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

#include "update_engine/common/hwid_override.h"

#include <map>
#include <string>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <brillo/key_value_store.h>

using std::map;
using std::string;

namespace chromeos_update_engine {

const char HwidOverride::kHwidOverrideKey[] = "HWID_OVERRIDE";

HwidOverride::HwidOverride() {}

HwidOverride::~HwidOverride() {}

string HwidOverride::Read(const base::FilePath& root) {
  brillo::KeyValueStore lsb_release;
  lsb_release.Load(base::FilePath(root.value() + "/etc/lsb-release"));
  string result;
  if (lsb_release.GetString(kHwidOverrideKey, &result))
    return result;
  return "";
}

}  // namespace chromeos_update_engine
