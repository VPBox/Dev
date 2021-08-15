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

#ifndef _DNS_DNSTLSQUERYMAP_H
#define _DNS_DNSTLSQUERYMAP_H

#include <future>
#include <map>
#include <mutex>
#include <vector>

#include <android-base/thread_annotations.h>
#include <netdutils/Slice.h>

#include "DnsTlsServer.h"

namespace android {
namespace net {

// Keeps track of queries and responses.  This class matches responses with queries.
// All methods are thread-safe and non-blocking.
class DnsTlsQueryMap {
  public:
    struct Query {
        // The new ID number assigned to this query.
        uint16_t newId;
        // A query that has been passed to recordQuery(), with its original ID number.
        const netdutils::Slice query;
    };

    typedef DnsTlsServer::Response Response;
    typedef DnsTlsServer::Result Result;

    struct QueryFuture {
        QueryFuture(Query query, std::future<Result> result)
            : query(query), result(std::move(result)) {}
        Query query;
        // A future which will resolve to the result of this query.
        std::future<Result> result;
    };

    // Returns an object containing everything needed to complete processing of
    // this query, or null if the query could not be recorded.
    std::unique_ptr<QueryFuture> recordQuery(const netdutils::Slice query);

    // Process a response, including a new ID.  If the response
    // is not recognized as matching any query, it will be ignored.
    void onResponse(std::vector<uint8_t> response);

    // Clear all map contents.  This causes all pending queries to resolve with failure.
    void clear();

    // Get all pending queries.  This returns a shallow copy, mostly for thread-safety.
    std::vector<Query> getAll();

    // Mark a query has having been retried.  If the query hits the retry limit, it will
    // be expired at the next call to cleanup.
    void markTried(uint16_t newId);
    void cleanup();

    // Returns true if there are no pending queries.
    bool empty();

  private:
    std::mutex mLock;

    struct QueryPromise {
        QueryPromise(Query query) : query(query) {}
        Query query;
        // Number of times the query has been tried.  Limited to kMaxTries.
        int tries = 0;
        // A promise whose future is returned by recordQuery()
        // It is fulfilled by onResponse().
        std::promise<Result> result;
    };

    // The maximum number of times we will send a query before abandoning it.
    static constexpr int kMaxTries = 3;

    // Outstanding queries by newId.
    std::map<uint16_t, QueryPromise> mQueries GUARDED_BY(mLock);

    // Get a "newId" number that is not currently in use.  Returns -1 if there are none.
    int32_t getFreeId() REQUIRES(mLock);

    // Fulfill the result with an error code.
    static void expire(QueryPromise* _Nonnull p);
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_DNSTLSQUERYMAP_H
