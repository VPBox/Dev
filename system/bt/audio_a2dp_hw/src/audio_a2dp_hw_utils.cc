/******************************************************************************
 *
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

#include "audio_a2dp_hw.h"
#include "osi/include/properties.h"

#define CASE_RETURN_STR(const) \
  case const:                  \
    return #const;

const char* audio_a2dp_hw_dump_ctrl_event(tA2DP_CTRL_CMD event) {
  switch (event) {
    CASE_RETURN_STR(A2DP_CTRL_CMD_NONE)
    CASE_RETURN_STR(A2DP_CTRL_CMD_CHECK_READY)
    CASE_RETURN_STR(A2DP_CTRL_CMD_START)
    CASE_RETURN_STR(A2DP_CTRL_CMD_STOP)
    CASE_RETURN_STR(A2DP_CTRL_CMD_SUSPEND)
    CASE_RETURN_STR(A2DP_CTRL_GET_INPUT_AUDIO_CONFIG)
    CASE_RETURN_STR(A2DP_CTRL_GET_OUTPUT_AUDIO_CONFIG)
    CASE_RETURN_STR(A2DP_CTRL_SET_OUTPUT_AUDIO_CONFIG)
    CASE_RETURN_STR(A2DP_CTRL_CMD_OFFLOAD_START)
    CASE_RETURN_STR(A2DP_CTRL_GET_PRESENTATION_POSITION)
  }

  return "UNKNOWN A2DP_CTRL_CMD";
}

bool delay_reporting_enabled() {
  return !osi_property_get_bool("persist.bluetooth.disabledelayreports", false);
}
