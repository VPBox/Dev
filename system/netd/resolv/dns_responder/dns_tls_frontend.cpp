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

#include "dns_tls_frontend.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <sys/eventfd.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_TAG "DnsTlsFrontend"
#include <log/log.h>
#include <netdutils/SocketOption.h>

#include "NetdConstants.h"  // SHA256_SIZE

using android::netdutils::enableSockopt;

namespace {

// Copied from DnsTlsTransport.
bool getSPKIDigest(const X509* cert, std::vector<uint8_t>* out) {
    int spki_len = i2d_X509_PUBKEY(X509_get_X509_PUBKEY(cert), nullptr);
    unsigned char spki[spki_len];
    unsigned char* temp = spki;
    if (spki_len != i2d_X509_PUBKEY(X509_get_X509_PUBKEY(cert), &temp)) {
        ALOGE("SPKI length mismatch");
        return false;
    }
    out->resize(SHA256_SIZE);
    unsigned int digest_len = 0;
    int ret = EVP_Digest(spki, spki_len, out->data(), &digest_len, EVP_sha256(), nullptr);
    if (ret != 1) {
        ALOGE("Server cert digest extraction failed");
        return false;
    }
    if (digest_len != out->size()) {
        ALOGE("Wrong digest length: %d", digest_len);
        return false;
    }
    return true;
}

std::string errno2str() {
    char error_msg[512] = { 0 };
    return strerror_r(errno, error_msg, sizeof(error_msg));
}

#define APLOGI(fmt, ...) ALOGI(fmt ": [%d] %s", __VA_ARGS__, errno, errno2str().c_str())

std::string addr2str(const sockaddr* sa, socklen_t sa_len) {
    char host_str[NI_MAXHOST] = { 0 };
    int rv = getnameinfo(sa, sa_len, host_str, sizeof(host_str), nullptr, 0,
                         NI_NUMERICHOST);
    if (rv == 0) return std::string(host_str);
    return std::string();
}

bssl::UniquePtr<EVP_PKEY> make_private_key() {
    bssl::UniquePtr<BIGNUM> e(BN_new());
    if (!e) {
        ALOGE("BN_new failed");
        return nullptr;
    }
    if (!BN_set_word(e.get(), RSA_F4)) {
        ALOGE("BN_set_word failed");
        return nullptr;
    }

    bssl::UniquePtr<RSA> rsa(RSA_new());
    if (!rsa) {
        ALOGE("RSA_new failed");
        return nullptr;
    }
    if (!RSA_generate_key_ex(rsa.get(), 2048, e.get(), nullptr)) {
        ALOGE("RSA_generate_key_ex failed");
        return nullptr;
    }

    bssl::UniquePtr<EVP_PKEY> privkey(EVP_PKEY_new());
    if (!privkey) {
        ALOGE("EVP_PKEY_new failed");
        return nullptr;
    }
    if(!EVP_PKEY_assign_RSA(privkey.get(), rsa.get())) {
        ALOGE("EVP_PKEY_assign_RSA failed");
        return nullptr;
    }

    // |rsa| is now owned by |privkey|, so no need to free it.
    rsa.release();
    return privkey;
}

bssl::UniquePtr<X509> make_cert(EVP_PKEY* privkey, EVP_PKEY* parent_key) {
    bssl::UniquePtr<X509> cert(X509_new());
    if (!cert) {
        ALOGE("X509_new failed");
        return nullptr;
    }

    ASN1_INTEGER_set(X509_get_serialNumber(cert.get()), 1);

    // Set one hour expiration.
    X509_gmtime_adj(X509_get_notBefore(cert.get()), 0);
    X509_gmtime_adj(X509_get_notAfter(cert.get()), 60 * 60);

    X509_set_pubkey(cert.get(), privkey);

    if (!X509_sign(cert.get(), parent_key, EVP_sha256())) {
        ALOGE("X509_sign failed");
        return nullptr;
    }

    return cert;
}

}

namespace test {

bool DnsTlsFrontend::startServer() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    // reset queries_ to 0 every time startServer called
    // which would help us easy to check queries_ via calling waitForQueries
    queries_ = 0;

    ctx_.reset(SSL_CTX_new(TLS_server_method()));
    if (!ctx_) {
        ALOGE("SSL context creation failed");
        return false;
    }

    SSL_CTX_set_ecdh_auto(ctx_.get(), 1);

    // Make certificate chain
    std::vector<bssl::UniquePtr<EVP_PKEY>> keys(chain_length_);
    for (int i = 0; i < chain_length_; ++i) {
        keys[i] = make_private_key();
    }
    std::vector<bssl::UniquePtr<X509>> certs(chain_length_);
    for (int i = 0; i < chain_length_; ++i) {
        int next = std::min(i + 1, chain_length_ - 1);
        certs[i] = make_cert(keys[i].get(), keys[next].get());
    }

