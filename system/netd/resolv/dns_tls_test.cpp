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

#define LOG_TAG "dns_tls_test"
#define LOG_NDEBUG 1  // Set to 0 to enable verbose debug logging

#include <gtest/gtest.h>

#include "DnsTlsDispatcher.h"
#include "DnsTlsQueryMap.h"
#include "DnsTlsServer.h"
#include "DnsTlsSessionCache.h"
#include "DnsTlsSocket.h"
#include "DnsTlsTransport.h"
#include "IDnsTlsSocket.h"
#include "IDnsTlsSocketFactory.h"
#include "IDnsTlsSocketObserver.h"

#include "dns_responder/dns_tls_frontend.h"

#include <chrono>
#include <arpa/inet.h>
#include <android-base/macros.h>
#include <netdutils/Slice.h>

#include "log/log.h"

namespace android {
namespace net {

using netdutils::Slice;
using netdutils::makeSlice;

typedef std::vector<uint8_t> bytevec;

static void parseServer(const char* server, in_port_t port, sockaddr_storage* parsed) {
    sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(parsed);
    if (inet_pton(AF_INET, server, &(sin->sin_addr)) == 1) {
        // IPv4 parse succeeded, so it's IPv4
        sin->sin_family = AF_INET;
        sin->sin_port = htons(port);
        return;
    }
    sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(parsed);
    if (inet_pton(AF_INET6, server, &(sin6->sin6_addr)) == 1){
        // IPv6 parse succeeded, so it's IPv6.
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(port);
        return;
    }
    ALOGE("Failed to parse server address: %s", server);
}

bytevec FINGERPRINT1 = { 1 };
bytevec FINGERPRINT2 = { 2 };

std::string SERVERNAME1 = "dns.example.com";
std::string SERVERNAME2 = "dns.example.org";

// BaseTest just provides constants that are useful for the tests.
class BaseTest : public ::testing::Test {
  protected:
    BaseTest() {
        parseServer("192.0.2.1", 853, &V4ADDR1);
        parseServer("192.0.2.2", 853, &V4ADDR2);
        parseServer("2001:db8::1", 853, &V6ADDR1);
        parseServer("2001:db8::2", 853, &V6ADDR2);

        SERVER1 = DnsTlsServer(V4ADDR1);
        SERVER1.fingerprints.insert(FINGERPRINT1);
        SERVER1.name = SERVERNAME1;
    }

    sockaddr_storage V4ADDR1;
    sockaddr_storage V4ADDR2;
    sockaddr_storage V6ADDR1;
    sockaddr_storage V6ADDR2;

    DnsTlsServer SERVER1;
};

bytevec make_query(uint16_t id, size_t size) {
    bytevec vec(size);
    vec[0] = id >> 8;
    vec[1] = id;
    // Arbitrarily fill the query body with unique data.
    for (size_t i = 2; i < size; ++i) {
        vec[i] = id + i;
    }
    return vec;
}

// Query constants
const unsigned MARK = 123;
const uint16_t ID = 52;
const uint16_t SIZE = 22;
const bytevec QUERY = make_query(ID, SIZE);

template <class T>
class FakeSocketFactory : public IDnsTlsSocketFactory {
  public:
    FakeSocketFactory() {}
    std::unique_ptr<IDnsTlsSocket> createDnsTlsSocket(
            const DnsTlsServer& server ATTRIBUTE_UNUSED,
            unsigned mark ATTRIBUTE_UNUSED,
            IDnsTlsSocketObserver* observer,
            DnsTlsSessionCache* cache ATTRIBUTE_UNUSED) override {
        return std::make_unique<T>(observer);
    }
};

bytevec make_echo(uint16_t id, const Slice query) {
    bytevec response(query.size() + 2);
    response[0] = id >> 8;
    response[1] = id;
    // Echo the query as the fake response.
    memcpy(response.data() + 2, query.base(), query.size());
    return response;
}

// Simplest possible fake server.  This just echoes the query as the response.
class FakeSocketEcho : public IDnsTlsSocket {
  public:
    explicit FakeSocketEcho(IDnsTlsSocketObserver* observer) : mObserver(observer) {}
    bool query(uint16_t id, const Slice query) override {
        // Return the response immediately (asynchronously).
        std::thread(&IDnsTlsSocketObserver::onResponse, mObserver, make_echo(id, query)).detach();
        return true;
    }

