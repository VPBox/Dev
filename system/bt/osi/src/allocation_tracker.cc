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

#define LOG_TAG "bt_osi_allocation_tracker"

#include "osi/include/allocation_tracker.h"

#include <base/logging.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <unordered_map>

#include "osi/include/allocator.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

typedef struct {
  uint8_t allocator_id;
  void* ptr;
  size_t size;
  bool freed;
} allocation_t;

static const size_t canary_size = 8;
static char canary[canary_size];
static std::unordered_map<void*, allocation_t*> allocations;
static std::mutex tracker_lock;
static bool enabled = false;

// Memory allocation statistics
static size_t alloc_counter = 0;
static size_t free_counter = 0;
static size_t alloc_total_size = 0;
static size_t free_total_size = 0;

void allocation_tracker_init(void) {
  std::unique_lock<std::mutex> lock(tracker_lock);
  if (enabled) return;

  // randomize the canary contents
  for (size_t i = 0; i < canary_size; i++) canary[i] = (char)osi_rand();

  LOG_DEBUG(LOG_TAG, "canary initialized");

  enabled = true;
}

// Test function only. Do not call in the normal course of operations.
void allocation_tracker_uninit(void) {
  std::unique_lock<std::mutex> lock(tracker_lock);
  if (!enabled) return;

  allocations.clear();
  enabled = false;
}

void allocation_tracker_reset(void) {
  std::unique_lock<std::mutex> lock(tracker_lock);
  if (!enabled) return;

  allocations.clear();
}

size_t allocation_tracker_expect_no_allocations(void) {
  std::unique_lock<std::mutex> lock(tracker_lock);
  if (!enabled) return 0;

  size_t unfreed_memory_size = 0;

  for (const auto& entry : allocations) {
    allocation_t* allocation = entry.second;
    if (!allocation->freed) {
      unfreed_memory_size +=
          allocation->size;  // Report back the unfreed byte count
      LOG_ERROR(LOG_TAG,
                "%s found unfreed allocation. address: 0x%zx size: %zd bytes",
                __func__, (uintptr_t)allocation->ptr, allocation->size);
    }
  }

  return unfreed_memory_size;
}

void* allocation_tracker_notify_alloc(uint8_t allocator_id, void* ptr,
                                      size_t requested_size) {
  char* return_ptr;
  {
    std::unique_lock<std::mutex> lock(tracker_lock);
    if (!enabled || !ptr) return ptr;

    // Keep statistics
    alloc_counter++;
    alloc_total_size += allocation_tracker_resize_for_canary(requested_size);

    return_ptr = ((char*)ptr) + canary_size;

    auto map_entry = allocations.find(return_ptr);
    allocation_t* allocation;
    if (map_entry != allocations.end()) {
      allocation = map_entry->second;
      CHECK(allocation->freed);  // Must have been freed before
    } else {
      allocation = (allocation_t*)calloc(1, sizeof(allocation_t));
      allocations[return_ptr] = allocation;
    }

    allocation->allocator_id = allocator_id;
    allocation->freed = false;
    allocation->size = requested_size;
    allocation->ptr = return_ptr;
  }

  // Add the canary on both sides
  memcpy(return_ptr - canary_size, canary, canary_size);
  memcpy(return_ptr + requested_size, canary, canary_size);

  return return_ptr;
}

void* allocation_tracker_notify_free(UNUSED_ATTR uint8_t allocator_id,
                                     void* ptr) {
  std::unique_lock<std::mutex> lock(tracker_lock);

  if (!enabled || !ptr) return ptr;

  auto map_entry = allocations.find(ptr);
  CHECK(map_entry != allocations.end());
  allocation_t* allocation = map_entry->second;
  CHECK(allocation);          // Must have been tracked before
  CHECK(!allocation->freed);  // Must not be a double free
  CHECK(allocation->allocator_id ==
        allocator_id);  // Must be from the same allocator

  // Keep statistics
  free_counter++;
  free_total_size += allocation_tracker_resize_for_canary(allocation->size);

  allocation->freed = true;

  UNUSED_ATTR const char* beginning_canary = ((char*)ptr) - canary_size;
  UNUSED_ATTR const char* end_canary = ((char*)ptr) + allocation->size;

  for (size_t i = 0; i < canary_size; i++) {
    CHECK(beginning_canary[i] == canary[i]);
    CHECK(end_canary[i] == canary[i]);
  }

  // Free the hash map entry to avoid unlimited memory usage growth.
  // Double-free of memory is detected with "assert(allocation)" above
  // as the allocation entry will not be present.
  allocations.erase(ptr);
  free(allocation);

  return ((char*)ptr) - canary_size;
}

size_t allocation_tracker_resize_for_canary(size_t size) {
  return (!enabled) ? size : size + (2 * canary_size);
}

void osi_allocator_debug_dump(int fd) {
  dprintf(fd, "\nBluetooth Memory Allocation Statistics:\n");

  std::unique_lock<std::mutex> lock(tracker_lock);

  dprintf(fd, "  Total allocated/free/used counts : %zu / %zu / %zu\n",
          alloc_counter, free_counter, alloc_counter - free_counter);
  dprintf(fd, "  Total allocated/free/used octets : %zu / %zu / %zu\n",
          alloc_total_size, free_total_size,
          alloc_total_size - free_total_size);
}
