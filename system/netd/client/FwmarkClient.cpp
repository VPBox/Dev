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

#include "FwmarkClient.h"

#include "FwmarkCommand.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <algorithm>  // std::size()
#include <iterator>

namespace {

// Env flag to control whether FwmarkClient sends sockets to netd for marking.
// This can only be disabled when the process running as root and is meant for kernel testing.
inline constexpr char ANDROID_NO_USE_FWMARK_CLIENT[] = "ANDROID_NO_USE_FWMARK_CLIENT";

const sockaddr_un FWMARK_SERVER_PATH = {AF_UNIX, "/dev/socket/fwmarkd"};

bool commandHasFd(int cmdId) {
    return (cmdId != FwmarkCommand::QUERY_USER_ACCESS) &&
        (cmdId != FwmarkCommand::SET_COUNTERSET) &&
        (cmdId != FwmarkCommand::DELETE_TAGDATA);
}

}  // namespace

bool FwmarkClient::shouldSetFwmark(int family) {
    // Checking whether family is supported before checking whether this can be
    // disabled. Because there are existing processes using AF_LOCAL socket but it
    // doesn't have permission to call geteuid(). Reference b/135422468.
    if (!FwmarkCommand::isSupportedFamily(family)) {
        return false;
    }

    // Permit processes running as root to disable marking. This is required, for
    // example, to run the kernel networking tests.
    if (getenv(ANDROID_NO_USE_FWMARK_CLIENT) && geteuid() == 0) {
        return false;
    }

    return true;
}

FwmarkClient::FwmarkClient() : mChannel(-1) {
}

FwmarkClient::~FwmarkClient() {
    if (mChannel >= 0) {
        close(mChannel);
    }
}

int FwmarkClient::send(FwmarkCommand* data, int fd, FwmarkConnectInfo* connectInfo) {
    mChannel = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (mChannel == -1) {
        return -errno;
    }

    if (TEMP_FAILURE_RETRY(connect(mChannel, reinterpret_cast<const sockaddr*>(&FWMARK_SERVER_PATH),
                                   sizeof(FWMARK_SERVER_PATH))) == -1) {
        // If we are unable to connect to the fwmark server, assume there's no error. This protects
        // against future changes if the fwmark server goes away.
        // TODO: This means that fd will very likely be misrouted. See if we can delete this in a
        //       separate CL.
        return 0;
    }

    iovec iov[2] = {
        { data, sizeof(*data) },
        { connectInfo, (connectInfo ? sizeof(*connectInfo) : 0) },
    };
    msghdr message;
    memset(&message, 0, sizeof(message));
    message.msg_iov = iov;
    message.msg_iovlen = std::size(iov);

    union {
        cmsghdr cmh;
        char cmsg[CMSG_SPACE(sizeof(fd))];
    } cmsgu;

    if (commandHasFd(data->cmdId)) {
        memset(cmsgu.cmsg, 0, sizeof(cmsgu.cmsg));
        message.msg_control = cmsgu.cmsg;
        message.msg_controllen = sizeof(cmsgu.cmsg);

        cmsghdr* const cmsgh = CMSG_FIRSTHDR(&message);
        cmsgh->cmsg_len = CMSG_LEN(sizeof(fd));
        cmsgh->cmsg_level = SOL_SOCKET;
        cmsgh->cmsg_type = SCM_RIGHTS;
        memcpy(CMSG_DATA(cmsgh), &fd, sizeof(fd));
    }

    if (TEMP_FAILURE_RETRY(sendmsg(mChannel, &message, 0)) == -1) {
        return -errno;
    }

    int error = 0;

    if (TEMP_FAILURE_RETRY(recv(mChannel, &error, sizeof(error), 0)) == -1) {
        return -errno;
    }

    return error;
}
