/*
* Copyright (C) 2016 The Android Open Source Project
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
#ifndef GTEST_IS_THREADSAFE
#error "GTest did not detect pthread library."
#endif

#include <android/hardware/tests/msgq/1.0/ITestMsgQ.h>
#include <fmq/EventFlag.h>
#include <fmq/MessageQueue.h>
#include <hidl/ServiceManagement.h>

// libutils:
using android::OK;
using android::sp;
using android::status_t;

// generated
using android::hardware::tests::msgq::V1_0::ITestMsgQ;

// libhidl
using android::hardware::kSynchronizedReadWrite;
using android::hardware::kUnsynchronizedWrite;
using android::hardware::MessageQueue;
using android::hardware::MQDescriptorSync;
using android::hardware::MQDescriptorUnsync;
using android::hardware::details::waitForHwService;

typedef MessageQueue<uint16_t, kSynchronizedReadWrite> MessageQueueSync;
typedef MessageQueue<uint16_t, kUnsynchronizedWrite> MessageQueueUnsync;

static sp<ITestMsgQ> waitGetTestService() {
    // waitForHwService is required because ITestMsgQ is not in manifest.xml.
    // "Real" HALs shouldn't be doing this.
    waitForHwService(ITestMsgQ::descriptor, "default");
    return ITestMsgQ::getService();
}

class UnsynchronizedWriteClientMultiProcess : public ::testing::Test {
   protected:
    void getQueue(MessageQueueUnsync** fmq, sp<ITestMsgQ>* service, bool setupQueue) {
        *service = waitGetTestService();
        *fmq = nullptr;
        if (*service == nullptr) return;
        if (!(*service)->isRemote()) return;
        (*service)->getFmqUnsyncWrite(setupQueue,
                                      [fmq](bool ret, const MQDescriptorUnsync<uint16_t>& in) {
                                          ASSERT_TRUE(ret);
                                          *fmq = new (std::nothrow) MessageQueueUnsync(in);
                                      });
    }
};

class SynchronizedReadWriteClient : public ::testing::Test {
protected:
    virtual void TearDown() {
        delete mQueue;
    }

    virtual void SetUp() {
        static constexpr size_t kNumElementsInQueue = 1024;
        mService = waitGetTestService();
        ASSERT_NE(mService, nullptr);
        ASSERT_TRUE(mService->isRemote());
        // create a queue on the client side
        mQueue = new (std::nothrow)
            MessageQueueSync(kNumElementsInQueue, true /* configure event flag word */);
        ASSERT_NE(nullptr, mQueue);
        ASSERT_TRUE(mQueue->isValid());
        mNumMessagesMax = mQueue->getQuantumCount();

        // tell server to set up the queue on its end
        ASSERT_TRUE(mService->configureFmqSyncReadWrite(*mQueue->getDesc()));
    }

    sp<ITestMsgQ> mService;
    MessageQueueSync* mQueue = nullptr;
    size_t mNumMessagesMax = 0;
};

class UnsynchronizedWriteClient : public ::testing::Test {
protected:
    virtual void TearDown() {
        delete mQueue;
    }

    virtual void SetUp() {
        mService = waitGetTestService();
        ASSERT_NE(mService, nullptr);
        ASSERT_TRUE(mService->isRemote());
        mService->getFmqUnsyncWrite(true /* configureFmq */,
                                    [this](bool ret, const MQDescriptorUnsync<uint16_t>& in) {
                                        ASSERT_TRUE(ret);
                                        mQueue = new (std::nothrow) MessageQueueUnsync(in);
                                    });
        ASSERT_NE(nullptr, mQueue);
        ASSERT_TRUE(mQueue->isValid());
        mNumMessagesMax = mQueue->getQuantumCount();
    }

    sp<ITestMsgQ> mService;
    MessageQueueUnsync*  mQueue = nullptr;
    size_t mNumMessagesMax = 0;
};

/*
 * Utility function to verify data read from the fast message queue.
 */
bool verifyData(uint16_t* data, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (data[i] != i) return false;
    }
    return true;
}

