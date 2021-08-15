/*
 * Copyright 2018 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_fdtostring_tests"
#include <log/log.h>

#include <audio_utils/FdToString.h>
#include <gtest/gtest.h>

using namespace android::audio_utils;

TEST(audio_utils_fdtostring, basic) {
    const std::string PREFIX{"aa "};
    const std::string TEST_STRING{"hello world"};

    FdToString fdToString(PREFIX);
    const int fd = fdToString.fd();
    ASSERT_TRUE(fd >= 0);

    write(fd, TEST_STRING.c_str(), TEST_STRING.size());

    const std::string result = fdToString.getStringAndClose();

    ASSERT_EQ((PREFIX + TEST_STRING), result);
}

TEST(audio_utils_fdtostring, multilines) {
    const std::string PREFIX{"aa "};
    const std::string DELIM{"\n"};
    const std::string TEST_STRING1{"hello world\n"};
    const std::string TEST_STRING2{"goodbye\n"};

    FdToString fdToString(PREFIX);
    const int fd = fdToString.fd();
    ASSERT_TRUE(fd >= 0);

    write(fd, TEST_STRING1.c_str(), TEST_STRING1.size());
    write(fd, DELIM.c_str(), DELIM.size()); // double newline
    write(fd, TEST_STRING2.c_str(), TEST_STRING2.size());

    const std::string result = fdToString.getStringAndClose();

    ASSERT_EQ((PREFIX + TEST_STRING1 + PREFIX + DELIM + PREFIX + TEST_STRING2), result);
}
