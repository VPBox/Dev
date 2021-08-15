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

#ifndef _CHRE_SENSOR_H_
#define _CHRE_SENSOR_H_

/**
 * @file
 * API dealing with sensor interaction in the Context Hub Runtime
 * Environment.
 *
 * This includes the definition of our sensor types and the ability to
 * configure them for receiving events.
 */

#include <stdbool.h>
#include <stdint.h>

#include <chre/event.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The CHRE_SENSOR_TYPE_* defines are the sensor types supported.
 *
 * Unless otherwise noted, each of these sensor types is based off of a
 * corresponding sensor type in the Android API's sensors.h interface.
 * For a given CHRE_SENSOR_TYPE_FOO, it corresponds to the SENSOR_TYPE_FOO in
 * hardware/libhardware/include/hardware/sensors.h of the Android code base.
 *
 * Unless otherwise noted below, a CHRE_SENSOR_TYPE_FOO should be assumed
 * to work the same as the Android SENSOR_TYPE_FOO, as documented in the
 * sensors.h documentation and as detailed within the Android Compatibility
 * Definition Document.
 *
 * Note that every sensor will generate CHRE_EVENT_SENSOR_SAMPLING_CHANGE
 * events, so it is not listed with each individual sensor.
 */

/**
 * Accelerometer.
 *
 * Generates: CHRE_EVENT_SENSOR_ACCELEROMETER_DATA
 *
 * Note that the ACCELEROMETER_DATA is always the fully calibrated data,
 * including factory calibration and runtime calibration if available.
 */
#define CHRE_SENSOR_TYPE_ACCELEROMETER  UINT8_C(1)

/**
 * Instantaneous motion detection.
 *
 * Generates: CHRE_EVENT_SENSOR_INSTANT_MOTION_DETECT_DATA
 *
 * This is a one-shot sensor.
 *
 * This does not have a direct analogy within sensors.h.  This is similar
 * to SENSOR_TYPE_MOTION_DETECT, but this triggers instantly upon any
 * motion, instead of waiting for a period of continuous motion.
 */
#define CHRE_SENSOR_TYPE_INSTANT_MOTION_DETECT  UINT8_C(2)

/**
 * Stationary detection.
 *
 * Generates: CHRE_EVENT_SENSOR_STATIONARY_DETECT_DATA
 *
 * This is a one-shot sensor.
 */
#define CHRE_SENSOR_TYPE_STATIONARY_DETECT  UINT8_C(3)

/**
 * Gyroscope.
 *
 * Generates: CHRE_EVENT_SENSOR_GYROSCOPE_DATA and
 *     optionally CHRE_EVENT_SENSOR_GYROSCOPE_BIAS_INFO
 *
 * Note that the GYROSCOPE_DATA is always the fully calibrated data, including
 * factory calibration and runtime calibration if available.
 */
#define CHRE_SENSOR_TYPE_GYROSCOPE  UINT8_C(6)

/**
 * Uncalibrated gyroscope.
 *
 * Generates: CHRE_EVENT_SENSOR_UNCALIBRATED_GYROSCOPE_DATA
 *
 * Note that the UNCALIBRATED_GYROSCOPE_DATA must be factory calibrated data,
 * but not runtime calibrated.
 */
#define CHRE_SENSOR_TYPE_UNCALIBRATED_GYROSCOPE  UINT8_C(7)

/**
 * Magnetometer.
 *
 * Generates: CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_DATA and
 *     optionally CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_BIAS_INFO
 *
 * Note that the GEOMAGNETIC_FIELD_DATA is always the fully calibrated data,
 * including factory calibration and runtime calibration if available.
 */
#define CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD  UINT8_C(8)

/**
 * Uncalibrated magnetometer.
 *
 * Generates: CHRE_EVENT_SENSOR_UNCALIBRATED_GEOMAGNETIC_FIELD_DATA
 *
 * Note that the UNCALIBRATED_GEOMAGNETIC_FIELD_DATA must be factory calibrated
 * data, but not runtime calibrated.
 */
#define CHRE_SENSOR_TYPE_UNCALIBRATED_GEOMAGNETIC_FIELD  UINT8_C(9)

/**
 * Barometric pressure sensor.
 *
 * Generates: CHRE_EVENT_SENSOR_PRESSURE_DATA
 */
#define CHRE_SENSOR_TYPE_PRESSURE  UINT8_C(10)

/**
 * Ambient light sensor.
 *
 * Generates: CHRE_EVENT_SENSOR_LIGHT_DATA
 *
 * This is an on-change sensor.
 */
