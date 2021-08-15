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

#ifndef CHRE_CORE_SENSOR_REQUEST_MANAGER_H_
#define CHRE_CORE_SENSOR_REQUEST_MANAGER_H_

#include "chre/core/request_multiplexer.h"
#include "chre/core/sensor.h"
#include "chre/core/sensor_request.h"
#include "chre/core/timer_pool.h"
#include "chre/platform/atomic.h"
#include "chre/platform/system_time.h"
#include "chre/platform/system_timer.h"
#include "chre/util/fixed_size_vector.h"
#include "chre/util/non_copyable.h"
#include "chre/util/optional.h"

namespace chre {

class SensorRequestManager : public NonCopyable {
 public:
  /**
   * Performs initialization of the SensorRequestManager and populates the
   * sensor list with platform sensors.
   */
  SensorRequestManager();

  /**
   * Destructs the sensor request manager and releases platform sensor resources
   * if requested.
   */
  ~SensorRequestManager();

  /**
   * Initializes the underlying platform-specific sensors. Must be called
   * prior to invoking any other methods in this class.
   */
  void init();

  /**
   * Determines whether the runtime is aware of a given sensor type. The
   * supplied sensorHandle is only populated if the sensor type is known.
   *
   * @param sensorType The type of the sensor.
   * @param sensorHandle A non-null pointer to a uint32_t to use as a sensor
   *                     handle for nanoapps.
   * @return true if the supplied sensor type is available for use.
   */
  bool getSensorHandle(SensorType sensorType, uint32_t *sensorHandle) const;

  /**
   * Sets a sensor request for the given nanoapp for the provided sensor handle.
   * If the nanoapp has made a previous request, it is replaced by this request.
   * If the request changes the mode to SensorMode::Off the request is removed.
   *
   * @param nanoapp A non-null pointer to the nanoapp requesting this change.
   * @param sensorHandle The sensor handle for which this sensor request is
   *        directed at.
   * @param request The new sensor request for this nanoapp.
   * @return true if the request was set successfully. If the sensorHandle is
   *         out of range or the platform sensor fails to update to the new
   *         request false will be returned.
   */
  bool setSensorRequest(Nanoapp *nanoapp, uint32_t sensorHandle,
                        const SensorRequest& sensorRequest);

  /**
   * Populates the supplied info struct if the sensor handle exists.
   *
   * @param sensorHandle The handle of the sensor.
   * @param nanoapp The nanoapp requesting this change.
   * @param info A non-null pointer to a chreSensorInfo struct.
   * @return true if the supplied sensor handle exists.
   */
  bool getSensorInfo(uint32_t sensorHandle, const Nanoapp& nanoapp,
                     struct chreSensorInfo *info) const;
  /*
   * Removes all requests of a sensorType and unregisters all nanoapps for its
   * events.
   *
   * @param sensorType The sensor type whose requests are to be removed.
   * @return true if all requests of the sensor type have been successfully
   *         removed.
   */
  bool removeAllRequests(SensorType sensorType);

  /**
   * Obtains a pointer to the Sensor of the specified sensorType.
   *
   * NOTE: Some platform implementations invoke this method from different
   * threads assuming the underlying list of sensors doesn't change after
   * initialization.
   *
   * @param sensorType The SensorType of the sensor.
   * @return A pointer to the Sensor of sensorType, or nullptr if sensorType is
   *         invalid or the requested SensorType is not supported on the current
   *         platform.
   */
  Sensor *getSensor(SensorType sensorType);

  /**
   * Populates the supplied sampling status struct if the sensor handle exists.
   *
   * @param sensorHandle The handle of the sensor.
   * @param status A non-null pointer to a chreSensorSamplingStatus struct.
   * @return true if the supplied sensor handle exists.
   */
  bool getSensorSamplingStatus(uint32_t sensorHandle,
                               struct chreSensorSamplingStatus *status) const;

  /**
   * Obtains the list of open requests of the specified SensorType.
   *
   * @param sensorType The SensorType of the sensor.
   * @return The list of open requests of this sensor in a DynamicVector.
   */
  const DynamicVector<SensorRequest>& getRequests(SensorType sensorType) const;

