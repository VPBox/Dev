/******************************************************************************
 *
 *  Copyright (C) 2012-2014 Broadcom Corporation
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

#ifndef NFC_HAL_TARGET_H
#define NFC_HAL_TARGET_H

#include "data_types.h"
#include "gki.h"

/****************************************************************************
** NCI related configuration
****************************************************************************/

/* Initial Max Control Packet Payload Size (until receiving payload size in
 * INIT_CORE_RSP) */
#ifndef NFC_HAL_NCI_INIT_CTRL_PAYLOAD_SIZE
#define NFC_HAL_NCI_INIT_CTRL_PAYLOAD_SIZE 0xFF
#endif

/* Number of bytes to reserve in front of NCI messages (e.g. for transport
 * header) */
#ifndef NFC_HAL_NCI_MSG_OFFSET_SIZE
#define NFC_HAL_NCI_MSG_OFFSET_SIZE 1
#endif

/* NFC-WAKE */
#ifndef NFC_HAL_LP_NFC_WAKE_GPIO
#define NFC_HAL_LP_NFC_WAKE_GPIO UPIO_GENERAL3
#endif

/* NFCC snooze mode idle timeout before deassert NFC_WAKE in ms */
#ifndef NFC_HAL_LP_IDLE_TIMEOUT
#define NFC_HAL_LP_IDLE_TIMEOUT 100
#endif

/* NFC snooze mode */
#ifndef NFC_HAL_LP_SNOOZE_MODE
#define NFC_HAL_LP_SNOOZE_MODE NFC_HAL_LP_SNOOZE_MODE_UART
#endif

/* Idle Threshold Host in 100ms unit */
#ifndef NFC_HAL_LP_IDLE_THRESHOLD_HOST
#define NFC_HAL_LP_IDLE_THRESHOLD_HOST 0
#endif

/* Idle Threshold HC in 100ms unit */
#ifndef NFC_HAL_LP_IDLE_THRESHOLD_HC
#define NFC_HAL_LP_IDLE_THRESHOLD_HC 0
#endif

/* Default NFCC power-up baud rate */
#ifndef NFC_HAL_DEFAULT_BAUD
#define NFC_HAL_DEFAULT_BAUD USERIAL_BAUD_115200
#endif

/* time (in ms) between power off and on NFCC */
#ifndef NFC_HAL_POWER_CYCLE_DELAY
#define NFC_HAL_POWER_CYCLE_DELAY 100
#endif

/* time (in ms) between power off and on NFCC */
#ifndef NFC_HAL_NFCC_ENABLE_TIMEOUT
#define NFC_HAL_NFCC_ENABLE_TIMEOUT 1000
#endif

#ifndef NFC_HAL_PRM_DEBUG
#define NFC_HAL_PRM_DEBUG TRUE
#endif

/* max patch data length (Can be overridden by platform for ACL HCI command
 * size) */
#ifndef NFC_HAL_PRM_HCD_CMD_MAXLEN
#define NFC_HAL_PRM_HCD_CMD_MAXLEN 250
#endif

/* Require PreI2C patch by default */
#ifndef NFC_HAL_PRE_I2C_PATCH_INCLUDED
#define NFC_HAL_PRE_I2C_PATCH_INCLUDED TRUE
#endif

/* over-riden for platforms that have transport packet limitations */
#ifndef NFC_HAL_PRM_MIN_NCI_CMD_PAYLOAD_SIZE
#define NFC_HAL_PRM_MIN_NCI_CMD_PAYLOAD_SIZE (32)
#endif

/* amount of time to wait for authenticating/committing patch to NVM */
#ifndef NFC_HAL_PRM_COMMIT_DELAY
#define NFC_HAL_PRM_COMMIT_DELAY (30000)
#endif

/* amount of time to wait after downloading preI2C patch before downloading
 * LPM/FPM patch */
#ifndef NFC_HAL_PRM_POST_I2C_FIX_DELAY
#define NFC_HAL_PRM_POST_I2C_FIX_DELAY (200)
#endif

/* NFCC will respond to more than one technology during listen discovery  */
#ifndef NFC_HAL_DM_MULTI_TECH_RESP
#define NFC_HAL_DM_MULTI_TECH_RESP TRUE
#endif

/* Data rate for 15693 command/response, it must be same as
 * RW_I93_FLAG_DATA_RATE in nfc_target.h */
#define NFC_HAL_I93_FLAG_DATA_RATE_HIGH 0x02

#ifndef NFC_HAL_I93_FLAG_DATA_RATE
#define NFC_HAL_I93_FLAG_DATA_RATE NFC_HAL_I93_FLAG_DATA_RATE_HIGH
#endif

/* NFC HAL HCI */
#ifndef NFC_HAL_HCI_INCLUDED
#define NFC_HAL_HCI_INCLUDED TRUE
#endif

/* Quick Timer */
#ifndef QUICK_TIMER_TICKS_PER_SEC
#define QUICK_TIMER_TICKS_PER_SEC 100 /* 10ms timer */
#endif

#ifndef NFC_HAL_SHARED_TRANSPORT_ENABLED
#define NFC_HAL_SHARED_TRANSPORT_ENABLED FALSE
#endif

/* Enable verbose tracing by default */
#ifndef NFC_HAL_TRACE_VERBOSE
#define NFC_HAL_TRACE_VERBOSE TRUE
#endif

#ifndef NFC_HAL_INITIAL_TRACE_LEVEL
#define NFC_HAL_INITIAL_TRACE_LEVEL 5
#endif

/* Map NFC serial port to USERIAL_PORT_6 by default */
#ifndef USERIAL_NFC_PORT
#define USERIAL_NFC_PORT (USERIAL_PORT_6)
#endif

/* Restore NFCC baud rate to default on shutdown if baud rate was updated */
#ifndef NFC_HAL_RESTORE_BAUD_ON_SHUTDOWN
#define NFC_HAL_RESTORE_BAUD_ON_SHUTDOWN TRUE
#endif

/* Enable protocol tracing by default */
#ifndef NFC_HAL_TRACE_PROTOCOL
#define NFC_HAL_TRACE_PROTOCOL TRUE
#endif

#endif /* GKI_TARGET_H */