  private:
    IDnsTlsSocketObserver* const mObserver;
};

class TransportTest : public BaseTest {};

TEST_F(TransportTest, Query) {
    FakeSocketFactory<FakeSocketEcho> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    auto r = transport.query(makeSlice(QUERY)).get();

    EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
    EXPECT_EQ(QUERY, r.response);
}

// Fake Socket that echoes the observed query ID as the response body.
class FakeSocketId : public IDnsTlsSocket {
  public:
    explicit FakeSocketId(IDnsTlsSocketObserver* observer) : mObserver(observer) {}
    bool query(uint16_t id, const Slice query ATTRIBUTE_UNUSED) override {
        // Return the response immediately (asynchronously).
        bytevec response(4);
        // Echo the ID in the header to match the response to the query.
        // This will be overwritten by DnsTlsQueryMap.
        response[0] = id >> 8;
        response[1] = id;
        // Echo the ID in the body, so that the test can verify which ID was used by
        // DnsTlsQueryMap.
        response[2] = id >> 8;
        response[3] = id;
        std::thread(&IDnsTlsSocketObserver::onResponse, mObserver, response).detach();
        return true;
    }

  private:
    IDnsTlsSocketObserver* const mObserver;
};

// Test that IDs are properly reused
TEST_F(TransportTest, IdReuse) {
    FakeSocketFactory<FakeSocketId> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    for (int i = 0; i < 100; ++i) {
        // Send a query.
        std::future<DnsTlsServer::Result> f = transport.query(makeSlice(QUERY));
        // Wait for the response.
        DnsTlsServer::Result r = f.get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);

        // All queries should have an observed ID of zero, because it is returned to the ID pool
        // after each use.
        EXPECT_EQ(0, (r.response[2] << 8) | r.response[3]);
    }
}

// These queries might be handled in serial or parallel as they race the
// responses.
TEST_F(TransportTest, RacingQueries_10000) {
    FakeSocketFactory<FakeSocketEcho> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    // Fewer than 65536 queries to avoid ID exhaustion.
    const int num_queries = 10000;
    results.reserve(num_queries);
    for (int i = 0; i < num_queries; ++i) {
        results.push_back(transport.query(makeSlice(QUERY)));
    }
    for (auto& result : results) {
        auto r = result.get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(QUERY, r.response);
    }
}

// A server that waits until sDelay queries are queued before responding.
class FakeSocketDelay : public IDnsTlsSocket {
  public:
    explicit FakeSocketDelay(IDnsTlsSocketObserver* observer) : mObserver(observer) {}
    ~FakeSocketDelay() { std::lock_guard guard(mLock); }
    static size_t sDelay;
    static bool sReverse;

    bool query(uint16_t id, const Slice query) override {
        ALOGV("FakeSocketDelay got query with ID %d", int(id));
        std::lock_guard guard(mLock);
        // Check for duplicate IDs.
        EXPECT_EQ(0U, mIds.count(id));
        mIds.insert(id);

        // Store response.
        mResponses.push_back(make_echo(id, query));

        ALOGV("Up to %zu out of %zu queries", mResponses.size(), sDelay);
        if (mResponses.size() == sDelay) {
            std::thread(&FakeSocketDelay::sendResponses, this).detach();
        }
        return true;
    }

  private:
    void sendResponses() {
        std::lock_guard guard(mLock);
        if (sReverse) {
            std::reverse(std::begin(mResponses), std::end(mResponses));
        }
        for (auto& response : mResponses) {
            mObserver->onResponse(response);
        }
        mIds.clear();
        mResponses.clear();
    }