/*
 * Utility function to initialize data to be written to the FMQ
 */
inline void initData(uint16_t* data, size_t count) {
    for (size_t i = 0; i < count; i++) {
        data[i] = i;
    }
}

/*
 * Verify that for an unsynchronized flavor of FMQ, multiple readers
 * can recover from a write overflow condition.
 */
TEST_F(UnsynchronizedWriteClientMultiProcess, MultipleReadersAfterOverflow) {
    const size_t dataLen = 16;

    pid_t pid;
    /* creating first reader process */
    if ((pid = fork()) == 0) {
        sp<ITestMsgQ> testService;
        MessageQueueUnsync*  queue = nullptr;
        getQueue(&queue, &testService, true /* setupQueue */);
        ASSERT_NE(testService, nullptr);
        ASSERT_TRUE(testService->isRemote());
        ASSERT_NE(queue, nullptr);
        ASSERT_TRUE(queue->isValid());

        size_t numMessagesMax = queue->getQuantumCount();

        // The following two writes will cause a write overflow.
        auto ret = testService->requestWriteFmqUnsync(numMessagesMax);
        ASSERT_TRUE(ret.isOk());
        ASSERT_TRUE(ret);

        ret = testService->requestWriteFmqUnsync(1);
        ASSERT_TRUE(ret.isOk());
        ASSERT_TRUE(ret);

        // The following read should fail due to the overflow.
        std::vector<uint16_t> readData(numMessagesMax);
        ASSERT_FALSE(queue->read(&readData[0], numMessagesMax));

        /*
         * Request another write to verify that the reader can recover from the
         * overflow condition.
         */
        ASSERT_LT(dataLen, numMessagesMax);
        ret = testService->requestWriteFmqUnsync(dataLen);
        ASSERT_TRUE(ret.isOk());
        ASSERT_TRUE(ret);

        // Verify that the read is successful.
        ASSERT_TRUE(queue->read(&readData[0], dataLen));
        ASSERT_TRUE(verifyData(&readData[0], dataLen));

        delete queue;
        exit(0);
    }

    ASSERT_GT(pid, 0 /* parent should see PID greater than 0 for a good fork */);

    int status;
    // wait for the first reader process to exit.
    ASSERT_EQ(pid, waitpid(pid, &status, 0 /* options */));

    // creating second reader process.
    if ((pid = fork()) == 0) {
        sp<ITestMsgQ> testService;
        MessageQueueUnsync* queue = nullptr;

        getQueue(&queue, &testService, false /* setupQueue */);
        ASSERT_NE(testService, nullptr);
        ASSERT_TRUE(testService->isRemote());
        ASSERT_NE(queue, nullptr);
        ASSERT_TRUE(queue->isValid());

        // This read should fail due to the write overflow.
        std::vector<uint16_t> readData(dataLen);
        ASSERT_FALSE(queue->read(&readData[0], dataLen));

        /*
         * Request another write to verify that the process that recover from
         * the overflow condition.
         */
        auto ret = testService->requestWriteFmqUnsync(dataLen);
        ASSERT_TRUE(ret.isOk());
        ASSERT_TRUE(ret);

        // verify that the read is successful.
        ASSERT_TRUE(queue->read(&readData[0], dataLen));
        ASSERT_TRUE(verifyData(&readData[0], dataLen));

        delete queue;
        exit(0);
    }

    ASSERT_GT(pid, 0 /* parent should see PID greater than 0 for a good fork */);
    ASSERT_EQ(pid, waitpid(pid, &status, 0 /* options */));
}

/*
 * Test that basic blocking works using readBlocking()/writeBlocking() APIs
 * using the EventFlag object owned by FMQ.
 */
TEST_F(SynchronizedReadWriteClient, BlockingReadWrite1) {
    const size_t dataLen = 64;
    uint16_t data[dataLen] = {0};

    /*
     * Request service to perform a blocking read. This call is oneway and will
     * return immediately.
     */
    mService->requestBlockingRead(dataLen);
    bool ret = mQueue->writeBlocking(data,
                                     dataLen,
                                     static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                     static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                     5000000000 /* timeOutNanos */);
    ASSERT_TRUE(ret);
    ret = mQueue->writeBlocking(data, mNumMessagesMax,
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                5000000000 /* timeOutNanos */);
    ASSERT_TRUE(ret);
}

