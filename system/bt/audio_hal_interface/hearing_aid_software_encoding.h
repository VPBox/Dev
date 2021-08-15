/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <functional>
#include "common/message_loop_thread.h"

namespace bluetooth {
namespace audio {
namespace hearing_aid {

struct StreamCallbacks {
  std::function<bool(bool start_media_task)> on_resume_;
  std::function<bool(void)> on_suspend_;
};

// Check if new bluetooth_audio is enabled
bool is_hal_2_0_enabled();

// Initialize BluetoothAudio HAL: openProvider
bool init(StreamCallbacks stream_cb,
          bluetooth::common::MessageLoopThread* message_loop);

// Clean up BluetoothAudio HAL
void cleanup();

// Send command to the BluetoothAudio HAL: StartSession, EndSession
void start_session();
void end_session();

void set_remote_delay(uint16_t delay_report_ms);

// Read from the FMQ of BluetoothAudio HAL
size_t read(uint8_t* p_buf, uint32_t len);

}  // namespace hearing_aid
}  // namespace audio
}  // namespace bluetooth
