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

#ifndef NETD_SERVER_PROCESS_H_
#define NETD_SERVER_PROCESS_H_

#include "netdutils/DumpWriter.h"

#include <string>

namespace android {
namespace net {
namespace process {

// Does what is says on the tin.
void blockSigPipe();

void writePidFile(const std::string& pidFile);
void removePidFile(const std::string& pidFile);

class ScopedPidFile {
  public:
    ScopedPidFile() = delete;
    ScopedPidFile(const std::string& filename) : pidFile(filename) {
        removePidFile(pidFile);
        writePidFile(pidFile);
    }
    ScopedPidFile(const ScopedPidFile&) = delete;
    ScopedPidFile(ScopedPidFile&&) = delete;

    ~ScopedPidFile() {
        removePidFile(pidFile);
    }

    ScopedPidFile& operator=(const ScopedPidFile&) = delete;
    ScopedPidFile& operator=(ScopedPidFile&&) = delete;

    const std::string pidFile;
};

void dump(netdutils::DumpWriter& dw);

}  // namespace process
}  // namespace net
}  // namespace android

#endif  // NETD_SERVER_PROCESS_H_
