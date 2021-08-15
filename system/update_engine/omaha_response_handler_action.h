//
// Copyright (C) 2011 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_OMAHA_RESPONSE_HANDLER_ACTION_H_
#define UPDATE_ENGINE_OMAHA_RESPONSE_HANDLER_ACTION_H_

#include <string>

#include <gtest/gtest_prod.h>  // for FRIEND_TEST

#include "update_engine/common/action.h"
#include "update_engine/omaha_request_action.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/system_state.h"

// This class reads in an Omaha response and converts what it sees into
// an install plan which is passed out.

namespace chromeos_update_engine {

class OmahaResponseHandlerAction;

template <>
class ActionTraits<OmahaResponseHandlerAction> {
 public:
  typedef OmahaResponse InputObjectType;
  typedef InstallPlan OutputObjectType;
};

class OmahaResponseHandlerAction : public Action<OmahaResponseHandlerAction> {
 public:
  explicit OmahaResponseHandlerAction(SystemState* system_state);

  typedef ActionTraits<OmahaResponseHandlerAction>::InputObjectType
      InputObjectType;
  typedef ActionTraits<OmahaResponseHandlerAction>::OutputObjectType
      OutputObjectType;
  void PerformAction() override;

  // This is a synchronous action, and thus TerminateProcessing() should
  // never be called
  void TerminateProcessing() override { CHECK(false); }

  const InstallPlan& install_plan() const { return install_plan_; }

  // Debugging/logging
  static std::string StaticType() { return "OmahaResponseHandlerAction"; }
  std::string Type() const override { return StaticType(); }

 private:
  // Returns true if payload hash checks are mandatory based on the state
  // of the system and the contents of the Omaha response. False otherwise.
  bool AreHashChecksMandatory(const OmahaResponse& response);

  // Global system context.
  SystemState* system_state_;

  // The install plan, if we have an update.
  InstallPlan install_plan_;

  // File used for communication deadline to Chrome.
  std::string deadline_file_;

  friend class OmahaResponseHandlerActionTest;
  friend class OmahaResponseHandlerActionProcessorDelegate;
  FRIEND_TEST(UpdateAttempterTest, CreatePendingErrorEventResumedTest);
  FRIEND_TEST(UpdateAttempterTest, RollbackMetricsNotRollbackFailure);
  FRIEND_TEST(UpdateAttempterTest, RollbackMetricsNotRollbackSuccess);
  FRIEND_TEST(UpdateAttempterTest, RollbackMetricsRollbackFailure);
  FRIEND_TEST(UpdateAttempterTest, RollbackMetricsRollbackSuccess);
  FRIEND_TEST(UpdateAttempterTest, SetRollbackHappenedNotRollback);
  FRIEND_TEST(UpdateAttempterTest, SetRollbackHappenedRollback);
  FRIEND_TEST(UpdateAttempterTest, UpdateDeferredByPolicyTest);

  DISALLOW_COPY_AND_ASSIGN(OmahaResponseHandlerAction);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_OMAHA_RESPONSE_HANDLER_ACTION_H_
