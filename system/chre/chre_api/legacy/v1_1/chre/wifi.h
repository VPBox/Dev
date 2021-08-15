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

#ifndef _CHRE_WIFI_H_
#define _CHRE_WIFI_H_

/**
 * @file
 * WiFi (IEEE 802.11) API, currently covering scanning features useful for
 * determining location and offloading certain connectivity scans.
 *
 * In this file, specification references use the following shorthand:
 *
 *    Shorthand | Full specification name
 *   -----------+-------------------------
 *     "802.11" | IEEE Std 802.11-2007
 *     "HT"     | IEEE Std 802.11n-2009
 *     "VHT"    | IEEE Std 802.11ac-2013
 *
 */

#include <chre/common.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The set of flags returned by chreWifiGetCapabilities().
 * @defgroup CHRE_WIFI_CAPABILITIES
 * @{
 */

//! No WiFi APIs are supported
#define CHRE_WIFI_CAPABILITIES_NONE             UINT32_C(0)

//! Listening to scan results is supported, as enabled via
//! chreWifiConfigureScanMonitorAsync()
#define CHRE_WIFI_CAPABILITIES_SCAN_MONITORING  UINT32_C(1 << 0)

//! Requesting WiFi scans on-demand is supported via chreWifiRequestScanAsync()
#define CHRE_WIFI_CAPABILITIES_ON_DEMAND_SCAN   UINT32_C(1 << 1)

/** @} */

/**
 * Produce an event ID in the block of IDs reserved for WiFi
 * @param offset  Index into WiFi event ID block; valid range [0,15]
 */
#define CHRE_WIFI_EVENT_ID(offset)  (CHRE_EVENT_WIFI_FIRST_EVENT + (offset))

/**
 * nanoappHandleEvent argument: struct chreAsyncResult
 *
 * Communicates the asynchronous result of a request to the WiFi API. The
 * requestType field in chreAsyncResult is set to a value from enum
 * chreWifiRequestType.
 */
#define CHRE_EVENT_WIFI_ASYNC_RESULT  CHRE_WIFI_EVENT_ID(0)

/**
 * nanoappHandleEvent argument: struct chreWifiScanEvent
 *
 * Provides results of a WiFi scan.
 */
#define CHRE_EVENT_WIFI_SCAN_RESULT  CHRE_WIFI_EVENT_ID(1)

// NOTE: Do not add new events with ID > 15; only values 0-15 are reserved
// (see chre/event.h)

/**
 * The maximum amount of time that is allowed to elapse between a call to
 * chreWifiRequestScanAsync() that returns true, and the associated
 * CHRE_EVENT_WIFI_ASYNC_RESULT used to indicate whether the scan completed
 * successfully or not.
 */
#define CHRE_WIFI_SCAN_RESULT_TIMEOUT_NS  (30 * CHRE_NSEC_PER_SEC)

/**
 * The current compatibility version of the chreWifiScanEvent structure,
 * including nested structures.
 */
#define CHRE_WIFI_SCAN_EVENT_VERSION  UINT8_C(1)

/**
 * Maximum number of frequencies that can be explicitly specified when
 * requesting a scan
 * @see #chreWifiScanParams
 */
#define CHRE_WIFI_FREQUENCY_LIST_MAX_LEN  (20)

/**
 * Maximum number of SSIDs that can be explicitly specified when requesting a
 * scan
 * @see #chreWifiScanParams
 */
#define CHRE_WIFI_SSID_LIST_MAX_LEN  (20)

/**
 * The maximum number of octets in an SSID (see 802.11 7.3.2.1)
 */
#define CHRE_WIFI_SSID_MAX_LEN  (32)

/**
 * The number of octets in a BSSID (see 802.11 7.1.3.3.3)
 */
#define CHRE_WIFI_BSSID_LEN  (6)

/**
 * Set of flags which can either indicate a frequency band. Specified as a bit
 * mask to allow for combinations in future API versions.
 * @defgroup CHRE_WIFI_BAND_MASK
 * @{
 */

#define CHRE_WIFI_BAND_MASK_2_4_GHZ  UINT8_C(1 << 0)  //!< 2.4 GHz
#define CHRE_WIFI_BAND_MASK_5_GHZ    UINT8_C(1 << 1)  //!< 5 GHz

/** @} */

/**
 * Characteristics of a scanned device given in struct chreWifiScanResult.flags
 * @defgroup CHRE_WIFI_SCAN_RESULT_FLAGS
 * @{
 */

#define CHRE_WIFI_SCAN_RESULT_FLAGS_NONE                         UINT8_C(0)

//! Element ID 61 (HT Operation) is present (see HT 7.3.2)
#define CHRE_WIFI_SCAN_RESULT_FLAGS_HT_OPS_PRESENT               UINT8_C(1 << 0)

//! Element ID 192 (VHT Operation) is present (see VHT 8.4.2)
#define CHRE_WIFI_SCAN_RESULT_FLAGS_VHT_OPS_PRESENT              UINT8_C(1 << 1)

//! Element ID 127 (Extended Capbilities) is present, and bit 70 (Fine Timing
//! Measurement Responder) is set to 1 (see IEEE draft 802.11mc 8.4.2.26)
#define CHRE_WIFI_SCAN_RESULT_FLAGS_IS_80211MC_RTT_RESPONDER     UINT8_C(1 << 2)

//! HT Operation element indicates that a secondary channel is present
//! (see HT 7.3.2.57)
#define CHRE_WIFI_SCAN_RESULT_FLAGS_HAS_SECONDARY_CHANNEL_OFFSET UINT8_C(1 << 3)

//! HT Operation element indicates that the secondary channel is below the
//! primary channel (see HT 7.3.2.57)
#define CHRE_WIFI_SCAN_RESULT_FLAGS_SECONDARY_CHANNEL_OFFSET_IS_BELOW  \
                                                                 UINT8_C(1 << 4)

/** @} */

/**
 * Identifies the authentication methods supported by an AP. Note that not every
 * combination of flags may be possible. Based on WIFI_PNO_AUTH_CODE_* from
 * hardware/libhardware_legacy/include/hardware_legacy/gscan.h in Android.
 * @defgroup CHRE_WIFI_SECURITY_MODE_FLAGS
 * @{
 */

#define CHRE_WIFI_SECURITY_MODE_UNKONWN  UINT8_C(0)

#define CHRE_WIFI_SECURITY_MODE_OPEN  UINT8_C(1 << 0)  //!< No auth/security
#define CHRE_WIFI_SECURITY_MODE_WEP   UINT8_C(1 << 1)
#define CHRE_WIFI_SECURITY_MODE_PSK   UINT8_C(1 << 2)  //!< WPA-PSK or WPA2-PSK
#define CHRE_WIFI_SECURITY_MODE_EAP   UINT8_C(1 << 3)  //!< Any type of EAPOL

/** @} */

/**
 * Identifies a WiFi frequency band
 */
enum chreWifiBand {
    CHRE_WIFI_BAND_2_4_GHZ = CHRE_WIFI_BAND_MASK_2_4_GHZ,
    CHRE_WIFI_BAND_5_GHZ   = CHRE_WIFI_BAND_MASK_5_GHZ,
};

/**
 * Indicates the BSS operating channel width determined from the VHT and/or HT
 * Operation elements. Refer to VHT 8.4.2.161 and HT 7.3.2.57.
 */
enum chreWifiChannelWidth {
    CHRE_WIFI_CHANNEL_WIDTH_20_MHZ         = 0,
    CHRE_WIFI_CHANNEL_WIDTH_40_MHZ         = 1,
    CHRE_WIFI_CHANNEL_WIDTH_80_MHZ         = 2,
    CHRE_WIFI_CHANNEL_WIDTH_160_MHZ        = 3,
    CHRE_WIFI_CHANNEL_WIDTH_80_PLUS_80_MHZ = 4,
};

/**
 * Indicates the type of scan requested or performed
 */
enum chreWifiScanType {
    //! Perform a purely active scan using probe requests. Do not scan channels
    //! restricted to use via Dynamic Frequency Selection (DFS) only.
    CHRE_WIFI_SCAN_TYPE_ACTIVE = 0,

    //! Perform an active scan on unrestricted channels, and also perform a
    //! passive scan on channels that are restricted to use via Dynamic
    //! Frequency Selection (DFS), e.g. the U-NIII bands 5250-5350MHz and
    //! 5470-5725MHz in the USA as mandated by FCC regulation.
    CHRE_WIFI_SCAN_TYPE_ACTIVE_PLUS_PASSIVE_DFS = 1,

    //! Perform a passive scan, only listening for beacons.
    CHRE_WIFI_SCAN_TYPE_PASSIVE = 2,
};

/**
 * Indicates a type of request made in this API. Used to populate the resultType
 * field of struct chreAsyncResult sent with CHRE_EVENT_WIFI_ASYNC_RESULT.
 */
enum chreWifiRequestType {
    CHRE_WIFI_REQUEST_TYPE_CONFIGURE_SCAN_MONITOR = 1,
    CHRE_WIFI_REQUEST_TYPE_REQUEST_SCAN           = 2,
};

/**
 * SSID with an explicit length field, used when an array of SSIDs is supplied.
 */
struct chreWifiSsidListItem {
    //! Number of valid bytes in ssid. Valid range [0, CHRE_WIFI_SSID_MAX_LEN]
    uint8_t ssidLen;

    //! Service Set Identifier (SSID)
    uint8_t ssid[CHRE_WIFI_SSID_MAX_LEN];
};

/**
 * Data structure passed to chreWifiRequestScanAsync
 */
struct chreWifiScanParams {
    //! Set to a value from enum chreWifiScanType
    uint8_t scanType;

    //! Indicates whether the client is willing to tolerate receiving cached
    //! results of a previous scan, and if so, the maximum age of the scan that
    //! the client will accept. "Age" in this case is defined as the elapsed
    //! time between when the most recent scan was completed and the request is
    //! received, in milliseconds. If set to 0, no cached results may be
    //! provided, and all scan results must come from a "fresh" WiFi scan, i.e.
    //! one that completes strictly after this request is received. If more than
    //! one scan is cached and meets this age threshold, only the newest scan is
    //! provided.
    uint32_t maxScanAgeMs;

    //! If set to 0, scan all frequencies. Otherwise, this indicates the number
    //! of frequencies to scan, as specified in the frequencyList array. Valid
    //! range [0, CHRE_WIFI_FREQUENCY_LIST_MAX_LEN].
    uint16_t frequencyListLen;

    //! Pointer to an array of frequencies to scan, given as channel center
    //! frequencies in MHz. This field may be NULL if frequencyListLen is 0.
    const uint32_t *frequencyList;

    //! If set to 0, do not restrict scan to any SSIDs. Otherwise, this
    //! indicates the number of SSIDs in the ssidList array to be used for
    //! directed probe requests. Not applicable and ignore when scanType is
    //! CHRE_WIFI_SCAN_TYPE_PASSIVE.
    uint8_t ssidListLen;

    //! Pointer to an array of SSIDs to use for directed probe requests. May be
    //! NULL if ssidListLen is 0.
    const struct chreWifiSsidListItem *ssidList;
};

/**
 * Provides information about a single access point (AP) detected in a scan.
 */
struct chreWifiScanResult {
    //! Number of milliseconds prior to referenceTime in the enclosing
    //! chreWifiScanEvent struct when the probe response or beacon frame that
    //! was used to populate this structure was received.
    uint32_t ageMs;

    //! Capability Information field sent by the AP (see 802.11 7.3.1.4). This
    //! field must reflect native byte order and bit ordering, such that
    //! (capabilityInfo & 1) gives the bit for the ESS subfield.
    uint16_t capabilityInfo;

    //! Number of valid bytes in ssid. Valid range [0, CHRE_WIFI_SSID_MAX_LEN]
    uint8_t ssidLen;

    //! Service Set Identifier (SSID), a series of 0 to 32 octets identifying
    //! the access point. Note that this is commonly a human-readable ASCII
    //! string, but this is not the required encoding per the standard.
    uint8_t ssid[CHRE_WIFI_SSID_MAX_LEN];

    //! Basic Service Set Identifier (BSSID), represented in big-endian byte
    //! order, such that the first octet of the OUI is accessed in byte index 0.
    uint8_t bssid[CHRE_WIFI_BSSID_LEN];

    //! A set of flags from CHRE_WIFI_SCAN_RESULT_FLAGS_*
    uint8_t flags;

    //! RSSI (Received Signal Strength Indicator), in dBm. Typically negative.
    int8_t  rssi;

    //! Operating band, set to a value from enum chreWifiBand
    uint8_t band;

    /**
     * Indicates the center frequency of the primary 20MHz channel, given in
     * MHz. This value is derived from the channel number via the formula:
     *
     *     primaryChannel (MHz) = CSF + 5 * primaryChannelNumber
     *
     * Where CSF is the channel starting frequency (in MHz) given by the
     * operating class/band (i.e. 2407 or 5000), and primaryChannelNumber is the
     * channel number in the range [1, 200].
     *
     * Refer to VHT 22.3.14.
     */
    uint32_t primaryChannel;

    /**
     * If the channel width is 20 MHz, this field is not relevant and set to 0.
     * If the channel width is 40, 80, or 160 MHz, then this denotes the channel
     * center frequency (in MHz). If the channel is 80+80 MHz, then this denotes
     * the center frequency of segment 0, which contains the primary channel.
     * This value is derived from the frequency index using the same formula as
     * for primaryChannel.
     *
     * Refer to VHT 8.4.2.161, and VHT 22.3.14.
     *
     * @see #primaryChannel
     */
    uint32_t centerFreqPrimary;

    /**
     * If the channel width is 80+80MHz, then this denotes the center frequency
     * of segment 1, which does not contain the primary channel. Otherwise, this
     * field is not relevant and set to 0.
     *
     * @see #centerFreqPrimary
     */
    uint32_t centerFreqSecondary;

    //! @see #chreWifiChannelWidth
    uint8_t channelWidth;

    //! Flags from CHRE_WIFI_SECURITY_MODE_* indicating supported authentication
    //! and associated security modes
    //! @see CHRE_WIFI_SECURITY_MODE_FLAGS
    uint8_t securityMode;

    //! Reserved; set to 0
    uint8_t reserved[10];
};

/**
 * Data structure sent with events of type CHRE_EVENT_WIFI_SCAN_RESULT.
 */
struct chreWifiScanEvent {
    //! Indicates the version of the structure, for compatibility purposes.
    //! Clients do not normally need to worry about this field; the CHRE
    //! implementation guarantees that the client only receives the structure
    //! version it expects.
    uint8_t version;

    //! The number of entries in the results array in this event. The CHRE
    //! implementation may split scan results across multiple events for memory
    //! concerns, etc.
    uint8_t resultCount;

    //! The total number of results returned by the scan. Allows an event
    //! consumer to identify when it has received all events associated with a
    //! scan.
    uint8_t resultTotal;

    //! Sequence number for this event within the series of events comprising a
    //! complete scan result. Scan events are delivered strictly in order, i.e.
    //! this is monotonically increasing for the results of a single scan. Valid
    //! range [0, <number of events for scan> - 1]. The number of events for a
    //! scan is typically given by
    //! ceil(resultTotal / <max results per event supported by platform>).
    uint8_t eventIndex;

    //! A value from enum chreWifiScanType indicating the type of scan performed
    uint8_t scanType;

    //! If a directed scan was performed to a limited set of SSIDs, then this
    //! identifies the number of unique SSIDs included in the probe requests.
    //! Otherwise, this is set to 0, indicating that the scan was not limited by
    //! SSID. Note that if this is non-zero, the list of SSIDs used is not
    //! included in the scan event.
    uint8_t ssidSetSize;

    //! If 0, indicates that all frequencies applicable for the scanType were
    //! scanned. Otherwise, indicates the number of frequencies scanned, as
    //! specified in scannedFreqList.
    uint16_t scannedFreqListLen;

    //! Timestamp when the scan was completed, from the same time base as
    //! chreGetTime() (in nanoseconds)
    uint64_t referenceTime;

    //! Pointer to an array containing scannedFreqListLen values comprising the
    //! set of frequencies that were scanned. Frequencies are specified as
    //! channel center frequencies in MHz. May be NULL if scannedFreqListLen is
    //! 0.
    const uint32_t *scannedFreqList;

    //! Pointer to an array containing resultCount entries. May be NULL if
    //! resultCount is 0.
    const struct chreWifiScanResult *results;
};

/**
 * Retrieves a set of flags indicating the WiFi features supported by the
 * current CHRE implementation. The value returned by this function must be
 * consistent for the entire duration of the Nanoapp's execution.
 *
 * The client must allow for more flags to be set in this response than it knows
 * about, for example if the implementation supports a newer version of the API
 * than the client was compiled against.
 *
 * @return A bitmask with zero or more CHRE_WIFI_CAPABILITIES_* flags set
 *
 * @since v1.1
 */
uint32_t chreWifiGetCapabilities(void);

/**
 * Manages a client's request to receive the results of WiFi scans performed for
 * other purposes, for example scans done to maintain connectivity and scans
 * requested by other clients. The presence of this request has no effect on the
 * frequency or configuration of the WiFi scans performed - it is purely a
 * registration by the client to receive the results of scans that would
 * otherwise occur normally. This should include all available scan results,
 * including those that are not normally sent to the applications processor,
 * such as Preferred Network Offload (PNO) scans. Scan results provided because
 * of this registration must not contain cached results - they are always
 * expected to contain the fresh results from a recent scan.
 *
 * An active scan monitor subscription must persist across temporary conditions
 * under which no WiFi scans will be performed, for example if WiFi is
 * completely disabled via user-controlled settings, or if the WiFi system
 * restarts independently of CHRE. Likewise, a request to enable a scan monitor
 * subscription must succeed under normal conditions, even in circumstances
 * where no WiFi scans will be performed. In these cases, the scan monitor
 * implementation must produce scan results once the temporary condition is
 * cleared, for example after WiFi is enabled by the user.
 *
 * These scan results are delivered to the Nanoapp's handle event callback using
 * CHRE_EVENT_WIFI_SCAN_RESULT.
 *
 * An active scan monitor subscription is not necessary to receive the results
 * of an on-demand scan request sent via chreWifiRequestScanAsync().
 *
 * This result of this request is delivered asynchronously via an event of type
 * CHRE_EVENT_WIFI_ASYNC_RESULT. Refer to the note in {@link #chreAsyncResult}
 * for more details.
 *
 * @param enable Set to true to enable monitoring scan results, false to
 *        disable
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
bool chreWifiConfigureScanMonitorAsync(bool enable, const void *cookie);

/**
 * Sends an on-demand request for WiFi scan results. This may trigger a new
 * scan, or be entirely serviced from cache, depending on the maxScanAgeMs
 * parameter.
 *
 * This resulting status of this request is delivered asynchronously via an
 * event of type CHRE_EVENT_WIFI_ASYNC_RESULT. The result must be delivered
 * within CHRE_WIFI_SCAN_RESULT_TIMEOUT_NS of the this request. Refer to the
 * note in {@link #chreAsyncResult} for more details.
 *
 * A successful result provided in CHRE_EVENT_WIFI_ASYNC_RESULT indicates that
 * the scan results are ready to be delivered in a subsequent event (or events,
 * which arrive consecutively without any other scan results in between)
 * of type CHRE_EVENT_WIFI_SCAN_RESULT.
 *
 * It is not valid for a client to request a new scan while a result is pending
 * based on a previous scan request from the same client. In this situation, the
 * CHRE implementation is expected to return a result with CHRE_ERROR_BUSY.
 * However, if a scan is currently pending or in progress due to a request from
 * another client, whether within the CHRE or otherwise, the implementation must
 * not fail the request for this reason. If the pending scan satisfies the
 * client's request parameters, then the implementation should use its results
 * to satisfy the request rather than scheduling a new scan.
 *
 * @param params A set of parameters for the scan request. Must not be NULL.
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
bool chreWifiRequestScanAsync(const struct chreWifiScanParams *params,
                              const void *cookie);

/**
 * Convenience function which calls chreWifiRequestScanAsync() with a default
 * set of scan parameters.
 *
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
inline bool chreWifiRequestScanAsyncDefault(const void *cookie) {
    struct chreWifiScanParams params = {};
    params.scanType         = CHRE_WIFI_SCAN_TYPE_ACTIVE;
    params.maxScanAgeMs     = 5000;  // 5 seconds
    params.frequencyListLen = 0;
    params.ssidListLen      = 0;
    return chreWifiRequestScanAsync(&params, cookie);
}

#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_WIFI_H_ */
