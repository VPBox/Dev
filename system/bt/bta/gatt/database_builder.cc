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

#include "database_builder.h"

#include "bt_trace.h"

#include <base/logging.h>
#include <algorithm>

using bluetooth::Uuid;

namespace gatt {

void DatabaseBuilder::AddService(uint16_t handle, uint16_t end_handle,
                                 const Uuid& uuid, bool is_primary) {
  // general case optimization - we add services in order
  if (database.services.empty() ||
      database.services.back().end_handle < handle) {
    database.services.emplace_back(Service{.handle = handle,
                                           .end_handle = end_handle,
                                           .is_primary = is_primary,
                                           .uuid = uuid});
  } else {
    auto& vec = database.services;

    // Find first service whose start handle is bigger than new service handle
    auto it = std::lower_bound(
        vec.begin(), vec.end(), handle,
        [](Service s, uint16_t handle) { return s.end_handle < handle; });

    // Insert new service just before it
    vec.emplace(it, Service{.handle = handle,
                            .end_handle = end_handle,
                            .is_primary = is_primary,
                            .uuid = uuid});
  }

  services_to_discover.insert({handle, end_handle});
}

void DatabaseBuilder::AddIncludedService(uint16_t handle, const Uuid& uuid,
                                         uint16_t start_handle,
                                         uint16_t end_handle) {
  Service* service = FindService(database.services, handle);
  if (!service) {
    LOG(ERROR) << "Illegal action to add to non-existing service!";
    return;
  }

  /* We discover all Primary Services first. If included service was not seen
   * before, it must be a Secondary Service */
  if (!FindService(database.services, start_handle)) {
    AddService(start_handle, end_handle, uuid, false /* not primary */);
  }

  service->included_services.push_back(IncludedService{
      .handle = handle,
      .uuid = uuid,
      .start_handle = start_handle,
      .end_handle = end_handle,
  });
}

void DatabaseBuilder::AddCharacteristic(uint16_t handle, uint16_t value_handle,
                                        const Uuid& uuid, uint8_t properties) {
  Service* service = FindService(database.services, handle);
  if (!service) {
    LOG(ERROR) << "Illegal action to add to non-existing service!";
    return;
  }

  if (service->end_handle < value_handle)
    LOG(WARNING) << "Remote device violates spec: value_handle="
                 << loghex(value_handle) << " is after service end_handle="
                 << loghex(service->end_handle);

  service->characteristics.emplace_back(
      Characteristic{.declaration_handle = handle,
                     .value_handle = value_handle,
                     .properties = properties,
                     .uuid = uuid});
  return;
}

void DatabaseBuilder::AddDescriptor(uint16_t handle, const Uuid& uuid) {
  Service* service = FindService(database.services, handle);
  if (!service) {
    LOG(ERROR) << "Illegal action to add to non-existing service!";
    return;
  }

  if (service->characteristics.empty()) {
    LOG(ERROR) << __func__
               << ": Illegal action to add to non-existing characteristic!";
    return;
  }

  Characteristic* char_node = &service->characteristics.front();
  for (auto it = service->characteristics.begin();
       it != service->characteristics.end(); it++) {
    if (it->declaration_handle > handle) break;
    char_node = &(*it);
  }

  char_node->descriptors.emplace_back(
      gatt::Descriptor{.handle = handle, .uuid = uuid});
}

bool DatabaseBuilder::StartNextServiceExploration() {
  while (!services_to_discover.empty()) {
    auto handle_range = services_to_discover.begin();
    pending_service = *handle_range;
    services_to_discover.erase(handle_range);

    // Empty service declaration, nothing to explore, skip to next.
    if (pending_service.first == pending_service.second) continue;

    pending_characteristic = HANDLE_MIN;
    return true;
  }
  return false;
}

const std::pair<uint16_t, uint16_t>&
DatabaseBuilder::CurrentlyExploredService() {
  return pending_service;
}

std::pair<uint16_t, uint16_t> DatabaseBuilder::NextDescriptorRangeToExplore() {
  Service* service = FindService(database.services, pending_service.first);
  if (!service || service->characteristics.empty()) {
    return {HANDLE_MAX, HANDLE_MAX};
  }

  for (auto it = service->characteristics.cbegin();
       it != service->characteristics.cend(); it++) {
    if (it->declaration_handle > pending_characteristic) {
      auto next = std::next(it);

      /* Characteristic Declaration is followed by Characteristic Value
       * Declaration, first descriptor is after that, see BT Spect 5.0 Vol 3,
       * Part G 3.3.2 and 3.3.3 */
      uint16_t start = it->declaration_handle + 2;
      uint16_t end;
      if (next != service->characteristics.end())
        end = next->declaration_handle - 1;
      else
        end = service->end_handle;

      // No place for descriptor - skip to next characteristic
      if (start > end) continue;

      pending_characteristic = start;
      return {start, end};
    }
  }

  pending_characteristic = HANDLE_MAX;
  return {HANDLE_MAX, HANDLE_MAX};
}

bool DatabaseBuilder::InProgress() const { return !database.services.empty(); }

Database DatabaseBuilder::Build() {
  Database tmp = database;
  database.Clear();
  return tmp;
}

void DatabaseBuilder::Clear() { database.Clear(); }

std::string DatabaseBuilder::ToString() const { return database.ToString(); }

}  // namespace gatt
