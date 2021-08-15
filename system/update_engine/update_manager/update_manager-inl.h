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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_UPDATE_MANAGER_INL_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_UPDATE_MANAGER_INL_H_

#include <memory>
#include <string>

#include <base/bind.h>
#include <base/location.h>
#include <brillo/message_loops/message_loop.h>

#include "update_engine/update_manager/evaluation_context.h"

namespace chromeos_update_manager {

template <typename R, typename... Args>
EvalStatus UpdateManager::EvaluatePolicy(
    EvaluationContext* ec,
    EvalStatus (Policy::*policy_method)(
        EvaluationContext*, State*, std::string*, R*, Args...) const,
    R* result,
    Args... args) {
  // If expiration timeout fired, dump the context and reset expiration.
  // IMPORTANT: We must still proceed with evaluation of the policy in this
  // case, so that the evaluation time (and corresponding reevaluation timeouts)
  // are readjusted.
  if (ec->is_expired()) {
    LOG(WARNING) << "Request timed out, evaluation context: "
                 << ec->DumpContext();
    ec->ResetExpiration();
  }

  // Reset the evaluation context.
  ec->ResetEvaluation();

  const std::string policy_name = policy_->PolicyRequestName(policy_method);
  LOG(INFO) << policy_name << ": START";

  // First try calling the actual policy.
  std::string error;
  EvalStatus status = (policy_.get()->*policy_method)(
      ec, state_.get(), &error, result, args...);
  // If evaluating the main policy failed, defer to the default policy.
  if (status == EvalStatus::kFailed) {
    LOG(WARNING) << "Evaluating policy failed: " << error
                 << "\nEvaluation context: " << ec->DumpContext();
    error.clear();
    status = (default_policy_.*policy_method)(
        ec, state_.get(), &error, result, args...);
    if (status == EvalStatus::kFailed) {
      LOG(WARNING) << "Evaluating default policy failed: " << error;
    } else if (status == EvalStatus::kAskMeAgainLater) {
      LOG(ERROR)
          << "Default policy would block; this is a bug, forcing failure.";
      status = EvalStatus::kFailed;
    }
  }

  LOG(INFO) << policy_name << ": END";

  return status;
}

template <typename R, typename... Args>
void UpdateManager::OnPolicyReadyToEvaluate(
    scoped_refptr<EvaluationContext> ec,
    base::Callback<void(EvalStatus status, const R& result)> callback,
    EvalStatus (Policy::*policy_method)(
        EvaluationContext*, State*, std::string*, R*, Args...) const,
    Args... args) {
  // Evaluate the policy.
  R result;
  EvalStatus status = EvaluatePolicy(ec.get(), policy_method, &result, args...);

  if (status != EvalStatus::kAskMeAgainLater) {
    // AsyncPolicyRequest finished.
    callback.Run(status, result);
    return;
  }

  // Re-schedule the policy request based on used variables.
  base::Closure reeval_callback =
      base::Bind(&UpdateManager::OnPolicyReadyToEvaluate<R, Args...>,
                 base::Unretained(this),
                 ec,
                 callback,
                 policy_method,
                 args...);
  if (ec->RunOnValueChangeOrTimeout(reeval_callback))
    return;  // Reevaluation scheduled successfully.

  // Scheduling a reevaluation can fail because policy method didn't use any
  // non-const variable nor there's any time-based event that will change the
  // status of evaluation.  Alternatively, this may indicate an error in the use
  // of the scheduling interface.
  LOG(ERROR) << "Failed to schedule a reevaluation of policy "
             << policy_->PolicyRequestName(policy_method) << "; this is a bug.";
  callback.Run(status, result);
}

template <typename R, typename... ActualArgs, typename... ExpectedArgs>
EvalStatus UpdateManager::PolicyRequest(
    EvalStatus (Policy::*policy_method)(
        EvaluationContext*, State*, std::string*, R*, ExpectedArgs...) const,
    R* result,
    ActualArgs... args) {
  scoped_refptr<EvaluationContext> ec(
      new EvaluationContext(clock_, evaluation_timeout_));
  // A PolicyRequest always consists on a single evaluation on a new
  // EvaluationContext.
  // IMPORTANT: To ensure that ActualArgs can be converted to ExpectedArgs, we
  // explicitly instantiate EvaluatePolicy with the latter in lieu of the
  // former.
  EvalStatus ret = EvaluatePolicy<R, ExpectedArgs...>(
      ec.get(), policy_method, result, args...);
  // Sync policy requests must not block, if they do then this is an error.
  DCHECK(EvalStatus::kAskMeAgainLater != ret);
  LOG_IF(WARNING, EvalStatus::kAskMeAgainLater == ret)
      << "Sync request used with an async policy; this is a bug";
  return ret;
}

template <typename R, typename... ActualArgs, typename... ExpectedArgs>
void UpdateManager::AsyncPolicyRequest(
    base::Callback<void(EvalStatus, const R& result)> callback,
    EvalStatus (Policy::*policy_method)(
        EvaluationContext*, State*, std::string*, R*, ExpectedArgs...) const,
    ActualArgs... args) {
  scoped_refptr<EvaluationContext> ec = new EvaluationContext(
      clock_,
      evaluation_timeout_,
      expiration_timeout_,
      std::unique_ptr<base::Callback<void(EvaluationContext*)>>(
          new base::Callback<void(EvaluationContext*)>(
              base::Bind(&UpdateManager::UnregisterEvalContext,
                         weak_ptr_factory_.GetWeakPtr()))));
  if (!ec_repo_.insert(ec.get()).second) {
    LOG(ERROR) << "Failed to register evaluation context; this is a bug.";
  }

  // IMPORTANT: To ensure that ActualArgs can be converted to ExpectedArgs, we
  // explicitly instantiate UpdateManager::OnPolicyReadyToEvaluate with the
  // latter in lieu of the former.
  base::Closure eval_callback =
      base::Bind(&UpdateManager::OnPolicyReadyToEvaluate<R, ExpectedArgs...>,
                 base::Unretained(this),
                 ec,
                 callback,
                 policy_method,
                 args...);
  brillo::MessageLoop::current()->PostTask(FROM_HERE, eval_callback);
}

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_UPDATE_MANAGER_INL_H_
