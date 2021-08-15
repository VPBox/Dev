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

#include "database.h"
#include "bt_trace.h"
#include "stack/include/gattdefs.h"

#include <base/logging.h>
#include <list>
#include <memory>
#include <sstream>

using bluetooth::Uuid;

namespace gatt {

namespace {
const Uuid PRIMARY_SERVICE = Uuid::From16Bit(GATT_UUID_PRI_SERVICE);
const Uuid SECONDARY_SERVICE = Uuid::From16Bit(GATT_UUID_SEC_SERVICE);
const Uuid INCLUDE = Uuid::From16Bit(GATT_UUID_INCLUDE_SERVICE);
const Uuid CHARACTERISTIC = Uuid::From16Bit(GATT_UUID_CHAR_DECLARE);

bool HandleInRange(const Service& svc, uint16_t handle) {
  return handle >= svc.handle && handle <= svc.end_handle;
}
}  // namespace

Service* FindService(std::list<Service>& services, uint16_t handle) {
  for (Service& service : services) {
    if (handle >= service.handle && handle <= service.end_handle)
      return &service;
  }

  return nullptr;
}

std::string Database::ToString() const {
  std::stringstream tmp;

  for (const Service& service : services) {
    tmp << "Service: handle=" << loghex(service.handle)
        << ", end_handle=" << loghex(service.end_handle)
        << ", uuid=" << service.uuid << "\n";

    for (const auto& is : service.included_services) {
      tmp << "\t Included service: handle=" << loghex(is.handle)
          << ", start_handle=" << loghex(is.start_handle)
          << ", end_handle=" << loghex(is.end_handle) << ", uuid=" << is.uuid
          << "\n";
    }

    for (const Characteristic& c : service.characteristics) {
      tmp << "\t Characteristic: declaration_handle="
          << loghex(c.declaration_handle)
          << ", value_handle=" << loghex(c.value_handle) << ", uuid=" << c.uuid
          << ", prop=" << loghex(c.properties) << "\n";

      for (const Descriptor& d : c.descriptors) {
        tmp << "\t\t Descriptor: handle=" << loghex(d.handle)
            << ", uuid=" << d.uuid << "\n";
      }
    }
  }
  return tmp.str();
}

std::vector<StoredAttribute> Database::Serialize() const {
  std::vector<StoredAttribute> nv_attr;

  if (services.empty()) return std::vector<StoredAttribute>();

  for (const Service& service : services) {
    // TODO: add constructor to NV_ATTR, use emplace_back
    nv_attr.push_back({service.handle,
                       service.is_primary ? PRIMARY_SERVICE : SECONDARY_SERVICE,
                       {.service = {.uuid = service.uuid,
                                    .end_handle = service.end_handle}}});
  }

  for (const Service& service : services) {
    for (const IncludedService& p_isvc : service.included_services) {
      nv_attr.push_back({p_isvc.handle,
                         INCLUDE,
                         {.included_service = {.handle = p_isvc.start_handle,
                                               .end_handle = p_isvc.end_handle,
                                               .uuid = p_isvc.uuid}}});
    }

    for (const Characteristic& charac : service.characteristics) {
      nv_attr.push_back(
          {charac.declaration_handle,
           CHARACTERISTIC,
           {.characteristic = {.properties = charac.properties,
                               .value_handle = charac.value_handle,
                               .uuid = charac.uuid}}});

      for (const Descriptor& desc : charac.descriptors) {
        nv_attr.push_back({desc.handle, desc.uuid, {}});
      }
    }
  }

  return nv_attr;
}

Database Database::Deserialize(const std::vector<StoredAttribute>& nv_attr,
                               bool* success) {
  // clear reallocating
  Database result;
  auto it = nv_attr.cbegin();

  for (; it != nv_attr.cend(); ++it) {
    const auto& attr = *it;
    if (attr.type != PRIMARY_SERVICE && attr.type != SECONDARY_SERVICE) break;
    result.services.emplace_back(
        Service{.handle = attr.handle,
                .end_handle = attr.value.service.end_handle,
                .is_primary = (attr.type == PRIMARY_SERVICE),
                .uuid = attr.value.service.uuid});
  }

  auto current_service_it = result.services.begin();
  for (; it != nv_attr.cend(); it++) {
    const auto& attr = *it;

    // go to the service this attribute belongs to; attributes are stored in
    // order, so iterating just forward is enough
    while (current_service_it != result.services.end() &&
           current_service_it->end_handle < attr.handle) {
      current_service_it++;
    }

    if (current_service_it == result.services.end() ||
        !HandleInRange(*current_service_it, attr.handle)) {
      LOG(ERROR) << "Can't find service for attribute with handle: "
                 << loghex(attr.handle);
      *success = false;
      return result;
    }

    if (attr.type == INCLUDE) {
      Service* included_service =
          FindService(result.services, attr.value.included_service.handle);
      if (!included_service) {
        LOG(ERROR) << __func__ << ": Non-existing included service!";
        *success = false;
        return result;
      }
      current_service_it->included_services.push_back(IncludedService{
          .handle = attr.handle,
          .uuid = attr.value.included_service.uuid,
          .start_handle = attr.value.included_service.handle,
          .end_handle = attr.value.included_service.end_handle,
      });
    } else if (attr.type == CHARACTERISTIC) {
      current_service_it->characteristics.emplace_back(
          Characteristic{.declaration_handle = attr.handle,
                         .value_handle = attr.value.characteristic.value_handle,
                         .properties = attr.value.characteristic.properties,
                         .uuid = attr.value.characteristic.uuid});

    } else {
      current_service_it->characteristics.back().descriptors.emplace_back(
          Descriptor{.handle = attr.handle, .uuid = attr.type});
    }
  }
  *success = true;
  return result;
}

}  // namespace gatt
