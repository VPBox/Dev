/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <poll.h> /* poll */
#include <sys/socket.h>

#include <thread>

#include <android-base/parseint.h>
#include <android-base/unique_fd.h>
#include <gtest/gtest.h>

#include "NetdClient.h"
#include "netdclient_priv.h"

namespace {

// Keep in sync with FrameworkListener.cpp (500, "Command not recognized")
constexpr char NOT_SUPPORT_MSG[] = "500 Command not recognized";

void serverLoop(int dnsProxyFd) {
    while (true) {
        pollfd fds[1] = {{.fd = dnsProxyFd, .events = POLLIN}};
        enum { SERVERFD = 0 };

        const int s = TEMP_FAILURE_RETRY(poll(fds, std::size(fds), -1));
        if (s <= 0) break;

        if (fds[SERVERFD].revents & POLLIN) {
            char buf[4096];
            TEMP_FAILURE_RETRY(read(fds[SERVERFD].fd, &buf, sizeof(buf)));
            // TODO: verify command
            TEMP_FAILURE_RETRY(write(fds[SERVERFD].fd, NOT_SUPPORT_MSG, sizeof(NOT_SUPPORT_MSG)));
        }
    }
}

}  // namespace

TEST(NetdClientTest, getNetworkForDnsInternal) {
    // Test invalid fd
    unsigned dnsNetId = 0;
    const int invalidFd = -1;
    EXPECT_EQ(-EBADF, getNetworkForDnsInternal(invalidFd, &dnsNetId));

    // Test what the client does if the resolver does not support the "getdnsnetid" command.
    android::base::unique_fd clientFd, serverFd;
    ASSERT_TRUE(android::base::Socketpair(AF_UNIX, &clientFd, &serverFd));

    std::thread serverThread = std::thread(serverLoop, serverFd.get());

    EXPECT_EQ(-EOPNOTSUPP, getNetworkForDnsInternal(clientFd.get(), &dnsNetId));

    clientFd.reset();  // Causes serverLoop() to exit
    serverThread.join();
}

TEST(NetdClientTest, getNetworkForDns) {
    // Test null input
    unsigned* testNull = nullptr;
    EXPECT_EQ(-EFAULT, getNetworkForDns(testNull));
}
