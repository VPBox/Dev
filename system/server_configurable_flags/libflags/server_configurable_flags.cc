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
 * limitations under the License
 */

#include "server_configurable_flags/disaster_recovery.h"
#include "server_configurable_flags/get_flags.h"

#if defined(__BIONIC__)
#include <cutils/properties.h>
#endif  // __BIONIC__
#include <regex>
#include <string>

#include "android-base/file.h"
#include "android-base/logging.h"
#include "android-base/properties.h"
#include "android-base/strings.h"
#include "android-base/unique_fd.h"

#define SYSTEM_PROPERTY_PREFIX "persist.device_config."

#define ATTEMPTED_BOOT_COUNT_PROPERTY "persist.device_config.attempted_boot_count"

#define RESET_PERFORMED_PROPERTY "device_config.reset_performed"

#define RESET_FLAGS_FILE_PATH "/data/server_configurable_flags/reset_flags"

#define ATTEMPTED_BOOT_COUNT_THRESHOLD 4

namespace server_configurable_flags {

static const std::regex NAME_VALID_CHARACTER_REGIX("^[\\w\\.\\-@:]*$");

static std::string MakeSystemPropertyName(const std::string& experiment_category_name,
                                          const std::string& experiment_flag_name) {
  return SYSTEM_PROPERTY_PREFIX + experiment_category_name + "." + experiment_flag_name;
}

static bool ValidateExperimentSegment(const std::string& segment) {
  return std::regex_match(segment, NAME_VALID_CHARACTER_REGIX) && segment.find(".") != 0 &&
         segment.find(".") != segment.size() - 1;
}

#if defined(__BIONIC__)
static void ResetFlag(const char* key, const char* value, void* cookie) {
  if (strcmp(ATTEMPTED_BOOT_COUNT_PROPERTY, key) &&
      android::base::StartsWith(key, SYSTEM_PROPERTY_PREFIX) && strlen(value) > 0 &&
      android::base::SetProperty(key, "")) {
    std::string* reset_flags = static_cast<std::string*>(cookie);
    if (reset_flags->length() > 0) {
      reset_flags->append(";");
    }
    reset_flags->append(key);
    android::base::SetProperty(RESET_PERFORMED_PROPERTY, "true");
  }
}

// Reset all system properties used as flags into empty value,
// and record reset flags' names in /data/server_configurable_flags/reset_flags
static void ResetAllFlags() {
  std::string reset_flags;
  property_list(ResetFlag, &reset_flags);

  if (reset_flags.length() > 0) {
    android::base::unique_fd fd(
        TEMP_FAILURE_RETRY(open(RESET_FLAGS_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, 0666)));
    if (fd == -1) {
      LOG(INFO) << __FUNCTION__ << " failed to open file " << RESET_FLAGS_FILE_PATH;
    } else if (!WriteStringToFd(reset_flags, fd)) {
      LOG(INFO) << __FUNCTION__ << " failed to write file " << RESET_FLAGS_FILE_PATH;
    } else {
      LOG(INFO) << __FUNCTION__ << " successfully write to file " << RESET_FLAGS_FILE_PATH;
    }
  }
}
#endif  // __BIONIC__

void ServerConfigurableFlagsReset(ResetMode reset_mode) {
  LOG(INFO) << __FUNCTION__ << " reset_mode value: " << reset_mode;
#if defined(__BIONIC__)
  if (reset_mode == BOOT_FAILURE) {
    int fail_count = android::base::GetIntProperty(ATTEMPTED_BOOT_COUNT_PROPERTY, 0);
    if (fail_count < ATTEMPTED_BOOT_COUNT_THRESHOLD) {
      LOG(INFO) << __FUNCTION__ << " attempted boot count is under threshold, skipping reset.";

      // ATTEMPTED_BOOT_COUNT_PROPERTY will be reset to 0 when sys.boot_completed is set to 1.
      // The code lives in flags_health_check.rc.
      android::base::SetProperty(ATTEMPTED_BOOT_COUNT_PROPERTY, std::to_string(fail_count + 1));
    } else {
      LOG(INFO) << __FUNCTION__ << " attempted boot count reaches threshold, resetting flags.";
      ResetAllFlags();
    }
  } else if (reset_mode == UPDATABLE_CRASHING) {
    LOG(INFO) << __FUNCTION__ << " updatable crashing detected, resetting flags.";
    ResetAllFlags();
  } else {
    LOG(ERROR) << __FUNCTION__ << " invalid reset_mode, skipping reset.";
  }
#else
  LOG(ERROR) << __FUNCTION__ << " ServerConfigurableFlagsReset is not available for this build.";
#endif  // __BIONIC__
}

std::string GetServerConfigurableFlag(const std::string& experiment_category_name,
                                      const std::string& experiment_flag_name,
                                      const std::string& default_value) {
  if (!ValidateExperimentSegment(experiment_category_name)) {
    LOG(ERROR) << __FUNCTION__ << " invalid category name " << experiment_category_name;
    return default_value;
  }
  if (!ValidateExperimentSegment(experiment_flag_name)) {
    LOG(ERROR) << __FUNCTION__ << " invalid flag name " << experiment_flag_name;
    return default_value;
  }
  return android::base::GetProperty(
      MakeSystemPropertyName(experiment_category_name, experiment_flag_name), default_value);
}

}  // namespace server_configurable_flags
