/*
 * Copyright 2016 The Android Open Source Project
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
#define LOG_TAG "PosixAsyncIO_test.cpp"

#include <android-base/test_utils.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <string>
#include <unistd.h>
#include <log/log.h>

#include "PosixAsyncIO.h"

namespace android {

constexpr int TEST_PACKET_SIZE = 512;

static const std::string dummyDataStr =
    "/*\n * Copyright 2015 The Android Open Source Project\n *\n * Licensed un"
    "der the Apache License, Version 2.0 (the \"License\");\n * you may not us"
    "e this file except in compliance with the License.\n * You may obtain a c"
    "opy of the License at\n *\n *      http://www.apache.org/licenses/LICENSE"
    "-2.0\n *\n * Unless required by applicable law or agreed to in writing, s"
    "oftware\n * distributed under the License is distributed on an \"AS IS\" "
    "BASIS,\n * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express o"
    "r implied.\n * Se";


class PosixAsyncIOTest : public ::testing::Test {
protected:
    TemporaryFile dummy_file;

    PosixAsyncIOTest() {}
    ~PosixAsyncIOTest() {}
};

TEST_F(PosixAsyncIOTest, testRead) {
    char buf[TEST_PACKET_SIZE + 1];
    buf[TEST_PACKET_SIZE] = '\0';
    EXPECT_EQ(write(dummy_file.fd, dummyDataStr.c_str(), TEST_PACKET_SIZE), TEST_PACKET_SIZE);
    struct aiocb aio;
    struct aiocb *aiol[] = {&aio};
    aio.aio_fildes = dummy_file.fd;
    aio.aio_buf = buf;
    aio.aio_offset = 0;
    aio.aio_nbytes = TEST_PACKET_SIZE;

    EXPECT_EQ(aio_read(&aio), 0);
    EXPECT_EQ(aio_suspend(aiol, 1, nullptr), 0);
    EXPECT_EQ(aio_return(&aio), TEST_PACKET_SIZE);
    EXPECT_STREQ(buf, dummyDataStr.c_str());
}

TEST_F(PosixAsyncIOTest, testWrite) {
    char buf[TEST_PACKET_SIZE + 1];
    buf[TEST_PACKET_SIZE] = '\0';
    struct aiocb aio;
    struct aiocb *aiol[] = {&aio};
    aio.aio_fildes = dummy_file.fd;
    aio.aio_buf = const_cast<char*>(dummyDataStr.c_str());
    aio.aio_offset = 0;
    aio.aio_nbytes = TEST_PACKET_SIZE;

    EXPECT_EQ(aio_write(&aio), 0);
    EXPECT_EQ(aio_suspend(aiol, 1, nullptr), 0);
    EXPECT_EQ(aio_return(&aio), TEST_PACKET_SIZE);
    EXPECT_EQ(read(dummy_file.fd, buf, TEST_PACKET_SIZE), TEST_PACKET_SIZE);
    EXPECT_STREQ(buf, dummyDataStr.c_str());
}

TEST_F(PosixAsyncIOTest, testError) {
    char buf[TEST_PACKET_SIZE + 1];
    buf[TEST_PACKET_SIZE] = '\0';
    struct aiocb aio;
    struct aiocb *aiol[] = {&aio};
    aio.aio_fildes = -1;
    aio.aio_buf = const_cast<char*>(dummyDataStr.c_str());
    aio.aio_offset = 0;
    aio.aio_nbytes = TEST_PACKET_SIZE;

    EXPECT_EQ(aio_write(&aio), 0);
    EXPECT_EQ(aio_suspend(aiol, 1, nullptr), 0);
    EXPECT_EQ(aio_return(&aio), -1);
    EXPECT_EQ(aio_error(&aio), EBADF);
}

} // namespace android
