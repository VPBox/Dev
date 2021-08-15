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

#include "chre/platform/platform_sensor.h"

namespace chre {

PlatformSensor::PlatformSensor(PlatformSensor&& other) {
  *this = std::move(other);
}

PlatformSensor::~PlatformSensor() {}

void PlatformSensor::init() {
  // TODO: Implement this. Probably we would open some files provided to mock
  // sensor data. Perhaps from command-line arguemnts.
}

void PlatformSensor::deinit() {
  // TODO: Implement this. Probably we would close the files opened previously
  // by init.
}

bool PlatformSensor::getSensors(DynamicVector<Sensor> *sensors) {
  CHRE_ASSERT(sensors);

  // TODO: Implement this. Perhaps look at all sensor trace files provided and
  // return the list of sensor data available.
  return false;
}

bool PlatformSensor::applyRequest(const SensorRequest& request) {
  // TODO: Implement this. Perhaps consider the request and start to pass in
  // sensor samples from mock sensor data once the sensor has transitioned to
  // being enabled. Maybe consider resampling input data if the provided mock
  // data rate is higher than requested.
  return false;
}

bool PlatformSensor::flushAsync() {
  // TODO: Implement this
  return false;
}

SensorType PlatformSensor::getSensorType() const {
  // TODO: Implement this.
  return SensorType::Unknown;
}

uint64_t PlatformSensor::getMinInterval() const {
  // TODO: Implement this.
  return 0;
}

const char *PlatformSensor::getSensorName() const {
  // TODO: Implement this.
  return "";
}

PlatformSensor& PlatformSensor::operator=(PlatformSensor&& other) {
  // TODO: Implement this.
  return *this;
}

ChreSensorData *PlatformSensor::getLastEvent() const {
  // TODO: Implement this.
  return nullptr;
}

bool PlatformSensor::getSamplingStatus(
    struct chreSensorSamplingStatus *status) const {
  // TODO: Implement this.
  return false;
}

bool PlatformSensor::getThreeAxisBias(
    struct chreSensorThreeAxisData *bias) const {
  // TODO: Implement this.
  return false;
}

void PlatformSensorBase::setLastEvent(const ChreSensorData *event) {
  // TODO: Implement this.
}

}  // namespace chre
