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

#include <netdb.h>

#include "TestMetrics.h"

namespace android {
namespace net {
namespace metrics {

android::binder::Status TestOnDnsEvent::onDnsEvent(int32_t netId, int32_t eventType,
                                                   int32_t returnCode, int32_t /*latencyMs*/,
                                                   const std::string& hostname,
                                                   const ::std::vector<std::string>& ipAddresses,
                                                   int32_t ipAddressesCount, int32_t /*uid*/) {
    // A bitwise-OR combination of all expected test cases.
    // Currently, the maximum number of test case is 32 because a 32-bits bitwise-OR combination
    // is used for checking and recording verified test cases.
    static const uint32_t kExpectedTests = (1 << mResults.size()) - 1;

    // A bitwise-OR combination for recording verified test cases.
    static uint32_t verifiedTests = 0;

    for (size_t i = 0; i < mResults.size(); ++i) {
        // Generate bitwise flag to trace which testcase is running.
        const uint32_t currentTestFlag = 1 << i;

        // Ignore verified testcase.
        if (verifiedTests & currentTestFlag) continue;

        // Verify expected event content.
        auto& result = mResults[i];
        if (netId != result.netId) continue;
        if (eventType != result.eventType) continue;
        if (hostname != result.hostname) continue;
        if (returnCode != result.returnCode) continue;
        if (ipAddressesCount != result.ipAddressesCount) continue;
        if (result.returnCode == 0 /*success*/) {
            // Only verify first address.
            // TODO: Check all addresses.
            if (ipAddresses.empty() || ipAddresses.front() != result.ipAddress) continue;
        }

        // Record current testcase as verified.
        verifiedTests |= currentTestFlag;
        break;
    }

    // All testcases of onDnsEvent are verified. Notify who was waiting for test results.
    if (verifiedTests == kExpectedTests) {
        setVerified(EventFlag::onDnsEvent);
        notify();
    }

    return android::binder::Status::ok();
};

}  // namespace metrics
}  // namespace net
}  // namespace android