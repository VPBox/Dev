/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef IORAP_BINDER_IIORAP_IMPL_H
#define IORAP_BINDER_IIORAP_IMPL_H

#include "binder/iiorap_def.h"
#include "common/macros.h"

#include "com/google/android/startop/iorap/BnIorap.h"

#include <memory>

namespace android {
template <typename Service>
class BinderService;
}

namespace iorap::manager {
  class EventManager;
};

namespace iorap {
namespace binder {

// Implementation of the IIorap service. This enables remote clients
// to connect to this process via the binder service manager.
//
// See also IIorap.aidl.
class IIorapImpl : public ::com::google::android::startop::iorap::BnIorap {
public:
  static bool Start(std::shared_ptr<iorap::manager::EventManager> event_manager);
  static constexpr const char* getServiceName() { return "iorapd"; };

// Join all parameter declarations by splitting each parameter with a comma.
// Types are used fully.
#define IIORAP_IMPL_ARGS(...) \
  IORAP_PP_MAP_SEP(IORAP_BINDER_PARAM_JOIN_ALL, IORAP_PP_COMMA, __VA_ARGS__)
#define IIORAP_IMPL_BODY(name, ...) \
  ::android::binder::Status name(IIORAP_IMPL_ARGS(__VA_ARGS__)) override;

IIORAP_IFACE_DEF(/*begin*/IORAP_PP_NOP, IIORAP_IMPL_BODY, /*end*/IORAP_PP_NOP);

#undef IIORAP_IMPL_BODY
#undef IIORAP_IMPL_ARGS

  class Impl;
private:
  IIorapImpl();  // open for BinderService::publish.

  friend class ::android::BinderService<IIorapImpl>;

  std::unique_ptr<Impl> impl_;
};

}
}


#endif //IORAP_BINDER_IIORAP_IMPL_H
