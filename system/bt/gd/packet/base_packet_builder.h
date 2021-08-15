/*
 * Copyright 2019 The Android Open Source Project
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

#include <cstdint>
#include <forward_list>
#include <iterator>
#include <memory>
#include <vector>

#include "packet/bit_inserter.h"

namespace bluetooth {
namespace packet {

// A little-endian PacketBuilder can contain a big-endian PacketBuilder,
// so BasePacketBuilder provides a common base class.
class BasePacketBuilder {
 public:
  virtual ~BasePacketBuilder() = default;

  virtual size_t size() const = 0;

  // Write to the vector with the given iterator.
  virtual void Serialize(BitInserter& it) const = 0;

 protected:
  BasePacketBuilder() = default;
};

}  // namespace packet
}  // namespace bluetooth