/*
 * Test that basic blocking works using readBlocking()/writeBlocking() APIs
 * using the EventFlag object owned by FMQ and using the default EventFlag
 * notification bit mask.
 */
TEST_F(SynchronizedReadWriteClient, BlockingReadWrite2) {
    const size_t dataLen = 64;
    std::vector<uint16_t> data(mNumMessagesMax);

    /*
     * Request service to perform a blocking read using default EventFlag
     * notification bit mask. This call is oneway and will
     * return immediately.
     */
    mService->requestBlockingReadDefaultEventFlagBits(dataLen);

    /* Cause a context switch to allow service to block */
    sched_yield();

    bool ret = mQueue->writeBlocking(&data[0],
                                     dataLen);
    ASSERT_TRUE(ret);

    /*
     * If the blocking read was successful, another write of size
     * mNumMessagesMax will succeed.
     */
    ret = mQueue->writeBlocking(&data[0], mNumMessagesMax, 5000000000 /* timeOutNanos */);
    ASSERT_TRUE(ret);
}

/*
 * Test that repeated blocking reads and writes work using readBlocking()/writeBlocking() APIs
 * using the EventFlag object owned by FMQ.
 * Each write operation writes the same amount of data as a single read
 * operation.
 */
TEST_F(SynchronizedReadWriteClient, BlockingReadWriteRepeat1) {
    const size_t dataLen = 64;
    uint16_t data[dataLen] = {0};
    bool ret = false;

    /*
     * Request service to perform a blocking read. This call is oneway and will
     * return immediately.
     */
    const size_t writeCount = 1024;
    mService->requestBlockingReadRepeat(dataLen, writeCount);

    for (size_t i = 0; i < writeCount; i++) {
        ret = mQueue->writeBlocking(data, dataLen,
                                    static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                    static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                    5000000000 /* timeOutNanos */);
        ASSERT_TRUE(ret);
    }

    ret = mQueue->writeBlocking(data, mNumMessagesMax,
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                5000000000 /* timeOutNanos */);

    ASSERT_TRUE(ret);
}

/*
 * Test that repeated blocking reads and writes work using readBlocking()/writeBlocking() APIs
 * using the EventFlag object owned by FMQ. Each read operation reads twice the
 * amount of data as a single write.
 *
 */
TEST_F(SynchronizedReadWriteClient, BlockingReadWriteRepeat2) {
    const size_t dataLen = 64;
    uint16_t data[dataLen] = {0};
    bool ret = false;

    /*
     * Request service to perform a blocking read. This call is oneway and will
     * return immediately.
     */
    const size_t writeCount = 1024;
    mService->requestBlockingReadRepeat(dataLen*2, writeCount/2);

    for (size_t i = 0; i < writeCount; i++) {
        ret = mQueue->writeBlocking(data, dataLen,
                                    static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                    static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                    5000000000 /* timeOutNanos */);
        ASSERT_TRUE(ret);
    }

    ret = mQueue->writeBlocking(data, mNumMessagesMax,
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                5000000000 /* timeOutNanos */);
    ASSERT_TRUE(ret);
}

/*
 * Test that basic blocking works using readBlocking()/writeBlocking() APIs
 * using the EventFlag object owned by FMQ. Each write operation writes twice
 * the amount of data as a single read.
 */
TEST_F(SynchronizedReadWriteClient, BlockingReadWriteRepeat3) {
    const size_t dataLen = 64;
    uint16_t data[dataLen] = {0};
    bool ret = false;

    /*
     * Request service to perform a blocking read. This call is oneway and will
     * return immediately.
     */
    size_t writeCount = 1024;
    mService->requestBlockingReadRepeat(dataLen/2, writeCount*2);

    for (size_t i = 0; i < writeCount; i++) {
        ret = mQueue->writeBlocking(data, dataLen,
                                    static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                    static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                    5000000000 /* timeOutNanos */);
        ASSERT_TRUE(ret);
    }
    ret = mQueue->writeBlocking(data, mNumMessagesMax,
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                                static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY),
                                5000000000 /* timeOutNanos */);
    ASSERT_TRUE(ret);
}

