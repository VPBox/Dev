/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless requied by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef DNS_RESPONDER_H
#define DNS_RESPONDER_H

#include <arpa/nameser.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <android-base/thread_annotations.h>
#include "android-base/unique_fd.h"

namespace test {

struct DNSHeader;
struct DNSQuestion;
struct DNSRecord;

inline const std::string kDefaultListenAddr = "127.0.0.3";
inline const std::string kDefaultListenService = "53";
inline const int kDefaultPollTimoutMillis = -1;

/*
 * Simple DNS responder, which replies to queries with the registered response
 * for that type. Class is assumed to be IN. If no response is registered, the
 * default error response code is returned.
 */
class DNSResponder {
  public:
    DNSResponder(std::string listen_address = kDefaultListenAddr,
                 std::string listen_service = kDefaultListenService,
                 int poll_timeout_ms = kDefaultPollTimoutMillis,
                 ns_rcode error_rcode = ns_rcode::ns_r_servfail);
    ~DNSResponder();

    enum class Edns : uint8_t {
        ON,
        FORMERR_ON_EDNS, // DNS server not supporting EDNS will reply FORMERR.
        FORMERR_UNCOND,  // DNS server reply FORMERR unconditionally
        DROP             // DNS server not supporting EDNS will not do any response.
    };

    void addMapping(const std::string& name, ns_type type, const std::string& addr);
    void removeMapping(const std::string& name, ns_type type);
    void setResponseProbability(double response_probability);
    void setEdns(Edns edns);
    bool running() const;
    bool startServer();
    bool stopServer();
    const std::string& listen_address() const {
        return listen_address_;
    }
    const std::string& listen_service() const {
        return listen_service_;
    }
    std::vector<std::pair<std::string, ns_type>> queries() const;
    std::string dumpQueries() const;
    void clearQueries();
    std::condition_variable& getCv() { return cv; }
    std::mutex& getCvMutex() { return cv_mutex_; }
    void setDeferredResp(bool deferred_resp);

  private:
    // Key used for accessing mappings.
    struct QueryKey {
        std::string name;
        unsigned type;

        QueryKey(std::string n, unsigned t) : name(move(n)), type(t) {}
        bool operator == (const QueryKey& o) const {
            return name == o.name && type == o.type;
        }
        bool operator < (const QueryKey& o) const {
            if (name < o.name) return true;
            if (name > o.name) return false;
            return type < o.type;
        }
    };

    struct QueryKeyHash {
        size_t operator() (const QueryKey& key) const {
            return std::hash<std::string>()(key.name) +
                   static_cast<size_t>(key.type);
        }
    };

    void requestHandler();

    // Parses and generates a response message for incoming DNS requests.
    // Returns false to ignore the request, which might be due to either parsing error
    // or unresponsiveness.
    bool handleDNSRequest(const char* buffer, ssize_t buffer_len,
                          char* response, size_t* response_len) const;

    bool addAnswerRecords(const DNSQuestion& question, std::vector<DNSRecord>* answers) const;

    bool fillAnswerRdata(const std::string& rdatastr, DNSRecord& record) const;

    bool generateErrorResponse(DNSHeader* header, ns_rcode rcode,
                               char* response, size_t* response_len) const;
    bool makeErrorResponse(DNSHeader* header, ns_rcode rcode, char* response,
                           size_t* response_len) const;

    // Add a new file descriptor to be polled by the handler thread.
    bool addFd(int fd, uint32_t events);

    // Read the query sent from the client and send the answer back to the client. It
    // makes sure the I/O communicated with the client is correct.
    void handleQuery();

    // Trigger the handler thread to terminate.
    bool sendToEventFd();

    // Used in the handler thread for the termination signal.
    void handleEventFd();

    // Address and service to listen on, currently limited to UDP.
    const std::string listen_address_;
    const std::string listen_service_;
    // epoll_wait() timeout in ms.
    const int poll_timeout_ms_;
    // Error code to return for requests for an unknown name.
    const ns_rcode error_rcode_;
    // Probability that a valid response is being sent instead of being sent
    // instead of returning error_rcode_.
    std::atomic<double> response_probability_ = 1.0;
    // Maximum number of fds for epoll.
    const int EPOLL_MAX_EVENTS = 2;

    // Control how the DNS server behaves when it receives the requests containing OPT RR.
    // If it's set Edns::ON, the server can recognize and reply the response; if it's set
    // Edns::FORMERR_ON_EDNS, the server behaves like an old DNS server that doesn't support EDNS0,
    // and replying FORMERR; if it's Edns::DROP, the server doesn't support EDNS0 either, and
    // ignoring the requests.
    std::atomic<Edns> edns_ = Edns::ON;

    // Mappings from (name, type) to registered response and the
    // mutex protecting them.
    std::unordered_map<QueryKey, std::string, QueryKeyHash> mappings_
        GUARDED_BY(mappings_mutex_);
    mutable std::mutex mappings_mutex_;
    // Query names received so far and the corresponding mutex.
    mutable std::vector<std::pair<std::string, ns_type>> queries_
        GUARDED_BY(queries_mutex_);
    mutable std::mutex queries_mutex_;
    // Socket on which the server is listening.
    android::base::unique_fd socket_;
    // File descriptor for epoll.
    android::base::unique_fd epoll_fd_;
    // Eventfd used to signal for the handler thread termination.
    android::base::unique_fd event_fd_;
    // Thread for handling incoming threads.
    std::thread handler_thread_ GUARDED_BY(update_mutex_);
    std::mutex update_mutex_;
    std::condition_variable cv;
    std::mutex cv_mutex_;

    std::condition_variable cv_for_deferred_resp_;
    std::mutex cv_mutex_for_deferred_resp_;
    bool deferred_resp_ GUARDED_BY(cv_mutex_for_deferred_resp_) = false;
};

}  // namespace test

#endif  // DNS_RESPONDER_H
