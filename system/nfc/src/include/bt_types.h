/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
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

#ifndef BT_TYPES_H
#define BT_TYPES_H

#include "data_types.h"
#include "nfc_types.h"

/* NCI Command, Notification or Data*/
#define BT_EVT_TO_NFC_NCI 0x4000
/* messages between NFC and NCI task */
#define BT_EVT_TO_NFC_MSGS 0x4300

/* start timer */
#define BT_EVT_TO_START_TIMER 0x3c00

/* start quick timer */
#define BT_EVT_TO_START_QUICK_TIMER 0x3e00
#endif
