//
// Copyright (C) 2015 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_HARDWARE_ANDROID_H_
#define UPDATE_ENGINE_HARDWARE_ANDROID_H_

#include <string>

#include <base/macros.h>
#include <base/time/time.h>

#include "update_engine/common/hardware.h"
#include "update_engine/common/hardware_interface.h"

namespace chromeos_update_engine {

// Implements the real interface with the hardware in the Android platform.
class HardwareAndroid final : public HardwareInterface {
 public:
  HardwareAndroid() = default;
  ~HardwareAndroid() override = default;

  // HardwareInterface methods.
  bool IsOfficialBuild() const override;
  bool IsNormalBootMode() const override;
  bool AreDevFeaturesEnabled() const override;
  bool IsOOBEEnabled() const override;
  bool IsOOBEComplete(base::Time* out_time_of_oobe) const override;
  std::string GetHardwareClass() const override;
  std::string GetFirmwareVersion() const override;
  std::string GetECVersion() const override;
  int GetMinKernelKeyVersion() const override;
  int GetMinFirmwareKeyVersion() const override;
  int GetMaxFirmwareKeyRollforward() const override;
  bool SetMaxFirmwareKeyRollforward(int firmware_max_rollforward) override;
  bool SetMaxKernelKeyRollforward(int kernel_max_rollforward) override;
  int GetPowerwashCount() const override;
  bool SchedulePowerwash(bool is_rollback) override;
  bool CancelPowerwash() override;
  bool GetNonVolatileDirectory(base::FilePath* path) const override;
  bool GetPowerwashSafeDirectory(base::FilePath* path) const override;
  int64_t GetBuildTimestamp() const override;
  bool GetFirstActiveOmahaPingSent() const override;
  bool SetFirstActiveOmahaPingSent() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(HardwareAndroid);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_HARDWARE_ANDROID_H_
