/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef ANDROID_APEXD_STRING_LOG_H_
#define ANDROID_APEXD_STRING_LOG_H_

// Simple helper class to create strings similar to LOGs.
// Usage sample:
//   std::string msg = StringLog() << "Hello " << std::hex << 1234;

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <errno.h>

template <typename T>
class BaseStringLog {
 public:
  BaseStringLog() {}

  // Pipe in values.
  template <class U>
  T& operator<<(const U& t) {
    os_stream << t;
    return static_cast<T&>(*this);
  }

  // Pipe in modifiers.
  T& operator<<(std::ostream& (*f)(std::ostream&)) {
    os_stream << f;
    return static_cast<T&>(*this);
  }

  // Get the current string.
  // NOLINTNEXTLINE(google-explicit-constructor)
  operator std::string() const { return os_stream.str(); }

 private:
  std::ostringstream os_stream;
};

class StringLog : public BaseStringLog<StringLog> {};

class PStringLog : public BaseStringLog<PStringLog> {
 public:
  PStringLog() : errno_(errno) {}

  // Get the current string.
  // NOLINTNEXTLINE(google-explicit-constructor)
  operator std::string() const {
    return (BaseStringLog::operator std::string())
        .append(": ")
        .append(strerror(errno_));
  }

 private:
  int errno_;
};

#endif  // ANDROID_APEXD_STRING_LOG_H_
