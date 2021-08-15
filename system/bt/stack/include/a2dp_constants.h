/******************************************************************************
 *
 *  Copyright 2000-2012 Broadcom Corporation
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

//
// A2DP constants defined by the Profile Specification
//

#ifndef A2DP_CONSTANTS_H
#define A2DP_CONSTANTS_H

#include <inttypes.h>

/* Profile supported features */
#define A2DP_SUPF_PLAYER 0x0001
#define A2DP_SUPF_MIC 0x0002
#define A2DP_SUPF_TUNER 0x0004
#define A2DP_SUPF_MIXER 0x0008

#define A2DP_SUPF_HEADPHONE 0x0001
#define A2DP_SUPF_SPEAKER 0x0002
#define A2DP_SUPF_RECORDER 0x0004
#define A2DP_SUPF_AMP 0x0008

/* AV Media Codec Types (Audio Codec ID) */
#define A2DP_MEDIA_CT_SBC 0x00 /* SBC media codec type */
#define A2DP_MEDIA_CT_AAC 0x02 /* AAC media codec type */
/* Non-A2DP media codec type (vendor-specific codec) */
#define A2DP_MEDIA_CT_NON_A2DP 0xFF

typedef uint8_t tA2DP_CODEC_TYPE; /* A2DP Codec type: A2DP_MEDIA_CT_* */

#endif  // A2DP_CONSTANTS_H
