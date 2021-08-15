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

#include "update_engine/update_manager/policy.h"

#include <string>

using std::string;

namespace chromeos_update_manager {

string ToString(EvalStatus status) {
  switch (status) {
    case EvalStatus::kFailed:
      return "kFailed";
    case EvalStatus::kSucceeded:
      return "kSucceeded";
    case EvalStatus::kAskMeAgainLater:
      return "kAskMeAgainLater";
    case EvalStatus::kContinue:
      return "kContinue";
  }
  return "Invalid";
}

}  // namespace chromeos_update_manager
