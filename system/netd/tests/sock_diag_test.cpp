/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * sock_diag_test.cpp - unit tests for SockDiag.cpp
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/inet_diag.h>

#include <gtest/gtest.h>

#include "NetdConstants.h"
#include "SockDiag.h"


#define NUM_SOCKETS 500


class SockDiagTest : public ::testing::Test {
};

uint16_t bindAndListen(int s) {
    for (int i = 0; i < 10; i++) {
        uint16_t port = 1024 + arc4random_uniform(0xffff - 1024);
        sockaddr_in6 sin6 = { .sin6_family = AF_INET6, .sin6_port = htons(port) };
        if (bind(s, (sockaddr *) &sin6, sizeof(sin6)) == 0) {
            listen(s, 1);
            return port;
        }
    }
    close(s);
    return 0;
}

const char *tcpStateName(uint8_t state) {
    static const char *states[] = {
        "???",
        "TCP_ESTABLISHED",
        "TCP_SYN_SENT",
        "TCP_SYN_RECV",
        "TCP_FIN_WAIT1",
        "TCP_FIN_WAIT2",
        "TCP_TIME_WAIT",
        "TCP_CLOSE",
        "TCP_CLOSE_WAIT",
        "TCP_LAST_ACK",
        "TCP_LISTEN",
        "TCP_CLOSING",
        "TCP_NEW_SYN_RECV",
    };
    return states[(state < ARRAY_SIZE(states)) ? state : 0];
}

TEST_F(SockDiagTest, TestDump) {
    int v4socket = socket(AF_INET, SOCK_STREAM, 0);
    int v6socket = socket(AF_INET6, SOCK_STREAM, 0);
    int listensocket = socket(AF_INET6, SOCK_STREAM, 0);
    ASSERT_NE(-1, v4socket) << "Failed to open IPv4 socket";
    ASSERT_NE(-1, v6socket) << "Failed to open IPv6 socket";
    ASSERT_NE(-1, listensocket) << "Failed to open listen socket";

    uint16_t port = bindAndListen(listensocket);
    ASSERT_NE(0, port) << "Can't bind to server port";

    // Connect to loopback.
    sockaddr_in server4 = { .sin_family = AF_INET, .sin_port = htons(port) };
    sockaddr_in6 server6 = { .sin6_family = AF_INET6, .sin6_port = htons(port) };
    ASSERT_EQ(0, connect(v4socket, (sockaddr *) &server4, sizeof(server4)))
        << "IPv4 connect failed: " << strerror(errno);
    ASSERT_EQ(0, connect(v6socket, (sockaddr *) &server6, sizeof(server6)))
        << "IPv6 connect failed: " << strerror(errno);

    sockaddr_in6 client46, client6;
    socklen_t clientlen = std::max(sizeof(client46), sizeof(client6));
    int accepted4 = accept(listensocket, (sockaddr *) &client46, &clientlen);
    int accepted6 = accept(listensocket, (sockaddr *) &client6, &clientlen);
    ASSERT_NE(-1, accepted4);
    ASSERT_NE(-1, accepted6);

    int v4SocketsSeen = 0;
    bool seenclient46 = false;
    bool seenNull = false;
    char src[INET6_ADDRSTRLEN], dst[INET6_ADDRSTRLEN];

    fprintf(stderr, "Ports:\n  server=%d. client46=%d, client6=%d\n",
            port, ntohs(client46.sin6_port), ntohs(client6.sin6_port));

    auto checkIPv4Dump = [&] (uint8_t /* proto */, const inet_diag_msg *msg) {
        if (msg == nullptr) {
            EXPECT_FALSE(seenNull);
            seenNull = true;
            return 0;
        }
        EXPECT_EQ(htonl(INADDR_LOOPBACK), msg->id.idiag_src[0]);
        v4SocketsSeen++;
        seenclient46 |= (msg->id.idiag_sport == client46.sin6_port);
        inet_ntop(AF_INET, msg->id.idiag_src, src, sizeof(src));
        inet_ntop(AF_INET, msg->id.idiag_src, dst, sizeof(dst));
        fprintf(stderr, "  v4 %s:%d -> %s:%d %s\n",
                src, htons(msg->id.idiag_sport),
                dst, htons(msg->id.idiag_dport),
                tcpStateName(msg->idiag_state));
        return 0;
    };

    int v6SocketsSeen = 0;
    bool seenClient6 = false, seenServer46 = false, seenServer6 = false;

    auto checkIPv6Dump = [&] (uint8_t /* proto */, const inet_diag_msg *msg) {
        if (msg == nullptr) {
            EXPECT_FALSE(seenNull);
            seenNull = true;
            return 0;
        }
        struct in6_addr *saddr = (struct in6_addr *) msg->id.idiag_src;
        EXPECT_TRUE(
            IN6_IS_ADDR_LOOPBACK(saddr) ||
            (IN6_IS_ADDR_V4MAPPED(saddr) && saddr->s6_addr32[3] == htonl(INADDR_LOOPBACK)));
        v6SocketsSeen++;
        seenClient6 |= (msg->id.idiag_sport == client6.sin6_port);
        seenServer46 |= (msg->id.idiag_sport == htons(port));
        seenServer6 |= (msg->id.idiag_sport == htons(port));
        inet_ntop(AF_INET6, msg->id.idiag_src, src, sizeof(src));
        inet_ntop(AF_INET6, msg->id.idiag_src, dst, sizeof(dst));
        fprintf(stderr, "  v6 [%s]:%d -> [%s]:%d %s\n",
                src, htons(msg->id.idiag_sport),
                dst, htons(msg->id.idiag_dport),
                tcpStateName(msg->idiag_state));
        return 0;
    };

    SockDiag sd;
    ASSERT_TRUE(sd.open()) << "Failed to open SOCK_DIAG socket";

    seenNull = false;
    int ret = sd.sendDumpRequest(IPPROTO_TCP, AF_INET, "127.0.0.1");
    ASSERT_EQ(0, ret) << "Failed to send IPv4 dump request: " << strerror(-ret);
    fprintf(stderr, "Sent IPv4 dump\n");
    sd.readDiagMsg(IPPROTO_TCP, checkIPv4Dump);
    EXPECT_GE(v4SocketsSeen, 1);
    EXPECT_TRUE(seenclient46);
    EXPECT_FALSE(seenServer46);

    seenNull = false;
    ret = sd.sendDumpRequest(IPPROTO_TCP, AF_INET6, "127.0.0.1");
    ASSERT_EQ(0, ret) << "Failed to send mapped dump request: " << strerror(-ret);
    fprintf(stderr, "Sent mapped dump\n");
    sd.readDiagMsg(IPPROTO_TCP, checkIPv6Dump);
    EXPECT_TRUE(seenServer46);

    seenNull = false;
    ret = sd.sendDumpRequest(IPPROTO_TCP, AF_INET6, "::1");
    ASSERT_EQ(0, ret) << "Failed to send IPv6 dump request: " << strerror(-ret);
    fprintf(stderr, "Sent IPv6 dump\n");

    sd.readDiagMsg(IPPROTO_TCP, checkIPv6Dump);
    EXPECT_GE(v6SocketsSeen, 1);
    EXPECT_TRUE(seenClient6);
    EXPECT_TRUE(seenServer6);

    close(v4socket);
    close(v6socket);
    close(listensocket);
    close(accepted4);
    close(accepted6);
}

