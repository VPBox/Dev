/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_PAL_WIFI_H_
#define CHRE_PAL_WIFI_H_

/**
 * @file
 * Defines the interface between the common CHRE core system and the
 * platform-specific WiFi module.
 */

#include <stdbool.h>
#include <stdint.h>

#include "chre_api/chre/common.h"
#include "chre_api/chre/wifi.h"
#include "chre/pal/system.h"
#include "chre/pal/version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initial version of the CHRE WiFi PAL, tied to CHRE API v1.1.
 */
#define CHRE_PAL_WIFI_API_V1_0  CHRE_PAL_CREATE_API_VERSION(1, 0)

// v1.1 skipped to avoid confusion with CHRE API v1.1

/**
 * Introduced alongside CHRE API v1.2, adding support for RTT ranging and radio
 * chain preference
 */
#define CHRE_PAL_WIFI_API_V1_2  CHRE_PAL_CREATE_API_VERSION(1, 2)

/**
 * The version of the WiFi GNSS PAL defined in this header file.
 */
#define CHRE_PAL_WIFI_API_CURRENT_VERSION  CHRE_PAL_WIFI_API_V1_2

struct chrePalWifiCallbacks {
    /**
     * Callback invoked to inform the CHRE of the result of changes to the scan
     * monitor registration status requested via configureScanMonitor in struct
     * chrePalWifiApi.
     *
     * Unsolicited calls to this function must not be made. In other words,
     * this callback should only be invoked as the direct result of an earlier
     * call to configureScanMonitor. If the scan monitor registration is lost,
     * for example due to a reset of the WiFi subsystem, then the PAL
     * implementation is required to silently re-register the scan monitor when
     * it recovers, as needed.
     *
     * @param enabled true if the scan monitor is currently active and
     *        scanEventCallback will receive unsolicited scan results, false
     *        otherwise
     * @param errorCode An error code from enum chreError
     *
     * @see chrePalWifiApi.configureScanMonitor
     * @see #chreError
     */
    void (*scanMonitorStatusChangeCallback)(bool enabled, uint8_t errorCode);

    /**
     * Callback invoked to inform the CHRE of the result of a request for a
     * scan requested via requestScan in struct chrePalWifiApi.
     *
     * Unsolicited calls to this function must not be made. See
     * scanMonitorStatusChangeCallback() for more information.
     *
     * This function must only be called after the final status of the scan
     * request is known. For example, it must not be called at the point when
     * the scan is initially scheduled if it can still fail prior to delivering
     * a result.
     *
     * @param pending true if the request was successful and the results of the
     *        scan are pending delivery (via scanEventCallback), false otherwise
     * @param errorCode An error code from enum chreError
     */
    void (*scanResponseCallback)(bool pending, uint8_t errorCode);

    /**
     * Callback used to pass scan results from the WiFi module to the core CHRE
     * system, which distributes it to clients (nanoapps).
     *
     * This function call passes ownership of the event memory to the core CHRE
     * system, i.e. the PAL module must not modify the referenced data until the
     * associated API function is called to release the memory.
     *
     * If the results of a given scan are be split across multiple events, and
     * therefore multiple calls to this callback, then the events must be
     * delivered in order, and in one contiguous series of callbacks with no
     * interleaving of events that correspond to any other scan.
     *
     * The PAL module must not deliver the same scan event twice. As a specific
     * example: if an explicit scan request is made via requestScan(), the PAL
     * implementation must not redeliver the result a second time because scan
     * monitoring is enabled.
     *
     * @param event Event data to distribute to clients. The WiFi module
     *        must ensure that this memory remains accessible until it is passed
     *        to the releaseScanEvent() function in struct chrePalWifiApi.
     *
     * @see chrePalWifiApi.configureScanMonitor
     * @see chrePalWifiApi.requestScan
     */
    void (*scanEventCallback)(struct chreWifiScanEvent *event);

