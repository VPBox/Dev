//
// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_VERITY_WRITER_INTERFACE_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_VERITY_WRITER_INTERFACE_H_

#include <cstdint>
#include <memory>

#include <base/macros.h>

#include "update_engine/payload_consumer/install_plan.h"

namespace chromeos_update_engine {

class VerityWriterInterface {
 public:
  virtual ~VerityWriterInterface() = default;

  virtual bool Init(const InstallPlan::Partition& partition) = 0;
  // Update partition data at [offset : offset + size) stored in |buffer|.
  // Data not in |hash_tree_data_extent| or |fec_data_extent| is ignored.
  // Will write verity data to the target partition once all the necessary
  // blocks has passed.
  virtual bool Update(uint64_t offset, const uint8_t* buffer, size_t size) = 0;

 protected:
  VerityWriterInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(VerityWriterInterface);
};

namespace verity_writer {
std::unique_ptr<VerityWriterInterface> CreateVerityWriter();
}

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_VERITY_WRITER_INTERFACE_H_
