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

#ifndef UPDATE_ENGINE_DBUS_TEST_UTILS_H_
#define UPDATE_ENGINE_DBUS_TEST_UTILS_H_

#include <set>
#include <string>

#include <base/bind.h>
#include <brillo/message_loops/message_loop.h>
#include <gmock/gmock.h>

namespace chromeos_update_engine {
namespace dbus_test_utils {

#define MOCK_SIGNAL_HANDLER_EXPECT_SIGNAL_HANDLER(                           \
    mock_signal_handler, mock_proxy, signal)                                 \
  do {                                                                       \
    EXPECT_CALL((mock_proxy),                                                \
                Register##signal##SignalHandler(::testing::_, ::testing::_)) \
        .WillOnce(::chromeos_update_engine::dbus_test_utils::GrabCallbacks(  \
            &(mock_signal_handler)));                                        \
  } while (false)

template <typename T>
class MockSignalHandler {
 public:
  MockSignalHandler() = default;
  ~MockSignalHandler() {
    if (callback_connected_task_ != brillo::MessageLoop::kTaskIdNull)
      brillo::MessageLoop::current()->CancelTask(callback_connected_task_);
  }

  // Returns whether the signal handler is registered.
  bool IsHandlerRegistered() const { return signal_callback_ != nullptr; }

  const base::Callback<T>& signal_callback() { return *signal_callback_.get(); }

  void GrabCallbacks(
      const base::Callback<T>& signal_callback,
      dbus::ObjectProxy::OnConnectedCallback on_connected_callback) {
    signal_callback_.reset(new base::Callback<T>(signal_callback));
    on_connected_callback_.reset(
        new dbus::ObjectProxy::OnConnectedCallback(on_connected_callback));
    // Notify from the main loop that the callback was connected.
    callback_connected_task_ = brillo::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&MockSignalHandler<T>::OnCallbackConnected,
                   base::Unretained(this)));
  }

 private:
  void OnCallbackConnected() {
    callback_connected_task_ = brillo::MessageLoop::kTaskIdNull;
    on_connected_callback_->Run("", "", true);
  }

  brillo::MessageLoop::TaskId callback_connected_task_{
      brillo::MessageLoop::kTaskIdNull};

  std::unique_ptr<base::Callback<T>> signal_callback_;
  std::unique_ptr<dbus::ObjectProxy::OnConnectedCallback>
      on_connected_callback_;
};

// Defines the action that will call MockSignalHandler<T>::GrabCallbacks for the
// right type.
ACTION_P(GrabCallbacks, mock_signal_handler) {
  mock_signal_handler->GrabCallbacks(arg0, arg1);
}

}  // namespace dbus_test_utils
}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_DBUS_TEST_UTILS_H_