    /**
     * Callback used to pass RTT ranging results from the WiFi module to the
     * core CHRE system, which distributes it to clients (nanoapps).
     *
     * Like scanEventCallback, this function call passes ownership of the event
     * memory to the core CHRE system.
     *
     * Only valid if requestedApiVersion given to chrePalWifiGetApi() is greater
     * than or equal to CHRE_PAL_WIFI_API_V1_2.
     *
     * @param errorCode An error code from enum chreError, with CHRE_ERROR_NONE
     *        indicating successful completion of the ranging operation
     * @param event Event data to distribute to clients. Unlike with scan
     *        events, RTT ranging results must be provided for all requested MAC
     *        addresses in a single event. Ignored and may be NULL if errorCode
     *        is not CHRE_ERROR_NONE.
     *
     * @since v1.2
     */
    void (*rangingEventCallback)(uint8_t errorCode,
                                 struct chreWifiRangingEvent *event);
};

struct chrePalWifiApi {
    /**
     * Version of the module providing this API. This value should be
     * constructed from CHRE_PAL_CREATE_MODULE_VERSION using the supported
     * API version constant (CHRE_PAL_WIFI_API_*) and the module-specific patch
     * version.
     */
    uint32_t moduleVersion;

    /**
     * Initializes the WiFi module. Initialization must complete synchronously.
     *
     * @param systemApi Structure containing CHRE system function pointers which
     *        the PAL implementation should prefer to use over equivalent
     *        functionality exposed by the underlying platform. The module does
     *        not need to deep-copy this structure; its memory remains
     *        accessible at least until after close() is called.
     * @param callbacks Structure containing entry points to the core CHRE
     *        system. The module does not need to deep-copy this structure; its
     *        memory remains accessible at least until after close() is called.
     *
     * @return true if initialization was successful, false otherwise
     */
    bool (*open)(const struct chrePalSystemApi *systemApi,
                 const struct chrePalWifiCallbacks *callbacks);

    /**
     * Performs clean shutdown of the WiFi module, usually done in preparation
     * for stopping the CHRE. The WiFi module must ensure that it will not
     * invoke any callbacks past this point, and complete any relevant teardown
     * activities before returning from this function.
     */
    void (*close)(void);

    //! @see chreWifiGetCapabilities()
    uint32_t (*getCapabilities)(void);

    /**
     * Configures whether the scanEventCallback receives unsolicited scan
     * results, i.e. the results of scans not performed at the request of CHRE.
     *
     * While not expected, a duplicate request, e.g. one that requests to enable
     * scan monitoring when it is already enabled, must follow the successful
     * callback flow.
     *
     * @param enable true to enable listening for all available scan results
     *
     * @return true if the request was accepted for processing, in which case a
     *         subsequent call to scanMonitorStatusChangeCallback will be used
     *         to communicate the result of the operation
     *
     * @see chreWifiConfigureScanMonitorAsync()
     */
    bool (*configureScanMonitor)(bool enable);

    /**
     * Request that the WiFi chipset perform a scan, or deliver results from its
     * cache if the parameters allow for it. If this function returns true, then
     * the scanResponseCallback will be invoked to provide the result of the
     * scan. If that indicates a successful result (the scan data is pending),
     * then scanEventCallback() will be invoked one more more times to deliver
     * the results of the scan. The results for the requested scan are delivered
     * in scanEventCallback() regardless of the most recent setting passed to
     * configureScanMonitor().
     *
     * The asynchronous flow of a scan request made through this API is
     * as follows:
     *
     *  1. requestScan() called, returns true if request accepted, otherwise
     *     false (in which case the request fails at this stage and further
     *     steps do not occur)
     *  2. Scan is performed, or an error is encountered preventing the
     *     successful delivery of the scan result
     *  3. scanResponseCallback() is invoked to indicate whether the scan
     *     succeeded, or the reason for failure (in which case the request fails
     *     at this stage and further steps do not occur)
     *  4. scanEventCallback() is invoked 1 or more times (even if the scan
     *     resulted in no visible APs)
     *
     * Note that the callbacks in steps 3 and 4 must complete in the sequence
     * given, and the call(s) to scanEventCallback() occurring immediately after
     * scanResponseCallback() must be associated with this scan request, and not
     * results delivered pursuant to an active scan monitor registration.
     *
     * This function must follow the CHRE API-defined behavior regarding
     * timeouts. In other words, if a successful scan result is not produced by
     * the lower layers within CHRE_WIFI_SCAN_RESULT_TIMEOUT_NS,
     * scanResponseCallback() must be invoked to indicate the failure, and any
     * late arriving scan result from the lower layers must be dropped.
     *
     * At most 1 scan can be in progress from this API at any given time.
     * In other words, the implementation should return false if another scan
     * initiated via this function has not completed, i.e. it has not failed
     * yet, or the final scan event has not yet been delivered via
     * scanEventCallback(). However, this function must accept and queue a scan
     * request made from this API while a scan requested by another client, such
     * as the applications processor, is in progress.
     *
     * @param params See chreWifiRequestScanAsync(). If requestedApiVersion
     *        supplied to chrePalWifiGetApi is at least CHRE_PAL_WIFI_API_V1_2,
     *        then the new "radioChainPref" parameter will be included.
     *
     * @return true if the request was accepted for further processing, in which
     *         case a subsequent call to scanResponseCallback will be used to
     *         communicate the result of the operation
     *
     * @see #chreWifiScanParams
     * @see chreWifiRequestScanAsync()
     */
    bool (*requestScan)(const struct chreWifiScanParams *params);

