//
//  Copyright 2015 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "bluetooth/scan_filter.h"

#include <raw_address.h>

namespace bluetooth {

ScanFilter::ScanFilter(const ScanFilter& other) {
  device_name_ = other.device_name_;
  device_address_ = other.device_address_;

  if (other.service_uuid_) service_uuid_.reset(new Uuid(*other.service_uuid_));

  if (other.service_uuid_mask_)
    service_uuid_mask_.reset(new Uuid(*other.service_uuid_mask_));
}

ScanFilter& ScanFilter::operator=(const ScanFilter& other) {
  device_name_ = other.device_name_;
  device_address_ = other.device_address_;

  if (other.service_uuid_)
    service_uuid_.reset(new Uuid(*other.service_uuid_));
  else
    service_uuid_ = nullptr;

  if (other.service_uuid_mask_)
    service_uuid_mask_.reset(new Uuid(*other.service_uuid_mask_));
  else
    service_uuid_mask_ = nullptr;

  return *this;
}

bool ScanFilter::SetDeviceAddress(const std::string& device_address) {
  if (!RawAddress::IsValidAddress(device_address)) return false;

  device_address_ = device_address;
  return true;
}

void ScanFilter::SetServiceUuid(const Uuid& service_uuid) {
  service_uuid_.reset(new Uuid(service_uuid));
  service_uuid_mask_.reset();
}

void ScanFilter::SetServiceUuidWithMask(const Uuid& service_uuid,
                                        const Uuid& mask) {
  service_uuid_.reset(new Uuid(service_uuid));
  service_uuid_mask_.reset(new Uuid(mask));
}

bool ScanFilter::operator==(const ScanFilter& rhs) const {
  if (device_name_ != rhs.device_name_) return false;

  if (device_address_ != rhs.device_address_) return false;

  // Both must be either NULL or non-NULL. If only one of them is NULL, then
  // return false.
  if (!!service_uuid_ != !!rhs.service_uuid_) return false;

  if (service_uuid_ && rhs.service_uuid_ &&
      *service_uuid_ != *rhs.service_uuid_)
    return false;

  // Both must be either NULL or non-NULL. If only one of them is NULL, then
  // return false.
  if (!!service_uuid_mask_ != !!rhs.service_uuid_mask_) return false;

  if (service_uuid_mask_ && rhs.service_uuid_mask_ &&
      *service_uuid_mask_ != *rhs.service_uuid_mask_)
    return false;

  return true;
}

}  // namespace bluetooth
