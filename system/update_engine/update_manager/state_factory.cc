//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/update_manager/state_factory.h"

#include <memory>

#include <base/logging.h>
#if USE_DBUS
#include <session_manager/dbus-proxies.h>
#endif  // USE_DBUS

#include "update_engine/common/clock_interface.h"
#if USE_DBUS
#include "update_engine/dbus_connection.h"
#endif  // USE_DBUS
#include "update_engine/update_manager/fake_shill_provider.h"
#include "update_engine/update_manager/real_config_provider.h"
#include "update_engine/update_manager/real_device_policy_provider.h"
#include "update_engine/update_manager/real_random_provider.h"
#include "update_engine/update_manager/real_state.h"
#include "update_engine/update_manager/real_system_provider.h"
#include "update_engine/update_manager/real_time_provider.h"
#include "update_engine/update_manager/real_updater_provider.h"
#if USE_SHILL
#include "update_engine/shill_proxy.h"
#include "update_engine/update_manager/real_shill_provider.h"
#endif  // USE_SHILL

using std::unique_ptr;

namespace chromeos_update_manager {

State* DefaultStateFactory(
    policy::PolicyProvider* policy_provider,
    org::chromium::KioskAppServiceInterfaceProxyInterface* kiosk_app_proxy,
    chromeos_update_engine::SystemState* system_state) {
  chromeos_update_engine::ClockInterface* const clock = system_state->clock();
  unique_ptr<RealConfigProvider> config_provider(
      new RealConfigProvider(system_state->hardware()));
#if USE_DBUS
  scoped_refptr<dbus::Bus> bus =
      chromeos_update_engine::DBusConnection::Get()->GetDBus();
  unique_ptr<RealDevicePolicyProvider> device_policy_provider(
      new RealDevicePolicyProvider(
          std::make_unique<org::chromium::SessionManagerInterfaceProxy>(bus),
          policy_provider));
#else
  unique_ptr<RealDevicePolicyProvider> device_policy_provider(
      new RealDevicePolicyProvider(policy_provider));
#endif  // USE_DBUS
#if USE_SHILL
  unique_ptr<RealShillProvider> shill_provider(
      new RealShillProvider(new chromeos_update_engine::ShillProxy(), clock));
#else
  unique_ptr<FakeShillProvider> shill_provider(new FakeShillProvider());
#endif  // USE_SHILL
  unique_ptr<RealRandomProvider> random_provider(new RealRandomProvider());
  unique_ptr<RealSystemProvider> system_provider(new RealSystemProvider(
      system_state->hardware(), system_state->boot_control(), kiosk_app_proxy));

  unique_ptr<RealTimeProvider> time_provider(new RealTimeProvider(clock));
  unique_ptr<RealUpdaterProvider> updater_provider(
      new RealUpdaterProvider(system_state));

  if (!(config_provider->Init() && device_policy_provider->Init() &&
        random_provider->Init() &&
#if USE_SHILL
        shill_provider->Init() &&
#endif  // USE_SHILL
        system_provider->Init() && time_provider->Init() &&
        updater_provider->Init())) {
    LOG(ERROR) << "Error initializing providers";
    return nullptr;
  }

  return new RealState(config_provider.release(),
                       device_policy_provider.release(),
                       random_provider.release(),
                       shill_provider.release(),
                       system_provider.release(),
                       time_provider.release(),
                       updater_provider.release());
}

}  // namespace chromeos_update_manager
