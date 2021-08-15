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

#ifndef _CHRE_SENSOR_TYPES_H_
#define _CHRE_SENSOR_TYPES_H_

/**
 * @file
 * Standalone definition of sensor types, and the data structures of the sample
 * events they emit.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
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
 * Start value for all of the vendor-defined private sensors.
 *
 * @since v1.2
 */
#define CHRE_SENSOR_TYPE_VENDOR_START  UINT8_C(192)

/**
 * Accelerometer.
 *
 * Generates: CHRE_EVENT_SENSOR_ACCELEROMETER_DATA and
 *     optionally CHRE_EVENT_SENSOR_ACCELEROMETER_BIAS_INFO
 *
 * Note that the ACCELEROMETER_DATA is always the fully calibrated data,
 * including factory calibration and runtime calibration if available.
 *
 * @see chreConfigureSensorBiasEvents
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
 *
 * @see chreConfigureSensorBiasEvents
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
 *
 * @see chreConfigureSensorBiasEvents
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
 * Step detection.
 *
 * Generates: CHRE_EVENT_SENSOR_STEP_DETECT_DATA
 *
 * @since v1.3
 */
#define CHRE_SENSOR_TYPE_STEP_DETECT  UINT8_C(23)

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

#if CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD_TEMPERATURE >= CHRE_SENSOR_TYPE_VENDOR_START
#error Too many sensor types
#endif

/**
 * Values that can be stored in the accuracy field of chreSensorDataHeader.
 * If CHRE_SENSOR_ACCURACY_UNKNOWN is returned, then the driver did not provide
 * accuracy information with the data. Values in the range
 * [CHRE_SENSOR_ACCURACY_VENDOR_START, CHRE_SENSOR_ACCURACY_VENDOR_END] are
 * reserved for vendor-specific values for vendor sensor types, and are not used
 * by CHRE for standard sensor types.
 *
 * Otherwise, the values have the same meaning as defined in the Android
 * Sensors definition:
 * https://developer.android.com/reference/android/hardware/SensorManager
 *
 * @since v1.3
 *
 * @defgroup CHRE_SENSOR_ACCURACY
 * @{
 */

#define CHRE_SENSOR_ACCURACY_UNKNOWN       UINT8_C(0)
#define CHRE_SENSOR_ACCURACY_UNRELIABLE    UINT8_C(1)
#define CHRE_SENSOR_ACCURACY_LOW           UINT8_C(2)
#define CHRE_SENSOR_ACCURACY_MEDIUM        UINT8_C(3)
#define CHRE_SENSOR_ACCURACY_HIGH          UINT8_C(4)
#define CHRE_SENSOR_ACCURACY_VENDOR_START  UINT8_C(192)
#define CHRE_SENSOR_ACCURACY_VENDOR_END    UINT8_MAX

/** @} */

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
     * The accuracy of the sensor data.
     *
     * @ref CHRE_SENSOR_ACCURACY
     *
     * @since v1.3
     */
    uint8_t accuracy;

    /**
     * Reserved bytes.
     *
     * This must be 0.
     */
    uint8_t reserved;
};

/**
 * Data for a sensor which reports on three axes.
 *
 * This is used by CHRE_EVENT_SENSOR_ACCELEROMETER_DATA,
 * CHRE_EVENT_SENSOR_ACCELEROMETER_BIAS_INFO,
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
 * Used by: CHRE_EVENT_SENSOR_INSTANT_MOTION_DETECT_DATA,
 *     CHRE_EVENT_SENSOR_STATIONARY_DETECT_DATA, and
 *     CHRE_EVENT_SENSOR_STEP_DETECT_DATA.
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
                //! @deprecated As of v1.2, this field is deprecated and must
                //! always be set to 0
                uint8_t invalid : 1;
                uint8_t padding0 : 6;
            };
        };
    } readings[1];
};


#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_SENSOR_TYPES_H_ */
