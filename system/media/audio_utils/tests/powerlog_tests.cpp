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
#define LOG_TAG "audio_utils_powerlog_tests"

#include <audio_utils/PowerLog.h>
#include <gtest/gtest.h>
#include <iostream>
#include <log/log.h>

using namespace android;

static size_t countNewLines(const std::string &s) {
    return std::count(s.begin(), s.end(), '\n');
}

TEST(audio_utils_powerlog, basic) {
    auto plog = std::make_unique<PowerLog>(
            48000 /* sampleRate */,
            1 /* channelCount */,
            AUDIO_FORMAT_PCM_16_BIT,
            100 /* entries */,
            1 /* framesPerEntry */);

    // header
    EXPECT_EQ((size_t)1, countNewLines(plog->dumpToString()));

    const int16_t zero = 0;
    const int16_t half = 0x4000;

    plog->log(&half, 1 /* frame */, 0 /* nowNs */);
    plog->log(&half, 1 /* frame */, 1 /* nowNs */);
    plog->log(&half, 1 /* frame */, 2 /* nowNs */);

    // one line / signal
    EXPECT_EQ((size_t)2, countNewLines(plog->dumpToString()));

    plog->log(&zero, 1 /* frame */, 3 /* nowNs */);
    // zero termination doesn't change this.
    EXPECT_EQ((size_t)2, countNewLines(plog->dumpToString()));

    // but adding next line does.
    plog->log(&half, 1 /* frame */, 4 /* nowNs */);
    EXPECT_EQ((size_t)3, countNewLines(plog->dumpToString()));

    // truncating on lines
    EXPECT_EQ((size_t)2, countNewLines(plog->dumpToString(
            "" /* prefix */, 2 /* lines */)));

    // truncating on time
    EXPECT_EQ((size_t)3, countNewLines(plog->dumpToString(
            "" /* prefix */, 0 /* lines */, 2 /* limitNs */)));
    EXPECT_EQ((size_t)2, countNewLines(plog->dumpToString(
            "" /* prefix */, 0 /* lines */, 3 /* limitNs */)));
    plog->dump(0 /* fd (stdout) */);

    // The output below depends on the local time zone.
    // The indentation below is exact, check alignment.
    /*
Signal power history:
 12-31 16:00:00.000: [   -6.0   -6.0   -6.0 ] sum(-1.2)
 12-31 16:00:00.000: [   -6.0
     */
}

TEST(audio_utils_powerlog, c) {
    power_log_t *power_log = power_log_create(
            48000 /* sample_rate */,
            1 /* channel_count */,
            AUDIO_FORMAT_PCM_16_BIT,
            100 /* entries */,
            1 /* frames_per_entry */);

    // sanity test
    const int16_t zero = 0;
    const int16_t quarter = 0x2000;

    power_log_log(power_log, &quarter, 1 /* frame */, 0 /* now_ns */);
    power_log_log(power_log, &zero, 1 /* frame */, 1 /* now_ns */);
    power_log_dump(power_log, 0 /* fd */, "  " /* prefix */, 0 /* lines */, 0 /* limit_ns */);
    power_log_destroy(power_log);

    // This has a 2 character prefix offset from the previous test when dumping.
    // The indentation below is exact, check alignment.
    /*
  Signal power history:
   12-31 16:00:00.000: [  -12.0 ] sum(-12.0)
     */
}
