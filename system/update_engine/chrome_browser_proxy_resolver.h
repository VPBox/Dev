//
// Copyright (C) 2011 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_CHROME_BROWSER_PROXY_RESOLVER_H_
#define UPDATE_ENGINE_CHROME_BROWSER_PROXY_RESOLVER_H_

#include <deque>
#include <map>
#include <string>
#include <vector>

#include <base/memory/weak_ptr.h>

#include "update_engine/common/proxy_resolver.h"

namespace chromeos_update_engine {

class ChromeBrowserProxyResolver : public ProxyResolver {
 public:
  ChromeBrowserProxyResolver();
  ~ChromeBrowserProxyResolver() override;

  // ProxyResolver:
  ProxyRequestId GetProxiesForUrl(const std::string& url,
                                  const ProxiesResolvedFn& callback) override;
  bool CancelProxyRequest(ProxyRequestId request) override;

 private:
  // Callback for calls made by GetProxiesForUrl().
  void OnGetChromeProxyServers(ProxyRequestId request_id,
                               bool success,
                               const std::vector<std::string>& proxies);

  // Finds the callback identified by |request_id| in |pending_callbacks_|,
  // passes |proxies| to it, and deletes it. Does nothing if the request has
  // been cancelled.
  void RunCallback(ProxyRequestId request_id,
                   const std::deque<std::string>& proxies);

  // Next ID to return from GetProxiesForUrl().
  ProxyRequestId next_request_id_;

  // Callbacks that were passed to GetProxiesForUrl() but haven't yet been run.
  std::map<ProxyRequestId, ProxiesResolvedFn> pending_callbacks_;

  base::WeakPtrFactory<ChromeBrowserProxyResolver> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserProxyResolver);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_CHROME_BROWSER_PROXY_RESOLVER_H_