  /**
   * Configures a nanoapp to receive bias events.
   *
   * @param nanoapp A non-null pointer to the nanoapp making this request.
   * @param sensorHandle The handle of the sensor to receive bias events for.
   * @param enable true to enable bias event reporting.
   *
   * @return true if the configuration was successful.
   */
  bool configureBiasEvents(
      Nanoapp *nanoapp, uint32_t sensorHandle, bool enable);

  /**
   * Synchronously retrieves the current bias for a sensor that supports
   * data in the chreSensorThreeAxisData format.
   *
   * @param sensorHandle The handle of the sensor to retrieve bias data for.
   * @param bias A non-null pointer to store the current bias data.
   *
   * @return false if the sensor handle was invalid or the sensor does not
   *     report bias data in the chreSensorThreeAxisData format.
   */
  bool getThreeAxisBias(
      uint32_t sensorHandle, struct chreSensorThreeAxisData *bias) const;

  /**
   * Makes a sensor flush request for a nanoapp asynchronously.
   *
   * @param nanoapp A non-null pointer to the nanoapp requesting this change.
   * @param sensorHandle The sensor handle for which this sensor request is
   *        directed at.
   * @param cookie An opaque pointer to data that will be used in the
   *        chreSensorFlushCompleteEvent.
   *
   * @return true if the request was accepted, false otherwise
   */
  bool flushAsync(Nanoapp *nanoapp, uint32_t sensorHandle, const void *cookie);

  /**
   * Invoked by the PlatformSensor when a flush complete event is received for a
   * given sensor for a request done through flushAsync(). This method can be
   * invoked from any thread, and defers processing the event to the main CHRE
   * event loop.
   *
   * @param errorCode An error code from enum chreError
   * @param sensorType The SensorType of sensor that has completed the flush.
   */
  void handleFlushCompleteEvent(uint8_t errorCode, SensorType sensorType);

  /**
   * Invoked by the PlatformSensor when a sensor event is received for a given
   * sensor. This method should be invoked from the same thread.
   *
   * @param sensorType the type of sensor the sensor data corresponds to
   * @param event the event data formatted as one of the chreSensorXXXData
   *     defined in the CHRE API, implicitly specified by sensorType.
   */
  void handleSensorEvent(SensorType sensorType, void *event);

  /**
   * Prints state in a string buffer. Must only be called from the context of
   * the main CHRE thread.
   *
   * @param buffer Pointer to the start of the buffer.
   * @param bufferPos Pointer to buffer position to start the print (in-out).
   * @param size Size of the buffer in bytes.
   */
  void logStateToBuffer(char *buffer, size_t *bufferPos,
                        size_t bufferSize) const;

 private:
  //! An internal structure to store incoming sensor flush requests
  struct FlushRequest {
    FlushRequest(SensorType type, uint32_t id, const void *cookiePtr) {
      sensorType = type;
      nanoappInstanceId = id;
      cookie = cookiePtr;
    }

    //! The sensor type the flush request is for.
    SensorType sensorType;
    //! The ID of the nanoapp that requested the flush.
    uint32_t nanoappInstanceId;
    //! The opaque pointer provided in flushAsync().
    const void *cookie;
    //! The timestamp at which this request should complete.
    Nanoseconds deadlineTimestamp = SystemTime::getMonotonicTime() +
        Nanoseconds(CHRE_SENSOR_FLUSH_COMPLETE_TIMEOUT_NS);
    //! True if this flush request is active and is pending completion.
    bool isActive = false;
  };

  /**
   * This allows tracking the state of a sensor with the various requests for it
   * and can trigger a change in mode/rate/latency when required.
   */
  class SensorRequests {
   public:
    SensorRequests() : mFlushRequestPending(false) {}

     /**
      * Initializes the sensor object. This method must only be invoked once
      * when the SensorRequestManager initializes.
      *
      * @param sensor The sensor object to initialize with.
      */
    void setSensor(Sensor&& sensor) {
      CHRE_ASSERT(!mSensor.has_value());
      mSensor = std::move(sensor);
    }

