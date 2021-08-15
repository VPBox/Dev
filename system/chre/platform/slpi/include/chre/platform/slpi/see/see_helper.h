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

#ifndef CHRE_PLATFORM_SLPI_SEE_SEE_HELPER_H_
#define CHRE_PLATFORM_SLPI_SEE_SEE_HELPER_H_

extern "C" {

#include "sns_client.h"

}  // extern "C"

#include "sns_suid.pb.h"

#include "chre/core/sensor_type.h"
#include "chre/platform/condition_variable.h"
#include "chre/platform/mutex.h"
#include "chre/platform/slpi/see/see_cal_helper.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/non_copyable.h"
#include "chre/util/optional.h"
#include "chre/util/time.h"
#include "chre/util/unique_ptr.h"

namespace chre {

inline bool suidsMatch(const sns_std_suid& suid0, const sns_std_suid& suid1) {
  return (suid0.suid_high == suid1.suid_high
          && suid0.suid_low == suid1.suid_low);
}

//! A callback interface for receiving SeeHelper data events.
class SeeHelperCallbackInterface {
 public:
  struct SamplingStatusData {
    SensorType sensorType;
    struct chreSensorSamplingStatus status;
    bool enabledValid;
    bool intervalValid;
    bool latencyValid;
  };

  virtual ~SeeHelperCallbackInterface() {}

  //! Invoked by the SEE thread to update sampling status.
  virtual void onSamplingStatusUpdate(
      UniquePtr<SamplingStatusData>&& status) = 0;

  //! Invoked by the SEE thread to provide sensor data events. The event data
  //! format is one of the chreSensorXXXData defined in the CHRE API, implicitly
  //! specified by sensorType.
  virtual void onSensorDataEvent(
      SensorType sensorType, UniquePtr<uint8_t>&& eventData) = 0;

  //! Invoked by the SEE thread to update the AP wake/suspend status.
  virtual void onHostWakeSuspendEvent(bool apAwake) = 0;

  //! Invoked by the SEE thread to provide the sensor bias event.
  //! The bias is generated with the sensorHandle field set to that of
  //! runtime-calibrated sensors, regardless of whether the runtime-calibrated
  //! or uncalibrated versions of the sensor is enabled.
  virtual void onSensorBiasEvent(
      UniquePtr<struct chreSensorThreeAxisData>&& biasData) = 0;

  //! Invoked by the SEE thread to notify a flush complete
  virtual void onFlushCompleteEvent(SensorType sensorType) = 0;
};

//! Default timeout for waitForService. Have a longer timeout since there may be
//! external dependencies blocking SEE initialization.
constexpr Nanoseconds kDefaultSeeWaitTimeout = Seconds(30);

//! Default timeout for sendReq response
constexpr Nanoseconds kDefaultSeeRespTimeout = Seconds(1);

//! Default timeout for sendReq indication
constexpr Nanoseconds kDefaultSeeIndTimeout = Seconds(2);

//! Allowed number of consecutive missing responses.
constexpr uint32_t kSeeNumMissingResp = 5;

//! Length of the char array to store sensor string attributes.
constexpr size_t kSeeAttrStrValLen = 64;

//! A struct to facilitate getAttributesSync().
struct SeeAttributes {
  char vendor[kSeeAttrStrValLen];
  char name[kSeeAttrStrValLen];
  char type[kSeeAttrStrValLen];
  int64_t hwId;
  float maxSampleRate;
  uint8_t streamType;
  bool passiveRequest;
};

//! A struct to facilitate making sensor request
struct SeeSensorRequest {
  SensorType sensorType;
  bool enable;
  bool passive;
  float samplingRateHz;
  uint32_t batchPeriodUs;
};

/**
 * A helper class for making requests to Qualcomm's Sensors Execution
 * Environment (SEE) via the sns_client API and waiting for the response and the
 * corresponding indication message if applicable.
 * Not safe to use from multiple threads. Only one synchronous request can be
 * made at a time.
 */
class SeeHelper : public NonCopyable {
 public:
  //! A struct to facilitate mapping between 'SUID + sns_client' and
  //! SensorType.
  struct SensorInfo {
    sns_std_suid suid;
    SensorType sensorType;
    sns_client *client;
    //! The SUID of the underlying physical sensor, different from suid if
    //! resampler is used.
    sns_std_suid physicalSuid;
  };

  /**
   * Constructor for a SeeHelper that manages its own SeeCalHelper
   */
  SeeHelper();

