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

#ifndef NETUTILS_UNIQUEFD_H
#define NETUTILS_UNIQUEFD_H

#include <unistd.h>
#include <ostream>

#include "netdutils/Fd.h"

namespace android {
namespace netdutils {

// Stricter unique_fd implementation that:
// *) Does not implement release()
// *) Does not implicitly cast to int
// *) Uses a strongly typed wrapper (Fd) for the underlying file descriptor
//
// Users of UniqueFd should endeavor to treat this as a completely
// opaque object. The only code that should interpret the wrapped
// value is in Syscalls.h
class UniqueFd {
  public:
    UniqueFd() = default;

    UniqueFd(Fd fd) : mFd(fd) {}

    ~UniqueFd() { reset(); }

    // Disallow copy
    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    // Allow move
    UniqueFd(UniqueFd&& other) { std::swap(mFd, other.mFd); }
    UniqueFd& operator=(UniqueFd&& other) {
        std::swap(mFd, other.mFd);
        return *this;
    }

    // Cleanup any currently owned Fd, replacing it with the optional
    // parameter fd
    void reset(Fd fd = Fd());

    // Implict cast to Fd
    operator const Fd &() const { return mFd; }

  private:
    Fd mFd;
};

std::ostream& operator<<(std::ostream& os, const UniqueFd& fd);

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_UNIQUEFD_H */
