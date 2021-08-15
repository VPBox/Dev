/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef NETD_INCLUDE_FWMARK_COMMAND_H
#define NETD_INCLUDE_FWMARK_COMMAND_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

// Additional information sent with ON_CONNECT_COMPLETE command
struct FwmarkConnectInfo {
    int error;
    unsigned latencyMs;
    union {
        sockaddr s;
        sockaddr_in sin;
        sockaddr_in6 sin6;
    } addr;

    FwmarkConnectInfo() {}

    FwmarkConnectInfo(const int connectErrno, const unsigned latency, const sockaddr* saddr) {
        error = connectErrno;
        latencyMs = latency;
        if (saddr->sa_family == AF_INET) {
            addr.sin = *((struct sockaddr_in*) saddr);
        } else if (saddr->sa_family == AF_INET6) {
            addr.sin6 = *((struct sockaddr_in6*) saddr);
        } else {
            // Cannot happen because we only call this if shouldSetFwmark returns true, and thus
            // the address family is one we understand.
            addr.s.sa_family = AF_UNSPEC;
        }
    }
};

// Commands sent from clients to the fwmark server to mark sockets (i.e., set their SO_MARK).
// ON_CONNECT_COMPLETE command should be accompanied by FwmarkConnectInfo which should  contain
// info about that connect attempt
// TODO: rework this struct into a more flexible data structure such as union or
// a hierarchy class.
struct FwmarkCommand {
    enum {
        ON_ACCEPT,
        ON_CONNECT,
        SELECT_NETWORK,
        PROTECT_FROM_VPN,
        SELECT_FOR_USER,
        QUERY_USER_ACCESS,
        ON_CONNECT_COMPLETE,
        TAG_SOCKET,
        UNTAG_SOCKET,
        // TODO: use binder to pass the following two request in future after we
        // completely get rid of qtaguid module, since these are privileged
        // command.
        SET_COUNTERSET,
        DELETE_TAGDATA,
    } cmdId;
    unsigned netId;  // used only in the SELECT_NETWORK command; ignored otherwise.
    uid_t uid;       // used in the SELECT_FOR_USER, QUERY_USER_ACCESS, TAG_SOCKET,
                     // SET_COUNTERSET, and DELETE_TAGDATA command; ignored otherwise.
    uint32_t trafficCtrlInfo;  // used in TAG_SOCKET, SET_COUNTERSET and SET_PACIFIER command;
                               // ignored otherwise. Depend on the case, it can be a tag, a
                               // counterSet or a pacifier signal.

    static bool isSupportedFamily(int socketFamily) {
        return socketFamily == AF_INET || socketFamily == AF_INET6;
    }
};

#endif  // NETD_INCLUDE_FWMARK_COMMAND_H
