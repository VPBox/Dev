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

#include "chre/apps/wifi_offload/rpc_log_record.h"

namespace wifi_offload {

RpcLogRecord::RpcLogRecord()
    : record_type_(RpcLogRecordType::CMD_BASE), timestamp_chre_ms_(0) {}

bool RpcLogRecord::operator==(const RpcLogRecord &other) const {
  if (this == &other) {
    return true;
  }
  return record_type_ == other.record_type_ &&
         timestamp_chre_ms_ == other.timestamp_chre_ms_;
}

flatbuffers::Offset<RpcLogRecord::FbsType> RpcLogRecord::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  return fbs::CreateRpcLogRecord(*builder, static_cast<uint8_t>(record_type_),
                                 timestamp_chre_ms_);
}

bool RpcLogRecord::Deserialize(const RpcLogRecord::FbsType &fbs_record) {
  uint8_t rec = fbs_record.record_type();
  if ((rec > static_cast<uint8_t>(RpcLogRecordType::CMD_BASE) &&
       rec < static_cast<uint8_t>(RpcLogRecordType::CMD_LAST_ITEM)) ||
      (rec > static_cast<uint8_t>(RpcLogRecordType::EVENT_RECVD_BASE) &&
       rec < static_cast<uint8_t>(RpcLogRecordType::EVENT_RECVD_LAST_ITEM)) ||
      (rec > static_cast<uint8_t>(RpcLogRecordType::EVENT_SENT_BASE) &&
       rec < static_cast<uint8_t>(RpcLogRecordType::EVENT_SENT_LAST_ITEM)) ||
      (rec > static_cast<uint8_t>(RpcLogRecordType::REQ_BASE) &&
       rec < static_cast<uint8_t>(RpcLogRecordType::REQ_LAST_ITEM))) {
    record_type_ = static_cast<RpcLogRecordType>(fbs_record.record_type());
  } else {
    LOGE("Failed to deserialize RpcLogRecord. Invalid record type %" PRIu8,
         rec);
    return false;
  }

  timestamp_chre_ms_ = fbs_record.timestamp_chre_ms();
  return true;
}

}  // namespace wifi_offload
