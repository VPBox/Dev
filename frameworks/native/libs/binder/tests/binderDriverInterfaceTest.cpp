/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <gtest/gtest.h>
#include <linux/android/binder.h>
#include <binder/IBinder.h>
#include <sys/mman.h>
#include <poll.h>

#define BINDER_DEV_NAME "/dev/binder"

testing::Environment* binder_env;

class BinderDriverInterfaceTestEnv : public ::testing::Environment {
        virtual void SetUp() {
            int ret;
            uint32_t max_threads = 0;

            m_binderFd = open(BINDER_DEV_NAME, O_RDWR | O_NONBLOCK | O_CLOEXEC);
            ASSERT_GE(m_binderFd, 0);
            m_buffer = mmap(nullptr, 64*1024, PROT_READ, MAP_SHARED, m_binderFd, 0);
            ASSERT_NE(m_buffer, (void *)nullptr);
            ret = ioctl(m_binderFd, BINDER_SET_MAX_THREADS, &max_threads);
            EXPECT_EQ(0, ret);
            EnterLooper();
        }
        virtual void TearDown() {
            close(m_binderFd);
        }
    private:
        int m_binderFd;
        void *m_buffer;
    public:
        int getBinderFd(void) {
            return m_binderFd;
        }
        void EnterLooper(void) {
            int ret;
            const uint32_t bc[] = {
                BC_ENTER_LOOPER,
            };
            struct binder_write_read bwr = binder_write_read();
            bwr.write_buffer = (uintptr_t)bc;
            bwr.write_size = sizeof(bc);
            ret = ioctl(m_binderFd, BINDER_WRITE_READ, &bwr);
            EXPECT_EQ(0, ret);
            if (ret < 0) {
                    EXPECT_EQ(0, errno);
            }
            EXPECT_EQ(sizeof(bc), bwr.write_consumed);
        }
};

class BinderDriverInterfaceTest : public ::testing::Test {
    public:
        virtual void SetUp() {
            m_binderFd = static_cast<BinderDriverInterfaceTestEnv *>(binder_env)->getBinderFd();
        }
        virtual void TearDown() {
        }
    protected:
        /* The ioctl must either return 0, or if it doesn't errno should be accepted_errno */
        void binderTestIoctlSuccessOrError(int cmd, void *arg, int accepted_errno) {
            int ret;

            ret = ioctl(m_binderFd, cmd, arg);
            if (ret != 0) {
                EXPECT_EQ(errno, accepted_errno);
            }
        }

        void binderTestIoctlRetErr2(int cmd, void *arg, int expect_ret, int expect_errno, int accept_errno) {
            int ret;

            ret = ioctl(m_binderFd, cmd, arg);
            EXPECT_EQ(expect_ret, ret);
            if (ret < 0) {
                if (errno != accept_errno)
                    EXPECT_EQ(expect_errno, errno);
            }
        }
        void binderTestIoctlErr2(int cmd, void *arg, int expect_errno, int accept_errno) {
            binderTestIoctlRetErr2(cmd, arg, -1, expect_errno, accept_errno);
        }
        void binderTestIoctlErr1(int cmd, void *arg, int expect_errno) {
            binderTestIoctlErr2(cmd, arg, expect_errno, expect_errno);
        }
        void binderTestIoctl(int cmd, void *arg) {
            binderTestIoctlRetErr2(cmd, arg, 0, 0, 0);
        }
        void binderTestIoctlUnimplemented(int cmd, void *arg) {
            int ret;

            ret = ioctl(m_binderFd, cmd, arg);
            if (ret < 0) {
                /* Not currently implmented. Allow ret == -1, errno == EINVAL */
                EXPECT_EQ(-1, ret);
                EXPECT_EQ(EINVAL, errno);
            }
        }
        void binderTestReadEmpty(void) {
            size_t i;
            uint32_t br[32];
            struct binder_write_read bwr = binder_write_read();
            SCOPED_TRACE("TestReadEmpty");
            bwr.read_buffer = (uintptr_t)br;
            bwr.read_size = sizeof(br);
            binderTestIoctlErr1(BINDER_WRITE_READ, &bwr, EAGAIN);
            EXPECT_EQ(0u, bwr.read_consumed);
            for (i = 0; i * sizeof(uint32_t) < bwr.read_consumed; i++) {
                SCOPED_TRACE(testing::Message() << "i = " << i);
                EXPECT_EQ(BR_NOOP, br[i]);
            }
        }
        void binderWaitForReadData(int timeout_ms) {
            int ret;
            pollfd pfd = pollfd();

            pfd.fd = m_binderFd;
            pfd.events = POLLIN;
            ret = poll(&pfd, 1, timeout_ms);
            EXPECT_EQ(1, ret);
        }
    private:
        int m_binderFd;
};

