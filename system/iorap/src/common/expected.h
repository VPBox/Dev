// Copyright (C) 2018 The Android Open Source Project
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

#ifndef IORAP_SRC_COMMON_EXPECTED_H_
#define IORAP_SRC_COMMON_EXPECTED_H_

#include <type_traits>
#include <utility>

#include <android-base/logging.h>  // CHECK/DCHECK.

// Ignore the tautological-undefined-compare warning.
// We obviously want to do this to protect against undefined behavior
// that sets a reference to a null value.
#define DCHECK_UB_NOT_NULL(x) \
  DCHECK(reinterpret_cast<volatile decltype(x)>(x) != nullptr)

/**
 * Result<Value, Error>-like interface.
 *
 * Subset of the experimental standard C++ proposal (p0323r3)
 *
 * Example:
 *
 *   expected<std::string, status_t> x = function_which_might_fail();
 *   if (x) {
 *     std::string str = x.value();
 *   } else {
 *     status_t err = x.error();
 *   }
 */

namespace iorap {
namespace detail {
  // Use perfect forwarding for expected_data constructors with overloading.
  struct expected_tag{};
  struct expected_tag_right : public expected_tag {
    static constexpr bool is_right_v = true;
  };
  struct expected_tag_error : public expected_tag {
    static constexpr bool is_right_v = false;
  };

  template <typename T, typename E, bool DefineDestructor>
  struct expected_data;

  // This doesn't always work because this code could be instantiated with a non-trivial T/E,
  // and then the union becomes invalid.
  template <typename T, typename E>
  struct expected_data<T, E, /*DefineDestructor*/true> {
    // Mark everything 'constexpr' to keep the code the same as the other partial specialization.

    template <typename U>
    constexpr expected_data(U&& either, expected_tag_right)
        : right_{std::forward<U>(either)}, is_right_{true} {}

    template <typename U>
    constexpr expected_data(U&& either, expected_tag_error)
        : error_{std::forward<U>(either)}, is_right_{false} {}

    constexpr bool has_value() const {
      return is_right_;
    }

    constexpr const T& value() const {
      return right_;
    }

    constexpr T& value() {
      return right_;
    }

    constexpr const E& error() const {
      return error_;
    }

    constexpr E& error() {
      return error_;
    }

    // Using an "anonymous union" here allows non-trivial types to be stored.
    union {
      T right_;
      E error_;
    };

    bool is_right_;

    // Below code differs slightly by handling non-trivial constructors/destructors.
    bool moved_from_{false};

    // Note: Destructors cannot be templated, so it is illegal to use SFINAE to try to
    // conditionalize this destructor somehow.
    ~expected_data() {
      if (moved_from_) { return; }
      if (is_right_) {
        right_.~T();
      } else {
        error_.~E();
      }
    }

    expected_data(expected_data&& other)
        noexcept(
            noexcept(T(std::move(other.right_))) &&
            noexcept(E(std::move(other.error_)))
        ) {
      DCHECK_UB_NOT_NULL(&other) << __PRETTY_FUNCTION__;
      DCHECK_EQ(other.moved_from_, false) << __PRETTY_FUNCTION__;
      if (other.is_right_) {
        new (&right_) T(std::move(other.right_));
      } else {
        new (&error_) E(std::move(other.error_));
      }
      other.moved_from_ = true;
      is_right_ = other.is_right_;
    }

    expected_data(const expected_data& other) {
      DCHECK_UB_NOT_NULL(&other) << __PRETTY_FUNCTION__;
      DCHECK_EQ(other.moved_from_, false) << __PRETTY_FUNCTION__;
      if (other.is_right_) {
        new (&right_) T(other.right_);
      } else {
        new (&error_) E(other.error_);
      }
      is_right_ = other.is_right_;
    }

    expected_data& operator=(const expected_data& other) {
      DCHECK_UB_NOT_NULL(&other) << __PRETTY_FUNCTION__;
      DCHECK_EQ(other.moved_from_, false) << __PRETTY_FUNCTION__;

      if (this == &other) {
        return *this;
      }

      if (other.is_right_) {
        if (!is_right_) {
          error_.~E();
          new (&right_) T(other.right_);
        } else {
          right_ = other.right_;
        }
      } else {
        if (is_right_) {
          right_.~T();
          new (&error_) E(other.error_);
        } else {
          error_ = other.error_;
        }
      }
      is_right_ = other.is_right_;

      return *this;
    }

    expected_data& operator=(expected_data&& other) {
      DCHECK_UB_NOT_NULL(&other) << __PRETTY_FUNCTION__;
      DCHECK_EQ(other.moved_from_, false) << __PRETTY_FUNCTION__;

      if (this == &other) {
        return *this;
      }

      if (other.is_right_) {
        if (!is_right_) {
          error_.~E();
          new (&right_) T(std::move(other.right_));
        } else {
          right_ = std::move(other.right_);
        }
      } else {
        if (is_right_) {
          right_.~T();
          new (&error_) E(std::move(other.error_));
        } else {
          error_ = std::move(other.error_);
        }
      }

      other.moved_from_ = true;
      is_right_ = other.is_right_;

      return *this;
    }
  };

  // Trivial-destructor copy of the above struct.
  //
  // A separate copy is required because otherwise compilation fails with an error about
  // the union having an implicitly deleted constructor.
  //
  // Having this implementation gives us the property that
  //
  //     (is_trivially_destructible<T> && is_trivially_destructible<E>
  //       ==> is_trivially_destructible<expected<T, E>>)
  template <typename T, typename E>
  struct expected_data<T, E, /*DefineDestructor*/false> {
    template <typename U>
    constexpr expected_data(U&& either, expected_tag_right)
        : right_{std::forward<U>(either)}, is_right_{true} {}

