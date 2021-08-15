/******************************************************************************
 *
 *  Copyright 2017 Google, Inc.
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
#include "btif/include/btif_profile_queue.h"

#include <gtest/gtest.h>

#include <base/bind.h>
#include <base/callback.h>
#include <base/location.h>

#include "stack_manager.h"
#include "types/raw_address.h"

typedef void(tBTIF_CBACK)(uint16_t event, char* p_param);
typedef void(tBTIF_COPY_CBACK)(uint16_t event, char* p_dest, char* p_src);

// NOTE: Local re-implementation of functions to avoid thread context switching
static bool sStackRunning;
bool get_stack_is_running(void) { return sStackRunning; }
static stack_manager_t sStackManager = {nullptr, nullptr, nullptr, nullptr,
                                        get_stack_is_running};
const stack_manager_t* stack_manager_get_interface() { return &sStackManager; }
bt_status_t do_in_jni_thread(const base::Location& from_here,
                             base::OnceClosure task) {
  std::move(task).Run();
  return BT_STATUS_SUCCESS;
}
bool is_on_jni_thread() { return true; }

enum ResultType {
  NOT_SET = 0,
  UNKNOWN,
  UUID1_ADDR1,
  UUID1_ADDR2,
  UUID2_ADDR1,
  UUID2_ADDR2
};

static ResultType sResult;

class BtifProfileQueueTest : public ::testing::Test {
 public:
  static const uint16_t kTestUuid1 = 0x9527;
  static const uint16_t kTestUuid2 = 0x819F;
  static const RawAddress kTestAddr1;
  static const RawAddress kTestAddr2;

 protected:
  void SetUp() override {
    sStackRunning = true;
    sResult = NOT_SET;
  };
  void TearDown() override { btif_queue_release(); };
};

const RawAddress BtifProfileQueueTest::kTestAddr1{
    {0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
const RawAddress BtifProfileQueueTest::kTestAddr2{
    {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56}};

static bt_status_t test_connect_cb(RawAddress* bda, uint16_t uuid) {
  sResult = UNKNOWN;
  if (*bda == BtifProfileQueueTest::kTestAddr1) {
    if (uuid == BtifProfileQueueTest::kTestUuid1) {
      sResult = UUID1_ADDR1;
    } else if (uuid == BtifProfileQueueTest::kTestUuid2) {
      sResult = UUID2_ADDR1;
    }
  } else if (*bda == BtifProfileQueueTest::kTestAddr2) {
    if (uuid == BtifProfileQueueTest::kTestUuid1) {
      sResult = UUID1_ADDR2;
    } else if (uuid == BtifProfileQueueTest::kTestUuid2) {
      sResult = UUID2_ADDR2;
    }
  }
  return BT_STATUS_SUCCESS;
}

TEST_F(BtifProfileQueueTest, test_connect) {
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
}

static bt_status_t test_connect_cb_fail(RawAddress* bda, uint16_t uuid) {
  sResult = UNKNOWN;
  if (*bda == BtifProfileQueueTest::kTestAddr1) {
    if (uuid == BtifProfileQueueTest::kTestUuid1) {
      sResult = UUID1_ADDR1;
    } else if (uuid == BtifProfileQueueTest::kTestUuid2) {
      sResult = UUID2_ADDR1;
    }
  } else if (*bda == BtifProfileQueueTest::kTestAddr2) {
    if (uuid == BtifProfileQueueTest::kTestUuid1) {
      sResult = UUID1_ADDR2;
    } else if (uuid == BtifProfileQueueTest::kTestUuid2) {
      sResult = UUID2_ADDR2;
    }
  }
  return BT_STATUS_BUSY;
}

TEST_F(BtifProfileQueueTest, test_connect_fail_still_can_advance_the_queue) {
  sResult = NOT_SET;
  // First connect-message for UUID1-ADDR1 is executed, but does not be removed
  // from connect-queue yet.
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  sResult = NOT_SET;
  // Second connect-message for UUID2-ADDR1 be pushed into connect-queue, but is
  // not executed
  btif_queue_connect(kTestUuid2, &kTestAddr1, test_connect_cb_fail);
  EXPECT_EQ(sResult, NOT_SET);
  // Third connect-message for UUID1-ADDR2 be pushed into connect-queue, but is
  // not executed
  btif_queue_connect(kTestUuid1, &kTestAddr2, test_connect_cb_fail);
  EXPECT_EQ(sResult, NOT_SET);
  // Fourth connect-message for UUID2-ADDR2 be pushed into connect-queue, but is
  // not executed
  btif_queue_connect(kTestUuid2, &kTestAddr2, test_connect_cb_fail);
  EXPECT_EQ(sResult, NOT_SET);
  // removed First connect-message from connect-queue, check it can advance to
  // subsequent connect-message.
  btif_queue_advance();
  EXPECT_EQ(sResult, UUID2_ADDR2);
}

TEST_F(BtifProfileQueueTest, test_connect_same_uuid_do_not_repeat) {
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  // Second connection request on the same UUID do not repeat
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Not even after we advance the queue
  sResult = NOT_SET;
  btif_queue_advance();
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
}

TEST_F(BtifProfileQueueTest, test_multiple_connects) {
  // First item is executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  // Second item with advance is executed
  sResult = NOT_SET;
  btif_queue_advance();
  btif_queue_connect(kTestUuid2, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID2_ADDR1);
}

TEST_F(BtifProfileQueueTest, test_multiple_connects_without_advance) {
  // First item is executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  // Second item without advance is not executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid2, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Third item for same UUID1, but different address ADDR2
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr2, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Fourth item for same UUID2, but different address ADDR2
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid2, &kTestAddr2, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Connect next doesn't work
  sResult = NOT_SET;
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
  // Advance moves queue to execute second item
  sResult = NOT_SET;
  btif_queue_advance();
  EXPECT_EQ(sResult, UUID2_ADDR1);
  // Advance moves queue to execute third item
  sResult = NOT_SET;
  btif_queue_advance();
  EXPECT_EQ(sResult, UUID1_ADDR2);
  // Advance moves queue to execute fourth item
  sResult = NOT_SET;
  btif_queue_advance();
  EXPECT_EQ(sResult, UUID2_ADDR2);
}

TEST_F(BtifProfileQueueTest, test_cleanup_first_allow_second) {
  // First item is executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  // Second item without advance is not executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid2, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Connect next doesn't work
  sResult = NOT_SET;
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
  // Cleanup UUID1 allows the next profile connection to be executed
  sResult = NOT_SET;
  btif_queue_cleanup(kTestUuid1);
  btif_queue_connect_next();
  EXPECT_EQ(sResult, UUID2_ADDR1);
}

TEST_F(BtifProfileQueueTest, test_cleanup_both) {
  // First item is executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  // Second item without advance is not executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid2, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Connect next doesn't work
  sResult = NOT_SET;
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
  // Cleanup both leaves nothing to execute
  sResult = NOT_SET;
  btif_queue_cleanup(kTestUuid1);
  btif_queue_cleanup(kTestUuid2);
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
}

TEST_F(BtifProfileQueueTest, test_cleanup_both_reverse_order) {
  // First item is executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid1, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, UUID1_ADDR1);
  // Second item without advance is not executed
  sResult = NOT_SET;
  btif_queue_connect(kTestUuid2, &kTestAddr1, test_connect_cb);
  EXPECT_EQ(sResult, NOT_SET);
  // Connect next doesn't work
  sResult = NOT_SET;
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
  // Cleanup both in reverse order leaves nothing to execute
  sResult = NOT_SET;
  btif_queue_cleanup(kTestUuid2);
  btif_queue_cleanup(kTestUuid1);
  btif_queue_connect_next();
  EXPECT_EQ(sResult, NOT_SET);
}