    std::mutex mLock;
    IDnsTlsSocketObserver* const mObserver;
    std::set<uint16_t> mIds GUARDED_BY(mLock);
    std::vector<bytevec> mResponses GUARDED_BY(mLock);
};

size_t FakeSocketDelay::sDelay;
bool FakeSocketDelay::sReverse;

TEST_F(TransportTest, ParallelColliding) {
    FakeSocketDelay::sDelay = 10;
    FakeSocketDelay::sReverse = false;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    // Fewer than 65536 queries to avoid ID exhaustion.
    results.reserve(FakeSocketDelay::sDelay);
    for (size_t i = 0; i < FakeSocketDelay::sDelay; ++i) {
        results.push_back(transport.query(makeSlice(QUERY)));
    }
    for (auto& result : results) {
        auto r = result.get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(QUERY, r.response);
    }
}

TEST_F(TransportTest, ParallelColliding_Max) {
    FakeSocketDelay::sDelay = 65536;
    FakeSocketDelay::sReverse = false;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    // Exactly 65536 queries should still be possible in parallel,
    // even if they all have the same original ID.
    results.reserve(FakeSocketDelay::sDelay);
    for (size_t i = 0; i < FakeSocketDelay::sDelay; ++i) {
        results.push_back(transport.query(makeSlice(QUERY)));
    }
    for (auto& result : results) {
        auto r = result.get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(QUERY, r.response);
    }
}

TEST_F(TransportTest, ParallelUnique) {
    FakeSocketDelay::sDelay = 10;
    FakeSocketDelay::sReverse = false;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<bytevec> queries(FakeSocketDelay::sDelay);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    results.reserve(FakeSocketDelay::sDelay);
    for (size_t i = 0; i < FakeSocketDelay::sDelay; ++i) {
        queries[i] = make_query(i, SIZE);
        results.push_back(transport.query(makeSlice(queries[i])));
    }
    for (size_t i = 0 ; i < FakeSocketDelay::sDelay; ++i) {
        auto r = results[i].get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(queries[i], r.response);
    }
}

TEST_F(TransportTest, ParallelUnique_Max) {
    FakeSocketDelay::sDelay = 65536;
    FakeSocketDelay::sReverse = false;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<bytevec> queries(FakeSocketDelay::sDelay);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    // Exactly 65536 queries should still be possible in parallel,
    // and they should all be mapped correctly back to the original ID.
    results.reserve(FakeSocketDelay::sDelay);
    for (size_t i = 0; i < FakeSocketDelay::sDelay; ++i) {
        queries[i] = make_query(i, SIZE);
        results.push_back(transport.query(makeSlice(queries[i])));
    }
    for (size_t i = 0 ; i < FakeSocketDelay::sDelay; ++i) {
        auto r = results[i].get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(queries[i], r.response);
    }
}

TEST_F(TransportTest, IdExhaustion) {
    const int num_queries = 65536;
    // A delay of 65537 is unreachable, because the maximum number
    // of outstanding queries is 65536.
    FakeSocketDelay::sDelay = num_queries + 1;
    FakeSocketDelay::sReverse = false;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    // Issue the maximum number of queries.
    results.reserve(num_queries);
    for (int i = 0; i < num_queries; ++i) {
        results.push_back(transport.query(makeSlice(QUERY)));
    }

    // The ID space is now full, so subsequent queries should fail immediately.
    auto r = transport.query(makeSlice(QUERY)).get();
    EXPECT_EQ(DnsTlsTransport::Response::internal_error, r.code);
    EXPECT_TRUE(r.response.empty());

    for (auto& result : results) {
        // All other queries should remain outstanding.
        EXPECT_EQ(std::future_status::timeout,
                result.wait_for(std::chrono::duration<int>::zero()));
    }
}

// Responses can come back from the server in any order.  This should have no
// effect on Transport's observed behavior.
TEST_F(TransportTest, ReverseOrder) {
    FakeSocketDelay::sDelay = 10;
    FakeSocketDelay::sReverse = true;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<bytevec> queries(FakeSocketDelay::sDelay);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    results.reserve(FakeSocketDelay::sDelay);
    for (size_t i = 0; i < FakeSocketDelay::sDelay; ++i) {
        queries[i] = make_query(i, SIZE);
        results.push_back(transport.query(makeSlice(queries[i])));
    }
    for (size_t i = 0 ; i < FakeSocketDelay::sDelay; ++i) {
        auto r = results[i].get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(queries[i], r.response);
    }
}

TEST_F(TransportTest, ReverseOrder_Max) {
    FakeSocketDelay::sDelay = 65536;
    FakeSocketDelay::sReverse = true;
    FakeSocketFactory<FakeSocketDelay> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    std::vector<bytevec> queries(FakeSocketDelay::sDelay);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    results.reserve(FakeSocketDelay::sDelay);
    for (size_t i = 0; i < FakeSocketDelay::sDelay; ++i) {
        queries[i] = make_query(i, SIZE);
        results.push_back(transport.query(makeSlice(queries[i])));
    }
    for (size_t i = 0 ; i < FakeSocketDelay::sDelay; ++i) {
        auto r = results[i].get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(queries[i], r.response);
    }
}

// Returning null from the factory indicates a connection failure.
class NullSocketFactory : public IDnsTlsSocketFactory {
  public:
    NullSocketFactory() {}
    std::unique_ptr<IDnsTlsSocket> createDnsTlsSocket(
            const DnsTlsServer& server ATTRIBUTE_UNUSED,
            unsigned mark ATTRIBUTE_UNUSED,
            IDnsTlsSocketObserver* observer ATTRIBUTE_UNUSED,
            DnsTlsSessionCache* cache ATTRIBUTE_UNUSED) override {
        return nullptr;
    }
};

TEST_F(TransportTest, ConnectFail) {
    NullSocketFactory factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    auto r = transport.query(makeSlice(QUERY)).get();

    EXPECT_EQ(DnsTlsTransport::Response::network_error, r.code);
    EXPECT_TRUE(r.response.empty());
}

// Simulate a socket that connects but then immediately receives a server
// close notification.
class FakeSocketClose : public IDnsTlsSocket {
  public:
    explicit FakeSocketClose(IDnsTlsSocketObserver* observer)
        : mCloser(&IDnsTlsSocketObserver::onClosed, observer) {}
    ~FakeSocketClose() { mCloser.join(); }
    bool query(uint16_t id ATTRIBUTE_UNUSED,
               const Slice query ATTRIBUTE_UNUSED) override {
        return true;
    }

