//
// Copyright (C) 2018 The Android Open Source Project
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

#include <string>

#include "update_engine/common/action.h"
#include "update_engine/common/boot_control_interface.h"

#include <gtest/gtest_prod.h>

namespace chromeos_update_engine {

class UpdateBootFlagsAction : public AbstractAction {
 public:
  explicit UpdateBootFlagsAction(BootControlInterface* boot_control)
      : boot_control_(boot_control) {}

  void PerformAction() override;

  void TerminateProcessing() override;

  static std::string StaticType() { return "UpdateBootFlagsAction"; }
  std::string Type() const override { return StaticType(); }

  void CompleteUpdateBootFlags(bool successful);

 private:
  FRIEND_TEST(UpdateBootFlagsActionTest, SimpleTest);
  FRIEND_TEST(UpdateBootFlagsActionTest, DoubleActionTest);

  // Originally, both of these flags are false. Once UpdateBootFlags is called,
  // |is_running_| is set to true. As soon as UpdateBootFlags completes its
  // asynchronous run, |is_running_| is reset to false and |updated_boot_flags_|
  // is set to true. From that point on there will be no more changes to these
  // flags.
  //
  // True if have updated the boot flags.
  static bool updated_boot_flags_;
  // True if we are still updating the boot flags.
  static bool is_running_;

  // Used for setting the boot flag.
  BootControlInterface* boot_control_;

  DISALLOW_COPY_AND_ASSIGN(UpdateBootFlagsAction);
};

}  // namespace chromeos_update_engine
