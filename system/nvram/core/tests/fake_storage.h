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

#ifndef NVRAM_TEST_FAKE_STORAGE_H_
#define NVRAM_TEST_FAKE_STORAGE_H_

#include <nvram/core/storage.h>

namespace nvram {
namespace storage {

// Setup the header storage read functions to return Status::kStorageError.
void SetHeaderReadError(bool error);

// Setup the header storage write functions to return Status::kStorageError.
void SetHeaderWriteError(bool error);

// Setup the storage read calls for space |index| to return
// Status::kStorageError.
void SetSpaceReadError(uint32_t index, bool error);

// Setup the storage write calls for space |index| to return
// Status::kStorageError.
void SetSpaceWriteError(uint32_t index, bool error);

// Clears all storage.
void Clear();

}  // namespace storage
}  // namespace nvram

#endif  // NVRAM_TEST_FAKE_STORAGE_H_
