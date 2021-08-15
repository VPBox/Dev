/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <vector>

namespace android {
namespace apex {

static constexpr const char* kApexDataDir = "/data/apex";
static constexpr const char* kActiveApexPackagesDataDir = "/data/apex/active";
static constexpr const char* kApexBackupDir = "/data/apex/backup";
static constexpr const char* kApexPackageSystemDir = "/system/apex";
static const std::vector<std::string> kApexPackageBuiltinDirs = {
    kApexPackageSystemDir, "/product/apex"};
static constexpr const char* kApexRoot = "/apex";
static constexpr const char* kStagedSessionsDir = "/data/app-staging";

static constexpr const char* kApexPackageSuffix = ".apex";

}  // namespace apex
}  // namespace android
