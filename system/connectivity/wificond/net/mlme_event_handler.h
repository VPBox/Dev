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

#ifndef WIFICOND_NET_MLME_EVENT_HANDLER_H_
#define WIFICOND_NET_MLME_EVENT_HANDLER_H_

#include <functional>
#include <memory>

#include <wificond/net/mlme_event.h>

namespace android {
namespace wificond {

// Abstract class for handling mlme events.
class MlmeEventHandler {
 public:
  virtual ~MlmeEventHandler() {}

  virtual void OnConnect(std::unique_ptr<MlmeConnectEvent> event) = 0;
  virtual void OnRoam(const std::unique_ptr<MlmeRoamEvent> event) = 0;
  virtual void OnAssociate(std::unique_ptr<MlmeAssociateEvent> event) = 0;
  virtual void OnDisconnect(std::unique_ptr<MlmeDisconnectEvent> event) = 0;
  virtual void OnDisassociate(std::unique_ptr<MlmeDisassociateEvent> event) = 0;

};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_MLME_EVENT_HANDLER_H_
