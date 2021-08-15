/*
 * Copyright (C) 2018, The Android Open Source Project
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

#include "aidl_to_cpp.h"
#include "aidl_language.h"
#include "logging.h"

#include <functional>
#include <unordered_map>

using std::ostringstream;

namespace android {
namespace aidl {
namespace cpp {

std::string ConstantValueDecorator(const AidlTypeSpecifier& type, const std::string& raw_value) {
  if (type.GetName() == "String" && !type.IsArray() && !type.IsUtf8InCpp()) {
    return "::android::String16(" + raw_value + ")";
  }

  return raw_value;
};

std::string GetTransactionIdFor(const AidlMethod& method) {
  ostringstream output;

  output << "::android::IBinder::FIRST_CALL_TRANSACTION + ";
  output << method.GetId() << " /* " << method.GetName() << " */";
  return output.str();
}
}  // namespace cpp
}  // namespace aidl
}  // namespace android
