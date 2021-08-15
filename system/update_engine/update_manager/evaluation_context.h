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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_EVALUATION_CONTEXT_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_EVALUATION_CONTEXT_H_

#include <map>
#include <memory>
#include <string>

#include <base/bind.h>
#include <base/callback.h>
#include <base/memory/ref_counted.h>
#include <base/memory/weak_ptr.h>
#include <base/time/time.h>
#include <brillo/message_loops/message_loop.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/update_manager/boxed_value.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// The EvaluationContext class is the interface between a policy implementation
// and the state. The EvaluationContext tracks the variables used by a policy
// request and caches the returned values, owning those cached values.
// The same EvaluationContext should be re-used for all the evaluations of the
// same policy request (an AsyncPolicyRequest might involve several
// re-evaluations). Each evaluation of the EvaluationContext is run at a given
// point in time, which is used as a reference for the evaluation timeout and
// the time based queries of the policy, such as
// Is{Wallclock,Monotonic}TimeGreaterThan().
//
// Example:
//
//   scoped_refptr<EvaluationContext> ec = new EvaluationContext(...);
//
//   ...
//   // The following call to ResetEvaluation() is optional. Use it to reset the
//   // evaluation time if the EvaluationContext isn't used right after its
//   // construction.
//   ec->ResetEvaluation();
//   EvalStatus status = policy->SomeMethod(ec, state, &result, args...);
//
//   ...
//   // Run a closure when any of the used async variables changes its value or
//   // the timeout for re-query the values happens again.
//   ec->RunOnValueChangeOrTimeout(closure);
//   // If the provided |closure| wants to re-evaluate the policy, it should
//   // call ec->ResetEvaluation() to start a new evaluation.
//
class EvaluationContext : public base::RefCounted<EvaluationContext>,
                          private BaseVariable::ObserverInterface {
 public:
  EvaluationContext(
      chromeos_update_engine::ClockInterface* clock,
      base::TimeDelta evaluation_timeout,
      base::TimeDelta expiration_timeout,
      std::unique_ptr<base::Callback<void(EvaluationContext*)>> unregister_cb);
  EvaluationContext(chromeos_update_engine::ClockInterface* clock,
                    base::TimeDelta evaluation_timeout)
      : EvaluationContext(
            clock,
            evaluation_timeout,
            base::TimeDelta::Max(),
            std::unique_ptr<base::Callback<void(EvaluationContext*)>>()) {}
  ~EvaluationContext();

  // Returns a pointer to the value returned by the passed variable |var|. The
  // EvaluationContext instance keeps the ownership of the returned object. The
  // returned object is valid during the life of the evaluation, even if the
  // passed Variable changes it.
  //
  // In case of error, a null value is returned.
  template <typename T>
  const T* GetValue(Variable<T>* var);

  // Returns whether the evaluation time has surpassed |timestamp|, on either
  // the ClockInterface::GetWallclockTime() or
  // ClockInterface::GetMonotonicTime() scales, respectively.
  bool IsWallclockTimeGreaterThan(base::Time timestamp);
  bool IsMonotonicTimeGreaterThan(base::Time timestamp);

  // Returns whether the evaluation context has expired.
  bool is_expired() const { return is_expired_; }

  // TODO(deymo): Move the following methods to an interface only visible by the
  // UpdateManager class and not the policy implementations.

  // Resets the EvaluationContext to its initial state removing all the
  // non-const cached variables and re-setting the evaluation time. This should
  // be called right before any new evaluation starts.
  void ResetEvaluation();

  // Clears the expiration status of the EvaluationContext and resets its
  // expiration timeout based on |expiration_timeout_|. This should be called if
  // expiration occurred, prior to re-evaluating the policy.
  void ResetExpiration();

  // Schedules the passed |callback| closure to be called when a cached
  // variable changes its value, a polling interval passes, or the context
  // expiration occurs. If none of these events can happen, for example if
  // there's no cached variable, this method returns false.
  //
  // Right before the passed closure is called the EvaluationContext is
  // reset, removing all the non-const cached values.
  bool RunOnValueChangeOrTimeout(base::Closure callback);

  // Returns a textual representation of the evaluation context,
  // including the variables and their values. This is intended only
  // to help with debugging and the format may change in the future.
  std::string DumpContext() const;

  // Removes all the Observers callbacks and timeout events scheduled by
  // RunOnValueChangeOrTimeout(). Also releases and returns the closure
  // associated with these events. This method is idempotent.
  std::unique_ptr<base::Closure> RemoveObserversAndTimeout();

 private:
  friend class UmEvaluationContextTest;

  // BaseVariable::ObserverInterface override.
  void ValueChanged(BaseVariable* var) override;

  // Called from the main loop when a scheduled timeout has passed.
  void OnTimeout();

  // Removes the observers from the used Variables and cancels the timeout,
  // then executes the scheduled callback.
  void OnValueChangedOrTimeout();

  // If |monotonic_deadline| is not Time::Max(), returns the remaining time
  // until it is reached, or zero if it has passed. Otherwise, returns
  // TimeDelta::Max().
  base::TimeDelta RemainingTime(base::Time monotonic_deadline) const;

  // Returns a monotonic clock timestamp at which |timeout| will have elapsed
  // since the current time.
  base::Time MonotonicDeadline(base::TimeDelta timeout);

  // A map to hold the cached values for every variable.
  typedef std::map<BaseVariable*, BoxedValue> ValueCacheMap;

  // The cached values of the called Variables.
  ValueCacheMap value_cache_;

  // A callback used for triggering re-evaluation upon a value change or poll
  // timeout, or notifying about the evaluation context expiration. It is up to
  // the caller to determine whether or not expiration occurred via
  // is_expired().
  std::unique_ptr<base::Closure> callback_;

  // The TaskId returned by the message loop identifying the timeout callback.
  // Used for canceling the timeout callback.
  brillo::MessageLoop::TaskId timeout_event_ = brillo::MessageLoop::kTaskIdNull;

  // Whether a timeout event firing marks the expiration of the evaluation
  // context.
  bool timeout_marks_expiration_;

  // Whether the evaluation context has indeed expired.
  bool is_expired_ = false;

  // Pointer to the mockable clock interface;
  chromeos_update_engine::ClockInterface* const clock_;

  // The timestamps when the evaluation of this EvaluationContext started,
  // corresponding to ClockInterface::GetWallclockTime() and
  // ClockInterface::GetMonotonicTime(), respectively. These values are reset
  // every time ResetEvaluation() is called.
  base::Time evaluation_start_wallclock_;
  base::Time evaluation_start_monotonic_;

  // The timestamps when a reevaluation should be triggered due to various
  // expected value changes, corresponding to ClockInterface::GetWallclockTime()
  // and ClockInterface::GetMonotonicTIme(), respectively. These timestamps are
  // greater or equal to corresponding |evaluation_start_{wallclock,monotonic}_|
  // counterparts since they are in the future; however, they may be smaller
  // than the current corresponding times during the course of evaluation.
  base::Time reevaluation_time_wallclock_;
  base::Time reevaluation_time_monotonic_;

  // The timeout of an evaluation.
  const base::TimeDelta evaluation_timeout_;

  // The timestamp in the ClockInterface::GetMonotonicTime() scale at which the
  // current evaluation should finish.
  base::Time evaluation_monotonic_deadline_;

  // The expiration timeout of the evaluation context.
  const base::TimeDelta expiration_timeout_;

  // The monotonic clock deadline at which expiration occurs.
  base::Time expiration_monotonic_deadline_;

  // A callback for unregistering the context upon destruction.
  std::unique_ptr<base::Callback<void(EvaluationContext*)>> unregister_cb_;

  base::WeakPtrFactory<EvaluationContext> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(EvaluationContext);
};

}  // namespace chromeos_update_manager

// Include the implementation of the template methods.
#include "update_engine/update_manager/evaluation_context-inl.h"

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_EVALUATION_CONTEXT_H_
