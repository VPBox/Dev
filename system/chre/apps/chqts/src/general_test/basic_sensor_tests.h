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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_BASIC_SENSOR_TESTS_H_
#define _GTS_NANOAPPS_GENERAL_TEST_BASIC_SENSOR_TESTS_H_

#include <general_test/basic_sensor_test_base.h>

#include <chre.h>

namespace general_test {

class BasicAccelerometerTest : public BasicSensorTestBase {
 public:
  BasicAccelerometerTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_ACCELEROMETER;
  }
  bool isRequiredSensor() const override { return true; }
  bool isOnChangeSensor() const override { return false; }
  bool isOneShotSensor() const override { return false; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicInstantMotionDetectTest : public BasicSensorTestBase {
 public:
  BasicInstantMotionDetectTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_INSTANT_MOTION_DETECT;
  }
  bool isRequiredSensor() const override { return true; }
  bool isOnChangeSensor() const override { return false; }
  bool isOneShotSensor() const override { return true; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicStationaryDetectTest : public BasicSensorTestBase {
 public:
  BasicStationaryDetectTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_STATIONARY_DETECT;
  }
  bool isRequiredSensor() const override { return true; }
  bool isOnChangeSensor() const override { return false; }
  bool isOneShotSensor() const override { return true; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicGyroscopeTest : public BasicSensorTestBase {
 public:
  BasicGyroscopeTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_GYROSCOPE;
  }
  bool isRequiredSensor() const override { return true; }
  bool isOnChangeSensor() const override { return false; }
  bool isOneShotSensor() const override { return false; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicMagnetometerTest : public BasicSensorTestBase {
 public:
  BasicMagnetometerTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD;
  }
  bool isRequiredSensor() const override { return false; }
  bool isOnChangeSensor() const override { return false; }
  bool isOneShotSensor() const override { return false; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicBarometerTest : public BasicSensorTestBase {
 public:
  BasicBarometerTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_PRESSURE;
  }
  bool isRequiredSensor() const override { return false; }
  bool isOnChangeSensor() const override { return false; }
  bool isOneShotSensor() const override { return false; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicLightSensorTest : public BasicSensorTestBase {
 public:
  BasicLightSensorTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_LIGHT;
  }
  bool isRequiredSensor() const override { return false; }
  bool isOnChangeSensor() const override { return true; }
  bool isOneShotSensor() const override { return false; }
  void confirmDataIsSane(const void* eventData) override;
};

class BasicProximityTest : public BasicSensorTestBase {
 public:
  BasicProximityTest()
      : BasicSensorTestBase() {}

 protected:
  uint8_t getSensorType() const override {
    return CHRE_SENSOR_TYPE_PROXIMITY;
  }
  bool isRequiredSensor() const override { return false; }
  bool isOnChangeSensor() const override { return true; }
  bool isOneShotSensor() const override { return false; }
  void confirmDataIsSane(const void* eventData) override;
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_BASIC_SENSOR_TESTS_H_
