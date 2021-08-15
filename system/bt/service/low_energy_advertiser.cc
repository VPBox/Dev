//
//  Copyright 2015 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "service/low_energy_advertiser.h"

#include "service/adapter.h"
#include "service/logging_helpers.h"
#include "stack/include/bt_types.h"
#include "stack/include/hcidefs.h"

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/callback.h>
#include <base/logging.h>

using std::lock_guard;
using std::mutex;

namespace bluetooth {

namespace {

BLEStatus GetBLEStatus(int status) {
  if (status == BT_STATUS_FAIL) return BLE_STATUS_FAILURE;

  return static_cast<BLEStatus>(status);
}

// The Bluetooth Core Specification defines time interval (e.g. Page Scan
// Interval, Advertising Interval, etc) units as 0.625 milliseconds (or 1
// Baseband slot). The HAL advertising functions expect the interval in this
// unit. This function maps an AdvertiseSettings::Mode value to the
// corresponding time unit.
int GetAdvertisingIntervalUnit(AdvertiseSettings::Mode mode) {
  int ms;

  switch (mode) {
    case AdvertiseSettings::MODE_BALANCED:
      ms = kAdvertisingIntervalMediumMs;
      break;
    case AdvertiseSettings::MODE_LOW_LATENCY:
      ms = kAdvertisingIntervalLowMs;
      break;
    case AdvertiseSettings::MODE_LOW_POWER:
      FALLTHROUGH_INTENDED; /* FALLTHROUGH */
    default:
      ms = kAdvertisingIntervalHighMs;
      break;
  }

  // Convert milliseconds to Bluetooth units.
  return (ms * 1000) / 625;
}

int8_t GetAdvertisingTxPower(AdvertiseSettings::TxPowerLevel tx_power) {
  int8_t power;

  switch (tx_power) {
    case AdvertiseSettings::TX_POWER_LEVEL_ULTRA_LOW:
      power = -21;
      break;
    case AdvertiseSettings::TX_POWER_LEVEL_LOW:
      power = -15;
      break;
    case AdvertiseSettings::TX_POWER_LEVEL_MEDIUM:
      power = -7;
      break;
    case AdvertiseSettings::TX_POWER_LEVEL_HIGH:
      FALLTHROUGH_INTENDED; /* FALLTHROUGH */
    default:
      power = 1;
      break;
  }

  return power;
}

void GetAdvertiseParams(const AdvertiseSettings& settings, bool has_scan_rsp,
                        AdvertiseParameters* out_params) {
  CHECK(out_params);

  out_params->min_interval = GetAdvertisingIntervalUnit(settings.mode());
  out_params->max_interval =
      out_params->min_interval + kAdvertisingIntervalDeltaUnit;

  if (settings.connectable())
    out_params->advertising_event_properties =
        kAdvertisingEventTypeLegacyConnectable;
  else if (has_scan_rsp)
    out_params->advertising_event_properties =
        kAdvertisingEventTypeLegacyScannable;
  else
    out_params->advertising_event_properties =
        kAdvertisingEventTypeLegacyNonConnectable;

  out_params->channel_map = kAdvertisingChannelAll;
  out_params->tx_power = GetAdvertisingTxPower(settings.tx_power_level());

  // TODO: expose those new setting through AdvertiseSettings
  out_params->primary_advertising_phy = 0x01;
  out_params->secondary_advertising_phy = 0x01;
  out_params->scan_request_notification_enable = 0;
}

}  // namespace

// LowEnergyAdvertiser implementation
// ========================================================

LowEnergyAdvertiser::LowEnergyAdvertiser(const Uuid& uuid, int advertiser_id)
    : app_identifier_(uuid),
      advertiser_id_(advertiser_id),
      adv_started_(false),
      adv_start_callback_(nullptr),
      adv_stop_callback_(nullptr) {}

LowEnergyAdvertiser::~LowEnergyAdvertiser() {
  // Automatically unregister the advertiser.
  VLOG(1) << "LowEnergyAdvertiser unregistering advertiser: " << advertiser_id_;

  // Stop advertising and ignore the result.
  hal::BluetoothGattInterface::Get()->GetAdvertiserHALInterface()->Enable(
      advertiser_id_, false, base::DoNothing(), 0, 0, base::DoNothing());
  hal::BluetoothGattInterface::Get()->GetAdvertiserHALInterface()->Unregister(
      advertiser_id_);
}

bool LowEnergyAdvertiser::StartAdvertising(const AdvertiseSettings& settings,
                                           const AdvertiseData& advertise_data,
                                           const AdvertiseData& scan_response,
                                           const StatusCallback& callback) {
  VLOG(2) << __func__;
  lock_guard<mutex> lock(adv_fields_lock_);

  if (IsAdvertisingStarted()) {
    LOG(WARNING) << "Already advertising";
    return false;
  }

  if (IsStartingAdvertising()) {
    LOG(WARNING) << "StartAdvertising already pending";
    return false;
  }

  if (!advertise_data.IsValid()) {
    LOG(ERROR) << "Invalid advertising data";
    return false;
  }

  if (!scan_response.IsValid()) {
    LOG(ERROR) << "Invalid scan response data";
    return false;
  }

  advertise_settings_ = settings;

  AdvertiseParameters params;
  GetAdvertiseParams(settings, !scan_response.data().empty(), &params);

  hal::BluetoothGattInterface::Get()
      ->GetAdvertiserHALInterface()
      ->StartAdvertising(
          advertiser_id_,
          base::Bind(&LowEnergyAdvertiser::EnableCallback,
                     base::Unretained(this), true, advertiser_id_),
          params, advertise_data.data(), scan_response.data(),
          settings.timeout().InSeconds(),
          base::Bind(&LowEnergyAdvertiser::EnableCallback,
                     base::Unretained(this), false, advertiser_id_));
  ;

  adv_start_callback_.reset(new StatusCallback(callback));
  return true;
}

bool LowEnergyAdvertiser::StopAdvertising(const StatusCallback& callback) {
  VLOG(2) << __func__;
  lock_guard<mutex> lock(adv_fields_lock_);

  if (!IsAdvertisingStarted()) {
    LOG(ERROR) << "Not advertising";
    return false;
  }

  if (IsStoppingAdvertising()) {
    LOG(ERROR) << "StopAdvertising already pending";
    return false;
  }

  hal::BluetoothGattInterface::Get()->GetAdvertiserHALInterface()->Enable(
      advertiser_id_, false,
      base::Bind(&LowEnergyAdvertiser::EnableCallback, base::Unretained(this),
                 false, advertiser_id_),
      0, 0, base::Bind(&LowEnergyAdvertiser::EnableCallback,
                       base::Unretained(this), false, advertiser_id_));

  // OK to set this at the end since we're still holding |adv_fields_lock_|.
  adv_stop_callback_.reset(new StatusCallback(callback));

  return true;
}

bool LowEnergyAdvertiser::IsAdvertisingStarted() const {
  return adv_started_.load();
}

bool LowEnergyAdvertiser::IsStartingAdvertising() const {
  return !IsAdvertisingStarted() && adv_start_callback_;
}

bool LowEnergyAdvertiser::IsStoppingAdvertising() const {
  return IsAdvertisingStarted() && adv_stop_callback_;
}

const Uuid& LowEnergyAdvertiser::GetAppIdentifier() const {
  return app_identifier_;
}

int LowEnergyAdvertiser::GetInstanceId() const { return advertiser_id_; }

void LowEnergyAdvertiser::EnableCallback(bool enable, uint8_t advertiser_id,
                                         uint8_t status) {
  if (advertiser_id != advertiser_id_) return;

  lock_guard<mutex> lock(adv_fields_lock_);

  VLOG(1) << __func__ << "advertiser_id: " << advertiser_id
          << " status: " << status << " enable: " << enable;

  if (enable) {
    CHECK(adv_start_callback_);
    CHECK(!adv_stop_callback_);

    // Terminate operation in case of error.
    if (status != BT_STATUS_SUCCESS) {
      LOG(ERROR) << "Failed to enable multi-advertising";
      InvokeAndClearStartCallback(GetBLEStatus(status));
      return;
    }

    // All pending tasks are complete. Report success.
    adv_started_ = true;
    InvokeAndClearStartCallback(BLE_STATUS_SUCCESS);

  } else {
    CHECK(!adv_start_callback_);
    CHECK(adv_stop_callback_);

    if (status == BT_STATUS_SUCCESS) {
      VLOG(1) << "Multi-advertising stopped for advertiser_id: "
              << advertiser_id;
      adv_started_ = false;
    } else {
      LOG(ERROR) << "Failed to stop multi-advertising";
    }

    InvokeAndClearStopCallback(GetBLEStatus(status));
  }
}

void LowEnergyAdvertiser::InvokeAndClearStartCallback(BLEStatus status) {
  // We allow NULL callbacks.
  if (*adv_start_callback_) (*adv_start_callback_)(status);

  adv_start_callback_ = nullptr;
}

void LowEnergyAdvertiser::InvokeAndClearStopCallback(BLEStatus status) {
  // We allow NULL callbacks.
  if (*adv_stop_callback_) (*adv_stop_callback_)(status);

  adv_stop_callback_ = nullptr;
}

// LowEnergyAdvertiserFactory implementation
// ========================================================

LowEnergyAdvertiserFactory::LowEnergyAdvertiserFactory() {}

LowEnergyAdvertiserFactory::~LowEnergyAdvertiserFactory() {}

bool LowEnergyAdvertiserFactory::RegisterInstance(
    const Uuid& app_uuid, const RegisterCallback& callback) {
  VLOG(1) << __func__;
  lock_guard<mutex> lock(pending_calls_lock_);

  if (pending_calls_.find(app_uuid) != pending_calls_.end()) {
    LOG(ERROR) << "Low-Energy advertiser with given Uuid already registered - "
               << "Uuid: " << app_uuid.ToString();
    return false;
  }

  BleAdvertiserInterface* hal_iface =
      hal::BluetoothGattInterface::Get()->GetAdvertiserHALInterface();

  VLOG(1) << __func__ << " calling register!";
  hal_iface->RegisterAdvertiser(
      base::Bind(&LowEnergyAdvertiserFactory::RegisterAdvertiserCallback,
                 base::Unretained(this), callback, app_uuid));
  VLOG(1) << __func__ << " call finished!";

  pending_calls_.insert(app_uuid);

  return true;
}

void LowEnergyAdvertiserFactory::RegisterAdvertiserCallback(
    const RegisterCallback& callback, const Uuid& app_uuid,
    uint8_t advertiser_id, uint8_t status) {
  VLOG(1) << __func__;
  lock_guard<mutex> lock(pending_calls_lock_);

  auto iter = pending_calls_.find(app_uuid);
  if (iter == pending_calls_.end()) {
    VLOG(1) << "Ignoring callback for unknown app_id: " << app_uuid.ToString();
    return;
  }

  // No need to construct a advertiser if the call wasn't successful.
  std::unique_ptr<LowEnergyAdvertiser> advertiser;
  BLEStatus result = BLE_STATUS_FAILURE;
  if (status == BT_STATUS_SUCCESS) {
    advertiser.reset(new LowEnergyAdvertiser(app_uuid, advertiser_id));

    result = BLE_STATUS_SUCCESS;
  }

  // Notify the result via the result callback.
  callback(result, app_uuid, std::move(advertiser));

  pending_calls_.erase(iter);
}

}  // namespace bluetooth
