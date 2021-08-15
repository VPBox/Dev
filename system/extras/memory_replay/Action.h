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

#ifndef _MEMORY_REPLAY_ACTION_H
#define _MEMORY_REPLAY_ACTION_H

#include <stdint.h>

class Pointers;

class Action {
 public:
  Action() {}
  virtual ~Action() {}

  virtual uint64_t Execute(Pointers* pointers) = 0;

  bool IsError() { return is_error_; };

  virtual bool EndThread() { return false; }

  virtual bool DoesFree() { return false; }

  static size_t MaxActionSize();
  static Action* CreateAction(uintptr_t key_pointer, const char* type,
                              const char* line, void* action_memory);

 protected:
  bool is_error_ = false;
};

#endif // _MEMORY_REPLAY_ACTION_H
