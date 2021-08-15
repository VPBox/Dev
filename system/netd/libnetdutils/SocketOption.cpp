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

#include "netdutils/SocketOption.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <utility>

#include "netdutils/Syscalls.h"

namespace android {
namespace netdutils {

Status enableSockopt(Fd sock, int level, int optname) {
    auto& sys = sSyscalls.get();
    const int on = 1;
    return sys.setsockopt(sock, level, optname, &on, sizeof(on));
}

Status enableTcpKeepAlives(Fd sock, unsigned idleTime, unsigned numProbes, unsigned probeInterval) {
    RETURN_IF_NOT_OK(enableSockopt(sock, SOL_SOCKET, SO_KEEPALIVE));

    auto& sys = sSyscalls.get();
    if (idleTime != 0) {
        RETURN_IF_NOT_OK(sys.setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, &idleTime, sizeof(idleTime)));
    }
    if (numProbes != 0) {
        RETURN_IF_NOT_OK(sys.setsockopt(sock, SOL_TCP, TCP_KEEPCNT, &numProbes, sizeof(numProbes)));
    }
    if (probeInterval != 0) {
        RETURN_IF_NOT_OK(sys.setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, &probeInterval,
                sizeof(probeInterval)));
    }

    return status::ok;
}

}  // namespace netdutils
}  // namespace android