/*
 * Test that writeBlocking()/readBlocking() APIs do not block on
 * attempts to write/read 0 messages and return true.
 */
TEST_F(SynchronizedReadWriteClient, BlockingReadWriteZeroMessages) {
    uint16_t data = 0;

    /*
     * Trigger a blocking write for zero messages with no timeout.
     */
    bool ret = mQueue->writeBlocking(
            &data,
            0,
            static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
            static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY));
    ASSERT_TRUE(ret);

    /*
     * Trigger a blocking read for zero messages with no timeout.
     */
    ret = mQueue->readBlocking(&data,
                               0,
                               static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_FULL),
                               static_cast<uint32_t>(ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY));
    ASSERT_TRUE(ret);
}

/*
 * Request mService to write a small number of messages
 * to the FMQ. Read and verify data.
 */
TEST_F(SynchronizedReadWriteClient, SmallInputReaderTest1) {
    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);
    bool ret = mService->requestWriteFmqSync(dataLen);
    ASSERT_TRUE(ret);
    uint16_t readData[dataLen] = {};
    ASSERT_TRUE(mQueue->read(readData, dataLen));
    ASSERT_TRUE(verifyData(readData, dataLen));
}

/*
 * Request mService to write a small number of messages
 * to the FMQ. Read and verify each message using
 * beginRead/Commit read APIs.
 */
TEST_F(SynchronizedReadWriteClient, SmallInputReaderTest2) {
    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);
    auto ret = mService->requestWriteFmqSync(dataLen);

    ASSERT_TRUE(ret.isOk());
    ASSERT_TRUE(ret);

    MessageQueueSync::MemTransaction tx;
    ASSERT_TRUE(mQueue->beginRead(dataLen, &tx));

    auto first = tx.getFirstRegion();
    auto second = tx.getSecondRegion();
    size_t firstRegionLength = first.getLength();

    for (size_t i = 0; i < dataLen; i++) {
        if (i < firstRegionLength) {
            ASSERT_EQ(i, *(first.getAddress() + i));
        } else {
            ASSERT_EQ(i, *(second.getAddress() + i - firstRegionLength));
        }
    }

    ASSERT_TRUE(mQueue->commitRead(dataLen));
}

/*
 * Write a small number of messages to FMQ. Request
 * mService to read and verify that the write was succesful.
 */
TEST_F(SynchronizedReadWriteClient, SmallInputWriterTest1) {
    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);
    size_t originalCount = mQueue->availableToWrite();
    uint16_t data[dataLen];
    initData(data, dataLen);
    ASSERT_TRUE(mQueue->write(data, dataLen));
    bool ret = mService->requestReadFmqSync(dataLen);
    ASSERT_TRUE(ret);
    size_t availableCount = mQueue->availableToWrite();
    ASSERT_EQ(originalCount, availableCount);
}

/*
 * Write a small number of messages to FMQ using the beginWrite()/CommitWrite()
 * APIs. Request mService to read and verify that the write was succesful.
 */
TEST_F(SynchronizedReadWriteClient, SmallInputWriterTest2) {
    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);
    size_t originalCount = mQueue->availableToWrite();
    uint16_t data[dataLen];
    initData(data, dataLen);

    MessageQueueSync::MemTransaction tx;
    ASSERT_TRUE(mQueue->beginWrite(dataLen, &tx));

    auto first = tx.getFirstRegion();
    auto second = tx.getSecondRegion();

    size_t firstRegionLength = first.getLength();
    uint16_t* firstBaseAddress = first.getAddress();
    uint16_t* secondBaseAddress = second.getAddress();

    for (size_t i = 0; i < dataLen; i++) {
        if (i < firstRegionLength) {
            *(firstBaseAddress + i) = i;
        } else {
            *(secondBaseAddress + i - firstRegionLength) = i;
        }
    }

    ASSERT_TRUE(mQueue->commitWrite(dataLen));

    auto ret = mService->requestReadFmqSync(dataLen);
    ASSERT_TRUE(ret.isOk());
    ASSERT_TRUE(ret);
    size_t availableCount = mQueue->availableToWrite();
    ASSERT_EQ(originalCount, availableCount);
}

