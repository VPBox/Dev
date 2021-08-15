/******************************************************************************
 *
 *  Copyright (C) 2010-2014 Broadcom Corporation
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
 *  This is the public interface file for NFA SNEP, Broadcom's NFC
 *  application layer for mobile phones.
 *
 ******************************************************************************/
#ifndef NFA_SNEP_API_H
#define NFA_SNEP_API_H

#include "nfa_api.h"

/*****************************************************************************
**  Constants and data types
*****************************************************************************/

/* return an NDEF message           */
#define NFA_SNEP_REQ_CODE_GET 0x01
/* accept an NDEF message           */
#define NFA_SNEP_REQ_CODE_PUT 0x02
/* do not send remaining fragments  */

#define tNFA_SNEP_REQ_CODE uint8_t

#define tNFA_SNEP_RESP_CODE uint8_t

/* NFA SNEP callback events */
/* Server/client Registeration Status   */
#define NFA_SNEP_REG_EVT 0x00
/* LLCP link has been activated, client only   */
#define NFA_SNEP_ACTIVATED_EVT 0x01
/* LLCP link has been deactivated, client only */
#define NFA_SNEP_DEACTIVATED_EVT 0x02
/* Data link has been created           */
#define NFA_SNEP_CONNECTED_EVT 0x03
/* GET request from client              */
#define NFA_SNEP_GET_REQ_EVT 0x04
/* PUT request from client              */
#define NFA_SNEP_PUT_REQ_EVT 0x05
/* GET response from server             */
#define NFA_SNEP_GET_RESP_EVT 0x06
/* PUT response from server             */
#define NFA_SNEP_PUT_RESP_EVT 0x07
/* Failed to connect or disconnected    */
#define NFA_SNEP_DISC_EVT 0x08

#define NFA_SNEP_ALLOC_BUFF_EVT                                         \
  0x09                              /* Request to allocate a buffer for \
                                       NDEF*/
#define NFA_SNEP_FREE_BUFF_EVT 0x0A /* Request to deallocate buffer for NDEF*/
/* GET response sent to client          */
#define NFA_SNEP_GET_RESP_CMPL_EVT 0x0B

typedef uint8_t tNFA_SNEP_EVT;

/* Data for NFA_SNEP_REG_EVT */
typedef struct {
  tNFA_STATUS status;
  tNFA_HANDLE reg_handle; /* handle for registered server/client */
  char service_name[LLCP_MAX_SN_LEN + 1]; /* only for server */
} tNFA_SNEP_REG;

/* Data for NFA_SNEP_ACTIVATED_EVT */
typedef struct {
  tNFA_HANDLE client_handle; /* handle for registered client    */
} tNFA_SNEP_ACTIVATED;

/* Data for NFA_SNEP_DEACTIVATED_EVT */
typedef tNFA_SNEP_ACTIVATED tNFA_SNEP_DEACTIVATED;

/* Data for NFA_SNEP_CONNECTED_EVT */
/*
** for server, new handle will be assigned for conn_handle
** for client, handle used in NFA_SnepConnect () is returned in conn_handle
*/
typedef struct {
  tNFA_HANDLE reg_handle;  /* server/client handle            */
  tNFA_HANDLE conn_handle; /* handle for data link connection */
} tNFA_SNEP_CONNECT;

/* Data for NFA_SNEP_GET_REQ_EVT */
typedef struct {
  tNFA_HANDLE conn_handle;    /* handle for data link connection */
  uint32_t acceptable_length; /* acceptable length from client   */
  uint32_t ndef_length;       /* NDEF message length             */
  uint8_t* p_ndef;            /* NDEF message                    */
} tNFA_SNEP_GET_REQ;

/* Data for NFA_SNEP_PUT_REQ_EVT */
typedef struct {
  tNFA_HANDLE conn_handle; /* handle for data link connection */
  uint32_t ndef_length;    /* NDEF message length             */
  uint8_t* p_ndef;         /* NDEF message                    */
} tNFA_SNEP_PUT_REQ;

