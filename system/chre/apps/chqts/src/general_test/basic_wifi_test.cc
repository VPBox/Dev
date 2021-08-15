/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <general_test/basic_wifi_test.h>

#include <chre.h>
#include <shared/send_message.h>
#include <shared/time_util.h>

using nanoapp_testing::kOneMillisecondInNanoseconds;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendFatalFailureToHostUint8;
using nanoapp_testing::sendSuccessToHost;

/*
 * Test to check expected functionality of the CHRE WiFi APIs.
 *
 * 1. If scan monitor is not supported, skip to 5;
 *    otherwise enables scan monitor.
 * 2. Checks async result of enabling scan monitor.
 * 3. Disables scan monitor.
 * 4. Checks async result of disabling scan monitor.
 * 5. If on demand WiFi scan is not supported, skip to end;
 *    otherwise sends default scan request.
 * 6. Checks the result of on demand WiFi scan.
 */
namespace general_test {

namespace {

//! A dummy cookie to pass into the enable
//! configure scan monitoring async request.
constexpr uint32_t kEnableScanMonitoringCookie = 0x1337;

//! A dummy cookie to pass into the disable
//! configure scan monitoring async request.
constexpr uint32_t kDisableScanMonitoringCookie = 0x1338;

//! A dummy cookie to pass into request scan async.
constexpr uint32_t kOnDemandScanCookie = 0xcafe;

//! Starting frequency of band 2.4 GHz
constexpr uint32_t kWifiBandStartFreq_2_4_GHz = 2407;

//! Starting frequency of band 5 GHz
constexpr uint32_t kWifiBandStartFreq_5_GHz = 5000;

//! Frequency of channel 14
constexpr uint32_t kWifiBandFreqOfChannel_14 = 2484;

/**
 * Calls API testConfigureScanMonitorAsync. Sends fatal failure to host
 * if API call fails.
 *
 * @param enable Set to true to enable monitoring scan results,
 *        false to disable.
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 */
void testConfigureScanMonitorAsync(bool enable, const void * cookie) {
  if (!chreWifiConfigureScanMonitorAsync(enable, cookie)) {
    if (enable) {
      sendFatalFailureToHost("Failed to request to enable scan monitor.");
    } else {
      sendFatalFailureToHost("Failed to request to disable scan monitor.");
    }
  }
}

/**
 * Calls API chreWifiRequestScanAsyncDefault. Sends fatal failure to host
 * if API call fails.
 */
void testRequestScanAsync() {
  if (!chreWifiRequestScanAsyncDefault(&kOnDemandScanCookie)) {
    sendFatalFailureToHost("Failed to request for on-demand WiFi scan.");
  }
}

/**
 * Validates primaryChannel and sends fatal failure to host if failing.
 * 1. (primaryChannel - start frequecny) is a multiple of 5.
 * 2. primaryChannelNumber is multiple of 5 and between [1, maxChannelNumber].
 *
 * @param primaryChannel primary channel of a WiFi scan result.
 * @param startFrequency start frequency of band 2.4/5 GHz.
 * @param maxChannelNumber max channel number of band 2.4/5 GHz.
 */
void validatePrimaryChannel(uint32_t primaryChannel,
                            uint32_t startFrequency,
                            uint8_t maxChannelNumber) {
  if ((primaryChannel - startFrequency) % 5 != 0) {
    chreLog(CHRE_LOG_ERROR,
            "primaryChannel - %d must be a multiple of 5,"
            "got primaryChannel: %d",
            startFrequency, primaryChannel);
  }

  uint32_t primaryChannelNumber = (primaryChannel - startFrequency) / 5;
  if (primaryChannelNumber < 1 || primaryChannelNumber > maxChannelNumber) {
    chreLog(CHRE_LOG_ERROR,
            "primaryChannelNumber must be between 1 and %d,"
            "got primaryChannel: %d",
            maxChannelNumber, primaryChannel);
  }
}

/**
 * Validates primaryChannel for band 2.4/5 GHz.
 *
 * primaryChannelNumber of band 2.4 GHz is between 1 and 13,
 * plus a special case for channel 14 (primaryChannel == 2484);
 * primaryChannelNumber of band 5 GHz is between 1 and 200,
 * ref: IEEE Std 802.11-2016, 19.3.15.2.
 * Also, (primaryChannel - start frequecny) is a multiple of 5,
 * except channel 14 of 2.4 GHz.
 *
 * @param result WiFi scan result.
 */
void validatePrimaryChannel(const chreWifiScanResult& result) {
  // channel 14 (primaryChannel = 2484) is not applicable for this test.
  if (result.band == CHRE_WIFI_BAND_2_4_GHZ &&
      result.primaryChannel != kWifiBandFreqOfChannel_14) {
    validatePrimaryChannel(result.primaryChannel,
                           kWifiBandStartFreq_2_4_GHz,
                           13);
  } else if (result.band == CHRE_WIFI_BAND_5_GHZ) {
    validatePrimaryChannel(result.primaryChannel,
                           kWifiBandStartFreq_5_GHz,
                           200);
  }
}

/**
 * Validates centerFreqPrimary and centerFreqSecondary
 * TODO (jacksun) add test when channelWidth is 20, 40, 80, or 160 MHz
 */
void validateCenterFreq(const chreWifiScanResult& result) {
  if (result.channelWidth != CHRE_WIFI_CHANNEL_WIDTH_80_PLUS_80_MHZ
      && result.centerFreqSecondary != 0) {
    // TODO (jacksun) Format the centerFreqSecondary into the message
    // after redesigning of sendFatalFailureToHost()
    sendFatalFailureToHost(
        "centerFreqSecondary must be 0 if channelWidth is not 80+80MHZ");
  }
}

} // anonymous namespace

BasicWifiTest::BasicWifiTest()
    : Test(CHRE_API_VERSION_1_1) {
}

void BasicWifiTest::setUp(
    uint32_t messageSize, const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "Expected 0 byte message, got more bytes:", &messageSize);
  } else {
    mWifiCapabilities = chreWifiGetCapabilities();
    startScanMonitorTestStage();
  }
}

