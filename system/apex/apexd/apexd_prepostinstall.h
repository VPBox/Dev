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

#ifndef ANDROID_APEXD_APEXD_PREPOSTINSTALL_H_
#define ANDROID_APEXD_APEXD_PREPOSTINSTALL_H_

#include <string>
#include <vector>

#include "status.h"

namespace android {
namespace apex {

class ApexFile;

Status StagePreInstall(const std::vector<ApexFile>& apexes);
int RunPreInstall(char** argv);

Status StagePostInstall(const std::vector<ApexFile>& apexes);
int RunPostInstall(char** argv);

}  // namespace apex
}  // namespace android

#endif  // ANDROID_APEXD_APEXD_PREPOSTINSTALL_H_