  /**
   * Constructor for a SeeHelper that uses the supplied SeeCalHelper object
   * rather than creating its own. Caller must ensure that the lifetime of the
   * SeeCalHelper object exceeds the lifetime of this SeeHelper instance.
   *
   * TODO: this would be a good case for a shared ptr implementation
   *
   * @param calHelper Non-null pointer to a calibration helper object to use
   */
  SeeHelper(SeeCalHelper *calHelper);

  /**
   * Deinits clients before destructing this object.
   */
  ~SeeHelper();

  /**
   * Makes a request to SEE to enable an on-change sensor, with no additional
   * payload. Can be used for registering a calibration sensor, for example.
   *
   * @param suid Sensor UID, usually determined via findSuidSync()
   *
   * @return true on success
   */
  bool configureOnChangeSensor(const sns_std_suid& suid, bool enable);

  /**
   * A synchronous call to discover SUID(s) that supports the specified data
   * type. This API will clear the provided dynamic vector before populating it.
   *
   * @param dataType A data type string, "accel" for example.
   * @param suids A non-null pointer to a list of sensor UIDs that support the
   *              specified data type.
   * @param minNumSuids The minimum number of SUIDs it needs to find before
   *                    returning true. Otherwise, it'll re-try internally
   *                    until it times out. It's illegal to set it to 0.
   * @param maxRetries Maximum amount of times to retry looking up the SUID
   *                   until giving up.
   * @param retryDelay Time delay between retry attempts (msec).
   *
   * @return true if at least minNumSuids were successfully found
   */
  bool findSuidSync(const char *dataType, DynamicVector<sns_std_suid> *suids,
                    uint8_t minNumSuids, uint32_t maxRetries,
                    Milliseconds retryDelay);

  /**
   * Version of findSuidSync providing default timeout/retry behavior.
   *
   * @see findSuidSync
   */
  bool findSuidSync(const char *dataType, DynamicVector<sns_std_suid> *suids,
                    uint8_t minNumSuids = 1) {
    uint32_t maxRetries = (mHaveTimedOutOnSuidLookup) ? 0 : 40;
    return findSuidSync(dataType, suids, minNumSuids, maxRetries,
                        Milliseconds(250) /* retryDelay */);
  }

  /**
   * A synchronous call to obtain the attributes of the specified SUID.
   *
   * @param suid The SUID of the sensor
   * @param attr A non-null pointer to the attibutes of the specified SUID that
   *             include sensor vendor, name and max sampling rate, etc.
   *
   * @return true if the attribute was successfully obtained and attr populated.
   */
  bool getAttributesSync(const sns_std_suid& suid, SeeAttributes *attr);

  /**
   * @return the SeeCalHelper instance used by this SeeHelper
   */
  SeeCalHelper *getCalHelper() {
    return mCalHelper;
  }

  /**
   * Initializes and waits for the sensor client service to become available,
   * and obtains remote_proc and cal sensors' info for future operations. This
   * function must be called first to initialize the object and be called only
   * once.
   *
   * @param cbIf A pointer to the callback interface that will be invoked to
   *             handle all async requests with callback data type defined in
   *             the interface.
   * @param timeout The wait timeout in microseconds.
   * @param skipDefaultSensorInit If true, don't register remote proc status and
   *                              calibration sensors (e.g. if another SeeHelper
   *                              instance will manage these)
   *
   * @return true if all initialization steps succeeded.
   */
  bool init(SeeHelperCallbackInterface *cbIf,
            Microseconds timeout = kDefaultSeeWaitTimeout,
            bool skipDefaultSensorInit = false);

  /**
   * Makes a sensor configuration request to SEE.
   *
   * @param request The sensor request to make.
   *
   * @return true if the request has been successfully made.
   */
  bool makeRequest(const SeeSensorRequest& request);

  /**
   * Makes a sensor flush request to SEE.
   *
   * @param sensorType The type of sensor to request the flush.
   *
   * @return true if the request has been successfully made.
   */
  bool flush(SensorType sensorType);