#define CHRE_SENSOR_TYPE_LIGHT  UINT8_C(12)

/**
 * Proximity detection.
 *
 * Generates: CHRE_EVENT_SENSOR_PROXIMITY_DATA
 *
 * This is an on-change sensor.
 */
#define CHRE_SENSOR_TYPE_PROXIMITY  UINT8_C(13)

/**
 * Uncalibrated accelerometer.
 *
 * Generates: CHRE_EVENT_SENSOR_UNCALIBRATED_ACCELEROMETER_DATA
 *
 * Note that the UNCALIBRATED_ACCELEROMETER_DATA must be factory calibrated
 * data, but not runtime calibrated.
 */
#define CHRE_SENSOR_TYPE_UNCALIBRATED_ACCELEROMETER  UINT8_C(55)

/**
 * Accelerometer temperature.
 *
 * Generates: CHRE_EVENT_SENSOR_ACCELEROMETER_TEMPERATURE_DATA
 */
#define CHRE_SENSOR_TYPE_ACCELEROMETER_TEMPERATURE  UINT8_C(56)

/**
 * Gyroscope temperature.
 *
 * Generates: CHRE_EVENT_SENSOR_GYROSCOPE_TEMPERATURE_DATA
 */
#define CHRE_SENSOR_TYPE_GYROSCOPE_TEMPERATURE  UINT8_C(57)

/**
 * Magnetometer temperature.
 *
 * Generates: CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_TEMPERATURE_DATA
 */
#define CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD_TEMPERATURE  UINT8_C(58)

/**
 * Base value for all of the data events for sensors.
 *
 * The value for a data event FOO is
 * CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_FOO
 *
 * This allows for easy mapping, and also explains why there are gaps
 * in our values since we don't have all possible sensor types assigned.
 */
#define CHRE_EVENT_SENSOR_DATA_EVENT_BASE  CHRE_EVENT_SENSOR_FIRST_EVENT

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x', 'y', and 'z' fields within
 * 'readings', or by the 3D array 'v' (v[0] == x; v[1] == y; v[2] == z).
 *
 * All values are in SI units (m/s^2) and measure the acceleration applied to
 * the device.
 */
#define CHRE_EVENT_SENSOR_ACCELEROMETER_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_ACCELEROMETER)

/**
 * nanoappHandleEvent argument: struct chreSensorOccurrenceData
 *
 * Since this is a one-shot sensor, after this event is delivered to the
 * nanoapp, the sensor automatically goes into DONE mode.  Sensors of this
 * type must be configured with a ONE_SHOT mode.
 */
#define CHRE_EVENT_SENSOR_INSTANT_MOTION_DETECT_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_INSTANT_MOTION_DETECT)

/**
 * nanoappHandleEvent argument: struct chreSensorOccurrenceData
 *
 * Since this is a one-shot sensor, after this event is delivered to the
 * nanoapp, the sensor automatically goes into DONE mode.  Sensors of this
 * type must be configured with a ONE_SHOT mode.
 */
#define CHRE_EVENT_SENSOR_STATIONARY_DETECT_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_STATIONARY_DETECT)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x', 'y', and 'z' fields within
 * 'readings', or by the 3D array 'v' (v[0] == x; v[1] == y; v[2] == z).
 *
 * All values are in radians/second and measure the rate of rotation
 * around the X, Y and Z axis.
 */
#define CHRE_EVENT_SENSOR_GYROSCOPE_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_GYROSCOPE)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x', 'y', and 'z' fields within
 * 'readings', or by the 3D array 'v' (v[0] == x; v[1] == y; v[2] == z).
 *
 * All values are in micro-Tesla (uT) and measure the geomagnetic
 * field in the X, Y and Z axis.
 */
#define CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD)

/**
 * nanoappHandleEvent argument: struct chreSensorFloatData
 *
 * The data can be interpreted using the 'pressure' field within 'readings'.
 * This value is in hectopascals (hPa).
 */
#define CHRE_EVENT_SENSOR_PRESSURE_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_PRESSURE)

/**
 * nanoappHandleEvent argument: struct chreSensorFloatData
 *
 * The data can be interpreted using the 'light' field within 'readings'.
 * This value is in SI lux units.
 */
#define CHRE_EVENT_SENSOR_LIGHT_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_LIGHT)

