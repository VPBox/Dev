/*
 * Copyright 2017 The Android Open Source Project
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
#define LOG_TAG "audio_utils_errorlog_tests"

#include <audio_utils/SimpleLog.h>
#include <gtest/gtest.h>
#include <iostream>
#include <log/log.h>

using namespace android;

static size_t countNewLines(const std::string &s) {
    return std::count(s.begin(), s.end(), '\n');
}

TEST(audio_utils_simplelog, basic) {
    auto slog = std::make_unique<SimpleLog>();
    const int64_t oneSecond = 1000000000;

    EXPECT_EQ((size_t)0, countNewLines(slog->dumpToString()));

    const int nine = 9;
    slog->log("Hello %d", nine);
    slog->log("World");

    slog->logs(-1 /* nowNs */, std::string("ABC")); // may take a std::string as well

    // two lines (no header)
    EXPECT_EQ((size_t)3, countNewLines(slog->dumpToString()));

    // another two lines (this is out of time order, but the log doesn't care)
    slog->log(oneSecond /* nowNs */, "Hello World %d", 10);
    slog->log(oneSecond * 2 /* nowNs */, "%s", "Goodbye");

    EXPECT_EQ((size_t)5, countNewLines(slog->dumpToString()));


    // truncate on lines
    EXPECT_EQ((size_t)1, countNewLines(slog->dumpToString("" /* prefix */, 1 /* lines */)));

    // truncate on time
    EXPECT_EQ((size_t)5, countNewLines(
            slog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond /* limitNs */)));

    // truncate on time (more)
    EXPECT_EQ((size_t)4, countNewLines(
            slog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond * 2 /* limitNs */)));

    // truncate on time (more)
    EXPECT_EQ((size_t)3, countNewLines(
            slog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond * 2 + 1 /* limitNs */)));

    std::cout << slog->dumpToString() << std::flush;

    slog->dump(0 /* fd (stdout) */, "  "); // add a prefix

    // The output below depends on the local time zone and current time.
    // The indentation below is exact, check alignment.
    /*
08-28 11:11:30.057 Hello 9
08-28 11:11:30.057 World
08-28 11:11:30.057 ABC
12-31 16:00:01.000 Hello World 10
12-31 16:00:02.000 Goodbye
  08-28 11:11:30.057 Hello 9
  08-28 11:11:30.057 World
  08-28 11:11:30.057 ABC
  12-31 16:00:01.000 Hello World 10
  12-31 16:00:02.000 Goodbye
     */
}
