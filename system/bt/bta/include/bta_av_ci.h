/******************************************************************************
 *
 *  Copyright 2005-2012 Broadcom Corporation
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

/******************************************************************************
 *
 *  This is the interface file for advanced audio/video call-in functions.
 *
 ******************************************************************************/
#ifndef BTA_AV_CI_H
#define BTA_AV_CI_H

#include "bta_av_api.h"

/*****************************************************************************
 *  Function Declarations
 ****************************************************************************/
/*******************************************************************************
 *
 * Function         bta_av_ci_src_data_ready
 *
 * Description      This function sends an event to the AV indicating that
 *                  the phone has audio stream data ready to send and AV
 *                  should call bta_av_co_audio_source_data_path().
 *
 * Returns          void
 *
 ******************************************************************************/
extern void bta_av_ci_src_data_ready(tBTA_AV_CHNL chnl);

/*******************************************************************************
 *
 * Function         bta_av_ci_setconfig
 *
 * Description      This function must be called in response to function
 *                  bta_av_co_audio_setconfig().
 *                  Parameter err_code is set to an AVDTP status value;
 *                  AVDT_SUCCESS if the codec configuration is ok,
 *                  otherwise error.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void bta_av_ci_setconfig(tBTA_AV_HNDL bta_av_handle, uint8_t err_code,
                                uint8_t category, uint8_t num_seid,
                                uint8_t* p_seid, bool recfg_needed,
                                uint8_t avdt_handle);

#endif /* BTA_AV_CI_H */