TEST_F(SockDiagTest, TestMicroBenchmark) {
    fprintf(stderr, "Benchmarking closing %d sockets\n", NUM_SOCKETS);

    int listensocket = socket(AF_INET6, SOCK_STREAM, 0);
    ASSERT_NE(-1, listensocket) << "Failed to open listen socket";

    uint16_t port = bindAndListen(listensocket);
    ASSERT_NE(0, port) << "Can't bind to server port";
    sockaddr_in6 server = { .sin6_family = AF_INET6, .sin6_port = htons(port) };

    using ms = std::chrono::duration<float, std::ratio<1, 1000>>;

    int clientsockets[NUM_SOCKETS], serversockets[NUM_SOCKETS];
    uint16_t clientports[NUM_SOCKETS];
    sockaddr_in6 client;
    socklen_t clientlen;

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < NUM_SOCKETS; i++) {
        int s = socket(AF_INET6, SOCK_STREAM, 0);
        clientlen = sizeof(client);
        ASSERT_EQ(0, connect(s, (sockaddr *) &server, sizeof(server)))
            << "Connecting socket " << i << " failed " << strerror(errno);
        serversockets[i] = accept(listensocket, (sockaddr *) &client, &clientlen);
        ASSERT_NE(-1, serversockets[i])
            << "Accepting socket " << i << " failed " << strerror(errno);
        clientports[i] = client.sin6_port;
        clientsockets[i] = s;
    }
    fprintf(stderr, "  Connecting: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());

    SockDiag sd;
    ASSERT_TRUE(sd.open()) << "Failed to open SOCK_DIAG socket";

    start = std::chrono::steady_clock::now();
    int ret = sd.destroySockets("::1");
    EXPECT_LE(0, ret) << ": Failed to destroy sockets on ::1: " << strerror(-ret);
    fprintf(stderr, "  Destroying: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());

    int err;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < NUM_SOCKETS; i++) {
        ret = send(clientsockets[i], "foo", sizeof("foo"), 0);
        err = errno;
        EXPECT_EQ(-1, ret) << "Client socket " << i << " not closed";
        if (ret == -1) {
            // Since we're connected to ourselves, the error might be ECONNABORTED (if we destroyed
            // the socket) or ECONNRESET (if the other end was destroyed and sent a RST).
            EXPECT_TRUE(errno == ECONNABORTED || errno == ECONNRESET)
                << "Client socket: unexpected error: " << strerror(errno);
        }

        ret = send(serversockets[i], "foo", sizeof("foo"), 0);
        err = errno;
        EXPECT_EQ(-1, ret) << "Server socket " << i << " not closed";
        if (ret == -1) {
            EXPECT_TRUE(errno == ECONNABORTED || errno == ECONNRESET)
                << "Server socket: unexpected error: " << strerror(errno);
        }
    }
    fprintf(stderr, "   Verifying: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());



    start = std::chrono::steady_clock::now();
    for (int i = 0; i < NUM_SOCKETS; i++) {
        close(clientsockets[i]);
        close(serversockets[i]);
    }
    fprintf(stderr, "     Closing: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());

    close(listensocket);
}