    template <typename U>
    constexpr expected_data(U&& either, expected_tag_error)
        : error_{std::forward<U>(either)}, is_right_{false} {}

    constexpr bool has_value() const {
      return is_right_;
    }

    constexpr const T& value() const {
      return right_;
    }

    constexpr T& value() {
      return right_;
    }

    constexpr const E& error() const {
      return error_;
    }

    constexpr E& error() {
      return error_;
    }

    // Using an "anonymous union" here allows non-trivial types to be stored.
    union {
      T right_;
      E error_;
    };

    bool is_right_;

    ~expected_data() = default;
  };

  // Select between trivial and non-trivial implementations. Trivial implementations
  // are more optimized and constexpr-compatible.
  template <typename T, typename E>
  using expected_pick_data_t =
      expected_data<T, E,
        !(std::is_trivially_destructible_v<T> && std::is_trivially_destructible_v<E>) >;
}  // namespace detail

template <typename E>
struct unexpected;

// Subset of std::experimental::expected proposal (p0323r3).
template <typename T, typename E>
struct expected {
  // Never-empty: expected<T,E> values have either 'T' or 'E' in them.
  template <typename U = T, typename _ = std::enable_if_t<std::is_default_constructible_v<U>>>
  constexpr expected() noexcept(noexcept(T{})) : expected(T{}) {}

  constexpr expected(const T& value) : data_{value, detail::expected_tag_right{}} {}
  constexpr expected(T&& value) : data_{std::move(value), detail::expected_tag_right{}} {}
  constexpr expected(const E& error) : data_{error, detail::expected_tag_error{}} {}
  constexpr expected(E&& error) : data_{std::move(error), detail::expected_tag_error{}} {}

  template <typename G = E>
  constexpr expected(unexpected<G> const& u) : expected{u.value()} {}

  template <typename G = E>
  constexpr expected(unexpected<G>&& u) : expected{std::move(u.value())} {}

  explicit constexpr operator bool() const {
    return has_value();
  }

  constexpr bool has_value() const {
    return data_.has_value();
  }

  constexpr const T& operator*() const {
    return data_.value();
  }

  constexpr T& operator*() {
    return data_.value();
  }

  // TODO: arrow operator?

  constexpr T& value() & {
    CHECK(has_value());
    return data_.value();
  }

  constexpr const T& value() const & {
    CHECK(has_value());
    return data_.value();
  }

  constexpr T&& value() && {
    CHECK(has_value());
    return std::move(data_.value());
  }

  constexpr const T& value() const && {
    CHECK(has_value());
    return std::move(data_.value());
  }

  constexpr E& error() {
    DCHECK(!has_value());
    return data_.error();
  }

  constexpr const E& error() const {
    DCHECK(!has_value());
    return data_.error();
  }

  // TODO: other functions such as operator=, unexpected, etc.
 private:
  detail::expected_pick_data_t<T, E> data_;
};

// TODO: move to tests file
namespace {
  struct TestType {
    TestType() {}
    ~TestType() {}
  };
  struct TestType2 : TestType {};

  static_assert(std::is_trivially_destructible_v<expected<int, /*error*/double> >);
  static_assert(!std::is_trivially_destructible_v<expected<TestType, /*error*/double> >);
  static_assert(!std::is_trivially_destructible_v<expected<int, /*error*/TestType> >);
  static_assert(!std::is_trivially_destructible_v<expected<TestType, /*error*/TestType2> >);

  // Ensure expected is constexpr-compatible.
  struct TestCase {
    static constexpr auto t1 = expected<int, double>{};
  };
}  // namespace <anonymous>

template <typename E>
struct unexpected {
  unexpected() = delete;
  constexpr explicit unexpected(const E& error) : error_{error} {}
  constexpr explicit unexpected(E&& error) : error_{std::move(error)} {}
  constexpr const E& value() const& { return error_; }
  constexpr E& value() & { return error_; }
  constexpr E&& value() && { return std::move(error_); }
  constexpr E const&& value() const&& { return std::move(error_); }
 private:
  E error_;
};

template <class E>
constexpr bool operator==(const unexpected<E>& x, const unexpected<E>& y) {
  return x.value() == y.value();
}

template <class E>
constexpr bool operator!=(const unexpected<E>& x, const unexpected<E>& y) {
  return !(x == y);
}

// TODO: move below codes to separate utils file
//
// future C++20 implementation of std::identity
struct identity {
  template <typename U>
  constexpr auto operator()(U&& v) const noexcept {
    return std::forward<U>(v);
  }
};

// Given a lambda [...](auto&& var) {...}
//   apply std::forward to 'var' to achieve perfect forwarding.
//
// Note that this doesn't work when var is a template type, i.e.
//   template <typename T>
//   void func(T&& tvar) {...}
//
// It would be invalid to use this macro with 'tvar' in that context.
#define IORAP_FORWARD_LAMBDA(var) std::forward<decltype(var)>(var)

// Borrowed non-null pointer, i.e. we do not own the lifetime.
//
// Function calls: This pointer is not used past the call.
// Struct fields: This pointer is not used past the lifetime of the struct.
template <class T, class = std::enable_if_t<std::is_pointer<T>::value>>
using borrowed = T _Nonnull;
// TODO: need a DCHECK or high warning levels, since null is technically well-defined.

}  // namespace iorap

#endif  // IORAP_SRC_COMMON_EXPECTED_H_
