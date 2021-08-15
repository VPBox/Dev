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

#ifndef NETDUTILS_SOCKETOPTION_H
#define NETDUTILS_SOCKETOPTION_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

#include "netdutils/Fd.h"
#include "netdutils/Status.h"

namespace android {
namespace netdutils {

// Turn on simple "boolean" socket options.
//
// This is simple wrapper for options that are enabled via code of the form:
//
//     int on = 1;
//     setsockopt(..., &on, sizeof(on));
Status enableSockopt(Fd sock, int level, int optname);

// Turn on TCP keepalives, and set keepalive parameters for this socket.
//
// A parameter value of zero does not set that parameter.
//
// Typical system defaults are:
//
//     idleTime (in seconds)
//     $ cat /proc/sys/net/ipv4/tcp_keepalive_time
//     7200
//
//     numProbes
//     $ cat /proc/sys/net/ipv4/tcp_keepalive_probes
//     9
//
//     probeInterval (in seconds)
//     $ cat /proc/sys/net/ipv4/tcp_keepalive_intvl
//     75
Status enableTcpKeepAlives(Fd sock, unsigned idleTime, unsigned numProbes, unsigned probeInterval);

}  // namespace netdutils
}  // namespace android

#endif /* NETDUTILS_SOCKETOPTION_H */
