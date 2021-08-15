/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <endian.h>
#include <keymaster/logger.h>

#include "../auth_token_table.h"

using std::vector;

namespace keystore {
namespace test {

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

TEST(AuthTokenTableTest, Create) {
    AuthTokenTable table;
}

static HardwareAuthToken make_token(uint64_t rsid, uint64_t ssid = 0, uint64_t challenge = 0,
                                    uint64_t timestamp = 0) {
    HardwareAuthToken token;
    token.userId = rsid;
    token.authenticatorId = ssid;
    token.authenticatorType = HardwareAuthenticatorType::PASSWORD;
    token.challenge = challenge;
    token.timestamp = timestamp;
    return token;
}

static AuthorizationSet make_set(uint64_t rsid, uint32_t timeout = 10000) {
    AuthorizationSetBuilder builder;
    builder.Authorization(TAG_USER_AUTH_TYPE, HardwareAuthenticatorType::PASSWORD)
        .Authorization(TAG_USER_SECURE_ID, rsid);
    // Use timeout == 0 to indicate tags that require auth per operation.
    if (timeout != 0) builder.Authorization(TAG_AUTH_TIMEOUT, timeout);
    return std::move(builder);
}

// Tests obviously run so fast that a real-time clock with a one-second granularity rarely changes
// output during a test run.  This test clock "ticks" one second every time it's called.
static time_t monotonic_clock() {
    static time_t time = 0;
    return time++;
}

TEST(AuthTokenTableTest, SimpleAddAndFindTokens) {
    AuthTokenTable table;

    table.AddAuthenticationToken(make_token(1, 2));
    table.AddAuthenticationToken(make_token(3, 4));
    EXPECT_EQ(2U, table.size());

    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    ASSERT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(1U, found.userId);
    EXPECT_EQ(2U, found.authenticatorId);

    ASSERT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(1U, found.userId);
    EXPECT_EQ(2U, found.authenticatorId);

    ASSERT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(3U, found.userId);
    EXPECT_EQ(4U, found.authenticatorId);

    ASSERT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(4), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(3U, found.userId);
    EXPECT_EQ(4U, found.authenticatorId);

    ASSERT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(5), KeyPurpose::SIGN, 0), rc));
}

TEST(AuthTokenTableTest, FlushTable) {
    AuthTokenTable table(3, monotonic_clock);

    table.AddAuthenticationToken(make_token(1));
    table.AddAuthenticationToken(make_token(2));
    table.AddAuthenticationToken(make_token(3));

    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    // All three should be in the table.
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));

    table.Clear();
    EXPECT_EQ(0U, table.size());
}

TEST(AuthTokenTableTest, TableOverflow) {
    AuthTokenTable table(3, monotonic_clock);

    table.AddAuthenticationToken(make_token(1));
    table.AddAuthenticationToken(make_token(2));
    table.AddAuthenticationToken(make_token(3));

    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    // All three should be in the table.
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));

    table.AddAuthenticationToken(make_token(4));

    // Oldest should be gone.
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));

    // Others should be there, including the new one (4).  Search for it first, then the others, so
    // 4 becomes the least recently used.
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(4), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));

    table.AddAuthenticationToken(make_token(5));

    // 5 should have replaced 4.
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(4), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(5), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));

    table.AddAuthenticationToken(make_token(6));
    table.AddAuthenticationToken(make_token(7));

    // 2 and 5 should be gone
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(5), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(6), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(7), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));

    table.AddAuthenticationToken(make_token(8));
    table.AddAuthenticationToken(make_token(9));
    table.AddAuthenticationToken(make_token(10));

    // Only the three most recent should be there.
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(3), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(4), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(5), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(6), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
        (std::tie(rc, found) = table.FindAuthorization(make_set(7), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(8), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(9), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(10), KeyPurpose::SIGN, 0), rc));
}

TEST(AuthTokenTableTest, AuthenticationNotRequired) {
    AuthTokenTable table;
    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    EXPECT_EQ(AuthTokenTable::AUTH_NOT_REQUIRED,
              (std::tie(rc, found) = table.FindAuthorization(
                   AuthorizationSetBuilder().Authorization(TAG_NO_AUTH_REQUIRED), KeyPurpose::SIGN,
                   0 /* no challenge */),
               rc));
}

