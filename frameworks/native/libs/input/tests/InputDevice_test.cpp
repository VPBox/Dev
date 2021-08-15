/*
 * Copyright (C) 2019 The Android Open Source Project
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


#include <gtest/gtest.h>
#include <input/InputDevice.h>

namespace android {

// --- InputDeviceIdentifierTest ---

TEST(InputDeviceIdentifierTest, getCanonicalName) {
    InputDeviceIdentifier identifier;
    identifier.name = "test device";
    ASSERT_EQ(std::string("test_device"), identifier.getCanonicalName());

    identifier.name = "deviceName-123 version_C!";
    ASSERT_EQ(std::string("deviceName-123_version_C_"), identifier.getCanonicalName());
}

} // namespace android