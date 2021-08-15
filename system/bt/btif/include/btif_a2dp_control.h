/******************************************************************************
 *
 *  Copyright 2016 The Android Open Source Project
 *  Copyright 2009-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef BTIF_A2DP_CONTROL_H
#define BTIF_A2DP_CONTROL_H

#include "audio_a2dp_hw/include/audio_a2dp_hw.h"

// Initialize the A2DP control module. It should be called during the
// startup stage of A2DP streaming.
void btif_a2dp_control_init(void);

// Cleanup the A2DP control module. It should be called during the shutdown
// stage of A2DP streaming.
void btif_a2dp_control_cleanup(void);

// Acknowledge A2DP command to the origin of audio streaming.
// |status| is the acknowledement status - see |tA2DP_CTRL_ACK|.
void btif_a2dp_command_ack(tA2DP_CTRL_ACK status);

// Increment the total number audio data bytes that have been encoded since
// last encoding attempt.
// |bytes_read| is the number of bytes to increment by.
void btif_a2dp_control_log_bytes_read(uint32_t bytes_read);

// Set the audio delay reported to the audio HAL in uints of 1/10ms.
// |delay| is the audio delay to set.
void btif_a2dp_control_set_audio_delay(uint16_t delay);

// Reset the remote audio device's delay value and reset the counter that keeps
// track of the number of audio bytes sent
void btif_a2dp_control_reset_audio_delay(void);

#endif /* BTIF_A2DP_CONTROL_H */
