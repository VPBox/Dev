/******************************************************************************
 *
 *  Copyright (C) 2016 The Android Open Source Project
 *  Copyright (C) 2009-2012 Broadcom Corporation
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

#ifndef BTIF_A2DP_AUDIO_INTERFACE_H
#define BTIF_A2DP_AUDIO_INTERFACE_H

#include "bta_av_api.h"

/*Default LDAC bitrate */
#define DEFAULT_LDAC_BITRATE_48KHZ 660000
#define DEFAULT_LDAC_BITRATE_441KHZ 606000

void btif_a2dp_audio_on_started(tBTA_AV_STATUS status);
void btif_a2dp_audio_on_stopped(tBTA_AV_STATUS status);
void btif_a2dp_audio_on_suspended(tBTA_AV_STATUS status);
void btif_a2dp_audio_interface_start_session(void);
void btif_a2dp_audio_interface_end_session(void);

#endif /* BTIF_A2DP_AUDIO_INTERFACE_H */
