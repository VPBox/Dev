/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef IORAP_MANAGER_EVENT_MANAGER_H_
#define IORAP_MANAGER_EVENT_MANAGER_H_

#include "binder/app_launch_event.h"
#include "binder/request_id.h"

#include <memory>

namespace iorap::perfetto {
  struct RxProducerFactory;
}  // namespace iorap::perfetto

namespace iorap::manager {

class EventManager {
 public:
  static std::shared_ptr<EventManager> Create();
  static std::shared_ptr<EventManager> Create(
      /*borrow*/perfetto::RxProducerFactory& perfetto_factory);

  // Handles an AppLaunchEvent:
  //
  // * Intent starts and app launch starts are treated critically
  //   and will be handled immediately. This means the caller
  //   (e.g. the binder pool thread) could be starved in the name
  //   of low latency.
  //
  // * Other types are handled in a separate thread.
  bool OnAppLaunchEvent(binder::RequestId request_id,
                        const binder::AppLaunchEvent& event);

  class Impl;
 private:
  std::unique_ptr<Impl> impl_;

  EventManager(perfetto::RxProducerFactory& perfetto_factory);
};

}  // namespace iorap::manager

#endif  // IORAP_MANAGER_EVENT_MANAGER_H_