TEST(AuthTokenTableTest, OperationHandleNotFound) {
    AuthTokenTable table;
    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    table.AddAuthenticationToken(make_token(1, 0, 1, 5));
    EXPECT_EQ(AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
              (std::tie(rc, found) =
                   table.FindAuthorization(make_set(1, 0 /* no timeout */), KeyPurpose::SIGN,
                                           2 /* non-matching challenge */),
               rc));
    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(
                   make_set(1, 0 /* no timeout */), KeyPurpose::SIGN, 1 /* matching challenge */),
               rc));
    table.MarkCompleted(1);
    EXPECT_EQ(AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
              (std::tie(rc, found) = table.FindAuthorization(
                   make_set(1, 0 /* no timeout */), KeyPurpose::SIGN, 1 /* used challenge */),
               rc));
}

TEST(AuthTokenTableTest, OperationHandleRequired) {
    AuthTokenTable table;
    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    table.AddAuthenticationToken(make_token(1));
    EXPECT_EQ(AuthTokenTable::OP_HANDLE_REQUIRED,
              (std::tie(rc, found) = table.FindAuthorization(
                   make_set(1, 0 /* no timeout */), KeyPurpose::SIGN, 0 /* no op handle */),
               rc));
}

TEST(AuthTokenTableTest, AuthSidChanged) {
    AuthTokenTable table;
    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    table.AddAuthenticationToken(make_token(1, 3, /* op handle */ 1));
    EXPECT_EQ(AuthTokenTable::AUTH_TOKEN_WRONG_SID,
              (std::tie(rc, found) = table.FindAuthorization(make_set(2, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 1 /* op handle */),
               rc));
}

TEST(AuthTokenTableTest, TokenExpired) {
    AuthTokenTable table(5, monotonic_clock);
    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    auto key_info = make_set(1, 5 /* five second timeout */);

    // monotonic_clock "ticks" one second each time it's called, which is once per request, so the
    // sixth request should fail, since key_info says the key is good for five seconds.
    //
    // Note that this tests the decision of the AuthTokenTable to reject a request it knows is
    // expired.  An additional check of the secure timestamp (in the token) will be made by
    // keymaster when the found token is passed to it.
    table.AddAuthenticationToken(make_token(1, 0));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       key_info, KeyPurpose::SIGN, 0 /* no op handle */),
                                   rc));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       key_info, KeyPurpose::SIGN, 0 /* no op handle */),
                                   rc));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       key_info, KeyPurpose::SIGN, 0 /* no op handle */),
                                   rc));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       key_info, KeyPurpose::SIGN, 0 /* no op handle */),
                                   rc));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       key_info, KeyPurpose::SIGN, 0 /* no op handle */),
                                   rc));
    EXPECT_EQ(AuthTokenTable::AUTH_TOKEN_EXPIRED,
              (std::tie(rc, found) =
                   table.FindAuthorization(key_info, KeyPurpose::SIGN, 0 /* no op handle */),
               rc));
}

TEST(AuthTokenTableTest, MarkNonexistentEntryCompleted) {
    AuthTokenTable table;
    // Marking a nonexistent entry completed is ignored.  This test is mainly for code coverage.
    table.MarkCompleted(1);
}

