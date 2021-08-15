//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/daemon_state_android.h"

#include <base/logging.h>

#include "update_engine/common/boot_control.h"
#include "update_engine/common/boot_control_stub.h"
#include "update_engine/common/hardware.h"
#include "update_engine/common/prefs.h"
#include "update_engine/update_attempter_android.h"

namespace chromeos_update_engine {

bool DaemonStateAndroid::Initialize() {
  boot_control_ = boot_control::CreateBootControl();
  if (!boot_control_) {
    LOG(WARNING) << "Unable to create BootControl instance, using stub "
                 << "instead. All update attempts will fail.";
    boot_control_.reset(new BootControlStub());
  }

  hardware_ = hardware::CreateHardware();
  if (!hardware_) {
    LOG(ERROR) << "Error initializing the HardwareInterface.";
    return false;
  }

  LOG_IF(INFO, !hardware_->IsNormalBootMode()) << "Booted in dev mode.";
  LOG_IF(INFO, !hardware_->IsOfficialBuild()) << "Booted non-official build.";

  // Initialize prefs.
  base::FilePath non_volatile_path;
  // TODO(deymo): Fall back to in-memory prefs if there's no physical directory
  // available.
  if (!hardware_->GetNonVolatileDirectory(&non_volatile_path)) {
    LOG(ERROR) << "Failed to get a non-volatile directory.";
    return false;
  }
  Prefs* prefs = new Prefs();
  prefs_.reset(prefs);
  if (!prefs->Init(non_volatile_path.Append(kPrefsSubDirectory))) {
    LOG(ERROR) << "Failed to initialize preferences.";
    return false;
  }

  // The CertificateChecker singleton is used by the update attempter.
  certificate_checker_.reset(
      new CertificateChecker(prefs_.get(), &openssl_wrapper_));
  certificate_checker_->Init();

  // Initialize the UpdateAttempter before the UpdateManager.
  update_attempter_.reset(new UpdateAttempterAndroid(
      this, prefs_.get(), boot_control_.get(), hardware_.get()));

  return true;
}

bool DaemonStateAndroid::StartUpdater() {
  // The DaemonState in Android is a passive daemon. It will only start applying
  // an update when instructed to do so from the exposed binder API.
  update_attempter_->Init();
  return true;
}

void DaemonStateAndroid::AddObserver(ServiceObserverInterface* observer) {
  service_observers_.insert(observer);
}

void DaemonStateAndroid::RemoveObserver(ServiceObserverInterface* observer) {
  service_observers_.erase(observer);
}

ServiceDelegateAndroidInterface* DaemonStateAndroid::service_delegate() {
  return update_attempter_.get();
}

}  // namespace chromeos_update_engine
