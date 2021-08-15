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

#ifndef _DNS_DNSTLSSOCKET_H
#define _DNS_DNSTLSSOCKET_H

#include <openssl/ssl.h>
#include <future>
#include <mutex>

#include <android-base/thread_annotations.h>
#include <android-base/unique_fd.h>
#include <netdutils/Slice.h>
#include <netdutils/Status.h>

#include "DnsTlsServer.h"
#include "IDnsTlsSocket.h"
#include "LockedQueue.h"

namespace android {
namespace net {

class IDnsTlsSocketObserver;
class DnsTlsSessionCache;

// A class for managing a TLS socket that sends and receives messages in
// [length][value] format, with a 2-byte length (i.e. DNS-over-TCP format).
// This class is not aware of query-response pairing or anything else about DNS.
// For the observer:
// This class is not re-entrant: the observer is not permitted to wait for a call to query()
// or the destructor in a callback.  Doing so will result in deadlocks.
// This class may call the observer at any time after initialize(), until the destructor
// returns (but not after).
class DnsTlsSocket : public IDnsTlsSocket {
  public:
    DnsTlsSocket(const DnsTlsServer& server, unsigned mark,
                 IDnsTlsSocketObserver* _Nonnull observer, DnsTlsSessionCache* _Nonnull cache)
        : mMark(mark), mServer(server), mObserver(observer), mCache(cache) {}
    ~DnsTlsSocket();

    // Creates the SSL context for this session and connect.  Returns false on failure.
    // This method should be called after construction and before use of a DnsTlsSocket.
    // Only call this method once per DnsTlsSocket.
    bool initialize() EXCLUDES(mLock);

    // Send a query on the provided SSL socket.  |query| contains
    // the body of a query, not including the ID header. This function will typically return before
    // the query is actually sent.  If this function fails, DnsTlsSocketObserver will be
    // notified that the socket is closed.
    // Note that success here indicates successful sending, not receipt of a response.
    // Thread-safe.
    bool query(uint16_t id, const netdutils::Slice query) override EXCLUDES(mLock);

  private:
    // Lock to be held by the SSL event loop thread.  This is not normally in contention.
    std::mutex mLock;

    // Forwards queries and receives responses.  Blocks until the idle timeout.
    void loop() EXCLUDES(mLock);
    std::unique_ptr<std::thread> mLoopThread GUARDED_BY(mLock);

    // On success, sets mSslFd to a socket connected to mAddr (the
    // connection will likely be in progress if mProtocol is IPPROTO_TCP).
    // On error, returns the errno.
    netdutils::Status tcpConnect() REQUIRES(mLock);

    // Connect an SSL session on the provided socket.  If connection fails, closing the
    // socket remains the caller's responsibility.
    bssl::UniquePtr<SSL> sslConnect(int fd) REQUIRES(mLock);

    // Disconnect the SSL session and close the socket.
    void sslDisconnect() REQUIRES(mLock);

    // Writes a buffer to the socket.
    bool sslWrite(const netdutils::Slice buffer) REQUIRES(mLock);

    // Reads exactly the specified number of bytes from the socket, or fails.
    // Returns SSL_ERROR_NONE on success.
    // If |wait| is true, then this function always blocks.  Otherwise, it
    // will return SSL_ERROR_WANT_READ if there is no data from the server to read.
    int sslRead(const netdutils::Slice buffer, bool wait) REQUIRES(mLock);

    bool sendQuery(const std::vector<uint8_t>& buf) REQUIRES(mLock);
    bool readResponse() REQUIRES(mLock);

    // Similar to query(), this function uses incrementEventFd to send a message to the
    // loop thread.  However, instead of incrementing the counter by one (indicating a
    // new query), it wraps the counter to negative, which we use to indicate a shutdown
    // request.
    void requestLoopShutdown() EXCLUDES(mLock);

    // This function sends a message to the loop thread by incrementing mEventFd.
    bool incrementEventFd(int64_t count) EXCLUDES(mLock);

    // Queue of pending queries.  query() pushes items onto the queue and notifies
    // the loop thread by incrementing mEventFd.  loop() reads items off the queue.
    LockedQueue<std::vector<uint8_t>> mQueue;

    // eventfd socket used for notifying the SSL thread when queries are ready to send.
    // This socket acts similarly to an atomic counter, incremented by query() and cleared
    // by loop().  We have to use a socket because the SSL thread needs to wait in poll()
    // for input from either a remote server or a query thread.  Since eventfd does not have
    // EOF, we indicate a close request by setting the counter to a negative number.
    // This file descriptor is opened by initialize(), and closed implicitly after
    // destruction.
    base::unique_fd mEventFd;

    // SSL Socket fields.
    bssl::UniquePtr<SSL_CTX> mSslCtx GUARDED_BY(mLock);
    base::unique_fd mSslFd GUARDED_BY(mLock);
    bssl::UniquePtr<SSL> mSsl GUARDED_BY(mLock);
    static constexpr std::chrono::seconds kIdleTimeout = std::chrono::seconds(20);

    const unsigned mMark;  // Socket mark
    const DnsTlsServer mServer;
    IDnsTlsSocketObserver* _Nonnull const mObserver;
    DnsTlsSessionCache* _Nonnull const mCache;
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_DNSTLSSOCKET_H
