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

#include "wifilogd/memory_reader.h"

namespace android {
namespace wifilogd {

const uint8_t* MemoryReader::GetBytesOrDie(size_t n_bytes) {
  CHECK(head_);
  CHECK(n_bytes <= n_bytes_avail_);

  const uint8_t* old_head = head_;
  head_ += n_bytes;
  n_bytes_avail_ -= n_bytes;

  return old_head;
}

}  // namespace wifilogd
}  // namespace android