  /**
   * Register a SensorType with the SUID of the SEE sensor/driver.
   *
   * Only registered SUIDs will call the indication callback provided in init()
   * with populated CHRE sensor events. Each SUID/SensorType pair can only be
   * registered once. It's illegal to register SensorType::Unknown.
   *
   * If an SUID is registered with a second SensorType, another client may be
   * created to disambiguate the SUID representation.
   *
   * @param sensorType The SensorType to register.
   * @param suid The SUID of the sensor.
   * @param resample Whether to resample this sensorType.
   * @param prevRegistered A non-null pointer to a boolean that indicates
   *        whether the SUID/SensorType pair has been previously registered.
   *
   * @return true if the SUID/SensorType pair was successfully registered.
   */
  bool registerSensor(SensorType sensorType, const sns_std_suid& suid,
                      bool resample, bool *prevRegistered);

  /**
   * Checks whether the given SensorType has been successfully registered
   * already via registerSensor().
   *
   * @param sensorType The SensorType to check.
   *
   * @return true if the given sensor type has been registered, false otherwise
   */
  bool sensorIsRegistered(SensorType sensorType) const;

 protected:
  struct SnsClientApi {
    decltype(sns_client_init)   *sns_client_init;
    decltype(sns_client_deinit) *sns_client_deinit;
    decltype(sns_client_send)   *sns_client_send;
  };

  //! Contains the API this SeeHelper instance uses to interact with SEE
  const SnsClientApi *mSnsClientApi = &kDefaultApi;

  /**
   * Get the cached SUID of a calibration sensor that corresponds to the
   * specified sensorType.
   *
   * @param sensorType The sensor type of the calibration sensor.
   *
   * @return A constant reference to the calibration sensor's SUID if present.
   *         Otherwise, a reference to sns_suid_sensor_init_zero is returned.
   */
  const sns_std_suid& getCalSuidFromSensorType(SensorType sensorType) const {
    return mCalHelper->getCalSuidFromSensorType(sensorType);
  }

  /**
   * A convenience method to send a request and wait for the indication if it's
   * a synchronous one using the default client obtained in init().
   *
   * @see sendReq
   */
  bool sendReq(
      const sns_std_suid& suid,
      void *syncData, const char *syncDataType,
      uint32_t msgId, void *payload, size_t payloadLen,
      bool batchValid, uint32_t batchPeriodUs, bool passive,
      bool waitForIndication,
      Nanoseconds timeoutResp = kDefaultSeeRespTimeout,
      Nanoseconds timeoutInd = kDefaultSeeIndTimeout) {
    return sendReq(mSeeClients[0], suid,
                   syncData, syncDataType,
                   msgId, payload, payloadLen,
                   batchValid, batchPeriodUs, passive,
                   waitForIndication,
                   timeoutResp, timeoutInd);
  }

 private:
  static const SnsClientApi kDefaultApi;

  //! Used to synchronize responses and indications.
  ConditionVariable mCond;

  //! Used with mCond, and to protect access to member variables from other
  //! threads.
  Mutex mMutex;

  //! Callback interface for sensor events.
  SeeHelperCallbackInterface *mCbIf = nullptr;

  //! The list of SEE clients initiated by SeeHelper.
  DynamicVector<sns_client *> mSeeClients;

  //! The list of SensorTypes registered and their corresponding SUID and
  //! client.
  DynamicVector<SensorInfo> mSensorInfos;

  //! Data struct to store sync APIs data.
  void *mSyncData = nullptr;

  //! The data type whose indication this SeeHelper is waiting for in
  //! findSuidSync.
  const char *mSyncDataType = nullptr;

  //! The SUID whose indication this SeeHelper is waiting for in a sync call.
  sns_std_suid mSyncSuid = sns_suid_sensor_init_zero;

  //! true if we are waiting on an indication for a sync call.
  bool mWaitingOnInd = false;

  //! true if we are waiting on a response of a request.
  bool mWaitingOnResp = false;

  //! true if we've timed out in findSuidSync at least once
  bool mHaveTimedOutOnSuidLookup = false;

  //! The response error of the request we just made.
  sns_std_error mRespError;

  //! A transaction ID that increments for each request.
  uint32_t mCurrentTxnId = 0;

  //! The number of consecutive missing responses.
  uint32_t mNumMissingResp = 0;

  //! The SUID for the remote_proc sensor.
  Optional<sns_std_suid> mRemoteProcSuid;

  //! The SUID for the resampler sensor.
  Optional<sns_std_suid> mResamplerSuid;

  //! Handles sensor calibration data
  SeeCalHelper *mCalHelper;

  //! true if we own the memory to mCalHelper and should free it when done
  bool mOwnsCalHelper;

  /**
   * Initializes the SEE remote processor sensor and makes a data request.
   *
   * @return true if the remote proc sensor was successfully initialized.
   */
  bool initRemoteProcSensor();

