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

#include <chre.h>
#include <cinttypes>

#include "chre/util/nanoapp/log.h"
#include "chre/util/time.h"

#define LOG_TAG "[WwanWorld]"

#ifdef CHRE_NANOAPP_INTERNAL
namespace chre {
namespace {
#endif  // CHRE_NANOAPP_INTERNAL

//! A dummy cookie to pass into the cell info request.
const uint32_t kCellInfoCookie = 0x1337;

//! The interval for cell info requests.
const Nanoseconds kCellInfoInterval = Nanoseconds(Seconds(10));

//! A handle for  the cyclic timer to request periodic cell info.
uint32_t gCellInfoTimerHandle;

namespace {

/**
 * Handles a timer event.
 *
 * @param eventData The cookie passed to the timer request.
 */
void handleTimerEvent(const void *eventData) {
  const uint32_t *timerHandle = static_cast<const uint32_t *>(eventData);
  if (*timerHandle == gCellInfoTimerHandle) {
    if (chreWwanGetCellInfoAsync(&kCellInfoCookie)) {
      LOGI("Requested cell info successfully");
    } else {
      LOGE("Failed to request cell info");
    }
  } else {
    LOGE("Received invalid timer handle");
  }
}

/**
 * Logs a CHRE WWAN cell info result.
 *
 * @param cell the cell info to log.
 */
void logChreWwanInfo(const chreWwanCellInfo *cell) {
  LOGI("Found cell at time %" PRIu64, cell->timeStamp);
  LOGI("  timestamp type %" PRIu8, cell->timeStampType);
  LOGI("  registered %" PRIu8, cell->registered);

  switch (cell->cellInfoType) {
    case CHRE_WWAN_CELL_INFO_TYPE_LTE:
      LOGI("  LTE cell detected");
      LOGI("    mcc %" PRId32, cell->CellInfo.lte.cellIdentityLte.mcc);
      LOGI("    mnc %" PRId32, cell->CellInfo.lte.cellIdentityLte.mnc);
      LOGI("    ci %" PRId32, cell->CellInfo.lte.cellIdentityLte.ci);
      LOGI("    pci %" PRId32, cell->CellInfo.lte.cellIdentityLte.pci);
      LOGI("    tac %" PRId32, cell->CellInfo.lte.cellIdentityLte.tac);
      LOGI("    earfcn %" PRId32, cell->CellInfo.lte.cellIdentityLte.earfcn);
      break;
    case CHRE_WWAN_CELL_INFO_TYPE_GSM:
      LOGI("  GSM cell detected");
      LOGI("    mcc %" PRId32, cell->CellInfo.gsm.cellIdentityGsm.mcc);
      LOGI("    mnc %" PRId32, cell->CellInfo.gsm.cellIdentityGsm.mnc);
      LOGI("    lac %" PRId32, cell->CellInfo.gsm.cellIdentityGsm.lac);
      LOGI("    cid %" PRId32, cell->CellInfo.gsm.cellIdentityGsm.cid);
      LOGI("    arfcn %" PRId32, cell->CellInfo.gsm.cellIdentityGsm.arfcn);
      LOGI("    bsic %" PRIu8, cell->CellInfo.gsm.cellIdentityGsm.bsic);
      break;
    case CHRE_WWAN_CELL_INFO_TYPE_WCDMA:
      LOGI("  WCDMA cell detected");
      LOGI("    mcc %" PRId32, cell->CellInfo.wcdma.cellIdentityWcdma.mcc);
      LOGI("    mnc %" PRId32, cell->CellInfo.wcdma.cellIdentityWcdma.mnc);
      LOGI("    lac %" PRId32, cell->CellInfo.wcdma.cellIdentityWcdma.lac);
      LOGI("    cid %" PRId32, cell->CellInfo.wcdma.cellIdentityWcdma.cid);
      LOGI("    psc %" PRId32, cell->CellInfo.wcdma.cellIdentityWcdma.psc);
      LOGI("    uarfcn %" PRId32, cell->CellInfo.wcdma.cellIdentityWcdma.uarfcn);
      break;
    default:
      // TODO: Support logging all cell types.
      LOGI("  unsupported cell info %" PRIu8, cell->cellInfoType);
      break;
  };
}

/**
 * Handles a WWAN cell info result.
 *
 * @param result a WWAN cell info result.
 */
void handleCellInfoResult(const chreWwanCellInfoResult *result) {
  if (result->errorCode != CHRE_ERROR_NONE) {
    LOGE("Failed to request WWAN cell info with %" PRIu8, result->errorCode);
  } else {
    LOGD("Received cell info result with version %" PRIu8, result->version);

    for (uint8_t i = 0; i < result->cellInfoCount; i++) {
      logChreWwanInfo(&result->cells[i]);
    }
  }
}

}  // namespace


bool nanoappStart() {
  LOGI("App started as instance %" PRIu32, chreGetInstanceId());

  const char *wwanCapabilitiesStr;
  uint32_t wwanCapabilities = chreWwanGetCapabilities();
  switch (wwanCapabilities) {
    case CHRE_WWAN_GET_CELL_INFO:
      wwanCapabilitiesStr = "GET_CELL_INFO";
      break;
    case CHRE_WWAN_CAPABILITIES_NONE:
      wwanCapabilitiesStr = "NONE";
      break;
    default:
      wwanCapabilitiesStr = "INVALID";
  }

  LOGI("Detected WWAN support as: %s (%" PRIu32 ")",
       wwanCapabilitiesStr, wwanCapabilities);

  if (wwanCapabilities & CHRE_WWAN_GET_CELL_INFO) {
    gCellInfoTimerHandle = chreTimerSet(kCellInfoInterval.toRawNanoseconds(),
                                        &gCellInfoTimerHandle /* data */,
                                        false /* oneShot */);
    if (gCellInfoTimerHandle == CHRE_TIMER_INVALID) {
      LOGE("Failed to set a periodic cell info timer");
    } else {
      LOGI("Set a timer to request periodic cell info");
    }
  }

  return true;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  switch (eventType) {
    case CHRE_EVENT_TIMER:
      handleTimerEvent(eventData);
      break;
    case CHRE_EVENT_WWAN_CELL_INFO_RESULT:
      handleCellInfoResult(
          static_cast<const chreWwanCellInfoResult *>(eventData));
      break;
    default:
      LOGW("Unhandled event type %" PRIu16, eventType);
  }
}

void nanoappEnd() {
  LOGI("Stopped");
}

#ifdef CHRE_NANOAPP_INTERNAL
}  // anonymous namespace
}  // namespace chre

#include "chre/util/nanoapp/app_id.h"
#include "chre/platform/static_nanoapp_init.h"

CHRE_STATIC_NANOAPP_INIT(WwanWorld, chre::kWwanWorldAppId, 0);
#endif  // CHRE_NANOAPP_INTERNAL
