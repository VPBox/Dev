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

#include <nvram/messages/blob.h>

extern "C" {
#include <stdlib.h>
#include <string.h>
}

namespace nvram {

Blob::Blob() {}

Blob::~Blob() {
  free(data_);
  data_ = nullptr;
  size_ = 0;
}

Blob::Blob(Blob&& other) : Blob() {
  swap(*this, other);
}

Blob& Blob::operator=(Blob&& other) {
  swap(*this, other);
  return *this;
}

void swap(Blob& first, Blob& second) {
  // This does not use std::swap since it needs to work in environments that are
  // lacking a standard library.
  uint8_t* data_tmp = first.data_;
  size_t size_tmp = first.size_;
  first.data_ = second.data_;
  first.size_ = second.size_;
  second.data_ = data_tmp;
  second.size_ = size_tmp;
}

bool Blob::Assign(const void* data, size_t size) {
  free(data_);
  data_ = static_cast<uint8_t*>(malloc(size));
  if (!data_) {
    size_ = 0;
    return false;
  }
  memcpy(data_, data, size);
  size_ = size;
  return true;
}

bool Blob::Resize(size_t size) {
  uint8_t* tmp_data = static_cast<uint8_t*>(realloc(data_, size));
  if (size != 0 && !tmp_data) {
    return false;
  }

  data_ = tmp_data;
  size_ = size;
  return true;
}

}  // namespace nvram