/**
 * nanoappHandleEvent argument: struct chreSensorByteData
 *
 * The data is interpreted from the following fields in 'readings':
 * o 'isNear': If set to 1, we are nearby (on the order of centimeters);
 *       if set to 0, we are far.
 * o 'invalid': If set to 1, this is not a valid reading of this data.
 *
 * As an on-change sensor, there can be an event generated upon configuring
 * this sensor.  This is when we might get an 'invalid' reading.  Thus,
 * this field must be checked on the first event before interpreting 'isNear'.
 */
#define CHRE_EVENT_SENSOR_PROXIMITY_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_PROXIMITY)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x', 'y', and 'z' fields within
 * 'readings', or by the 3D array 'v' (v[0] == x; v[1] == y; v[2] == z).
 *
 * All values are in SI units (m/s^2) and measure the acceleration applied to
 * the device.
 */
#define CHRE_EVENT_SENSOR_UNCALIBRATED_ACCELEROMETER_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_UNCALIBRATED_ACCELEROMETER)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x', 'y', and 'z' fields within
 * 'readings', or by the 3D array 'v' (v[0] == x; v[1] == y; v[2] == z).
 *
 * All values are in radians/second and measure the rate of rotation
 * around the X, Y and Z axis.
 */
#define CHRE_EVENT_SENSOR_UNCALIBRATED_GYROSCOPE_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_UNCALIBRATED_GYROSCOPE)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x', 'y', and 'z' fields within
 * 'readings', or by the 3D array 'v' (v[0] == x; v[1] == y; v[2] == z).
 *
 * All values are in micro-Tesla (uT) and measure the geomagnetic
 * field in the X, Y and Z axis.
 */
#define CHRE_EVENT_SENSOR_UNCALIBRATED_GEOMAGNETIC_FIELD_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_UNCALIBRATED_GEOMAGNETIC_FIELD)

/**
 * nanoappHandleEvent argument: struct chreSensorFloatData
 *
 * The data can be interpreted using the 'temperature' field within 'readings'.
 * This value is in degrees Celsius.
 */
#define CHRE_EVENT_SENSOR_ACCELEROMETER_TEMPERATURE_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_ACCELEROMETER_TEMPERATURE)

/**
 * nanoappHandleEvent argument: struct chreSensorFloatData
 *
 * The data can be interpreted using the 'temperature' field within 'readings'.
 * This value is in degrees Celsius.
 */
#define CHRE_EVENT_SENSOR_GYROSCOPE_TEMPERATURE_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_GYROSCOPE_TEMPERATURE)

/**
 * nanoappHandleEvent argument: struct chreSensorFloatData
 *
 * The data can be interpreted using the 'temperature' field within 'readings'.
 * This value is in degrees Celsius.
 */
#define CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_TEMPERATURE_DATA \
    (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD_TEMPERATURE)

/**
 * First value for sensor events which are not data from the sensor.
 *
 * Unlike the data event values, these other event values don't have any
 * mapping to sensor types.
 */
#define CHRE_EVENT_SENSOR_OTHER_EVENTS_BASE \
    (CHRE_EVENT_SENSOR_FIRST_EVENT + 0x0100)

/**
 * nanoappHandleEvent argument: struct chreSensorSamplingStatusEvent
 *
 * Indicates that the interval and/or the latency which this sensor is
 * sampling at has changed.
 */
#define CHRE_EVENT_SENSOR_SAMPLING_CHANGE \
    (CHRE_EVENT_SENSOR_OTHER_EVENTS_BASE + 0)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x_bias', 'y_bias', and 'z_bias'
 * field within 'readings', or by the 3D array 'bias' (bias[0] == x_bias;
 * bias[1] == y_bias; bias[2] == z_bias).
 *
 * All values are in radians/second and measure the rate of rotation
 * around the X, Y and Z axis.
 */
#define CHRE_EVENT_SENSOR_GYROSCOPE_BIAS_INFO \
    (CHRE_EVENT_SENSOR_OTHER_EVENTS_BASE + 1)

/**
 * nanoappHandleEvent argument: struct chreSensorThreeAxisData
 *
 * The data can be interpreted using the 'x_bias', 'y_bias', and 'z_bias'
 * field within 'readings', or by the 3D array 'bias' (bias[0] == x_bias;
 * bias[1] == y_bias; bias[2] == z_bias).
 *
 * All values are in micro-Tesla (uT) and measure the geomagnetic
 * field in the X, Y and Z axis.
 */
#define CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_BIAS_INFO \
    (CHRE_EVENT_SENSOR_OTHER_EVENTS_BASE + 2)


#if CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_BIAS_INFO > CHRE_EVENT_SENSOR_LAST_EVENT
#error Too many sensor events.
#endif


