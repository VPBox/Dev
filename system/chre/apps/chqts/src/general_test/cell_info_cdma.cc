/*
 * Copyright (C) 2017 The Android Open Source Project
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
#include <general_test/cell_info_cdma.h>

namespace general_test {

bool CellInfoCdma::validateIdentity(
    const struct chreWwanCellIdentityCdma identity) {
  bool valid = false;
  constexpr int32_t max = INT32_MAX;

  if (!isBoundedInt32(identity.networkId, 0, 65535, max)) {
    sendFatalFailureInt32(
        "Invalid CDMA Network Id: %d", identity.networkId);
  } else if (!isBoundedInt32(identity.systemId, 0, 32767, max)) {
    sendFatalFailureInt32(
        "Invalid CDMA System Id: %d", identity.systemId);
  } else if (!isBoundedInt32(identity.basestationId, 0, 65535, max)) {
    sendFatalFailureInt32("Invalid CDMA Base Station Id: %d",
                          identity.basestationId);
  } else if (!isBoundedInt32(identity.longitude, -2592000, 2592000, max)) {
    sendFatalFailureInt32("Invalid CDMA Longitude: %d", identity.longitude);
  } else if (!isBoundedInt32(identity.latitude, -1296000, 1296000, max)) {
    sendFatalFailureInt32("Invalid CDMA Latitude: %d", identity.latitude);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoCdma::validateSignalStrengthCdma(
    const struct chreWwanSignalStrengthCdma strength) {
  bool valid = false;

  // TODO: Find exact limits on dbm and ecio
  if ((strength.dbm < 0) || (strength.dbm > 160)) {
    sendFatalFailureInt32("Invalid CDMA/CDMA dbm: %d", strength.dbm);
  } else if ((strength.ecio < 0) || (strength.ecio > 1600)) {
    sendFatalFailureInt32("Invalid CDMA/CDMA ecio: %d", strength.ecio);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoCdma::validateSignalStrengthEvdo(
    const struct chreWwanSignalStrengthEvdo strength) {
  bool valid = false;

  // TODO: Find exact limits on dbm and ecio
  if ((strength.dbm < 0) || (strength.dbm > 160)) {
    sendFatalFailureInt32("Invalid CDMA/EVDO dbm: %d", strength.dbm);
  } else if ((strength.ecio < 0) || (strength.ecio > 1600)) {
    sendFatalFailureInt32("Invalid CDMA/EVDO ecio: %d", strength.ecio);
  } else if ((strength.signalNoiseRatio < 0)
             || (strength.signalNoiseRatio > 8)) {
    sendFatalFailureInt32("Invalid evdo signal noise ratio: %d",
                          strength.signalNoiseRatio);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoCdma::validate(const struct chreWwanCellInfoCdma& cell) {
  return (validateIdentity(cell.cellIdentityCdma)
          && validateSignalStrengthCdma(cell.signalStrengthCdma)
          && validateSignalStrengthEvdo(cell.signalStrengthEvdo));
}

} // namespace general_test
