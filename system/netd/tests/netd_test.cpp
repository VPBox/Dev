/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless requied by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <errno.h>
#include <sys/capability.h>

#include <gtest/gtest.h>

TEST(NetUtilsWrapperTest, TestFileCapabilities) {
    errno = 0;
    ASSERT_EQ(NULL, cap_get_file("/system/bin/netutils-wrapper-1.0"));
    ASSERT_EQ(ENODATA, errno);
}

TEST(NetdSELinuxTest, CheckProperMTULabels) {
    // Since we expect the egrep regexp to filter everything out,
    // we thus expect no matches and thus a return code of 1
    // NOLINTNEXTLINE(cert-env33-c)
    ASSERT_EQ(W_EXITCODE(1, 0), system("ls -Z /sys/class/net/*/mtu | egrep -q -v "
                                       "'^u:object_r:sysfs_net:s0 /sys/class/net/'"));
}
