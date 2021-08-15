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

#include <inttypes.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <time.h>

#include <new>

#include "Action.h"
#include "Threads.h"
#include "Pointers.h"

static uint64_t nanotime() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<uint64_t>(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

class EndThreadAction : public Action {
 public:
  EndThreadAction() {}

  bool EndThread() override { return true; }

  uint64_t Execute(Pointers*) override { return 0; }
};

class AllocAction : public Action {
 public:
  explicit AllocAction(uintptr_t key_pointer) : key_pointer_(key_pointer) {}

 protected:
  uintptr_t key_pointer_ = 0;
  size_t size_ = 0;
};

class MallocAction : public AllocAction {
 public:
  MallocAction(uintptr_t key_pointer, const char* line) : AllocAction(key_pointer) {
    if (sscanf(line, "%zu", &size_) != 1) {
      is_error_ = true;
    }
  }

  uint64_t Execute(Pointers* pointers) override {
    uint64_t time_nsecs = nanotime();
    void* memory = malloc(size_);
    time_nsecs = nanotime() - time_nsecs;

    memset(memory, 1, size_);
    pointers->Add(key_pointer_, memory);

    return time_nsecs;
  }
};

class CallocAction : public AllocAction {
 public:
  CallocAction(uintptr_t key_pointer, const char* line) : AllocAction(key_pointer) {
    if (sscanf(line, "%zu %zu", &n_elements_, &size_) != 2) {
      is_error_ = true;
    }
  }

  uint64_t Execute(Pointers* pointers) override {
    uint64_t time_nsecs = nanotime();
    void* memory = calloc(n_elements_, size_);
    time_nsecs = nanotime() - time_nsecs;

    memset(memory, 0, n_elements_ * size_);
    pointers->Add(key_pointer_, memory);

    return time_nsecs;
  }

 private:
  size_t n_elements_ = 0;
};

class ReallocAction : public AllocAction {
 public:
  ReallocAction(uintptr_t key_pointer, const char* line) : AllocAction(key_pointer) {
    if (sscanf(line, "%" SCNxPTR " %zu", &old_pointer_, &size_) != 2) {
      is_error_ = true;
    }
  }

  bool DoesFree() override { return old_pointer_ != 0; }

  uint64_t Execute(Pointers* pointers) override {
    void* old_memory = nullptr;
    if (old_pointer_ != 0) {
      old_memory = pointers->Remove(old_pointer_);
    }

    uint64_t time_nsecs = nanotime();
    void* memory = realloc(old_memory, size_);
    time_nsecs = nanotime() - time_nsecs;

    memset(memory, 1, size_);
    pointers->Add(key_pointer_, memory);

    return time_nsecs;
  }

 private:
  uintptr_t old_pointer_ = 0;
};

class MemalignAction : public AllocAction {
 public:
  MemalignAction(uintptr_t key_pointer, const char* line) : AllocAction(key_pointer) {
    if (sscanf(line, "%zu %zu", &align_, &size_) != 2) {
      is_error_ = true;
    }
  }

  uint64_t Execute(Pointers* pointers) override {
    uint64_t time_nsecs = nanotime();
    void* memory = memalign(align_, size_);
    time_nsecs = nanotime() - time_nsecs;

    memset(memory, 1, size_);
    pointers->Add(key_pointer_, memory);

    return time_nsecs;
  }

 private:
  size_t align_ = 0;
};

class FreeAction : public AllocAction {
 public:
  explicit FreeAction(uintptr_t key_pointer) : AllocAction(key_pointer) {
  }

  bool DoesFree() override { return key_pointer_ != 0; }

  uint64_t Execute(Pointers* pointers) override {
    if (key_pointer_) {
      void* memory = pointers->Remove(key_pointer_);
      uint64_t time_nsecs = nanotime();
      free(memory);
      return nanotime() - time_nsecs;
    }
    return 0;
  }
};

size_t Action::MaxActionSize() {
  size_t max = MAX(sizeof(EndThreadAction), sizeof(MallocAction));
  max = MAX(max, sizeof(CallocAction));
  max = MAX(max, sizeof(ReallocAction));
  max = MAX(max, sizeof(MemalignAction));
  return MAX(max, sizeof(FreeAction));
}

Action* Action::CreateAction(uintptr_t key_pointer, const char* type,
                             const char* line, void* action_memory) {
  Action* action = nullptr;
  if (strcmp(type, "malloc") == 0) {
    action = new (action_memory) MallocAction(key_pointer, line);
  } else if (strcmp(type, "free") == 0) {
    action = new (action_memory) FreeAction(key_pointer);
  } else if (strcmp(type, "calloc") == 0) {
    action = new (action_memory) CallocAction(key_pointer, line);
  } else if (strcmp(type, "realloc") == 0) {
    action = new (action_memory) ReallocAction(key_pointer, line);
  } else if (strcmp(type, "memalign") == 0) {
    action = new (action_memory) MemalignAction(key_pointer, line);
  } else if (strcmp(type, "thread_done") == 0) {
    action = new (action_memory) EndThreadAction();
  }

  if (action == nullptr || action->IsError()) {
    return nullptr;
  }
  return action;
}