  private:
    std::thread mCloser;
};

TEST_F(TransportTest, CloseRetryFail) {
    FakeSocketFactory<FakeSocketClose> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    auto r = transport.query(makeSlice(QUERY)).get();

    EXPECT_EQ(DnsTlsTransport::Response::network_error, r.code);
    EXPECT_TRUE(r.response.empty());
}

// Simulate a server that occasionally closes the connection and silently
// drops some queries.
class FakeSocketLimited : public IDnsTlsSocket {
  public:
    static int sLimit;  // Number of queries to answer per socket.
    static size_t sMaxSize;  // Silently discard queries greater than this size.
    explicit FakeSocketLimited(IDnsTlsSocketObserver* observer)
        : mObserver(observer), mQueries(0) {}
    ~FakeSocketLimited() {
        {
            ALOGV("~FakeSocketLimited acquiring mLock");
            std::lock_guard guard(mLock);
            ALOGV("~FakeSocketLimited acquired mLock");
            for (auto& thread : mThreads) {
                ALOGV("~FakeSocketLimited joining response thread");
                thread.join();
                ALOGV("~FakeSocketLimited joined response thread");
            }
            mThreads.clear();
        }

        if (mCloser) {
            ALOGV("~FakeSocketLimited joining closer thread");
            mCloser->join();
            ALOGV("~FakeSocketLimited joined closer thread");
        }
    }
    bool query(uint16_t id, const Slice query) override {
        ALOGV("FakeSocketLimited::query acquiring mLock");
        std::lock_guard guard(mLock);
        ALOGV("FakeSocketLimited::query acquired mLock");
        ++mQueries;

        if (mQueries <= sLimit) {
            ALOGV("size %zu vs. limit of %zu", query.size(), sMaxSize);
            if (query.size() <= sMaxSize) {
                // Return the response immediately (asynchronously).
                mThreads.emplace_back(&IDnsTlsSocketObserver::onResponse, mObserver, make_echo(id, query));
            }
        }
        if (mQueries == sLimit) {
            mCloser = std::make_unique<std::thread>(&FakeSocketLimited::sendClose, this);
        }
        return mQueries <= sLimit;
    }

  private:
    void sendClose() {
        {
            ALOGV("FakeSocketLimited::sendClose acquiring mLock");
            std::lock_guard guard(mLock);
            ALOGV("FakeSocketLimited::sendClose acquired mLock");
            for (auto& thread : mThreads) {
                ALOGV("FakeSocketLimited::sendClose joining response thread");
                thread.join();
                ALOGV("FakeSocketLimited::sendClose joined response thread");
            }
            mThreads.clear();
        }
        mObserver->onClosed();
    }
    std::mutex mLock;
    IDnsTlsSocketObserver* const mObserver;
    int mQueries GUARDED_BY(mLock);
    std::vector<std::thread> mThreads GUARDED_BY(mLock);
    std::unique_ptr<std::thread> mCloser GUARDED_BY(mLock);
};

int FakeSocketLimited::sLimit;
size_t FakeSocketLimited::sMaxSize;

TEST_F(TransportTest, SilentDrop) {
    FakeSocketLimited::sLimit = 10;  // Close the socket after 10 queries.
    FakeSocketLimited::sMaxSize = 0;  // Silently drop all queries
    FakeSocketFactory<FakeSocketLimited> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);