void BasicWifiTest::handleEvent(uint32_t /* senderInstanceId */,
                                uint16_t eventType,
                                const void *eventData) {
  if (eventData == nullptr) {
    sendFatalFailureToHost("Received null eventData");
  }
  switch (eventType) {
    case CHRE_EVENT_WIFI_ASYNC_RESULT:
      handleChreWifiAsyncEvent(
          static_cast<const chreAsyncResult *>(eventData));
      break;
    case CHRE_EVENT_WIFI_SCAN_RESULT:
      {
        const auto *result = static_cast<const chreWifiScanEvent *>(eventData);
        if (isActiveWifiScanType(result)) {
          // The first chreWifiScanResult is expected to come immediately,
          // but a long delay is possible if it's implemented incorrectly,
          // e.g. the async result comes right away (before the scan is actually
          // completed), then there's a long delay to the scan result.
          if (mStartTimestampNs != 0
              && chreGetTime() - mStartTimestampNs >
                  50 * kOneMillisecondInNanoseconds) {
            sendFatalFailureToHost(
                "Did not receive chreWifiScanResult within 50 milliseconds.");
          }
          mStartTimestampNs = 0;
          validateWifiScanEvent(result);
        }
      }
      break;
    default:
      unexpectedEvent(eventType);
      break;
  }
}

void BasicWifiTest::handleChreWifiAsyncEvent(const chreAsyncResult *result) {
  if (!mCurrentWifiRequest.has_value()) {
    nanoapp_testing::sendFailureToHost("Unexpected async result");
  }
  validateChreAsyncResult(result, mCurrentWifiRequest.value());

  switch (result->requestType) {
    case CHRE_WIFI_REQUEST_TYPE_REQUEST_SCAN:
      mStartTimestampNs = chreGetTime();
      break;
    case CHRE_WIFI_REQUEST_TYPE_CONFIGURE_SCAN_MONITOR:
      if (mCurrentWifiRequest->cookie == &kDisableScanMonitoringCookie) {
        mTestSuccessMarker.markStageAndSuccessOnFinish(
            BASIC_WIFI_TEST_STAGE_SCAN_MONITOR);
        startScanAsyncTestStage();
      } else {
        testConfigureScanMonitorAsync(false /* enable */,
                                      &kDisableScanMonitoringCookie);
        resetCurrentWifiRequest(&kDisableScanMonitoringCookie,
                                CHRE_WIFI_REQUEST_TYPE_CONFIGURE_SCAN_MONITOR,
                                CHRE_ASYNC_RESULT_TIMEOUT_NS);
      }
      break;
    default:
      sendFatalFailureToHostUint8(
          "Received unexpected requestType %d", result->requestType);
      break;
  }
}

bool BasicWifiTest::isActiveWifiScanType(const chreWifiScanEvent *eventData) {
  return (eventData->scanType == CHRE_WIFI_SCAN_TYPE_ACTIVE);
}

