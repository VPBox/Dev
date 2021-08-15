/*
**
** Copyright 2018, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_CONCURRENCY_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_CONCURRENCY_H_

#include <type_traits>

namespace keystore {

template <typename LockedType> class UnlockProxyLockHelper {
  private:
    std::function<void(LockedType*)> unlock_;
    LockedType* value_;

  public:
    using lockedType = LockedType;
    UnlockProxyLockHelper() : value_(nullptr) {}
    UnlockProxyLockHelper(LockedType* value, std::function<void(LockedType*)>&& unlock)
        : unlock_(std::move(unlock)), value_(value) {}
    ~UnlockProxyLockHelper() {
        if (unlock_) unlock_(value_);
    }
    UnlockProxyLockHelper(UnlockProxyLockHelper&& rhs)
        : unlock_(std::move(rhs.unlock_)), value_(rhs.value_) {
        rhs.value_ = nullptr;
        rhs.unlock_ = {};
    }
    UnlockProxyLockHelper& operator=(UnlockProxyLockHelper&& rhs) {
        if (this != &rhs) {
            UnlockProxyLockHelper dummy(std::move(*this));
            unlock_ = std::move(rhs.unlock_);
            value_ = std::move(rhs.value_);
            rhs.value_ = nullptr;
            rhs.unlock_ = {};
        }
        return *this;
    }
    UnlockProxyLockHelper(const UnlockProxyLockHelper& rhs) = delete;
    UnlockProxyLockHelper& operator=(const UnlockProxyLockHelper& rhs) = delete;

    template <typename T = LockedType>
    std::enable_if_t<!std::is_const<LockedType>::value, T*> value() {
        return value_;
    }
    const LockedType* value() const { return value_; }
};

template <typename LockedType, typename MutexType, template <typename> class GuardType>
class MutexProxyLockHelper {
  private:
    GuardType<MutexType> lock_;
    LockedType* value_;

  public:
    using lockedType = LockedType;
    MutexProxyLockHelper() : value_(nullptr) {}
    MutexProxyLockHelper(LockedType* value, GuardType<MutexType>&& lock)
        : lock_(std::move(lock)), value_(value) {}

    template <typename T = LockedType>
    std::enable_if_t<!std::is_const<LockedType>::value, T*> value() {
        return value_;
    }
    const LockedType* value() const { return value_; }
};

template <typename Implementation> class ProxyLock {
  private:
    Implementation impl_;

  public:
    ProxyLock() : impl_() {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    template <typename... Args> ProxyLock(Args&&... args) : impl_{std::forward<Args>(args)...} {}
    explicit ProxyLock(Implementation&& impl) : impl_(std::move(impl)) {}
    explicit operator bool() const { return impl_.value() != nullptr; }

    template <typename T = typename Implementation::lockedType>
    std::enable_if_t<!std::is_const<typename Implementation::lockedType>::value, T*> operator->() {
        return impl_.value();
    }

    template <typename T = typename Implementation::lockedType>
    std::enable_if_t<!std::is_const<typename Implementation::lockedType>::value, T&> operator*() {
        return *impl_.value();
    }

    const std::remove_const_t<typename Implementation::lockedType>* operator->() const {
        return impl_.value();
    }

    const std::remove_const_t<typename Implementation::lockedType>& operator*() const {
        return *impl_.value();
    }
};

}  // namespace keystore

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_CONCURRENCY_H_
