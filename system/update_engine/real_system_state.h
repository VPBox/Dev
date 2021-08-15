//
// Copyright (C) 2013 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_REAL_SYSTEM_STATE_H_
#define UPDATE_ENGINE_REAL_SYSTEM_STATE_H_

#include "update_engine/system_state.h"

#include <memory>
#include <set>

#include <policy/device_policy.h>

#if USE_CHROME_KIOSK_APP
#include <kiosk-app/dbus-proxies.h>
#endif  // USE_CHROME_KIOSK_APP

#include "update_engine/certificate_checker.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/clock.h"
#include "update_engine/common/dlcservice_interface.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/prefs.h"
#include "update_engine/connection_manager_interface.h"
#include "update_engine/daemon_state_interface.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/metrics_reporter_omaha.h"
#include "update_engine/p2p_manager.h"
#include "update_engine/payload_state.h"
#include "update_engine/power_manager_interface.h"
#include "update_engine/update_attempter.h"
#include "update_engine/update_manager/update_manager.h"

namespace chromeos_update_engine {

// A real implementation of the SystemStateInterface which is
// used by the actual product code.
class RealSystemState : public SystemState, public DaemonStateInterface {
 public:
  // Constructs all system objects that do not require separate initialization;
  // see Initialize() below for the remaining ones.
  RealSystemState() = default;
  ~RealSystemState() override;

  // Initializes and sets systems objects that require an initialization
  // separately from construction. Returns |true| on success.
  bool Initialize();

  // DaemonStateInterface overrides.
  // Start the periodic update attempts. Must be called at the beginning of the
  // program to start the periodic update check process.
  bool StartUpdater() override;

  void AddObserver(ServiceObserverInterface* observer) override;
  void RemoveObserver(ServiceObserverInterface* observer) override;
  const std::set<ServiceObserverInterface*>& service_observers() override {
    CHECK(update_attempter_.get());
    return update_attempter_->service_observers();
  }

  // SystemState overrides.
  inline void set_device_policy(
      const policy::DevicePolicy* device_policy) override {
    device_policy_ = device_policy;
  }

  inline const policy::DevicePolicy* device_policy() override {
    return device_policy_;
  }

  inline BootControlInterface* boot_control() override {
    return boot_control_.get();
  }

  inline ClockInterface* clock() override { return &clock_; }

  inline ConnectionManagerInterface* connection_manager() override {
    return connection_manager_.get();
  }

  inline HardwareInterface* hardware() override { return hardware_.get(); }

  inline MetricsReporterInterface* metrics_reporter() override {
    return &metrics_reporter_;
  }

  inline PrefsInterface* prefs() override { return prefs_.get(); }

  inline PrefsInterface* powerwash_safe_prefs() override {
    return powerwash_safe_prefs_.get();
  }

  inline PayloadStateInterface* payload_state() override {
    return &payload_state_;
  }

  inline UpdateAttempter* update_attempter() override {
    return update_attempter_.get();
  }

  inline OmahaRequestParams* request_params() override {
    return &request_params_;
  }

  inline P2PManager* p2p_manager() override { return p2p_manager_.get(); }

  inline chromeos_update_manager::UpdateManager* update_manager() override {
    return update_manager_.get();
  }

  inline PowerManagerInterface* power_manager() override {
    return power_manager_.get();
  }

  inline bool system_rebooted() override { return system_rebooted_; }

  inline DlcServiceInterface* dlcservice() override {
    return dlcservice_.get();
  }

 private:
  // Real DBus proxies using the DBus connection.
#if USE_CHROME_KIOSK_APP
  std::unique_ptr<org::chromium::KioskAppServiceInterfaceProxy>
      kiosk_app_proxy_;
#endif  // USE_CHROME_KIOSK_APP

  // Interface for the power manager.
  std::unique_ptr<PowerManagerInterface> power_manager_;

  // Interface for dlcservice.
  std::unique_ptr<DlcServiceInterface> dlcservice_;

  // Interface for the clock.
  std::unique_ptr<BootControlInterface> boot_control_;

  // Interface for the clock.
  Clock clock_;

  // The latest device policy object from the policy provider.
  const policy::DevicePolicy* device_policy_{nullptr};

  // The connection manager object that makes download decisions depending on
  // the current type of connection.
  std::unique_ptr<ConnectionManagerInterface> connection_manager_;

  // Interface for the hardware functions.
  std::unique_ptr<HardwareInterface> hardware_;

  // The Metrics reporter for reporting UMA stats.
  MetricsReporterOmaha metrics_reporter_;

  // Interface for persisted store.
  std::unique_ptr<PrefsInterface> prefs_;

  // Interface for persisted store that persists across powerwashes.
  std::unique_ptr<PrefsInterface> powerwash_safe_prefs_;

  // All state pertaining to payload state such as response, URL, backoff
  // states.
  PayloadState payload_state_;

  // OpenSSLWrapper and CertificateChecker used for checking SSL certificates.
  OpenSSLWrapper openssl_wrapper_;
  std::unique_ptr<CertificateChecker> certificate_checker_;

  // Pointer to the update attempter object.
  std::unique_ptr<UpdateAttempter> update_attempter_;

  // Common parameters for all Omaha requests.
  OmahaRequestParams request_params_{this};

  std::unique_ptr<P2PManager> p2p_manager_;

  std::unique_ptr<chromeos_update_manager::UpdateManager> update_manager_;

  policy::PolicyProvider policy_provider_;

  // If true, this is the first instance of the update engine since the system
  // rebooted. Important for tracking whether you are running instance of the
  // update engine on first boot or due to a crash/restart.
  bool system_rebooted_{false};

  ActionProcessor processor_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_REAL_SYSTEM_STATE_H_
