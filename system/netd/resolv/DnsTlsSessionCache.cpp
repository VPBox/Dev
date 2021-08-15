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

#include "DnsTlsSessionCache.h"

#define LOG_TAG "DnsTlsSessionCache"
//#define LOG_NDEBUG 0

#include "log/log.h"

namespace android {
namespace net {

bool DnsTlsSessionCache::prepareSsl(SSL* ssl) {
    // Add this cache as the 0-index extra data for the socket.
    // This is used by newSessionCallback.
    int ret = SSL_set_ex_data(ssl, 0, this);
    return ret == 1;
}

void DnsTlsSessionCache::prepareSslContext(SSL_CTX* ssl_ctx) {
    SSL_CTX_set_session_cache_mode(ssl_ctx, SSL_SESS_CACHE_CLIENT);
    SSL_CTX_sess_set_new_cb(ssl_ctx, &DnsTlsSessionCache::newSessionCallback);
}

// static
int DnsTlsSessionCache::newSessionCallback(SSL* ssl, SSL_SESSION* session) {
    if (!ssl || !session) {
        ALOGE("Null SSL object in new session callback");
        return 0;
    }
    DnsTlsSessionCache* cache = reinterpret_cast<DnsTlsSessionCache*>(
            SSL_get_ex_data(ssl, 0));
    if (!cache) {
        ALOGE("null transport in new session callback");
        return 0;
    }
    ALOGV("Recording session");
    cache->recordSession(session);
    return 1;  // Increment the refcount of session.
}

void DnsTlsSessionCache::recordSession(SSL_SESSION* session) {
    std::lock_guard guard(mLock);
    mSessions.emplace_front(session);
    if (mSessions.size() > kMaxSize) {
        ALOGV("Too many sessions; trimming");
        mSessions.pop_back();
    }
}

bssl::UniquePtr<SSL_SESSION> DnsTlsSessionCache::getSession() {
    std::lock_guard guard(mLock);
    if (mSessions.size() == 0) {
        ALOGV("No known sessions");
        return nullptr;
    }
    bssl::UniquePtr<SSL_SESSION> ret = std::move(mSessions.front());
    mSessions.pop_front();
    return ret;
}

}  // end of namespace net
}  // end of namespace android
