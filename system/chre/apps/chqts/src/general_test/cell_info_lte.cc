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
#include <general_test/cell_info_lte.h>

namespace general_test {

bool CellInfoLte::validateIdentity(
    const struct chreWwanCellIdentityLte identity) {
  bool valid = false;

  if (!isBoundedInt32(identity.mcc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid LTE Mobile Country Code: %d", identity.mcc);
  } else if (!isBoundedInt32(identity.mnc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid LTE Mobile Network Code: %d", identity.mnc);
  } else if (!isBoundedInt32(identity.ci, 0, 268435455, INT32_MAX)) {
    sendFatalFailureInt32("Invalid LTE Cell Identity: %d", identity.ci);
  } else if (!isBoundedInt32(identity.pci, 0, 503, INT32_MAX)) {
    sendFatalFailureInt32("Invalid LTE Physical Cell Id: %d", identity.pci);
  } else if (!isBoundedInt32(identity.tac, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid LTE Tracking Area Code: %d", identity.tac);
  } else if (!isBoundedInt32(identity.earfcn, 0, 262144, INT32_MAX)) {
    sendFatalFailureInt32("Invalid LTE Absolute RF Channel Number: %d",
                          identity.earfcn);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoLte::validateSignalStrength(
    const struct chreWwanSignalStrengthLte strength) {
  bool valid = false;
  constexpr int32_t max = INT32_MAX;

  if (!isBoundedInt32(strength.signalStrength, 0, 31, 99)) {
    sendFatalFailureInt32("Invalid LTE Signal Strength: %d",
                          strength.signalStrength);
  } else if (!isBoundedInt32(strength.rsrp, 44, 140, max)) {
    sendFatalFailureInt32("Invalid LTE Reference Signal Receive Power: %d",
                          strength.rsrp);
  } else if (!isBoundedInt32(strength.rsrq, 3, 20, max)) {
    sendFatalFailureInt32(
        "Invalid LTE Reference Signal Receive Quality: %d",
        strength.rsrq);
  } else if (!isBoundedInt32(strength.rssnr, -200, 300, max)) {
    sendFatalFailureInt32(
        "Invalid LTE Reference Signal Signal-to-noise Ratio: %d",
        strength.rssnr);
  } else if (!isBoundedInt32(strength.cqi, 0, 15, max)) {
    sendFatalFailureInt32("Invalid LTE Channel Quality Indicator: %d",
                          strength.cqi);
  } else if (!isBoundedInt32(strength.timingAdvance, 0, max, max)) {
    sendFatalFailureInt32("Invalid LTE Timing Advance (ms): %d",
                          strength.timingAdvance);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoLte::validate(const struct chreWwanCellInfoLte& cell) {
  return (validateIdentity(cell.cellIdentityLte)
          && validateSignalStrength(cell.signalStrengthLte));
}

} // namespace general_test