/*
 * Verify that the FMQ is empty and read fails when it is empty.
 */
TEST_F(SynchronizedReadWriteClient, ReadWhenEmpty) {
    ASSERT_EQ(0UL, mQueue->availableToRead());
    const size_t numMessages = 2;
    ASSERT_LE(numMessages, mNumMessagesMax);
    uint16_t readData[numMessages];
    ASSERT_FALSE(mQueue->read(readData, numMessages));
}

/*
 * Verify FMQ is empty.
 * Write enough messages to fill it.
 * Verify availableToWrite() method returns is zero.
 * Try writing another message and verify that
 * the attempted write was unsuccesful. Request mService
 * to read and verify the messages in the FMQ.
 */

TEST_F(SynchronizedReadWriteClient, WriteWhenFull) {
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));
    ASSERT_EQ(0UL, mQueue->availableToWrite());
    ASSERT_FALSE(mQueue->write(&data[0], 1));
    bool ret = mService->requestReadFmqSync(mNumMessagesMax);
    ASSERT_TRUE(ret);
}

/*
 * Verify FMQ is empty.
 * Request mService to write data equal to queue size.
 * Read and verify data in mQueue.
 */
TEST_F(SynchronizedReadWriteClient, LargeInputTest1) {
    bool ret = mService->requestWriteFmqSync(mNumMessagesMax);
    ASSERT_TRUE(ret);
    std::vector<uint16_t> readData(mNumMessagesMax);
    ASSERT_TRUE(mQueue->read(&readData[0], mNumMessagesMax));
    ASSERT_TRUE(verifyData(&readData[0], mNumMessagesMax));
}

/*
 * Request mService to write more than maximum number of messages to the FMQ.
 * Verify that the write fails. Verify that availableToRead() method
 * still returns 0 and verify that attempt to read fails.
 */
TEST_F(SynchronizedReadWriteClient, LargeInputTest2) {
    ASSERT_EQ(0UL, mQueue->availableToRead());
    const size_t numMessages = 2048;
    ASSERT_GT(numMessages, mNumMessagesMax);
    bool ret = mService->requestWriteFmqSync(numMessages);
    ASSERT_FALSE(ret);
    uint16_t readData;
    ASSERT_EQ(0UL, mQueue->availableToRead());
    ASSERT_FALSE(mQueue->read(&readData, 1));
}

/*
 * Write until FMQ is full.
 * Verify that the number of messages available to write
 * is equal to mNumMessagesMax.
 * Verify that another write attempt fails.
 * Request mService to read. Verify read count.
 */

TEST_F(SynchronizedReadWriteClient, LargeInputTest3) {
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));
    ASSERT_EQ(0UL, mQueue->availableToWrite());
    ASSERT_FALSE(mQueue->write(&data[0], 1));

    bool ret = mService->requestReadFmqSync(mNumMessagesMax);
    ASSERT_TRUE(ret);
}

/*
 * Confirm that the FMQ is empty. Request mService to write to FMQ.
 * Do multiple reads to empty FMQ and verify data.
 */
TEST_F(SynchronizedReadWriteClient, MultipleRead) {
    const size_t chunkSize = 100;
    const size_t chunkNum = 5;
    const size_t numMessages = chunkSize * chunkNum;
    ASSERT_LE(numMessages, mNumMessagesMax);
    size_t availableToRead = mQueue->availableToRead();
    size_t expectedCount = 0;
    ASSERT_EQ(expectedCount, availableToRead);
    bool ret = mService->requestWriteFmqSync(numMessages);
    ASSERT_TRUE(ret);
    uint16_t readData[numMessages] = {};
    for (size_t i = 0; i < chunkNum; i++) {
        ASSERT_TRUE(mQueue->read(readData + i * chunkSize, chunkSize));
    }
    ASSERT_TRUE(verifyData(readData, numMessages));
}

