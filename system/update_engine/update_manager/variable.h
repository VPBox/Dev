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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_VARIABLE_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_VARIABLE_H_

#include <algorithm>
#include <list>
#include <string>

#include <base/bind.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/time/time.h>
#include <brillo/message_loops/message_loop.h>
#include <gtest/gtest_prod.h>  // for FRIEND_TEST

namespace chromeos_update_manager {

// The VariableMode specifies important behavior of the variable in terms of
// whether, how and when the value of the variable changes.
enum VariableMode {
  // Const variables never changes during the life of a policy request, so the
  // EvaluationContext caches the value even between different evaluations of
  // the same policy request.
  kVariableModeConst,

  // Poll variables, or synchronous variables, represent a variable with a value
  // that can be queried at any time, but it is not known when the value
  // changes on the source of information. In order to detect if the value of
  // the variable changes, it has to be queried again.
  kVariableModePoll,

  // Async variables are able to produce a signal or callback whenever the
  // value changes. This means that it's not required to poll the value to
  // detect when it changes, instead, you should register an observer to get
  // a notification when that happens.
  kVariableModeAsync,
};

// This class is a base class with the common functionality that doesn't
// depend on the variable's type, implemented by all the variables.
class BaseVariable {
 public:
  // Interface for observing changes on variable value.
  class ObserverInterface {
   public:
    virtual ~ObserverInterface() {}

    // Called when the value on the variable changes.
    virtual void ValueChanged(BaseVariable* variable) = 0;
  };

  virtual ~BaseVariable() {
    if (!observer_list_.empty()) {
      LOG(WARNING) << "Variable " << name_ << " deleted with "
                   << observer_list_.size() << " observers.";
    }
    DCHECK(observer_list_.empty()) << "Don't destroy the variable without "
                                      "removing the observers.";
  }

  // Returns the variable name as a string.
  const std::string& GetName() const { return name_; }

  // Returns the variable mode.
  VariableMode GetMode() const { return mode_; }

  // For VariableModePoll variables, it returns the polling interval of this
  // variable. In other case, it returns 0.
  base::TimeDelta GetPollInterval() const { return poll_interval_; }

  // Adds and removes observers for value changes on the variable. This only
  // works for kVariableAsync variables since the other modes don't track value
  // changes. Adding the same observer twice has no effect.
  virtual void AddObserver(BaseVariable::ObserverInterface* observer) {
    if (std::find(observer_list_.begin(), observer_list_.end(), observer) ==
        observer_list_.end()) {
      observer_list_.push_back(observer);
    }
  }

  virtual void RemoveObserver(BaseVariable::ObserverInterface* observer) {
    observer_list_.remove(observer);
  }

 protected:
  // Creates a BaseVariable using the default polling interval (5 minutes).
  BaseVariable(const std::string& name, VariableMode mode)
      : BaseVariable(
            name, mode, base::TimeDelta::FromMinutes(kDefaultPollMinutes)) {}

  // Creates a BaseVariable with mode kVariableModePoll and the provided
  // polling interval.
  BaseVariable(const std::string& name, base::TimeDelta poll_interval)
      : BaseVariable(name, kVariableModePoll, poll_interval) {}

  // Reset the poll interval on a polling variable to the given one.
  void SetPollInterval(base::TimeDelta poll_interval) {
    DCHECK_EQ(kVariableModePoll, mode_)
        << "Can't set the poll_interval on a " << mode_ << " variable";
    poll_interval_ = poll_interval;
  }

  // Calls ValueChanged on all the observers.
  void NotifyValueChanged() {
    // Fire all the observer methods from the main loop as single call. In order
    // to avoid scheduling these callbacks when it is not needed, we check
    // first the list of observers.
    if (!observer_list_.empty()) {
      brillo::MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&BaseVariable::OnValueChangedNotification,
                     base::Unretained(this)));
    }
  }

 private:
  friend class UmEvaluationContextTest;
  FRIEND_TEST(UmBaseVariableTest, RepeatedObserverTest);
  FRIEND_TEST(UmBaseVariableTest, NotifyValueChangedTest);
  FRIEND_TEST(UmBaseVariableTest, NotifyValueRemovesObserversTest);

  BaseVariable(const std::string& name,
               VariableMode mode,
               base::TimeDelta poll_interval)
      : name_(name),
        mode_(mode),
        poll_interval_(mode == kVariableModePoll ? poll_interval
                                                 : base::TimeDelta()) {}

  void OnValueChangedNotification() {
    // A ValueChanged() method can change the list of observers, for example
    // removing itself and invalidating the iterator, so we create a snapshot
    // of the observers first. Also, to support the case when *another* observer
    // is removed, we check for them.
    std::list<BaseVariable::ObserverInterface*> observer_list_copy(
        observer_list_);

    for (auto& observer : observer_list_copy) {
      if (std::find(observer_list_.begin(), observer_list_.end(), observer) !=
          observer_list_.end()) {
        observer->ValueChanged(this);
      }
    }
  }

  // The default PollInterval in minutes.
  static constexpr int kDefaultPollMinutes = 5;

  // The variable's name as a string.
  const std::string name_;

  // The variable's mode.
  const VariableMode mode_;

  // The variable's polling interval for VariableModePoll variable and 0 for
  // other modes.
  base::TimeDelta poll_interval_;

  // The list of value changes observers.
  std::list<BaseVariable::ObserverInterface*> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(BaseVariable);
};

// Interface to an Update Manager variable of a given type. Implementation
// internals are hidden as protected members, since policies should not be
// using them directly.
template <typename T>
class Variable : public BaseVariable {
 public:
  ~Variable() override {}

 protected:
  // Only allow to get values through the EvaluationContext class and not
  // directly from the variable.
  friend class EvaluationContext;

  // Needed to be able to verify variable contents during unit testing.
  friend class UmTestUtils;
  FRIEND_TEST(UmRealRandomProviderTest, GetRandomValues);

  Variable(const std::string& name, VariableMode mode)
      : BaseVariable(name, mode) {}

  Variable(const std::string& name, const base::TimeDelta poll_interval)
      : BaseVariable(name, poll_interval) {}

  // Gets the current value of the variable. The current value is copied to a
  // new object and returned. The caller of this method owns the object and
  // should delete it.
  //
  // In case of and error getting the current value or the |timeout| timeout is
  // exceeded, a null value is returned and the |errmsg| is set.
  //
  // The caller can pass a null value for |errmsg|, in which case the error
  // message won't be set.
  virtual const T* GetValue(base::TimeDelta timeout, std::string* errmsg) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Variable);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_VARIABLE_H_
