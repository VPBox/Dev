/*
 * Copyright (C) 2019, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <unordered_map>
#include "aidl_language.h"

namespace android {
namespace aidl {
namespace mappings {

using SignatureMap = std::unordered_map<std::string, std::string>;

SignatureMap generate_mappings(const AidlDefinedType* iface);
}  // namespace mappings
}  // namespace aidl
}  // namespace android
