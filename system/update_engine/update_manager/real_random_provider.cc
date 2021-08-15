//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/update_manager/real_random_provider.h"

#include <stdio.h>
#include <unistd.h>

#include <string>

#include <base/files/file_path.h>
#include <base/files/scoped_file.h>
#include <base/strings/stringprintf.h>

#include "update_engine/update_manager/variable.h"

using std::string;

namespace {

// The device providing randomness.
const char* kRandomDevice = "/dev/urandom";

}  // namespace

namespace chromeos_update_manager {

// A random seed variable.
class RandomSeedVariable : public Variable<uint64_t> {
 public:
  // RandomSeedVariable is initialized as kVariableModeConst to let the
  // EvaluationContext cache the value between different evaluations of the same
  // policy request.
  RandomSeedVariable(const string& name, FILE* fp)
      : Variable<uint64_t>(name, kVariableModeConst), fp_(fp) {}
  ~RandomSeedVariable() override {}

 protected:
  const uint64_t* GetValue(base::TimeDelta /* timeout */,
                           string* errmsg) override {
    uint64_t result;
    // Aliasing via char pointer abides by the C/C++ strict-aliasing rules.
    char* const buf = reinterpret_cast<char*>(&result);
    unsigned int buf_rd = 0;

    while (buf_rd < sizeof(result)) {
      int rd = fread(buf + buf_rd, 1, sizeof(result) - buf_rd, fp_.get());
      if (rd == 0 || ferror(fp_.get())) {
        // Either EOF on fp or read failed.
        if (errmsg) {
          *errmsg = base::StringPrintf(
              "Error reading from the random device: %s", kRandomDevice);
        }
        return nullptr;
      }
      buf_rd += rd;
    }

    return new uint64_t(result);
  }

 private:
  base::ScopedFILE fp_;

  DISALLOW_COPY_AND_ASSIGN(RandomSeedVariable);
};

bool RealRandomProvider::Init(void) {
  FILE* fp = fopen(kRandomDevice, "r");
  if (!fp)
    return false;
  var_seed_.reset(new RandomSeedVariable("seed", fp));
  return true;
}

}  // namespace chromeos_update_manager
