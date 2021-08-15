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

#include "update_engine/update_manager/real_system_provider.h"

#include <base/bind.h>
#include <base/callback.h>
#include <base/logging.h>
#include <base/time/time.h>
#if USE_CHROME_KIOSK_APP
#include <kiosk-app/dbus-proxies.h>
#endif  // USE_CHROME_KIOSK_APP

#include "update_engine/common/utils.h"
#include "update_engine/update_manager/generic_variables.h"
#include "update_engine/update_manager/variable.h"

using std::string;

namespace chromeos_update_manager {

namespace {

// The maximum number of consecutive failures before returning the default
// constructor value for T instead of failure.
const int kRetryPollVariableMaxRetry = 5;

// The polling interval to be used whenever GetValue() returns an error.
const int kRetryPollVariableRetryIntervalSeconds = 5 * 60;

// The RetryPollVariable variable is a polling variable that allows the function
// returning the value to fail a few times and shortens the polling rate when
// that happens.
template <typename T>
class RetryPollVariable : public Variable<T> {
 public:
  RetryPollVariable(const string& name,
                    const base::TimeDelta poll_interval,
                    base::Callback<bool(T* res)> func)
      : Variable<T>(name, poll_interval),
        func_(func),
        base_interval_(poll_interval) {
    DCHECK_LT(kRetryPollVariableRetryIntervalSeconds,
              base_interval_.InSeconds());
  }

 protected:
  // Variable override.
  const T* GetValue(base::TimeDelta /* timeout */,
                    string* /* errmsg */) override {
    std::unique_ptr<T> result(new T());
    if (!func_.Run(result.get())) {
      if (failed_attempts_ >= kRetryPollVariableMaxRetry) {
        // Give up on the retries, set back the desired polling interval and
        // return the default.
        this->SetPollInterval(base_interval_);
        return result.release();
      }
      this->SetPollInterval(
          base::TimeDelta::FromSeconds(kRetryPollVariableRetryIntervalSeconds));
      failed_attempts_++;
      return nullptr;
    }
    failed_attempts_ = 0;
    this->SetPollInterval(base_interval_);
    return result.release();
  }

 private:
  // The function to be called, stored as a base::Callback.
  base::Callback<bool(T*)> func_;

  // The desired polling interval when |func_| works and returns true.
  base::TimeDelta base_interval_;

  // The number of consecutive failed attempts made.
  int failed_attempts_ = 0;

  DISALLOW_COPY_AND_ASSIGN(RetryPollVariable);
};

}  // namespace

bool RealSystemProvider::Init() {
  var_is_normal_boot_mode_.reset(new ConstCopyVariable<bool>(
      "is_normal_boot_mode", hardware_->IsNormalBootMode()));

  var_is_official_build_.reset(new ConstCopyVariable<bool>(
      "is_official_build", hardware_->IsOfficialBuild()));

  var_is_oobe_complete_.reset(new CallCopyVariable<bool>(
      "is_oobe_complete",
      base::Bind(&chromeos_update_engine::HardwareInterface::IsOOBEComplete,
                 base::Unretained(hardware_),
                 nullptr)));

  var_num_slots_.reset(new ConstCopyVariable<unsigned int>(
      "num_slots", boot_control_->GetNumSlots()));

  var_kiosk_required_platform_version_.reset(new RetryPollVariable<string>(
      "kiosk_required_platform_version",
      base::TimeDelta::FromHours(5),  // Same as Chrome's CWS poll.
      base::Bind(&RealSystemProvider::GetKioskAppRequiredPlatformVersion,
                 base::Unretained(this))));

  return true;
}

bool RealSystemProvider::GetKioskAppRequiredPlatformVersion(
    string* required_platform_version) {
#if USE_CHROME_KIOSK_APP
  brillo::ErrorPtr error;
  if (!kiosk_app_proxy_->GetRequiredPlatformVersion(required_platform_version,
                                                    &error)) {
    LOG(WARNING) << "Failed to get kiosk required platform version";
    required_platform_version->clear();
    return false;
  }
#endif  // USE_CHROME_KIOSK_APP

  return true;
}

}  // namespace chromeos_update_manager
