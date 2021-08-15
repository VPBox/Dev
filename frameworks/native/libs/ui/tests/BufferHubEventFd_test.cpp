/*
 * Copyright 2018 The Android Open Source Project
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

#define LOG_TAG "BufferHubEventFdTest"

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <array>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <ui/BufferHubEventFd.h>

namespace android {

namespace {

const int kTimeout = 100;
const std::chrono::milliseconds kTimeoutMs(kTimeout);
const int kTestRuns = 5;

using ::testing::Contains;
using BufferHubEventFdTest = ::testing::Test;

} // namespace

TEST_F(BufferHubEventFdTest, EventFd_testSingleEpollFd) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());

    base::unique_fd epollFd(epoll_create(64));
    ASSERT_GE(epollFd.get(), 0);

    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    std::array<epoll_event, 1> events;
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    eventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    // Check that it can receive consecutive signal.
    eventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    // Check that it can receive consecutive signal from a duplicated eventfd.
    BufferHubEventFd dupEventFd(dup(eventFd.get()));
    ASSERT_TRUE(dupEventFd.isValid());
    dupEventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
    dupEventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubEventFdTest, EventFd_testCreateEpollFdAndAddSignaledEventFd) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());
    eventFd.signal();

    base::unique_fd epollFd(epoll_create(64));
    ASSERT_GE(epollFd.get(), 0);

    // Make sure that the epoll set has not been signal yet.
    std::array<epoll_event, 1> events;
    ASSERT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    // Check that adding an signaled fd into this epoll set will trigger the epoll set.
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubEventFdTest, EventFd_testAddSignaledEventFdToEpollFd) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());

    base::unique_fd epollFd(epoll_create(64));
    ASSERT_GE(epollFd.get(), 0);

    eventFd.signal();

    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    std::array<epoll_event, 1> events;
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubEventFdTest, EventFd_testConsecutiveSignalsFromAEventFd) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());
    base::unique_fd epollFd(epoll_create(64));
    ASSERT_GE(epollFd.get(), 0);
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    std::array<epoll_event, 1> events;
    for (int i = 0; i < kTestRuns; ++i) {
        eventFd.signal();
        EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
        EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
    }
}

TEST_F(BufferHubEventFdTest, EventFd_testConsecutiveSignalsFromADuplicatedEventFd) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());
    base::unique_fd epollFd(epoll_create(64));
    ASSERT_GE(epollFd.get(), 0);
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    BufferHubEventFd dupEventFd(dup(eventFd.get()));
    ASSERT_TRUE(dupEventFd.isValid());

    std::array<epoll_event, 1> events;
    for (int i = 0; i < kTestRuns; ++i) {
        dupEventFd.signal();
        EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
        EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
    }
}

TEST_F(BufferHubEventFdTest, EventFd_testClear) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());

    base::unique_fd epollFd(epoll_create(64));
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};

    ASSERT_GE(epollFd.get(), 0);
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    eventFd.signal();
    eventFd.clear();

    std::array<epoll_event, 1> events;
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubEventFdTest, EventFd_testDupEventFd) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());

    base::unique_fd epollFd(epoll_create(64));
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};

    ASSERT_GE(epollFd.get(), 0);
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    // Technically, the dupliated eventFd and the original eventFd are pointing
    // to the same kernel object. This test signals the duplicated eventFd but epolls the origianl
    // eventFd.
    BufferHubEventFd dupedEventFd(dup(eventFd.get()));
    ASSERT_GE(dupedEventFd.get(), 0);

    std::array<epoll_event, 1> events;
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    dupedEventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    dupedEventFd.signal();

    dupedEventFd.clear();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubEventFdTest, EventFd_testTwoEpollFds) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());

    base::unique_fd epollFd1(epoll_create(64));
    base::unique_fd epollFd2(epoll_create(64));
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};

    ASSERT_GE(epollFd1.get(), 0);
    ASSERT_GE(epollFd2.get(), 0);

    // Register the same eventFd to two EpollFds.
    ASSERT_EQ(epoll_ctl(epollFd1.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);
    ASSERT_EQ(epoll_ctl(epollFd2.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    std::array<epoll_event, 1> events;
    EXPECT_EQ(epoll_wait(epollFd1.get(), events.data(), events.size(), 0), 0);
    EXPECT_EQ(epoll_wait(epollFd2.get(), events.data(), events.size(), 0), 0);

    eventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd1.get(), events.data(), events.size(), 0), 1);
    EXPECT_EQ(epoll_wait(epollFd2.get(), events.data(), events.size(), 0), 1);

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd1.get(), events.data(), events.size(), 0), 0);
    EXPECT_EQ(epoll_wait(epollFd2.get(), events.data(), events.size(), 0), 0);

    eventFd.signal();
    EXPECT_EQ(epoll_wait(epollFd1.get(), events.data(), events.size(), 0), 1);

    eventFd.clear();
    EXPECT_EQ(epoll_wait(epollFd1.get(), events.data(), events.size(), 0), 0);
    EXPECT_EQ(epoll_wait(epollFd2.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubEventFdTest, EventFd_testTwoEventFds) {
    BufferHubEventFd eventFd1;
    BufferHubEventFd eventFd2;

    ASSERT_TRUE(eventFd1.isValid());
    ASSERT_TRUE(eventFd2.isValid());

    base::unique_fd epollFd(epoll_create(64));
    epoll_event e1 = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 1}};
    epoll_event e2 = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 2}};

    ASSERT_GE(epollFd.get(), 0);
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd1.get(), &e1), 0);
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd2.get(), &e2), 0);

    std::array<epoll_event, 2> events;
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    // Signal one by one.
    eventFd1.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
    EXPECT_EQ(events[0].data.u32, e1.data.u32);

    eventFd2.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
    EXPECT_EQ(events[0].data.u32, e2.data.u32);

    // Signal both.
    eventFd1.signal();
    eventFd2.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 2);

    uint32_t u32s[] = {events[0].data.u32, events[1].data.u32};
    EXPECT_THAT(u32s, Contains(e1.data.u32));
    EXPECT_THAT(u32s, Contains(e2.data.u32));

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    eventFd1.signal();
    eventFd2.signal();
    eventFd2.clear();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);
}

TEST_F(BufferHubEventFdTest, EventFd_testPollingThreadWithTwoEventFds) {
    BufferHubEventFd eventFd1;
    BufferHubEventFd eventFd2;

    ASSERT_TRUE(eventFd1.isValid());
    ASSERT_TRUE(eventFd2.isValid());

    base::unique_fd epollFd(epoll_create(64));
    epoll_event e1 = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 1}};
    epoll_event e2 = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 2}};

    ASSERT_GE(epollFd.get(), 0);
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd1.get(), &e1), 0);
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd2.get(), &e2), 0);

    int countEvent1 = 0;
    int countEvent2 = 0;
    std::atomic<bool> stop{false};
    std::mutex mx;
    std::condition_variable cv;

    std::thread pollingThread([&] {
        std::array<epoll_event, 2> events;
        while (true) {
            if (stop.load()) {
                break;
            }
            int ret = epoll_wait(epollFd.get(), events.data(), events.size(), kTimeout);
            ALOGE_IF(ret < 0 && errno != ETIMEDOUT, "Epoll failed.");

            std::lock_guard<std::mutex> lock(mx);
            for (int i = 0; i < ret; i++) {
                if (events[i].data.u32 == e1.data.u32) {
                    countEvent1++;
                    cv.notify_one();
                } else if (events[i].data.u32 == e2.data.u32) {
                    countEvent2++;
                    cv.notify_one();
                }
            }
        }
    });

    {
        std::unique_lock<std::mutex> lock(mx);

        eventFd1.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEvent1 == 1; }));

        eventFd1.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEvent1 == 2; }));

        eventFd2.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEvent2 == 1; }));

        eventFd1.clear();
        eventFd2.clear();
        EXPECT_EQ(countEvent1, 2);
        EXPECT_EQ(countEvent2, 1);

        eventFd1.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEvent1 == 3; }));

        eventFd2.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEvent2 == 2; }));
    }

    stop.store(true);
    pollingThread.join();
}

TEST_F(BufferHubEventFdTest, EventFd_testTwoPollingThreads) {
    BufferHubEventFd eventFd;
    ASSERT_TRUE(eventFd.isValid());

    base::unique_fd epollFd1(epoll_create(64));
    base::unique_fd epollFd2(epoll_create(64));
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};

    ASSERT_GE(epollFd1.get(), 0);
    ASSERT_GE(epollFd2.get(), 0);

    // Register the same eventFd to two EpollFds.
    ASSERT_EQ(epoll_ctl(epollFd1.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);
    ASSERT_EQ(epoll_ctl(epollFd2.get(), EPOLL_CTL_ADD, eventFd.get(), &e), 0);

    int countEpoll1 = 0;
    int countEpoll2 = 0;
    std::atomic<bool> stop{false};
    std::mutex mx;
    std::condition_variable cv;

    std::thread pollingThread1([&] {
        std::array<epoll_event, 1> events;
        while (!stop.load()) {
            int ret = epoll_wait(epollFd1.get(), events.data(), events.size(), kTimeout);
            ALOGE_IF(ret < 0 && errno != ETIMEDOUT, "Epoll failed.");

            if (ret > 0) {
                std::lock_guard<std::mutex> lock(mx);
                countEpoll1++;
                cv.notify_one();
            }
        }
    });

    std::thread pollingThread2([&] {
        std::array<epoll_event, 1> events;
        while (!stop.load()) {
            int ret = epoll_wait(epollFd2.get(), events.data(), events.size(), kTimeout);
            ALOGE_IF(ret < 0 && errno != ETIMEDOUT, "Epoll failed.");

            if (ret > 0) {
                std::lock_guard<std::mutex> lock(mx);
                countEpoll2++;
                cv.notify_one();
            }
        }
    });

    {
        std::unique_lock<std::mutex> lock(mx);

        eventFd.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEpoll1 == 1; }));
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEpoll2 == 1; }));

        eventFd.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEpoll1 == 2; }));
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEpoll2 == 2; }));

        eventFd.clear();
        EXPECT_EQ(countEpoll1, 2);
        EXPECT_EQ(countEpoll2, 2);

        eventFd.signal();
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEpoll1 == 3; }));
        EXPECT_TRUE(cv.wait_for(lock, kTimeoutMs, [&] { return countEpoll2 == 3; }));
    }

    stop.store(true);
    pollingThread1.join();
    pollingThread2.join();
}

} // namespace android
