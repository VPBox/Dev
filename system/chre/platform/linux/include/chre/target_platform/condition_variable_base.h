/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_LINUX_CONDITION_VARIABLE_BASE_H_
#define CHRE_PLATFORM_LINUX_CONDITION_VARIABLE_BASE_H_

#include <condition_variable>

namespace chre {

/**
 * Storage for the Linux implementation of the condition variable.
 */
class ConditionVariableBase {
 protected:
  //! Defer to the std::condition_variable_any implementation.
  std::condition_variable_any mConditionVariable;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_CONDITION_VARIABLE_BASE_H_