/*
 * Write to FMQ in bursts.
 * Request mService to read data. Verify the read was successful.
 */
TEST_F(SynchronizedReadWriteClient, MultipleWrite) {
    const size_t chunkSize = 100;
    const size_t chunkNum = 5;
    const size_t numMessages = chunkSize * chunkNum;
    ASSERT_LE(numMessages, mNumMessagesMax);
    uint16_t data[numMessages];
    initData(&data[0], numMessages);

    for (size_t i = 0; i < chunkNum; i++) {
        ASSERT_TRUE(mQueue->write(data + i * chunkSize, chunkSize));
    }
    bool ret = mService->requestReadFmqSync(numMessages);
    ASSERT_TRUE(ret);
}

/*
 * Write enough messages into the FMQ to fill half of it.
 * Request mService to read back the same.
 * Write mNumMessagesMax messages into the queue. This should cause a
 * wrap around. Request mService to read and verify the data.
 */
TEST_F(SynchronizedReadWriteClient, ReadWriteWrapAround) {
    size_t numMessages = mNumMessagesMax / 2;
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], numMessages));
    bool ret = mService->requestReadFmqSync(numMessages);
    ASSERT_TRUE(ret);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));
    ret = mService->requestReadFmqSync(mNumMessagesMax);
    ASSERT_TRUE(ret);
}

/*
 * Use beginWrite/commitWrite/getSlot APIs to test wrap arounds are handled
 * correctly.
 * Write enough messages into the FMQ to fill half of it
 * and read back the same.
 * Write mNumMessagesMax messages into the queue. This will cause a
 * wrap around. Read and verify the data.
 */
TEST_F(SynchronizedReadWriteClient, ReadWriteWrapAround2) {
    size_t numMessages = mNumMessagesMax / 2;
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], numMessages));
    auto ret = mService->requestReadFmqSync(numMessages);

    ASSERT_TRUE(ret.isOk());
    ASSERT_TRUE(ret);

    /*
     * The next write and read will have to deal with with wrap arounds.
     */
    MessageQueueSync::MemTransaction tx;
    ASSERT_TRUE(mQueue->beginWrite(mNumMessagesMax, &tx));

    ASSERT_EQ(tx.getFirstRegion().getLength() + tx.getSecondRegion().getLength(),  mNumMessagesMax);

    for (size_t i = 0; i < mNumMessagesMax; i++) {
        uint16_t* ptr = tx.getSlot(i);
        *ptr = data[i];
    }

    ASSERT_TRUE(mQueue->commitWrite(mNumMessagesMax));

    ret = mService->requestReadFmqSync(mNumMessagesMax);

    ASSERT_TRUE(ret.isOk());
    ASSERT_TRUE(ret);
}

/*
 * Request mService to write a small number of messages
 * to the FMQ. Read and verify data.
 */
TEST_F(UnsynchronizedWriteClient, SmallInputReaderTest1) {
    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);
    bool ret = mService->requestWriteFmqUnsync(dataLen);
    ASSERT_TRUE(ret);
    uint16_t readData[dataLen] = {};
    ASSERT_TRUE(mQueue->read(readData, dataLen));
    ASSERT_TRUE(verifyData(readData, dataLen));
}

/*
 * Write a small number of messages to FMQ. Request
 * mService to read and verify that the write was succesful.
 */
TEST_F(UnsynchronizedWriteClient, SmallInputWriterTest1) {
    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);
    uint16_t data[dataLen];
    initData(data, dataLen);
    ASSERT_TRUE(mQueue->write(data, dataLen));
    bool ret = mService->requestReadFmqUnsync(dataLen);
    ASSERT_TRUE(ret);
}

/*
 * Verify that the FMQ is empty and read fails when it is empty.
 */
