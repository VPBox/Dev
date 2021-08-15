/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "netdutils/Status.h"
#include "netdutils/StatusOr.h"

#include <sstream>

#include <gtest/gtest.h>

namespace android {
namespace netdutils {
namespace {

TEST(StatusTest, valueSemantics) {
    // Default constructor
    EXPECT_EQ(status::ok, Status());

    // Copy constructor
    Status status1(1);
    Status status2(status1);  // NOLINT(performance-unnecessary-copy-initialization)
    EXPECT_EQ(1, status2.code());

    // Copy assignment
    Status status3;
    status3 = status2;
    EXPECT_EQ(1, status3.code());

    // Same with const objects
    const Status status4(4);
    const Status status5(status4);  // NOLINT(performance-unnecessary-copy-initialization)
    Status status6;
    status6 = status5;
    EXPECT_EQ(4, status6.code());
}

TEST(StatusTest, errorMessages) {
    Status s(42, "for tea too");
    EXPECT_EQ(42, s.code());
    EXPECT_FALSE(s.ok());
    EXPECT_EQ(s.msg(), "for tea too");
}

TEST(StatusOrTest, moveSemantics) {
    // Status objects should be cheaply movable.
    EXPECT_TRUE(std::is_nothrow_move_constructible<Status>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<Status>::value);

    // Should move from a temporary Status (twice)
    Status s(Status(Status(42, "move me")));
    EXPECT_EQ(42, s.code());
    EXPECT_EQ(s.msg(), "move me");

    Status s2(666, "EDAEMON");
    EXPECT_NE(s, s2);
    s = s2;  // Invokes the move-assignment operator.
    EXPECT_EQ(666, s.code());
    EXPECT_EQ(s.msg(), "EDAEMON");
    EXPECT_EQ(s, s2);

    // A moved-from Status can be re-used.
    s2 = s;

    // Now both objects are valid.
    EXPECT_EQ(666, s.code());
    EXPECT_EQ(s.msg(), "EDAEMON");
    EXPECT_EQ(s, s2);
}

TEST(StatusTest, ignoredStatus) {
    statusFromErrno(ENOTTY, "Not a typewriter, what did you expect?").ignoreError();
}

TEST(StatusOrTest, ostream) {
    {
      StatusOr<int> so(11);
      std::stringstream ss;
      ss << so;
      // TODO: Fix StatusOr to optionally output "value:".
      EXPECT_EQ("StatusOr[status: Status[code: 0, msg: \"\"]]", ss.str());
    }
    {
      StatusOr<int> err(status::undefined);
      std::stringstream ss;
      ss << err;
      EXPECT_EQ("StatusOr[status: Status[code: 2147483647, msg: \"undefined\"]]", ss.str());
    }
}

}  // namespace
}  // namespace netdutils
}  // namespace android
