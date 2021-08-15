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

#include <gtest/gtest.h>
#include <pthread.h>
#include <unistd.h>

#include <utility>

#include "Action.h"
#include "Pointers.h"
#include "Thread.h"

typedef std::pair<Thread*, volatile bool*> thread_data_t;

TEST(ThreadTest, ready) {
  Thread thread;

  // A thread should be ready immediately. If not, this will hang forever.
  thread.WaitForReady();
}

void* ThreadWaitForReady(void* data) {
  thread_data_t* thread_data = reinterpret_cast<thread_data_t*>(data);
  Thread* thread = thread_data->first;
  volatile bool* finish = thread_data->second;

  thread->WaitForReady();
  *finish = true;

  return nullptr;
}

TEST(ThreadTest, ready_thread) {
  Thread thread;
  volatile bool finish = false;
  thread_data_t thread_data = std::make_pair(&thread, &finish);

  thread.SetPending();

  pthread_t thread_id;
  ASSERT_TRUE(pthread_create(&thread_id, nullptr, ThreadWaitForReady, &thread_data) == 0);

  ASSERT_FALSE(finish);
  sleep(1);
  ASSERT_FALSE(finish);

  thread.ClearPending();
  ASSERT_TRUE(pthread_join(thread_id, nullptr) == 0);
  ASSERT_TRUE(finish);
}

void* ThreadWaitForPending(void* data) {
  thread_data_t* thread_data = reinterpret_cast<thread_data_t*>(data);
  Thread* thread = thread_data->first;
  volatile bool* finish = thread_data->second;

  thread->WaitForPending();
  *finish = true;

  return nullptr;
}

TEST(ThreadTest, pending) {
  Thread thread;
  volatile bool finish = false;
  thread_data_t thread_data = std::make_pair(&thread, &finish);

  pthread_t thread_id;
  ASSERT_TRUE(pthread_create(&thread_id, nullptr, ThreadWaitForPending, &thread_data) == 0);

  ASSERT_FALSE(finish);
  sleep(1);
  ASSERT_FALSE(finish);

  thread.SetPending();
  ASSERT_TRUE(pthread_join(thread_id, nullptr) == 0);
  ASSERT_TRUE(finish);
}

TEST(ThreadTest, pointers) {
  Pointers pointers(2);
  Thread thread;

  ASSERT_TRUE(thread.pointers() == nullptr);
  thread.set_pointers(&pointers);
  ASSERT_TRUE(thread.pointers() == &pointers);
}

TEST(ThreadTest, action) {
  Thread thread;

  Action* action = thread.CreateAction(0x1234, "thread_done", "");
  ASSERT_EQ(action, thread.GetAction());

  // Verify the action object is not garbage.
  action->Execute(nullptr);

  ASSERT_TRUE(action->EndThread());
  ASSERT_FALSE(action->DoesFree());
}
