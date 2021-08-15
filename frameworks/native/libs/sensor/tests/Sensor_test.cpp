/*
 * Copyright (C) 2012 The Android Open Source Project
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

#define LOG_TAG "Sensor_test"

#include <sensor/Sensor.h>
#include <hardware/sensors.h>
#include <utils/Errors.h>

#include <gtest/gtest.h>

namespace android {

// Returns true if the two sensors have the same attributes. Does not compare
// UUID since that should not be transmitted via flatten/unflatten.
static bool sensorsMatch(const Sensor& a, const Sensor& b) {
    return a.getName() == b.getName () &&
        a.getVendor() == b.getVendor () &&
        a.getHandle() == b.getHandle () &&
        a.getType() == b.getType () &&
        a.getMinValue() == b.getMinValue () &&
        a.getMaxValue() == b.getMaxValue () &&
        a.getResolution() == b.getResolution () &&
        a.getPowerUsage() == b.getPowerUsage () &&
        a.getMinDelay() == b.getMinDelay () &&
        a.getMinDelayNs() == b.getMinDelayNs () &&
        a.getVersion() == b.getVersion () &&
        a.getFifoReservedEventCount() == b.getFifoReservedEventCount () &&
        a.getFifoMaxEventCount() == b.getFifoMaxEventCount () &&
        a.getStringType() == b.getStringType () &&
        a.getRequiredPermission() == b.getRequiredPermission () &&
        a.isRequiredPermissionRuntime() == b.isRequiredPermissionRuntime () &&
        a.getRequiredAppOp() == b.getRequiredAppOp () &&
        a.getMaxDelay() == b.getMaxDelay () &&
        a.getFlags() == b.getFlags () &&
        a.isWakeUpSensor() == b.isWakeUpSensor () &&
        a.isDynamicSensor() == b.isDynamicSensor () &&
        a.hasAdditionalInfo() == b.hasAdditionalInfo () &&
        a.getReportingMode() == b.getReportingMode();
}

// Creates and returns a sensor_t struct with some default values filled in.
static sensor_t getTestSensorT() {
    sensor_t hwSensor = {};
    hwSensor.name = "Test Sensor";
    hwSensor.vendor = "Test Vendor";
    hwSensor.version = 1;
    hwSensor.handle = 2;
    hwSensor.type = SENSOR_TYPE_ACCELEROMETER;
    hwSensor.maxRange = 10.f;
    hwSensor.resolution = 1.f;
    hwSensor.power = 5.f;
    hwSensor.minDelay = 1000;
    hwSensor.fifoReservedEventCount = 50;
    hwSensor.fifoMaxEventCount = 100;
    hwSensor.stringType = SENSOR_STRING_TYPE_ACCELEROMETER;
    hwSensor.requiredPermission = "";
    hwSensor.maxDelay = 5000;
    hwSensor.flags = SENSOR_FLAG_CONTINUOUS_MODE;
    return hwSensor;
}

TEST(SensorTest, FlattenAndUnflatten) {
    sensor_t hwSensor = getTestSensorT();
    Sensor sensor1(&hwSensor, SENSORS_DEVICE_API_VERSION_1_4);
    Sensor sensor2;

    std::vector<uint8_t> buffer(sensor1.getFlattenedSize());
    ASSERT_EQ(OK, sensor1.flatten(buffer.data(), buffer.size()));
    ASSERT_EQ(OK, sensor2.unflatten(buffer.data(), buffer.size()));

    EXPECT_TRUE(sensorsMatch(sensor1, sensor2));
}

} // namespace android
