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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_EVALUATION_CONTEXT_INL_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_EVALUATION_CONTEXT_INL_H_

#include <string>

#include <base/logging.h>

namespace chromeos_update_manager {

template <typename T>
const T* EvaluationContext::GetValue(Variable<T>* var) {
  if (var == nullptr) {
    LOG(ERROR) << "GetValue received an uninitialized variable.";
    return nullptr;
  }

  // Search for the value on the cache first.
  ValueCacheMap::iterator it = value_cache_.find(var);
  if (it != value_cache_.end())
    return reinterpret_cast<const T*>(it->second.value());

  // Get the value from the variable if not found on the cache.
  std::string errmsg;
  const T* result =
      var->GetValue(RemainingTime(evaluation_monotonic_deadline_), &errmsg);
  if (result == nullptr) {
    LOG(WARNING) << "Error reading Variable " << var->GetName() << ": \""
                 << errmsg << "\"";
  }
  // Cache the value for the next time. The map of CachedValues keeps the
  // ownership of the pointer until the map is destroyed.
  value_cache_.emplace(static_cast<BaseVariable*>(var), BoxedValue(result));
  return result;
}

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_EVALUATION_CONTEXT_INL_H_
