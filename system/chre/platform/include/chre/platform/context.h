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

#ifndef CHRE_PLATFORM_CONTEXT_H_
#define CHRE_PLATFORM_CONTEXT_H_

namespace chre {

// TODO: Consider removing this. The event loop should be interacted with by
// posting an event to the EventLoop thread to avoid synchronization issues.
/**
 * @return true to indicate that the current thread is the thread that is
 * currently blocked by the event loop. This is used by the event loop to
 * determine whether it needs to lock shared data structures or not.
 */
bool inEventLoopThread();

}  // namespace chre

#endif //CHRE_CONTEXT_H
