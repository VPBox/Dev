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

#ifndef NETUTILS_FD_H
#define NETUTILS_FD_H

#include <ostream>

#include "netdutils/Status.h"

namespace android {
namespace netdutils {

// Strongly typed wrapper for file descriptors with value semantics.
// This class should typically hold unowned file descriptors.
class Fd {
  public:
    constexpr Fd() = default;

    constexpr Fd(int fd) : mFd(fd) {}

    int get() const { return mFd; }

    bool operator==(const Fd& other) const { return get() == other.get(); }
    bool operator!=(const Fd& other) const { return get() != other.get(); }

  private:
    int mFd = -1;
};

// Return true if fd appears valid (non-negative)
inline bool isWellFormed(const Fd fd) {
    return fd.get() >= 0;
}

std::ostream& operator<<(std::ostream& os, const Fd& fd);

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_FD_H */
