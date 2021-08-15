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
#include <general_test/cell_info_tdscdma.h>

namespace general_test {

bool CellInfoTdscdma::validateIdentity(
    const struct chreWwanCellIdentityTdscdma identity) {
  bool valid = false;

  if (!isBoundedInt32(identity.mcc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid TDSCDMA Mobile Country Code: %d", identity.mcc);
  } else if (!isBoundedInt32(identity.mnc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid TDSCDMA Mobile Network Code: %d", identity.mnc);
  } else if (!isBoundedInt32(identity.lac, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid TDSCDMA Location Area Code: %d", identity.lac);
  } else if (!isBoundedInt32(identity.cid, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid TDSCDMA Cell Identity: %d", identity.cid);
  } else if (!isBoundedInt32(identity.cpid, 0, 127, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid TDSCDMA Cell Parameters ID: %d", identity.cpid);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoTdscdma::validateSignalStrength(
    const struct chreWwanSignalStrengthTdscdma strength) {
  bool valid = false;

  if (!isBoundedInt32(strength.rscp, 25, 120, INT32_MAX)) {
    sendFatalFailureInt32("Invalid TDSCDMA Received Signal Code Power: %d",
                          strength.rscp);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoTdscdma::validate(const struct chreWwanCellInfoTdscdma& cell) {
  return (validateIdentity(cell.cellIdentityTdscdma)
          && validateSignalStrength(cell.signalStrengthTdscdma));
}

} // namespace general_test
