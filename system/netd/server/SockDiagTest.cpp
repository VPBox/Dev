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

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <linux/inet_diag.h>

#include <gtest/gtest.h>

#include "Fwmark.h"
#include "NetdConstants.h"
#include "SockDiag.h"
#include "UidRanges.h"

namespace android {
namespace net {

class SockDiagTest : public ::testing::Test {
protected:
    static bool isLoopbackSocket(const inet_diag_msg *msg) {
        return SockDiag::isLoopbackSocket(msg);
    };
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
    int v4socket = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    ASSERT_NE(-1, v4socket) << "Failed to open IPv4 socket: " << strerror(errno);
    int v6socket = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC, 0);
    ASSERT_NE(-1, v6socket) << "Failed to open IPv6 socket: " << strerror(errno);
    int listensocket = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC, 0);
    ASSERT_NE(-1, listensocket) << "Failed to open listen socket: " << strerror(errno);

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
    int accepted4 = accept4(
            listensocket, (sockaddr *) &client46, &clientlen, SOCK_CLOEXEC);
    int accepted6 = accept4(
            listensocket, (sockaddr *) &client6, &clientlen, SOCK_CLOEXEC);
    ASSERT_NE(-1, accepted4);
    ASSERT_NE(-1, accepted6);

    int v4SocketsSeen = 0;
    bool seenclient46 = false;
    char src[INET6_ADDRSTRLEN], dst[INET6_ADDRSTRLEN];

    fprintf(stderr, "Ports:\n  server=%d. client46=%d, client6=%d\n",
            port, ntohs(client46.sin6_port), ntohs(client6.sin6_port));

    auto checkIPv4Dump = [&] (uint8_t /* proto */, const inet_diag_msg *msg) {
        EXPECT_EQ(htonl(INADDR_LOOPBACK), msg->id.idiag_src[0]);
        v4SocketsSeen++;
        seenclient46 |= (msg->id.idiag_sport == client46.sin6_port);
        inet_ntop(AF_INET, msg->id.idiag_src, src, sizeof(src));
        inet_ntop(AF_INET, msg->id.idiag_src, dst, sizeof(dst));
        fprintf(stderr, "  v4 %s:%d -> %s:%d %s\n",
                src, htons(msg->id.idiag_sport),
                dst, htons(msg->id.idiag_dport),
                tcpStateName(msg->idiag_state));
        if (msg->idiag_state == TCP_ESTABLISHED) {
            EXPECT_TRUE(isLoopbackSocket(msg));
        }
        return false;
    };

    int v6SocketsSeen = 0;
    bool seenClient6 = false, seenServer46 = false, seenServer6 = false;

    auto checkIPv6Dump = [&] (uint8_t /* proto */, const inet_diag_msg *msg) {
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
        if (msg->idiag_state == TCP_ESTABLISHED) {
            EXPECT_TRUE(isLoopbackSocket(msg));
        }
        return false;
    };

    SockDiag sd;
    ASSERT_TRUE(sd.open()) << "Failed to open SOCK_DIAG socket";

    int ret = sd.sendDumpRequest(IPPROTO_TCP, AF_INET, "127.0.0.1");
    ASSERT_EQ(0, ret) << "Failed to send IPv4 dump request: " << strerror(-ret);
    fprintf(stderr, "Sent IPv4 dump\n");
    sd.readDiagMsg(IPPROTO_TCP, checkIPv4Dump);
    EXPECT_GE(v4SocketsSeen, 1);
    EXPECT_TRUE(seenclient46);
    EXPECT_FALSE(seenServer46);

