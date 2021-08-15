/******************************************************************************
 *
 *  Copyright 2016 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <base/logging.h>

#include "common/leaky_bonded_queue.h"

namespace testing {

using bluetooth::common::LeakyBondedQueue;

#define ITEM_EQ(a, b)                  \
  do {                                 \
    EXPECT_EQ(a, b);                   \
    EXPECT_EQ((a)->index, (b)->index); \
  } while (0)

class Item {
 public:
  Item(int i) { index = i; }
  virtual ~Item() {}
  int index;
};

class MockItem : public Item {
 public:
  MockItem(int i) : Item(i) {}
  ~MockItem() { Destruct(); }
  MOCK_METHOD0(Destruct, void());
};

TEST(LeakyBondedQueueTest, TestEnqueueDequeue) {
  MockItem* item1 = new MockItem(1);
  MockItem* item2 = new MockItem(2);
  MockItem* item3 = new MockItem(3);
  MockItem* item4 = new MockItem(4);
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(3);
  EXPECT_EQ(queue->Capacity(), static_cast<size_t>(3));
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  queue->Enqueue(item2);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  queue->Enqueue(item3);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(3));
  EXPECT_CALL(*item1, Destruct()).Times(1);
  queue->Enqueue(item4);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(3));
  MockItem* item2_2 = queue->Dequeue();
  MockItem* item3_3 = queue->Dequeue();
  MockItem* item4_4 = queue->Dequeue();
  EXPECT_THAT(item2_2, NotNull());
  ITEM_EQ(item2_2, item2);
  EXPECT_THAT(item3_3, NotNull());
  ITEM_EQ(item3_3, item3);
  EXPECT_THAT(item4_4, NotNull());
  ITEM_EQ(item4_4, item4);
  LOG(INFO) << "All done release items";
  EXPECT_CALL(*item2_2, Destruct()).Times(1);
  delete item2_2;
  EXPECT_CALL(*item3_3, Destruct()).Times(1);
  delete item3_3;
  EXPECT_CALL(*item4_4, Destruct()).Times(1);
  delete item4_4;
  delete queue;
}

TEST(LeakyBondedQueueTest, TestEnqueueDequeue2) {
  MockItem* item1 = new MockItem(1);
  MockItem* item2 = new MockItem(2);
  MockItem* item3 = new MockItem(3);
  MockItem* item4 = new MockItem(4);
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(2);
  EXPECT_EQ(queue->Capacity(), static_cast<size_t>(2));
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  MockItem* item1_1 = queue->Dequeue();
  ITEM_EQ(item1, item1_1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item2);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  queue->Enqueue(item3);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item2, Destruct()).Times(1);
  queue->Enqueue(item4);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item3, Destruct()).Times(1);
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  MockItem* item4_4_4 = queue->Dequeue();
  MockItem* item1_1_1 = queue->Dequeue();
  ITEM_EQ(item4_4_4, item4);
  ITEM_EQ(item1_1_1, item1);
  EXPECT_CALL(*item1_1_1, Destruct()).Times(1);
  delete item1_1_1;
  EXPECT_CALL(*item4_4_4, Destruct()).Times(1);
  delete item4_4_4;
  delete queue;
}

TEST(LeakyBondedQueueTest, TestEnqueuePop) {
  MockItem* item1 = new MockItem(1);
  MockItem* item2 = new MockItem(2);
  MockItem* item3 = new MockItem(3);
  MockItem* item4 = new MockItem(4);
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(2);
  EXPECT_EQ(queue->Capacity(), static_cast<size_t>(2));
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  MockItem* item1_1 = queue->Dequeue();
  ITEM_EQ(item1, item1_1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item2);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  queue->Enqueue(item3);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  MockItem* item2_2 = queue->EnqueueWithPop(item4);
  EXPECT_THAT(item2_2, NotNull());
  ITEM_EQ(item2_2, item2);
  EXPECT_CALL(*item2, Destruct()).Times(1);
  delete item2_2;
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  MockItem* item3_3 = queue->EnqueueWithPop(item1);
  EXPECT_THAT(item3_3, NotNull());
  ITEM_EQ(item3_3, item3);
  EXPECT_CALL(*item3, Destruct()).Times(1);
  delete item3_3;
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  MockItem* item4_4_4 = queue->Dequeue();
  MockItem* item1_1_1 = queue->Dequeue();
  ITEM_EQ(item4_4_4, item4);
  ITEM_EQ(item1_1_1, item1);
  EXPECT_CALL(*item1_1_1, Destruct()).Times(1);
  delete item1_1_1;
  EXPECT_CALL(*item4_4_4, Destruct()).Times(1);
  delete item4_4_4;
  delete queue;
}

TEST(LeakyBondedQueueTest, TestQueueClear) {
  MockItem* item1 = new MockItem(1);
  MockItem* item2 = new MockItem(2);
  MockItem* item3 = new MockItem(3);
  MockItem* item4 = new MockItem(4);
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(2);
  EXPECT_EQ(queue->Capacity(), static_cast<size_t>(2));
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  MockItem* item1_1 = queue->Dequeue();
  ITEM_EQ(item1, item1_1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item2);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  queue->Enqueue(item3);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item2, Destruct()).Times(1);
  queue->Enqueue(item4);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item3, Destruct()).Times(1);
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item1, Destruct()).Times(1);
  EXPECT_CALL(*item4, Destruct()).Times(1);
  queue->Clear();
  delete queue;
}

TEST(LeakyBondedQueueTest, TestQueueFree) {
  MockItem* item1 = new MockItem(1);
  MockItem* item2 = new MockItem(2);
  MockItem* item3 = new MockItem(3);
  MockItem* item4 = new MockItem(4);
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(2);
  EXPECT_EQ(queue->Capacity(), static_cast<size_t>(2));
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  MockItem* item1_1 = queue->Dequeue();
  ITEM_EQ(item1, item1_1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(0));
  queue->Enqueue(item2);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(1));
  queue->Enqueue(item3);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item2, Destruct()).Times(1);
  queue->Enqueue(item4);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item3, Destruct()).Times(1);
  queue->Enqueue(item1);
  EXPECT_EQ(queue->Length(), static_cast<size_t>(2));
  EXPECT_CALL(*item1, Destruct()).Times(1);
  EXPECT_CALL(*item4, Destruct()).Times(1);
  delete queue;
}

TEST(LeakyBondedQueueTest, TestPushNull) {
  MockItem* item1 = nullptr;
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(2);
  queue->Enqueue(item1);
  MockItem* item1_1 = queue->Dequeue();
  EXPECT_THAT(item1_1, IsNull());
}

TEST(LeakyBondedQueueTest, TestPushNullOverflowQueue) {
  MockItem* item1 = nullptr;
  MockItem* item2 = nullptr;
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(1);
  queue->Enqueue(item1);
  queue->Enqueue(item2);
  MockItem* item2_2 = queue->Dequeue();
  EXPECT_THAT(item2_2, IsNull());
}

TEST(LeakyBondedQueueTest, TestPushNullDeleteQueue) {
  MockItem* item1 = nullptr;
  MockItem* item2 = nullptr;
  LeakyBondedQueue<MockItem>* queue = new LeakyBondedQueue<MockItem>(2);
  queue->Enqueue(item1);
  queue->Enqueue(item2);
  delete queue;
}
}  // namespace testing
