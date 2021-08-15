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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_REAL_RANDOM_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_REAL_RANDOM_PROVIDER_H_

#include <memory>

#include "update_engine/update_manager/random_provider.h"

namespace chromeos_update_manager {

// RandomProvider implementation class.
class RealRandomProvider : public RandomProvider {
 public:
  RealRandomProvider() {}

  Variable<uint64_t>* var_seed() override { return var_seed_.get(); }

  // Initializes the provider and returns whether it succeeded.
  bool Init();

 private:
  // The seed() scoped variable.
  std::unique_ptr<Variable<uint64_t>> var_seed_;

  DISALLOW_COPY_AND_ASSIGN(RealRandomProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_REAL_RANDOM_PROVIDER_H_
