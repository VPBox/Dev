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

static void CheckMmapRecordDataEqual(const MmapRecord& r1, const MmapRecord& r2) {
  ASSERT_EQ(0, memcmp(r1.data, r2.data, sizeof(*r1.data)));
  ASSERT_STREQ(r1.filename, r2.filename);
}

static void CheckCommRecordDataEqual(const CommRecord& r1, const CommRecord& r2) {
  ASSERT_EQ(0, memcmp(r1.data, r2.data, sizeof(*r1.data)));
  ASSERT_STREQ(r1.comm, r2.comm);
}

static void CheckBuildIdRecordDataEqual(const BuildIdRecord& r1, const BuildIdRecord& r2) {
  ASSERT_EQ(r1.pid, r2.pid);
  ASSERT_EQ(r1.build_id, r2.build_id);
  ASSERT_STREQ(r1.filename, r2.filename);
}

static void CheckSampleRecordDataEqual(const SampleRecord& r1, const SampleRecord& r2) {
  ASSERT_EQ(r1.sample_type, r2.sample_type);
  if (r1.sample_type & PERF_SAMPLE_IP) {
    EXPECT_EQ(r1.ip_data.ip, r2.ip_data.ip);
  }
  if (r1.sample_type & PERF_SAMPLE_TID) {
    EXPECT_EQ(r1.tid_data.pid, r2.tid_data.pid);
    EXPECT_EQ(r1.tid_data.tid, r2.tid_data.tid);
  }
  if (r1.sample_type & PERF_SAMPLE_TIME) {
    EXPECT_EQ(r1.time_data.time, r2.time_data.time);
  }
  if (r1.sample_type & PERF_SAMPLE_ID) {
    EXPECT_EQ(r1.id_data.id, r2.id_data.id);
  }
  if (r1.sample_type & PERF_SAMPLE_CPU) {
    EXPECT_EQ(r1.cpu_data.cpu, r2.cpu_data.cpu);
  }
  if (r1.sample_type & PERF_SAMPLE_PERIOD) {
    EXPECT_EQ(r1.period_data.period, r2.period_data.period);
  }
  if (r1.sample_type & PERF_SAMPLE_CALLCHAIN) {
    ASSERT_EQ(r1.callchain_data.ip_nr, r2.callchain_data.ip_nr);
    for (size_t i = 0; i < r1.callchain_data.ip_nr; ++i) {
      EXPECT_EQ(r1.callchain_data.ips[i], r2.callchain_data.ips[i]);
    }
  }
  if (r1.sample_type & PERF_SAMPLE_REGS_USER) {
    ASSERT_EQ(r1.regs_user_data.abi, r2.regs_user_data.abi);
    if (r1.regs_user_data.abi != 0) {
      ASSERT_EQ(r1.regs_user_data.reg_mask, r2.regs_user_data.reg_mask);
      ASSERT_EQ(0, memcmp(r1.regs_user_data.regs, r2.regs_user_data.regs,
                          r1.regs_user_data.reg_nr * sizeof(uint64_t)));
    }
  }
  if (r1.sample_type & PERF_SAMPLE_STACK_USER) {
    ASSERT_EQ(r1.stack_user_data.size, r2.stack_user_data.size);
    if (r1.stack_user_data.size > 0) {
      ASSERT_EQ(r1.stack_user_data.dyn_size, r2.stack_user_data.dyn_size);
      ASSERT_EQ(0, memcmp(r1.stack_user_data.data, r2.stack_user_data.data,
                          r1.stack_user_data.size));
    }
  }
}

static void CheckRecordEqual(const Record& r1, const Record& r2) {
  ASSERT_EQ(r1.type(), r2.type());
  ASSERT_EQ(r1.misc(), r2.misc());
  ASSERT_EQ(r1.size(), r2.size());
  if (r1.type() == PERF_RECORD_SAMPLE) {
    CheckSampleRecordDataEqual(static_cast<const SampleRecord&>(r1),
                               static_cast<const SampleRecord&>(r2));
    return;
  }
  ASSERT_EQ(0, memcmp(&r1.sample_id, &r2.sample_id, sizeof(r1.sample_id)));
  if (r1.type() == PERF_RECORD_MMAP) {
    CheckMmapRecordDataEqual(static_cast<const MmapRecord&>(r1), static_cast<const MmapRecord&>(r2));
  } else if (r1.type() == PERF_RECORD_COMM) {
    CheckCommRecordDataEqual(static_cast<const CommRecord&>(r1), static_cast<const CommRecord&>(r2));
  } else if (r1.type() == PERF_RECORD_BUILD_ID) {
    CheckBuildIdRecordDataEqual(static_cast<const BuildIdRecord&>(r1),
                                static_cast<const BuildIdRecord&>(r2));
  }
}
