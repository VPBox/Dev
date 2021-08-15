//
//  Copyright 2017, The Android Open Source Project
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

#include "android/os/parcel_uuid.h"

using android::OK;
using android::Parcel;
using android::status_t;
using ::bluetooth::Uuid;

namespace android {
namespace os {

namespace {
static uint64_t uuid_lsb(const Uuid& uuid) {
  uint64_t lsb = 0;

  auto uu = uuid.To128BitBE();
  for (int i = 8; i <= 15; i++) {
    lsb <<= 8;
    lsb |= uu[i];
  }

  return lsb;
}

static uint64_t uuid_msb(const Uuid& uuid) {
  uint64_t msb = 0;

  auto uu = uuid.To128BitBE();
  for (int i = 0; i <= 7; i++) {
    msb <<= 8;
    msb |= uu[i];
  }

  return msb;
}
}  // namespace

status_t ParcelUuid::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt64(uuid_msb(uuid));
  if (status != OK) return status;

  status = parcel->writeInt64(uuid_lsb(uuid));
  return status;
}

status_t ParcelUuid::readFromParcel(const Parcel* parcel) {
  int64_t uuid_msb, uuid_lsb;

  status_t status = parcel->readInt64(&uuid_msb);
  if (status != OK) return status;

  status = parcel->readInt64(&uuid_lsb);
  if (status != OK) return status;

  std::array<uint8_t, Uuid::kNumBytes128> uu;
  for (int i = 0; i < 8; i++) {
    uu[7 - i] = (uuid_msb >> (8 * i)) & 0xFF;
    uu[15 - i] = (uuid_lsb >> (8 * i)) & 0xFF;
  }

  uuid = Uuid::From128BitBE(uu);
  return OK;
}

}  // namespace os
}  // namespace android