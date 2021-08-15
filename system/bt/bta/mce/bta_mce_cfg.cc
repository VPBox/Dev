/******************************************************************************
 *
 *  Copyright 2014 The Android Open Source Project
 *  Copyright 2003-2012 Broadcom Corporation
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
 *  This file contains compile-time configurable constants for MCE
 *
 ******************************************************************************/

#include "bt_common.h"
#include "bt_types.h"
#include "bta_api.h"
#include "bta_mce_api.h"

#ifndef BTA_MCE_SDP_DB_SIZE
#define BTA_MCE_SDP_DB_SIZE 4500
#endif

static tSDP_DISCOVERY_DB
    bta_mce_sdp_db_data[BTA_MCE_SDP_DB_SIZE / sizeof(tSDP_DISCOVERY_DB)];

/* MCE configuration structure */
const tBTA_MCE_CFG bta_mce_cfg = {
    (BTA_MCE_SDP_DB_SIZE / sizeof(tSDP_DISCOVERY_DB)) *
        sizeof(tSDP_DISCOVERY_DB),
    bta_mce_sdp_db_data /* The data buffer to keep SDP database */
};

const tBTA_MCE_CFG* p_bta_mce_cfg = &bta_mce_cfg;
