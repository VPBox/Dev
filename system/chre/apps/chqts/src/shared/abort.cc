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

#include <shared/abort.h>

#include <cstdint>

#include <chre.h>

namespace nanoapp_testing {

void abort(AbortBlame reason) {
  uint32_t abortCode = UINT32_C(0x10000) + static_cast<uint16_t>(reason);
  chreAbort(abortCode);

  // We should never get here.  But in the case that the CHRE has a bug
  // in its chreAbort() implementation, let's try to blow things up.
  // Some compilers are smart about not wanting to crash like this, so
  // we need to fool them via indirection.
  uint16_t zero = static_cast<uint16_t>(reason)
      - static_cast<uint8_t>(reason);
  uint8_t *badPointer = reinterpret_cast<uint8_t*>(zero);
  *badPointer = 1;

  // If we're here, we have a buggy CHRE on a platform where it's okay
  // to write to nullptr.  At the very least, let's not return from this
  // function.
  while (true) {}
}

}  // namespace nanoapp_testing
