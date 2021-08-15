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

#ifndef _TEST_METRICS_H_
#define _TEST_METRICS_H_

#include "BaseTestMetricsListener.h"

namespace android {
namespace net {
namespace metrics {

class TestOnDnsEvent : public BaseTestMetricsListener {
  public:
    // Both latencyMs and uid are not verified. No special reason.
    struct TestResult {
        int netId;
        int eventType;
        int returnCode;
        int ipAddressesCount;
        std::string hostname;
        std::string ipAddress;  // Check first address only.
    };

    TestOnDnsEvent() = delete;
    TestOnDnsEvent(const std::vector<TestResult>& results) : mResults(results){};

    // BaseTestMetricsListener::isVerified() override.
    bool isVerified() override { return (getVerified() & EventFlag::onDnsEvent) != 0; }

    // Override for testing verification.
    android::binder::Status onDnsEvent(int32_t netId, int32_t eventType, int32_t returnCode,
                                       int32_t /*latencyMs*/, const std::string& hostname,
                                       const std::vector<std::string>& ipAddresses,
                                       int32_t ipAddressesCount, int32_t /*uid*/) override;

  private:
    const std::vector<TestResult>& mResults;  // Expected results for test verification.
};

}  // namespace metrics
}  // namespace net
}  // namespace android

#endif  // _TEST_METRICS_H_
