//
// Copyright (C) 2010 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_PROXY_RESOLVER_H_
#define UPDATE_ENGINE_COMMON_PROXY_RESOLVER_H_

#include <deque>
#include <string>

#include <base/logging.h>
#include <brillo/message_loops/message_loop.h>

#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

extern const char kNoProxy[];

// Callback for a call to GetProxiesForUrl().
// Resultant proxies are in |out_proxy|. Each will be in one of the
// following forms:
// http://<host>[:<port>] - HTTP proxy
// socks{4,5}://<host>[:<port>] - SOCKS4/5 proxy
// kNoProxy - no proxy
typedef base::Callback<void(const std::deque<std::string>& proxies)>
    ProxiesResolvedFn;

// An id that identifies a proxy request. Used to cancel an ongoing request
// before the callback is called.
typedef brillo::MessageLoop::TaskId ProxyRequestId;

// A constant identifying an invalid ProxyRequestId.
extern const ProxyRequestId kProxyRequestIdNull;

class ProxyResolver {
 public:
  ProxyResolver() {}
  virtual ~ProxyResolver() {}

  // Finds proxies for the given URL and returns them via the callback.
  // Returns the id of the pending request on success or kProxyRequestIdNull
  // otherwise.
  virtual ProxyRequestId GetProxiesForUrl(
      const std::string& url, const ProxiesResolvedFn& callback) = 0;

  // Cancel the proxy resolution request initiated by GetProxiesForUrl(). The
  // |request| value must be the one provided by GetProxiesForUrl().
  virtual bool CancelProxyRequest(ProxyRequestId request) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyResolver);
};

// Always says to not use a proxy
class DirectProxyResolver : public ProxyResolver {
 public:
  DirectProxyResolver() = default;
  ~DirectProxyResolver() override;
  ProxyRequestId GetProxiesForUrl(const std::string& url,
                                  const ProxiesResolvedFn& callback) override;
  bool CancelProxyRequest(ProxyRequestId request) override;

  // Set the number of direct (non-) proxies to be returned by resolver.
  // The default value is 1; higher numbers are currently used in testing.
  inline void set_num_proxies(size_t num_proxies) {
    num_proxies_ = num_proxies;
  }

 private:
  // The ID of the main loop callback.
  brillo::MessageLoop::TaskId idle_callback_id_{
      brillo::MessageLoop::kTaskIdNull};

  // Number of direct proxies to return on resolved list; currently used for
  // testing.
  size_t num_proxies_{1};

  // The MainLoop callback, from here we return to the client.
  void ReturnCallback(const ProxiesResolvedFn& callback);
  DISALLOW_COPY_AND_ASSIGN(DirectProxyResolver);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_PROXY_RESOLVER_H_
