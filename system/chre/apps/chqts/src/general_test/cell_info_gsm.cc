/**
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
#include <general_test/cell_info_gsm.h>

namespace general_test {

bool CellInfoGsm::validateIdentity(
    const struct chreWwanCellIdentityGsm identity) {
  bool valid = false;

  if (!isBoundedInt32(identity.mcc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid GSM Mobile Country Code: %d", identity.mcc);
  } else if (!isBoundedInt32(identity.mnc, 0, 999, INT32_MAX)) {
    sendFatalFailureInt32(
        "Invalid GSM Mobile Network Code: %d", identity.mnc);
  } else if (!isBoundedInt32(identity.lac, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32("Invalid GSM Location Area Code", identity.lac);
  } else if (!isBoundedInt32(identity.cid, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32("Invalid GSM Cell Identity: %d", identity.cid);
  } else if (!isBoundedInt32(identity.arfcn, 0, 65535, INT32_MAX)) {
    sendFatalFailureInt32("Invalid GSM Absolute RF Channel Number: %d",
                          identity.arfcn);
  } else if ((identity.bsic > 63) && (identity.bsic != UINT8_MAX)) {
    sendFatalFailureUint8("Invalid GSM Base Station Identity Code: %d",
                          identity.bsic);
  } else {
    valid = true;

    for (uint8_t byte : identity.reserved) {
      if (byte != 0) {
        valid = false;
        sendFatalFailureUint8("Invalid GSM reserved byte: %d",
                              byte);
      }

      if (!valid) {
        break;
      }
    }
  }

  return valid;
}

bool CellInfoGsm::validateSignalStrength(
    const struct chreWwanSignalStrengthGsm strength) {
  bool valid = false;

  if (!isBoundedInt32(strength.signalStrength, 0, 31, 99)) {
    sendFatalFailureInt32("Invalid GSM signal strength: %d",
                          strength.signalStrength);
  } else if (!isBoundedInt32(strength.bitErrorRate, 0, 7, 99)) {
    sendFatalFailureInt32("Invalid GSM bit error rate: %d",
                          strength.bitErrorRate);
  } else {
    valid = true;
  }

  return valid;
}

bool CellInfoGsm::validate(const struct chreWwanCellInfoGsm& cell) {
  return (validateIdentity(cell.cellIdentityGsm)
          && validateSignalStrength(cell.signalStrengthGsm));
}

} // namespace general_test
