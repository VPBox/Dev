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

#ifndef _DNS_DNSTLSDISPATCHER_H
#define _DNS_DNSTLSDISPATCHER_H

#include <list>
#include <map>
#include <memory>
#include <mutex>

#include <android-base/thread_annotations.h>
#include <netdutils/Slice.h>

#include "DnsTlsServer.h"
#include "DnsTlsTransport.h"
#include "IDnsTlsSocketFactory.h"
#include "resolv_private.h"

namespace android {
namespace net {

// This is a singleton class that manages the collection of active DnsTlsTransports.
// Queries made here are dispatched to an existing or newly constructed DnsTlsTransport.
class DnsTlsDispatcher {
  public:
    // Default constructor.
    DnsTlsDispatcher();

    // Constructor with dependency injection for testing.
    explicit DnsTlsDispatcher(std::unique_ptr<IDnsTlsSocketFactory> factory)
        : mFactory(std::move(factory)) {}

    // Enqueues |query| for resolution via the given |tlsServers| on the
    // network indicated by |mark|; writes the response into |ans|, and stores
    // the count of bytes written in |resplen|. Returns a success or error code.
    // The order in which servers from |tlsServers| are queried may not be the
    // order passed in by the caller.
    DnsTlsTransport::Response query(const std::list<DnsTlsServer>& tlsServers,
                                    res_state _Nonnull statp, const netdutils::Slice query,
                                    const netdutils::Slice ans, int* _Nonnull resplen);

    // Given a |query|, sends it to the server on the network indicated by |mark|,
    // and writes the response into |ans|,  and indicates
    // the number of bytes written in |resplen|.  Returns a success or error code.
    DnsTlsTransport::Response query(const DnsTlsServer& server, unsigned mark,
                                    const netdutils::Slice query, const netdutils::Slice ans,
                                    int* _Nonnull resplen);

  private:
    // This lock is static so that it can be used to annotate the Transport struct.
    // DnsTlsDispatcher is a singleton in practice, so making this static does not change
    // the locking behavior.
    static std::mutex sLock;

    // Key = <mark, server>
    typedef std::pair<unsigned, const DnsTlsServer> Key;

    // Transport is a thin wrapper around DnsTlsTransport, adding reference counting and
    // usage monitoring so we can expire idle sessions from the cache.
    struct Transport {
        Transport(const DnsTlsServer& server, unsigned mark, IDnsTlsSocketFactory* _Nonnull factory)
            : transport(server, mark, factory) {}
        // DnsTlsTransport is thread-safe, so it doesn't need to be guarded.
        DnsTlsTransport transport;
        // This use counter and timestamp are used to ensure that only idle sessions are
        // destroyed.
        int useCount GUARDED_BY(sLock) = 0;
        // lastUsed is only guaranteed to be meaningful after useCount is decremented to zero.
        std::chrono::time_point<std::chrono::steady_clock> lastUsed GUARDED_BY(sLock);
    };

    // Cache of reusable DnsTlsTransports.  Transports stay in cache as long as
    // they are in use and for a few minutes after.
    // The key is a (netid, server) pair.  The netid is first for lexicographic comparison speed.
    std::map<Key, std::unique_ptr<Transport>> mStore GUARDED_BY(sLock);

    // The last time we did a cleanup.  For efficiency, we only perform a cleanup once every
    // few minutes.
    std::chrono::time_point<std::chrono::steady_clock> mLastCleanup GUARDED_BY(sLock);

    // Drop any cache entries whose useCount is zero and which have not been used recently.
    // This function performs a linear scan of mStore.
    void cleanup(std::chrono::time_point<std::chrono::steady_clock> now) REQUIRES(sLock);

    // Return a sorted list of DnsTlsServers in preference order.
    std::list<DnsTlsServer> getOrderedServerList(const std::list<DnsTlsServer>& tlsServers,
                                                 unsigned mark) const;

    // Trivial factory for DnsTlsSockets.  Dependency injection is only used for testing.
    std::unique_ptr<IDnsTlsSocketFactory> mFactory;
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_DNSTLSDISPATCHER_H
