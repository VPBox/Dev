/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "netdutils/Status.h"

#include <sstream>

#include "android-base/stringprintf.h"

namespace android {
namespace netdutils {

Status statusFromErrno(int err, const std::string& msg) {
    return Status(err, base::StringPrintf("[%s] : %s", strerror(err), msg.c_str()));
}

bool equalToErrno(const Status& status, int err) {
    return status.code() == err;
}

std::string toString(const Status& status) {
    std::stringstream ss;
    ss << status;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Status& s) {
    return os << "Status[code: " << s.code() << ", msg: \"" << s.msg() << "\"]";
}

}  // namespace netdutils
}  // namespace android
