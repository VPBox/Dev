/******************************************************************************
 *
 *  Copyright 2016 The Android Open Source Project
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

#ifndef BTIF_HD_H
#define BTIF_HD_H

#include <hardware/bluetooth.h>
#include <hardware/bt_hd.h>
#include <stdint.h>
#include "bta_hd_api.h"

typedef enum {
  BTIF_HD_DISABLED = 0,
  BTIF_HD_ENABLED,
  BTIF_HD_DISABLING
} BTIF_HD_STATUS;

/* BTIF-HD control block */
typedef struct {
  BTIF_HD_STATUS status;
  bool app_registered;
  bool service_dereg_active;
  bool forced_disc;
} btif_hd_cb_t;

extern btif_hd_cb_t btif_hd_cb;

extern void btif_hd_remove_device(RawAddress bd_addr);
extern void btif_hd_service_registration();

#endif
