/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
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

#include <base/bind.h>
#include <base/logging.h>
#include <base/threading/thread.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#include "btcore/include/module.h"
#include "common/message_loop_thread.h"
#include "osi/include/alarm.h"
#include "osi/include/fixed_queue.h"
#include "stack/include/btu.h"

class TimeoutHelper {
 public:
  TimeoutHelper() { sem_init(&sem, 0, 0); }

  ~TimeoutHelper() { sem_destroy(&sem); }

  void wait(int seconds, base::Closure callback) {
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += seconds;

    int semvalue;
    sem_getvalue(&sem, &semvalue);

    // Call the callback if timeout occured
    if (sem_timedwait(&sem, &timeout) == -1 && !callback.is_null()) {
      callback.Run();
    }
  }

  void notify() { sem_post(&sem); }

 private:
  sem_t sem;
};

TimeoutHelper helper;

// External function definitions
void btu_task_start_up(void* context);
void btu_task_shut_down(void* context);

/* Below are methods and variables that must be implemented if we don't want to
 * compile the whole stack. They will be removed, or changed into mocks one by
 * one in the future, as the refactoring progresses */
bt_status_t do_in_jni_thread(const base::Location& from_here,
                             base::OnceClosure task) {
  helper.notify();
  return BT_STATUS_SUCCESS;
}

void btu_init_core(){};
void btif_init_ok(unsigned short, char*){};
void BTE_InitStack(){};
void bta_sys_init(){};
void bta_sys_free(){};
void btu_free_core(){};
const module_t* get_module(const char*) { return nullptr; };
bool module_init(module_t const*) { return true; };
void module_clean_up(module_t const*){};

bluetooth::common::MessageLoopThread bt_startup_thread("test alarm thread");

class BtuMessageLoopTest : public testing::Test {
 public:
  MOCK_METHOD0(TestCallback, void(void));
  base::MessageLoop* message_loop;

  virtual void SetUp() {
    // Initialize alarms to prevent btu_task_shut_down from crashing
    alarm_new("test alarm");
    bt_startup_thread.StartUp();
    // btu_task_start_up calls btif_transfer_context to let the stack know
    // start up is finished
    btu_task_start_up(nullptr);
    helper.wait(5, base::Bind(&BtuMessageLoopTest::Fail, base::Unretained(this),
                              "BTU startup timed out"));
  }

  virtual void TearDown() {
    btu_task_shut_down(nullptr);
    alarm_cleanup();
    bt_startup_thread.ShutDown();
  }

  void Fail(std::string message) { FAIL() << message; }
};

TEST_F(BtuMessageLoopTest, send_message) {
  message_loop = get_main_message_loop();
  EXPECT_FALSE(message_loop == nullptr);

  EXPECT_CALL(*this, TestCallback()).Times(1);
  message_loop->task_runner()->PostTask(
      FROM_HERE,
      base::Bind(&BtuMessageLoopTest::TestCallback, base::Unretained(this)));

  message_loop->task_runner()->PostTask(
      FROM_HERE, base::Bind(&TimeoutHelper::notify, base::Unretained(&helper)));

  // Prevent the test from ending before the message loop posts the function
  helper.wait(5, base::Bind(&BtuMessageLoopTest::Fail, base::Unretained(this),
                            "Timed out waiting for callback"));
}
