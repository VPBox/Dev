/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hidl/HidlLazyUtils.h>
#include <hidl/HidlTransportSupport.h>
#include <sys/wait.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/StrongPointer.h>

#pragma once

namespace android {
namespace hardware {
namespace details {
template <class Interface, typename Func>
__attribute__((warn_unused_result)) status_t registerPassthroughServiceImplementation(
    Func registerServiceCb, const std::string& name = "default") {
    sp<Interface> service = Interface::getService(name, true /* getStub */);

    if (service == nullptr) {
        ALOGE("Could not get passthrough implementation for %s/%s.",
            Interface::descriptor, name.c_str());
        return EXIT_FAILURE;
    }

    LOG_FATAL_IF(service->isRemote(), "Implementation of %s/%s is remote!",
            Interface::descriptor, name.c_str());

    status_t status = registerServiceCb(service, name);

    if (status == OK) {
        ALOGI("Registration complete for %s/%s.",
            Interface::descriptor, name.c_str());
    } else {
        ALOGE("Could not register service %s/%s (%d).",
            Interface::descriptor, name.c_str(), status);
    }

    return status;
}
}  // namespace details

/**
 * Registers passthrough service implementation.
 */
template <class Interface>
__attribute__((warn_unused_result)) status_t registerPassthroughServiceImplementation(
    const std::string& name = "default") {
    return details::registerPassthroughServiceImplementation<Interface>(
        [](const sp<Interface>& service, const std::string& name) {
            return service->registerAsService(name);
        },
        name);
}

/**
 * Creates default passthrough service implementation. This method never returns.
 *
 * Return value is exit status.
 */
template <class Interface>
__attribute__((warn_unused_result)) status_t defaultPassthroughServiceImplementation(
    const std::string& name, size_t maxThreads = 1) {
    configureRpcThreadpool(maxThreads, true);
    status_t result = registerPassthroughServiceImplementation<Interface>(name);

    if (result != OK) {
        return result;
    }

    joinRpcThreadpool();
    return UNKNOWN_ERROR;
}
template<class Interface>
__attribute__((warn_unused_result))
status_t defaultPassthroughServiceImplementation(size_t maxThreads = 1) {
    return defaultPassthroughServiceImplementation<Interface>("default", maxThreads);
}

/**
 * Registers a passthrough service implementation that exits when there are 0 clients.
 *
 * If this function is called multiple times to register different services, then this process will
 * only exit once all services have 0 clients. This function does not know about clients registered
 * through registerPassthroughServiceImplementation, so if that function is used in conjunction with
 * this one, the process may exit while a client is still using the HAL.
 */
template <class Interface>
__attribute__((warn_unused_result)) status_t registerLazyPassthroughServiceImplementation(
    const std::string& name = "default") {
    // Make LazyServiceRegistrar static so that multiple calls to
    // registerLazyPassthroughServiceImplementation work as expected: each HAL is registered and the
    // process only exits once all HALs have 0 clients.
    using android::hardware::LazyServiceRegistrar;
    static auto serviceCounter(std::make_shared<LazyServiceRegistrar>());

    return details::registerPassthroughServiceImplementation<Interface>(
        [](const sp<Interface>& service, const std::string& name) {
            return serviceCounter->registerService(service, name);
        },
        name);
}

/**
 * Creates default passthrough service implementation that exits when there are 0 clients. This
 * method never returns.
 *
 * Return value is exit status.
 */
template <class Interface>
__attribute__((warn_unused_result)) status_t defaultLazyPassthroughServiceImplementation(
    const std::string& name, size_t maxThreads = 1) {
    configureRpcThreadpool(maxThreads, true);
    status_t result = registerLazyPassthroughServiceImplementation<Interface>(name);

    if (result != OK) {
        return result;
    }

    joinRpcThreadpool();
    return UNKNOWN_ERROR;
}
template <class Interface>
__attribute__((warn_unused_result)) status_t defaultLazyPassthroughServiceImplementation(
    size_t maxThreads = 1) {
    return defaultLazyPassthroughServiceImplementation<Interface>("default", maxThreads);
}

}  // namespace hardware
}  // namespace android
