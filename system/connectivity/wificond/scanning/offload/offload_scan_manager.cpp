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
#include "wificond/scanning/offload/offload_scan_manager.h"

#include <vector>

#include <android-base/logging.h>

#include "wificond/scanning/offload/hidl_call_util.h"
#include "wificond/scanning/offload/offload_scan_utils.h"
#include "wificond/scanning/offload/offload_service_utils.h"
#include "wificond/scanning/offload/scan_stats.h"
#include "wificond/scanning/scan_result.h"

using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::ScanFilter;
using android::hardware::wifi::offload::V1_0::ScanParam;
using android::hardware::wifi::offload::V1_0::ScanStats;
using android::hardware::wifi::offload::V1_0::OffloadStatus;
using android::hardware::wifi::offload::V1_0::OffloadStatusCode;

using ::com::android::server::wifi::wificond::NativeScanResult;
using ::com::android::server::wifi::wificond::NativeScanStats;
using std::vector;
using std::weak_ptr;
using std::shared_ptr;

using namespace std::placeholders;

namespace {
const uint32_t kSubscriptionDelayMs = 5000;
}

namespace android {
namespace wificond {

OffloadCallbackHandlersImpl::OffloadCallbackHandlersImpl(
    OffloadScanManager* offload_scan_manager)
    : offload_scan_manager_(offload_scan_manager) {}

OffloadCallbackHandlersImpl::~OffloadCallbackHandlersImpl() {}

void OffloadCallbackHandlersImpl::OnScanResultHandler(
    const vector<ScanResult>& scanResult) {
  if (offload_scan_manager_ != nullptr) {
    offload_scan_manager_->ReportScanResults(scanResult);
  }
}

void OffloadCallbackHandlersImpl::OnErrorHandler(const OffloadStatus& status) {
  if (offload_scan_manager_ != nullptr) {
    offload_scan_manager_->ReportError(status);
  }
}

OffloadScanManager::OffloadScanManager(
    weak_ptr<OffloadServiceUtils> utils,
    shared_ptr<OffloadScanCallbackInterface> callback)
    : wifi_offload_hal_(nullptr),
      wifi_offload_callback_(nullptr),
      death_recipient_(nullptr),
      offload_status_(OffloadScanManager::kError),
      service_available_(false),
      offload_service_utils_(utils),
      offload_callback_handlers_(new OffloadCallbackHandlersImpl(this)),
      event_callback_(callback) {
  if (InitService()) {
    offload_status_ = OffloadScanManager::kNoError;
  }
}

bool OffloadScanManager::InitService() {
  wifi_offload_hal_ = offload_service_utils_.lock()->GetOffloadService();
  if (wifi_offload_hal_ == nullptr) {
    LOG(ERROR) << "No Offload Service available";
    return false;
  }

  death_recipient_ = offload_service_utils_.lock()->GetOffloadDeathRecipient(
      std::bind(&OffloadScanManager::OnObjectDeath, this, _1));
  uint64_t cookie = reinterpret_cast<uint64_t>(wifi_offload_hal_.get());

  auto link_to_death_status =
      wifi_offload_hal_->linkToDeath(death_recipient_, cookie);
  if (!link_to_death_status.isOk()) {
    LOG(ERROR) << "Unable to register death handler "
               << link_to_death_status.description();
    return false;
  }

  wifi_offload_callback_ = offload_service_utils_.lock()->GetOffloadCallback(
      offload_callback_handlers_.get());
  if (wifi_offload_callback_ == nullptr) {
    LOG(ERROR) << "Invalid Offload callback object";
    return false;
  }

  auto set_callback_status =
      wifi_offload_hal_->setEventCallback(wifi_offload_callback_);
  if (!set_callback_status.isOk()) {
    LOG(ERROR) << "Unable to set event callback for Offload HAL";
    return false;
  }

  service_available_ = true;
  return true;
}

bool OffloadScanManager::InitServiceIfNeeded() {
  if (!service_available_) {
    return InitService();
  }
  return true;
}

bool OffloadScanManager::stopScan(OffloadScanManager::ReasonCode* reason_code) {
  if (!InitServiceIfNeeded() ||
      (getOffloadStatus() != OffloadScanManager::kNoError)) {
    *reason_code = OffloadScanManager::kNotAvailable;
    return false;
  }
  const auto& res = wifi_offload_hal_->unsubscribeScanResults();
  if (!res.isOk()) {
    *reason_code = OffloadScanManager::kTransactionFailed;
    LOG(WARNING) << "unsubscribeScanResults() failed " << res.description();
    return false;
  }
  *reason_code = OffloadScanManager::kNone;
  return true;
}

bool OffloadScanManager::GetScanStats(NativeScanStats* native_scan_stats) {
  const auto& result = HIDL_INVOKE(wifi_offload_hal_, getScanStats);
  const auto& offload_status_and_scan_stats = result.first;
  bool transport_status = result.second;
  if (!transport_status) {
    return false;
  }
  OffloadStatus offload_status = offload_status_and_scan_stats.first;
  ScanStats scan_stats = offload_status_and_scan_stats.second;
  if (offload_status.code != OffloadStatusCode::OK) {
    LOG(WARNING) << offload_status.description;
    return false;
  }
  *native_scan_stats = OffloadScanUtils::convertToNativeScanStats(scan_stats);
  return true;
}

bool OffloadScanManager::VerifyAndConvertHIDLStatus(
    std::pair<OffloadStatus, bool> result,
    OffloadScanManager::ReasonCode* reason_code) {
  const auto& offload_status = result.first;
  bool transport_status = result.second;
  if (!transport_status) {
    *reason_code = OffloadScanManager::kTransactionFailed;
    return false;
  }
  if (offload_status.code != OffloadStatusCode::OK) {
    LOG(WARNING) << offload_status.description;
    *reason_code = OffloadScanManager::kOperationFailed;
    return false;
  }
  return true;
}

bool OffloadScanManager::startScan(
    uint32_t interval_ms, int32_t rssi_threshold,
    const vector<vector<uint8_t>>& scan_ssids,
    const vector<vector<uint8_t>>& match_ssids,
    const vector<uint8_t>& match_security, const vector<uint32_t>& freqs,
    OffloadScanManager::ReasonCode* reason_code) {
  if (!InitServiceIfNeeded() ||
      getOffloadStatus() != OffloadScanManager::kNoError) {
    *reason_code = OffloadScanManager::kNotAvailable;
    LOG(WARNING) << "Offload HAL scans are not available";
    return false;
  }
  ScanParam param =
      OffloadScanUtils::createScanParam(scan_ssids, freqs, interval_ms);
  ScanFilter filter = OffloadScanUtils::createScanFilter(
      match_ssids, match_security, rssi_threshold);

  if (!ConfigureScans(param, filter, reason_code)) {
    return false;
  }

  if (!SubscribeScanResults(reason_code)) {
    return false;
  }

  *reason_code = OffloadScanManager::kNone;
  return true;
}

bool OffloadScanManager::ConfigureScans(
    ScanParam param, ScanFilter filter,
    OffloadScanManager::ReasonCode* reason_code) {
  const auto& result =
      HIDL_INVOKE(wifi_offload_hal_, configureScans, param, filter);
  if (!VerifyAndConvertHIDLStatus(result, reason_code)) {
    return false;
  }
  return true;
}

bool OffloadScanManager::SubscribeScanResults(
    OffloadScanManager::ReasonCode* reason_code) {
  const auto& result = HIDL_INVOKE(wifi_offload_hal_, subscribeScanResults,
                                   kSubscriptionDelayMs);
  if (!VerifyAndConvertHIDLStatus(result, reason_code)) {
    return false;
  }
  return true;
}

OffloadScanManager::StatusCode OffloadScanManager::getOffloadStatus() const {
  if (!service_available_) {
    return OffloadScanManager::kNoService;
  }
  return offload_status_;
}

bool OffloadScanManager::getScanResults(
    std::vector<NativeScanResult>* out_scan_results) {
  for (const auto& scan_result : cached_scan_results_) {
    out_scan_results->push_back(scan_result);
  }
  return true;
}

bool OffloadScanManager::getScanStats(NativeScanStats* native_scan_stats) {
  if (!InitServiceIfNeeded()) {
    LOG(ERROR) << "Offload HAL service unavailable";
    return false;
  }
  if (getOffloadStatus() != OffloadScanManager::kNoError) {
    LOG(WARNING) << "Unable to get scan stats due to Wifi Offload HAL error";
    return false;
  }
  return GetScanStats(native_scan_stats);
}

OffloadScanManager::~OffloadScanManager() {
  if (wifi_offload_hal_ != nullptr) {
    wifi_offload_hal_->unlinkToDeath(death_recipient_);
  }
}

void OffloadScanManager::ReportScanResults(
    const vector<ScanResult>& scanResult) {
  cached_scan_results_.clear();
  if (!OffloadScanUtils::convertToNativeScanResults(scanResult,
                                                    &cached_scan_results_)) {
    LOG(WARNING) << "Unable to convert scan results to native format";
    return;
  }
  if (event_callback_ != nullptr) {
    event_callback_->OnOffloadScanResult();
  } else {
    LOG(WARNING)
        << "No callback to report Offload HAL's scan results to wificond";
  }
}

void OffloadScanManager::ReportError(const OffloadStatus& status) {
  OffloadStatusCode status_code = status.code;
  OffloadScanManager::StatusCode status_result = OffloadScanManager::kNoError;
  switch (status_code) {
    case OffloadStatusCode::OK:
      status_result = OffloadScanManager::kNoError;
      break;
    case OffloadStatusCode::TIMEOUT:
      status_result = OffloadScanManager::kTimeOut;
      break;
    case OffloadStatusCode::NO_CONNECTION:
      status_result = OffloadScanManager::kNotConnected;
      break;
    case OffloadStatusCode::ERROR:
      status_result = OffloadScanManager::kError;
      break;
    default:
      LOG(WARNING) << "Invalid Offload Error reported";
      return;
  }
  if (status_result != OffloadScanManager::kNoError) {
    LOG(WARNING) << "Offload Error reported " << status.description;
    if (event_callback_ != nullptr) {
      event_callback_->OnOffloadError(
          OffloadScanCallbackInterface::REMOTE_FAILURE);
    } else {
      LOG(WARNING) << "No callback to report Offload HAL Errors to wificond";
    }
  }
  offload_status_ = status_result;
}

void OffloadScanManager::OnObjectDeath(uint64_t cookie) {
  if (wifi_offload_hal_ == reinterpret_cast<IOffload*>(cookie)) {
    LOG(ERROR) << "Death Notification for Wifi Offload HAL";
    wifi_offload_hal_.clear();
    if (event_callback_ != nullptr) {
      event_callback_->OnOffloadError(
          OffloadScanCallbackInterface::BINDER_DEATH);
    } else {
      LOG(WARNING)
          << "No callback to report Offload HAL Binder death to wificond";
    }
    service_available_ = false;
    death_recipient_.clear();
  }
}

}  // namespace wificond
}  // namespace android
