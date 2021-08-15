/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Interface to the A2DP LDAC ABR
//

#ifndef A2DP_VENDOR_LDAC_ABR_H
#define A2DP_VENDOR_LDAC_ABR_H

#include <stdio.h>

#include <ldacBT_abr.h>

// Initial EQMID for ABR mode.
#define LDAC_ABR_MODE_EQMID LDACBT_EQMID_SQ

// Loads the LDAC ABR library.
// Return true on success, otherwise false.
bool A2DP_VendorLoadLdacAbr(void);

// Unloads the LDAC ABR library.
void A2DP_VendorUnloadLdacAbr(void);

// Gets the LDAC ABR handle.
// Return the LDAC ABR handle.
HANDLE_LDAC_ABR a2dp_ldac_abr_get_handle(void);

// Free the LDAC ABR handle.
void a2dp_ldac_abr_free_handle(HANDLE_LDAC_ABR hLdacAbr);

// Initializes the LDAC ABR handle.
// Return 0 on success, or -1 on failure.
int a2dp_ldac_abr_init(HANDLE_LDAC_ABR hLdacAbr, unsigned int interval_ms);

// Sets thresholds for the LDAC ABR handle.
// Return 0 on success, or -1 on failure.
int a2dp_ldac_abr_set_thresholds(HANDLE_LDAC_ABR hLdacAbr,
                                 unsigned int th_critical,
                                 unsigned int th_dangerous_trend,
                                 unsigned int th_4hqsq);

// LDAC ABR main process.
// It controls LDAC encoder bit rate based on A2DP transmit queue length.
// return current EQMID of LDAC encoder.
int a2dp_ldac_abr_proc(HANDLE_LDAC_BT hLdacParam, HANDLE_LDAC_ABR hLdacAbr,
                       size_t transmit_queue_length, unsigned int flag_enable);

#endif  // A2DP_VENDOR_LDAC_ENCODER_H