    ret = sd.sendDumpRequest(IPPROTO_TCP, AF_INET6, "127.0.0.1");
    ASSERT_EQ(0, ret) << "Failed to send mapped dump request: " << strerror(-ret);
    fprintf(stderr, "Sent mapped dump\n");
    sd.readDiagMsg(IPPROTO_TCP, checkIPv6Dump);
    EXPECT_TRUE(seenServer46);

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

bool fillDiagAddr(__be32 addr[4], const sockaddr *sa) {
    switch (sa->sa_family) {
        case AF_INET: {
            sockaddr_in *sin = (sockaddr_in *) sa;
            memcpy(addr, &sin->sin_addr, sizeof(sin->sin_addr));
            return true;
        }
        case AF_INET6: {
            sockaddr_in6 *sin6 = (sockaddr_in6 *) sa;
            memcpy(addr, &sin6->sin6_addr, sizeof(sin6->sin6_addr));
            return true;
        }
        default:
            return false;
    }
}

inet_diag_msg makeDiagMessage(__u8 family,  const sockaddr *src, const sockaddr *dst) {
    inet_diag_msg msg = {
        .idiag_family = family,
        .idiag_state = TCP_ESTABLISHED,
        .idiag_uid = AID_APP + 123,
        .idiag_inode = 123456789,
        .id = {
            .idiag_sport = 1234,
            .idiag_dport = 4321,
        }
    };
    EXPECT_TRUE(fillDiagAddr(msg.id.idiag_src, src));
    EXPECT_TRUE(fillDiagAddr(msg.id.idiag_dst, dst));
    return msg;
}

inet_diag_msg makeDiagMessage(const char* srcstr, const char* dststr) {
    addrinfo hints = { .ai_flags = AI_NUMERICHOST }, *src, *dst;
    EXPECT_EQ(0, getaddrinfo(srcstr, nullptr, &hints, &src));
    EXPECT_EQ(0, getaddrinfo(dststr, nullptr, &hints, &dst));
    EXPECT_EQ(src->ai_addr->sa_family, dst->ai_addr->sa_family);
    inet_diag_msg msg = makeDiagMessage(src->ai_addr->sa_family, src->ai_addr, dst->ai_addr);
    freeaddrinfo(src);
    freeaddrinfo(dst);
    return msg;
}

TEST_F(SockDiagTest, TestIsLoopbackSocket) {
    inet_diag_msg msg;

    msg = makeDiagMessage("127.0.0.1", "127.0.0.1");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("::1", "::1");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("::1", "::ffff:127.0.0.1");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("192.0.2.1", "192.0.2.1");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("192.0.2.1", "8.8.8.8");
    EXPECT_FALSE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("192.0.2.1", "127.0.0.1");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("2001:db8::1", "2001:db8::1");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("2001:db8::1", "2001:4860:4860::6464");
    EXPECT_FALSE(isLoopbackSocket(&msg));

    // While isLoopbackSocket returns true on these sockets, we usually don't want to close them
    // because they aren't specific to any particular network and thus don't become unusable when
    // an app's routing changes or its network access is removed.
    //
    // This isn't a problem, as anything that calls destroyLiveSockets will skip them because
    // destroyLiveSockets only enumerates ESTABLISHED, SYN_SENT, and SYN_RECV sockets.
    msg = makeDiagMessage("127.0.0.1", "0.0.0.0");
    EXPECT_TRUE(isLoopbackSocket(&msg));

    msg = makeDiagMessage("::1", "::");
    EXPECT_TRUE(isLoopbackSocket(&msg));
}

enum MicroBenchmarkTestType {
    ADDRESS,
    UID,
    UID_EXCLUDE_LOOPBACK,
    UIDRANGE,
    UIDRANGE_EXCLUDE_LOOPBACK,
    PERMISSION,
};

const char *testTypeName(MicroBenchmarkTestType mode) {
#define TO_STRING_TYPE(x) case ((x)): return #x;
    switch((mode)) {
        TO_STRING_TYPE(ADDRESS);
        TO_STRING_TYPE(UID);
        TO_STRING_TYPE(UID_EXCLUDE_LOOPBACK);
        TO_STRING_TYPE(UIDRANGE);
        TO_STRING_TYPE(UIDRANGE_EXCLUDE_LOOPBACK);
        TO_STRING_TYPE(PERMISSION);
    }
#undef TO_STRING_TYPE
}

static struct {
    unsigned netId;
    bool explicitlySelected;
    Permission permission;
} permissionTestcases[] = {
    { 42, false, PERMISSION_NONE,    },
    { 42, false, PERMISSION_NETWORK, },
    { 42, false, PERMISSION_SYSTEM,  },
    { 42, true,  PERMISSION_NONE,    },
    { 42, true,  PERMISSION_NETWORK, },
    { 42, true,  PERMISSION_SYSTEM,  },
    { 43, false, PERMISSION_NONE,    },
    { 43, false, PERMISSION_NETWORK, },
    { 43, false, PERMISSION_SYSTEM,  },
    { 43, true,  PERMISSION_NONE,    },
    { 43, true,  PERMISSION_NETWORK, },
    { 43, true,  PERMISSION_SYSTEM,  },
};

class SockDiagMicroBenchmarkTest : public ::testing::TestWithParam<MicroBenchmarkTestType> {

public:
    void SetUp() {
        ASSERT_TRUE(mSd.open()) << "Failed to open SOCK_DIAG socket";
    }

protected:
    SockDiag mSd;