    // Install certificate chain.
    if (SSL_CTX_use_certificate(ctx_.get(), certs[0].get()) <= 0) {
        ALOGE("SSL_CTX_use_certificate failed");
        return false;
    }
    if (SSL_CTX_use_PrivateKey(ctx_.get(), keys[0].get()) <= 0 ) {
        ALOGE("SSL_CTX_use_PrivateKey failed");
        return false;
    }
    for (int i = 1; i < chain_length_; ++i) {
        if (SSL_CTX_add1_chain_cert(ctx_.get(), certs[i].get()) != 1) {
            ALOGE("SSL_CTX_add1_chain_cert failed");
            return false;
        }
    }

    // Report the fingerprint of the "middle" cert.  For N = 2, this is the root.
    int fp_index = chain_length_ / 2;
    if (!getSPKIDigest(certs[fp_index].get(), &fingerprint_)) {
        ALOGE("getSPKIDigest failed");
        return false;
    }

    // Set up TCP server socket for clients.
    addrinfo frontend_ai_hints{
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE
    };
    addrinfo* frontend_ai_res = nullptr;
    int rv = getaddrinfo(listen_address_.c_str(), listen_service_.c_str(),
                         &frontend_ai_hints, &frontend_ai_res);
    ScopedAddrinfo frontend_ai_res_cleanup(frontend_ai_res);
    if (rv) {
        ALOGE("frontend getaddrinfo(%s, %s) failed: %s", listen_address_.c_str(),
            listen_service_.c_str(), gai_strerror(rv));
        return false;
    }

    for (const addrinfo* ai = frontend_ai_res ; ai ; ai = ai->ai_next) {
        android::base::unique_fd s(socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol));
        if (s.get() < 0) {
            APLOGI("ignore creating socket failed %d", s.get());
            continue;
        }
        enableSockopt(s.get(), SOL_SOCKET, SO_REUSEPORT).ignoreError();
        enableSockopt(s.get(), SOL_SOCKET, SO_REUSEADDR).ignoreError();
        std::string host_str = addr2str(ai->ai_addr, ai->ai_addrlen);
        if (bind(s.get(), ai->ai_addr, ai->ai_addrlen)) {
            APLOGI("failed to bind TCP %s:%s", host_str.c_str(), listen_service_.c_str());
            continue;
        }
        ALOGI("bound to TCP %s:%s", host_str.c_str(), listen_service_.c_str());
        socket_ = std::move(s);
        break;
    }

    if (listen(socket_.get(), 1) < 0) {
        APLOGI("failed to listen socket %d", socket_.get());
        return false;
    }

    // Set up UDP client socket to backend.
    addrinfo backend_ai_hints{
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM
    };
    addrinfo* backend_ai_res = nullptr;
    rv = getaddrinfo(backend_address_.c_str(), backend_service_.c_str(),
                         &backend_ai_hints, &backend_ai_res);
    ScopedAddrinfo backend_ai_res_cleanup(backend_ai_res);
    if (rv) {
        ALOGE("backend getaddrinfo(%s, %s) failed: %s", listen_address_.c_str(),
            listen_service_.c_str(), gai_strerror(rv));
        return false;
    }
    backend_socket_.reset(socket(backend_ai_res->ai_family, backend_ai_res->ai_socktype,
                                 backend_ai_res->ai_protocol));
    if (backend_socket_.get() < 0) {
        APLOGI("backend socket %d creation failed", backend_socket_.get());
        return false;
    }

    // connect() always fails in the test DnsTlsSocketTest.SlowDestructor because of
    // no backend server. Don't check it.
    connect(backend_socket_.get(), backend_ai_res->ai_addr, backend_ai_res->ai_addrlen);

    // Set up eventfd socket.
    event_fd_.reset(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC));
    if (event_fd_.get() == -1) {
        APLOGI("failed to create eventfd %d", event_fd_.get());
        return false;
    }

    {
        std::lock_guard lock(update_mutex_);
        handler_thread_ = std::thread(&DnsTlsFrontend::requestHandler, this);
    }
    ALOGI("server started successfully");
    return true;
}

