/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef ANDROID_APEXD_APEXD_H_
#define ANDROID_APEXD_APEXD_H_

#include <string>
#include <vector>

#include <android-base/macros.h>

#include "apex_constants.h"
#include "apex_file.h"
#include "status.h"
#include "status_or.h"

namespace android {
namespace apex {

class CheckpointInterface;

Status resumeRollbackIfNeeded();

Status scanPackagesDirAndActivate(const char* apex_package_dir);
void scanStagedSessionsDirAndStage();

Status preinstallPackages(const std::vector<std::string>& paths) WARN_UNUSED;
Status postinstallPackages(const std::vector<std::string>& paths) WARN_UNUSED;

Status stagePackages(const std::vector<std::string>& tmpPaths) WARN_UNUSED;
Status unstagePackages(const std::vector<std::string>& paths) WARN_UNUSED;

StatusOr<std::vector<ApexFile>> submitStagedSession(
    const int session_id,
    const std::vector<int>& child_session_ids) WARN_UNUSED;
Status markStagedSessionReady(const int session_id) WARN_UNUSED;
Status markStagedSessionSuccessful(const int session_id) WARN_UNUSED;
Status rollbackActiveSession();
Status rollbackActiveSessionAndReboot();

Status activatePackage(const std::string& full_path) WARN_UNUSED;
Status deactivatePackage(const std::string& full_path) WARN_UNUSED;

std::vector<ApexFile> getActivePackages();
StatusOr<ApexFile> getActivePackage(const std::string& package_name);

std::vector<ApexFile> getFactoryPackages();

Status abortActiveSession();

int onBootstrap();
void onStart(CheckpointInterface* checkpoint_service);
void onAllPackagesReady();
void unmountDanglingMounts();

}  // namespace apex
}  // namespace android

#endif  // ANDROID_APEXD_APEXD_H_
