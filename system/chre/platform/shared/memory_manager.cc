/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "chre/platform/memory_manager.h"

#include "chre/util/system/debug_dump.h"

namespace chre {

void *MemoryManager::nanoappAlloc(Nanoapp *app, uint32_t bytes) {
  AllocHeader *header = nullptr;
  if (bytes > 0) {
    if (mAllocationCount >= kMaxAllocationCount) {
      LOGE("Failed to allocate memory from Nanoapp ID %" PRIu32
           ": allocation count exceeded limit.", app->getInstanceId());
    } else if ((mTotalAllocatedBytes + bytes) > kMaxAllocationBytes) {
      LOGE("Failed to allocate memory from Nanoapp ID %" PRIu32
           ": not enough space.", app->getInstanceId());
    } else {
      header = static_cast<AllocHeader*>(
          doAlloc(app, sizeof(AllocHeader) + bytes));

      if (header != nullptr) {
        app->setTotalAllocatedBytes(app->getTotalAllocatedBytes() + bytes);
        mTotalAllocatedBytes += bytes;
        if (mTotalAllocatedBytes > mPeakAllocatedBytes) {
          mPeakAllocatedBytes = mTotalAllocatedBytes;
        }
        mAllocationCount++;
        header->data.bytes = bytes;
        header->data.instanceId = app->getInstanceId();
        header++;
      }
    }
  }
  return header;
}

void MemoryManager::nanoappFree(Nanoapp *app, void *ptr) {
  if (ptr != nullptr) {
    AllocHeader *header = static_cast<AllocHeader*>(ptr);
    header--;

    // TODO: Clean up API contract of chreSendEvent to specify nanoapps can't
    // release ownership of data to other nanoapps so a CHRE_ASSERT_LOG can be
    // used below and the code can return.
    if (app->getInstanceId() != header->data.instanceId) {
      LOGW("Nanoapp ID=%" PRIu32 " tried to free data from nanoapp ID=%" PRIu32,
          app->getInstanceId(), header->data.instanceId);
    }

    size_t nanoAppTotalAllocatedBytes = app->getTotalAllocatedBytes();
    if (nanoAppTotalAllocatedBytes >= header->data.bytes) {
      app->setTotalAllocatedBytes(
          nanoAppTotalAllocatedBytes - header->data.bytes);
    } else {
      app->setTotalAllocatedBytes(0);
    }

    if (mTotalAllocatedBytes >= header->data.bytes) {
      mTotalAllocatedBytes -= header->data.bytes;
    } else {
      mTotalAllocatedBytes = 0;
    }
    if (mAllocationCount > 0) {
      mAllocationCount--;
    }

    doFree(app, header);
  }
}

void MemoryManager::logStateToBuffer(char *buffer, size_t *bufferPos,
                                     size_t bufferSize) const {
  debugDumpPrint(buffer, bufferPos, bufferSize,
                 "\nNanoapp heap usage: %zu bytes allocated, %zu peak bytes"
                 " allocated, count %zu\n", getTotalAllocatedBytes(),
                 getPeakAllocatedBytes(), getAllocationCount());
}

}  // namespace chre