TEST(AuthTokenTableTest, SupersededEntries) {
    AuthTokenTable table;
    AuthTokenTable::Error rc;
    HardwareAuthToken found;

    // Add two identical tokens, without challenges.  The second should supersede the first, based
    // on timestamp (fourth arg to make_token).
    table.AddAuthenticationToken(make_token(1, 0, 0, 0));
    table.AddAuthenticationToken(make_token(1, 0, 0, 1));
    EXPECT_EQ(1U, table.size());
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(1U, found.timestamp);

    // Add a third token, this with a different RSID.  It should not be superseded.
    table.AddAuthenticationToken(make_token(2, 0, 0, 2));
    EXPECT_EQ(2U, table.size());

    // Add two more, superseding each of the two in the table.
    table.AddAuthenticationToken(make_token(1, 0, 0, 3));
    table.AddAuthenticationToken(make_token(2, 0, 0, 4));
    EXPECT_EQ(2U, table.size());
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(1), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(3U, found.timestamp);
    EXPECT_EQ(
        AuthTokenTable::OK,
        (std::tie(rc, found) = table.FindAuthorization(make_set(2), KeyPurpose::SIGN, 0), rc));
    EXPECT_EQ(4U, found.timestamp);

    // Add another, this one with a challenge value.  It should supersede the old one since it is
    // newer, and matches other than the challenge.
    table.AddAuthenticationToken(make_token(1, 0, 1, 5));
    EXPECT_EQ(2U, table.size());

    // And another, also with a challenge.  Because of the challenge values, the one just added
    // cannot be superseded.
    table.AddAuthenticationToken(make_token(1, 0, 2, 6));
    EXPECT_EQ(3U, table.size());

    // Should be able to find each of them, by specifying their challenge, with a key that is not
    // timed (timed keys don't care about challenges).
    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout*/),
                                                             KeyPurpose::SIGN, 1 /* challenge */),
               rc));
    EXPECT_EQ(5U, found.timestamp);
    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 2 /* challenge */),
               rc));
    EXPECT_EQ(6U, found.timestamp);

    // Add another, without a challenge, and the same timestamp as the last one.  This new one
    // actually could be considered already-superseded, but the table doesn't handle that case,
    // since it seems unlikely to occur in practice.
    table.AddAuthenticationToken(make_token(1, 0, 0, 6));
    EXPECT_EQ(4U, table.size());
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       make_set(1), KeyPurpose::SIGN, 0 /* challenge */),
                                   rc));
    EXPECT_EQ(6U, found.timestamp);

    // Add another without a challenge but an increased timestamp. This should supersede the
    // previous challenge-free entry.
    table.AddAuthenticationToken(make_token(1, 0, 0, 7));
    EXPECT_EQ(4U, table.size());
    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 2 /* challenge */),
               rc));
    EXPECT_EQ(6U, found.timestamp);
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       make_set(1), KeyPurpose::SIGN, 0 /* challenge */),
                                   rc));
    EXPECT_EQ(7U, found.timestamp);

    // Mark the entry with challenge 2 as complete.  Since there's a newer challenge-free entry, the
    // challenge entry will be superseded.
    table.MarkCompleted(2);
    EXPECT_EQ(3U, table.size());
    EXPECT_EQ(AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 2 /* challenge */),
               rc));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       make_set(1), KeyPurpose::SIGN, 0 /* challenge */),
                                   rc));
    EXPECT_EQ(7U, found.timestamp);

    // Add another SID 1 entry with a challenge.  It supersedes the previous SID 1 entry with
    // no challenge (timestamp 7), but not the one with challenge 1 (timestamp 5).
    table.AddAuthenticationToken(make_token(1, 0, 3, 8));
    EXPECT_EQ(3U, table.size());

    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 1 /* challenge */),
               rc));
    EXPECT_EQ(5U, found.timestamp);

    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 3 /* challenge */),
               rc));
    EXPECT_EQ(8U, found.timestamp);

    // SID 2 entry is still there.
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       make_set(2), KeyPurpose::SIGN, 0 /* challenge */),
                                   rc));
    EXPECT_EQ(4U, found.timestamp);

    // Mark the entry with challenge 3 as complete.  Since the older challenge 1 entry is
    // incomplete, nothing is superseded.
    table.MarkCompleted(3);
    EXPECT_EQ(3U, table.size());

    EXPECT_EQ(AuthTokenTable::OK,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 1 /* challenge */),
               rc));
    EXPECT_EQ(5U, found.timestamp);

    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       make_set(1), KeyPurpose::SIGN, 0 /* challenge */),
                                   rc));
    EXPECT_EQ(8U, found.timestamp);

    // Mark the entry with challenge 1 as complete.  Since there's a newer one (with challenge 3,
    // completed), the challenge 1 entry is superseded and removed.
    table.MarkCompleted(1);
    EXPECT_EQ(2U, table.size());
    EXPECT_EQ(AuthTokenTable::AUTH_TOKEN_NOT_FOUND,
              (std::tie(rc, found) = table.FindAuthorization(make_set(1, 0 /* no timeout */),
                                                             KeyPurpose::SIGN, 1 /* challenge */),
               rc));
    EXPECT_EQ(AuthTokenTable::OK, (std::tie(rc, found) = table.FindAuthorization(
                                       make_set(1), KeyPurpose::SIGN, 0 /* challenge */),
                                   rc));
    EXPECT_EQ(8U, found.timestamp);
}

}  // namespace test
}  // namespace keystore
