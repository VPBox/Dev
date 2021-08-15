//
// Copyright (C) 2009 The Android Open Source Project
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

#include "update_engine/common/action_processor.h"

#include <string>
#include <utility>

#include <base/logging.h>

#include "update_engine/common/action.h"
#include "update_engine/common/error_code_utils.h"

using std::string;
using std::unique_ptr;

namespace chromeos_update_engine {

ActionProcessor::~ActionProcessor() {
  if (IsRunning())
    StopProcessing();
}

void ActionProcessor::EnqueueAction(unique_ptr<AbstractAction> action) {
  action->SetProcessor(this);
  actions_.push_back(std::move(action));
}

bool ActionProcessor::IsRunning() const {
  return current_action_ != nullptr || suspended_;
}

void ActionProcessor::StartProcessing() {
  CHECK(!IsRunning());
  if (!actions_.empty()) {
    current_action_ = std::move(actions_.front());
    actions_.pop_front();
    LOG(INFO) << "ActionProcessor: starting " << current_action_->Type();
    current_action_->PerformAction();
  }
}

void ActionProcessor::StopProcessing() {
  CHECK(IsRunning());
  if (current_action_) {
    current_action_->TerminateProcessing();
  }
  LOG(INFO) << "ActionProcessor: aborted "
            << (current_action_ ? current_action_->Type() : "")
            << (suspended_ ? " while suspended" : "");
  current_action_.reset();
  suspended_ = false;
  // Delete all the actions before calling the delegate.
  actions_.clear();
  if (delegate_)
    delegate_->ProcessingStopped(this);
}

void ActionProcessor::SuspendProcessing() {
  // No current_action_ when not suspended means that the action processor was
  // never started or already finished.
  if (suspended_ || !current_action_) {
    LOG(WARNING) << "Called SuspendProcessing while not processing.";
    return;
  }
  suspended_ = true;

  // If there's a current action we should notify it that it should suspend, but
  // the action can ignore that and terminate at any point.
  LOG(INFO) << "ActionProcessor: suspending " << current_action_->Type();
  current_action_->SuspendAction();
}

void ActionProcessor::ResumeProcessing() {
  if (!suspended_) {
    LOG(WARNING) << "Called ResumeProcessing while not suspended.";
    return;
  }
  suspended_ = false;
  if (current_action_) {
    // The current_action_ did not call ActionComplete while suspended, so we
    // should notify it of the resume operation.
    LOG(INFO) << "ActionProcessor: resuming " << current_action_->Type();
    current_action_->ResumeAction();
  } else {
    // The last action called ActionComplete while suspended, so there is
    // already a log message with the type of the finished action. We simply
    // state that we are resuming processing and the next function will log the
    // start of the next action or processing completion.
    LOG(INFO) << "ActionProcessor: resuming processing";
    StartNextActionOrFinish(suspended_error_code_);
  }
}

void ActionProcessor::ActionComplete(AbstractAction* actionptr,
                                     ErrorCode code) {
  CHECK_EQ(actionptr, current_action_.get());
  if (delegate_)
    delegate_->ActionCompleted(this, actionptr, code);
  string old_type = current_action_->Type();
  current_action_->ActionCompleted(code);
  current_action_.reset();
  LOG(INFO) << "ActionProcessor: finished "
            << (actions_.empty() ? "last action " : "") << old_type
            << (suspended_ ? " while suspended" : "") << " with code "
            << utils::ErrorCodeToString(code);
  if (!actions_.empty() && code != ErrorCode::kSuccess) {
    LOG(INFO) << "ActionProcessor: Aborting processing due to failure.";
    actions_.clear();
  }
  if (suspended_) {
    // If an action finished while suspended we don't start the next action (or
    // terminate the processing) until the processor is resumed. This condition
    // will be flagged by a nullptr current_action_ while suspended_ is true.
    suspended_error_code_ = code;
    return;
  }
  StartNextActionOrFinish(code);
}

void ActionProcessor::StartNextActionOrFinish(ErrorCode code) {
  if (actions_.empty()) {
    if (delegate_) {
      delegate_->ProcessingDone(this, code);
    }
    return;
  }
  current_action_ = std::move(actions_.front());
  actions_.pop_front();
  LOG(INFO) << "ActionProcessor: starting " << current_action_->Type();
  current_action_->PerformAction();
}

}  // namespace chromeos_update_engine
