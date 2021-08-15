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

#ifndef UPDATE_ENGINE_COMMON_HWID_OVERRIDE_H_
#define UPDATE_ENGINE_COMMON_HWID_OVERRIDE_H_

#include <map>
#include <string>

#include <base/files/file_path.h>
#include <base/macros.h>

namespace chromeos_update_engine {

// Class that allows HWID to be read from <root>/etc/lsb-release.
class HwidOverride {
 public:
  HwidOverride();
  ~HwidOverride();

  // Read HWID from an /etc/lsb-release file under given root.
  // An empty string is returned if there is any error.
  static std::string Read(const base::FilePath& root);

  static const char kHwidOverrideKey[];

 private:
  DISALLOW_COPY_AND_ASSIGN(HwidOverride);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_HWID_OVERRIDE_H_
