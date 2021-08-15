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

#ifndef WIFICOND_TESTS_INTEGRATION_TEST_BINDER_DISPATCHER_H_
#define WIFICOND_TESTS_INTEGRATION_TEST_BINDER_DISPATCHER_H_

#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#include "looper_backed_event_loop.h"

namespace android {
namespace wificond {
namespace tests {
namespace integration {

// Class to use for waiting for binder callbacks during integration tests.
//
// The class provides function to:
// 1. |DispatchFor|: Process binder events for the provided timeout_millis or
//                   until interrupted by |InterruptDispatch|.
// 2. |InterruptDispatch|: Indicate the occurence of the required event.
//
// The |DispatchFor| is meant to be called from within the tests, while the
// |InterruptDispatch| is expected to be called from the appropriate binder
// object callback.
class BinderDispatcher {
 public:
  BinderDispatcher();
  ~BinderDispatcher();
  // Dispatch binder events for |timeout_millis| or until interrupted by
  // a call to |InterruptDispatch|.
  // Returns true if interrupted, false otherwise.
  bool DispatchFor(int timeout_millis);
  // Interrupt the |DispatchFor| method when the required event is found.
  void InterruptDispatch();

 private:
  // Prepare the thread for receiving binder events and setup the looper.
  void Init();
  void OnBinderEvent(int fd);

  std::unique_ptr<LooperBackedEventLoop> event_dispatcher_;
  // This fd is reinitialized for every test, by tearing down the static
  // |ProcessState| instance associated with this process after every test.
  int binder_fd_;
  bool needs_init_;
  bool was_interrupted_;

  DISALLOW_COPY_AND_ASSIGN(BinderDispatcher);
};

}  // namespace integration
}  // namespace tests
}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TESTS_INTEGRATION_TEST_BINDER_DISPATCHER_H_
