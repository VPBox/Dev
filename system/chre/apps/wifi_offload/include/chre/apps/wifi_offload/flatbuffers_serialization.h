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

#ifndef CHRE_WIFI_OFFLOAD_FLATBUFFERS_SERIALIZATION_H_
#define CHRE_WIFI_OFFLOAD_FLATBUFFERS_SERIALIZATION_H_

#include <cstring>

/**
 * @file
 * Serialize/deserialize API for messages passed between WifiOffload nanoapp
 * and Offload HAL.
 */

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/scan_config.h"
#include "chre/apps/wifi_offload/scan_result.h"
#include "chre/apps/wifi_offload/scan_result_message.h"
#include "chre/apps/wifi_offload/scan_stats.h"

namespace wifi_offload {
namespace fbs {

constexpr size_t kInitialFlatBufferSize = 256;

/**
 * Serializes the input object into a given buffer.
 *
 * @param stats/config/results object to be serialized
 * @param buffer Buffer to hold the result of serialization. Caller is
 *        responsible for allocating enough buffer size to hold the serialized
 *        data. If buffer is not large enough, serialization will abort and
 *        buffer will stay unmodified. If set to null, serialize function will
 *        return the required buffer size to hold the serialized data.
 * @param buffer_len Length of the buffer allocated by the caller
 *
 * @return zero if buffer is not big enough to hold the serialized data,
 *         otherwise size of serialized data in buffer.
 */
size_t Serialize(const wifi_offload::ScanStats &stats, uint8_t *buffer,
                 size_t buffer_len);
size_t Serialize(const wifi_offload::ScanConfig &config, uint8_t *buffer,
                 size_t buffer_len);
size_t Serialize(const wifi_offload::Vector<wifi_offload::ScanResult> &results,
                 uint8_t *buffer, size_t buffer_len);

/**
 * Deserializes from buffer into a given output object.
 *
 * @param buffer Buffer that holds the serialized data
 * @param buffer_len Length of buffer
 * @param stats/config/results Pointer to the output object to hold the result
 *        of deserialization
 *
 * @return true if deserialized successfully, false otherwise
 */
bool Deserialize(const uint8_t *buffer, size_t buffer_len,
                 wifi_offload::ScanStats *stats);
bool Deserialize(const uint8_t *buffer, size_t buffer_len,
                 wifi_offload::ScanConfig *config);
bool Deserialize(const uint8_t *buffer, size_t buffer_len,
                 wifi_offload::Vector<wifi_offload::ScanResult> *results);

template <typename SerializeType>
size_t Serialize(const SerializeType &obj, uint8_t *out_buffer,
                 size_t out_buffer_len, const char *log_tag = "") {
  flatbuffers::FlatBufferBuilder builder(kInitialFlatBufferSize);
  const auto fbs_obj = obj.Serialize(&builder);
  builder.Finish(fbs_obj);

  const uint8_t *data = builder.GetBufferPointer();
  const size_t size = builder.GetSize();

  if (out_buffer == nullptr) {
    LOGI("%s output buffer is null. Returning serialized size %zu.", log_tag,
         size);
    return size;
  }

  if (size > out_buffer_len) {
    LOGE("Serialized %s size %zu too big for provided buffer %zu; dropping",
         log_tag, size, out_buffer_len);
    return 0;
  }

  std::memcpy(out_buffer, data, size);
  LOGI("Serialized %s to buffer size %zu", log_tag, size);
  return size;
}

template <typename SerializeType>
bool Deserialize(const uint8_t *in_buffer, size_t in_buffer_len,
                 SerializeType *obj, const char *log_tag = "") {
  if (in_buffer == nullptr || in_buffer_len == 0) {
    LOGE("%s deserialize buffer is null or has size zero.", log_tag);
    return false;
  }

  if (obj == nullptr) {
    LOGE("%s deserialize output pointer is null.", log_tag);
    return false;
  }

  flatbuffers::Verifier verifier(in_buffer, in_buffer_len);
  if (!verifier.VerifyBuffer<typename SerializeType::FbsType>(nullptr)) {
    LOGE("Failed to verify %s deserialize buffer.", log_tag);
    return false;
  }

  const auto fbs_obj =
      flatbuffers::GetRoot<typename SerializeType::FbsType>(in_buffer);
  if (fbs_obj == nullptr) {
    LOGE("Deserialized %s object is null or has missing members.", log_tag);
    return false;
  }

  return obj->Deserialize(*fbs_obj);
}

}  //  namespace fbs
}  //  namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_FLATBUFFERS_SERIALIZATION_H_
