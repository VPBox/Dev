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

#ifndef _DNS_IDNSTLSSOCKET_H
#define _DNS_IDNSTLSSOCKET_H

#include <cstddef>
#include <cstdint>

#include <netdutils/Slice.h>

namespace android {
namespace net {

class IDnsTlsSocketObserver;
class DnsTlsSessionCache;

// A class for managing a TLS socket that sends and receives messages in
// [length][value] format, with a 2-byte length (i.e. DNS-over-TCP format).
// This interface is not aware of query-response pairing or anything else about DNS.
class IDnsTlsSocket {
  public:
    virtual ~IDnsTlsSocket(){};
    // Send a query on the provided SSL socket.  |query| contains
    // the body of a query, not including the ID bytes.  This function will typically return before
    // the query is actually sent.  If this function fails, the observer will be
    // notified that the socket is closed.
    // Note that a true return value indicates successful sending, not receipt of a response.
    virtual bool query(uint16_t id, const netdutils::Slice query) = 0;
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_IDNSTLSSOCKET_H