/**
 * Value indicating we want the smallest possible latency for a sensor.
 *
 * This literally translates to 0 nanoseconds for the chreSensorConfigure()
 * argument.  While we won't get exactly 0 nanoseconds, the CHRE will
 * queue up this event As Soon As Possible.
 */
#define CHRE_SENSOR_LATENCY_ASAP  UINT64_C(0)

/**
 * Special value indicating non-importance, or non-applicability of the sampling
 * interval.
 *
 * @see chreSensorConfigure
 * @see chreSensorSamplingStatus
 */
#define CHRE_SENSOR_INTERVAL_DEFAULT  UINT64_C(-1)

/**
 * Special value indicating non-importance of the latency.
 *
 * @see chreSensorConfigure
 * @see chreSensorSamplingStatus
 */
#define CHRE_SENSOR_LATENCY_DEFAULT  UINT64_C(-1)

/**
 * Special value indicating non-importance of the batch interval.
 *
 * @see chreSensorConfigureWithBatchInterval
 */
#define CHRE_SENSOR_BATCH_INTERVAL_DEFAULT  UINT64_C(-1)

// This is used to define elements of enum chreSensorConfigureMode.
#define CHRE_SENSOR_CONFIGURE_RAW_POWER_ON           (1 << 0)

// This is used to define elements of enum chreSensorConfigureMode.
#define CHRE_SENSOR_CONFIGURE_RAW_REPORT_CONTINUOUS  (1 << 1)

// This is used to define elements of enum chreSensorConfigureMode.
#define CHRE_SENSOR_CONFIGURE_RAW_REPORT_ONE_SHOT    (2 << 1)


/**
 * Modes we can configure a sensor to use.
 *
 * Our mode will affect not only how/if we receive events, but
 * also whether or not the sensor will be powered on our behalf.
 *
 * @see chreSensorConfigure
 */
enum chreSensorConfigureMode {
    /**
     * Get events from the sensor.
     *
     * Power: Turn on if not already on.
     * Reporting: Continuous.  Send each new event as it comes (subject to
     *     batching and latency).
     */
    CHRE_SENSOR_CONFIGURE_MODE_CONTINUOUS =
        (CHRE_SENSOR_CONFIGURE_RAW_POWER_ON |
         CHRE_SENSOR_CONFIGURE_RAW_REPORT_CONTINUOUS),

    /**
     * Get a single event from the sensor and then become DONE.
     *
     * Once the event is sent, the sensor automatically
     * changes to CHRE_SENSOR_CONFIGURE_MODE_DONE mode.
     *
     * Power: Turn on if not already on.
     * Reporting: One shot.  Send the next event and then be DONE.
     */
    CHRE_SENSOR_CONFIGURE_MODE_ONE_SHOT =
        (CHRE_SENSOR_CONFIGURE_RAW_POWER_ON |
         CHRE_SENSOR_CONFIGURE_RAW_REPORT_ONE_SHOT),

    /**
     * Get events from a sensor that are generated for other apps.
     *
     * This is considered passive because the sensor will not be powered
     * on for the sake of our nanoapp.  If and only if another app in
     * the system has requested this sensor power on will we get events.
     *
     * This can be useful for something which is interested in seeing data,
     * but not interested enough to be responsible for powering on the sensor.
     *
     * Power: Do not power the sensor on our behalf.
     * Reporting: Continuous.  Send each event as it comes.
     */
    CHRE_SENSOR_CONFIGURE_MODE_PASSIVE_CONTINUOUS =
        CHRE_SENSOR_CONFIGURE_RAW_REPORT_CONTINUOUS,

    /**
     * Get a single event from a sensor that is generated for other apps.
     *
     * See CHRE_SENSOR_CONFIGURE_MODE_PASSIVE_CONTINUOUS for more details
     * on what be "passive" means.
     *
     * Power: Do not power the sensor on our behalf.
     * Reporting: One shot.  Send only the next event and then be DONE.
     */
    CHRE_SENSOR_CONFIGURE_MODE_PASSIVE_ONE_SHOT =
        CHRE_SENSOR_CONFIGURE_RAW_REPORT_ONE_SHOT,

    /**
     * Indicate we are done using this sensor and no longer interested in it.
     *
     * See chreSensorConfigure for more details on expressing interest or
     * lack of interest in a sensor.
     *
     * Power: Do not power the sensor on our behalf.
     * Reporting: None.
     */
    CHRE_SENSOR_CONFIGURE_MODE_DONE = 0,
};

/**
 * A structure containing information about a Sensor.
 *
 * See documentation of individual fields below.
 */
