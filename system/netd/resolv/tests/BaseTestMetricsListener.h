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

#ifndef _BASE_TEST_METRICS_LISTENER_H_
#define _BASE_TEST_METRICS_LISTENER_H_

#include <string>
#include <vector>

#include <binder/BinderService.h>

#include "android/net/metrics/BnNetdEventListener.h"

enum EventFlag : uint32_t {
    onDnsEvent = 1 << 0,
    onPrivateDnsValidationEvent = 1 << 1,
    onConnectEvent = 1 << 2,
    onWakeupEvent = 1 << 3,
    onTcpSocketStatsEvent = 1 << 4,
    onNat64PrefixEvent = 1 << 5,
};

namespace android {
namespace net {
namespace metrics {

class BaseTestMetricsListener : public BnNetdEventListener {
  public:
    BaseTestMetricsListener() = default;
    ~BaseTestMetricsListener() = default;

    // Returns TRUE if the verification was successful. Otherwise, returns FALSE.
    virtual bool isVerified() = 0;

    std::condition_variable& getCv() { return mCv; }
    std::mutex& getCvMutex() { return mCvMutex; }

    android::binder::Status onDnsEvent(int32_t /*netId*/, int32_t /*eventType*/,
                                       int32_t /*returnCode*/, int32_t /*latencyMs*/,
                                       const std::string& /*hostname*/,
                                       const ::std::vector<std::string>& /*ipAddresses*/,
                                       int32_t /*ipAddressesCount*/, int32_t /*uid*/) override;
    android::binder::Status onPrivateDnsValidationEvent(int32_t /*netId*/,
                                                        const ::android::String16& /*ipAddress*/,
                                                        const ::android::String16& /*hostname*/,
                                                        bool /*validated*/) override;
    android::binder::Status onConnectEvent(int32_t /*netId*/, int32_t /*error*/,
                                           int32_t /*latencyMs*/,
                                           const ::android::String16& /*ipAddr*/, int32_t /*port*/,
                                           int32_t /*uid*/) override;
    android::binder::Status onWakeupEvent(const ::android::String16& /*prefix*/, int32_t /*uid*/,
                                          int32_t /*ethertype*/, int32_t /*ipNextHeader*/,
                                          const ::std::vector<uint8_t>& /*dstHw*/,
                                          const ::android::String16& /*srcIp*/,
                                          const ::android::String16& /*dstIp*/, int32_t /*srcPort*/,
                                          int32_t /*dstPort*/, int64_t /*timestampNs*/) override;
    android::binder::Status onTcpSocketStatsEvent(
            const ::std::vector<int32_t>& /*networkIds*/,
            const ::std::vector<int32_t>& /*sentPackets*/,
            const ::std::vector<int32_t>& /*lostPackets*/, const ::std::vector<int32_t>& /*rttUs*/,
            const ::std::vector<int32_t>& /*sentAckDiffMs*/) override;
    android::binder::Status onNat64PrefixEvent(int32_t /*netId*/, bool /*added*/,
                                               const ::std::string& /*prefixString*/,
                                               int32_t /*prefixLength*/) override;

  private:
    // The verified event(s) as a bitwise-OR combination of enum EventFlag flags.
    uint32_t mVerified{};

    // This lock prevents racing condition between signaling thread(s) and waiting thread(s).
    std::mutex mCvMutex;

    // Condition variable signaled when notify() is called.
    std::condition_variable mCv;

  protected:
    // Notify who is waiting for test results. See also mCvMutex and mCv.
    void notify();

    // Get current verified event(s).
    uint32_t getVerified() const { return mVerified; }

    // Set the specific event as verified if its verification was successful.
    void setVerified(EventFlag event);
};

}  // namespace metrics
}  // namespace net
}  // namespace android

#endif  // _BASE_TEST_METRICS_LISTENER_H_
