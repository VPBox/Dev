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
#include <general_test/cell_info_wcdma.h>

namespace general_test {

bool CellInfoWcdma::validateIdentity(
    const struct chreWwanCellIdentityWcdma identity) {
  bool valid = false;

  if (!isBoundedInt32(identity.mcc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid WCDMA Mobile Country Code: %d", identity.mcc);
  } else if (!isBoundedInt32(identity.mnc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid WCDMA Mobile Network Code: %d", identity.mnc);
  } else if (!isBoundedInt32(identity.lac, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid WCDMA Location Area Code: %d", identity.lac);
  } else if (!isBoundedInt32(identity.cid, 0, 268435455, INT32_MAX)) {
    sendFatalFailureInt32("Invalid WCDMA Cell Identity: %d", identity.cid);
  } else if (!isBoundedInt32(identity.psc, 0, 511, INT32_MAX)) {
    sendFatalFailureInt32("Invalid WCDMA Primary Scrambling Code: %d",
                          identity.psc);
  } else if (!isBoundedInt32(identity.uarfcn, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32("Invalid WCDMA Absolute RF Channel Number: %d",
                          identity.uarfcn);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoWcdma::validateSignalStrength(
    const struct chreWwanSignalStrengthWcdma strength) {
  bool valid = false;

  if (!isBoundedInt32(strength.signalStrength, 0, 31, 99)) {
    sendFatalFailureInt32("Invalid WCDMA Signal Strength: %d",
                          strength.signalStrength);
  } else if (!isBoundedInt32(strength.bitErrorRate, 0, 7, 99)) {
    sendFatalFailureInt32("Invalid WCDMA Bit Error Rate: %d",
                          strength.bitErrorRate);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoWcdma::validate(const struct chreWwanCellInfoWcdma& cell) {
  return (validateIdentity(cell.cellIdentityWcdma)
          && validateSignalStrength(cell.signalStrengthWcdma));
}

} // namespace general_test