struct chreSensorInfo {
    /**
     * The name of the sensor.
     *
     * A text name, useful for logging/debugging, describing the Sensor.  This
     * is not assured to be unique (i.e. there could be multiple sensors with
     * the name "Temperature").
     *
     * CHRE implementations may not set this as NULL.  An empty
     * string, while discouraged, is legal.
     */
    const char *sensorName;

    /**
     * One of the CHRE_SENSOR_TYPE_* defines above.
     */
    uint8_t sensorType;

    /**
     * Flag indicating if this sensor is on-change.
     *
     * An on-change sensor only generates events when underlying state
     * changes.  This has the same meaning as on-change does in the Android
     * Sensors HAL.  See sensors.h for much more details.
     *
     * A value of 1 indicates this is on-change.  0 indicates this is not
     * on-change.
     */
    uint8_t isOnChange  : 1;

    /**
     * Flag indicating if this sensor is one-shot.
     *
     * A one-shot sensor only triggers a single event, and then automatically
     * disables itself.
     *
     * A value of 1 indicates this is one-shot.  0 indicates this is not
     * on-change.
     */
    uint8_t isOneShot   : 1;
    uint8_t unusedFlags : 6;

    /**
     * The minimum sampling interval supported by this sensor, in nanoseconds.
     *
     * Requests to chreSensorConfigure with a lower interval than this will
     * fail.  If the sampling interval is not applicable to this sensor, this
     * will be set to CHRE_SENSOR_INTERVAL_DEFAULT.
     *
     * This field will be set to 0 when running on CHRE API versions prior to
     * v1.1, indicating that the minimum interval is not known.
     *
     * @since v1.1
     */
    uint64_t minInterval;
};

/**
 * Header used in every structure containing batchable data from a sensor.
 *
 * The typical structure for sensor data looks like:
 *
 *   struct chreSensorTypeData {
 *       struct chreSensorDataHeader header;
 *       struct chreSensorTypeSampleData {
 *           uint32_t timestampDelta;
 *           union {
 *               <type> value;
 *               <type> interpretation0;
 *               <type> interpretation1;
 *           };
 *       } readings[1];
 *   };
 *
 * Despite 'readings' being declared as an array of 1 element,
 * an instance of the struct will actually have 'readings' as
 * an array of header.readingCount elements (which may be 1).
 * The 'timestampDelta' is in relation to the previous 'readings' (or
 * the baseTimestamp for readings[0].  So,
 * Timestamp for readings[0] == header.baseTimestamp +
 *     readings[0].timestampDelta.
 * Timestamp for readings[1] == timestamp for readings[0] +
 *     readings[1].timestampDelta.
 * And thus, in order to determine the timestamp for readings[N], it's
 * necessary to process through all of the N-1 readings.  The advantage,
 * though, is that our entire readings can span an arbitrary length of time,
 * just as long as any two consecutive readings differ by no more than
 * 4.295 seconds (timestampDelta, like all time in the CHRE, is in
 * nanoseconds).
 *
 * If a sensor has batched readings where two consecutive readings differ by
 * more than 4.295 seconds, the CHRE will split them across multiple
 * instances of the struct, and send multiple events.
 *
 * The value from the sensor is typically expressed in a union,
 * allowing a generic access to the data ('value'), along with
 * differently named access giving a more natural interpretation
 * of the data for the specific sensor types which use this
 * structure.  This allows, for example, barometer code to
 * reference readings[N].pressure, and an ambient light sensor
 * to reference readings[N].light, while both use the same
 * structure.
 */
struct chreSensorDataHeader {
    /**
     * The base timestamp, in nanoseconds.
     */
    uint64_t baseTimestamp;

    /**
     * The handle of the sensor producing this event.
     */
    uint32_t sensorHandle;

    /**
     * The number elements in the 'readings' array.
     *
     * This must be at least 1.
     */
    uint16_t readingCount;

    /**
     * Reserved bytes.
     *
     * These must be 0.
     */
    uint8_t reserved[2];
};

/**
 * Data for a sensor which reports on three axes.
 *
 * This is used by CHRE_EVENT_SENSOR_ACCELEROMETER_DATA,
 * CHRE_EVENT_SENSOR_UNCALIBRATED_ACCELEROMETER_DATA,
 * CHRE_EVENT_SENSOR_GYROSCOPE_DATA,
 * CHRE_EVENT_SENSOR_GYROSCOPE_BIAS_INFO,
 * CHRE_EVENT_SENSOR_UNCALIBRATED_GYROSCOPE_DATA,
 * CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_DATA,
 * CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_BIAS_INFO, and
 * CHRE_EVENT_SENSOR_UNCALIBRATED_GEOMAGNETIC_FIELD_DATA.
 */
