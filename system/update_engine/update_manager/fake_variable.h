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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_VARIABLE_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_VARIABLE_H_

#include <memory>
#include <string>

#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// A fake typed variable to use while testing policy implementations. The
// variable can be instructed to return any object of its type.
template <typename T>
class FakeVariable : public Variable<T> {
 public:
  FakeVariable(const std::string& name, VariableMode mode)
      : Variable<T>(name, mode) {}
  FakeVariable(const std::string& name, base::TimeDelta poll_interval)
      : Variable<T>(name, poll_interval) {}
  ~FakeVariable() override {}

  // Sets the next value of this variable to the passed |p_value| pointer. Once
  // returned by GetValue(), the pointer is released and has to be set again.
  // A value of null means that the GetValue() call will fail and return
  // null.
  void reset(const T* p_value) { ptr_.reset(p_value); }

  // Make the NotifyValueChanged() public for FakeVariables.
  void NotifyValueChanged() { Variable<T>::NotifyValueChanged(); }

 protected:
  // Variable<T> overrides.
  // Returns the pointer set with reset(). The ownership of the object is passed
  // to the caller and the pointer is release from the FakeVariable. A second
  // call to GetValue() without reset() will return null and set the error
  // message.
  const T* GetValue(base::TimeDelta /* timeout */,
                    std::string* errmsg) override {
    if (ptr_ == nullptr && errmsg != nullptr)
      *errmsg = this->GetName() + " is an empty FakeVariable";
    // Passes the pointer ownership to the caller.
    return ptr_.release();
  }

 private:
  // The pointer returned by GetValue().
  std::unique_ptr<const T> ptr_;

  DISALLOW_COPY_AND_ASSIGN(FakeVariable);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_VARIABLE_H_
