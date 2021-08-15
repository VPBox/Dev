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

#ifndef _DNS_DNSTLSTRANSPORT_H
#define _DNS_DNSTLSTRANSPORT_H

#include <future>
#include <map>
#include <mutex>
#include <vector>

#include <android-base/thread_annotations.h>
#include <android-base/unique_fd.h>
#include <netdutils/Slice.h>

#include "DnsTlsQueryMap.h"
#include "DnsTlsServer.h"
#include "DnsTlsSessionCache.h"
#include "IDnsTlsSocket.h"
#include "IDnsTlsSocketObserver.h"

namespace android {
namespace net {

class IDnsTlsSocketFactory;

// Manages at most one DnsTlsSocket at a time.  This class handles socket lifetime issues,
// such as reopening the socket and reissuing pending queries.
class DnsTlsTransport : public IDnsTlsSocketObserver {
  public:
    DnsTlsTransport(const DnsTlsServer& server, unsigned mark,
                    IDnsTlsSocketFactory* _Nonnull factory)
        : mMark(mark), mServer(server), mFactory(factory) {}
    ~DnsTlsTransport();

    typedef DnsTlsServer::Response Response;
    typedef DnsTlsServer::Result Result;

    // Given a |query|, this method sends it to the server and returns the result asynchronously.
    std::future<Result> query(const netdutils::Slice query) EXCLUDES(mLock);

    // Check that a given TLS server is fully working on the specified netid, and has the
    // provided SHA-256 fingerprint (if nonempty).  This function is used in ResolverController
    // to ensure that we don't enable DNS over TLS on networks where it doesn't actually work.
    static bool validate(const DnsTlsServer& server, unsigned netid, uint32_t mark);

    // Implement IDnsTlsSocketObserver
    void onResponse(std::vector<uint8_t> response) override;
    void onClosed() override EXCLUDES(mLock);

  private:
    std::mutex mLock;

    DnsTlsSessionCache mCache;
    DnsTlsQueryMap mQueries;

    const unsigned mMark;  // Socket mark
    const DnsTlsServer mServer;
    IDnsTlsSocketFactory* _Nonnull const mFactory;

    void doConnect() REQUIRES(mLock);

    // doReconnect is used by onClosed.  It runs on the reconnect thread.
    void doReconnect() EXCLUDES(mLock);
    std::unique_ptr<std::thread> mReconnectThread GUARDED_BY(mLock);

    // Used to prevent onClosed from starting a reconnect during the destructor.
    bool mClosing GUARDED_BY(mLock) = false;

    // Sending queries on the socket is thread-safe, but construction/destruction is not.
    std::unique_ptr<IDnsTlsSocket> mSocket GUARDED_BY(mLock);

    // Send a query to the socket.
    bool sendQuery(const DnsTlsQueryMap::Query q) REQUIRES(mLock);
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_DNSTLSTRANSPORT_H
