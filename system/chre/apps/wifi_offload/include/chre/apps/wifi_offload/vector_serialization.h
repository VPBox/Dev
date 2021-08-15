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

#ifndef CHRE_WIFI_OFFLOAD_WIFI_VECTOR_SERIALIZATION_H_
#define CHRE_WIFI_OFFLOAD_WIFI_VECTOR_SERIALIZATION_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"

namespace wifi_offload {

template <typename T>
flatbuffers::Offset<
    flatbuffers::Vector<flatbuffers::Offset<typename T::FbsType>>>
SerializeVector(const Vector<T> &native_vector,
                flatbuffers::FlatBufferBuilder *builder) {
  wifi_offload::Vector<flatbuffers::Offset<typename T::FbsType>> offset_vector;
  offset_vector.reserve(native_vector.size());
  for (const auto &elem : native_vector) {
    offset_vector.push_back(elem.Serialize(builder));
  }
  return builder->CreateVector(offset_vector);
}

template <typename T>
bool DeserializeVector(
    const flatbuffers::Vector<flatbuffers::Offset<typename T::FbsType>>
        &flatbuffer_vector,
    Vector<T> *native_vector) {
  if (native_vector == nullptr) {
    return false;
  }

  native_vector->clear();
  native_vector->reserve(flatbuffer_vector.size());
  for (const auto &flatbuffer_elem : flatbuffer_vector) {
    T native_elem;
    if (flatbuffer_elem && native_elem.Deserialize(*flatbuffer_elem)) {
      native_vector->push_back(std::move(native_elem));
    } else {
      return false;
    }
  }
  return true;
}

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_WIFI_VECTOR_SERIALIZATION_H_
