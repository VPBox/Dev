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
// A2DP API
//

#ifndef A2DP_API_H
#define A2DP_API_H

#include <inttypes.h>

#include "a2dp_constants.h"
#include "a2dp_error_codes.h"
#include "sdp_api.h"

/*****************************************************************************
 *  constants
 ****************************************************************************/

//
// |MAX_PCM_FRAME_NUM_PER_TICK| controls how many buffers we can hold in
// the A2DP buffer queues during temporary link congestion.
//
#ifndef MAX_PCM_FRAME_NUM_PER_TICK
#define MAX_PCM_FRAME_NUM_PER_TICK 14
#endif

/* the return values from A2DP_BitsSet() */
#define A2DP_SET_ONE_BIT 1   /* one and only one bit is set */
#define A2DP_SET_ZERO_BIT 0  /* all bits clear */
#define A2DP_SET_MULTL_BIT 2 /* multiple bits are set */

/*****************************************************************************
 *  type definitions
 ****************************************************************************/

/* This data type is used in A2DP_FindService() to initialize the SDP database
 * to hold the result service search. */
typedef struct {
  uint32_t db_len;   /* Length, in bytes, of the discovery database */
  uint16_t num_attr; /* The number of attributes in p_attrs */
  uint16_t* p_attrs; /* The attributes filter. If NULL, A2DP API sets the
                      * attribute filter
                      * to be ATTR_ID_SERVICE_CLASS_ID_LIST,
                      * ATTR_ID_BT_PROFILE_DESC_LIST,
                      * ATTR_ID_SUPPORTED_FEATURES, ATTR_ID_SERVICE_NAME and
                      * ATTR_ID_PROVIDER_NAME.
                      * If not NULL, the input is taken as the filter. */
} tA2DP_SDP_DB_PARAMS;

/* This data type is used in tA2DP_FIND_CBACK to report the result of the SDP
 * discovery process. */
typedef struct {
  uint16_t service_len;  /* Length, in bytes, of the service name */
  uint16_t provider_len; /* Length, in bytes, of the provider name */
  char* p_service_name;  /* Pointer the service name.  This character string may
                          * not be null terminated.
                          * Use the service_len parameter to safely copy this
                          * string */
  char* p_provider_name; /* Pointer the provider name.  This character string
                          * may not be null terminated.
                          * Use the provider_len parameter to safely copy this
                          * string */
  uint16_t features;     /* Profile supported features */
  uint16_t avdt_version; /* AVDTP protocol version */
} tA2DP_Service;

/* This is the callback to notify the result of the SDP discovery process. */
typedef void(tA2DP_FIND_CBACK)(bool found, tA2DP_Service* p_service,
                               const RawAddress& peer_address);

/*****************************************************************************
 *  external function declarations
 ****************************************************************************/
/******************************************************************************
 *
 * Function         A2DP_AddRecord
 *
 * Description      This function is called by a server application to add
 *                  SRC or SNK information to an SDP record.  Prior to
 *                  calling this function the application must call
 *                  SDP_CreateRecord() to create an SDP record.
 *
 *                  Input Parameters:
 *                      service_uuid:  Indicates SRC or SNK.
 *
 *                      p_service_name:  Pointer to a null-terminated character
 *                      string containing the service name.
 *
 *                      p_provider_name:  Pointer to a null-terminated character
 *                      string containing the provider name.
 *
 *                      features:  Profile supported features.
 *
 *                      sdp_handle:  SDP handle returned by SDP_CreateRecord().
 *
 *                  Output Parameters:
 *                      None.
 *
 * Returns          A2DP_SUCCESS if function execution succeeded,
 *                  A2DP_INVALID_PARAMS if bad parameters are given.
 *                  A2DP_FAIL if function execution failed.
 *
 *****************************************************************************/
extern tA2DP_STATUS A2DP_AddRecord(uint16_t service_uuid, char* p_service_name,
                                   char* p_provider_name, uint16_t features,
                                   uint32_t sdp_handle);

/******************************************************************************
 *
 * Function         A2DP_FindService
 *
 * Description      This function is called by a client application to
 *                  perform service discovery and retrieve SRC or SNK SDP
 *                  record information from a server.  Information is
 *                  returned for the first service record found on the
 *                  server that matches the service UUID.  The callback
 *                  function will be executed when service discovery is
 *                  complete.  There can only be one outstanding call to
 *                  A2DP_FindService() at a time; the application must wait
 *                  for the callback before it makes another call to
 *                  the function.
 *
 *                  Input Parameters:
 *                      service_uuid:  Indicates SRC or SNK.
 *
 *                      bd_addr:  BD address of the peer device.
 *
 *                      p_db:  Pointer to the information to initialize
 *                             the discovery database.
 *
 *                      p_cback:  Pointer to the A2DP_FindService()
 *                      callback function.
 *
 *                  Output Parameters:
 *                      None.
 *
 * Returns          A2DP_SUCCESS if function execution succeeded,
 *                  A2DP_INVALID_PARAMS if bad parameters are given.
 *                  A2DP_BUSY if discovery is already in progress.
 *                  A2DP_FAIL if function execution failed.
 *
 *****************************************************************************/
extern tA2DP_STATUS A2DP_FindService(uint16_t service_uuid,
                                     const RawAddress& bd_addr,
                                     tA2DP_SDP_DB_PARAMS* p_db,
                                     tA2DP_FIND_CBACK* p_cback);

/******************************************************************************
 *
 * Function         A2DP_GetAvdtpVersion()
 *
 * Description      Gets the local version of AVDTP
 *
 * Returns          The local version of AVDTP.
 *
 *****************************************************************************/
extern uint16_t A2DP_GetAvdtpVersion(void);

/******************************************************************************
 *
 * Function         A2DP_SetTraceLevel
 *
 * Description      Sets the trace level for A2D. If 0xff is passed, the
 *                  current trace level is returned.
 *
 *                  Input Parameters:
 *                      new_level:  The level to set the A2DP tracing to:
 *                      0xff-returns the current setting.
 *                      0-turns off tracing.
 *                      >= 1-Errors.
 *                      >= 2-Warnings.
 *                      >= 3-APIs.
 *                      >= 4-Events.
 *                      >= 5-Debug.
 *
 * Returns          The new trace level or current trace level if
 *                  the input parameter is 0xff.
 *
 *****************************************************************************/
extern uint8_t A2DP_SetTraceLevel(uint8_t new_level);

/******************************************************************************
 * Function         A2DP_BitsSet
 *
 * Description      Check the given num for the number of bits set
 * Returns          A2DP_SET_ONE_BIT, if one and only one bit is set
 *                  A2DP_SET_ZERO_BIT, if all bits clear
 *                  A2DP_SET_MULTL_BIT, if multiple bits are set
 *****************************************************************************/
extern uint8_t A2DP_BitsSet(uint64_t num);

// Initializes the A2DP control block.
void A2DP_Init(void);

#endif  // A2DP_API_H
