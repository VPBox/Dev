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

#ifndef ANDROID_HARDWARE_CONTEXTHUB_V1_0_CONTEXTHUB_H
#define ANDROID_HARDWARE_CONTEXTHUB_V1_0_CONTEXTHUB_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>

#include <android/hardware/contexthub/1.0/IContexthub.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include "chre_host/socket_client.h"
#include "chre_host/host_protocol_host.h"
#include "chre_host/fragmented_load_transaction.h"

namespace android {
namespace hardware {
namespace contexthub {
namespace V1_0 {
namespace implementation {

using ::android::chre::FragmentedLoadRequest;
using ::android::chre::FragmentedLoadTransaction;
using ::android::hardware::contexthub::V1_0::ContextHub;
using ::android::hardware::contexthub::V1_0::ContextHubMsg;
using ::android::hardware::contexthub::V1_0::IContexthub;
using ::android::hardware::contexthub::V1_0::IContexthubCallback;
using ::android::hardware::contexthub::V1_0::NanoAppBinary;
using ::android::hardware::contexthub::V1_0::Result;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::sp;

class GenericContextHub : public IContexthub {
 public:
  GenericContextHub();

  Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& options) override;

  // Methods from ::android::hardware::contexthub::V1_0::IContexthub follow.
  Return<void> getHubs(getHubs_cb _hidl_cb) override;
  Return<Result> registerCallback(uint32_t hubId, const sp<IContexthubCallback>& cb) override;
  Return<Result> sendMessageToHub(uint32_t hubId, const ContextHubMsg& msg) override;
  Return<Result> loadNanoApp(uint32_t hubId, const NanoAppBinary& appBinary, uint32_t transactionId) override;
  Return<Result> unloadNanoApp(uint32_t hubId, uint64_t appId, uint32_t transactionId) override;
  Return<Result> enableNanoApp(uint32_t hubId, uint64_t appId, uint32_t transactionId) override;
  Return<Result> disableNanoApp(uint32_t hubId, uint64_t appId, uint32_t transactionId) override;
  Return<Result> queryApps(uint32_t hubId) override;

 private:
  ::android::chre::SocketClient mClient;
  sp<IContexthubCallback> mCallbacks;
  std::mutex mCallbacksLock;

  class SocketCallbacks : public ::android::chre::SocketClient::ICallbacks,
                          public ::android::chre::IChreMessageHandlers {
   public:
    explicit SocketCallbacks(GenericContextHub& parent);
    void onMessageReceived(const void *data, size_t length) override;
    void onConnected() override;
    void onDisconnected() override;

    void handleNanoappMessage(
        const ::chre::fbs::NanoappMessageT& message) override;

    void handleHubInfoResponse(
        const ::chre::fbs::HubInfoResponseT& response) override;

    void handleNanoappListResponse(
        const ::chre::fbs::NanoappListResponseT& response) override;

    void handleLoadNanoappResponse(
      const ::chre::fbs::LoadNanoappResponseT& response) override;

    void handleUnloadNanoappResponse(
      const ::chre::fbs::UnloadNanoappResponseT& response) override;

    void handleDebugDumpData(
      const ::chre::fbs::DebugDumpDataT& data) override;

    void handleDebugDumpResponse(
      const ::chre::fbs::DebugDumpResponseT& response) override;

   private:
    GenericContextHub& mParent;
    bool mHaveConnected = false;

    /**
     * Acquires mParent.mCallbacksLock and invokes the synchronous callback
     * argument if mParent.mCallbacks is not null.
     */
    void invokeClientCallback(std::function<Return<void>()> callback);
  };

  class DeathRecipient : public hidl_death_recipient {
   public:
    explicit DeathRecipient(const sp<GenericContextHub> contexthub);
    void serviceDied(uint64_t cookie,
                     const wp<::android::hidl::base::V1_0::IBase>& who)
        override;

   private:
    sp<GenericContextHub> mGenericContextHub;
  };

  sp<SocketCallbacks> mSocketCallbacks;
  sp<DeathRecipient> mDeathRecipient;

  // Cached hub info used for getHubs(), and synchronization primitives to make
  // that function call synchronous if we need to query it
  ContextHub mHubInfo;
  bool mHubInfoValid = false;
  std::mutex mHubInfoMutex;
  std::condition_variable mHubInfoCond;

  static constexpr int kInvalidFd = -1;
  int mDebugFd = kInvalidFd;
  bool mDebugDumpPending = false;
  std::mutex mDebugDumpMutex;
  std::condition_variable mDebugDumpCond;

  // The pending fragmented load request
  uint32_t mCurrentFragmentId = 0;
  std::optional<FragmentedLoadTransaction> mPendingLoadTransaction;
  std::mutex mPendingLoadTransactionMutex;

  // Use 30KB fragment size to fit within 32KB memory fragments at the kernel
  static constexpr size_t kLoadFragmentSizeBytes = 30 * 1024;

  // Write a string to mDebugFd
  void writeToDebugFile(const char *str);
  void writeToDebugFile(const char *str, size_t len);

  // Unregisters callback when context hub service dies
  void handleServiceDeath(uint32_t hubId);

  /**
   * Checks to see if a load response matches the currently pending
   * fragmented load transaction. mPendingLoadTransactionMutex must
   * be acquired prior to calling this function.
   *
   * @param response the received load response
   *
   * @return true if the response matches a pending load transaction
   *         (if any), false otherwise
   */
  bool isExpectedLoadResponseLocked(
      const ::chre::fbs::LoadNanoappResponseT& response);

  /**
   * Sends a fragmented load request to CHRE. The caller must ensure that
   * transaction.isComplete() returns false prior to invoking this method.
   *
   * @param transaction the FragmentedLoadTransaction object
   *
   * @return the result of the request
   */
  Result sendFragmentedLoadNanoAppRequest(
      FragmentedLoadTransaction& transaction);
};

extern "C" IContexthub* HIDL_FETCH_IContexthub(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace contexthub
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_CONTEXTHUB_V1_0_CONTEXTHUB_H
