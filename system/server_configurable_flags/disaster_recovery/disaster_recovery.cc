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

#include <server_configurable_flags/disaster_recovery.h>
#include <server_configurable_flags/get_flags.h>
#include <string>

#include "android-base/logging.h"

// flags_heatlh_check binary takes 1 argument -- reset_mode
// If reset_mode == BOOT_FAILURE, the binary will examine how many
// consecutive reboots have failed. If there are more than 4 consecutive
// reboot failures, flag reset will be performed.
// If reset_mode == UPDATABLE_CRASHING, the binary will directly perform
// flag reset actions.
int main(int argc, char* argv[]) {
  if (argc != 2) {
    LOG(ERROR) << "argc: " << std::to_string(argc) << ", it should only be 2.";
    return 1;
  }

  std::string reset_mode_str(argv[1]);
  server_configurable_flags::ResetMode reset_mode;
  if (reset_mode_str == "BOOT_FAILURE") {
    reset_mode = server_configurable_flags::BOOT_FAILURE;
  } else if (reset_mode_str == "UPDATABLE_CRASHING") {
    reset_mode = server_configurable_flags::UPDATABLE_CRASHING;
  } else {
    LOG(ERROR) << "invalid reset mode: " << reset_mode_str << ".";
    return 1;
  }

  server_configurable_flags::ServerConfigurableFlagsReset(reset_mode);

  return 0;
}