    /**
     * Invoked when the core CHRE system no longer needs a WiFi scan event
     * structure that was provided to it via scanEventCallback()
     *
     * @param event Event data to release
     */
    void (*releaseScanEvent)(struct chreWifiScanEvent *event);

    /**
     * Request that the WiFi chipset perform RTT ranging against a set of access
     * points specified in params. If this function returns true, then
     * rangingEventCallback must be invoked once to deliver the final result of
     * the operation, with the accompanying result structure if ranging was
     * performed.
     *
     * RTT functionality in CHRE is based off the Android HAL definitions
     * (hardware/interfaces/wifi/1.0/), but with less parameters. For
     * example, CHRE only supports ranging against access points, and two-sided
     * RTT. When mapping struct chreWifiRangingTarget into the equivalent fields
     * defined in the HAL in struct RttConfig, the following default values
     * should be used to fill the fields not specified in the CHRE structure:
     *
     * <pre>
     *   type = TWO_SIDED
     *   peer = AP
     *   burstPeriod = 0
     *   numBurst = 0
     *   numFramesPerBurst = 8
     *   numRetriesPerRttFrame = 0
     *   numRetriesPerFtmr = 0
     *   mustRequestLci = true
     *   mustRequestLcr = false (can be true, but not exposed by CHRE)
     *   burstDuration = 15
     *   preamble = implementation-dependent**
     *   bw = implementation-dependent**
     * </pre>
     *
     * **These are used to populate the Format And Bandwidth field in the Fine
     *   Timing Measurement Parameters element. Per the specification, proposed
     *   values must fall within the capabilities of the requesting device, and
     *   the configuration used is ultimately negotiated with the responding
     *   STA. Therefore, it is up to the underlying WiFi implementation to pick
     *   suitable values.
     *
     * Like {@link #requestScan}, this function must follow the CHRE API-defined
     * behavior regarding timeouts, indicating failure via rangingEventCallback
     * if the lower layers do not produce a result within
     * CHRE_WIFI_RANGING_RESULT_TIMEOUT_NS.
     *
     * Also like {@link #requestScan}, at most 1 RTT ranging request can be in
     * progress from this API at any given time. Implementations should return
     * false if this condition is not met, but must queue a request made from
     * this API while a request from another client, such as the applications
     * processor, is in progress.
     *
     * @return true if the request was accepted for further processing, in which
     *         case a subsequent call to rangingEventCallback will be used to
     *         communicate the result of the operation
     *
     * @see #chreWifiRangingParams
     * @see chreWifiRequestRangingAsync()
     *
     * @since v1.2
     */
    bool (*requestRanging)(const struct chreWifiRangingParams *params);

    /**
     * Invoked when the core CHRE system no longer needs a WiFi ranging result
     * event structure that was provided to it via rangingEventCallback()
     *
     * @param event Event data to release
     *
     * @since v1.2
     */
    void (*releaseRangingEvent)(struct chreWifiRangingEvent *event);
};

/**
 * Retrieve a handle for the CHRE WiFi PAL.
 *
 * @param requestedApiVersion The implementation of this function must return a
 *        pointer to a structure with the same major version as requested.
 *
 * @return Pointer to API handle, or NULL if a compatible API version is not
 *         supported by the module, or the API as a whole is not implemented. If
 *         non-NULL, the returned API handle must be valid as long as this
 *         module is loaded.
 */
const struct chrePalWifiApi *chrePalWifiGetApi(uint32_t requestedApiVersion);

#ifdef __cplusplus
}
#endif

#endif  // CHRE_PAL_WIFI_H_
