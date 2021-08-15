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

#include <general_test/simple_heap_alloc_test.h>

#include <cstddef>

#include <general_test/test_names.h>
#include <shared/abort.h>
#include <shared/array_length.h>
#include <shared/nano_string.h>
#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::MessageType;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendMessageToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {

// For most platforms, we expect that what the compiler toolchain claims
// is the maximum alignment needed for any type is accurate.  However, we
// do support one CHRE implementation where it is configured for a lower
// max alignment than what the toolchain claims.
// To support this, we allow for a compiler define set for building this
// test.  For the most part, we need to just trust the CHRE implementation
// that this number is correct.  However, we make a basic sanity check of
// this in testMaxAlignment().

constexpr size_t kMaxAlignment =
#ifdef CHRE_CUSTOM_MAX_ALIGNMENT
    CHRE_CUSTOM_MAX_ALIGNMENT;
#else
    alignof(max_align_t);
#endif  // else CHRE_CUSTOM_MAX_ALIGNMENT

#ifdef CHRE_CUSTOM_MAX_ALIGNMENT
// We only test this when a CHRE implementation claims a custom max aligment.
// We use an argument here to try to keep the compiler from performing any
// of these calculations at compile-time, so they're forced to happen at
// runtime.  We do a mixture of multiplication and division, to force
// various instructions which might have alignment constraints.
static void testMaxAlignment(uint32_t zero) {
  // It's not sufficient to use alignas(kMaxAlignment).  Say kMaxAlignment
  // is 4.  Then alignas(4) could legally give something aligned on 32 bytes,
  // and we wouldn't be testing what we hoped to test.  So we ask for double
  // the alignment (alignas(8), in our example), and then offset into that
  // to assure that we're at exactly kMaxAlignment, and no more.

#ifdef CHRE_NO_DOUBLE_SUPPORT
  typedef float MyFloat;
#define FLOAT_C(value) value##f
#else
  typedef long double myFloat;
#define FLOAT_C(value) value
#endif

  alignas(kMaxAlignment * 2) uint8_t
      myFloatMemory[sizeof(MyFloat) * 3 + kMaxAlignment];
  MyFloat *mfArray =
      reinterpret_cast<MyFloat*>(myFloatMemory + kMaxAlignment);
  mfArray[0] = static_cast<MyFloat>(zero) + FLOAT_C(1.0);
  mfArray[1] = static_cast<MyFloat>(zero) + FLOAT_C(3.0);
  mfArray[2] = mfArray[0] / mfArray[1];
  if ((mfArray[0] * mfArray[1] + mfArray[2]) / FLOAT_C(3.0) == FLOAT_C(1.0)) {
    sendFatalFailureToHost("Float math is wrong");
  }

  constexpr size_t kUllSize = sizeof(unsigned long long);
  static_assert(kUllSize >= 8, "Size of long long violates spec");
  alignas(kMaxAlignment * 2) uint8_t
      longlongMemory[kUllSize * 3 + kMaxAlignment];
  unsigned long long *ullArray =
      reinterpret_cast<unsigned long long*>(longlongMemory + kMaxAlignment);
  ullArray[0] = static_cast<unsigned long long>(zero) +
      (1ULL << (kUllSize * 8 - 4));
  ullArray[1] = static_cast<unsigned long long>(zero) + (1ULL << 3);
  ullArray[2] = ullArray[0] * ullArray[1];
  constexpr unsigned long long kExpected = 747134227367742ULL;
  unsigned long long result = ullArray[2] / 12345ULL;
  if (((kUllSize == 8) && (result != kExpected)) ||
      ((kUllSize > 8) && (result <= kExpected))) {
    sendFatalFailureToHost("Long long math is wrong");
  }
}
#endif  // CHRE_CUSTOM_MAX_ALIGNMENT


SimpleHeapAllocTest::SimpleHeapAllocTest()
  : Test(CHRE_API_VERSION_1_0), mHasFreed(false) {
}

void SimpleHeapAllocTest::setUp(uint32_t messageSize,
                                const void * /* message */) {
  nanoapp_testing::memset(mPtrs, 0, sizeof(mPtrs));

  if (messageSize != 0) {
    sendFatalFailureToHost(
        "SimpleHeapAlloc message expects 0 additional bytes, got ",
        &messageSize);
  }

  // Allocate random small-ish sizes.
  static constexpr size_t kSizes[5] = {
    16, 53, 2, 32, 40 };

  mPtrs[0] = chreHeapAlloc(kSizes[0]);
  mPtrs[1] = chreHeapAlloc(kSizes[1]);
  // For mPtrs[2] we do _not_ use kSizes[2], because we're going to free
  // this in a moment, and intentionally want a different size.
  mPtrs[2] = chreHeapAlloc(23);
  mPtrs[3] = chreHeapAlloc(kSizes[3]);
  // We want to mix in a free among the allocs, just to make sure there
  // isn't some issue there.
  if (mPtrs[2] == nullptr) {
    sendFatalFailureToHost("Failed first allocation of mPtrs[2]");
  } else {
    chreHeapFree(mPtrs[2]);
  }
  mPtrs[4] = chreHeapAlloc(kSizes[4]);
  mPtrs[2] = chreHeapAlloc(kSizes[2]);

  for (uint32_t i = 0; i < arrayLength(mPtrs); i++) {
    if (mPtrs[i] == nullptr) {
      // If we're getting this failure, but convinced the CHRE is
      // correct, make sure that we're actually performing an allocation
      // for each element of mPtrs.
      sendFatalFailureToHost("Failed to allocate index ", &i);
    }
    const uintptr_t ptrValue = reinterpret_cast<uintptr_t>(mPtrs[i]);
    if ((ptrValue & (kMaxAlignment - 1)) != 0) {
      sendFatalFailureToHost("Misaligned allocation at index ", &i);
    }
    // Make sure all of the bytes are addressable.  Our assumption
    // is we'll crash here if that's not the case.  Not the most
    // friendly test, but it's better than allowing a bad CHRE.
    // TODO: If we convince ourselves that chreLog() should be
    //     safe enough to use here, we could log an 'info' message
    //     prior to each memset attempt.
    nanoapp_testing::memset(mPtrs[i], 0xFF, kSizes[i]);
  }
#ifdef CHRE_CUSTOM_MAX_ALIGNMENT
  testMaxAlignment(messageSize);
#endif  // CHRE_CUSTOM_MAX_ALIGNMENT
  sendMessageToHost(MessageType::kContinue);
}

void SimpleHeapAllocTest::handleEvent(uint32_t senderInstanceId,
                                      uint16_t eventType,
                                      const void* eventData) {
  // We ignore the return value, since we expect no data.
  getMessageDataFromHostEvent(senderInstanceId, eventType, eventData,
                              MessageType::kContinue, 0);
  if (mHasFreed) {
    sendFatalFailureToHost("Multiple kContinue messages sent");
  }

  chreHeapFree(mPtrs[3]);
  chreHeapFree(mPtrs[1]);
  chreHeapFree(mPtrs[2]);
  chreHeapFree(mPtrs[0]);
  chreHeapFree(mPtrs[4]);
  mHasFreed = true;

  sendSuccessToHost();
}

}  // namespace general_test
