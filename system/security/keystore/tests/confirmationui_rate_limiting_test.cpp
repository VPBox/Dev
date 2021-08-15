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

#include <gtest/gtest.h>

#include "../confirmationui_rate_limiting.h"
#include <keymaster/logger.h>

using std::vector;

namespace keystore {

namespace test {

namespace {

class StdoutLogger : public ::keymaster::Logger {
  public:
    StdoutLogger() { set_instance(this); }

    int log_msg(LogLevel level, const char* fmt, va_list args) const {
        int output_len = 0;
        switch (level) {
        case DEBUG_LVL:
            output_len = printf("DEBUG: ");
            break;
        case INFO_LVL:
            output_len = printf("INFO: ");
            break;
        case WARNING_LVL:
            output_len = printf("WARNING: ");
            break;
        case ERROR_LVL:
            output_len = printf("ERROR: ");
            break;
        case SEVERE_LVL:
            output_len = printf("SEVERE: ");
            break;
        }

        output_len += vprintf(fmt, args);
        output_len += printf("\n");
        return output_len;
    }
};

StdoutLogger logger;

class FakeClock : public std::chrono::steady_clock {
  private:
    static time_point sNow;

  public:
    static void setNow(time_point newNow) { sNow = newNow; }
    static time_point now() noexcept { return sNow; }
};

FakeClock::time_point FakeClock::sNow;

}  // namespace

/*
 * Test that there are no residual slots when various apps receive successful confirmations.
 */
TEST(ConfirmationUIRateLimitingTest, noPenaltyTest) {
    auto now = std::chrono::steady_clock::now();
    RateLimiting<FakeClock> rateLimiting;
    FakeClock::setNow(now);

    for (int i = 0; i < 10000; ++i) {
        ASSERT_TRUE(rateLimiting.tryPrompt(rand()));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    ASSERT_EQ(0U, rateLimiting.usedSlots());
}

TEST(ConfirmationUIRateLimitingTest, policyTest) {
    using namespace std::chrono_literals;
    auto now = std::chrono::steady_clock::now();
    RateLimiting<FakeClock> rateLimiting;
    FakeClock::setNow(now);

    // first three tries are free
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    // the next three tries get a 30s penalty
    for (int i = 3; i < 6; ++i) {
        FakeClock::setNow(FakeClock::now() + 29s);
        ASSERT_FALSE(rateLimiting.tryPrompt(20));
        FakeClock::setNow(FakeClock::now() + 1s);
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    // there after the penalty doubles with each cancellation
    for (int i = 6; i < 17; ++i) {
        FakeClock::setNow((FakeClock::now() + 60s * (1ULL << (i - 6))) - 1s);
        ASSERT_FALSE(rateLimiting.tryPrompt(20));
        FakeClock::setNow(FakeClock::now() + 1s);
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    ASSERT_EQ(1U, rateLimiting.usedSlots());

    FakeClock::setNow(FakeClock::now() + 24h - 1s);
    ASSERT_FALSE(rateLimiting.tryPrompt(20));

    // after 24h the counter is forgotten
    FakeClock::setNow(FakeClock::now() + 1s);
    ASSERT_TRUE(rateLimiting.tryPrompt(20));
    rateLimiting.processResult(ConfirmationResponseCode::Canceled);

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    for (int i = 1; i < 3; ++i) {
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    for (int i = 3; i < 6; ++i) {
        FakeClock::setNow(FakeClock::now() + 29s);
        ASSERT_FALSE(rateLimiting.tryPrompt(20));
        FakeClock::setNow(FakeClock::now() + 1s);
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    for (int i = 6; i < 17; ++i) {
        FakeClock::setNow((FakeClock::now() + 60s * (1ULL << (i - 6))) - 1s);
        ASSERT_FALSE(rateLimiting.tryPrompt(20));
        FakeClock::setNow(FakeClock::now() + 1s);
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    ASSERT_EQ(1U, rateLimiting.usedSlots());
}

TEST(ConfirmationUIRateLimitingTest, rewindTest) {
    using namespace std::chrono_literals;
    auto now = std::chrono::steady_clock::now();
    RateLimiting<FakeClock> rateLimiting;

    // first three tries are free
    for (int i = 0; i < 3; ++i) {
        FakeClock::setNow(now);
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    for (int i = 3; i < 6; ++i) {
        FakeClock::setNow(FakeClock::now() + 29s);
        ASSERT_FALSE(rateLimiting.tryPrompt(20));
        FakeClock::setNow(FakeClock::now() + 1s);
        ASSERT_TRUE(rateLimiting.tryPrompt(20));
        rateLimiting.processResult(ConfirmationResponseCode::Canceled);
    }

    FakeClock::setNow(FakeClock::now() + 59s);
    ASSERT_FALSE(rateLimiting.tryPrompt(20));
    FakeClock::setNow(FakeClock::now() + 1s);
    ASSERT_TRUE(rateLimiting.tryPrompt(20));
    rateLimiting.processResult(ConfirmationResponseCode::Aborted);

    FakeClock::setNow(FakeClock::now() - 1s);
    ASSERT_FALSE(rateLimiting.tryPrompt(20));
    FakeClock::setNow(FakeClock::now() + 1s);
    ASSERT_TRUE(rateLimiting.tryPrompt(20));
    rateLimiting.processResult(ConfirmationResponseCode::SystemError);

    // throw in a couple of successful confirmations by other apps to make sure there
    // is not cross talk
    for (int i = 0; i < 10000; ++i) {
        uid_t id = rand();
        if (id == 20) continue;
        ASSERT_TRUE(rateLimiting.tryPrompt(id));
        rateLimiting.processResult(ConfirmationResponseCode::OK);
    }

    FakeClock::setNow(FakeClock::now() - 1s);
    ASSERT_FALSE(rateLimiting.tryPrompt(20));
    FakeClock::setNow(FakeClock::now() + 1s);
    ASSERT_TRUE(rateLimiting.tryPrompt(20));
    rateLimiting.processResult(ConfirmationResponseCode::UIError);

    FakeClock::setNow(FakeClock::now() - 1s);
    ASSERT_FALSE(rateLimiting.tryPrompt(20));
    FakeClock::setNow(FakeClock::now() + 1s);
    ASSERT_TRUE(rateLimiting.tryPrompt(20));

    ASSERT_EQ(1U, rateLimiting.usedSlots());
}

}  // namespace test
}  // namespace keystore
