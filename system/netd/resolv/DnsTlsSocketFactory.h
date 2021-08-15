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

#ifndef _DNS_DNSTLSSOCKETFACTORY_H
#define _DNS_DNSTLSSOCKETFACTORY_H

#include <memory>

#include "DnsTlsSocket.h"
#include "IDnsTlsSocketFactory.h"

namespace android {
namespace net {

class IDnsTlsSocketObserver;
class DnsTlsSessionCache;
struct DnsTlsServer;

// Trivial RAII factory for DnsTlsSocket.  This is owned by DnsTlsDispatcher.
class DnsTlsSocketFactory : public IDnsTlsSocketFactory {
  public:
    std::unique_ptr<IDnsTlsSocket> createDnsTlsSocket(const DnsTlsServer& server, unsigned mark,
                                                      IDnsTlsSocketObserver* _Nonnull observer,
                                                      DnsTlsSessionCache* _Nonnull cache) override {
        auto socket = std::make_unique<DnsTlsSocket>(server, mark, observer, cache);
        if (!socket->initialize()) {
            return nullptr;
        }
        return std::move(socket);
    }
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_DNSTLSSOCKETFACTORY_H
