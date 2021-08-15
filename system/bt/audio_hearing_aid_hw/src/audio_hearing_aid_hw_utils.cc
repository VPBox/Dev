/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include "audio_hearing_aid_hw.h"

#define CASE_RETURN_STR(const) \
  case const:                  \
    return #const;

const char* audio_ha_hw_dump_ctrl_event(tHEARING_AID_CTRL_CMD event) {
  switch (event) {
    CASE_RETURN_STR(HEARING_AID_CTRL_CMD_NONE)
    CASE_RETURN_STR(HEARING_AID_CTRL_CMD_CHECK_READY)
    CASE_RETURN_STR(HEARING_AID_CTRL_CMD_START)
    CASE_RETURN_STR(HEARING_AID_CTRL_CMD_STOP)
    CASE_RETURN_STR(HEARING_AID_CTRL_CMD_SUSPEND)
    CASE_RETURN_STR(HEARING_AID_CTRL_GET_INPUT_AUDIO_CONFIG)
    CASE_RETURN_STR(HEARING_AID_CTRL_GET_OUTPUT_AUDIO_CONFIG)
    CASE_RETURN_STR(HEARING_AID_CTRL_SET_OUTPUT_AUDIO_CONFIG)
    CASE_RETURN_STR(HEARING_AID_CTRL_CMD_OFFLOAD_START)
    default:
      break;
  }

  return "UNKNOWN HEARING_AID_CTRL_CMD";
}
