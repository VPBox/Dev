//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/client_library/include/update_engine/client.h"

#include <memory>

#if USE_BINDER
#include "update_engine/client_library/client_binder.h"
#else  // !USE_BINDER
#include "update_engine/client_library/client_dbus.h"
#endif  // USE_BINDER

using std::unique_ptr;

namespace update_engine {

unique_ptr<UpdateEngineClient> UpdateEngineClient::CreateInstance() {
#if USE_BINDER
  auto update_engine_client_impl = new internal::BinderUpdateEngineClient{};
#else   // !USE_BINDER
  auto update_engine_client_impl = new internal::DBusUpdateEngineClient{};
#endif  // USE_BINDER
  auto ret = unique_ptr<UpdateEngineClient>{update_engine_client_impl};

  if (!update_engine_client_impl->Init()) {
    ret.reset();
  }

  return ret;
}

}  // namespace update_engine
