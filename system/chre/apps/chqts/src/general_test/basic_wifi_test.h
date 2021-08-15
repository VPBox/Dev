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
#ifndef _GTS_NANOAPPS_GENERAL_TEST_BASIC_WIFI_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_BASIC_WIFI_TEST_H_

#include <general_test/test.h>

#include <cstdint>

#include <shared/test_success_marker.h>

#include "chre/util/optional.h"

namespace general_test {

/**
 * A class which tests chre WiFi APIs, including:
 * chreWifiConfigureScanMonitorAsync
 * chreWifiRequestScanAsync
 * chreWifiRequestRangingAsync.
 *
 * Sends requests to those APIs and validates subsequent event data.
 * Sends success to host if all data is valid, otherwise sends fatal failure.
 */
class BasicWifiTest : public Test {
 public:
  BasicWifiTest();
 protected:
  /**
   * Handles WiFi events, including:
   * CHRE_EVENT_WIFI_ASYNC_RESULT
   * CHRE_EVENT_WIFI_SCAN_RESULT
   * CHRE_EVENT_WIFI_RANGING_RESULT
   *
   * @param senderInstanceId instance id from which the event is sent.
   * @param eventType one of the above events.
   * @param eventData a pointer to the details of a WiFi event.
   */
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void *eventData) override;

  /**
   * Calls chreWifiConfigureScanMonitorAsync with enable = true
   * if WiFi has scan monitor capability, otherwise calls
   * chreWifiRequestScanAsync if WiFi has on demand scan capability.
   *
   * @param messageSize the size of received message.
   * @param message a pointer to the received message.
   */
  void setUp(uint32_t messageSize, const void *message) override;
 private:
  /**
   * Validates chre WiFi async events.
   * If validation result is true, makes subsequent requests:
   * chreWifiConfigureScanMonitorAsync with enable = false
   * chreWifiRequestScanAsyncDefault
   *
   * @param eventData received WiFi async result data.
   */
  void handleChreWifiAsyncEvent(const chreAsyncResult *result);

  /**
   * @param eventData received WiFi scan event data.
   * @return true if scanType is CHRE_WIFI_SCAN_TYPE_ACTIVE, false otherwise.
   */
  bool isActiveWifiScanType(const chreWifiScanEvent *eventData);

  /**
   * Makes an API call, if corresponding WiFi ability exists;
   * otherwise procceeds to next test stage.
   */
  void startScanMonitorTestStage();
  void startScanAsyncTestStage();

  /**
   * This method must be called after making an async request to CHRE.
   *
   * @param cookie pointer to request cookie.
   * @param requestType a type of request.
   * @param timeoutNs expected maximum elapse to receive chre WiFi result.
   */
  void resetCurrentWifiRequest(const void *cookie,
                               uint8_t requestType,
                               uint64_t timeoutNs);

  /**
   * Validates a WiFi scan event, including event version, event order,
   * and WiFi scan results. Sends fatal failure to host if event data is
   * invalid, otherwise calls API chreWifiRequestRangingAsync.
   *
   * @param eventData received WiFi scan event data.
   */
  void validateWifiScanEvent(const chreWifiScanEvent *eventData);

  /**
   * Validates ssidLen, band, RSSI, primaryChannel and centerFreqSecondary
   * of all WiFi scan results. Sends fatal failure to host
   * if there are invalid fields.
   *
   * @param count the size of results.
   * @param results a pointer to the structure containing the results.
   */
  void validateWifiScanResult(uint8_t count,
                              const chreWifiScanResult *results);

  /**
   * Basic WiFi test stages and total number of stages.
   */
  enum BasicWifiTestStage {
    BASIC_WIFI_TEST_STAGE_SCAN_MONITOR = 0,
    BASIC_WIFI_TEST_STAGE_SCAN_ASYNC,
    BASIC_WIFI_TEST_STAGE_COUNT,
  };

  //! WiFi capabilities, used to make corresponding API calls.
  uint32_t mWifiCapabilities;

  //! TestSuccessMarker object to mark success of a stage.
  nanoapp_testing::TestSuccessMarker mTestSuccessMarker =
      nanoapp_testing::TestSuccessMarker(BASIC_WIFI_TEST_STAGE_COUNT);

  //! Used to indicate if a chreAsyncResult is being expected.
  chre::Optional<chreAsyncRequest> mCurrentWifiRequest;

  //! Start timestamp used to timing an event.
  uint64_t mStartTimestampNs = 0;

  //! Expected sequence number for an event within a series of events
  //! comprising a complete scan result.
  uint32_t mNextExpectedIndex = 0;

  //! The remaining results of WiFi scan.
  //! Used to identify when all events have been received.
  uint32_t mWiFiScanResultRemaining = 0;
};

} // namespace general_test

#endif // _GTS_NANOAPPS_GENERAL_TEST_BASIC_WIFI_TEST_H_