    // Queue up 10 queries.  They will all be ignored, and after the 10th,
    // the socket will close.  Transport will retry them all, until they
    // all hit the retry limit and expire.
    std::vector<std::future<DnsTlsTransport::Result>> results;
    results.reserve(FakeSocketLimited::sLimit);
    for (int i = 0; i < FakeSocketLimited::sLimit; ++i) {
        results.push_back(transport.query(makeSlice(QUERY)));
    }
    for (auto& result : results) {
        auto r = result.get();
        EXPECT_EQ(DnsTlsTransport::Response::network_error, r.code);
        EXPECT_TRUE(r.response.empty());
    }
}

TEST_F(TransportTest, PartialDrop) {
    FakeSocketLimited::sLimit = 10;  // Close the socket after 10 queries.
    FakeSocketLimited::sMaxSize = SIZE - 2;  // Silently drop "long" queries
    FakeSocketFactory<FakeSocketLimited> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);

    // Queue up 100 queries, alternating "short" which will be served and "long"
    // which will be dropped.
    const int num_queries = 10 * FakeSocketLimited::sLimit;
    std::vector<bytevec> queries(num_queries);
    std::vector<std::future<DnsTlsTransport::Result>> results;
    results.reserve(num_queries);
    for (int i = 0; i < num_queries; ++i) {
        queries[i] = make_query(i, SIZE + (i % 2));
        results.push_back(transport.query(makeSlice(queries[i])));
    }
    // Just check the short queries, which are at the even indices.
    for (int i = 0; i < num_queries; i += 2) {
        auto r = results[i].get();
        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        EXPECT_EQ(queries[i], r.response);
    }
}

// Simulate a malfunctioning server that injects extra miscellaneous
// responses to queries that were not asked.  This will cause wrong answers but
// must not crash the Transport.
class FakeSocketGarbage : public IDnsTlsSocket {
  public:
    explicit FakeSocketGarbage(IDnsTlsSocketObserver* observer) : mObserver(observer) {
        // Inject a garbage event.
        mThreads.emplace_back(&IDnsTlsSocketObserver::onResponse, mObserver, make_query(ID + 1, SIZE));
    }
    ~FakeSocketGarbage() {
        std::lock_guard guard(mLock);
        for (auto& thread : mThreads) {
            thread.join();
        }
    }
    bool query(uint16_t id, const Slice query) override {
        std::lock_guard guard(mLock);
        // Return the response twice.
        auto echo = make_echo(id, query);
        mThreads.emplace_back(&IDnsTlsSocketObserver::onResponse, mObserver, echo);
        mThreads.emplace_back(&IDnsTlsSocketObserver::onResponse, mObserver, echo);
        // Also return some other garbage
        mThreads.emplace_back(&IDnsTlsSocketObserver::onResponse, mObserver, make_query(id + 1, query.size() + 2));
        return true;
    }

  private:
    std::mutex mLock;
    std::vector<std::thread> mThreads GUARDED_BY(mLock);
    IDnsTlsSocketObserver* const mObserver;
};

TEST_F(TransportTest, IgnoringGarbage) {
    FakeSocketFactory<FakeSocketGarbage> factory;
    DnsTlsTransport transport(SERVER1, MARK, &factory);
    for (int i = 0; i < 10; ++i) {
        auto r = transport.query(makeSlice(QUERY)).get();

        EXPECT_EQ(DnsTlsTransport::Response::success, r.code);
        // Don't check the response because this server is malfunctioning.
    }
}

// Dispatcher tests
class DispatcherTest : public BaseTest {};

TEST_F(DispatcherTest, Query) {
    bytevec ans(4096);
    int resplen = 0;

    auto factory = std::make_unique<FakeSocketFactory<FakeSocketEcho>>();
    DnsTlsDispatcher dispatcher(std::move(factory));
    auto r = dispatcher.query(SERVER1, MARK, makeSlice(QUERY),
                              makeSlice(ans), &resplen);

    EXPECT_EQ(DnsTlsTransport::Response::success, r);
    EXPECT_EQ(int(QUERY.size()), resplen);
    ans.resize(resplen);
    EXPECT_EQ(QUERY, ans);
}

TEST_F(DispatcherTest, AnswerTooLarge) {
    bytevec ans(SIZE - 1);  // Too small to hold the answer
    int resplen = 0;

    auto factory = std::make_unique<FakeSocketFactory<FakeSocketEcho>>();
    DnsTlsDispatcher dispatcher(std::move(factory));
    auto r = dispatcher.query(SERVER1, MARK, makeSlice(QUERY),
                              makeSlice(ans), &resplen);

    EXPECT_EQ(DnsTlsTransport::Response::limit_error, r);
}

