/**
 * Copyright (c) 2019, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DNS_RESOLVER_H_
#define _DNS_RESOLVER_H_

#include "DnsProxyListener.h"
#include "ResolverController.h"
#include "netd_resolv/resolv.h"
#include "netdutils/Log.h"

namespace android {
namespace net {

class DnsResolver {
  public:
    static DnsResolver* getInstance();
    bool start();
    int setLogSeverity(int32_t logSeverity);

    DnsResolver(DnsResolver const&) = delete;
    void operator=(DnsResolver const&) = delete;

    ResolverController resolverCtrl;

  private:
    DnsResolver() {}
    DnsProxyListener mDnsProxyListener;
};

extern DnsResolver* gDnsResolv;
extern ResolverNetdCallbacks gResNetdCallbacks;
extern netdutils::Log gDnsResolverLog;

}  // namespace net
}  // namespace android

#endif  // _DNS_RESOLVER_H_
