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

#include <algorithm>

#include "netdutils/UniqueFd.h"
#include "netdutils/Syscalls.h"

namespace android {
namespace netdutils {

void UniqueFd::reset(Fd fd) {
    auto& sys = sSyscalls.get();
    std::swap(fd, mFd);
    if (isWellFormed(fd)) {
        expectOk(sys.close(fd));
    }
}

std::ostream& operator<<(std::ostream& os, const UniqueFd& fd) {
    return os << "UniqueFd[" << static_cast<Fd>(fd) << "]";
}

}  // namespace netdutils
}  // namespace android