template<class T>
class TrackingFakeSocketFactory : public IDnsTlsSocketFactory {
  public:
    TrackingFakeSocketFactory() {}
    std::unique_ptr<IDnsTlsSocket> createDnsTlsSocket(
            const DnsTlsServer& server,
            unsigned mark,
            IDnsTlsSocketObserver* observer,
            DnsTlsSessionCache* cache ATTRIBUTE_UNUSED) override {
        std::lock_guard guard(mLock);
        keys.emplace(mark, server);
        return std::make_unique<T>(observer);
    }
    std::multiset<std::pair<unsigned, DnsTlsServer>> keys;

  private:
    std::mutex mLock;
};

TEST_F(DispatcherTest, Dispatching) {
    FakeSocketDelay::sDelay = 5;
    FakeSocketDelay::sReverse = true;
    auto factory = std::make_unique<TrackingFakeSocketFactory<FakeSocketDelay>>();
    auto* weak_factory = factory.get();  // Valid as long as dispatcher is in scope.
    DnsTlsDispatcher dispatcher(std::move(factory));

    // Populate a vector of two servers and two socket marks, four combinations
    // in total.
    std::vector<std::pair<unsigned, DnsTlsServer>> keys;
    keys.emplace_back(MARK, SERVER1);
    keys.emplace_back(MARK + 1, SERVER1);
    keys.emplace_back(MARK, V4ADDR2);
    keys.emplace_back(MARK + 1, V4ADDR2);

    // Do several queries on each server.  They should all succeed.
    std::vector<std::thread> threads;
    for (size_t i = 0; i < FakeSocketDelay::sDelay * keys.size(); ++i) {
        auto key = keys[i % keys.size()];
        threads.emplace_back([key, i] (DnsTlsDispatcher* dispatcher) {
            auto q = make_query(i, SIZE);
            bytevec ans(4096);
            int resplen = 0;
            unsigned mark = key.first;
            const DnsTlsServer& server = key.second;
            auto r = dispatcher->query(server, mark, makeSlice(q),
                                       makeSlice(ans), &resplen);
            EXPECT_EQ(DnsTlsTransport::Response::success, r);
            EXPECT_EQ(int(q.size()), resplen);
            ans.resize(resplen);
            EXPECT_EQ(q, ans);
        }, &dispatcher);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    // We expect that the factory created one socket for each key.
    EXPECT_EQ(keys.size(), weak_factory->keys.size());
    for (auto& key : keys) {
        EXPECT_EQ(1U, weak_factory->keys.count(key));
    }
}

// Check DnsTlsServer's comparison logic.
AddressComparator ADDRESS_COMPARATOR;
bool isAddressEqual(const DnsTlsServer& s1, const DnsTlsServer& s2) {
    bool cmp1 = ADDRESS_COMPARATOR(s1, s2);
    bool cmp2 = ADDRESS_COMPARATOR(s2, s1);
    EXPECT_FALSE(cmp1 && cmp2);
    return !cmp1 && !cmp2;
}

void checkUnequal(const DnsTlsServer& s1, const DnsTlsServer& s2) {
    EXPECT_TRUE(s1 == s1);
    EXPECT_TRUE(s2 == s2);
    EXPECT_TRUE(isAddressEqual(s1, s1));
    EXPECT_TRUE(isAddressEqual(s2, s2));

    EXPECT_TRUE(s1 < s2 ^ s2 < s1);
    EXPECT_FALSE(s1 == s2);
    EXPECT_FALSE(s2 == s1);
}

class ServerTest : public BaseTest {};

TEST_F(ServerTest, IPv4) {
    checkUnequal(V4ADDR1, V4ADDR2);
    EXPECT_FALSE(isAddressEqual(V4ADDR1, V4ADDR2));
}

TEST_F(ServerTest, IPv6) {
    checkUnequal(V6ADDR1, V6ADDR2);
    EXPECT_FALSE(isAddressEqual(V6ADDR1, V6ADDR2));
}

TEST_F(ServerTest, MixedAddressFamily) {
    checkUnequal(V6ADDR1, V4ADDR1);
    EXPECT_FALSE(isAddressEqual(V6ADDR1, V4ADDR1));
}

TEST_F(ServerTest, IPv6ScopeId) {
    DnsTlsServer s1(V6ADDR1), s2(V6ADDR1);
    sockaddr_in6* addr1 = reinterpret_cast<sockaddr_in6*>(&s1.ss);
    addr1->sin6_scope_id = 1;
    sockaddr_in6* addr2 = reinterpret_cast<sockaddr_in6*>(&s2.ss);
    addr2->sin6_scope_id = 2;
    checkUnequal(s1, s2);
    EXPECT_FALSE(isAddressEqual(s1, s2));

    EXPECT_FALSE(s1.wasExplicitlyConfigured());
    EXPECT_FALSE(s2.wasExplicitlyConfigured());
}

TEST_F(ServerTest, IPv6FlowInfo) {
    DnsTlsServer s1(V6ADDR1), s2(V6ADDR1);
    sockaddr_in6* addr1 = reinterpret_cast<sockaddr_in6*>(&s1.ss);
    addr1->sin6_flowinfo = 1;
    sockaddr_in6* addr2 = reinterpret_cast<sockaddr_in6*>(&s2.ss);
    addr2->sin6_flowinfo = 2;
    // All comparisons ignore flowinfo.
    EXPECT_EQ(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    EXPECT_FALSE(s1.wasExplicitlyConfigured());
    EXPECT_FALSE(s2.wasExplicitlyConfigured());
}

TEST_F(ServerTest, Port) {
    DnsTlsServer s1, s2;
    parseServer("192.0.2.1", 853, &s1.ss);
    parseServer("192.0.2.1", 854, &s2.ss);
    checkUnequal(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    DnsTlsServer s3, s4;
    parseServer("2001:db8::1", 853, &s3.ss);
    parseServer("2001:db8::1", 852, &s4.ss);
    checkUnequal(s3, s4);
    EXPECT_TRUE(isAddressEqual(s3, s4));

    EXPECT_FALSE(s1.wasExplicitlyConfigured());
    EXPECT_FALSE(s2.wasExplicitlyConfigured());
}

TEST_F(ServerTest, Name) {
    DnsTlsServer s1(V4ADDR1), s2(V4ADDR1);
    s1.name = SERVERNAME1;
    checkUnequal(s1, s2);
    s2.name = SERVERNAME2;
    checkUnequal(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    EXPECT_TRUE(s1.wasExplicitlyConfigured());
    EXPECT_TRUE(s2.wasExplicitlyConfigured());
}

TEST_F(ServerTest, Fingerprint) {
    DnsTlsServer s1(V4ADDR1), s2(V4ADDR1);

    s1.fingerprints.insert(FINGERPRINT1);
    checkUnequal(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    s2.fingerprints.insert(FINGERPRINT2);
    checkUnequal(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    s2.fingerprints.insert(FINGERPRINT1);
    checkUnequal(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    s1.fingerprints.insert(FINGERPRINT2);
    EXPECT_EQ(s1, s2);
    EXPECT_TRUE(isAddressEqual(s1, s2));

    EXPECT_TRUE(s1.wasExplicitlyConfigured());
    EXPECT_TRUE(s2.wasExplicitlyConfigured());
}

TEST(QueryMapTest, Basic) {
    DnsTlsQueryMap map;

    EXPECT_TRUE(map.empty());

    bytevec q0 = make_query(999, SIZE);
    bytevec q1 = make_query(888, SIZE);
    bytevec q2 = make_query(777, SIZE);

    auto f0 = map.recordQuery(makeSlice(q0));
    auto f1 = map.recordQuery(makeSlice(q1));
    auto f2 = map.recordQuery(makeSlice(q2));

    // Check return values of recordQuery
    EXPECT_EQ(0, f0->query.newId);
    EXPECT_EQ(1, f1->query.newId);
    EXPECT_EQ(2, f2->query.newId);

    // Check side effects of recordQuery
    EXPECT_FALSE(map.empty());

    auto all = map.getAll();
    EXPECT_EQ(3U, all.size());

    EXPECT_EQ(0, all[0].newId);
    EXPECT_EQ(1, all[1].newId);
    EXPECT_EQ(2, all[2].newId);

    EXPECT_EQ(makeSlice(q0), all[0].query);
    EXPECT_EQ(makeSlice(q1), all[1].query);
    EXPECT_EQ(makeSlice(q2), all[2].query);

    bytevec a0 = make_query(0, SIZE);
    bytevec a1 = make_query(1, SIZE);
    bytevec a2 = make_query(2, SIZE);

    // Return responses out of order
    map.onResponse(a2);
    map.onResponse(a0);
    map.onResponse(a1);

    EXPECT_TRUE(map.empty());

    auto r0 = f0->result.get();
    auto r1 = f1->result.get();
    auto r2 = f2->result.get();

    EXPECT_EQ(DnsTlsQueryMap::Response::success, r0.code);
    EXPECT_EQ(DnsTlsQueryMap::Response::success, r1.code);
    EXPECT_EQ(DnsTlsQueryMap::Response::success, r2.code);

    const bytevec& d0 = r0.response;
    const bytevec& d1 = r1.response;
    const bytevec& d2 = r2.response;

    // The ID should match the query
    EXPECT_EQ(999, d0[0] << 8 | d0[1]);
    EXPECT_EQ(888, d1[0] << 8 | d1[1]);
    EXPECT_EQ(777, d2[0] << 8 | d2[1]);
    // The body should match the answer
    EXPECT_EQ(bytevec(a0.begin() + 2, a0.end()), bytevec(d0.begin() + 2, d0.end()));
    EXPECT_EQ(bytevec(a1.begin() + 2, a1.end()), bytevec(d1.begin() + 2, d1.end()));
    EXPECT_EQ(bytevec(a2.begin() + 2, a2.end()), bytevec(d2.begin() + 2, d2.end()));
}

TEST(QueryMapTest, FillHole) {
    DnsTlsQueryMap map;
    std::vector<std::unique_ptr<DnsTlsQueryMap::QueryFuture>> futures(UINT16_MAX + 1);
    for (uint32_t i = 0; i <= UINT16_MAX; ++i) {
        futures[i] = map.recordQuery(makeSlice(QUERY));
        ASSERT_TRUE(futures[i]);  // answers[i] should be nonnull.
        EXPECT_EQ(i, futures[i]->query.newId);
    }

    // The map should now be full.
    EXPECT_EQ(size_t(UINT16_MAX + 1), map.getAll().size());

    // Trying to add another query should fail because the map is full.
    EXPECT_FALSE(map.recordQuery(makeSlice(QUERY)));

    // Send an answer to query 40000
    auto answer = make_query(40000, SIZE);
    map.onResponse(answer);
    auto result = futures[40000]->result.get();
    EXPECT_EQ(DnsTlsQueryMap::Response::success, result.code);
    EXPECT_EQ(ID, result.response[0] << 8 | result.response[1]);
    EXPECT_EQ(bytevec(answer.begin() + 2, answer.end()),
              bytevec(result.response.begin() + 2, result.response.end()));

    // There should now be room in the map.
    EXPECT_EQ(size_t(UINT16_MAX), map.getAll().size());
    auto f = map.recordQuery(makeSlice(QUERY));
    ASSERT_TRUE(f);
    EXPECT_EQ(40000, f->query.newId);

    // The map should now be full again.
    EXPECT_EQ(size_t(UINT16_MAX + 1), map.getAll().size());
    EXPECT_FALSE(map.recordQuery(makeSlice(QUERY)));
}

class StubObserver : public IDnsTlsSocketObserver {
  public:
    bool closed = false;
    void onResponse(std::vector<uint8_t>) override {}

    void onClosed() override { closed = true; }
};

TEST(DnsTlsSocketTest, SlowDestructor) {
    constexpr char tls_addr[] = "127.0.0.3";
    constexpr char tls_port[] = "8530";  // High-numbered port so root isn't required.
    // This test doesn't perform any queries, so the backend address can be invalid.
    constexpr char backend_addr[] = "192.0.2.1";
    constexpr char backend_port[] = "1";

    test::DnsTlsFrontend tls(tls_addr, tls_port, backend_addr, backend_port);
    ASSERT_TRUE(tls.startServer());

    DnsTlsServer server;
    parseServer(tls_addr, 8530, &server.ss);

    StubObserver observer;
    ASSERT_FALSE(observer.closed);
    DnsTlsSessionCache cache;
    auto socket = std::make_unique<DnsTlsSocket>(server, MARK, &observer, &cache);
    ASSERT_TRUE(socket->initialize());

    // Test: Time the socket destructor.  This should be fast.
    auto before = std::chrono::steady_clock::now();
    socket.reset();
    auto after = std::chrono::steady_clock::now();
    auto delay = after - before;
    ALOGV("Shutdown took %lld ns", delay / std::chrono::nanoseconds{1});
    EXPECT_TRUE(observer.closed);
    // Shutdown should complete in milliseconds, but if the shutdown signal is lost
    // it will wait for the timeout, which is expected to take 20seconds.
    EXPECT_LT(delay, std::chrono::seconds{5});
}

} // end of namespace net
} // end of namespace android
