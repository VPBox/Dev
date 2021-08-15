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

#ifndef NVRAM_CORE_STORAGE_H_
#define NVRAM_CORE_STORAGE_H_

extern "C" {
#include <stdint.h>
}  // extern "C"

#include <nvram/messages/blob.h>

namespace nvram {
namespace storage {

// Indicates the result of a storage operation.
enum class Status {
  kSuccess,       // Operation successful.
  kNotFound,      // The element to be read could not be found.
  kStorageError,  // Failure on the underlying storage layer.
};

// Load NVRAM header from storage. See the comment on LoadSpace() for details on
// semantics and return values.
Status LoadHeader(Blob* blob);

// Write the binary-encoded NVRAM header |blob| to storage. See the comment on
// StoreSpace() for details on semantics and return values.
Status StoreHeader(const Blob& blob);

// Load NVRAM space data for a given index from storage.
//
// This must place the data in |blob| that was provided by the last store
// operation  If there is evidence that no header data is present in the storage
// system, this function must return Status::kNotFound. For all other error
// conditions, implementations should return Status::kStorageError.
//
// It's OK if the data placed in |blob| exceeds the size of the original data,
// i.e. contain trailing bytes that haven't actually been written. This allows
// implementations to write at block granularity of the underlying storage
// system, which may be simpler instead of having to track sizes accurately.
Status LoadSpace(uint32_t index, Blob* blob);

// Write the NVRAM space data for the given index to storage.
//
// Implementations must atomically replace the current data with the data
// provided in |blob|. This must be robust against crashes, i.e. reloading the
// data after the crash should either return the previous version of the data,
// or the updated data provided in |blob|. In particular, crashes must not
// result in any form of data corruption or loss.
//
// It's OK for the implementation to allocate a larger storage area than
// required, i.e. not match blob.size() accurately. This allows implementations
// to perform I/O at block granularity of the underlying storage subsystem in
// case this simplifies things. There is no requirement as to the values of any
// additional trailing bytes.
//
// This function must make sure that the data actually hits disk before
// returning. The return value must be Status::kSuccess if and only if data was
// stored successfully, the function should return kStorageError otherwise.
Status StoreSpace(uint32_t index, const Blob& blob);

// Delete the stored NVRAM space data for the given index.
//
// This function must atomically delete the storage corresponding to the NVRAM
// space data associated with index. A subsequent read operation for the same
// index should return Status::kNotFound.
//
// This function must be robust in the event of crashes, i.e. a crash should
// leave the system with either the previously existing data still intact and
// accessible, or the space data entirely removed.
//
// This function must return Status::kSuccess upon deleting the space data
// successfully and having committed that operation to the underlying storage
// medium. Status::kNotFound should be returned in case the space data
// positively doesn't exist on the medium, and Status::kStorageError should be
// returned for all other error conditions.
Status DeleteSpace(uint32_t index);

}  // namespace storage
}  // namespace nvram

#endif  // NVRAM_CORE_STORAGE_H
