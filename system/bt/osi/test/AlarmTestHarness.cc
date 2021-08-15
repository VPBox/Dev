/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
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

#include "AlarmTestHarness.h"

#include <hardware/bluetooth.h>

#include "osi/include/alarm.h"
#include "osi/include/wakelock.h"

static bool is_wake_lock_acquired = false;

static int acquire_wake_lock_cb(const char* lock_name) {
  is_wake_lock_acquired = true;
  return BT_STATUS_SUCCESS;
}

static int release_wake_lock_cb(const char* lock_name) {
  is_wake_lock_acquired = false;
  return BT_STATUS_SUCCESS;
}

static bt_os_callouts_t bt_wakelock_callouts = {
    sizeof(bt_os_callouts_t), NULL, acquire_wake_lock_cb, release_wake_lock_cb};

void AlarmTestHarness::SetUp() {
  AllocationTestHarness::SetUp();

  TIMER_INTERVAL_FOR_WAKELOCK_IN_MS = 500;

  wakelock_set_os_callouts(&bt_wakelock_callouts);
}

void AlarmTestHarness::TearDown() {
  alarm_cleanup();
  wakelock_cleanup();
  wakelock_set_os_callouts(NULL);

  AllocationTestHarness::TearDown();
}

bool AlarmTestHarness::WakeLockHeld() { return is_wake_lock_acquired; }
