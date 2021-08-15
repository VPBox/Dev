//
//  Copyright 2016 Google, Inc.
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

#include "android/bluetooth/uuid.h"

#include <binder/Parcel.h>

using android::Parcelable;
using android::Parcel;
using android::String16;
using android::status_t;
using android::OK;

namespace android {
namespace bluetooth {

status_t UUID::writeToParcel(Parcel* parcel) const {
  // The scheme used by android.os.ParcelUuid is to wrote the most significant
  // bits first as one 64-bit integer, followed by the least significant bits in
  // a second 64-bit integer. This is the same as writing the raw-bytes in
  // sequence, but we don't want to assume any host-endianness here. So follow
  // the same scheme and use the same Parcel APIs.
  ::bluetooth::Uuid::UUID128Bit bytes = uuid.To128BitBE();

  uint64_t most_sig_bits =
      ((((uint64_t)bytes[0]) << 56) | (((uint64_t)bytes[1]) << 48) |
       (((uint64_t)bytes[2]) << 40) | (((uint64_t)bytes[3]) << 32) |
       (((uint64_t)bytes[4]) << 24) | (((uint64_t)bytes[5]) << 16) |
       (((uint64_t)bytes[6]) << 8) | bytes[7]);

  uint64_t least_sig_bits =
      ((((uint64_t)bytes[8]) << 56) | (((uint64_t)bytes[9]) << 48) |
       (((uint64_t)bytes[10]) << 40) | (((uint64_t)bytes[11]) << 32) |
       (((uint64_t)bytes[12]) << 24) | (((uint64_t)bytes[13]) << 16) |
       (((uint64_t)bytes[14]) << 8) | bytes[15]);

  status_t status = parcel->writeUint64(most_sig_bits);
  if (status != OK) return status;

  status = parcel->writeUint64(least_sig_bits);
  return status;
}

status_t UUID::readFromParcel(const Parcel* parcel) {
  ::bluetooth::Uuid::UUID128Bit bytes;

  uint64_t most_sig_bits, least_sig_bits;
  status_t status = parcel->readUint64(&most_sig_bits);
  if (status != OK) return status;

  status = parcel->readUint64(&least_sig_bits);
  if (status != OK) return status;

  bytes[0] = (most_sig_bits >> 56) & 0xFF;
  bytes[1] = (most_sig_bits >> 48) & 0xFF;
  bytes[2] = (most_sig_bits >> 40) & 0xFF;
  bytes[3] = (most_sig_bits >> 32) & 0xFF;
  bytes[4] = (most_sig_bits >> 24) & 0xFF;
  bytes[5] = (most_sig_bits >> 16) & 0xFF;
  bytes[6] = (most_sig_bits >> 8) & 0xFF;
  bytes[7] = most_sig_bits & 0xFF;

  bytes[8] = (least_sig_bits >> 56) & 0xFF;
  bytes[9] = (least_sig_bits >> 48) & 0xFF;
  bytes[10] = (least_sig_bits >> 40) & 0xFF;
  bytes[11] = (least_sig_bits >> 32) & 0xFF;
  bytes[12] = (least_sig_bits >> 24) & 0xFF;
  bytes[13] = (least_sig_bits >> 16) & 0xFF;
  bytes[14] = (least_sig_bits >> 8) & 0xFF;
  bytes[15] = least_sig_bits & 0xFF;

  uuid = ::bluetooth::Uuid::From128BitBE(bytes);
  return status;
}

}  // namespace bluetooth
}  // namespace android
