//
// Copyright (C) 2012 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_CLIENT_LIBRARY_INCLUDE_UPDATE_ENGINE_STATUS_UPDATE_HANDLER_H_
#define UPDATE_ENGINE_CLIENT_LIBRARY_INCLUDE_UPDATE_ENGINE_STATUS_UPDATE_HANDLER_H_

#include <string>

#include "update_engine/client.h"
#include "update_engine/update_status.h"

namespace update_engine {

// Handles update_engine status changes. An instance of this class can be
// registered with UpdateEngineClient and will respond to any update_engine
// status changes.
class StatusUpdateHandler {
 public:
  virtual ~StatusUpdateHandler() = default;

  // Runs when we fail to register the handler due to an IPC error.
  virtual void IPCError(const std::string& error) = 0;

  // Runs every time update_engine reports a status change.
  virtual void HandleStatusUpdate(int64_t last_checked_time,
                                  double progress,
                                  UpdateStatus current_operation,
                                  const std::string& new_version,
                                  int64_t new_size) = 0;
};

}  // namespace update_engine

#endif  // UPDATE_ENGINE_CLIENT_LIBRARY_INCLUDE_UPDATE_ENGINE_STATUS_UPDATE_HANDLER_H_
