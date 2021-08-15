//
// Copyright (C) 2010 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_MOCK_FILE_WRITER_H_
#define UPDATE_ENGINE_MOCK_FILE_WRITER_H_

#include <gmock/gmock.h>
#include "update_engine/payload_consumer/file_writer.h"

namespace chromeos_update_engine {

class MockFileWriter : public FileWriter {
 public:
  MOCK_METHOD2(Write, bool(const void* bytes, size_t count));
  MOCK_METHOD3(Write, bool(const void* bytes, size_t count, ErrorCode* error));
  MOCK_METHOD0(Close, int());
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_MOCK_FILE_WRITER_H_
