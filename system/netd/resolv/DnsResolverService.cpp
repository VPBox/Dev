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

#define LOG_TAG "DnsResolverService"

#include "DnsResolverService.h"

#include <set>
#include <vector>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <json/value.h>
#include <json/writer.h>
#include <log/log.h>
#include <netdutils/DumpWriter.h>
#include <openssl/base64.h>
#include <private/android_filesystem_config.h>  // AID_SYSTEM

#include "BinderUtil.h"
#include "DnsResolver.h"
#include "NetdConstants.h"    // SHA256_SIZE
#include "NetdPermissions.h"  // PERM_*
#include "ResolverEventReporter.h"
#include "resolv_cache.h"

using aidl::android::net::ResolverParamsParcel;
using android::base::Join;
using android::base::StringPrintf;
using android::netdutils::DumpWriter;

namespace android {
namespace net {

namespace {

#define ENFORCE_ANY_PERMISSION(...)                                      \
    do {                                                                 \
        ::ndk::ScopedAStatus status = checkAnyPermission({__VA_ARGS__}); \
        if (!status.isOk()) {                                            \
            return status;                                               \
        }                                                                \
    } while (0)

#define ENFORCE_INTERNAL_PERMISSIONS() \
    ENFORCE_ANY_PERMISSION(PERM_CONNECTIVITY_INTERNAL, PERM_MAINLINE_NETWORK_STACK)

#define ENFORCE_NETWORK_STACK_PERMISSIONS() \
    ENFORCE_ANY_PERMISSION(PERM_NETWORK_STACK, PERM_MAINLINE_NETWORK_STACK)

inline ::ndk::ScopedAStatus statusFromErrcode(int ret) {
    if (ret) {
        return ::ndk::ScopedAStatus(
                AStatus_fromServiceSpecificErrorWithMessage(-ret, strerror(-ret)));
    }
    return ::ndk::ScopedAStatus(AStatus_newOk());
}

}  // namespace

DnsResolverService::DnsResolverService() {
    // register log callback to BnDnsResolver::logFunc
    BnDnsResolver::logFunc =
            std::bind(binderCallLogFn, std::placeholders::_1,
                      [](const std::string& msg) { gResNetdCallbacks.log(msg.c_str()); });
}

binder_status_t DnsResolverService::start() {
    // TODO: Add disableBackgroundScheduling(true) after libbinder_ndk support it. b/126506010
    // NetdNativeService does call disableBackgroundScheduling currently, so it is fine now.
    DnsResolverService* resolverService = new DnsResolverService();
    binder_status_t status =
            AServiceManager_addService(resolverService->asBinder().get(), getServiceName());
    if (status != STATUS_OK) {
        return status;
    }

    ABinderProcess_startThreadPool();

    // TODO: register log callback if binder NDK backend support it. b/126501406

    return STATUS_OK;
}

binder_status_t DnsResolverService::dump(int fd, const char**, uint32_t) {
    auto dump_permission = checkAnyPermission({PERM_DUMP});
    if (!dump_permission.isOk()) {
        return STATUS_PERMISSION_DENIED;
    }

    // This method does not grab any locks. If individual classes need locking
    // their dump() methods MUST handle locking appropriately.
    DumpWriter dw(fd);
    for (auto netId : resolv_list_caches()) {
        dw.println("NetId: %u", netId);
        gDnsResolv->resolverCtrl.dump(dw, netId);
        dw.blankline();
    }

    return STATUS_OK;
}

::ndk::ScopedAStatus DnsResolverService::isAlive(bool* alive) {
    ENFORCE_INTERNAL_PERMISSIONS();

    *alive = true;

    return ::ndk::ScopedAStatus(AStatus_newOk());
}

::ndk::ScopedAStatus DnsResolverService::registerEventListener(
        const std::shared_ptr<aidl::android::net::metrics::INetdEventListener>& listener) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    int res = ResolverEventReporter::getInstance().addListener(listener);

