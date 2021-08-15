/******************************************************************************
 *
 *  Copyright 2002-2012 Broadcom Corporation
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
 *  2DP internal header file
 *
 ******************************************************************************/
#ifndef A2DP_INT_H
#define A2DP_INT_H

#include "a2dp_api.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/
#define A2DP_VERSION 0x0103

/* Number of attributes in A2DP SDP record. */
#define A2DP_NUM_ATTR 6

/* Number of protocol elements in protocol element list. */
#define A2DP_NUM_PROTO_ELEMS 2

/*****************************************************************************
 *  Type definitions
 ****************************************************************************/

/* Control block used by A2DP_FindService(). */
typedef struct {
  tA2DP_FIND_CBACK* p_cback; /* pointer to application callback */
  tSDP_DISCOVERY_DB* p_db;   /* pointer to discovery database */
  uint16_t service_uuid;     /* service UUID of search */
} tA2DP_FIND_CB;

typedef struct {
  tA2DP_FIND_CB find; /* find service control block */
  uint8_t trace_level;
  uint16_t avdt_sdp_ver; /* AVDTP version */
} tA2DP_CB;

/******************************************************************************
 * Main Control Block
 ******************************************************************************/
extern tA2DP_CB a2dp_cb;

/* Used only for conformance testing */
extern void a2dp_set_avdt_sdp_ver(uint16_t avdt_sdp_ver);

#endif /* A2DP_INT_H */
