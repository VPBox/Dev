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

#define LOG_TAG "ContextHubHal"
#define LOG_NDEBUG 0

#include "generic_context_hub.h"

#include <chrono>
#include <cinttypes>
#include <vector>

#include <log/log.h>
#include <unistd.h>

namespace android {
namespace hardware {
namespace contexthub {
namespace V1_0 {
namespace implementation {

using ::android::chre::getStringFromByteVector;
using ::android::hardware::Return;
using ::android::hardware::contexthub::V1_0::AsyncEventType;
using ::android::hardware::contexthub::V1_0::Result;
using ::android::hardware::contexthub::V1_0::TransactionResult;
using ::android::chre::HostProtocolHost;
using ::flatbuffers::FlatBufferBuilder;

// Aliased for consistency with the way these symbols are referenced in
// CHRE-side code
namespace fbs = ::chre::fbs;

namespace {

constexpr uint32_t kDefaultHubId = 0;

constexpr uint8_t extractChreApiMajorVersion(uint32_t chreVersion) {
  return static_cast<uint8_t>(chreVersion >> 24);
}

constexpr uint8_t extractChreApiMinorVersion(uint32_t chreVersion) {
  return static_cast<uint8_t>(chreVersion >> 16);
}

constexpr uint16_t extractChrePatchVersion(uint32_t chreVersion) {
  return static_cast<uint16_t>(chreVersion);
}

/**
 * @return file descriptor contained in the hidl_handle, or -1 if there is none
 */
int hidlHandleToFileDescriptor(const hidl_handle& hh) {
  const native_handle_t *handle = hh.getNativeHandle();
  return (handle != nullptr && handle->numFds >= 1) ? handle->data[0] : -1;
}

}  // anonymous namespace

GenericContextHub::DeathRecipient::DeathRecipient(
    sp<GenericContextHub> contexthub) : mGenericContextHub(contexthub){}

void GenericContextHub::DeathRecipient::serviceDied(
    uint64_t cookie, const wp<::android::hidl::base::V1_0::IBase>& /* who */) {
  uint32_t hubId = static_cast<uint32_t>(cookie);
  mGenericContextHub->handleServiceDeath(hubId);
}

GenericContextHub::GenericContextHub() {
  constexpr char kChreSocketName[] = "chre";

  mSocketCallbacks = new SocketCallbacks(*this);
  if (!mClient.connectInBackground(kChreSocketName, mSocketCallbacks)) {
    ALOGE("Couldn't start socket client");
  }

  mDeathRecipient = new DeathRecipient(this);
}

Return<void> GenericContextHub::debug(
    const hidl_handle& hh_fd, const hidl_vec<hidl_string>& /*options*/) {
  // Timeout inside CHRE is typically 5 seconds, grant 500ms extra here to let
  // the data reach us
  constexpr auto kDebugDumpTimeout = std::chrono::milliseconds(5500);

  mDebugFd = hidlHandleToFileDescriptor(hh_fd);
  if (mDebugFd < 0) {
    ALOGW("Can't dump debug info to invalid fd");
  } else {
    writeToDebugFile("-- Dumping CHRE/ASH debug info --\n");

    ALOGV("Sending debug dump request");
    FlatBufferBuilder builder;
    HostProtocolHost::encodeDebugDumpRequest(builder);
    std::unique_lock<std::mutex> lock(mDebugDumpMutex);
    mDebugDumpPending = true;
    if (!mClient.sendMessage(builder.GetBufferPointer(), builder.GetSize())) {
      ALOGW("Couldn't send debug dump request");
    } else {
      mDebugDumpCond.wait_for(lock, kDebugDumpTimeout,
                              [this]() { return !mDebugDumpPending; });
      if (mDebugDumpPending) {
        ALOGI("Timed out waiting on debug dump data");
        mDebugDumpPending = false;
      }
    }
    writeToDebugFile("\n-- End of CHRE/ASH debug info --\n");

    mDebugFd = kInvalidFd;
    ALOGV("Debug dump complete");
  }

  return Void();
}

Return<void> GenericContextHub::getHubs(getHubs_cb _hidl_cb) {
  constexpr auto kHubInfoQueryTimeout = std::chrono::seconds(5);
  std::vector<ContextHub> hubs;
  ALOGV("%s", __func__);

  // If we're not connected yet, give it some time
  // TODO refactor from polling into conditional wait
  int maxSleepIterations = 250;
  while (!mHubInfoValid && !mClient.isConnected() && --maxSleepIterations > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  if (!mClient.isConnected()) {
    ALOGE("Couldn't connect to hub daemon");
  } else if (!mHubInfoValid) {
    // We haven't cached the hub details yet, so send a request and block
    // waiting on a response
    std::unique_lock<std::mutex> lock(mHubInfoMutex);
    FlatBufferBuilder builder;
    HostProtocolHost::encodeHubInfoRequest(builder);

    ALOGD("Sending hub info request");
    if (!mClient.sendMessage(builder.GetBufferPointer(), builder.GetSize())) {
      ALOGE("Couldn't send hub info request");
    } else {
      mHubInfoCond.wait_for(lock, kHubInfoQueryTimeout,
                            [this]() { return mHubInfoValid; });
    }
  }

  if (mHubInfoValid) {
    hubs.push_back(mHubInfo);
  } else {
    ALOGE("Unable to get hub info from CHRE");
  }

  _hidl_cb(hubs);
  return Void();
}

Return<Result> GenericContextHub::registerCallback(
    uint32_t hubId, const sp<IContexthubCallback>& cb) {
  Result result;
  ALOGV("%s", __func__);

  // TODO: currently we only support 1 hub behind this HAL implementation
  if (hubId == kDefaultHubId) {
    std::lock_guard<std::mutex> lock(mCallbacksLock);

    if (cb != nullptr) {
      if (mCallbacks != nullptr) {
        ALOGD("Modifying callback for hubId %" PRIu32, hubId);
        mCallbacks->unlinkToDeath(mDeathRecipient);
      }
      Return<bool> linkReturn = cb->linkToDeath(mDeathRecipient, hubId);
      if (!linkReturn.withDefault(false)) {
        ALOGW("Could not link death recipient to hubId %" PRIu32, hubId);
      }
    }

    mCallbacks = cb;
    result = Result::OK;
  } else {
    result = Result::BAD_PARAMS;
  }

  return result;
}

Return<Result> GenericContextHub::sendMessageToHub(uint32_t hubId,
                                                   const ContextHubMsg& msg) {
  Result result;
  ALOGV("%s", __func__);

  if (hubId != kDefaultHubId) {
    result = Result::BAD_PARAMS;
  } else {
    FlatBufferBuilder builder(1024);
    HostProtocolHost::encodeNanoappMessage(
        builder, msg.appName, msg.msgType, msg.hostEndPoint, msg.msg.data(),
        msg.msg.size());

    if (!mClient.sendMessage(builder.GetBufferPointer(), builder.GetSize())) {
      result = Result::UNKNOWN_FAILURE;
    } else {
      result = Result::OK;
    }
  }

  return result;
}

Return<Result> GenericContextHub::loadNanoApp(
    uint32_t hubId, const NanoAppBinary& appBinary, uint32_t transactionId) {
  Result result;
  ALOGV("%s", __func__);

  if (hubId != kDefaultHubId) {
    result = Result::BAD_PARAMS;
  } else {
    std::lock_guard<std::mutex> lock(mPendingLoadTransactionMutex);

    if (mPendingLoadTransaction.has_value()) {
      ALOGE("Pending load transaction exists. Overriding pending request");
    }

    uint32_t targetApiVersion = (appBinary.targetChreApiMajorVersion << 24) |
                                (appBinary.targetChreApiMinorVersion << 16);
    mPendingLoadTransaction = FragmentedLoadTransaction(
        transactionId, appBinary.appId, appBinary.appVersion, targetApiVersion,
        appBinary.customBinary, kLoadFragmentSizeBytes);

    result = sendFragmentedLoadNanoAppRequest(
        mPendingLoadTransaction.value());
    if (result != Result::OK) {
      mPendingLoadTransaction.reset();
    }
  }

  ALOGD("Attempted to send load nanoapp request for app of size %zu with ID "
        "0x%016" PRIx64 " as transaction ID %" PRIu32 ": result %" PRIu32,
        appBinary.customBinary.size(), appBinary.appId, transactionId, result);

  return result;
}

Result GenericContextHub::sendFragmentedLoadNanoAppRequest(
    FragmentedLoadTransaction& transaction) {
  Result result;
  const FragmentedLoadRequest& request = transaction.getNextRequest();

  FlatBufferBuilder builder(128 + request.binary.size());
  HostProtocolHost::encodeFragmentedLoadNanoappRequest(builder, request);

  if (!mClient.sendMessage(builder.GetBufferPointer(), builder.GetSize())) {
    ALOGE("Failed to send load request message (fragment ID = %zu)",
          request.fragmentId);
    result = Result::UNKNOWN_FAILURE;
  } else {
    mCurrentFragmentId = request.fragmentId;
    result = Result::OK;
  }

  return result;
}

Return<Result> GenericContextHub::unloadNanoApp(
    uint32_t hubId, uint64_t appId, uint32_t transactionId) {
  Result result;
  ALOGV("%s", __func__);

  if (hubId != kDefaultHubId) {
    result = Result::BAD_PARAMS;
  } else {
    FlatBufferBuilder builder(64);
    HostProtocolHost::encodeUnloadNanoappRequest(
        builder, transactionId, appId, false /* allowSystemNanoappUnload */);
    if (!mClient.sendMessage(builder.GetBufferPointer(), builder.GetSize())) {
      result = Result::UNKNOWN_FAILURE;
    } else {
      result = Result::OK;
    }
  }

  ALOGD("Attempted to send unload nanoapp request for app ID 0x%016" PRIx64
        " as transaction ID %" PRIu32 ": result %" PRIu32, appId, transactionId,
        result);

  return result;
}

Return<Result> GenericContextHub::enableNanoApp(
    uint32_t /* hubId */, uint64_t appId, uint32_t /* transactionId */) {
  // TODO
  ALOGW("Attempted to enable app ID 0x%016" PRIx64 ", but not supported",
        appId);
  return Result::TRANSACTION_FAILED;
}

Return<Result> GenericContextHub::disableNanoApp(
    uint32_t /* hubId */, uint64_t appId, uint32_t /* transactionId */) {
  // TODO
  ALOGW("Attempted to disable app ID 0x%016" PRIx64 ", but not supported",
        appId);
  return Result::TRANSACTION_FAILED;
}

Return<Result> GenericContextHub::queryApps(uint32_t hubId) {
  Result result;
  ALOGV("%s", __func__);

  if (hubId != kDefaultHubId) {
    result = Result::BAD_PARAMS;
  } else {
    FlatBufferBuilder builder(64);
    HostProtocolHost::encodeNanoappListRequest(builder);
    if (!mClient.sendMessage(builder.GetBufferPointer(), builder.GetSize())) {
      result = Result::UNKNOWN_FAILURE;
    } else {
      result = Result::OK;
    }
  }

  return result;
}

GenericContextHub::SocketCallbacks::SocketCallbacks(GenericContextHub& parent)
    : mParent(parent) {}

void GenericContextHub::SocketCallbacks::onMessageReceived(const void *data,
                                                           size_t length) {
  if (!HostProtocolHost::decodeMessageFromChre(data, length, *this)) {
    ALOGE("Failed to decode message");
  }
}

void GenericContextHub::SocketCallbacks::onConnected() {
  if (mHaveConnected) {
    ALOGI("Reconnected to CHRE daemon");
    invokeClientCallback([&]() {
      return mParent.mCallbacks->handleHubEvent(AsyncEventType::RESTARTED);
    });
  }
  mHaveConnected = true;
}

void GenericContextHub::SocketCallbacks::onDisconnected() {
  ALOGW("Lost connection to CHRE daemon");
}

void GenericContextHub::SocketCallbacks::handleNanoappMessage(
    const fbs::NanoappMessageT& message) {
  ContextHubMsg msg;
  msg.appName = message.app_id;
  msg.hostEndPoint = message.host_endpoint;
  msg.msgType = message.message_type;
  msg.msg = message.message;

  invokeClientCallback([&]() {
    return mParent.mCallbacks->handleClientMsg(msg);
  });
}

void GenericContextHub::SocketCallbacks::handleHubInfoResponse(
    const fbs::HubInfoResponseT& response) {
  ALOGD("Got hub info response");

  std::lock_guard<std::mutex> lock(mParent.mHubInfoMutex);
  if (mParent.mHubInfoValid) {
    ALOGI("Ignoring duplicate/unsolicited hub info response");
  } else {
    mParent.mHubInfo.name = getStringFromByteVector(response.name);
    mParent.mHubInfo.vendor = getStringFromByteVector(response.vendor);
    mParent.mHubInfo.toolchain = getStringFromByteVector(response.toolchain);
    mParent.mHubInfo.platformVersion = response.platform_version;
    mParent.mHubInfo.toolchainVersion = response.toolchain_version;
    mParent.mHubInfo.hubId = kDefaultHubId;

    mParent.mHubInfo.peakMips = response.peak_mips;
    mParent.mHubInfo.stoppedPowerDrawMw = response.stopped_power;
    mParent.mHubInfo.sleepPowerDrawMw = response.sleep_power;
    mParent.mHubInfo.peakPowerDrawMw = response.peak_power;

    mParent.mHubInfo.maxSupportedMsgLen = response.max_msg_len;
    mParent.mHubInfo.chrePlatformId = response.platform_id;

    uint32_t version = response.chre_platform_version;
    mParent.mHubInfo.chreApiMajorVersion = extractChreApiMajorVersion(version);
    mParent.mHubInfo.chreApiMinorVersion = extractChreApiMinorVersion(version);
    mParent.mHubInfo.chrePatchVersion = extractChrePatchVersion(version);

    mParent.mHubInfoValid = true;
    mParent.mHubInfoCond.notify_all();
  }
}

void GenericContextHub::SocketCallbacks::handleNanoappListResponse(
    const fbs::NanoappListResponseT& response) {
  std::vector<HubAppInfo> appInfoList;

  ALOGV("Got nanoapp list response with %zu apps", response.nanoapps.size());
  for (const std::unique_ptr<fbs::NanoappListEntryT>& nanoapp
         : response.nanoapps) {
    // TODO: determine if this is really required, and if so, have
    // HostProtocolHost strip out null entries as part of decode
    if (nanoapp == nullptr) {
      continue;
    }

    ALOGV("App 0x%016" PRIx64 " ver 0x%" PRIx32 " enabled %d system %d",
          nanoapp->app_id, nanoapp->version, nanoapp->enabled,
          nanoapp->is_system);
    if (!nanoapp->is_system) {
      HubAppInfo appInfo;

      appInfo.appId = nanoapp->app_id;
      appInfo.version = nanoapp->version;
      appInfo.enabled = nanoapp->enabled;

      appInfoList.push_back(appInfo);
    }
  }

  invokeClientCallback([&]() {
    return mParent.mCallbacks->handleAppsInfo(appInfoList);
  });
}

void GenericContextHub::SocketCallbacks::handleLoadNanoappResponse(
    const ::chre::fbs::LoadNanoappResponseT& response) {
  ALOGV("Got load nanoapp response for transaction %" PRIu32 " fragment %"
        PRIu32 " with result %d", response.transaction_id, response.fragment_id,
        response.success);
  std::unique_lock<std::mutex> lock(mParent.mPendingLoadTransactionMutex);

  // TODO: Handle timeout in receiving load response
  if (!mParent.mPendingLoadTransaction.has_value()) {
    ALOGE("Dropping unexpected load response (no pending transaction exists)");
  } else {
    FragmentedLoadTransaction& transaction =
        mParent.mPendingLoadTransaction.value();

    if (!mParent.isExpectedLoadResponseLocked(response)) {
      ALOGE("Dropping unexpected load response, expected transaction %"
            PRIu32 " fragment %" PRIu32 ", received transaction %" PRIu32
            " fragment %" PRIu32, transaction.getTransactionId(),
            mParent.mCurrentFragmentId, response.transaction_id,
            response.fragment_id);
    } else {
      TransactionResult result;
      bool continueLoadRequest = false;
      if (response.success && !transaction.isComplete()) {
        if (mParent.sendFragmentedLoadNanoAppRequest(transaction)
            == Result::OK) {
          continueLoadRequest = true;
          result = TransactionResult::SUCCESS;
        } else {
          result = TransactionResult::FAILURE;
        }
      } else {
        result = (response.success) ?
            TransactionResult::SUCCESS : TransactionResult::FAILURE;
      }

      if (!continueLoadRequest) {
        mParent.mPendingLoadTransaction.reset();
        lock.unlock();
        invokeClientCallback([&]() {
          return mParent.mCallbacks->handleTxnResult(
              response.transaction_id, result);
        });
      }
    }
  }
}

bool GenericContextHub::isExpectedLoadResponseLocked(
    const ::chre::fbs::LoadNanoappResponseT& response) {
  return mPendingLoadTransaction.has_value()
      && (mPendingLoadTransaction->getTransactionId()
          == response.transaction_id)
      && (response.fragment_id == 0
          || mCurrentFragmentId == response.fragment_id);
}

void GenericContextHub::SocketCallbacks::handleUnloadNanoappResponse(
    const ::chre::fbs::UnloadNanoappResponseT& response) {
  ALOGV("Got unload nanoapp response for transaction %" PRIu32 " with result "
        "%d", response.transaction_id, response.success);

  invokeClientCallback([&]() {
    TransactionResult result = (response.success) ?
        TransactionResult::SUCCESS : TransactionResult::FAILURE;
    return mParent.mCallbacks->handleTxnResult(response.transaction_id, result);
  });
}

void GenericContextHub::SocketCallbacks::handleDebugDumpData(
    const ::chre::fbs::DebugDumpDataT& data) {
  ALOGV("Got debug dump data, size %zu", data.debug_str.size());
  if (mParent.mDebugFd == kInvalidFd) {
    ALOGW("Got unexpected debug dump data message");
  } else {
    mParent.writeToDebugFile(
        reinterpret_cast<const char *>(data.debug_str.data()),
        data.debug_str.size());
  }
}

void GenericContextHub::SocketCallbacks::handleDebugDumpResponse(
    const ::chre::fbs::DebugDumpResponseT& response) {
  ALOGV("Got debug dump response, success %d, data count %" PRIu32,
        response.success, response.data_count);
  std::lock_guard<std::mutex> lock(mParent.mDebugDumpMutex);
  if (!mParent.mDebugDumpPending) {
    ALOGI("Ignoring duplicate/unsolicited debug dump response");
  } else {
    mParent.mDebugDumpPending = false;
    mParent.mDebugDumpCond.notify_all();
  }
}

void GenericContextHub::SocketCallbacks::invokeClientCallback(
    std::function<Return<void>()> callback) {
  std::lock_guard<std::mutex> lock(mParent.mCallbacksLock);
  if (mParent.mCallbacks != nullptr && !callback().isOk()) {
    ALOGE("Failed to invoke client callback");
  }
}

void GenericContextHub::writeToDebugFile(const char *str) {
  writeToDebugFile(str, strlen(str));
}

void GenericContextHub::writeToDebugFile(const char *str, size_t len) {
  ssize_t written = write(mDebugFd, str, len);
  if (written != (ssize_t) len) {
    ALOGW("Couldn't write to debug header: returned %zd, expected %zu "
          "(errno %d)", written, len, errno);
  }
}

void GenericContextHub::handleServiceDeath(uint32_t hubId) {
  std::lock_guard<std::mutex> lock(mCallbacksLock);
  ALOGI("Context hub service died for hubId %" PRIu32, hubId);
  mCallbacks.clear();
}

IContexthub* HIDL_FETCH_IContexthub(const char* /* name */) {
  return new GenericContextHub();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace contexthub
}  // namespace hardware
}  // namespace android