    /**
     * @return true if the sensor is supported by the platform.
     */
    bool isSensorSupported() const {
      return mSensor.has_value();
    }

    /**
     * @return The set of active requests for this sensor.
     */
    const DynamicVector<SensorRequest>& getRequests() const {
      return mMultiplexer.getRequests();
    }

    /**
     * @return true if the sensor is currently enabled.
     */
    bool isSensorEnabled() const {
      return !mMultiplexer.getRequests().empty();
    }

    /**
     * @return A constant reference to the sensor object. This method has an
     * undefined behavior if isSensorSupported() is false.
     */
    const Sensor& getSensor() const {
      return mSensor.value();
    }

    /**
     * @return A reference to the sensor object. This method has an undefined
     * behavior if isSensorSupported() is false.
     */
    Sensor& getSensor() {
      return mSensor.value();
    }

    /**
     * Gets the sensor's sampling status. The caller must ensure that
     * isSensorSupported() is true before invoking this method.
     *
     * @param status A non-null pointer where the sampling status will be
     * stored, if successful.
     *
     * @return true if getting the sampling status succeeded.
     */
    bool getSamplingStatus(struct chreSensorSamplingStatus *status) const {
      CHRE_ASSERT(isSensorSupported());
      return isSensorSupported() ? mSensor->getSamplingStatus(status) : false;
    }

    /**
     * Synchronously retrieves the current bias for a sensor that supports
     * data in the chreSensorThreeAxisData format. The caller must ensure that
     * isSensorSupported() is true before invoking this method.
     *
     * @param bias A non-null pointer to store the current bias data.
     *
     * @return false if sensor does not report bias data in the
     *     chreSensorThreeAxisData format.
     */
    bool getThreeAxisBias(struct chreSensorThreeAxisData *bias) const {
      CHRE_ASSERT(isSensorSupported());
      return isSensorSupported() ? mSensor->getThreeAxisBias(bias) : false;
    }

    /**
     * Searches through the list of sensor requests for a request owned by the
     * given nanoapp. The provided non-null index pointer is populated with the
     * index of the request if it is found.
     *
     * @param instanceId The instance ID of the nanoapp whose request is being
     *        searched for.
     * @param index A non-null pointer to an index that is populated if a
     *        request for this nanoapp is found.
     * @return A pointer to a SensorRequest that is owned by the provided
     *         nanoapp if one is found otherwise nullptr.
     */
    const SensorRequest *find(uint32_t instanceId, size_t *index) const;

    /**
     * Adds a new sensor request to the request multiplexer for this sensor.
     *
     * @param request The request to add to the multiplexer.
     * @param requestChanged A non-null pointer to a bool to indicate that the
     *        net request made to the sensor has changed. This boolean is always
     *        assigned to the status of the request changing (true or false).
     * @return true if the add operation was successful.
     */
    bool add(const SensorRequest& request, bool *requestChanged);

    /**
     * Removes a sensor request from the request multiplexer for this sensor.
     * The provided index must fall in the range of the sensor requests managed
     * by the multiplexer.
     *
     * @param removeIndex The index to remove the request from.
     * @param requestChanged A non-null pointer to a bool to indicate that the
     *        net request made to the sensor has changed. This boolean is always
     *        assigned to the status of the request changing (true or false).
     * @return true if the remove operation was successful.
     */
    bool remove(size_t removeIndex, bool *requestChanged);

    /**
     * Updates a sensor request in the request multiplexer for this sensor. The
     * provided index must fall in range of the sensor requests managed by the
     * multiplexer.
     *
     * @param updateIndex The index to update the request at.
     * @param request The new sensor request to replace the existing request
     *        with.
     * @return true if the update operation was successful.
     */
    bool update(size_t updateIndex, const SensorRequest& request,
                bool *requestChanged);

    /**
     * Removes all requests and consolidates all the maximal request changes
     * into one sensor configuration update.
     *
     * @return true if all the requests have been removed and sensor
     *         configuration successfully updated.
     */
    bool removeAll();

