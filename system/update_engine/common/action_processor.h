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

#ifndef UPDATE_ENGINE_COMMON_ACTION_PROCESSOR_H_
#define UPDATE_ENGINE_COMMON_ACTION_PROCESSOR_H_

#include <deque>
#include <memory>
#include <vector>

#include <base/macros.h>
#include <brillo/errors/error.h>

#include "update_engine/common/error_code.h"

#include <gtest/gtest_prod.h>

// The structure of these classes (Action, ActionPipe, ActionProcessor, etc.)
// is based on the KSAction* classes from the Google Update Engine code at
// http://code.google.com/p/update-engine/ . The author of this file sends
// a big thanks to that team for their high quality design, implementation,
// and documentation.

// See action.h for an overview of this class and other Action* classes.

// An ActionProcessor keeps a queue of Actions and processes them in order.

namespace chromeos_update_engine {

class AbstractAction;
class ActionProcessorDelegate;

class ActionProcessor {
 public:
  ActionProcessor() = default;

  virtual ~ActionProcessor();

  // Starts processing the first Action in the queue. If there's a delegate,
  // when all processing is complete, ProcessingDone() will be called on the
  // delegate.
  virtual void StartProcessing();

  // Aborts processing. If an Action is running, it will have
  // TerminateProcessing() called on it. The Action that was running and all the
  // remaining actions will be lost and must be re-enqueued if this Processor is
  // to use it.
  void StopProcessing();

  // Suspend the processing. If an Action is running, it will have the
  // SuspendProcessing() called on it, and it should suspend operations until
  // ResumeProcessing() is called on this class to continue. While suspended,
  // no new actions will be started. Calling SuspendProcessing while the
  // processing is suspended or not running this method performs no action.
  void SuspendProcessing();

  // Resume the suspended processing. If the ActionProcessor is not suspended
  // or not running in the first place this method performs no action.
  void ResumeProcessing();

  // Returns true iff the processing was started but not yet completed nor
  // stopped.
  bool IsRunning() const;

  // Adds another Action to the end of the queue.
  virtual void EnqueueAction(std::unique_ptr<AbstractAction> action);

  // Sets/gets the current delegate. Set to null to remove a delegate.
  ActionProcessorDelegate* delegate() const { return delegate_; }
  void set_delegate(ActionProcessorDelegate* delegate) { delegate_ = delegate; }

  // Returns a pointer to the current Action that's processing.
  AbstractAction* current_action() const { return current_action_.get(); }

  // Called by an action to notify processor that it's done. Caller passes self.
  // But this call deletes the action if there no other object has a reference
  // to it, so in that case, the caller should not try to access any of its
  // member variables after this call.
  void ActionComplete(AbstractAction* actionptr, ErrorCode code);

 private:
  FRIEND_TEST(ActionProcessorTest, ChainActionsTest);

  // Continue processing actions (if any) after the last action terminated with
  // the passed error code. If there are no more actions to process, the
  // processing will terminate.
  void StartNextActionOrFinish(ErrorCode code);

  // Actions that have not yet begun processing, in the order in which
  // they'll be processed.
  std::deque<std::unique_ptr<AbstractAction>> actions_;

  // A pointer to the currently processing Action, if any.
  std::unique_ptr<AbstractAction> current_action_;

  // The ErrorCode reported by an action that was suspended but finished while
  // being suspended. This error code is stored here to be reported back to the
  // delegate once the processor is resumed.
  ErrorCode suspended_error_code_{ErrorCode::kSuccess};

  // Whether the action processor is or should be suspended.
  bool suspended_{false};

  // A pointer to the delegate, or null if none.
  ActionProcessorDelegate* delegate_{nullptr};

  DISALLOW_COPY_AND_ASSIGN(ActionProcessor);
};

// A delegate object can be used to be notified of events that happen
// in an ActionProcessor. An instance of this class can be passed to an
// ActionProcessor to register itself.
class ActionProcessorDelegate {
 public:
  virtual ~ActionProcessorDelegate() = default;

  // Called when all processing in an ActionProcessor has completed. A pointer
  // to the ActionProcessor is passed. |code| is set to the exit code of the
  // last completed action.
  virtual void ProcessingDone(const ActionProcessor* processor,
                              ErrorCode code) {}

  // Called when processing has stopped. Does not mean that all Actions have
  // completed. If/when all Actions complete, ProcessingDone() will be called.
  virtual void ProcessingStopped(const ActionProcessor* processor) {}

  // Called whenever an action has finished processing, either successfully
  // or otherwise.
  virtual void ActionCompleted(ActionProcessor* processor,
                               AbstractAction* action,
                               ErrorCode code) {}
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_ACTION_PROCESSOR_H_
