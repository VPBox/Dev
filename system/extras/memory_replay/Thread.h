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

#ifndef _MEMORY_REPLAY_THREAD_H
#define _MEMORY_REPLAY_THREAD_H

#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>

class Action;
class Pointers;

constexpr size_t ACTION_MEMORY_SIZE = 128;

class Thread {
 public:
  Thread();
  virtual ~Thread();

  void WaitForReady();
  void WaitForPending();
  void SetPending();
  void ClearPending();

  Action* CreateAction(uintptr_t key_pointer, const char* type, const char* line);
  void AddTimeNsecs(uint64_t nsecs) { total_time_nsecs_ += nsecs; }

  void set_pointers(Pointers* pointers) { pointers_ = pointers; }
  Pointers* pointers() { return pointers_; }

  Action* GetAction() { return reinterpret_cast<Action*>(action_memory_); }

 private:
  pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond_;
  bool pending_ = false;

  pthread_t thread_id_;
  pid_t tid_ = 0;
  uint64_t total_time_nsecs_ = 0;

  Pointers* pointers_ = nullptr;

  // Per thread memory for an Action. Only one action can be processed.
  // at a time.
  static constexpr size_t ACTION_SIZE = 128;
  uint8_t action_memory_[ACTION_SIZE];

  friend class Threads;
};

#endif // _MEMORY_REPLAY_THREAD_H
