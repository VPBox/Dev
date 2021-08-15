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

#include "netdutils/BackoffSequence.h"

namespace android {
namespace netdutils {

TEST(BackoffSequence, defaults) {
    BackoffSequence<uint32_t> backoff;

    EXPECT_TRUE(backoff.hasNextTimeout());
    EXPECT_EQ(0x00000001U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000002U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000004U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000008U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000010U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000020U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000040U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000080U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000100U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000200U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000400U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000800U, backoff.getNextTimeout());
    EXPECT_EQ(0x00001000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00002000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00004000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00008000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00010000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00020000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00040000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00080000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00100000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00200000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00400000U, backoff.getNextTimeout());
    EXPECT_EQ(0x00800000U, backoff.getNextTimeout());
    EXPECT_EQ(0x01000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x02000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x04000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x08000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x10000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x20000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x40000000U, backoff.getNextTimeout());
    EXPECT_EQ(0x80000000U, backoff.getNextTimeout());
    // Maxes out, and stays there, ad infinitum.
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(backoff.hasNextTimeout());
        EXPECT_EQ(0xffffffffU, backoff.getNextTimeout());
    }
}

TEST(BackoffSequence, backoffToOncePerHour) {
    auto backoff = BackoffSequence<uint32_t>::Builder()
            .withInitialRetransmissionTime(1)
            .withMaximumRetransmissionTime(3600)
            .build();

    EXPECT_TRUE(backoff.hasNextTimeout());
    EXPECT_EQ(0x00000001U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000002U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000004U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000008U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000010U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000020U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000040U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000080U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000100U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000200U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000400U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000800U, backoff.getNextTimeout());
    // Maxes out, and stays there, ad infinitum.
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(backoff.hasNextTimeout());
        EXPECT_EQ(3600U, backoff.getNextTimeout());
    }
}

TEST(BackoffSequence, simpleMaxRetransCount) {
    auto backoff = BackoffSequence<uint32_t>::Builder()
            .withInitialRetransmissionTime(3)
            .withMaximumRetransmissionCount(7)
            .build();

    EXPECT_TRUE(backoff.hasNextTimeout());
    EXPECT_EQ(0x00000003U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000006U, backoff.getNextTimeout());
    EXPECT_EQ(0x0000000cU, backoff.getNextTimeout());
    EXPECT_EQ(0x00000018U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000030U, backoff.getNextTimeout());
    EXPECT_EQ(0x00000060U, backoff.getNextTimeout());
    EXPECT_EQ(0x000000c0U, backoff.getNextTimeout());

    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(backoff.hasNextTimeout());
        EXPECT_EQ(backoff.getEndOfSequenceIndicator(), backoff.getNextTimeout());
    }
}

TEST(BackoffSequence, simpleMaxDuration) {
    auto backoff = BackoffSequence<int>::Builder()
            .withInitialRetransmissionTime(3)
            .withMaximumRetransmissionDuration(7)
            .withEndOfSequenceIndicator(-1)
            .build();

    EXPECT_TRUE(backoff.hasNextTimeout());
    EXPECT_EQ(0x00000003, backoff.getNextTimeout());
    EXPECT_EQ(0x00000004, backoff.getNextTimeout());

    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(backoff.hasNextTimeout());
        EXPECT_EQ(backoff.getEndOfSequenceIndicator(), backoff.getNextTimeout());
        EXPECT_EQ(-1, backoff.getNextTimeout());
    }
}

TEST(PathologicalBackoffSequence, ZeroInitialRetransTime) {
    auto backoff = BackoffSequence<std::chrono::seconds>::Builder()
            .withInitialRetransmissionTime(std::chrono::seconds(0))
            .build();

    for (int i = 0; i < 10; i++) {
        // TODO: Decide whether this needs fixing, and how.
        EXPECT_TRUE(backoff.hasNextTimeout());
        EXPECT_EQ(backoff.getEndOfSequenceIndicator(), backoff.getNextTimeout());
    }
}

TEST(PathologicalBackoffSequence, MaxRetransDurationGreaterThanInitialRetransTime) {
    auto backoff = BackoffSequence<std::chrono::milliseconds>::Builder()
            .withInitialRetransmissionTime(std::chrono::milliseconds(5))
            .withMaximumRetransmissionDuration(std::chrono::milliseconds(3))
            .build();

    EXPECT_EQ(std::chrono::milliseconds(3), backoff.getNextTimeout());
    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(backoff.hasNextTimeout());
        EXPECT_EQ(backoff.getEndOfSequenceIndicator(), backoff.getNextTimeout());
    }
}

TEST(PathologicalBackoffSequence, MaxRetransDurationEqualsInitialRetransTime) {
    auto backoff = BackoffSequence<std::chrono::hours>::Builder()
            .withInitialRetransmissionTime(std::chrono::hours(5))
            .withMaximumRetransmissionDuration(std::chrono::hours(5))
            .build();

    EXPECT_EQ(std::chrono::hours(5), backoff.getNextTimeout());
    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(backoff.hasNextTimeout());
        EXPECT_EQ(backoff.getEndOfSequenceIndicator(), backoff.getNextTimeout());
    }
}

TEST(PathologicalBackoffSequence, MaxRetransTimeAndDurationGreaterThanInitialRetransTime) {
    auto backoff = BackoffSequence<std::chrono::nanoseconds>::Builder()
            .withInitialRetransmissionTime(std::chrono::nanoseconds(7))
            .withMaximumRetransmissionTime(std::chrono::nanoseconds(3))
            .withMaximumRetransmissionDuration(std::chrono::nanoseconds(5))
            .build();

    EXPECT_EQ(std::chrono::nanoseconds(3), backoff.getNextTimeout());
    EXPECT_EQ(std::chrono::nanoseconds(2), backoff.getNextTimeout());
    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(backoff.hasNextTimeout());
        EXPECT_EQ(backoff.getEndOfSequenceIndicator(), backoff.getNextTimeout());
    }
}

}  // namespace netdutils
}  // namespace android
