/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "../BlockingQueue.h"


#include <gtest/gtest.h>
#include <thread>

namespace android {


// --- BlockingQueueTest ---

/**
 * Sanity check of basic pop and push operation.
 */
TEST(BlockingQueueTest, Queue_AddAndRemove) {
    constexpr size_t capacity = 10;
    BlockingQueue<int> queue(capacity);

    ASSERT_TRUE(queue.push(1));
    ASSERT_EQ(queue.pop(), 1);
}

/**
 * Make sure the queue has strict capacity limits.
 */
TEST(BlockingQueueTest, Queue_ReachesCapacity) {
    constexpr size_t capacity = 3;
    BlockingQueue<int> queue(capacity);

    // First 3 elements should be added successfully
    ASSERT_TRUE(queue.push(1));
    ASSERT_TRUE(queue.push(2));
    ASSERT_TRUE(queue.push(3));
    ASSERT_FALSE(queue.push(4)) << "Queue should reach capacity at size " << capacity;
}

/**
 * Make sure the queue maintains FIFO order.
 * Add elements and remove them, and check the order.
 */
TEST(BlockingQueueTest, Queue_isFIFO) {
    constexpr size_t capacity = 10;
    BlockingQueue<int> queue(capacity);

    for (size_t i = 0; i < capacity; i++) {
        ASSERT_TRUE(queue.push(static_cast<int>(i)));
    }
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(queue.pop(), static_cast<int>(i));
    }
}

TEST(BlockingQueueTest, Queue_Clears) {
    constexpr size_t capacity = 2;
    BlockingQueue<int> queue(capacity);

    queue.push(1);
    queue.push(2);
    queue.clear();
    queue.push(3);
    // Should no longer receive elements 1 and 2
    ASSERT_EQ(3, queue.pop());
}

TEST(BlockingQueueTest, Queue_Erases) {
    constexpr size_t capacity = 4;
    BlockingQueue<int> queue(capacity);

    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);
    // Erase elements 2 and 4
    queue.erase([](int element) { return element == 2 || element == 4; });
    // Should no longer receive elements 2 and 4
    ASSERT_EQ(1, queue.pop());
    ASSERT_EQ(3, queue.pop());
}

// --- BlockingQueueTest - Multiple threads ---

TEST(BlockingQueueTest, Queue_AllowsMultipleThreads) {
    constexpr size_t capacity = 100; // large capacity to increase likelihood that threads overlap
    BlockingQueue<int> queue(capacity);

    // Fill queue from a different thread
    std::thread fillQueue([&queue](){
        for (size_t i = 0; i < capacity; i++) {
            ASSERT_TRUE(queue.push(static_cast<int>(i)));
        }
    });

    // Make sure all elements are received in correct order
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(queue.pop(), static_cast<int>(i));
    }

    fillQueue.join();
}

/**
 * When the queue has no elements, and pop is called, it should block
 * the current thread until an element is added to the queue (from another thread).
 * Here we create a separate thread and call pop on an empty queue. Next,
 * we check that the thread is blocked.
 */
TEST(BlockingQueueTest, Queue_BlocksWhileWaitingForElements) {
    constexpr size_t capacity = 1;
    BlockingQueue<int> queue(capacity);

    std::atomic_bool hasReceivedElement = false;

    // fill queue from a different thread
    std::thread waitUntilHasElements([&queue, &hasReceivedElement](){
        queue.pop(); // This should block until an element has been added
        hasReceivedElement = true;
    });

    ASSERT_FALSE(hasReceivedElement);
    queue.push(1);
    waitUntilHasElements.join();
    ASSERT_TRUE(hasReceivedElement);
}


} // namespace android
