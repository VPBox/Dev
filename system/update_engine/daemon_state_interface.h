//
// Copyright (C) 2016 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_DAEMON_STATE_INTERFACE_H_
#define UPDATE_ENGINE_DAEMON_STATE_INTERFACE_H_

#include "update_engine/service_observer_interface.h"

#include <memory>
#include <set>

namespace chromeos_update_engine {

class DaemonStateInterface {
 public:
  virtual ~DaemonStateInterface() = default;

  // Start the daemon loop. Should be called only once to start the daemon's
  // main functionality.
  virtual bool StartUpdater() = 0;

  // Add and remove an observer. All the registered observers will be called
  // whenever there's a new status to update.
  virtual void AddObserver(ServiceObserverInterface* observer) = 0;
  virtual void RemoveObserver(ServiceObserverInterface* observer) = 0;

  // Return the set of current observers.
  virtual const std::set<ServiceObserverInterface*>& service_observers() = 0;

 protected:
  DaemonStateInterface() = default;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_DAEMON_STATE_INTERFACE_H_
