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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_RANDOM_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_RANDOM_PROVIDER_H_

#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// Provider of random values.
class RandomProvider : public Provider {
 public:
  ~RandomProvider() override {}

  // Return a random number every time it is requested. Note that values
  // returned by the variables are cached by the EvaluationContext, so the
  // returned value will be the same during the same policy request. If more
  // random values are needed use a PRNG seeded with this value.
  virtual Variable<uint64_t>* var_seed() = 0;

 protected:
  RandomProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(RandomProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_RANDOM_PROVIDER_H_
