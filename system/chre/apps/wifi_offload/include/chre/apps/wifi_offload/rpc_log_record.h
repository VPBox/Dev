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

#ifndef CHRE_WIFI_OFFLOAD_RPC_LOG_RECORD_H_
#define CHRE_WIFI_OFFLOAD_RPC_LOG_RECORD_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"

namespace wifi_offload {

class RpcLogRecord {
 public:
  /**
   * Enumerates the type of log that is recorded
   */
  enum class RpcLogRecordType : uint8_t {
    CMD_BASE = 0x00,
    CMD_INIT,
    CMD_CONFIG_SCANS,
    CMD_SUBSCRIBE_SCAN_RESULTS,
    CMD_UNSUBSCRIBE_SCAN_RESULTS,
    CMD_GET_SCAN_STATS,
    CMD_RESET,
    CMD_LAST_ITEM,

    EVENT_RECVD_BASE = 0x40,
    EVENT_RECVD_SCAN_RESULT_ASYNC,
    EVENT_RECVD_SCAN_RESULT,
    EVENT_RECVD_LAST_ITEM,

    EVENT_SENT_BASE = 0x80,
    EVENT_SENT_SCAN_RESULT,
    EVENT_SENT_ABORT,
    EVENT_SENT_ERROR,
    EVENT_SENT_LAST_ITEM,

    REQ_BASE = 0xc0,
    REQ_SCAN,
    REQ_LAST_ITEM,
  };

  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::RpcLogRecord;

  RpcLogRecord();
  ~RpcLogRecord() = default;

  bool operator==(const RpcLogRecord &other) const;

  flatbuffers::Offset<RpcLogRecord::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const RpcLogRecord::FbsType &fbs_record);

  RpcLogRecordType record_type_;
  uint32_t timestamp_chre_ms_;  // See chreGetTime()
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_RPC_LOG_RECORD_H_
