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
#include <general_test/wwan_cell_info_test.h>

#include <general_test/cell_info_base.h>
#include <general_test/cell_info_cdma.h>
#include <general_test/cell_info_gsm.h>
#include <general_test/cell_info_lte.h>
#include <general_test/cell_info_tdscdma.h>
#include <general_test/cell_info_wcdma.h>

#include <shared/send_message.h>

/*
 * General philosophy behind this test:
 *   Make a call to chreWwanGetCellInfoAsync and then ensure the following:
 *     1) Data is received within CHRE_ASYNC_RESULT_TIMEOUT_NS + small buffer.
 *     2) Various fields in the returned data are correct.
 */

namespace general_test {

WwanCellInfoTest::WwanCellInfoTest()
    : Test(CHRE_API_VERSION_1_1) {
}

void WwanCellInfoTest::setUp(uint32_t messageSize, const void * /* message */) {
  if ((chreWwanGetCapabilities() & CHRE_WWAN_GET_CELL_INFO) == 0) {
    sendMessageToHost(nanoapp_testing::MessageType::kSkipped);
  } else if (!chreWwanGetCellInfoAsync(&mTimerHandle)) {
    nanoapp_testing::sendFatalFailureToHost(
        "chreWwanGetCellInfo failed unexpectedly");
  } else {
    mTimerHandle = chreTimerSet(CHRE_ASYNC_RESULT_TIMEOUT_NS,
                                &mTimerHandle, true /* oneShot */);

    if (mTimerHandle == CHRE_TIMER_INVALID) {
      nanoapp_testing::sendFatalFailureToHost(
          "Unable to set timer for automatic failure");
    }
  }
}

WwanCellInfoTest::~WwanCellInfoTest() {
  // Ensure the timer is cancelled
  cancelTimer();
}

void WwanCellInfoTest::handleEvent(uint32_t senderInstanceId,
                                   uint16_t eventType,
                                   const void *eventData) {
  // The only expected message is from the async call
  if (senderInstanceId != CHRE_INSTANCE_ID) {
    nanoapp_testing::sendFatalFailureToHost(
        "handleEvent received event from unexpected sender:",
        &senderInstanceId);
  } else if (eventType == CHRE_EVENT_WWAN_CELL_INFO_RESULT) {
    cancelTimer();
    validateCellInfoResult(eventData);
  } else if (eventType == CHRE_EVENT_TIMER) {
    nanoapp_testing::sendFatalFailureToHost(
        "chreWwanGetCellInfo did not return data in time");
  } else {
    uint32_t type = eventType;
    nanoapp_testing::sendFatalFailureToHost(
        "handleEvent received an unexpected eventType:", &type);
  }
}

void WwanCellInfoTest::cancelTimer() {
  if (mTimerHandle != CHRE_TIMER_INVALID) {
    chreTimerCancel(mTimerHandle);
    mTimerHandle = CHRE_TIMER_INVALID;
  }
}

void WwanCellInfoTest::validateCellInfo(uint8_t count,
                                        const struct chreWwanCellInfo *cells) const {
  bool valid = true;

  for (int i = 0; (i < count) && valid; ++i) {
    if (cells[i].reserved != 0) {
      valid = false;
      CellInfoBase::sendFatalFailureUint8(
          "Invalid reserved CellInfo field: %d", cells[i].reserved);
    }

    for (uint8_t byte : cells[i].reserved2) {
      if (byte != 0) {
        valid = false;
        CellInfoBase::sendFatalFailureUint8(
            "Invalid reserved2 field: %d", byte);
      }
    }

    if ((cells[i].timeStampType != CHRE_WWAN_CELL_TIMESTAMP_TYPE_UNKNOWN)
        && (cells[i].timeStampType
            != CHRE_WWAN_CELL_TIMESTAMP_TYPE_ANTENNA)
        && (cells[i].timeStampType
            != CHRE_WWAN_CELL_TIMESTAMP_TYPE_MODEM)
        && (cells[i].timeStampType
            != CHRE_WWAN_CELL_TIMESTAMP_TYPE_OEM_RIL)
        && (cells[i].timeStampType
            != CHRE_WWAN_CELL_TIMESTAMP_TYPE_JAVA_RIL)) {
      valid = false;
      CellInfoBase::sendFatalFailureUint8(
          "Invalid timeStampType: %d", cells[i].timeStampType);
    }

    if (cells[i].cellInfoType == CHRE_WWAN_CELL_INFO_TYPE_GSM) {
      valid &= CellInfoGsm::validate(cells[i].CellInfo.gsm);
    } else if (cells[i].cellInfoType == CHRE_WWAN_CELL_INFO_TYPE_CDMA) {
      valid &= CellInfoCdma::validate(cells[i].CellInfo.cdma);
    } else if (cells[i].cellInfoType == CHRE_WWAN_CELL_INFO_TYPE_LTE) {
      valid &= CellInfoLte::validate(cells[i].CellInfo.lte);
    } else if (cells[i].cellInfoType == CHRE_WWAN_CELL_INFO_TYPE_WCDMA) {
      valid &= CellInfoWcdma::validate(cells[i].CellInfo.wcdma);
    } else if (cells[i].cellInfoType == CHRE_WWAN_CELL_INFO_TYPE_TD_SCDMA) {
      valid &= CellInfoTdscdma::validate(cells[i].CellInfo.tdscdma);
    } else {
      valid = false;
      CellInfoBase::sendFatalFailureUint8(
          "Invalid cellInfoType: %d", cells[i].cellInfoType);
    }
  }

  if (valid) {
    nanoapp_testing::sendSuccessToHost();
  }
}

void WwanCellInfoTest::validateCellInfoResult(const void *eventData) const {
  const struct chreWwanCellInfoResult *result =
      static_cast<const chreWwanCellInfoResult *>(eventData);

  if (eventData == nullptr) {
    nanoapp_testing::sendFatalFailureToHost("Received eventData is null");
  } else if (result->version != CHRE_WWAN_CELL_INFO_RESULT_VERSION) {
    nanoapp_testing::sendFatalFailureToHost(
        "Received version is unexpected value");
  } else if (result->reserved != 0) {
    nanoapp_testing::sendFatalFailureToHost(
        "Received reserved field non-zero");
  } else {
    const uint32_t *receivedCookie =
        static_cast<const uint32_t *>(result->cookie);

    if (receivedCookie != &mTimerHandle) {
      nanoapp_testing::sendFatalFailureToHost(
          "Received cookie does not match");
    } else if (result->cellInfoCount != 0) {
      validateCellInfo(result->cellInfoCount, result->cells);
    } else {
      nanoapp_testing::sendSuccessToHost();
    }
  }
}

} // namespace general_test