    constexpr static int MAX_SOCKETS = 500;
    constexpr static int ADDRESS_SOCKETS = 500;
    constexpr static int UID_SOCKETS = 50;
    constexpr static int PERMISSION_SOCKETS = 16;

    constexpr static uid_t START_UID = 8000;  // START_UID + number of sockets must be <= 9999.
    constexpr static int CLOSE_UID = START_UID + UID_SOCKETS - 42;  // Close to the end
    static_assert(START_UID + MAX_SOCKETS < 9999, "Too many sockets");

    constexpr static int TEST_NETID = 42;  // One of the OEM netIds.


    int howManySockets() {
        MicroBenchmarkTestType mode = GetParam();
        switch (mode) {
        case ADDRESS:
            return ADDRESS_SOCKETS;
        case UID:
        case UID_EXCLUDE_LOOPBACK:
        case UIDRANGE:
        case UIDRANGE_EXCLUDE_LOOPBACK:
            return UID_SOCKETS;
        case PERMISSION:
            return ARRAY_SIZE(permissionTestcases);
        }
    }

    int modifySocketForTest(int s, int i) {
        MicroBenchmarkTestType mode = GetParam();
        switch (mode) {
        case UID:
        case UID_EXCLUDE_LOOPBACK:
        case UIDRANGE:
        case UIDRANGE_EXCLUDE_LOOPBACK: {
            uid_t uid = START_UID + i;
            return fchown(s, uid, -1);
        }
        case PERMISSION: {
            Fwmark fwmark;
            fwmark.netId = permissionTestcases[i].netId;
            fwmark.explicitlySelected = permissionTestcases[i].explicitlySelected;
            fwmark.permission = permissionTestcases[i].permission;
            return setsockopt(s, SOL_SOCKET, SO_MARK, &fwmark.intValue, sizeof(fwmark.intValue));
        }
        default:
            return 0;
        }
    }

    int destroySockets() {
        MicroBenchmarkTestType mode = GetParam();
        int ret;
        switch (mode) {
            case ADDRESS:
                ret = mSd.destroySockets("::1");
                EXPECT_LE(0, ret) << ": Failed to destroy sockets on ::1: " << strerror(-ret);
                break;
            case UID:
            case UID_EXCLUDE_LOOPBACK: {
                bool excludeLoopback = (mode == UID_EXCLUDE_LOOPBACK);
                ret = mSd.destroySockets(IPPROTO_TCP, CLOSE_UID, excludeLoopback);
                EXPECT_LE(0, ret) << ": Failed to destroy sockets for UID " << CLOSE_UID << ": " <<
                        strerror(-ret);
                break;
            }
            case UIDRANGE:
            case UIDRANGE_EXCLUDE_LOOPBACK: {
                bool excludeLoopback = (mode == UIDRANGE_EXCLUDE_LOOPBACK);
                const char *uidRangeStrings[] = { "8005-8012", "8042", "8043", "8090-8099" };
                std::set<uid_t> skipUids { 8007, 8043, 8098, 8099 };
                UidRanges uidRanges;
                uidRanges.parseFrom(ARRAY_SIZE(uidRangeStrings), (char **) uidRangeStrings);
                ret = mSd.destroySockets(uidRanges, skipUids, excludeLoopback);
                break;
            }
            case PERMISSION: {
                ret = mSd.destroySocketsLackingPermission(TEST_NETID, PERMISSION_NETWORK, false);
                break;
            }
        }
        return ret;
    }

    bool shouldHaveClosedSocket(int i) {
        MicroBenchmarkTestType mode = GetParam();
        switch (mode) {
            case ADDRESS:
                return true;
            case UID:
                return i == CLOSE_UID - START_UID;
            case UIDRANGE: {
                uid_t uid = i + START_UID;
                // Skip UIDs in skipUids.
                if (uid == 8007 || uid == 8043 || uid == 8098 || uid == 8099) {
                    return false;
                }
                // Include UIDs in uidRanges.
                if ((8005 <= uid && uid <= 8012) || uid == 8042 || (8090 <= uid && uid <= 8099)) {
                    return true;
                }
                return false;
            }
            case UID_EXCLUDE_LOOPBACK:
            case UIDRANGE_EXCLUDE_LOOPBACK:
                return false;
            case PERMISSION:
                if (permissionTestcases[i].netId != 42) return false;
                if (permissionTestcases[i].explicitlySelected != 1) return true;
                Permission permission = permissionTestcases[i].permission;
                return permission != PERMISSION_NETWORK && permission != PERMISSION_SYSTEM;
        }
    }

