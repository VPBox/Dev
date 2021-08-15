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

#include <memory>

#include "wifilogd/tests/mock_command_processor.h"
#include "wifilogd/tests/mock_os.h"

namespace android {
namespace wifilogd {

namespace {
constexpr auto kBufferSizeBytes = 4096;
}

// If we've properly mocked out all of our methods, then the base class
// should never actually call over to the Os instance. Hence the use
// of a StrictMock<MockOs>.
MockCommandProcessor::MockCommandProcessor()
    : CommandProcessor(kBufferSizeBytes,
                       std::make_unique<::testing::StrictMock<MockOs>>()) {}

MockCommandProcessor::~MockCommandProcessor() {}

}  // namespace wifilogd
}  // namespace android
