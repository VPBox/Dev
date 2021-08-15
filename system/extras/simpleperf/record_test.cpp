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

#include "event_attr.h"
#include "event_type.h"
#include "record.h"
#include "record_equal_test.h"

class RecordTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    const EventType* type = FindEventTypeByName("cpu-clock");
    ASSERT_TRUE(type != nullptr);
    event_attr = CreateDefaultPerfEventAttr(*type);
    event_attr.sample_id_all = 1;
  }

  void CheckRecordMatchBinary(Record& record) {
    std::vector<std::unique_ptr<Record>> records =
        ReadRecordsFromBuffer(event_attr, record.BinaryForTestingOnly(), record.size());
    ASSERT_EQ(1u, records.size());
    CheckRecordEqual(record, *records[0]);
  }

  perf_event_attr event_attr;
};

TEST_F(RecordTest, MmapRecordMatchBinary) {
  MmapRecord record(event_attr, true, 1, 2, 0x1000, 0x2000, 0x3000,
                    "MmapRecord", 0);
  CheckRecordMatchBinary(record);
}

TEST_F(RecordTest, CommRecordMatchBinary) {
  CommRecord record(event_attr, 1, 2, "CommRecord", 0, 7);
  CheckRecordMatchBinary(record);
}

TEST_F(RecordTest, SampleRecordMatchBinary) {
  event_attr.sample_type = PERF_SAMPLE_IP | PERF_SAMPLE_TID | PERF_SAMPLE_TIME
                           | PERF_SAMPLE_ID | PERF_SAMPLE_CPU
                           | PERF_SAMPLE_PERIOD | PERF_SAMPLE_CALLCHAIN;
  SampleRecord record(event_attr, 1, 2, 3, 4, 5, 6, 7, {8, 9, 10}, {}, 0);
  CheckRecordMatchBinary(record);
}

TEST_F(RecordTest, SampleRecord_exclude_kernel_callchain) {
  SampleRecord r(event_attr, 0, 1, 0, 0, 0, 0, 0, {}, {}, 0);
  ASSERT_TRUE(r.ExcludeKernelCallChain());

  event_attr.sample_type |= PERF_SAMPLE_CALLCHAIN;
  SampleRecord r1(event_attr, 0, 1, 0, 0, 0, 0, 0, {PERF_CONTEXT_USER, 2}, {}, 0);
  ASSERT_TRUE(r1.ExcludeKernelCallChain());
  ASSERT_EQ(2u, r1.ip_data.ip);
  SampleRecord r2(event_attr, r1.BinaryForTestingOnly());
  ASSERT_EQ(1u, r.ip_data.ip);
  ASSERT_EQ(2u, r2.callchain_data.ip_nr);
  ASSERT_EQ(PERF_CONTEXT_USER, r2.callchain_data.ips[0]);
  ASSERT_EQ(2u, r2.callchain_data.ips[1]);

  SampleRecord r3(event_attr, 0, 1, 0, 0, 0, 0, 0, {1, PERF_CONTEXT_USER, 2}, {}, 0);
  ASSERT_TRUE(r3.ExcludeKernelCallChain());
  ASSERT_EQ(2u, r3.ip_data.ip);
  SampleRecord r4(event_attr, r3.BinaryForTestingOnly());
  ASSERT_EQ(2u, r4.ip_data.ip);
  ASSERT_EQ(3u, r4.callchain_data.ip_nr);
  ASSERT_EQ(PERF_CONTEXT_USER, r4.callchain_data.ips[0]);
  ASSERT_EQ(PERF_CONTEXT_USER, r4.callchain_data.ips[1]);
  ASSERT_EQ(2u, r4.callchain_data.ips[2]);

  SampleRecord r5(event_attr, 0, 1, 0, 0, 0, 0, 0, {1, 2}, {}, 0);
  ASSERT_FALSE(r5.ExcludeKernelCallChain());
  SampleRecord r6(event_attr, 0, 1, 0, 0, 0, 0, 0, {1, 2, PERF_CONTEXT_USER}, {}, 0);
  ASSERT_FALSE(r6.ExcludeKernelCallChain());

  // Process consecutive context values.
  SampleRecord r7(event_attr, 0, 1, 0, 0, 0, 0, 0,
                  {1, 2, PERF_CONTEXT_USER, PERF_CONTEXT_USER, 3, 4}, {}, 0);
  r7.header.misc = PERF_RECORD_MISC_KERNEL;
  ASSERT_TRUE(r7.ExcludeKernelCallChain());
  CheckRecordEqual(r7, SampleRecord(event_attr, 0, 3, 0, 0, 0, 0, 0,
                                    {PERF_CONTEXT_USER, PERF_CONTEXT_USER, PERF_CONTEXT_USER,
                                     PERF_CONTEXT_USER, 3, 4}, {}, 0));
}