TEST_F(BinderDriverInterfaceTest, Version) {
    struct binder_version version;
    binderTestIoctl(BINDER_VERSION, &version);
    ASSERT_EQ(BINDER_CURRENT_PROTOCOL_VERSION, version.protocol_version);
}

TEST_F(BinderDriverInterfaceTest, OpenNoMmap) {
    int binderFd = open(BINDER_DEV_NAME, O_RDWR | O_NONBLOCK | O_CLOEXEC);
    ASSERT_GE(binderFd, 0);
    close(binderFd);
}

TEST_F(BinderDriverInterfaceTest, WriteReadNull) {
    binderTestIoctlErr1(BINDER_WRITE_READ, nullptr, EFAULT);
}

TEST_F(BinderDriverInterfaceTest, SetIdleTimeoutNull) {
    binderTestIoctlErr2(BINDER_SET_IDLE_TIMEOUT, nullptr, EFAULT, EINVAL);
}

TEST_F(BinderDriverInterfaceTest, SetMaxThreadsNull) {
    binderTestIoctlErr2(BINDER_SET_MAX_THREADS, nullptr, EFAULT, EINVAL); /* TODO: don't accept EINVAL */
}

TEST_F(BinderDriverInterfaceTest, SetIdlePriorityNull) {
    binderTestIoctlErr2(BINDER_SET_IDLE_PRIORITY, nullptr, EFAULT, EINVAL);
}

TEST_F(BinderDriverInterfaceTest, VersionNull) {
    binderTestIoctlErr2(BINDER_VERSION, nullptr, EFAULT, EINVAL); /* TODO: don't accept EINVAL */
}

TEST_F(BinderDriverInterfaceTest, SetIdleTimeoutNoTest) {
    int64_t idle_timeout = 100000;
    binderTestIoctlUnimplemented(BINDER_SET_IDLE_TIMEOUT, &idle_timeout);
}

TEST_F(BinderDriverInterfaceTest, SetMaxThreads) {
    uint32_t max_threads = 0;
    binderTestIoctl(BINDER_SET_MAX_THREADS, &max_threads);
}

TEST_F(BinderDriverInterfaceTest, SetIdlePriorityNoTest) {
    int idle_priority = 0;
    binderTestIoctlUnimplemented(BINDER_SET_IDLE_PRIORITY, &idle_priority);
}

TEST_F(BinderDriverInterfaceTest, SetContextMgrBusy) {
    int32_t dummy = 0;
    binderTestIoctlErr1(BINDER_SET_CONTEXT_MGR, &dummy, EBUSY);
}

TEST_F(BinderDriverInterfaceTest, ThreadExit) {
    int32_t dummy = 0;
    binderTestIoctl(BINDER_THREAD_EXIT, &dummy);
    static_cast<BinderDriverInterfaceTestEnv *>(binder_env)->EnterLooper();
}

TEST_F(BinderDriverInterfaceTest, WriteReadEmpty) {
    struct binder_write_read bwr = binder_write_read();
    binderTestIoctl(BINDER_WRITE_READ, &bwr);
}

TEST_F(BinderDriverInterfaceTest, Read) {
    binderTestReadEmpty();
}

TEST_F(BinderDriverInterfaceTest, IncRefsAcquireReleaseDecRefs) {
    const uint32_t bc[] = {
        BC_INCREFS,
        0,
        BC_ACQUIRE,
        0,
        BC_RELEASE,
        0,
        BC_DECREFS,
        0,
    };
    struct binder_write_read bwr = binder_write_read();
    bwr.write_buffer = (uintptr_t)bc;
    bwr.write_size = sizeof(bc);
    binderTestIoctl(BINDER_WRITE_READ, &bwr);
    EXPECT_EQ(sizeof(bc), bwr.write_consumed);
    binderTestReadEmpty();
}