    bool checkSocketState(int i, int sock, const char *msg) {
        const char data[] = "foo";
        const int ret = send(sock, data, sizeof(data), 0);
        const int err = errno;
        if (!shouldHaveClosedSocket(i)) {
            EXPECT_EQ((ssize_t) sizeof(data), ret) <<
                    "Write on open socket failed: " << strerror(err);
            return false;
        }

        EXPECT_EQ(-1, ret) << msg << " " << i << " not closed";
        if (ret != -1) {
            return false;
        }

        // Since we're connected to ourselves, the error might be ECONNABORTED (if we destroyed the
        // socket) or ECONNRESET (if the other end was destroyed and sent a RST).
        EXPECT_TRUE(err == ECONNABORTED || err == ECONNRESET)
            << msg << ": unexpected error: " << strerror(err);
        return (err == ECONNABORTED);  // Return true iff. SOCK_DESTROY closed this socket.
    }
};

TEST_P(SockDiagMicroBenchmarkTest, TestMicroBenchmark) {
    MicroBenchmarkTestType mode = GetParam();

    int numSockets = howManySockets();

    fprintf(stderr, "Benchmarking closing %d sockets based on %s\n",
            numSockets, testTypeName(mode));

    int listensocket = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC, 0);
    ASSERT_NE(-1, listensocket) << "Failed to open listen socket";

    uint16_t port = bindAndListen(listensocket);
    ASSERT_NE(0, port) << "Can't bind to server port";
    sockaddr_in6 server = { .sin6_family = AF_INET6, .sin6_port = htons(port) };

    using ms = std::chrono::duration<float, std::ratio<1, 1000>>;

    int clientsockets[MAX_SOCKETS], serversockets[MAX_SOCKETS];
    uint16_t clientports[MAX_SOCKETS];
    sockaddr_in6 client;
    socklen_t clientlen;

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < numSockets; i++) {
        int s = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC, 0);
        clientlen = sizeof(client);
        ASSERT_EQ(0, connect(s, (sockaddr *) &server, sizeof(server)))
            << "Connecting socket " << i << " failed " << strerror(errno);
        ASSERT_EQ(0, modifySocketForTest(s, i));
        serversockets[i] = accept4(
                listensocket, (sockaddr *) &client, &clientlen, SOCK_CLOEXEC);
        ASSERT_NE(-1, serversockets[i])
            << "Accepting socket " << i << " failed " << strerror(errno);
        clientports[i] = client.sin6_port;
        clientsockets[i] = s;
    }
    fprintf(stderr, "  Connecting: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());

    start = std::chrono::steady_clock::now();
    destroySockets();
    fprintf(stderr, "  Destroying: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());

    start = std::chrono::steady_clock::now();
    int socketsClosed = 0;
    for (int i = 0; i < numSockets; i++) {
        socketsClosed += checkSocketState(i, clientsockets[i], "Client socket");
        socketsClosed += checkSocketState(i, serversockets[i], "Server socket");
    }
    fprintf(stderr, "   Verifying: %6.1f ms (%d sockets destroyed)\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count(),
            socketsClosed);
    if (strstr(testTypeName(mode), "_EXCLUDE_LOOPBACK") == nullptr) {
        EXPECT_GT(socketsClosed, 0);  // Just in case there's a bug in the test.
    }

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < numSockets; i++) {
        close(clientsockets[i]);
        close(serversockets[i]);
    }
    fprintf(stderr, "     Closing: %6.1f ms\n",
            std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - start).count());

    close(listensocket);
}

// "SockDiagTest.cpp:232: error: undefined reference to 'SockDiagMicroBenchmarkTest::CLOSE_UID'".
constexpr int SockDiagMicroBenchmarkTest::CLOSE_UID;

INSTANTIATE_TEST_CASE_P(Address, SockDiagMicroBenchmarkTest,
                        testing::Values(ADDRESS, UID, UIDRANGE,
                                        UID_EXCLUDE_LOOPBACK, UIDRANGE_EXCLUDE_LOOPBACK,
                                        PERMISSION));

}  // namespace net
}  // namespace android