void DnsTlsFrontend::requestHandler() {
    ALOGD("Request handler started");
    enum { EVENT_FD = 0, LISTEN_FD = 1 };
    pollfd fds[2] = {{.fd = event_fd_.get(), .events = POLLIN},
                     {.fd = socket_.get(), .events = POLLIN}};

    while (true) {
        int poll_code = poll(fds, std::size(fds), -1);
        if (poll_code <= 0) {
            APLOGI("Poll failed with error %d", poll_code);
            break;
        }

        if (fds[EVENT_FD].revents & (POLLIN | POLLERR)) {
            handleEventFd();
            break;
        }
        if (fds[LISTEN_FD].revents & (POLLIN | POLLERR)) {
            sockaddr_storage addr;
            socklen_t len = sizeof(addr);

            ALOGD("Trying to accept a client");
            android::base::unique_fd client(
                    accept4(socket_.get(), reinterpret_cast<sockaddr*>(&addr), &len, SOCK_CLOEXEC));
            if (client.get() < 0) {
                // Stop
                APLOGI("failed to accept client socket %d", client.get());
                break;
            }

            bssl::UniquePtr<SSL> ssl(SSL_new(ctx_.get()));
            SSL_set_fd(ssl.get(), client.get());

            ALOGD("Doing SSL handshake");
            bool success = false;
            if (SSL_accept(ssl.get()) <= 0) {
                ALOGI("SSL negotiation failure");
            } else {
                ALOGD("SSL handshake complete");
                success = handleOneRequest(ssl.get());
            }

            if (success) {
                // Increment queries_ as late as possible, because it represents
                // a query that is fully processed, and the response returned to the
                // client, including cleanup actions.
                ++queries_;
            }
        }
    }
    ALOGD("Ending loop");
}

bool DnsTlsFrontend::handleOneRequest(SSL* ssl) {
    uint8_t queryHeader[2];
    if (SSL_read(ssl, &queryHeader, 2) != 2) {
        ALOGI("Not enough header bytes");
        return false;
    }
    const uint16_t qlen = (queryHeader[0] << 8) | queryHeader[1];
    uint8_t query[qlen];
    size_t qbytes = 0;
    while (qbytes < qlen) {
        int ret = SSL_read(ssl, query + qbytes, qlen - qbytes);
        if (ret <= 0) {
            ALOGI("Error while reading query");
            return false;
        }
        qbytes += ret;
    }
    int sent = send(backend_socket_.get(), query, qlen, 0);
    if (sent != qlen) {
        ALOGI("Failed to send query");
        return false;
    }
    const int max_size = 4096;
    uint8_t recv_buffer[max_size];
    int rlen = recv(backend_socket_.get(), recv_buffer, max_size, 0);
    if (rlen <= 0) {
        ALOGI("Failed to receive response");
        return false;
    }
    uint8_t responseHeader[2];
    responseHeader[0] = rlen >> 8;
    responseHeader[1] = rlen;
    if (SSL_write(ssl, responseHeader, 2) != 2) {
        ALOGI("Failed to write response header");
        return false;
    }
    if (SSL_write(ssl, recv_buffer, rlen) != rlen) {
        ALOGI("Failed to write response body");
        return false;
    }
    return true;
}

bool DnsTlsFrontend::stopServer() {
    std::lock_guard lock(update_mutex_);
    if (!running()) {
        ALOGI("server not running");
        return false;
    }

    ALOGI("stopping frontend");
    if (!sendToEventFd()) {
        return false;
    }
    handler_thread_.join();
    socket_.reset();
    backend_socket_.reset();
    event_fd_.reset();
    ctx_.reset();
    fingerprint_.clear();
    ALOGI("frontend stopped successfully");
    return true;
}

bool DnsTlsFrontend::waitForQueries(int number, int timeoutMs) const {
    constexpr int intervalMs = 20;
    int limit = timeoutMs / intervalMs;
    for (int count = 0; count <= limit; ++count) {
        bool done = queries_ >= number;
        // Always sleep at least one more interval after we are done, to wait for
        // any immediate post-query actions that the client may take (such as
        // marking this server as reachable during validation).
        usleep(intervalMs * 1000);
        if (done) {
            // For ensuring that calls have sufficient headroom for slow machines
            ALOGD("Query arrived in %d/%d of allotted time", count, limit);
            return true;
        }
    }
    return false;
}

bool DnsTlsFrontend::sendToEventFd() {
    const uint64_t data = 1;
    if (const ssize_t rt = write(event_fd_.get(), &data, sizeof(data)); rt != sizeof(data)) {
        APLOGI("failed to write eventfd, rt=%zd", rt);
        return false;
    }
    return true;
}

void DnsTlsFrontend::handleEventFd() {
    int64_t data;
    if (const ssize_t rt = read(event_fd_.get(), &data, sizeof(data)); rt != sizeof(data)) {
        APLOGI("ignore reading eventfd failed, rt=%zd", rt);
    }
}

}  // namespace test