struct chreSensorThreeAxisData {
    /**
     * @see chreSensorDataHeader
     */
    struct chreSensorDataHeader header;
    struct chreSensorThreeAxisSampleData {
        /**
         * @see chreSensorDataHeader
         */
        uint32_t timestampDelta;
        union {
            float values[3];
            float v[3];
            struct {
                float x;
                float y;
                float z;
            };
            float bias[3];
            struct {
                float x_bias;
                float y_bias;
                float z_bias;
            };
        };
    } readings[1];
};

/**
 * Data from a sensor where we only care about a event occurring.
 *
 * This is a bit unusual in that our readings have no data in addition
 * to the timestamp.  But since we only care about the occurrence, we
 * don't need to know anything else.
 *
 * Used by: CHRE_EVENT_SENSOR_INSTANT_MOTION_DETECT_DATA and
 *     CHRE_EVENT_SENSOR_STATIONARY_DETECT_DATA.
 */
struct chreSensorOccurrenceData {
    struct chreSensorDataHeader header;
    struct chreSensorOccurrenceSampleData {
        uint32_t timestampDelta;
        // This space intentionally left blank.
        // Only the timestamp is meaningful here, there
        // is no additional data.
    } readings[1];
};

/**
 * This is used by CHRE_EVENT_SENSOR_LIGHT_DATA,
 * CHRE_EVENT_SENSOR_PRESSURE_DATA,
 * CHRE_EVENT_SENSOR_ACCELEROMETER_TEMPERATURE_DATA,
 * CHRE_EVENT_SENSOR_GYROSCOPE_TEMPERATURE_DATA, and
 * CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_TEMPERATURE_DATA.
 */
struct chreSensorFloatData {
    struct chreSensorDataHeader header;
    struct chreSensorFloatSampleData {
        uint32_t timestampDelta;
        union {
            float value;
            float light;        //!< Unit: lux
            float pressure;     //!< Unit: hectopascals (hPa)
            float temperature;  //!< Unit: degrees Celsius
        };
    } readings[1];
};

/**
 * CHRE_EVENT_SENSOR_PROXIMITY_DATA.
 */
struct chreSensorByteData {
    struct chreSensorDataHeader header;
    struct chreSensorByteSampleData {
        uint32_t timestampDelta;
        union {
            uint8_t value;
            struct {
                uint8_t isNear : 1;
                uint8_t invalid : 1;
                uint8_t padding0 : 6;
            };
        };
    } readings[1];
};

/**
 * The status of a sensor's sampling configuration.
 */
struct chreSensorSamplingStatus {
    /**
     * The interval, in nanoseconds, at which the sensor is now sampling.
     *
     * If this is CHRE_SENSOR_INTERVAL_DEFAULT, then a sampling interval
     * isn't meaningful for this sensor.
     *
     * Note that if 'enabled' is false, this value is not meaningful.
     */
    uint64_t interval;

    /**
     * The latency, in nanoseconds, at which the senor is now reporting.
     *
     * If this is CHRE_SENSOR_LATENCY_DEFAULT, then a latency
     * isn't meaningful for this sensor.
     *
     * The effective batch interval can be derived from this value by
     * adding the current sampling interval.
     *
     * Note that if 'enabled' is false, this value is not meaningful.
     */
    uint64_t latency;

    /**
     * True if the sensor is actively powered and sampling; false otherwise.
     */
    bool enabled;
};

/**
 * The nanoappHandleEvent argument for CHRE_EVENT_SENSOR_SAMPLING_CHANGE.
 *
 * Note that only at least one of 'interval' or 'latency' must be
 * different than it was prior to this event.  Thus, one of these
 * fields may be (but doesn't need to be) the same as before.
 */
struct chreSensorSamplingStatusEvent {
    /**
     * The handle of the sensor which has experienced a change in sampling.
     */
    uint32_t sensorHandle;

    /**
     * The new sampling status.
     *
     * At least one of the field in this struct will be different from
     * the previous sampling status event.
     */
    struct chreSensorSamplingStatus status;
};


/**
 * Find the default sensor for a given sensor type.
 *
 * @param sensorType One of the CHRE_SENSOR_TYPE_* constants.
 * @param handle  If a sensor is found, then the memory will be filled with
 *     the value for the sensor's handle.  This argument must be non-NULL.
 * @returns true if a sensor was found, false otherwise.
 */
