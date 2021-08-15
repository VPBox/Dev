/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_HOST_LINK_BASE_H_
#define CHRE_PLATFORM_SLPI_HOST_LINK_BASE_H_

#include "timer.h"

namespace chre {

class HostLinkBase {
 public:
  /**
   * Blocks the current thread until the host has retrieved all messages pending
   * in the outbound queue, or a timeout occurs. For proper function, it should
   * not be possible for new messages to be added to the queue at the point when
   * this is called.
   *
   * @return true if the outbound queue was successfully emptied
   */
  static bool flushOutboundQueue();

  /**
   * Attempts to flush the outbound queue and gracefully inform the host that we
   * are exiting.
   */
  static void shutdown();

 private:
  static constexpr time_timetick_type kPollingIntervalUsec = 5000;
};

/**
 * Requests that the HostLink send the log buffer to the host.
 */
void requestHostLinkLogBufferFlush();

/**
 * Sends a request to the host to enable the audio feature.
 */
void sendAudioRequest();

/**
 * Sends a request to the host to disable the audio feature.
 */
void sendAudioRelease();

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_HOST_LINK_BASE_H_
