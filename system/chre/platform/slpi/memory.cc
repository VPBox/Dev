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

#include "chre/platform/memory.h"
#include "chre/platform/slpi/memory.h"

#ifdef CHRE_SLPI_SEE
#include "chre/platform/slpi/see/island_vote_client.h"
#endif

#include <cstdlib>

extern "C" {

#ifdef CHRE_SLPI_SEE
#include "sns_island_util.h"
#endif  // CHRE_SLPI_SEE

#if defined(CHRE_SLPI_SMGR) || defined (CHRE_SLPI_SEE)
#include "sns_memmgr.h"
#endif

} // extern "C"

namespace chre {

void *memoryAlloc(size_t size) {
#ifdef CHRE_SLPI_UIMG_ENABLED
  #if defined(CHRE_SLPI_SMGR)
    return SNS_OS_U_MALLOC(SNS_CHRE, size);
  #elif defined(CHRE_SLPI_SEE)
    void *ptr = sns_malloc(SNS_HEAP_CHRE_ISLAND, size);

    // Fall back to big image memory when uimg memory is exhausted.
    // Must exclude size 0 as clients may not explicitly free memory of size 0,
    // which may mistakenly hold the system in big image.
    if (ptr == nullptr && size != 0) {
      // Increment big image ref count to prevent system from entering uimg
      // while big image memory is in use.
      IslandVoteClientSingleton::get()->incrementBigImageRefCount();
      ptr = memoryAllocBigImage(size);

      // Big image allocation failed too.
      if (ptr == nullptr) {
        IslandVoteClientSingleton::get()->decrementBigImageRefCount();
      }
    }

    return ptr;
  #else
    #error SLPI UIMG memory allocation not supported
  #endif
#else
  return malloc(size);
#endif // CHRE_SLPI_UIMG_ENABLED
}

void *memoryAllocBigImage(size_t size) {
  return malloc(size);
}

void *palSystemApiMemoryAlloc(size_t size) {
  return malloc(size);
}

void memoryFree(void *pointer) {
#ifdef CHRE_SLPI_UIMG_ENABLED
  #if defined(CHRE_SLPI_SMGR)
    SNS_OS_FREE(pointer);
  #elif defined(CHRE_SLPI_SEE)
    if (sns_island_is_island_ptr(reinterpret_cast<intptr_t>(pointer))) {
      sns_free(pointer);
    } else {
      memoryFreeBigImage(pointer);

      // Must exclude nullptr as it's excluded in memoryAlloc() as well.
      // Note currently sns_island_is_island_ptr returns true for nullptr,
      // so this mainly serves as a protection in case the implementation of
      // sns_island_is_island_ptr changes in the future.
      if (pointer != nullptr) {
        IslandVoteClientSingleton::get()->decrementBigImageRefCount();
      }
    }
  #else
    #error SLPI UIMG memory free not supported
  #endif
#else
  free(pointer);
#endif // CHRE_SLPI_UIMG_ENABLED
}

void memoryFreeBigImage(void *pointer) {
  free(pointer);
}

void palSystemApiMemoryFree(void *pointer) {
  free(pointer);
}

}  // namespace chre
