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

#include <android-base/macros.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "include/bpf/BpfMap.h"
#include "include/bpf/BpfUtils.h"
#include "include/libbpf_android.h"

using ::testing::Test;

constexpr const char tp_prog_path[] =
        "/sys/fs/bpf/prog_bpf_load_tp_prog_tracepoint_sched_sched_switch";
constexpr const char tp_map_path[] = "/sys/fs/bpf/map_bpf_load_tp_prog_cpu_pid";

namespace android {
namespace bpf {

class BpfLoadTest : public testing::Test {
  protected:
    BpfLoadTest() {}
    int mProgFd, mMapFd;

    void SetUp() {
        SKIP_IF_BPF_NOT_SUPPORTED;

        unlink(tp_prog_path);
        unlink(tp_map_path);

        EXPECT_EQ(android::bpf::loadProg("/system/etc/bpf/bpf_load_tp_prog.o"), 0);

        mProgFd = bpf_obj_get(tp_prog_path);
        EXPECT_GT(mProgFd, 0);

        mMapFd = bpf_obj_get(tp_map_path);
        EXPECT_GT(mMapFd, 0);

        int ret = bpf_attach_tracepoint(mProgFd, "sched", "sched_switch");
        EXPECT_NE(ret, 0);
    }

    void TearDown() {
        SKIP_IF_BPF_NOT_SUPPORTED;

        close(mProgFd);
        close(mMapFd);
        unlink(tp_prog_path);
        unlink(tp_map_path);
    }

    void checkMapNonZero() {
        // The test program installs a tracepoint on sched:sched_switch
        // and expects the kernel to populate a PID corresponding to CPU
        android::bpf::BpfMap<uint32_t, uint32_t> m(mMapFd);

        // Wait for program to run a little
        sleep(1);

        int non_zero = 0;
        const auto iterFunc = [&non_zero](const uint32_t& key, const uint32_t& val,
                                          BpfMap<uint32_t, uint32_t>& map) {
            if (val && !non_zero) {
                non_zero = 1;
            }

            UNUSED(key);
            UNUSED(map);
            return android::netdutils::status::ok;
        };

        EXPECT_OK(m.iterateWithValue(iterFunc));
        EXPECT_EQ(non_zero, 1);
    }
};

TEST_F(BpfLoadTest, bpfCheckMap) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    checkMapNonZero();
}

}  // namespace bpf
}  // namespace android
