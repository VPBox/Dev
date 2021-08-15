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

#ifndef _DNS_IDNSTLSSOCKETOBSERVER_H
#define _DNS_IDNSTLSSOCKETOBSERVER_H

namespace android {
namespace net {

// Interface to listen for DNS query responses on a socket, and to be notified
// when the socket is closed by the remote peer.  This is only implemented by
// DnsTlsTransport, but it is a separate interface for clarity and to avoid a
// circular dependency with DnsTlsSocket.
class IDnsTlsSocketObserver {
  public:
    virtual ~IDnsTlsSocketObserver(){};
    virtual void onResponse(std::vector<uint8_t> response) = 0;

    virtual void onClosed() = 0;
};

}  // namespace net
}  // namespace android

#endif  // _DNS_IDNSTLSSOCKETOBSERVER_H