  /**
   * Initializes the SEE resampler sensor.
   *
   * @return true if the resampler sensor was successfully initialized.
   */
  bool initResamplerSensor();

  /**
   * Sends a request to SEE and waits for the response.
   *
   * @param client The pointer to sns_client to make the request with.
   * @param req A pointer to the sns_client_request_msg to be sent.
   * @param timeoutResp How long to wait for the response before abandoning it.
   *
   * @return true if the request was sent and the response was received
   *         successfully.
   */
  bool sendSeeReqSync(sns_client *client, sns_client_request_msg *req,
                      Nanoseconds timeoutResp);

  /**
   * Wrapper to send a SEE request and wait for the indication if it's a
   * synchronous one.
   *
   * Only one request can be pending at a time per instance of SeeHelper.
   *
   * @param client The pointer to sns_client to make requests with.
   * @param suid The SUID of the sensor the request is sent to
   * @param syncData The data struct or container to receive a sync call's data
   * @param syncDataType The data type we are waiting for.
   * @param msgId Message ID of the request to send
   * @param payload A non-null pointer to the pb-encoded message
   * @param payloadLen The length of payload
   * @param batchValid Whether batchPeriodUs is valid and applicable to this
   *                   request
   * @param batchPeriodUs The batch period in microseconds
   * @param passive Whether this is a passive request
   * @param waitForIndication Whether to wait for the indication of the
   *                          specified SUID or not.
   * @param timeoutResp How long to wait for the response before abandoning it
   * @param timeoutInd How long to wait for the indication before abandoning it
   *
   * @return true if the request has been sent and the response/indication it's
   *         waiting for has been successfully received
   */
  bool sendReq(
      sns_client *client, const sns_std_suid& suid,
      void *syncData, const char *syncDataType,
      uint32_t msgId, void *payload, size_t payloadLen,
      bool batchValid, uint32_t batchPeriodUs, bool passive,
      bool waitForIndication,
      Nanoseconds timeoutResp = kDefaultSeeRespTimeout,
      Nanoseconds timeoutInd = kDefaultSeeIndTimeout);

  /**
   * A helper function that prepares SeeHelper to wait for an indication.
   *
   * @see sendReq
   */
  void prepareWaitForInd(const sns_std_suid& suid, void *syncData,
                         const char *syncDataType);

  /**
   * A helper function that waits for the indication.
   *
   * @return true if the inication is received  before timeout.
   *
   * @see sendReq
   */
  bool waitForInd(bool reqSent, Nanoseconds timeoutInd);

  /**
   * Handles the payload of a sns_client_event_msg.
   */
  void handleSnsClientEventMsg(
      sns_client *client, const void *payload, size_t payloadLen);

  /**
   * Handles a response from SEE for a request sent with the specified
   * transaction ID.
   */
  void handleSeeResp(uint32_t txnId, sns_std_error error);

  /**
   * Extracts "this" from cbData and calls through to handleSnsClientEventMsg()
   *
   * @see sns_client_ind
   */
  static void seeIndCb(sns_client *client, void *msg, uint32_t msgLen,
                       void *cbData);

  /**
   * Extracts "this" from cbData and calls through to handleSeeResp()
   *
   * @see sns_client_resp
   */
  static void seeRespCb(sns_client *client, sns_std_error error, void *cbData);

  /**
   * A wrapper to initialize a sns_client.
   *
   * @see sns_client_init
   */
  bool waitForService(sns_client **client,
                      Microseconds timeout = kDefaultSeeWaitTimeout);

  /**
   * @return SensorInfo instance found in mSensorInfos with the given
   *         SensorType, or nullptr if not found
   */
  const SensorInfo *getSensorInfo(SensorType sensorType) const;
};

#ifdef CHRE_SLPI_UIMG_ENABLED
/**
 * A version of SeeHelper that explicitly uses the QMI API on the bottom edge
 * and therefore only works in big image (but goes through CM instead of QCM
 * within SEE).
 *
 * @see SeeHelper
 */
class BigImageSeeHelper : public SeeHelper {
 public:
  BigImageSeeHelper(SeeCalHelper *calHelper) : SeeHelper(calHelper) {
    mSnsClientApi = &kQmiApi;
  }

 private:
  static const SnsClientApi kQmiApi;
};
#endif  // CHRE_SLPI_UIMG_ENABLED

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SEE_SEE_HELPER_H_
