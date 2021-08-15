/******************************************************************************
 *
 *  Copyright (c) 2014 The Android Open Source Project
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

/*****************************************************************************
 *  Data types
 ****************************************************************************/

/* ASCII character string of arguments to the AT command */
#define BTA_HF_CLIENT_AT_MAX_LEN 512

typedef uint8_t tBTA_HF_CLIENT_AT_CMD;

/* Maximum combined buffer for received AT events string */
#define BTA_HF_CLIENT_AT_PARSER_MAX_LEN 4096

/* This structure holds prepared AT command queued for sending */
struct queued_at_cmd {
  tBTA_HF_CLIENT_AT_CMD cmd;
  char buf[BTA_HF_CLIENT_AT_MAX_LEN];
  uint16_t buf_len;
  struct queued_at_cmd* next;
};
typedef struct queued_at_cmd tBTA_HF_CLIENT_AT_QCMD;

/* Maximum number of indicators */
#define BTA_HF_CLIENT_AT_INDICATOR_COUNT 20

/* AT command parsing control block */
typedef struct {
  char buf[BTA_HF_CLIENT_AT_PARSER_MAX_LEN +
           1]; /* extra byte to always have \0 at the end */
  unsigned int offset;
  tBTA_HF_CLIENT_AT_CMD current_cmd;
  tBTA_HF_CLIENT_AT_QCMD* queued_cmd;
  alarm_t* resp_timer; /* AT response timer */
  alarm_t* hold_timer; /* AT hold timer */

  /* CIND: lookup table to store the sequence of incoming indicators and their
     values
     so when their values come later, we know which value in sequence match
     certain indicator */
  int indicator_lookup[BTA_HF_CLIENT_AT_INDICATOR_COUNT];

} tBTA_HF_CLIENT_AT_CB;
