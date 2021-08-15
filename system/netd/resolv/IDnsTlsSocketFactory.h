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

#ifndef _DNS_IDNSTLSSOCKETFACTORY_H
#define _DNS_IDNSTLSSOCKETFACTORY_H

#include "IDnsTlsSocket.h"

namespace android {
namespace net {

class IDnsTlsSocketObserver;
class DnsTlsSessionCache;
struct DnsTlsServer;

// Dependency injection interface for DnsTlsSocketFactory.
// This pattern allows mocking of DnsTlsSocket for tests.
class IDnsTlsSocketFactory {
  public:
    virtual ~IDnsTlsSocketFactory(){};
    virtual std::unique_ptr<IDnsTlsSocket> createDnsTlsSocket(
            const DnsTlsServer& server, unsigned mark, IDnsTlsSocketObserver* _Nonnull observer,
            DnsTlsSessionCache* _Nonnull cache) = 0;
};

}  // end of namespace net
}  // end of namespace android

#endif  // _DNS_IDNSTLSSOCKETFACTORY_H