TEST_F(UnsynchronizedWriteClient, ReadWhenEmpty) {
    ASSERT_EQ(0UL, mQueue->availableToRead());
    const size_t numMessages = 2;
    ASSERT_LE(numMessages, mNumMessagesMax);
    uint16_t readData[numMessages];
    ASSERT_FALSE(mQueue->read(readData, numMessages));
}

/*
 * Verify FMQ is empty.
 * Write enough messages to fill it.
 * Verify availableToWrite() method returns is zero.
 * Try writing another message and verify that
 * the attempted write was successful. Request mService
 * to read the messages in the FMQ and verify that it is unsuccesful.
 */

TEST_F(UnsynchronizedWriteClient, WriteWhenFull) {
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));
    ASSERT_EQ(0UL, mQueue->availableToWrite());
    ASSERT_TRUE(mQueue->write(&data[0], 1));
    bool ret = mService->requestReadFmqUnsync(mNumMessagesMax);
    ASSERT_FALSE(ret);
}

/*
 * Verify FMQ is empty.
 * Request mService to write data equal to queue size.
 * Read and verify data in mQueue.
 */
TEST_F(UnsynchronizedWriteClient, LargeInputTest1) {
    bool ret = mService->requestWriteFmqUnsync(mNumMessagesMax);
    ASSERT_TRUE(ret);
    std::vector<uint16_t> data(mNumMessagesMax);
    ASSERT_TRUE(mQueue->read(&data[0], mNumMessagesMax));
    ASSERT_TRUE(verifyData(&data[0], mNumMessagesMax));
}

/*
 * Request mService to write more than maximum number of messages to the FMQ.
 * Verify that the write fails. Verify that availableToRead() method
 * still returns 0 and verify that attempt to read fails.
 */
TEST_F(UnsynchronizedWriteClient, LargeInputTest2) {
    ASSERT_EQ(0UL, mQueue->availableToRead());
    const size_t numMessages = mNumMessagesMax + 1;
    bool ret = mService->requestWriteFmqUnsync(numMessages);
    ASSERT_FALSE(ret);
    uint16_t readData;
    ASSERT_EQ(0UL, mQueue->availableToRead());
    ASSERT_FALSE(mQueue->read(&readData, 1));
}

/*
 * Write until FMQ is full.
 * Verify that the number of messages available to write
 * is equal to mNumMessagesMax.
 * Verify that another write attempt is succesful.
 * Request mService to read. Verify that read is unsuccessful.
 * Perform another write and verify that the read is succesful
 * to check if the reader process can recover from the error condition.
 */
TEST_F(UnsynchronizedWriteClient, LargeInputTest3) {
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));
    ASSERT_EQ(0UL, mQueue->availableToWrite());
    ASSERT_TRUE(mQueue->write(&data[0], 1));

    bool ret = mService->requestReadFmqUnsync(mNumMessagesMax);
    ASSERT_FALSE(ret);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));

    ret = mService->requestReadFmqUnsync(mNumMessagesMax);
    ASSERT_TRUE(ret);
}

/*
 * Confirm that the FMQ is empty. Request mService to write to FMQ.
 * Do multiple reads to empty FMQ and verify data.
 */
TEST_F(UnsynchronizedWriteClient, MultipleRead) {
    const size_t chunkSize = 100;
    const size_t chunkNum = 5;
    const size_t numMessages = chunkSize * chunkNum;
    ASSERT_LE(numMessages, mNumMessagesMax);
    size_t availableToRead = mQueue->availableToRead();
    size_t expectedCount = 0;
    ASSERT_EQ(expectedCount, availableToRead);
    bool ret = mService->requestWriteFmqUnsync(numMessages);
    ASSERT_TRUE(ret);
    uint16_t readData[numMessages] = {};
    for (size_t i = 0; i < chunkNum; i++) {
        ASSERT_TRUE(mQueue->read(readData + i * chunkSize, chunkSize));
    }
    ASSERT_TRUE(verifyData(readData, numMessages));
}

/*
 * Write to FMQ in bursts.
 * Request mService to read data, verify that it was successful.
 */
