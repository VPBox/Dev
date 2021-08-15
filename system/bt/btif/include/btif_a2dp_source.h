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

#ifndef BTIF_A2DP_SOURCE_H
#define BTIF_A2DP_SOURCE_H

#include <stdbool.h>
#include <future>

#include "bta_av_api.h"

// Initialize the A2DP Source module.
// This function should be called by the BTIF state machine prior to using the
// module.
bool btif_a2dp_source_init(void);

// Startup the A2DP Source module.
// This function should be called by the BTIF state machine after
// btif_a2dp_source_init() to prepare to start streaming.
bool btif_a2dp_source_startup(void);

// Start the A2DP Source session.
// This function should be called by the BTIF state machine after
// btif_a2dp_source_startup() to start the streaming session for |peer_address|.
bool btif_a2dp_source_start_session(const RawAddress& peer_address,
                                    std::promise<void> peer_ready_promise);

// Restart the A2DP Source session.
// This function should be called by the BTIF state machine after
// btif_a2dp_source_startup() to restart the streaming session.
// |old_peer_address| is the peer address of the old session. This address
// can be empty.
// |new_peer_address| is the peer address of the new session. This address
// cannot be empty.
bool btif_a2dp_source_restart_session(const RawAddress& old_peer_address,
                                      const RawAddress& new_peer_address,
                                      std::promise<void> peer_ready_promise);

// End the A2DP Source session.
// This function should be called by the BTIF state machine to end the
// streaming session for |peer_address|.
bool btif_a2dp_source_end_session(const RawAddress& peer_address);

// Shutdown the A2DP Source module.
// This function should be called by the BTIF state machine to stop streaming.
void btif_a2dp_source_shutdown(void);

// Cleanup the A2DP Source module.
// This function should be called by the BTIF state machine during graceful
// cleanup.
void btif_a2dp_source_cleanup(void);

// Check whether the A2DP Source media task is running.
// Returns true if the A2DP Source media task is running, otherwise false.
bool btif_a2dp_source_media_task_is_running(void);

// Check whether the A2DP Source media task is shutting down.
// Returns true if the A2DP Source media task is shutting down.
bool btif_a2dp_source_media_task_is_shutting_down(void);

// Return true if the A2DP Source module is streaming.
bool btif_a2dp_source_is_streaming(void);

// Process a request to start the A2DP audio encoding task.
void btif_a2dp_source_start_audio_req(void);

// Process a request to stop the A2DP audio encoding task.
void btif_a2dp_source_stop_audio_req(void);

// Process a request to update the A2DP audio encoder with user preferred
// codec configuration.
// The peer address is |peer_addr|.
// |codec_user_config| contains the preferred codec user configuration.
void btif_a2dp_source_encoder_user_config_update_req(
    const RawAddress& peer_addr,
    const btav_a2dp_codec_config_t& codec_user_config);

// Process a request to update the A2DP audio encoding with new audio
// configuration feeding parameters stored in |codec_audio_config|.
// The fields that are used are: |codec_audio_config.sample_rate|,
// |codec_audio_config.bits_per_sample| and |codec_audio_config.channel_mode|.
void btif_a2dp_source_feeding_update_req(
    const btav_a2dp_codec_config_t& codec_audio_config);

// Process 'idle' request from the BTIF state machine during initialization.
void btif_a2dp_source_on_idle(void);

// Process 'stop' request from the BTIF state machine to stop A2DP streaming.
// |p_av_suspend| is the data associated with the request - see
// |tBTA_AV_SUSPEND|.
void btif_a2dp_source_on_stopped(tBTA_AV_SUSPEND* p_av_suspend);

// Process 'suspend' request from the BTIF state machine to suspend A2DP
// streaming.
// |p_av_suspend| is the data associated with the request - see
// |tBTA_AV_SUSPEND|.
void btif_a2dp_source_on_suspended(tBTA_AV_SUSPEND* p_av_suspend);

// Enable/disable discarding of transmitted frames.
// If |enable| is true, the discarding is enabled, otherwise is disabled.
void btif_a2dp_source_set_tx_flush(bool enable);

// Get the next A2DP buffer to send.
// Returns the next A2DP buffer to send if available, otherwise NULL.
BT_HDR* btif_a2dp_source_audio_readbuf(void);

// Dump debug-related information for the A2DP Source module.
// |fd| is the file descriptor to use for writing the ASCII formatted
// information.
void btif_a2dp_source_debug_dump(int fd);

#endif /* BTIF_A2DP_SOURCE_H */
