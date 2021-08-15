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

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Action.h"
#include "LineBuffer.h"
#include "NativeInfo.h"
#include "Pointers.h"
#include "Thread.h"
#include "Threads.h"

static char g_buffer[65535];

size_t GetMaxAllocs(int fd) {
  lseek(fd, 0, SEEK_SET);
  LineBuffer line_buf(fd, g_buffer, sizeof(g_buffer));
  char* line;
  size_t line_len;
  size_t num_allocs = 0;
  while (line_buf.GetLine(&line, &line_len)) {
    char* word = reinterpret_cast<char*>(memchr(line, ':', line_len));
    if (word == nullptr) {
      continue;
    }

    word++;
    while (*word++ == ' ');
    // This will treat a realloc as an allocation, even if it frees
    // another allocation. Since reallocs are relatively rare, this
    // shouldn't inflate the numbers that much.
    if (*word == 'f') {
      // Check if this is a free of zero.
      uintptr_t pointer;
      if (sscanf(word, "free %" SCNxPTR, &pointer) == 1 && pointer != 0) {
        num_allocs--;
      }
    } else if (*word != 't') {
      // Skip the thread_done message.
      num_allocs++;
    }
  }
  return num_allocs;
}

void ProcessDump(int fd, size_t max_allocs, size_t max_threads) {
  lseek(fd, 0, SEEK_SET);
  Pointers pointers(max_allocs);
  Threads threads(&pointers, max_threads);

  printf("Maximum threads available:   %zu\n", threads.max_threads());
  printf("Maximum allocations in dump: %zu\n", max_allocs);
  printf("Total pointers available:    %zu\n", pointers.max_pointers());
  printf("\n");

  PrintNativeInfo("Initial ");

  LineBuffer line_buf(fd, g_buffer, sizeof(g_buffer));
  char* line;
  size_t line_len;
  size_t line_number = 0;
  while (line_buf.GetLine(&line, &line_len)) {
    pid_t tid;
    int line_pos = 0;
    char type[128];
    uintptr_t key_pointer;

    // Every line is of this format:
    //   <tid>: <action_type> <pointer>
    // Some actions have extra arguments which will be used and verified
    // when creating the Action object.
    if (sscanf(line, "%d: %s %" SCNxPTR " %n", &tid, type, &key_pointer, &line_pos) != 3) {
      err(1, "Unparseable line found: %s\n", line);
    }
    line_number++;
    if ((line_number % 100000) == 0) {
      printf("  At line %zu:\n", line_number);
      PrintNativeInfo("    ");
    }
    Thread* thread = threads.FindThread(tid);
    if (thread == nullptr) {
      thread = threads.CreateThread(tid);
    }

    // Wait for the thread to complete any previous actions before handling
    // the next action.
    thread->WaitForReady();

    Action* action = thread->CreateAction(key_pointer, type, line + line_pos);
    if (action == nullptr) {
      err(1, "Cannot create action from line: %s\n", line);
    }

    bool does_free = action->DoesFree();
    if (does_free) {
      // Make sure that any other threads doing allocations are complete
      // before triggering the action. Otherwise, another thread could
      // be creating the allocation we are going to free.
      threads.WaitForAllToQuiesce();
    }

    // Tell the thread to execute the action.
    thread->SetPending();

    if (action->EndThread()) {
      // Wait for the thread to finish and clear the thread entry.
      threads.Finish(thread);
    }

    // Wait for this action to complete. This avoids a race where
    // another thread could be creating the same allocation where are
    // trying to free.
    if (does_free) {
      thread->WaitForReady();
    }
  }
  // Wait for all threads to stop processing actions.
  threads.WaitForAllToQuiesce();

  PrintNativeInfo("Final ");

  // Free any outstanding pointers.
  // This allows us to run a tool like valgrind to verify that no memory
  // is leaked and everything is accounted for during a run.
  threads.FinishAll();
  pointers.FreeAll();

  // Print out the total time making all allocation calls.
  printf("Total Allocation/Free Time: %" PRIu64 "ns %0.2fs\n",
         threads.total_time_nsecs(), threads.total_time_nsecs()/1000000000.0);
}

constexpr size_t DEFAULT_MAX_THREADS = 512;

int main(int argc, char** argv) {
  if (argc != 2 && argc != 3) {
    if (argc > 3) {
      fprintf(stderr, "Only two arguments are expected.\n");
    } else {
      fprintf(stderr, "Requires at least one argument.\n");
    }
    fprintf(stderr, "Usage: %s MEMORY_LOG_FILE [MAX_THREADS]\n", basename(argv[0]));
    return 1;
  }

  size_t max_threads = DEFAULT_MAX_THREADS;
  if (argc == 3) {
    max_threads = atoi(argv[2]);
  }

  int dump_fd = open(argv[1], O_RDONLY);
  if (dump_fd == -1) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
    return 1;
  }

  printf("Processing: %s\n", argv[1]);

  // Do a first pass to get the total number of allocations used at one
  // time to allow a single mmap that can hold the maximum number of
  // pointers needed at once.
  size_t max_allocs = GetMaxAllocs(dump_fd);
  ProcessDump(dump_fd, max_allocs, max_threads);

  close(dump_fd);

  return 0;
}
