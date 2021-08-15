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

#include "update_engine/chrome_browser_proxy_resolver.h"

#include <utility>

#include <base/bind.h>
#include <base/memory/ptr_util.h>
#include <base/strings/string_util.h>
#include <brillo/http/http_proxy.h>

#include "update_engine/dbus_connection.h"

namespace chromeos_update_engine {

ChromeBrowserProxyResolver::ChromeBrowserProxyResolver()
    : next_request_id_(kProxyRequestIdNull + 1), weak_ptr_factory_(this) {}

ChromeBrowserProxyResolver::~ChromeBrowserProxyResolver() = default;

ProxyRequestId ChromeBrowserProxyResolver::GetProxiesForUrl(
    const std::string& url, const ProxiesResolvedFn& callback) {
  const ProxyRequestId id = next_request_id_++;
  brillo::http::GetChromeProxyServersAsync(
      DBusConnection::Get()->GetDBus(),
      url,
      base::Bind(&ChromeBrowserProxyResolver::OnGetChromeProxyServers,
                 weak_ptr_factory_.GetWeakPtr(),
                 id));
  pending_callbacks_[id] = callback;
  return id;
}

bool ChromeBrowserProxyResolver::CancelProxyRequest(ProxyRequestId request) {
  return pending_callbacks_.erase(request) != 0;
}

void ChromeBrowserProxyResolver::OnGetChromeProxyServers(
    ProxyRequestId request_id,
    bool success,
    const std::vector<std::string>& proxies) {
  // If |success| is false, |proxies| will still hold the direct proxy option
  // which is what we do in our error case.
  auto it = pending_callbacks_.find(request_id);
  if (it == pending_callbacks_.end())
    return;

  ProxiesResolvedFn callback = it->second;
  pending_callbacks_.erase(it);
  callback.Run(std::deque<std::string>(proxies.begin(), proxies.end()));
}

}  // namespace chromeos_update_engine
