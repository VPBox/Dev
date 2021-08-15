/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once

#include <array>
#include <variant>

#include "bluetooth/uuid.h"
#include "packet.h"
#include "sdp_common.h"
#include "stack/include/bt_types.h"

namespace bluetooth {
namespace sdp {

// A helper class that helps extract data element objects from SDP packets.
class DataElementReader {
 public:
  // If the DataElement contains monostate, that means parsing has failed.
  using DataElement =
      std::variant<std::monostate, bool, int8_t, int16_t, int32_t, int64_t,
                   uint8_t, uint16_t, uint32_t, uint64_t, Octet16, Uuid,
                   std::string, DataElementReader>;

  DataElementReader(Iterator begin, Iterator end) : it_(begin), end_(end){};

  // Get the next Data Element in the data. If reading fails for any reason,
  // the DataElementReader becomes invalid and will continuously fail to read
  // from that point onward.
  DataElement ReadNext();

 private:
  // Extraction Helpers
  DataElement ParseFail();
  template <class IntegerType>
  DataElement ReadInteger();
  DataElement ReadLargeInt();

  // Extraction Functions
  DataElement ReadSignedInt(DataElementSize size);
  DataElement ReadUnsignedInt(DataElementSize size);
  DataElement ReadUuid(DataElementSize size);
  DataElement ReadString(DataElementSize size);
  DataElement ReadSequence(DataElementSize size);

  Iterator it_;
  Iterator end_;
};

}  // namespace sdp
}  // namespace bluetooth
