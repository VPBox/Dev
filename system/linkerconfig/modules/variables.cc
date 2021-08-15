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

#include "linkerconfig/variables.h"

#include <android-base/properties.h>

#include "linkerconfig/log.h"

namespace android {
namespace linkerconfig {
namespace modules {

std::map<std::string, std::string> Variables::variables_;

std::optional<std::string> Variables::GetValue(const std::string& variable) {
  // If variable is in predefined key-value pair, use this value
  if (variables_.find(variable) != variables_.end() &&
      !variables_[variable].empty()) {
    return {variables_[variable]};
  }

  // If variable is defined as property, use this value
  std::string prop_value = android::base::GetProperty(variable, "");
  if (!prop_value.empty()) {
    return {prop_value};
  }

  // If cannot find variable, return default value
  return std::nullopt;
}

void Variables::AddValue(const std::string& key, const std::string& value) {
  variables_[key] = value;
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android