TEST_F(RecordTest, SampleRecord_ReplaceRegAndStackWithCallChain) {
  event_attr.sample_type |= PERF_SAMPLE_CALLCHAIN | PERF_SAMPLE_REGS_USER | PERF_SAMPLE_STACK_USER;
  SampleRecord expected(event_attr, 0, 1, 2, 3, 4, 5, 6, {1, PERF_CONTEXT_USER, 2, 3, 4, 5}, {},
                        0);
  for (size_t stack_size : {8, 1024}) {
    SampleRecord r(event_attr, 0, 1, 2, 3, 4, 5, 6, {1}, std::vector<char>(stack_size), 10);
    r.ReplaceRegAndStackWithCallChain({2, 3, 4, 5});
    CheckRecordMatchBinary(r);
    CheckRecordEqual(r, expected);
  }
}

TEST_F(RecordTest, SampleRecord_UpdateUserCallChain) {
  event_attr.sample_type |= PERF_SAMPLE_CALLCHAIN | PERF_SAMPLE_REGS_USER | PERF_SAMPLE_STACK_USER;
  SampleRecord r(event_attr, 0, 1, 2, 3, 4, 5, 6, {1, PERF_CONTEXT_USER, 2}, {}, 0);
  r.UpdateUserCallChain({3, 4, 5});
  CheckRecordMatchBinary(r);
  SampleRecord expected(event_attr, 0, 1, 2, 3, 4, 5, 6, {1, PERF_CONTEXT_USER, 3, 4, 5}, {}, 0);
  CheckRecordEqual(r, expected);
}

TEST_F(RecordTest, SampleRecord_AdjustCallChainGeneratedByKernel) {
  event_attr.sample_type |= PERF_SAMPLE_CALLCHAIN | PERF_SAMPLE_REGS_USER | PERF_SAMPLE_STACK_USER;
  SampleRecord r(event_attr, 0, 1, 2, 3, 4, 5, 6, {1, 5, 0, PERF_CONTEXT_USER, 6, 0}, {}, 0);
  r.header.misc = PERF_RECORD_MISC_KERNEL;
  r.AdjustCallChainGeneratedByKernel();
  uint64_t adjustValue = (GetBuildArch() == ARCH_ARM || GetBuildArch() == ARCH_ARM64) ? 2 : 1;
  SampleRecord expected(event_attr, 0, 1, 2, 3, 4, 5, 6,
                        {1, 5 - adjustValue, PERF_CONTEXT_KERNEL, PERF_CONTEXT_USER,
                         6 - adjustValue, PERF_CONTEXT_USER}, {}, 0);
  expected.header.misc = PERF_RECORD_MISC_KERNEL;
  CheckRecordEqual(r, expected);
}

TEST_F(RecordTest, CommRecord) {
  CommRecord r(event_attr, 1, 2, "init_name", 3, 4);
  size_t record_size = r.size();
  std::string new_name = "a_much_longer_name";
  r.SetCommandName(new_name);
  ASSERT_EQ(r.size(), record_size + 8);
  ASSERT_EQ(std::string(r.comm), new_name);
  ASSERT_EQ(r.data->pid, 1u);
  ASSERT_EQ(r.data->tid, 2u);
  ASSERT_EQ(r.sample_id.id_data.id, 3u);
  ASSERT_EQ(r.sample_id.time_data.time, 4u);
  CheckRecordMatchBinary(r);
}
