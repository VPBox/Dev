/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#pragma once

#include "gatt/database.h"

#include <utility>

namespace gatt {

class DatabaseBuilder {
 public:
  constexpr static std::pair<uint16_t, uint16_t> EXPLORE_END =
      std::make_pair(0xFFFF, 0xFFFF);

  void AddService(uint16_t handle, uint16_t end_handle,
                  const bluetooth::Uuid& uuid, bool is_primary);
  void AddIncludedService(uint16_t handle, const bluetooth::Uuid& uuid,
                          uint16_t start_handle, uint16_t end_handle);
  void AddCharacteristic(uint16_t handle, uint16_t value_handle,
                         const bluetooth::Uuid& uuid, uint8_t properties);
  void AddDescriptor(uint16_t handle, const bluetooth::Uuid& uuid);

  /* Returns true if next service exploration started, false if there are no
   * more services to explore. */
  bool StartNextServiceExploration();

  /* Return pair with start and end handle of the currently explored service.
   */
  const std::pair<uint16_t, uint16_t>& CurrentlyExploredService();

  /* Return pair with start and end handle of the descriptor range to discover,
   * or DatabaseBuilder::EXPLORE_END if no more descriptors left.
   */
  std::pair<uint16_t, uint16_t> NextDescriptorRangeToExplore();

  /* Returns true, if GATT discovery is in progress, false if discovery was not
   * started, or is already finished.
   */
  // TODO(jpawlowski): in the future, we might create this object only for the
  // time of discovery, in such case InProgress won't be needed, because object
  // existence will mean discovery is pending
  bool InProgress() const;

  /* Call this method at end of GATT discovery, to obtain object representing
   * the database of remote device */
  Database Build();

  void Clear();

  /* Return text representation of internal state for debugging purposes */
  std::string ToString() const;

 private:
  Database database;
  /* Start and end handle of service that is currently being discovered on the
   * remote device */
  std::pair<uint16_t, uint16_t> pending_service;
  /* Characteristic inside pending_service that is currently being explored */
  uint16_t pending_characteristic;

  /* sorted, unique set of start_handle, end_handle pair of all services that
   * have not yet been discovered */
  std::set<std::pair<uint16_t, uint16_t>> services_to_discover;
};

}  // namespace gatt