TEST_F(UnsynchronizedWriteClient, MultipleWrite) {
    const size_t chunkSize = 100;
    const size_t chunkNum = 5;
    const size_t numMessages = chunkSize * chunkNum;
    ASSERT_LE(numMessages, mNumMessagesMax);
    uint16_t data[numMessages];
    initData(data, numMessages);
    for (size_t i = 0; i < chunkNum; i++) {
        ASSERT_TRUE(mQueue->write(data + i * chunkSize, chunkSize));
    }
    bool ret = mService->requestReadFmqUnsync(numMessages);
    ASSERT_TRUE(ret);
}

/*
 * Write enough messages into the FMQ to fill half of it.
 * Request mService to read back the same.
 * Write mNumMessagesMax messages into the queue. This should cause a
 * wrap around. Request mService to read and verify the data.
 */
TEST_F(UnsynchronizedWriteClient, ReadWriteWrapAround) {
    size_t numMessages = mNumMessagesMax / 2;
    std::vector<uint16_t> data(mNumMessagesMax);
    initData(&data[0], mNumMessagesMax);
    ASSERT_TRUE(mQueue->write(&data[0], numMessages));
    bool ret = mService->requestReadFmqUnsync(numMessages);
    ASSERT_TRUE(ret);
    ASSERT_TRUE(mQueue->write(&data[0], mNumMessagesMax));
    ret = mService->requestReadFmqUnsync(mNumMessagesMax);
    ASSERT_TRUE(ret);
}

/*
 * Request mService to write a small number of messages
 * to the FMQ. Read and verify data from two threads configured
 * as readers to the FMQ.
 */
TEST_F(UnsynchronizedWriteClient, SmallInputMultipleReaderTest) {
    auto desc = mQueue->getDesc();
    std::unique_ptr<MessageQueue<uint16_t, kUnsynchronizedWrite>> mQueue2(
            new (std::nothrow) MessageQueue<uint16_t, kUnsynchronizedWrite>(*desc));
    ASSERT_NE(nullptr, mQueue2.get());

    const size_t dataLen = 16;
    ASSERT_LE(dataLen, mNumMessagesMax);

    bool ret = mService->requestWriteFmqUnsync(dataLen);
    ASSERT_TRUE(ret);

    pid_t pid;
    if ((pid = fork()) == 0) {
        /* child process */
        uint16_t readData[dataLen] = {};
        ASSERT_TRUE(mQueue2->read(readData, dataLen));
        ASSERT_TRUE(verifyData(readData, dataLen));
        exit(0);
    } else {
        ASSERT_GT(pid,
                  0 /* parent should see PID greater than 0 for a good fork */);
        uint16_t readData[dataLen] = {};
        ASSERT_TRUE(mQueue->read(readData, dataLen));
        ASSERT_TRUE(verifyData(readData, dataLen));
    }
}

/*
 * Request mService to write into the FMQ until it is full.
 * Request mService to do another write and verify it is successful.
 * Use two reader processes to read and verify that both fail.
 */
TEST_F(UnsynchronizedWriteClient, OverflowNotificationTest) {
    auto desc = mQueue->getDesc();
    std::unique_ptr<MessageQueue<uint16_t, kUnsynchronizedWrite>> mQueue2(
            new (std::nothrow) MessageQueue<uint16_t, kUnsynchronizedWrite>(*desc));
    ASSERT_NE(nullptr, mQueue2.get());

    bool ret = mService->requestWriteFmqUnsync(mNumMessagesMax);
    ASSERT_TRUE(ret);
    ret = mService->requestWriteFmqUnsync(1);
    ASSERT_TRUE(ret);

    pid_t pid;
    if ((pid = fork()) == 0) {
        /* child process */
        std::vector<uint16_t> readData(mNumMessagesMax);
        ASSERT_FALSE(mQueue2->read(&readData[0], mNumMessagesMax));
        exit(0);
    } else {
        ASSERT_GT(pid, 0/* parent should see PID greater than 0 for a good fork */);
        std::vector<uint16_t> readData(mNumMessagesMax);
        ASSERT_FALSE(mQueue->read(&readData[0], mNumMessagesMax));
    }
}
