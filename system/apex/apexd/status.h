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

#ifndef ANDROID_APEXD_STATUS_H_
#define ANDROID_APEXD_STATUS_H_

#include <string>

#include <android-base/logging.h>

namespace android {
namespace apex {

class Status {
 public:
  Status() : ok_(true) {}
  explicit Status(const std::string& error_msg)
      : error_msg_(error_msg), ok_(false) {}

  // For better legible code.
  static Status Success() { return Status(); }
  static Status Fail(const std::string& error_msg) { return Status(error_msg); }

  bool Ok() const { return ok_; }

  const std::string& ErrorMessage() const {
    CHECK(!ok_);
    return error_msg_;
  }

 private:
  std::string error_msg_;
  bool ok_;
};

}  // namespace apex
}  // namespace android

#endif  // ANDROID_APEXD_STATUS_H_