TEST_F(BinderDriverInterfaceTest, Transaction) {
    binder_uintptr_t cookie = 1234;
    struct {
        uint32_t cmd1;
        struct binder_transaction_data arg1;
    } __attribute__((packed)) bc1 = {
        .cmd1 = BC_TRANSACTION,
        .arg1 = {
            .target = { 0 },
            .cookie = 0,
            .code = android::IBinder::PING_TRANSACTION,
            .flags = 0,
            .sender_pid = 0,
            .sender_euid = 0,
            .data_size = 0,
            .offsets_size = 0,
            .data = {
                .ptr = {0, 0},
            },
        },
    };
    struct {
        uint32_t cmd0;
        uint32_t cmd1;
        uint32_t cmd2;
        binder_transaction_data arg2;
        uint32_t pad[16];
    } __attribute__((packed)) br;
    struct binder_write_read bwr = binder_write_read();

    bwr.write_buffer = (uintptr_t)&bc1;
    bwr.write_size = sizeof(bc1);
    bwr.read_buffer = (uintptr_t)&br;
    bwr.read_size = sizeof(br);

    {
        SCOPED_TRACE("1st WriteRead");
        binderTestIoctlSuccessOrError(BINDER_WRITE_READ, &bwr, EAGAIN);
    }
    EXPECT_EQ(sizeof(bc1), bwr.write_consumed);
    if (bwr.read_consumed < offsetof(typeof(br), pad)) {
        SCOPED_TRACE("2nd WriteRead");
        binderWaitForReadData(10000);
        binderTestIoctl(BINDER_WRITE_READ, &bwr);
    }
    EXPECT_EQ(offsetof(typeof(br), pad), bwr.read_consumed);
    if (bwr.read_consumed > offsetof(typeof(br), cmd0))
        EXPECT_EQ(BR_NOOP, br.cmd0);
    if (bwr.read_consumed > offsetof(typeof(br), cmd1))
        EXPECT_EQ(BR_TRANSACTION_COMPLETE, br.cmd1);
    if (bwr.read_consumed > offsetof(typeof(br), cmd2))
        EXPECT_EQ(BR_REPLY, br.cmd2);
    if (bwr.read_consumed >= offsetof(typeof(br), pad)) {
        EXPECT_EQ(0u, br.arg2.target.ptr);
        EXPECT_EQ(0u, br.arg2.cookie);
        EXPECT_EQ(0u, br.arg2.code);
        EXPECT_EQ(0u, br.arg2.flags);
        EXPECT_EQ(0u, br.arg2.data_size);
        EXPECT_EQ(0u, br.arg2.offsets_size);

        SCOPED_TRACE("3rd WriteRead");

        binderTestReadEmpty();

        struct {
            uint32_t cmd1;
            binder_uintptr_t arg1;
        } __attribute__((packed)) bc2 = {
            .cmd1 = BC_FREE_BUFFER,
            .arg1 = br.arg2.data.ptr.buffer,
        };

        bwr.write_buffer = (uintptr_t)&bc2;
        bwr.write_size = sizeof(bc2);
        bwr.write_consumed = 0;
        bwr.read_size = 0;

        binderTestIoctl(BINDER_WRITE_READ, &bwr);
        EXPECT_EQ(sizeof(bc2), bwr.write_consumed);
    }
    binderTestReadEmpty();
}

TEST_F(BinderDriverInterfaceTest, RequestDeathNotification) {
    binder_uintptr_t cookie = 1234;
    struct {
        uint32_t cmd0;
        uint32_t arg0;
        uint32_t cmd1;
        struct binder_handle_cookie arg1;
        uint32_t cmd2;
        struct binder_handle_cookie arg2;
        uint32_t cmd3;
        uint32_t arg3;
    } __attribute__((packed)) bc = {
        .cmd0 = BC_INCREFS,
        .arg0 = 0,
        .cmd1 = BC_REQUEST_DEATH_NOTIFICATION,
        .arg1 = {
            .handle = 0,
            .cookie = cookie,
        },
        .cmd2 = BC_CLEAR_DEATH_NOTIFICATION,
        .arg2 = {
            .handle = 0,
            .cookie = cookie,
        },
        .cmd3 = BC_DECREFS,
        .arg3 = 0,
    };
    struct {
        uint32_t cmd0;
        uint32_t cmd1;
        binder_uintptr_t arg1;
        uint32_t pad[16];
    } __attribute__((packed)) br;
    struct binder_write_read bwr = binder_write_read();

    bwr.write_buffer = (uintptr_t)&bc;
    bwr.write_size = sizeof(bc);
    bwr.read_buffer = (uintptr_t)&br;
    bwr.read_size = sizeof(br);

    binderTestIoctl(BINDER_WRITE_READ, &bwr);
    EXPECT_EQ(sizeof(bc), bwr.write_consumed);
    EXPECT_EQ(sizeof(br) - sizeof(br.pad), bwr.read_consumed);
    EXPECT_EQ(BR_NOOP, br.cmd0);
    EXPECT_EQ(BR_CLEAR_DEATH_NOTIFICATION_DONE, br.cmd1);
    EXPECT_EQ(cookie, br.arg1);
    binderTestReadEmpty();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    binder_env = AddGlobalTestEnvironment(new BinderDriverInterfaceTestEnv());

    return RUN_ALL_TESTS();
}