bool chreSensorFindDefault(uint8_t sensorType, uint32_t *handle);

/**
 * Get the chreSensorInfo struct for a given sensor.
 *
 * @param sensorHandle  The sensor handle, as obtained from
 *     chreSensorFindDefault() or passed to nanoappHandleEvent().
 * @param info  If the sensor is valid, then this memory will be filled with
 *     the SensorInfo contents for this sensor.  This argument must be
 *     non-NULL.
 * @returns true if the senor handle is valid and 'info' was filled in;
 *     false otherwise.
 */
bool chreGetSensorInfo(uint32_t sensorHandle, struct chreSensorInfo *info);

/**
 * Get the chreSensorSamplingStatus struct for a given sensor.
 *
 * Note that this may be different from what was requested in
 * chreSensorConfigure(), for multiple reasons.  It's possible that the sensor
 * does not exactly support the interval requested in chreSensorConfigure(), so
 * a faster one was chosen.
 *
 * It's also possible that there is another user of this sensor who has
 * requested a faster interval and/or lower latency.  This latter scenario
 * should be noted, because it means the sensor rate can change due to no
 * interaction from this nanoapp.  Note that the
 * CHRE_EVENT_SENSOR_SAMPLING_CHANGE event will trigger in this case, so it's
 * not necessary to poll for such a change.
 *
 * @param sensorHandle  The sensor handle, as obtained from
 *     chreSensorFindDefault() or passed to nanoappHandleEvent().
 * @param status  If the sensor is valid, then this memory will be filled with
 *     the sampling status contents for this sensor.  This argument must be
 *     non-NULL.
 * @returns true if the senor handle is valid and 'status' was filled in;
 *     false otherwise.
 */
bool chreGetSensorSamplingStatus(uint32_t sensorHandle,
                                 struct chreSensorSamplingStatus *status);

/**
 * Configures a given sensor at a specific interval and latency and mode.
 *
 * If this sensor's chreSensorInfo has isOneShot set to 1,
 * then the mode must be one of the ONE_SHOT modes, or this method will fail.
 *
 * The CHRE wants to power as few sensors as possible, in keeping with its
 * low power design.  As such, it only turns on sensors when there are clients
 * actively interested in that sensor data, and turns off sensors as soon as
 * there are no clients interested in them.  Calling this method generally
 * indicates an interest, and using CHRE_SENSOR_CONFIGURE_MODE_DONE shows
 * when we are no longer interested.
 *
 * Thus, each initial Configure of a sensor (per nanoapp) needs to eventually
 * have a DONE call made, either directly or on its behalf.  Subsequent calls
 * to a Configure method within the same nanoapp, when there has been no DONE
 * in between, still only require a single DONE call.
 *
 * For example, the following is valid usage:
 * <code>
 *   chreSensorConfigure(myHandle, mode, interval0, latency0);
 *   [...]
 *   chreSensorConfigure(myHandle, mode, interval1, latency0);
 *   [...]
 *   chreSensorConfigure(myHandle, mode, interval1, latency1);
 *   [...]
 *   chreSensorConfigureModeOnly(myHandle, CHRE_SENSOR_CONFIGURE_MODE_DONE);
 * </code>
 *
 * The first call to Configure is the one which creates the requirement
 * to eventually call with DONE.  The subsequent calls are just changing the
 * interval/latency.  They have not changed the fact that this nanoapp is
 * still interested in output from the sensor 'myHandle'.  Thus, only one
 * single call for DONE is needed.
 *
 * There is a special case.  One-shot sensors, sensors which
 * just trigger a single event and never trigger again, implicitly go into
 * DONE mode after that single event triggers.  Thus, the
 * following are legitimate usages:
 * <code>
 *   chreSensorConfigure(myHandle, MODE_ONE_SHOT, interval, latency);
 *   [...]
 *   [myHandle triggers an event]
 *   [no need to configure to DONE].
 * </code>
 *
 * And:
 * <code>
 *   chreSensorConfigure(myHandle, MODE_ONE_SHOT, interval, latency);
 *   [...]
 *   chreSensorConfigureModeOnly(myHandle, MODE_DONE);
 *   [we cancelled myHandle before it ever triggered an event]
 * </code>
 *
 * Note that while PASSIVE modes, by definition, don't express
 * an interest in powering the sensor, DONE is still necessary
 * to silence the event reporting.
 *
 * @param sensorHandle  The handle to the sensor, as obtained from
 *     chreSensorFindDefault().
 * @param mode  The mode to use.  See descriptions within the
 *     chreSensorConfigureMode enum.
 * @param interval  The interval, in nanoseconds, at which we want events from
 *     the sensor.  On success, the sensor will be set to 'interval', or a value
 *     less than 'interval'.  There is a special value
 *     CHRE_SENSOR_INTERVAL_DEFAULT, in which we don't express a preference for
 *     the interval, and allow the sensor to chose what it wants.  Note that
 *     due to batching, we may receive events less frequently than
 *     'interval'.
 * @param latency  The maximum latency, in nanoseconds, allowed before the
 *     CHRE begins delivery of an event.  This will control how many events
 *     can be queued by the sensor before requiring a delivery event.
 *     Latency is defined as the "timestamp when event is queued by the CHRE"
 *     minus "timestamp of oldest unsent data reading".
 *     There is a special value CHRE_SENSOR_LATENCY_DEFAULT, in which we don't
 *     express a preference for the latency, and allow the sensor to choose what
 *     it wants.
 *     Note that there is no assurance of how long it will take an event to
 *     get through a CHRE's queueing system, and thus there is no ability to
 *     request a minimum time from the occurrence of a phenomenon to when the
 *     nanoapp receives the information.  The current CHRE API has no
 *     real-time elements, although future versions may introduce some to
 *     help with this issue.
 * @returns true if the configuration succeeded, false otherwise.
 *
 * @see chreSensorConfigureMode
 * @see chreSensorFindDefault
 * @see chreSensorInfo
 */
