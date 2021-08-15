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

#define LOG_TAG "DnsTlsQueryMap"
//#define LOG_NDEBUG 0

#include "DnsTlsQueryMap.h"

#include "log/log.h"

namespace android {
namespace net {

std::unique_ptr<DnsTlsQueryMap::QueryFuture> DnsTlsQueryMap::recordQuery(
        const netdutils::Slice query) {
    std::lock_guard guard(mLock);

    // Store the query so it can be matched to the response or reissued.
    if (query.size() < 2) {
        ALOGW("Query is too short");
        return nullptr;
    }
    int32_t newId = getFreeId();
    if (newId < 0) {
        ALOGW("All query IDs are in use");
        return nullptr;
    }
    Query q = { .newId = static_cast<uint16_t>(newId), .query = query };
    std::map<uint16_t, QueryPromise>::iterator it;
    bool inserted;
    std::tie(it, inserted) = mQueries.emplace(newId, q);
    if (!inserted) {
        ALOGE("Failed to store pending query");
        return nullptr;
    }
    return std::make_unique<QueryFuture>(q, it->second.result.get_future());
}

void DnsTlsQueryMap::expire(QueryPromise* p) {
    Result r = { .code = Response::network_error };
    p->result.set_value(r);
}

void DnsTlsQueryMap::markTried(uint16_t newId) {
    std::lock_guard guard(mLock);
    auto it = mQueries.find(newId);
    if (it != mQueries.end()) {
        it->second.tries++;
    }
}

void DnsTlsQueryMap::cleanup() {
    std::lock_guard guard(mLock);
    for (auto it = mQueries.begin(); it != mQueries.end();) {
        auto& p = it->second;
        if (p.tries >= kMaxTries) {
            expire(&p);
            it = mQueries.erase(it);
        } else {
            ++it;
        }
    }
}

int32_t DnsTlsQueryMap::getFreeId() {
    if (mQueries.empty()) {
        return 0;
    }
    uint16_t maxId = mQueries.rbegin()->first;
    if (maxId < UINT16_MAX) {
        return maxId + 1;
    }
    if (mQueries.size() == UINT16_MAX + 1) {
        // Map is full.
        return -1;
    }
    // Linear scan.
    uint16_t nextId = 0;
    for (auto& pair : mQueries) {
        uint16_t id = pair.first;
        if (id != nextId) {
            // Found a gap.
            return nextId;
        }
        nextId = id + 1;
    }
    // Unreachable (but the compiler isn't smart enough to prove it).
    return -1;
}

std::vector<DnsTlsQueryMap::Query> DnsTlsQueryMap::getAll() {
    std::lock_guard guard(mLock);
    std::vector<DnsTlsQueryMap::Query> queries;
    for (auto& q : mQueries) {
        queries.push_back(q.second.query);
    }
    return queries;
}

bool DnsTlsQueryMap::empty() {
    std::lock_guard guard(mLock);
    return mQueries.empty();
}

void DnsTlsQueryMap::clear() {
    std::lock_guard guard(mLock);
    for (auto& q : mQueries) {
        expire(&q.second);
    }
    mQueries.clear();
}

void DnsTlsQueryMap::onResponse(std::vector<uint8_t> response) {
    ALOGV("Got response of size %zu", response.size());
    if (response.size() < 2) {
        ALOGW("Response is too short");
        return;
    }
    uint16_t id = response[0] << 8 | response[1];
    std::lock_guard guard(mLock);
    auto it = mQueries.find(id);
    if (it == mQueries.end()) {
        ALOGW("Discarding response: unknown ID %d", id);
        return;
    }
    Result r = { .code = Response::success, .response = std::move(response) };
    // Rewrite ID to match the query
    const uint8_t* data = it->second.query.query.base();
    r.response[0] = data[0];
    r.response[1] = data[1];
    ALOGV("Sending result to dispatcher");
    it->second.result.set_value(std::move(r));
    mQueries.erase(it);
}

}  // end of namespace net
}  // end of namespace android