    return statusFromErrcode(res);
}

::ndk::ScopedAStatus DnsResolverService::checkAnyPermission(
        const std::vector<const char*>& permissions) {
    // TODO: Remove callback and move this to unnamed namespace after libbiner_ndk supports
    // check_permission.
    if (!gResNetdCallbacks.check_calling_permission) {
        return ::ndk::ScopedAStatus(AStatus_fromExceptionCodeWithMessage(
                EX_NULL_POINTER, "check_calling_permission is null"));
    }
    pid_t pid = AIBinder_getCallingPid();
    uid_t uid = AIBinder_getCallingUid();

    // If the caller is the system UID, don't check permissions.
    // Otherwise, if the system server's binder thread pool is full, and all the threads are
    // blocked on a thread that's waiting for us to complete, we deadlock. http://b/69389492
    //
    // From a security perspective, there is currently no difference, because:
    // 1. The only permissions we check in netd's binder interface are CONNECTIVITY_INTERNAL
    //    and NETWORK_STACK, which the system server always has (or MAINLINE_NETWORK_STACK, which
    //    is equivalent to having both CONNECTIVITY_INTERNAL and NETWORK_STACK).
    // 2. AID_SYSTEM always has all permissions. See ActivityManager#checkComponentPermission.
    if (uid == AID_SYSTEM) {
        return ::ndk::ScopedAStatus(AStatus_newOk());
    }

    for (const char* permission : permissions) {
        if (gResNetdCallbacks.check_calling_permission(permission)) {
            return ::ndk::ScopedAStatus(AStatus_newOk());
        }
    }

    auto err = StringPrintf("UID %d / PID %d does not have any of the following permissions: %s",
                            uid, pid, Join(permissions, ',').c_str());
    return ::ndk::ScopedAStatus(AStatus_fromExceptionCodeWithMessage(EX_SECURITY, err.c_str()));
}

namespace {

// Parse a base64 encoded string into a vector of bytes.
// On failure, return an empty vector.
static std::vector<uint8_t> parseBase64(const std::string& input) {
    std::vector<uint8_t> decoded;
    size_t out_len;
    if (EVP_DecodedLength(&out_len, input.size()) != 1) {
        return decoded;
    }
    // out_len is now an upper bound on the output length.
    decoded.resize(out_len);
    if (EVP_DecodeBase64(decoded.data(), &out_len, decoded.size(),
                         reinterpret_cast<const uint8_t*>(input.data()), input.size()) == 1) {
        // Possibly shrink the vector if the actual output was smaller than the bound.
        decoded.resize(out_len);
    } else {
        decoded.clear();
    }
    if (out_len != SHA256_SIZE) {
        decoded.clear();
    }
    return decoded;
}

}  // namespace

::ndk::ScopedAStatus DnsResolverService::setResolverConfiguration(
        const ResolverParamsParcel& resolverParams) {
    // Locking happens in PrivateDnsConfiguration and res_* functions.
    ENFORCE_INTERNAL_PERMISSIONS();
    // TODO: Remove this log after AIDL gen_log supporting more types, b/129732660
    auto entry =
            gDnsResolverLog.newEntry()
                    .prettyFunction(__PRETTY_FUNCTION__)
                    .args(resolverParams.netId, resolverParams.servers, resolverParams.domains,
                          resolverParams.sampleValiditySeconds, resolverParams.successThreshold,
                          resolverParams.minSamples, resolverParams.maxSamples,
                          resolverParams.baseTimeoutMsec, resolverParams.retryCount,
                          resolverParams.tlsServers, resolverParams.tlsFingerprints);

    std::set<std::vector<uint8_t>> decoded_fingerprints;
    for (const std::string& fingerprint : resolverParams.tlsFingerprints) {
        std::vector<uint8_t> decoded = parseBase64(fingerprint);
        if (decoded.empty()) {
            return ::ndk::ScopedAStatus(AStatus_fromServiceSpecificErrorWithMessage(
                    EINVAL, "ResolverController error: bad fingerprint"));
        }
        decoded_fingerprints.emplace(decoded);
    }

    int res =
            gDnsResolv->resolverCtrl.setResolverConfiguration(resolverParams, decoded_fingerprints);

    gResNetdCallbacks.log(entry.returns(res).withAutomaticDuration().toString().c_str());

    return statusFromErrcode(res);
}

::ndk::ScopedAStatus DnsResolverService::getResolverInfo(
        int32_t netId, std::vector<std::string>* servers, std::vector<std::string>* domains,
        std::vector<std::string>* tlsServers, std::vector<int32_t>* params,
        std::vector<int32_t>* stats, std::vector<int32_t>* wait_for_pending_req_timeout_count) {
    // Locking happens in PrivateDnsConfiguration and res_* functions.
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    int res = gDnsResolv->resolverCtrl.getResolverInfo(netId, servers, domains, tlsServers, params,
                                                       stats, wait_for_pending_req_timeout_count);

    return statusFromErrcode(res);
}

::ndk::ScopedAStatus DnsResolverService::startPrefix64Discovery(int32_t netId) {
    // Locking happens in Dns64Configuration.
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    gDnsResolv->resolverCtrl.startPrefix64Discovery(netId);

    return ::ndk::ScopedAStatus(AStatus_newOk());
}

::ndk::ScopedAStatus DnsResolverService::stopPrefix64Discovery(int32_t netId) {
    // Locking happens in Dns64Configuration.
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    gDnsResolv->resolverCtrl.stopPrefix64Discovery(netId);

    return ::ndk::ScopedAStatus(AStatus_newOk());
}

::ndk::ScopedAStatus DnsResolverService::getPrefix64(int netId, std::string* stringPrefix) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    netdutils::IPPrefix prefix{};
    int res = gDnsResolv->resolverCtrl.getPrefix64(netId, &prefix);
    *stringPrefix = prefix.toString();

    return statusFromErrcode(res);
}

::ndk::ScopedAStatus DnsResolverService::setLogSeverity(int32_t logSeverity) {
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    int res = gDnsResolv->setLogSeverity(logSeverity);

    return statusFromErrcode(res);
}

::ndk::ScopedAStatus DnsResolverService::destroyNetworkCache(int netId) {
    // Locking happens in res_cache.cpp functions.
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    gDnsResolv->resolverCtrl.destroyNetworkCache(netId);

    return ::ndk::ScopedAStatus(AStatus_newOk());
}

::ndk::ScopedAStatus DnsResolverService::createNetworkCache(int netId) {
    // Locking happens in res_cache.cpp functions.
    ENFORCE_NETWORK_STACK_PERMISSIONS();

    int res = gDnsResolv->resolverCtrl.createNetworkCache(netId);

    return statusFromErrcode(res);
}

}  // namespace net
}  // namespace android
