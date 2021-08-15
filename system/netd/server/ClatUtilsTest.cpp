/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ClatUtilsTest.cpp - unit tests for ClatUtils.cpp
 */

#include <gtest/gtest.h>

#include "ClatUtils.h"

#include <linux/if_arp.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "bpf/BpfUtils.h"
#include "netdbpf/bpf_shared.h"

namespace android {
namespace net {

class ClatUtilsTest : public ::testing::Test {
  public:
    void SetUp() {}
};

TEST_F(ClatUtilsTest, HardwareAddressTypeOfNonExistingIf) {
    ASSERT_EQ(-ENODEV, hardwareAddressType("not_existing_if"));
}

TEST_F(ClatUtilsTest, HardwareAddressTypeOfLoopback) {
    ASSERT_EQ(ARPHRD_LOOPBACK, hardwareAddressType("lo"));
}

// If wireless 'wlan0' interface exists it should be Ethernet.
TEST_F(ClatUtilsTest, HardwareAddressTypeOfWireless) {
    int type = hardwareAddressType("wlan0");
    if (type == -ENODEV) return;

    ASSERT_EQ(ARPHRD_ETHER, type);
}

// If cellular 'rmnet_data0' interface exists it should
// *probably* not be Ethernet and instead be RawIp.
TEST_F(ClatUtilsTest, HardwareAddressTypeOfCellular) {
    int type = hardwareAddressType("rmnet_data0");
    if (type == -ENODEV) return;

    ASSERT_NE(ARPHRD_ETHER, type);

    // ARPHRD_RAWIP is 530 on some pre-4.14 Qualcomm devices.
    if (type == 530) return;

    ASSERT_EQ(ARPHRD_RAWIP, type);
}

TEST_F(ClatUtilsTest, GetClatMapFd) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    int fd = getClatIngressMapFd();
    ASSERT_LE(3, fd);  // 0,1,2 - stdin/out/err, thus 3 <= fd
    close(fd);
}

TEST_F(ClatUtilsTest, GetClatRawIpProgFd) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    int fd = getClatIngressProgFd(false);
    ASSERT_LE(3, fd);
    close(fd);
}

TEST_F(ClatUtilsTest, GetClatEtherProgFd) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    int fd = getClatIngressProgFd(true);
    ASSERT_LE(3, fd);
    close(fd);
}

TEST_F(ClatUtilsTest, TryOpeningNetlinkSocket) {
    int fd = openNetlinkSocket();
    ASSERT_LE(3, fd);
    close(fd);
}

// The SKIP_IF_BPF_NOT_SUPPORTED macro is effectively a check for 4.9+ kernel
// combined with a launched on P device.  Ie. it's a test for 4.9-P or better.

// NET_SCH_INGRESS is only enabled starting with 4.9-Q and as such we need
// a separate way to test for this...
int doKernelSupportsNetSchIngress(void) {
    // NOLINTNEXTLINE(cert-env33-c)
    return system("zcat /proc/config.gz | egrep -q '^CONFIG_NET_SCH_INGRESS=[my]$'");
}

// NET_CLS_BPF is only enabled starting with 4.9-Q...
int doKernelSupportsNetClsBpf(void) {
    // NOLINTNEXTLINE(cert-env33-c)
    return system("zcat /proc/config.gz | egrep -q '^CONFIG_NET_CLS_BPF=[my]$'");
}

// Make sure the above functions actually execute correctly rather than failing
// due to missing binary or execution failure...
TEST_F(ClatUtilsTest, KernelSupportsNetFuncs) {
    // Make sure the file is present and readable and decompressable.
    // NOLINTNEXTLINE(cert-env33-c)
    ASSERT_EQ(W_EXITCODE(0, 0), system("zcat /proc/config.gz > /dev/null"));

    int v = doKernelSupportsNetSchIngress();
    int w = doKernelSupportsNetClsBpf();

    // They should always either return 0 (match) or 1 (no match),
    // anything else is some sort of exec/environment/etc failure.
    if (v != W_EXITCODE(1, 0)) ASSERT_EQ(v, W_EXITCODE(0, 0));
    if (w != W_EXITCODE(1, 0)) ASSERT_EQ(w, W_EXITCODE(0, 0));
}

// True iff CONFIG_NET_SCH_INGRESS is enabled in /proc/config.gz
bool kernelSupportsNetSchIngress(void) {
    return doKernelSupportsNetSchIngress() == W_EXITCODE(0, 0);
}

// True iff CONFIG_NET_CLS_BPF is enabled in /proc/config.gz
bool kernelSupportsNetClsBpf(void) {
    return doKernelSupportsNetClsBpf() == W_EXITCODE(0, 0);
}

// See Linux kernel source in include/net/flow.h
#define LOOPBACK_IFINDEX 1

TEST_F(ClatUtilsTest, AttachReplaceDetachClsactLo) {
    // Technically does not depend on ebpf, but does depend on clsact,
    // and we do not really care if it works on pre-4.9-Q anyway.
    SKIP_IF_BPF_NOT_SUPPORTED;
    if (!kernelSupportsNetSchIngress()) return;

    int fd = openNetlinkSocket();
    ASSERT_LE(3, fd);

    // This attaches and detaches a configuration-less and thus no-op clsact
    // qdisc to loopback interface (and it takes fractions of a second)
    EXPECT_EQ(0, tcQdiscAddDevClsact(fd, LOOPBACK_IFINDEX));
    EXPECT_EQ(0, tcQdiscReplaceDevClsact(fd, LOOPBACK_IFINDEX));
    EXPECT_EQ(0, tcQdiscDelDevClsact(fd, LOOPBACK_IFINDEX));
    close(fd);
}

void checkAttachBpfFilterClsactLo(const bool ethernet) {
    // This test requires kernel 4.9-Q or better
    SKIP_IF_BPF_NOT_SUPPORTED;
    if (!kernelSupportsNetSchIngress()) return;
    if (!kernelSupportsNetClsBpf()) return;

    int bpf_fd = getClatIngressProgFd(false);
    ASSERT_LE(3, bpf_fd);

    int fd = openNetlinkSocket();
    EXPECT_LE(3, fd);
    if (fd >= 0) {
        // This attaches and detaches a clsact plus ebpf program to loopback
        // interface, but it should not affect traffic by virtue of us not
        // actually populating the ebpf control map.
        // Furthermore: it only takes fractions of a second.
        EXPECT_EQ(0, tcQdiscAddDevClsact(fd, LOOPBACK_IFINDEX));
        EXPECT_EQ(0, tcFilterAddDevBpf(fd, LOOPBACK_IFINDEX, bpf_fd, ethernet));
        EXPECT_EQ(0, tcQdiscDelDevClsact(fd, LOOPBACK_IFINDEX));
        close(fd);
    }

    close(bpf_fd);
}

TEST_F(ClatUtilsTest, CheckAttachBpfFilterRawIpClsactLo) {
    checkAttachBpfFilterClsactLo(false);
}

TEST_F(ClatUtilsTest, CheckAttachBpfFilterEthernetClsactLo) {
    checkAttachBpfFilterClsactLo(true);
}

}  // namespace net
}  // namespace android
