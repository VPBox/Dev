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

#ifndef _CHRE_GNSS_H_
#define _CHRE_GNSS_H_

/**
 * @file
 * Global Navigation Satellite System (GNSS) API.
 *
 * These structures and definitions are based on the Android N GPS HAL.
 * Refer to that header file (located at this path as of the time of this
 * comment: hardware/libhardware/include/hardware/gps.h) and associated
 * documentation for further details and explanations for these fields.
 * References in comments like "(ref: GnssAccumulatedDeltaRangeState)" map to
 * the relevant element in the GPS HAL where additional information can be
 * found.
 *
 * In general, the parts of this API that are taken from the GPS HAL follow the
 * naming conventions established in that interface rather than the CHRE API
 * conventions, in order to avoid confusion and enable code re-use where
 * applicable.
 */

#include <stdbool.h>
#include <stdint.h>

#include <chre/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The set of flags that may be returned by chreGnssGetCapabilities()
 * @defgroup CHRE_GNSS_CAPABILITIES
 * @{
 */

//! A lack of flags indicates that GNSS is not supported in this CHRE
#define CHRE_GNSS_CAPABILITIES_NONE          UINT32_C(0)

//! GNSS position fixes are supported via chreGnssLocationSessionStartAsync()
#define CHRE_GNSS_CAPABILITIES_LOCATION      UINT32_C(1 << 0)

//! GNSS raw measurements are supported via
//! chreGnssMeasurementSessionStartAsync()
#define CHRE_GNSS_CAPABILITIES_MEASUREMENTS  UINT32_C(1 << 1)

//! Location fixes supplied from chreGnssConfigurePassiveLocationListener()
//! are tapped in at the GNSS engine level, so they include additional fixes
//! such as those requested by the AP, and not just those requested by other
//! nanoapps within CHRE (which is the case when this flag is not set)
#define CHRE_GNSS_CAPABILITIES_GNSS_ENGINE_BASED_PASSIVE_LISTENER \
                                             UINT32_C(1 << 2)

/** @} */

/**
 * The current version of struct chreGnssDataEvent associated with this API
 */
#define CHRE_GNSS_DATA_EVENT_VERSION  UINT8_C(0)

/**
 * The maximum time the CHRE implementation is allowed to elapse before sending
 * an event with the result of an asynchronous request, unless specified
 * otherwise
 */
#define CHRE_GNSS_ASYNC_RESULT_TIMEOUT_NS  (5 * CHRE_NSEC_PER_SEC)

/**
 * Produce an event ID in the block of IDs reserved for GNSS
 * @param offset  Index into GNSS event ID block; valid range [0,15]
 */
#define CHRE_GNSS_EVENT_ID(offset)  (CHRE_EVENT_GNSS_FIRST_EVENT + (offset))

/**
 * nanoappHandleEvent argument: struct chreAsyncResult
 *
 * Communicates the asynchronous result of a request to the GNSS API, such as
 * starting a location session via chreGnssLocationSessionStartAsync(). The
 * requestType field in chreAsyncResult is set to a value from enum
 * chreGnssRequestType.
 */
#define CHRE_EVENT_GNSS_ASYNC_RESULT  CHRE_GNSS_EVENT_ID(0)

/**
 * nanoappHandleEvent argument: struct chreGnssLocationEvent
 *
 * Represents a location fix provided by the GNSS subsystem.
 */
#define CHRE_EVENT_GNSS_LOCATION      CHRE_GNSS_EVENT_ID(1)

/**
 * nanoappHandleEvent argument: struct chreGnssDataEvent
 *
 * Represents a set of GNSS measurements with associated clock data.
 */
#define CHRE_EVENT_GNSS_DATA          CHRE_GNSS_EVENT_ID(2)

// NOTE: Do not add new events with ID > 15; only values 0-15 are reserved
// (see chre/event.h)

// Flags indicating the Accumulated Delta Range's states
// (ref: GnssAccumulatedDeltaRangeState)
#define CHRE_GNSS_ADR_STATE_UNKNOWN     UINT16_C(0)
#define CHRE_GNSS_ADR_STATE_VALID       UINT16_C(1 << 0)
#define CHRE_GNSS_ADR_STATE_RESET       UINT16_C(1 << 1)
#define CHRE_GNSS_ADR_STATE_CYCLE_SLIP  UINT16_C(1 << 2)

// Flags to indicate what fields in chreGnssClock are valid (ref: GnssClockFlags)
#define CHRE_GNSS_CLOCK_HAS_LEAP_SECOND        UINT16_C(1 << 0)
#define CHRE_GNSS_CLOCK_HAS_TIME_UNCERTAINTY   UINT16_C(1 << 1)
#define CHRE_GNSS_CLOCK_HAS_FULL_BIAS          UINT16_C(1 << 2)
#define CHRE_GNSS_CLOCK_HAS_BIAS               UINT16_C(1 << 3)
#define CHRE_GNSS_CLOCK_HAS_BIAS_UNCERTAINTY   UINT16_C(1 << 4)
#define CHRE_GNSS_CLOCK_HAS_DRIFT              UINT16_C(1 << 5)
#define CHRE_GNSS_CLOCK_HAS_DRIFT_UNCERTAINTY  UINT16_C(1 << 6)

// Flags to indicate which values are valid in a GpsLocation
// (ref: GpsLocationFlags)
#define CHRE_GPS_LOCATION_HAS_LAT_LONG  UINT16_C(1 << 0)
#define CHRE_GPS_LOCATION_HAS_ALTITUDE  UINT16_C(1 << 1)
#define CHRE_GPS_LOCATION_HAS_SPEED     UINT16_C(1 << 2)
#define CHRE_GPS_LOCATION_HAS_BEARING   UINT16_C(1 << 3)
#define CHRE_GPS_LOCATION_HAS_ACCURACY  UINT16_C(1 << 4)

/**
 * The maximum number of instances of struct chreGnssMeasurement that may be
 * included in a single struct chreGnssDataEvent.
 */
#define CHRE_GNSS_MAX_MEASUREMENT  UINT8_C(64)

// Flags indicating the GNSS measurement state (ref: GnssMeasurementState)
#define CHRE_GNSS_MEASUREMENT_STATE_UNKNOWN                UINT16_C(0)
#define CHRE_GNSS_MEASUREMENT_STATE_CODE_LOCK              UINT16_C(1 << 0)
#define CHRE_GNSS_MEASUREMENT_STATE_BIT_SYNC               UINT16_C(1 << 1)
#define CHRE_GNSS_MEASUREMENT_STATE_SUBFRAME_SYNC          UINT16_C(1 << 2)
#define CHRE_GNSS_MEASUREMENT_STATE_TOW_DECODED            UINT16_C(1 << 3)
#define CHRE_GNSS_MEASUREMENT_STATE_MSEC_AMBIGUOUS         UINT16_C(1 << 4)
#define CHRE_GNSS_MEASUREMENT_STATE_SYMBOL_SYNC            UINT16_C(1 << 5)
#define CHRE_GNSS_MEASUREMENT_STATE_GLO_STRING_SYNC        UINT16_C(1 << 6)
#define CHRE_GNSS_MEASUREMENT_STATE_GLO_TOD_DECODED        UINT16_C(1 << 7)
#define CHRE_GNSS_MEASUREMENT_STATE_BDS_D2_BIT_SYNC        UINT16_C(1 << 8)
#define CHRE_GNSS_MEASUREMENT_STATE_BDS_D2_SUBFRAME_SYNC   UINT16_C(1 << 9)
#define CHRE_GNSS_MEASUREMENT_STATE_GAL_E1BC_CODE_LOCK     UINT16_C(1 << 10)
#define CHRE_GNSS_MEASUREMENT_STATE_GAL_E1C_2ND_CODE_LOCK  UINT16_C(1 << 11)
#define CHRE_GNSS_MEASUREMENT_STATE_GAL_E1B_PAGE_SYNC      UINT16_C(1 << 12)
#define CHRE_GNSS_MEASUREMENT_STATE_SBAS_SYNC              UINT16_C(1 << 13)


/**
 * Indicates a type of request made in this API. Used to populate the resultType
 * field of struct chreAsyncResult sent with CHRE_EVENT_GNSS_ASYNC_RESULT.
 */
enum chreGnssRequestType {
    CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_START    = 1,
    CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_STOP     = 2,
    CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_START = 3,
    CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_STOP  = 4,
};

/**
 * Constellation type associated with an SV
 */
enum chreGnssConstellationType {
    CHRE_GNSS_CONSTELLATION_UNKNOWN = 0,
    CHRE_GNSS_CONSTELLATION_GPS     = 1,
    CHRE_GNSS_CONSTELLATION_SBAS    = 2,
    CHRE_GNSS_CONSTELLATION_GLONASS = 3,
    CHRE_GNSS_CONSTELLATION_QZSS    = 4,
    CHRE_GNSS_CONSTELLATION_BEIDOU  = 5,
    CHRE_GNSS_CONSTELLATION_GALILEO = 6,
};

/**
 * Enumeration of available values for the chreGnssMeasurement multipath indicator
 */
enum chreGnssMultipathIndicator {
    //! The indicator is not available or unknown
    CHRE_GNSS_MULTIPATH_INDICATOR_UNKNOWN     = 0,
    //! The measurement is indicated to be affected by multipath
    CHRE_GNSS_MULTIPATH_INDICATOR_PRESENT     = 1,
    //! The measurement is indicated to be not affected by multipath
    CHRE_GNSS_MULTIPATH_INDICATOR_NOT_PRESENT = 2,
};

/**
 * Represents an estimate of the GNSS clock time (see the Android GPS HAL for
 * more detailed information)
 */
struct chreGnssClock {
    //! The GNSS receiver hardware clock value in nanoseconds, including
    //! uncertainty
    int64_t time_ns;

    //! The difference between hardware clock inside GNSS receiver and the
    //! estimated GNSS time in nanoseconds; contains bias uncertainty
    int64_t full_bias_ns;

    //! Sub-nanosecond bias, adds to full_bias_ns
    float bias_ns;

    //! The clock's drift in nanoseconds per second
    float drift_nsps;

    //! 1-sigma uncertainty associated with the clock's bias in nanoseconds
    float bias_uncertainty_ns;

    //! 1-sigma uncertainty associated with the clock's drift in nanoseconds
    //! per second
    float drift_uncertainty_nsps;

    //! While this number stays the same, timeNs should flow continuously
    uint32_t hw_clock_discontinuity_count;

    //! A set of flags indicating the validity of the fields in this data
    //! structure (see GNSS_CLOCK_HAS_*)
    uint16_t flags;

    //! Reserved for future use; set to 0
    uint8_t reserved[2];
};

/**
 * Represents a GNSS measurement; contains raw and computed information (see the
 * Android GPS HAL for more detailed information)
 */
struct chreGnssMeasurement {
    //! Hardware time offset from time_ns for this measurement, in nanoseconds
    int64_t time_offset_ns;

    //! Accumulated delta range since the last channel reset in micro-meters
    int64_t accumulated_delta_range_um;

    //! Received GNSS satellite time at the time of measurement, in nanoseconds
    int64_t received_sv_time_in_ns;

    //! 1-sigma uncertainty of received GNSS satellite time, in nanoseconds
    int64_t received_sv_time_uncertainty_in_ns;

    //! Pseudorange rate at the timestamp in meters per second (uncorrected)
    float pseudorange_rate_mps;

    //! 1-sigma uncertainty of pseudorange rate in meters per second
    float pseudorange_rate_uncertainty_mps;

    //! 1-sigma uncertainty of the accumulated delta range in meters
    float accumulated_delta_range_uncertainty_m;

    //! Carrier-to-noise density in dB-Hz, in the range of [0, 63]
    float c_n0_dbhz;

    //! Signal to noise ratio (dB), power above observed noise at correlators
    float snr_db;

    //! Satellite sync state flags (GNSS_MEASUREMENT_STATE_*) - sets modulus for
    //! received_sv_time_in_ns
    uint16_t state;

    //! Set of ADR state flags (GNSS_ADR_STATE_*)
    uint16_t accumulated_delta_range_state;

    //! Satellite vehicle ID number
    int16_t svid;

    //! Constellation of the given satellite vehicle
    //! @see #chreGnssConstellationType
    uint8_t constellation;

    //! @see #chreGnssMultipathIndicator
    uint8_t multipath_indicator;

    //! Reserved for future use; set to 0
    uint8_t reserved[4];
};

/**
 * Data structure sent with events associated with CHRE_EVENT_GNSS_DATA, enabled
 * via chreGnssMeasurementSessionStartAsync()
 */
struct chreGnssDataEvent {
    //! Indicates the version of the structure, for compatibility purposes.
    //! Clients do not normally need to worry about this field; the CHRE
    //! implementation guarantees that it only sends the client the structure
    //! version it expects.
    uint8_t version;

    //! Number of chreGnssMeasurement entries included in this event. Must be in
    //! the range [0, CHRE_GNSS_MAX_MEASUREMENT]
    uint8_t measurement_count;

    //! Reserved for future use; set to 0
    uint8_t reserved[6];

    struct chreGnssClock clock;

    //! Pointer to an array containing measurement_count measurements
    const struct chreGnssMeasurement *measurements;
};

/**
 * Data structure sent with events of type CHRE_EVENT_GNSS_LOCATION, enabled via
 * chreGnssLocationSessionStartAsync(). This is modeled after GpsLocation in the
 * GPS HAL, but does not use the double data type.
 */
struct chreGnssLocationEvent {
    //! UTC timestamp for location fix in milliseconds since January 1, 1970
    uint64_t timestamp;

    //! Fixed point latitude, degrees times 10^7 (roughly centimeter resolution)
    int32_t latitude_deg_e7;

    //! Fixed point longitude, degrees times 10^7 (roughly centimeter
    //! resolution)
    int32_t longitude_deg_e7;

    //! Altitude in meters above the WGS 84 reference ellipsoid
    float altitude;

    //! Horizontal speed in meters per second
    float speed;

    //! Clockwise angle between north and current heading, in degrees; range
    //! [0, 360)
    float bearing;

    //! Expected horizontal accuracy in meters such that a circle with a radius
    //! of length 'accuracy' from the latitude and longitude has a 68%
    //! probability of including the true location. Use 0.0 if there is no
    //! accuracy.
    float accuracy;

    //! A set of flags indicating which fields in this structure are valid
    //! @see #GpsLocationFlags
    uint16_t flags;
};


/**
 * Retrieves a set of flags indicating the GNSS features supported by the
 * current CHRE implementation. The value returned by this function must be
 * consistent for the entire duration of the Nanoapp's execution.
 *
 * The client must allow for more flags to be set in this response than it knows
 * about, for example if the implementation supports a newer version of the API
 * than the client was compiled against.
 *
 * @return A bitmask with zero or more CHRE_GNSS_CAPABILITIES_* flags set
 *
 * @since v1.1
 */
uint32_t chreGnssGetCapabilities(void);

/**
 * Initiates a GNSS positioning session, or changes the requested interval of an
 * existing session. If starting or modifying the session was successful, then
 * the GNSS engine will work on determining the device's position.
 *
 * This result of this request is delivered asynchronously via an event of type
 * CHRE_EVENT_GNSS_ASYNC_RESULT. Refer to the note in {@link #chreAsyncResult}
 * for more details.
 *
 * If chreGnssGetCapabilities() returns a value that does not have the
 * CHRE_GNSS_CAPABILITIES_LOCATION flag set, then this method will return false.
 *
 * @param minIntervalMs The desired minimum interval between location fixes
 *        delivered to the client via CHRE_EVENT_GNSS_LOCATION, in milliseconds.
 *        The requesting client must allow for fixes to be delivered at shorter
 *        or longer interval than requested. For example, adverse RF conditions
 *        may result in fixes arriving at a longer interval, etc.
 * @param minTimeToNextFixMs The desired minimum time to the next location fix.
 *        If this is 0, the GNSS engine should start working on the next fix
 *        immediately. If greater than 0, the GNSS engine should not spend
 *        measurable power to produce a location fix until this amount of time
 *        has elapsed.
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
bool chreGnssLocationSessionStartAsync(uint32_t minIntervalMs,
                                       uint32_t minTimeToNextFixMs,
                                       const void *cookie);

/**
 * Terminates an existing GNSS positioning session. If no positioning session
 * is active at the time of this request, it is treated as if an active session
 * was successfully ended.
 *
 * This result of this request is delivered asynchronously via an event of type
 * CHRE_EVENT_GNSS_ASYNC_RESULT. Refer to the note in {@link #chreAsyncResult}
 * for more details.
 *
 * After CHRE_EVENT_GNSS_ASYNC_RESULT is delivered to the client, no more
 * CHRE_EVENT_GNSS_LOCATION events will be delievered until a new location
 * session is started.
 *
 * If chreGnssGetCapabilities() returns a value that does not have the
 * CHRE_GNSS_CAPABILITIES_LOCATION flag set, then this method will return false.
 *
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
bool chreGnssLocationSessionStopAsync(const void *cookie);

/**
 * Initiates a request to receive raw GNSS measurements. A GNSS measurement
 * session can exist independently of location sessions. In other words, a
 * Nanoapp is able to receive measurements at its requested interval both with
 * and without an active location session.
 *
 * This result of this request is delivered asynchronously via an event of type
 * CHRE_EVENT_GNSS_ASYNC_RESULT. Refer to the note in {@link #chreAsyncResult}
 * for more details.
 *
 * If chreGnssGetCapabilities() returns a value that does not have the
 * CHRE_GNSS_CAPABILITIES_MEASUREMENTS flag set, then this method will return
 * false.
 *
 * @param minIntervalMs The desired minimum interval between measurement reports
 *        delivered via CHRE_EVENT_GNSS_DATA. When requested at 1000ms or
 *        faster, and GNSS measurements are tracked, device should report
 *        measurements as fast as requested, and shall report no slower than
 *        once every 1000ms, on average.
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
bool chreGnssMeasurementSessionStartAsync(uint32_t minIntervalMs,
                                          const void *cookie);

/**
 * Terminates an existing raw GNSS measurement session. If no measurement
 * session is active at the time of this request, it is treated as if an active
 * session was successfully ended.
 *
 * This result of this request is delivered asynchronously via an event of type
 * CHRE_EVENT_GNSS_ASYNC_RESULT. Refer to the note in {@link #chreAsyncResult}
 * for more details.
 *
 * If chreGnssGetCapabilities() returns a value that does not have the
 * CHRE_GNSS_CAPABILITIES_MEASUREMENTS flag set, then this method will return
 * false.
 *
 * @param cookie An opaque value that will be included in the chreAsyncResult
 *        sent in relation to this request.
 *
 * @return true if the request was accepted for processing, false otherwise
 *
 * @since v1.1
 */
bool chreGnssMeasurementSessionStopAsync(const void *cookie);

/**
 * Controls whether this nanoapp will passively receive GNSS-based location
 * fixes produced as a result of location sessions initiated by other entities.
 * This function allows a nanoapp to opportunistically receive location fixes
 * via CHRE_EVENT_GNSS_LOCATION events without imposing additional power cost,
 * though with no guarantees as to when or how often those events will arrive.
 * There will be no duplication of events if a passive location listener and
 * location session are enabled in parallel.
 *
 * Enabling passive location listening is not required to receive events for an
 * active location session started via chreGnssLocationSessionStartAsync(). This
 * setting is independent of the active location session, so modifying one does
 * not have an effect on the other.
 *
 * If chreGnssGetCapabilities() returns a value that does not have the
 * CHRE_GNSS_CAPABILITIES_LOCATION flag set or the value returned by
 * chreGetApiVersion() is less than CHRE_API_VERSION_1_2, then this method will
 * return false.
 *
 * If chreGnssGetCapabilities() includes
 * CHRE_GNSS_CAPABILITIES_GNSS_ENGINE_BASED_PASSIVE_LISTENER, the passive
 * registration is recorded at the GNSS engine level, so events include fixes
 * requested by the applications processor and potentially other non-CHRE
 * clients. If this flag is not set, then only fixes requested by other nanoapps
 * within CHRE are provided.
 *
 * @param enable true to receive opportunistic location fixes, false to disable
 *
 * @return true if the configuration was processed successfully, false on error
 *     or if this feature is not supported
 *
 * @since v1.2
 */
bool chreGnssConfigurePassiveLocationListener(bool enable);

#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_GNSS_H_ */
