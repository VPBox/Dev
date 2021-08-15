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

#include "update_engine/update_manager/update_manager.h"

#ifdef __ANDROID__
#include "update_engine/update_manager/android_things_policy.h"
#else
#include "update_engine/update_manager/chromeos_policy.h"
#endif  // __ANDROID__
#include "update_engine/update_manager/state.h"

namespace chromeos_update_manager {

UpdateManager::UpdateManager(chromeos_update_engine::ClockInterface* clock,
                             base::TimeDelta evaluation_timeout,
                             base::TimeDelta expiration_timeout,
                             State* state)
    : default_policy_(clock),
      state_(state),
      clock_(clock),
      evaluation_timeout_(evaluation_timeout),
      expiration_timeout_(expiration_timeout),
      weak_ptr_factory_(this) {
#ifdef __ANDROID__
  policy_.reset(new AndroidThingsPolicy());
#else
  policy_.reset(new ChromeOSPolicy());
#endif  // __ANDROID__
}

UpdateManager::~UpdateManager() {
  // Remove pending main loop events associated with any of the outstanding
  // evaluation contexts. This will prevent dangling pending events, causing
  // these contexts to be destructed once the repo itself is destructed.
  for (auto& ec : ec_repo_)
    ec->RemoveObserversAndTimeout();
}

void UpdateManager::UnregisterEvalContext(EvaluationContext* ec) {
  if (!ec_repo_.erase(ec)) {
    LOG(ERROR) << "Unregistering an unknown evaluation context, this is a bug.";
  }
}

}  // namespace chromeos_update_manager
