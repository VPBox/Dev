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

#include <general_test/logging_sanity_test.h>

#include <cstddef>
#include <limits>

#include <shared/send_message.h>

#include <chre.h>

#include "chre/util/toolchain.h"

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {

LoggingSanityTest::LoggingSanityTest()
    : Test(CHRE_API_VERSION_1_0) {
}

void LoggingSanityTest::setUp(uint32_t messageSize,
                              const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "LoggingSanity message expects 0 additional bytes, got ",
        &messageSize);
  }

  // Test each warning level.
  chreLog(CHRE_LOG_ERROR, "Level: Error");
  chreLog(CHRE_LOG_WARN, "Level: Warn");
  chreLog(CHRE_LOG_INFO, "Level: Info");
  chreLog(CHRE_LOG_DEBUG, "Level: Debug");

  // Now we'll just test everything with INFO.
  constexpr chreLogLevel kInfo = CHRE_LOG_INFO;

  // Empty string
  chreLog(kInfo, "");

  // Try up through 10 arguments
  chreLog(kInfo, "%d", 1);
  chreLog(kInfo, "%d %d", 1, 2);
  chreLog(kInfo, "%d %d %d", 1, 2, 3);
  chreLog(kInfo, "%d %d %d %d", 1, 2, 3, 4);
  chreLog(kInfo, "%d %d %d %d %d", 1, 2, 3, 4, 5);
  chreLog(kInfo, "%d %d %d %d %d %d", 1, 2, 3, 4, 5, 6);
  chreLog(kInfo, "%d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7);
  chreLog(kInfo, "%d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8);
  chreLog(kInfo, "%d %d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8, 9);
  chreLog(kInfo, "%d %d %d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8, 9,
          10);

  // Various 'int' specifiers.  The value of the "%u" output depends on the
  // size of 'int' on this machine.
  chreLog(kInfo, "%d %u 0%o 0x%x 0x%X", -1, -1, 01234, 0xF4E, 0xF4E);

  // Generic testing of all specific types.  The format string is the same
  // as the chreLog() above us, just using the appropriate prefix for each.
  // We also use the min() value for all these signed types, assuring that
  // we'll get different %d vs %u output, and we'll get letters within our
  // %x and %X output.
#define INT_TYPES(kPrefix, type) \
  { \
    type value = std::numeric_limits<type>::min(); \
    chreLog(kInfo, "%" kPrefix "d %" kPrefix "u 0%" kPrefix "o 0x%" \
            kPrefix "x 0x%" kPrefix "X", value, value, value, value, \
            value); \
  }

  INT_TYPES("hh", char);
  INT_TYPES("h", short);
  INT_TYPES("l", long);
  INT_TYPES("ll", long long);
  INT_TYPES("z", size_t);
  INT_TYPES("t", ptrdiff_t);

  // Disables logging-related double promotion warnings
  CHRE_LOG_PREAMBLE

  float f = 12.34f;
  // Other required formats, including escaping the '%'.
  chreLog(kInfo, "%% %f %c %s %p", f, '?', "str", &f);

  // OPTIONAL specifiers.  See chreLog() API documentation for extensive
  // discussion of what OPTIONAL means.
  // <width> and '-'
  chreLog(kInfo, "(%5s) (%-5s) (%5d) (%-5d)", "str", "str", 10, 10);
  // '+'
  chreLog(kInfo, "(%+d) (%+d) (%+f) (%+f)", -5, 5, -5.f, 5.f);
  // ' '
  chreLog(kInfo, "(% d) (% d) (% f) (% f)", -5, 5, -5.f, 5.f);
  // '#'
  chreLog(kInfo, "%#o %#x %#X %#f", 8, 15, 15, 1.f);
  // '0' padding
  chreLog(kInfo, "%08d 0x%04x", 123, 0xF);
  // '.'<precision>
  chreLog(kInfo, "%.3d %.3d %.3f %.3f %.3s", 12, 1234, 1.5, 1.0625, "abcdef");

  // Re-enable logging-related double warnings
  CHRE_LOG_EPILOGUE

  // TODO: In some future Android release, when chreLog() is required to
  //     output to logcat, we'll just send a Continue to the Host and have
  //     the Host verify this output.  But for Android N, we leave it to
  //     the test runner to manually verify.
  sendSuccessToHost();
}

void LoggingSanityTest::handleEvent(uint32_t senderInstanceId,
                                    uint16_t eventType, const void* eventData) {
  unexpectedEvent(eventType);
}

}  // namespace general_test