    /**
     * Makes a specified flush request for this sensor, and sets the timeout
     * timer appropriately. If there already is a pending flush request, then
     * this method does nothing.
     *
     * @param request the request to make
     *
     * @return An error code from enum chreError
     */
    uint8_t makeFlushRequest(FlushRequest& request);

    /**
     * Clears any states (e.g. timeout timer and relevant flags) associated
     * with a pending flush request.
     */
    void clearPendingFlushRequest();

    /**
     * Cancels the pending timeout timer associated with a flush request.
     */
    void cancelPendingFlushRequestTimer();

    /**
     * @return true if a flush through makeFlushRequest is pending.
     */
    inline bool isFlushRequestPending() const {
      return mFlushRequestPending;
    }

   private:
    //! The sensor associated with this request multiplexer. If this Optional
    //! container does not have a value, then the platform does not support this
    //! type of sensor.
    Optional<Sensor> mSensor;

    //! The request multiplexer for this sensor.
    RequestMultiplexer<SensorRequest> mMultiplexer;

    //! The timeout timer handle for the current flush request.
    TimerHandle mFlushRequestTimerHandle = CHRE_TIMER_INVALID;

    //! True if a flush request is pending for this sensor.
    AtomicBool mFlushRequestPending;

    /**
     * Make a flush request through PlatformSensor.
     *
     * @return true if the flush request was successfully made.
     */
    bool doMakeFlushRequest();
  };

  //! The list of sensor requests.
  FixedSizeVector<SensorRequests, getSensorTypeCount()> mSensorRequests;

  //! A queue of flush requests made by nanoapps.
  static constexpr size_t kMaxFlushRequests = 16;
  FixedSizeVector<FlushRequest, kMaxFlushRequests> mFlushRequestQueue;

  /**
   * Helper function to convert SensorType to SensorRequests.
   */
  SensorRequests& getSensorRequests(SensorType sensorType) {
    size_t index = getSensorTypeArrayIndex(sensorType);
    return mSensorRequests[index];
  }

  /**
   * Posts an event to a nanoapp indicating the completion of a flush request.
   *
   * @param sensorHandle The handle of the sensor for this event.
   * @param errorCode An error code from enum chreError
   * @param request The corresponding FlushRequest.
   */
  void postFlushCompleteEvent(
    uint32_t sensorHandle, uint8_t errorCode, const FlushRequest& request);

  /**
   * Completes a flush request at the specified index by posting a
   * CHRE_EVENT_SENSOR_FLUSH_COMPLETE event with the specified errorCode,
   * removing the request from the queue, cleaning up states as necessary.
   *
   * @param index The index of the flush request.
   * @param errorCode The error code to send the completion event with.
   */
  void completeFlushRequestAtIndex(size_t index, uint8_t errorCode);

  /**
   * Dispatches the next flush request for the given sensor. If there are no
   * more pending flush requests, this method does nothing.
   *
   * @param sensorType The corresponding sensor type.
   */
  void dispatchNextFlushRequest(SensorType sensorType);

  /**
   * Handles a complete event for a sensor flush requested through flushAsync.
   * See handleFlushCompleteEvent which may be called from any thread. This
   * method is intended to be invoked on the CHRE event loop thread.
   *
   * @param errorCode An error code from enum chreError
   * @param sensorType The SensorType of sensor that has completed the flush.
   */
  void handleFlushCompleteEventSync(uint8_t errorCode, SensorType sensorType);

  /**
   * Cancels all pending flush requests for a given sensor and nanoapp.
   *
   * @param sensorType The type of sensor to cancel requests for.
   * @param nanoappInstanceId The ID of the nanoapp to cancel requests for,
   *     kSystemInstanceId to remove requests for all nanoapps.
   */
  void cancelFlushRequests(
      SensorType sensorType, uint32_t nanoappInstanceId = kSystemInstanceId);
};

}  // namespace chre

#endif  // CHRE_CORE_SENSOR_REQUEST_MANAGER_H_