void BasicWifiTest::startScanMonitorTestStage() {
  if (mWifiCapabilities & CHRE_WIFI_CAPABILITIES_SCAN_MONITORING) {
    testConfigureScanMonitorAsync(true /* enable */,
                                  &kEnableScanMonitoringCookie);
    resetCurrentWifiRequest(&kEnableScanMonitoringCookie,
                            CHRE_WIFI_REQUEST_TYPE_CONFIGURE_SCAN_MONITOR,
                            CHRE_ASYNC_RESULT_TIMEOUT_NS);
  } else {
    mTestSuccessMarker.markStageAndSuccessOnFinish(
        BASIC_WIFI_TEST_STAGE_SCAN_MONITOR);
    startScanAsyncTestStage();
  }
}

void BasicWifiTest::startScanAsyncTestStage() {
  if (mWifiCapabilities & CHRE_WIFI_CAPABILITIES_ON_DEMAND_SCAN) {
    testRequestScanAsync();
    resetCurrentWifiRequest(&kOnDemandScanCookie,
                            CHRE_WIFI_REQUEST_TYPE_REQUEST_SCAN,
                            CHRE_WIFI_SCAN_RESULT_TIMEOUT_NS);
  } else {
    mTestSuccessMarker.markStageAndSuccessOnFinish(
        BASIC_WIFI_TEST_STAGE_SCAN_ASYNC);
  }
}

void BasicWifiTest::resetCurrentWifiRequest(const void *cookie,
                                            uint8_t requestType,
                                            uint64_t timeoutNs) {
  chreAsyncRequest request = {
    .cookie = cookie,
    .requestType = requestType,
    .requestTimeNs = chreGetTime(),
    .timeoutNs = timeoutNs
  };
  mCurrentWifiRequest = request;
}

void BasicWifiTest::validateWifiScanEvent(const chreWifiScanEvent *eventData) {
  if (eventData->version != CHRE_WIFI_SCAN_EVENT_VERSION) {
    sendFatalFailureToHostUint8(
        "Got unexpected scan event version %d", eventData->version);
  }

  if (mNextExpectedIndex != eventData->eventIndex) {
    chreLog(CHRE_LOG_ERROR, "Expected index: %d, received index: %d",
            mNextExpectedIndex, eventData->eventIndex);
    sendFatalFailureToHost("Received out-of-order events");
  }
  mNextExpectedIndex++;

  if (eventData->eventIndex == 0) {
    mWiFiScanResultRemaining = eventData->resultTotal;
  }
  if (mWiFiScanResultRemaining < eventData->resultCount) {
    chreLog(CHRE_LOG_ERROR, "Remaining scan results %d, received %d",
            mWiFiScanResultRemaining, eventData->resultCount);
    sendFatalFailureToHost("Received too many WiFi scan results");
  }
  mWiFiScanResultRemaining -= eventData->resultCount;

  validateWifiScanResult(eventData->resultCount, eventData->results);
  if (mWiFiScanResultRemaining == 0) {
    mNextExpectedIndex = 0;
    mTestSuccessMarker.markStageAndSuccessOnFinish(
        BASIC_WIFI_TEST_STAGE_SCAN_ASYNC);
  }
}

void BasicWifiTest::validateWifiScanResult(
    uint8_t count, const chreWifiScanResult *results) {
  for (uint8_t i = 0; i < count; ++i) {
    if (results[i].ssidLen > CHRE_WIFI_SSID_MAX_LEN) {
      sendFatalFailureToHostUint8(
          "Got unexpected ssidLen %d", results[i].ssidLen);
    }

    // TODO: Enable fatal failures on band, RSSI, and primary channel
    //       validations when proper error waiver is implemented in CHQTS.
    if (results[i].band != CHRE_WIFI_BAND_2_4_GHZ
        && results[i].band != CHRE_WIFI_BAND_5_GHZ) {
      chreLog(CHRE_LOG_ERROR, "Got unexpected band %d", results[i].band);
    }

    // It's possible for WiFi RSSI be positive if the phone is placed
    // right next to a high-power AP (e.g. transmitting at 20 dBm),
    // in which case RSSI will be < 20 dBm. Place a high threshold to check
    // against values likely to be erroneous (36 dBm/4W).
    if (results[i].rssi >= 36) {
      chreLog(CHRE_LOG_ERROR, "RSSI should be less than 36, got: %d",
              results[i].rssi);
    }

    validatePrimaryChannel(results[i]);
    validateCenterFreq(results[i]);
  }
}

} // namespace general_test
