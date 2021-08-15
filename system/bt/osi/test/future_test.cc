/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include "AllocationTestHarness.h"

#include "common/message_loop_thread.h"
#include "osi/include/future.h"
#include "osi/include/osi.h"

using bluetooth::common::MessageLoopThread;

static const char* pass_back_data0 = "fancy a sandwich? it's a fancy sandwich";
static const char* pass_back_data1 =
    "what kind of ice cream truck plays the worst christmas song of all time?";

class FutureTest : public AllocationTestHarness {};

static void post_to_future(void* context) {
  future_ready((future_t*)context, (void*)pass_back_data0);
}

TEST_F(FutureTest, test_future_non_immediate) {
  future_t* future = future_new();
  ASSERT_TRUE(future != NULL);

  MessageLoopThread worker_thread("worker_thread");
  worker_thread.StartUp();
  worker_thread.DoInThread(FROM_HERE, base::Bind(post_to_future, future));

  EXPECT_EQ(pass_back_data0, future_await(future));

  worker_thread.ShutDown();
}

TEST_F(FutureTest, test_future_immediate) {
  future_t* future = future_new_immediate((void*)pass_back_data1);
  ASSERT_TRUE(future != NULL);
  EXPECT_EQ(pass_back_data1, future_await(future));
}
