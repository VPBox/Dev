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
// A2DP constants for low complexity subband codec (SBC)
//

#ifndef A2DP_SBC_CONSTANTS_H
#define A2DP_SBC_CONSTANTS_H

/*****************************************************************************
 *  Constants
 ****************************************************************************/
/* the length of the SBC Media Payload header. */
#define A2DP_SBC_MPL_HDR_LEN 1

/* the LOSC of SBC media codec capabilitiy */
#define A2DP_SBC_INFO_LEN 6

/* for Codec Specific Information Element */
#define A2DP_SBC_IE_SAMP_FREQ_MSK 0xF0 /* b7-b4 sampling frequency */
#define A2DP_SBC_IE_SAMP_FREQ_16 0x80  /* b7:16  kHz */
#define A2DP_SBC_IE_SAMP_FREQ_32 0x40  /* b6:32  kHz */
#define A2DP_SBC_IE_SAMP_FREQ_44 0x20  /* b5:44.1kHz */
#define A2DP_SBC_IE_SAMP_FREQ_48 0x10  /* b4:48  kHz */

#define A2DP_SBC_IE_CH_MD_MSK 0x0F    /* b3-b0 channel mode */
#define A2DP_SBC_IE_CH_MD_MONO 0x08   /* b3: mono */
#define A2DP_SBC_IE_CH_MD_DUAL 0x04   /* b2: dual */
#define A2DP_SBC_IE_CH_MD_STEREO 0x02 /* b1: stereo */
#define A2DP_SBC_IE_CH_MD_JOINT 0x01  /* b0: joint stereo */

#define A2DP_SBC_IE_BLOCKS_MSK 0xF0 /* b7-b4 number of blocks */
#define A2DP_SBC_IE_BLOCKS_4 0x80   /* 4 blocks */
#define A2DP_SBC_IE_BLOCKS_8 0x40   /* 8 blocks */
#define A2DP_SBC_IE_BLOCKS_12 0x20  /* 12blocks */
#define A2DP_SBC_IE_BLOCKS_16 0x10  /* 16blocks */

#define A2DP_SBC_IE_SUBBAND_MSK 0x0C /* b3-b2 number of subbands */
#define A2DP_SBC_IE_SUBBAND_4 0x08   /* b3: 4 */
#define A2DP_SBC_IE_SUBBAND_8 0x04   /* b2: 8 */

#define A2DP_SBC_IE_ALLOC_MD_MSK 0x03 /* b1-b0 allocation mode */
#define A2DP_SBC_IE_ALLOC_MD_S 0x02   /* b1: SNR */
#define A2DP_SBC_IE_ALLOC_MD_L 0x01   /* b0: loundess */

#define A2DP_SBC_IE_MIN_BITPOOL 2
#define A2DP_SBC_IE_MAX_BITPOOL 250

/* for media payload header */
#define A2DP_SBC_HDR_F_MSK 0x80
#define A2DP_SBC_HDR_S_MSK 0x40
#define A2DP_SBC_HDR_L_MSK 0x20
#define A2DP_SBC_HDR_NUM_MSK 0x0F

#endif  // A2DP_SBC_CONSTANTS_H
