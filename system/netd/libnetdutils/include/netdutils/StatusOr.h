/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef NETUTILS_STATUSOR_H
#define NETUTILS_STATUSOR_H

#include <cassert>
#include "netdutils/Status.h"

namespace android {
namespace netdutils {

// Wrapper around a combination of Status and application value type.
// T may be any copyable or movable type.
template <typename T>
class [[nodiscard]] StatusOr {
  public:
    // Constructs a new StatusOr with status::undefined status.
    // This is marked 'explicit' to try to catch cases like 'return {};',
    // where people think StatusOr<std::vector<int>> will be initialized
    // with an empty vector, instead of a status::undefined.
    explicit StatusOr() = default;

    // Implicit copy constructor and construction from T.
    // NOLINTNEXTLINE(google-explicit-constructor)
    StatusOr(Status status) : mStatus(std::move(status)) { assert(!isOk(mStatus)); }

    // Implicit construction from T. It is convenient and sensible to be able
    // to do 'return T()' when the return type is StatusOr<T>.
    // NOLINTNEXTLINE(google-explicit-constructor)
    StatusOr(const T& value) : mStatus(status::ok), mValue(value) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    StatusOr(T&& value) : mStatus(status::ok), mValue(std::move(value)) {}

    // Move constructor ok (if T supports move)
    StatusOr(StatusOr&&) noexcept = default;
    // Move assignment ok (if T supports move)
    StatusOr& operator=(StatusOr&&) noexcept = default;
    // Copy constructor ok (if T supports copy)
    StatusOr(const StatusOr&) = default;
    // Copy assignment ok (if T supports copy)
    StatusOr& operator=(const StatusOr&) = default;

    // Returns a const reference to wrapped type.
    // It is an error to call value() when !isOk(status())
    const T& value() const & { return mValue; }
    const T&& value() const && { return mValue; }

    // Returns an rvalue reference to wrapped type
    // It is an error to call value() when !isOk(status())
    //
    // If T is expensive to copy but supports efficient move, it can be moved
    // out of a StatusOr as follows:
    //   T value = std::move(statusor).value();
    T& value() & { return mValue; }
    T&& value() && { return mValue; }

    // Returns the Status object assigned at construction time.
    const Status status() const { return mStatus; }

    // Explicitly ignores the Status without triggering [[nodiscard]] errors.
    void ignoreError() const {}

    // Implicit cast to Status.
    // NOLINTNEXTLINE(google-explicit-constructor)
    operator Status() const { return status(); }

  private:
    Status mStatus = status::undefined;
    T mValue;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const StatusOr<T>& s) {
    return os << "StatusOr[status: " << s.status() << "]";
}

#define ASSIGN_OR_RETURN_IMPL(tmp, lhs, stmt) \
    auto tmp = (stmt);                        \
    RETURN_IF_NOT_OK(tmp);                    \
    lhs = std::move(tmp.value());

#define ASSIGN_OR_RETURN_CONCAT(line, lhs, stmt) \
    ASSIGN_OR_RETURN_IMPL(__CONCAT(_status_or_, line), lhs, stmt)

// Macro to allow exception-like handling of error return values.
//
// If the evaluation of stmt results in an error, return that error
// from the current function. Otherwise, assign the result to lhs.
//
// This macro supports both move and copy assignment operators. lhs
// may be either a new local variable or an existing non-const
// variable accessible in the current scope.
//
// Example usage:
// StatusOr<MyType> foo() { ... }
//
// ASSIGN_OR_RETURN(auto myVar, foo());
// ASSIGN_OR_RETURN(myExistingVar, foo());
// ASSIGN_OR_RETURN(myMemberVar, foo());
#define ASSIGN_OR_RETURN(lhs, stmt) ASSIGN_OR_RETURN_CONCAT(__LINE__, lhs, stmt)

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_STATUSOR_H */