bool chreSensorConfigure(uint32_t sensorHandle,
                         enum chreSensorConfigureMode mode,
                         uint64_t interval, uint64_t latency);

/**
 * Short cut for chreSensorConfigure where we only want to configure the mode
 * and do not care about interval/latency.
 *
 * @see chreSensorConfigure
 */
static inline bool chreSensorConfigureModeOnly(
        uint32_t sensorHandle, enum chreSensorConfigureMode mode) {
    return chreSensorConfigure(sensorHandle,
                               mode,
                               CHRE_SENSOR_INTERVAL_DEFAULT,
                               CHRE_SENSOR_LATENCY_DEFAULT);
}

/**
 * Convenience function that wraps chreSensorConfigure but enables batching to
 * be controlled by specifying the desired maximum batch interval rather
 * than maximum sample latency.  Users may find the batch interval to be a more
 * intuitive method of expressing the desired batching behavior.
 *
 * Batch interval is different from latency as the batch interval time is
 * counted starting when the prior event containing a batch of sensor samples is
 * delivered, while latency starts counting when the first sample is deferred to
 * start collecting a batch.  In other words, latency ignores the time between
 * the last sample in a batch to the first sample of the next batch, while it's
 * included in the batch interval, as illustrated below.
 *
 *  Time      0   1   2   3   4   5   6   7   8
 *  Batch             A           B           C
 *  Sample   a1  a2  a3  b1  b2  b3  c1  c2  c3
 *  Latency  [        ]  [        ]  [        ]
 *  BatchInt          |           |           |
 *
 * In the diagram, the effective sample interval is 1 time unit, latency is 2
 * time units, and batch interval is 3 time units.
 *
 * @param sensorHandle See chreSensorConfigure#sensorHandle
 * @param mode See chreSensorConfigure#mode
 * @param sampleInterval See chreSensorConfigure#interval, but note that
 *     CHRE_SENSOR_INTERVAL_DEFAULT is not a supported input to this method.
 * @param batchInterval The desired maximum interval, in nanoseconds, between
 *     CHRE enqueuing each batch of sensor samples.
 * @return Same as chreSensorConfigure
 *
 * @see chreSensorConfigure
 *
 * @since v1.1
 */
static inline bool chreSensorConfigureWithBatchInterval(
        uint32_t sensorHandle, enum chreSensorConfigureMode mode,
        uint64_t sampleInterval, uint64_t batchInterval) {
    bool result = false;

    if (sampleInterval != CHRE_SENSOR_INTERVAL_DEFAULT) {
        uint64_t latency;
        if (batchInterval == CHRE_SENSOR_BATCH_INTERVAL_DEFAULT) {
            latency = CHRE_SENSOR_LATENCY_DEFAULT;
        } else if (batchInterval > sampleInterval) {
            latency = batchInterval - sampleInterval;
        } else {
            latency = CHRE_SENSOR_LATENCY_ASAP;
        }
        result = chreSensorConfigure(sensorHandle, mode, sampleInterval,
                                     latency);
    }

    return result;
}

#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_SENSOR_H_ */
