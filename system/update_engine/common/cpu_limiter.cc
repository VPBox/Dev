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

#include "update_engine/common/cpu_limiter.h"

#include <string>

#include <base/bind.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/time/time.h>

#include "update_engine/common/utils.h"

namespace {

// Cgroup container is created in update-engine's upstart script located at
// /etc/init/update-engine.conf.
const char kCGroupSharesPath[] = "/sys/fs/cgroup/cpu/update-engine/cpu.shares";

}  // namespace

namespace chromeos_update_engine {

CPULimiter::~CPULimiter() {
  // Set everything back to normal on destruction.
  CPULimiter::SetCpuShares(CpuShares::kNormal);
}

void CPULimiter::StartLimiter() {
  if (manage_shares_id_ != brillo::MessageLoop::kTaskIdNull) {
    LOG(ERROR) << "Cpu shares timeout source hasn't been destroyed.";
    StopLimiter();
  }
  manage_shares_id_ = brillo::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&CPULimiter::StopLimiterCallback, base::Unretained(this)),
      base::TimeDelta::FromHours(2));
  SetCpuShares(CpuShares::kLow);
}

void CPULimiter::StopLimiter() {
  if (manage_shares_id_ != brillo::MessageLoop::kTaskIdNull) {
    // If the shares were never set and there isn't a message loop instance,
    // we avoid calling CancelTask(), which otherwise would have been a no-op.
    brillo::MessageLoop::current()->CancelTask(manage_shares_id_);
    manage_shares_id_ = brillo::MessageLoop::kTaskIdNull;
  }
  SetCpuShares(CpuShares::kNormal);
}

bool CPULimiter::SetCpuShares(CpuShares shares) {
  // Short-circuit to avoid re-setting the shares.
  if (shares_ == shares)
    return true;

  std::string string_shares = base::IntToString(static_cast<int>(shares));
  LOG(INFO) << "Setting cgroup cpu shares to  " << string_shares;
  if (!utils::WriteFile(
          kCGroupSharesPath, string_shares.c_str(), string_shares.size())) {
    LOG(ERROR) << "Failed to change cgroup cpu shares to " << string_shares
               << " using " << kCGroupSharesPath;
    return false;
  }
  shares_ = shares;
  LOG(INFO) << "CPU shares = " << static_cast<int>(shares_);
  return true;
}

void CPULimiter::StopLimiterCallback() {
  SetCpuShares(CpuShares::kNormal);
  manage_shares_id_ = brillo::MessageLoop::kTaskIdNull;
}

}  // namespace chromeos_update_engine
