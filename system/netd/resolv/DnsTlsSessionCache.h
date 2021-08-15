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

#ifndef _DNS_DNSTLSSESSIONCACHE_H
#define _DNS_DNSTLSSESSIONCACHE_H

#include <deque>
#include <mutex>

#include <openssl/ssl.h>

#include <android-base/thread_annotations.h>

namespace android {
namespace net {

// Cache of recently seen SSL_SESSIONs.  This is used to support session tickets.
// This class is thread-safe.
class DnsTlsSessionCache {
  public:
    // Prepare SSL objects to use this session cache.  These methods must be called
    // before making use of either object.
    void prepareSslContext(SSL_CTX* _Nonnull ssl_ctx);
    bool prepareSsl(SSL* _Nonnull ssl);

    // Get the most recently discovered session.  For TLS 1.3 compatibility and
    // maximum privacy, each session will only be returned once, so the caller
    // gains ownership of the session.  (Here and throughout,
    // bssl::UniquePtr<SSL_SESSION> is actually serving as a reference counted
    // pointer.)
    bssl::UniquePtr<SSL_SESSION> getSession() EXCLUDES(mLock);

  private:
    static constexpr size_t kMaxSize = 5;
    static int newSessionCallback(SSL* _Nullable ssl, SSL_SESSION* _Nullable session);

    std::mutex mLock;
    void recordSession(SSL_SESSION* _Nullable session) EXCLUDES(mLock);

    // Queue of sessions, from least recently added to most recently.
    std::deque<bssl::UniquePtr<SSL_SESSION>> mSessions GUARDED_BY(mLock);
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_DNSTLSSESSIONCACHE_H
