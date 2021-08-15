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

#ifndef TRUSTY_NVRAM_OPTIONAL_H_
#define TRUSTY_NVRAM_OPTIONAL_H_

namespace nvram {

// A generic option type. In addition to the underlying value, it holds a flag
// indicating whether the value is valid.
template <typename ValueType> class Optional {
 public:
  Optional() = default;
  explicit Optional(ValueType value) : value_(value), valid_(true) {}

  bool valid() const { return valid_; }
  const ValueType& value() const { return value_; }

  // Mark the object as valid and return a reference to the wrapped value.
  ValueType& Activate() {
    valid_ = true;
    value_ = ValueType{};
    return value_;
  }

  // Mark the object as invalid.
  void Clear() {
    valid_ = false;
  }

 private:
  ValueType value_{};
  bool valid_ = false;
};

}  // namespace nvram

#endif  // TRUSTY_NVRAM_OPTIONAL_H_
