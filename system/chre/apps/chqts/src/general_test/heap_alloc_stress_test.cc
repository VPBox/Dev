/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <general_test/heap_alloc_stress_test.h>

#include <cstddef>

#include <general_test/test_names.h>
#include <shared/abort.h>
#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::sendFailureToHost;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {

static void tryAbsurdMalloc(uint32_t hugeSize) {
  void *ptr = chreHeapAlloc(hugeSize);
  if (ptr != NULL) {
    sendFailureToHost("chreHeapAlloc claimed allocation of huge size ",
                      &hugeSize);
    chreHeapFree(ptr);
    nanoapp_testing::abort();
  }
}

HeapAllocStressTest::HeapAllocStressTest()
  : Test(CHRE_API_VERSION_1_0) {
}

void HeapAllocStressTest::setUp(uint32_t messageSize,
                                const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "HeapAllocStress message expects 0 additional bytes, got ",
        &messageSize);
  }

  // 1GB should be absurd on any CHRE implementation we anticipate for a
  // while.
  tryAbsurdMalloc(UINT32_C(0x40000000));

  // Let's also make sure there's nothing treating this as signed behind
  // the scenes and breaking things.
  tryAbsurdMalloc(UINT32_C(-16));

  // Since NULL is a valid response to chreHeapAlloc(), chreHeapFree()
  // must accept it as an argument.
  chreHeapFree(NULL);

  // We do not test chreHeapFree() with invalid pointers, because that's
  // an error by the caller, and there's no requirement for the CHRE
  // implementation to handle it nicely.


  // Now let's exhaust the heap, and make sure it properly frees up to allow
  // things to be allocated again.
  constexpr size_t kNumPtrs = 256;
  void **ptrs = reinterpret_cast<void**>(
      chreHeapAlloc(kNumPtrs * sizeof(void*)));
  if (ptrs == NULL) {
    // Oh, the irony.
    sendFatalFailureToHost(
        "Insufficient free heap to test heap exhaustion.");
  }

  size_t index;
  uint32_t last_alloc_size = 1024 * 1024 * 256;
  for (index = 0; (index < kNumPtrs); index++) {
    uint32_t curr_alloc_size = last_alloc_size;
    void *ptr = chreHeapAlloc(curr_alloc_size);
    while (ptr == NULL) {
      curr_alloc_size /= 2;
      if (curr_alloc_size < 16) {
        break;
      }
      ptr = chreHeapAlloc(curr_alloc_size);
    }
    if (ptr == NULL) {
      break;
    }
    last_alloc_size = curr_alloc_size;
    ptrs[index] = ptr;
  }
  if (index == 0) {
    sendFatalFailureToHost(
        "Failed to allocate anything for heap exhaustion");
  }

  // We should be able to free this allocation, and then obtain it again.
  index--;
  chreHeapFree(ptrs[index]);
  ptrs[index] = chreHeapAlloc(last_alloc_size);
  if (ptrs[index] == NULL) {
    sendFatalFailureToHost(
        "After exhausting heap and then free'ing, unable to alloc "
        "again for size ", &last_alloc_size);
  }

  // Everything's good, let's free up our memory.
  for (size_t i = 0; i <= index; i++) {
    chreHeapFree(ptrs[i]);
  }
  chreHeapFree(ptrs);

  sendSuccessToHost();
}

void HeapAllocStressTest::handleEvent(uint32_t /* senderInstanceId */,
                                      uint16_t eventType,
                                      const void* /* eventData */) {
  unexpectedEvent(eventType);
}

}  // namespace general_test
