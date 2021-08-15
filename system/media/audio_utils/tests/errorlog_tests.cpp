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

#include <audio_utils/ErrorLog.h>
#include <gtest/gtest.h>
#include <iostream>
#include <log/log.h>

using namespace android;

static size_t countNewLines(const std::string &s) {
    return std::count(s.begin(), s.end(), '\n');
}

TEST(audio_utils_errorlog, basic) {
    auto elog = std::make_unique<ErrorLog<int32_t>>(100 /* lines */);
    const int64_t oneSecond = 1000000000;

    EXPECT_EQ((size_t)1, countNewLines(elog->dumpToString()));

    elog->log(1 /* code */, 0 /* nowNs */);
    elog->log(2 /* code */, 1 /* nowNs */);

    // two separate errors (4 lines including 2 header lines)
    EXPECT_EQ((size_t)4, countNewLines(elog->dumpToString()));

    // aggregation at (oneSecond - 1)
    elog->log(2 /* code */, oneSecond /* nowNs */);
    EXPECT_EQ((size_t)4, countNewLines(elog->dumpToString()));

    // no aggregation if spaced exactly one second apart
    elog->log(2 /* code */, oneSecond * 2 /* nowNs */);
    EXPECT_EQ((size_t)5, countNewLines(elog->dumpToString()));

    // Check log:
    // truncate on lines
    EXPECT_EQ((size_t)3, countNewLines(elog->dumpToString("" /* prefix */, 3 /* lines */)));

    // truncate on time
    EXPECT_EQ((size_t)4, countNewLines(
            elog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond - 1 /* limitNs */)));

    // truncate on time
    EXPECT_EQ((size_t)4, countNewLines(
            elog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond /* limitNs */)));

    // truncate on time (more)
    EXPECT_EQ((size_t)3, countNewLines(
            elog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond + 1 /* limitNs */)));

    // truncate on time
    EXPECT_EQ((size_t)3, countNewLines(
            elog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond * 2 /* limitNs */)));

    // truncate on time (to first header line)
    EXPECT_EQ((size_t)1, countNewLines(
            elog->dumpToString("" /* prefix */, 0 /* lines */, oneSecond * 2 + 1/* limitNs */)));

    elog->dump(0 /* fd (stdout) */);

    // The output below depends on the local time zone.
    // The indentation below is exact, check alignment.
    /*
Errors: 4
 Code  Freq          First time           Last time
    1     1  12-31 16:00:00.000  12-31 16:00:00.000
    2     2  12-31 16:00:00.000  12-31 16:00:01.000
    2     1  12-31 16:00:02.000  12-31 16:00:02.000
     */
}

TEST(audio_utils_errorlog, c) {
    error_log_t *error_log =
            error_log_create(100 /* lines */, 1000000000 /* one second aggregation */);

    // just a sanity test
    error_log_log(error_log, 2 /* code */, 1 /* now_ns */);
    error_log_dump(error_log, 0 /* fd */, "  " /* prefix */, 0 /* lines */, 0 /* limit_ns */);
    error_log_destroy(error_log);

    // This has a 2 character prefix offset from the previous test when dumping.
    // The indentation below is exact, check alignment.
    /*
  Errors: 1
   Code  Freq          First time           Last time
      2     1  12-31 16:00:00.000  12-31 16:00:00.000
     */
}
