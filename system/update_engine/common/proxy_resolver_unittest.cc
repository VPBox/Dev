//
// Copyright (C) 2017 The Android Open Source Project
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

#include "update_engine/common/proxy_resolver.h"

#include <deque>
#include <string>

#include <gtest/gtest.h>

#include <base/bind.h>
#include <brillo/message_loops/fake_message_loop.h>

using std::deque;
using std::string;

namespace chromeos_update_engine {

class ProxyResolverTest : public ::testing::Test {
 protected:
  virtual ~ProxyResolverTest() = default;

  void SetUp() override { loop_.SetAsCurrent(); }

  void TearDown() override { EXPECT_FALSE(loop_.PendingTasks()); }

  brillo::FakeMessageLoop loop_{nullptr};
  DirectProxyResolver resolver_;
};

TEST_F(ProxyResolverTest, DirectProxyResolverCallbackTest) {
  bool called = false;
  deque<string> callback_proxies;
  auto callback = base::Bind(
      [](bool* called,
         deque<string>* callback_proxies,
         const deque<string>& proxies) {
        *called = true;
        *callback_proxies = proxies;
      },
      &called,
      &callback_proxies);

  EXPECT_NE(kProxyRequestIdNull,
            resolver_.GetProxiesForUrl("http://foo", callback));
  // Check the callback is not called until the message loop runs.
  EXPECT_FALSE(called);
  loop_.Run();
  EXPECT_TRUE(called);
  EXPECT_EQ(kNoProxy, callback_proxies.front());
}

TEST_F(ProxyResolverTest, DirectProxyResolverCancelCallbackTest) {
  bool called = false;
  auto callback = base::Bind(
      [](bool* called, const deque<string>& proxies) { *called = true; },
      &called);

  ProxyRequestId request = resolver_.GetProxiesForUrl("http://foo", callback);
  EXPECT_FALSE(called);
  EXPECT_TRUE(resolver_.CancelProxyRequest(request));
  loop_.Run();
  EXPECT_FALSE(called);
}

TEST_F(ProxyResolverTest, DirectProxyResolverSimultaneousCallbacksTest) {
  int called = 0;
  auto callback = base::Bind(
      [](int* called, const deque<string>& proxies) { (*called)++; }, &called);

  resolver_.GetProxiesForUrl("http://foo", callback);
  resolver_.GetProxiesForUrl("http://bar", callback);
  EXPECT_EQ(0, called);
  loop_.Run();
  EXPECT_EQ(2, called);
}

}  // namespace chromeos_update_engine
