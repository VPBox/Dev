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

#include "update_engine/payload_consumer/verity_writer_stub.h"

#include <memory>

namespace chromeos_update_engine {

namespace verity_writer {
std::unique_ptr<VerityWriterInterface> CreateVerityWriter() {
  return std::make_unique<VerityWriterStub>();
}
}  // namespace verity_writer

bool VerityWriterStub::Init(const InstallPlan::Partition& partition) {
  return partition.hash_tree_size == 0 && partition.fec_size == 0;
}

bool VerityWriterStub::Update(uint64_t offset,
                              const uint8_t* buffer,
                              size_t size) {
  return true;
}

}  // namespace chromeos_update_engine