/* Data for NFA_SNEP_GET_RESP_EVT */
typedef struct {
  tNFA_HANDLE conn_handle;       /* handle for data link connection */
  tNFA_SNEP_RESP_CODE resp_code; /* response code from server       */
  uint32_t ndef_length;          /* NDEF message length             */
  uint8_t* p_ndef;               /* NDEF message                    */
} tNFA_SNEP_GET_RESP;

/* Data for NFA_SNEP_PUT_RESP_EVT */
typedef struct {
  tNFA_HANDLE conn_handle;       /* handle for data link connection */
  tNFA_SNEP_RESP_CODE resp_code; /* response code from server       */
} tNFA_SNEP_PUT_RESP;

/* Data for NFA_SNEP_DISC_EVT */
typedef struct {
  tNFA_HANDLE conn_handle; /* handle for data link connection */
                           /* client_handle if connection failed */
} tNFA_SNEP_DISC;

/* Data for NFA_SNEP_ALLOC_BUFF_EVT */
typedef struct {
  tNFA_HANDLE conn_handle; /* handle for data link connection                */
  tNFA_SNEP_REQ_CODE
      req_code; /* NFA_SNEP_REQ_CODE_GET or NFA_SNEP_REQ_CODE_PUT */
  tNFA_SNEP_RESP_CODE resp_code; /* Response code if cannot allocate buffer */
  uint32_t ndef_length; /* NDEF message length                            */
  uint8_t* p_buff;      /* buffer for NDEF message                        */
} tNFA_SNEP_ALLOC;

/* Data for NFA_SNEP_FREE_BUFF_EVT */
typedef struct {
  tNFA_HANDLE conn_handle; /* handle for data link connection */
  uint8_t* p_buff;         /* buffer to free                  */
} tNFA_SNEP_FREE;

/* Data for NFA_SNEP_GET_RESP_CMPL_EVT */
typedef struct {
  tNFA_HANDLE conn_handle; /* handle for data link connection */
  uint8_t* p_buff;         /* buffer for NDEF message         */
} tNFA_SNEP_GET_RESP_CMPL;

/* Union of all SNEP callback structures */
typedef union {
  tNFA_SNEP_REG reg;                     /* NFA_SNEP_REG_EVT             */
  tNFA_SNEP_ACTIVATED activated;         /* NFA_SNEP_ACTIVATED_EVT       */
  tNFA_SNEP_DEACTIVATED deactivated;     /* NFA_SNEP_DEACTIVATED_EVT     */
  tNFA_SNEP_CONNECT connect;             /* NFA_SNEP_CONNECTED_EVT       */
  tNFA_SNEP_GET_REQ get_req;             /* NFA_SNEP_GET_REQ_EVT         */
  tNFA_SNEP_PUT_REQ put_req;             /* NFA_SNEP_PUT_REQ_EVT         */
  tNFA_SNEP_GET_RESP get_resp;           /* NFA_SNEP_GET_RESP_EVT        */
  tNFA_SNEP_PUT_RESP put_resp;           /* NFA_SNEP_PUT_RESP_EVT        */
  tNFA_SNEP_DISC disc;                   /* NFA_SNEP_DISC_EVT            */
  tNFA_SNEP_ALLOC alloc;                 /* NFA_SNEP_ALLOC_BUFF_EVT      */
  tNFA_SNEP_FREE free;                   /* NFA_SNEP_FREE_BUFF_EVT       */
  tNFA_SNEP_GET_RESP_CMPL get_resp_cmpl; /* NFA_SNEP_GET_RESP_CMPL_EVT   */
} tNFA_SNEP_EVT_DATA;

/* NFA SNEP callback */
typedef void(tNFA_SNEP_CBACK)(tNFA_SNEP_EVT event, tNFA_SNEP_EVT_DATA* p_data);

#endif /* NFA_P2P_API_H */
