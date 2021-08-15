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

#include "BaseTestMetricsListener.h"

namespace android {
namespace net {
namespace metrics {

void BaseTestMetricsListener::notify() {
    std::lock_guard lock(mCvMutex);
    mCv.notify_one();
}

void BaseTestMetricsListener::setVerified(EventFlag event) {
    mVerified |= event;
}

android::binder::Status BaseTestMetricsListener::onDnsEvent(
        int32_t /*netId*/, int32_t /*eventType*/, int32_t /*returnCode*/, int32_t /*latencyMs*/,
        const std::string& /*hostname*/, const ::std::vector<std::string>& /*ipAddresses*/,
        int32_t /*ipAddressesCount*/, int32_t /*uid*/) {
    // default no-op
    return android::binder::Status::ok();
};

android::binder::Status BaseTestMetricsListener::onPrivateDnsValidationEvent(
        int32_t /*netId*/, const ::android::String16& /*ipAddress*/,
        const ::android::String16& /*hostname*/, bool /*validated*/) {
    // default no-op
    return android::binder::Status::ok();
};

android::binder::Status BaseTestMetricsListener::onConnectEvent(
        int32_t /*netId*/, int32_t /*error*/, int32_t /*latencyMs*/,
        const ::android::String16& /*ipAddr*/, int32_t /*port*/, int32_t /*uid*/) {
    // default no-op
    return android::binder::Status::ok();
};

android::binder::Status BaseTestMetricsListener::onWakeupEvent(
        const ::android::String16& /*prefix*/, int32_t /*uid*/, int32_t /*ethertype*/,
        int32_t /*ipNextHeader*/, const ::std::vector<uint8_t>& /*dstHw*/,
        const ::android::String16& /*srcIp*/, const ::android::String16& /*dstIp*/,
        int32_t /*srcPort*/, int32_t /*dstPort*/, int64_t /*timestampNs*/) {
    // default no-op
    return android::binder::Status::ok();
};

android::binder::Status BaseTestMetricsListener::onTcpSocketStatsEvent(
        const ::std::vector<int32_t>& /*networkIds*/, const ::std::vector<int32_t>& /*sentPackets*/,
        const ::std::vector<int32_t>& /*lostPackets*/, const ::std::vector<int32_t>& /*rttUs*/,
        const ::std::vector<int32_t>& /*sentAckDiffMs*/) {
    // default no-op
    return android::binder::Status::ok();
};

android::binder::Status BaseTestMetricsListener::onNat64PrefixEvent(
        int32_t /*netId*/, bool /*added*/, const ::std::string& /*prefixString*/,
        int32_t /*prefixLength*/) {
    // default no-op
    return android::binder::Status::ok();
};

}  // namespace metrics
}  // namespace net
}  // namespace android