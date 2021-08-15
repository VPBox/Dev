/******************************************************************************
 *
 *  Copyright 2016 The Android Open Source Project
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

#include "bta/hf_client/bta_hf_client_int.h"
#include "bta/include/bta_hf_client_api.h"

namespace base {
class MessageLoop;
}  // namespace base

base::MessageLoop* get_main_message_loop() { return NULL; }

namespace {
const RawAddress bdaddr1({0x11, 0x22, 0x33, 0x44, 0x55, 0x66});
const RawAddress bdaddr2({0x66, 0x55, 0x44, 0x33, 0x22, 0x11});
}  // namespace

// TODO(jpawlowski): there is some weird dependency issue in tests, and the
// tests here fail to compile without this definition.
void LogMsg(uint32_t trace_set_mask, const char* fmt_str, ...) {}

class BtaHfClientTest : public testing::Test {
 protected:
  void SetUp() override {
    // Reset the memory block, this is the state on which the allocate handle
    // would start operating
    bta_hf_client_cb_arr_init();
  }
};

// Test that when we can allocate a device on the block and then check
// the status of the blocks
TEST_F(BtaHfClientTest, test_allocate_block_one_device) {
  uint16_t p_handle = 0;
  bool status = bta_hf_client_allocate_handle(bdaddr1, &p_handle);

  // Allocation should succeed
  EXPECT_EQ(true, status);
  EXPECT_GT(p_handle, 0);
}

// Test that we cannot allocate the same device on two separate control blocks
TEST_F(BtaHfClientTest, test_no_allocate_block_same_device) {
  uint16_t p_handle;
  bool status = bta_hf_client_allocate_handle(bdaddr1, &p_handle);

  // Allocation should succeed
  EXPECT_EQ(true, status);
  EXPECT_GT(p_handle, 0);

  EXPECT_TRUE(bta_hf_client_find_cb_by_bda(bdaddr1) != NULL);

  status = bta_hf_client_allocate_handle(bdaddr1, &p_handle);

  // Allocation should fail
  EXPECT_EQ(false, status);
}

// Test that we can allocate two different devices as separate control blocks
TEST_F(BtaHfClientTest, test_allocate_block_diff_device) {
  uint16_t p_handle_first;
  bool status = bta_hf_client_allocate_handle(bdaddr1, &p_handle_first);

  // Allocation should succeed
  EXPECT_EQ(true, status);
  EXPECT_GT(p_handle_first, 0);

  EXPECT_TRUE(bta_hf_client_find_cb_by_bda(bdaddr2) == NULL);

  uint16_t p_handle_second;
  status = bta_hf_client_allocate_handle(bdaddr2, &p_handle_second);

  // Allocation should succeed
  EXPECT_EQ(true, status);
  EXPECT_GT(p_handle_second, 0);
  EXPECT_NE(p_handle_first, p_handle_second);
}
