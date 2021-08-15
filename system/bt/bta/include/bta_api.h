/******************************************************************************
 *
 *  Copyright 2003-2014 Broadcom Corporation
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
 *  This is the public interface file for BTA, Broadcom's Bluetooth
 *  application layer for mobile phones.
 *
 ******************************************************************************/
#ifndef BTA_API_H
#define BTA_API_H

#include <hardware/bt_common_types.h>
#include <memory>
#include "bt_target.h"
#include "bt_types.h"
#include "btm_api.h"
#include "btm_ble_api.h"

/*****************************************************************************
 *  Constants and data types
 ****************************************************************************/

/* Status Return Value */
#define BTA_SUCCESS 0 /* Successful operation. */
#define BTA_FAILURE 1 /* Generic failure. */
#define BTA_PENDING 2 /* API cannot be completed right now */
#define BTA_BUSY 3
#define BTA_NO_RESOURCES 4
#define BTA_WRONG_MODE 5

typedef uint8_t tBTA_STATUS;

/*
 * Service ID
 *
 * NOTES: When you add a new Service ID for BTA AND require to change the value
 * of BTA_MAX_SERVICE_ID, make sure that the correct security ID of the new
 * service from Security service definitions (btm_api.h) should be added to
 * bta_service_id_to_btm_srv_id_lkup_tbl table in bta_dm_act.cc
 */

#define BTA_RES_SERVICE_ID 0         /* Reserved */
#define BTA_SPP_SERVICE_ID 1         /* Serial port profile. */
#define BTA_DUN_SERVICE_ID 2         /* Dial-up networking profile. */
#define BTA_A2DP_SOURCE_SERVICE_ID 3 /* A2DP Source profile. */
#define BTA_LAP_SERVICE_ID 4         /* LAN access profile. */
#define BTA_HSP_SERVICE_ID 5         /* Headset profile. */
#define BTA_HFP_SERVICE_ID 6         /* Hands-free profile. */
#define BTA_OPP_SERVICE_ID 7         /* Object push  */
#define BTA_FTP_SERVICE_ID 8         /* File transfer */
#define BTA_CTP_SERVICE_ID 9         /* Cordless Terminal */
#define BTA_ICP_SERVICE_ID 10        /* Intercom Terminal */
#define BTA_SYNC_SERVICE_ID 11       /* Synchronization */
#define BTA_BPP_SERVICE_ID 12        /* Basic printing profile */
#define BTA_BIP_SERVICE_ID 13        /* Basic Imaging profile */
#define BTA_PANU_SERVICE_ID 14       /* PAN User */
#define BTA_NAP_SERVICE_ID 15        /* PAN Network access point */
#define BTA_GN_SERVICE_ID 16         /* PAN Group Ad-hoc networks */
#define BTA_SAP_SERVICE_ID 17        /* SIM Access profile */
#define BTA_A2DP_SINK_SERVICE_ID 18  /* A2DP Sink */
#define BTA_AVRCP_SERVICE_ID 19      /* A/V remote control */
#define BTA_HID_SERVICE_ID 20        /* HID */
#define BTA_VDP_SERVICE_ID 21        /* Video distribution */
#define BTA_PBAP_SERVICE_ID 22       /* PhoneBook Access Server*/
#define BTA_HSP_HS_SERVICE_ID 23     /* HFP HS role */
#define BTA_HFP_HS_SERVICE_ID 24     /* HSP HS role */
#define BTA_MAP_SERVICE_ID 25        /* Message Access Profile */
#define BTA_MN_SERVICE_ID 26         /* Message Notification Service */
#define BTA_HDP_SERVICE_ID 27        /* Health Device Profile */
#define BTA_PCE_SERVICE_ID 28        /* PhoneBook Access Client */
#define BTA_SDP_SERVICE_ID 29        /* SDP Search */
#define BTA_HIDD_SERVICE_ID 30       /* HID Device */

/* BLE profile service ID */
#define BTA_BLE_SERVICE_ID 31  /* GATT profile */
#define BTA_USER_SERVICE_ID 32 /* User requested UUID */
#define BTA_MAX_SERVICE_ID 33

/* service IDs (BTM_SEC_SERVICE_FIRST_EMPTY + 1) to (BTM_SEC_MAX_SERVICES - 1)
 * are used by BTA JV */
#define BTA_FIRST_JV_SERVICE_ID (BTM_SEC_SERVICE_FIRST_EMPTY + 1)
#define BTA_LAST_JV_SERVICE_ID (BTM_SEC_MAX_SERVICES - 1)

typedef uint8_t tBTA_SERVICE_ID;

/* Service ID Mask */
#define BTA_RES_SERVICE_MASK 0x00000001    /* Reserved */
#define BTA_SPP_SERVICE_MASK 0x00000002    /* Serial port profile. */
#define BTA_DUN_SERVICE_MASK 0x00000004    /* Dial-up networking profile. */
#define BTA_FAX_SERVICE_MASK 0x00000008    /* Fax profile. */
#define BTA_LAP_SERVICE_MASK 0x00000010    /* LAN access profile. */
#define BTA_HSP_SERVICE_MASK 0x00000020    /* HSP AG role. */
#define BTA_HFP_SERVICE_MASK 0x00000040    /* HFP AG role */
#define BTA_OPP_SERVICE_MASK 0x00000080    /* Object push  */
#define BTA_FTP_SERVICE_MASK 0x00000100    /* File transfer */
#define BTA_CTP_SERVICE_MASK 0x00000200    /* Cordless Terminal */
#define BTA_ICP_SERVICE_MASK 0x00000400    /* Intercom Terminal */
#define BTA_SYNC_SERVICE_MASK 0x00000800   /* Synchronization */
#define BTA_BPP_SERVICE_MASK 0x00001000    /* Print server */
#define BTA_BIP_SERVICE_MASK 0x00002000    /* Basic Imaging */
#define BTA_PANU_SERVICE_MASK 0x00004000   /* PAN User */
#define BTA_NAP_SERVICE_MASK 0x00008000    /* PAN Network access point */
#define BTA_GN_SERVICE_MASK 0x00010000     /* PAN Group Ad-hoc networks */
#define BTA_SAP_SERVICE_MASK 0x00020000    /* PAN Group Ad-hoc networks */
#define BTA_A2DP_SERVICE_MASK 0x00040000   /* Advanced audio distribution */
#define BTA_AVRCP_SERVICE_MASK 0x00080000  /* A/V remote control */
#define BTA_HID_SERVICE_MASK 0x00100000    /* HID */
#define BTA_VDP_SERVICE_MASK 0x00200000    /* Video distribution */
#define BTA_PBAP_SERVICE_MASK 0x00400000   /* Phone Book Server */
#define BTA_HSP_HS_SERVICE_MASK 0x00800000 /* HFP HS role */
#define BTA_HFP_HS_SERVICE_MASK 0x01000000 /* HSP HS role */
#define BTA_MAS_SERVICE_MASK 0x02000000    /* Message Access Profile */
#define BTA_MN_SERVICE_MASK 0x04000000     /* Message Notification Profile */
#define BTA_HL_SERVICE_MASK 0x08000000     /* Health Device Profile */
#define BTA_PCE_SERVICE_MASK 0x10000000    /* Phone Book Client */
#define BTA_HIDD_SERVICE_MASK 0x20000000   /* HID Device */

#define BTA_BLE_SERVICE_MASK 0x40000000  /* GATT based service */
#define BTA_ALL_SERVICE_MASK 0x7FFFFFFF  /* All services supported by BTA. */
#define BTA_USER_SERVICE_MASK 0x80000000 /* Message Notification Profile */

typedef uint32_t tBTA_SERVICE_MASK;

/* extended service mask, including mask with one or more GATT UUID */
typedef struct {
  tBTA_SERVICE_MASK srvc_mask;
  uint8_t num_uuid;
  bluetooth::Uuid* p_uuid;
} tBTA_SERVICE_MASK_EXT;

/* Security Setting Mask */
#define BTA_SEC_NONE BTM_SEC_NONE /* No security. */
#define BTA_SEC_AUTHORIZE                                               \
  (BTM_SEC_IN_AUTHORIZE) /* Authorization required (only needed for out \
                            going connection )*/
#define BTA_SEC_AUTHENTICATE \
  (BTM_SEC_IN_AUTHENTICATE | \
   BTM_SEC_OUT_AUTHENTICATE) /* Authentication required. */
#define BTA_SEC_ENCRYPT \
  (BTM_SEC_IN_ENCRYPT | BTM_SEC_OUT_ENCRYPT) /* Encryption required. */
#define BTA_SEC_MODE4_LEVEL4                                               \
  (BTM_SEC_MODE4_LEVEL4) /* Mode 4 level 4 service, i.e. incoming/outgoing \
                            MITM and P-256 encryption */
#define BTA_SEC_MITM \
  (BTM_SEC_IN_MITM | BTM_SEC_OUT_MITM) /* Man-In-The_Middle protection */
#define BTA_SEC_IN_16_DIGITS \
  (BTM_SEC_IN_MIN_16_DIGIT_PIN) /* Min 16 digit for pin code */

typedef uint16_t tBTA_SEC;

/* Ignore for Discoverable, Connectable, Pairable and Connectable Paired only
 * device modes */
#define BTA_DM_IGNORE 0x00FF

/* Ignore for Discoverable, Connectable only for LE modes */
#define BTA_DM_LE_IGNORE 0xFF00

#define BTA_APP_ID_PAN_MULTI 0xFE /* app id for pan multiple connection */
#define BTA_ALL_APP_ID 0xFF

/* Discoverable Modes */
#define BTA_DM_NON_DISC BTM_NON_DISCOVERABLE /* Device is not discoverable. */
#define BTA_DM_GENERAL_DISC                         \
  BTM_GENERAL_DISCOVERABLE /* General discoverable. \
                              */
#define BTA_DM_BLE_NON_DISCOVERABLE \
  BTM_BLE_NON_DISCOVERABLE /* Device is not LE discoverable */
#define BTA_DM_BLE_GENERAL_DISCOVERABLE \
  BTM_BLE_GENERAL_DISCOVERABLE /* Device is LE General discoverable */
#define BTA_DM_BLE_LIMITED_DISCOVERABLE \
  BTM_BLE_LIMITED_DISCOVERABLE /* Device is LE Limited discoverable */
typedef uint16_t
    tBTA_DM_DISC; /* this discoverability mode is a bit mask among BR mode and
                     LE mode */

/* Connectable Modes */
#define BTA_DM_NON_CONN BTM_NON_CONNECTABLE /* Device is not connectable. */
#define BTA_DM_CONN BTM_CONNECTABLE         /* Device is connectable. */
#define BTA_DM_BLE_NON_CONNECTABLE \
  BTM_BLE_NON_CONNECTABLE /* Device is LE non-connectable. */
#define BTA_DM_BLE_CONNECTABLE \
  BTM_BLE_CONNECTABLE /* Device is LE connectable. */

typedef uint16_t tBTA_DM_CONN;

#define BTA_TRANSPORT_UNKNOWN 0
#define BTA_TRANSPORT_BR_EDR BT_TRANSPORT_BR_EDR
#define BTA_TRANSPORT_LE BT_TRANSPORT_LE
typedef tBT_TRANSPORT tBTA_TRANSPORT;

/* Pairable Modes */
#define BTA_DM_PAIRABLE 1
#define BTA_DM_NON_PAIRABLE 0

/* Connectable Paired Only Mode */
#define BTA_DM_CONN_ALL 0
#define BTA_DM_CONN_PAIRED 1

/* Inquiry Modes */
#define BTA_DM_INQUIRY_NONE BTM_INQUIRY_NONE /*No BR inquiry. */
#define BTA_DM_GENERAL_INQUIRY \
  BTM_GENERAL_INQUIRY /* Perform general inquiry. */
#define BTA_DM_LIMITED_INQUIRY \
  BTM_LIMITED_INQUIRY /* Perform limited inquiry. */

#define BTA_BLE_INQUIRY_NONE BTM_BLE_INQUIRY_NONE
#define BTA_BLE_GENERAL_INQUIRY \
  BTM_BLE_GENERAL_INQUIRY /* Perform LE general inquiry. */
#define BTA_BLE_LIMITED_INQUIRY \
  BTM_BLE_LIMITED_INQUIRY /* Perform LE limited inquiry. */
typedef uint8_t tBTA_DM_INQ_MODE;

/* Inquiry Filter Type */
#define BTA_DM_INQ_CLR BTM_CLR_INQUIRY_FILTER /* Clear inquiry filter. */
#define BTA_DM_INQ_DEV_CLASS \
  BTM_FILTER_COND_DEVICE_CLASS /* Filter on device class. */
#define BTA_DM_INQ_BD_ADDR \
  BTM_FILTER_COND_BD_ADDR /* Filter on a specific  BD address. */

typedef uint8_t tBTA_DM_INQ_FILT;

/* Authorize Response */
#define BTA_DM_AUTH_PERM \
  0 /* Authorized for future connections to the service */
#define BTA_DM_AUTH_TEMP 1 /* Authorized for current connection only */
#define BTA_DM_NOT_AUTH 2  /* Not authorized for the service */

typedef uint8_t tBTA_AUTH_RESP;

/* M/S preferred roles */
#define BTA_ANY_ROLE 0x00
#define BTA_MASTER_ROLE_PREF 0x01
#define BTA_MASTER_ROLE_ONLY 0x02
#define BTA_SLAVE_ROLE_ONLY \
  0x03 /* Used for PANU only, skip role switch to master */

typedef uint8_t tBTA_PREF_ROLES;

enum {

  BTA_DM_NO_SCATTERNET,      /* Device doesn't support scatternet, it might
                                support "role switch during connection" for
                                an incoming connection, when it already has
                                another connection in master role */
  BTA_DM_PARTIAL_SCATTERNET, /* Device supports partial scatternet. It can have
                                simulateous connection in Master and Slave roles
                                for short period of time */
  BTA_DM_FULL_SCATTERNET /* Device can have simultaneous connection in master
                            and slave roles */

};

/* Inquiry filter device class condition */
typedef struct {
  DEV_CLASS dev_class;      /* device class of interest */
  DEV_CLASS dev_class_mask; /* mask to determine the bits of device class of
                               interest */
} tBTA_DM_COD_COND;

/* Inquiry Filter Condition */
typedef union {
  RawAddress bd_addr;              /* BD address of  device to filter. */
  tBTA_DM_COD_COND dev_class_cond; /* Device class filter condition */
} tBTA_DM_INQ_COND;

/* Inquiry Parameters */
typedef struct {
  tBTA_DM_INQ_MODE mode; /* Inquiry mode, limited or general. */
  uint8_t duration;      /* Inquiry duration in 1.28 sec units. */
  uint8_t max_resps; /* Maximum inquiry responses.  Set to zero for unlimited
                        responses. */
  bool report_dup; /* report duplicated inquiry response with higher RSSI value
                      */
  tBTA_DM_INQ_FILT filter_type; /* Filter condition type. */
  tBTA_DM_INQ_COND filter_cond; /* Filter condition data. */
} tBTA_DM_INQ;

typedef struct {
  uint8_t bta_dm_eir_min_name_len; /* minimum length of local name when it is
                                      shortened */
#if (BTA_EIR_CANNED_UUID_LIST == TRUE)
  uint8_t bta_dm_eir_uuid16_len; /* length of 16-bit UUIDs */
  uint8_t* bta_dm_eir_uuid16;    /* 16-bit UUIDs */
#else
  uint32_t uuid_mask[BTM_EIR_SERVICE_ARRAY_SIZE]; /* mask of UUID list in EIR */
#endif
  int8_t* bta_dm_eir_inq_tx_power;     /* Inquiry TX power         */
  uint8_t bta_dm_eir_flag_len;         /* length of flags in bytes */
  uint8_t* bta_dm_eir_flags;           /* flags for EIR */
  uint8_t bta_dm_eir_manufac_spec_len; /* length of manufacturer specific in
                                          bytes */
  uint8_t* bta_dm_eir_manufac_spec;    /* manufacturer specific */
  uint8_t bta_dm_eir_additional_len;   /* length of additional data in bytes */
  uint8_t* bta_dm_eir_additional;      /* additional data */
} tBTA_DM_EIR_CONF;

/* advertising filter policy */
typedef tBTM_BLE_AFP tBTA_BLE_AFP;

enum {
  BTA_BLE_BATCH_SCAN_MODE_PASS = 1,
  BTA_BLE_BATCH_SCAN_MODE_ACTI = 2,
  BTA_BLE_BATCH_SCAN_MODE_PASS_ACTI = 3
};
typedef uint8_t tBTA_BLE_BATCH_SCAN_MODE;

enum { BTA_BLE_DISCARD_OLD_ITEMS = 0, BTA_BLE_DISCARD_LOWER_RSSI_ITEMS = 1 };
typedef uint8_t tBTA_BLE_DISCARD_RULE;

enum { BTA_BLE_ADV_SEEN_FIRST_TIME = 0, BTA_BLE_ADV_TRACKING_TIMEOUT = 1 };
typedef uint8_t tBTA_BLE_ADV_CHANGE_REASON;

/* BLE customer specific feature function type definitions */
/* data type used on customer specific feature for RSSI monitoring */
#define BTA_BLE_RSSI_ALERT_HI 0
#define BTA_BLE_RSSI_ALERT_RANGE 1
#define BTA_BLE_RSSI_ALERT_LO 2
typedef uint8_t tBTA_DM_BLE_RSSI_ALERT_TYPE;

#define BTA_BLE_RSSI_ALERT_NONE BTM_BLE_RSSI_ALERT_NONE     /*    (0) */
#define BTA_BLE_RSSI_ALERT_HI_BIT BTM_BLE_RSSI_ALERT_HI_BIT /*    (1) */
#define BTA_BLE_RSSI_ALERT_RANGE_BIT \
  BTM_BLE_RSSI_ALERT_RANGE_BIT                              /*    (1 << 1) */
#define BTA_BLE_RSSI_ALERT_LO_BIT BTM_BLE_RSSI_ALERT_LO_BIT /*    (1 << 2) */
typedef uint8_t tBTA_DM_BLE_RSSI_ALERT_MASK;

typedef void(tBTA_DM_BLE_RSSI_CBACK)(const RawAddress& bd_addr,
                                     tBTA_DM_BLE_RSSI_ALERT_TYPE alert_type,
                                     int8_t rssi);

typedef int8_t tBTA_DM_RSSI_VALUE;
typedef uint8_t tBTA_DM_LINK_QUALITY_VALUE;

typedef uint8_t tBTA_SIG_STRENGTH_MASK;

/* Security Callback Events */
#define BTA_DM_ENABLE_EVT 0    /* Enable Event */
#define BTA_DM_DISABLE_EVT 1   /* Disable Event */
#define BTA_DM_PIN_REQ_EVT 2   /* PIN request. */
#define BTA_DM_AUTH_CMPL_EVT 3 /* Authentication complete indication. */
#define BTA_DM_AUTHORIZE_EVT 4 /* Authorization request. */
#define BTA_DM_LINK_UP_EVT 5   /* Connection UP event */
#define BTA_DM_LINK_DOWN_EVT 6 /* Connection DOWN event */
#define BTA_DM_SIG_STRENGTH_EVT                                             \
  7                             /* Signal strength for bluetooth connection \
                                   */
#define BTA_DM_BUSY_LEVEL_EVT 8 /* System busy level */
#define BTA_DM_BOND_CANCEL_CMPL_EVT 9 /* Bond cancel complete indication */
#define BTA_DM_SP_CFM_REQ_EVT                     \
  10 /* Simple Pairing User Confirmation request. \
        */
#define BTA_DM_SP_KEY_NOTIF_EVT 11 /* Simple Pairing Passkey Notification */
#define BTA_DM_SP_RMT_OOB_EVT 12   /* Simple Pairing Remote OOB Data request. */
#define BTA_DM_SP_KEYPRESS_EVT 13  /* Key press notification event. */
#define BTA_DM_ROLE_CHG_EVT 14     /* Role Change event. */
#define BTA_DM_BLE_KEY_EVT 15      /* BLE SMP key event for peer device keys */
#define BTA_DM_BLE_SEC_REQ_EVT 16  /* BLE SMP security request */
#define BTA_DM_BLE_PASSKEY_NOTIF_EVT 17 /* SMP passkey notification event */
#define BTA_DM_BLE_PASSKEY_REQ_EVT 18   /* SMP passkey request event */
#define BTA_DM_BLE_OOB_REQ_EVT 19       /* SMP OOB request event */
#define BTA_DM_BLE_LOCAL_IR_EVT 20      /* BLE local IR event */
#define BTA_DM_BLE_LOCAL_ER_EVT 21      /* BLE local ER event */
#define BTA_DM_BLE_NC_REQ_EVT 22 /* SMP Numeric Comparison request event */
#define BTA_DM_SP_RMT_OOB_EXT_EVT \
  23 /* Simple Pairing Remote OOB Extended Data request. */
#define BTA_DM_BLE_AUTH_CMPL_EVT 24 /* BLE Auth complete */
#define BTA_DM_DEV_UNPAIRED_EVT 25
#define BTA_DM_HW_ERROR_EVT 26 /* BT Chip H/W error */
#define BTA_DM_LE_FEATURES_READ                                             \
  27                             /* Cotroller specific LE features are read \
                                    */
#define BTA_DM_ENER_INFO_READ 28 /* Energy info read */
#define BTA_DM_BLE_SC_OOB_REQ_EVT 29 /* SMP SC OOB request event */
typedef uint8_t tBTA_DM_SEC_EVT;

/* Structure associated with BTA_DM_ENABLE_EVT */
typedef struct { tBTA_STATUS status; } tBTA_DM_ENABLE;

/* Structure associated with BTA_DM_PIN_REQ_EVT */
typedef struct {
  /* Note: First 3 data members must be, bd_addr, dev_class, and bd_name in
   * order */
  RawAddress bd_addr;  /* BD address peer device. */
  DEV_CLASS dev_class; /* Class of Device */
  BD_NAME bd_name;     /* Name of peer device. */
  bool min_16_digit;   /* true if the pin returned must be at least 16 digits */
} tBTA_DM_PIN_REQ;

/* BLE related definition */

#define BTA_DM_AUTH_FAIL_BASE (HCI_ERR_MAX_ERR + 10)

/* Converts SMP error codes defined in smp_api.h to SMP auth fail reasons below.
 */
#define BTA_DM_AUTH_CONVERT_SMP_CODE(x) (BTA_DM_AUTH_FAIL_BASE + (x))

#define BTA_DM_AUTH_SMP_PASSKEY_FAIL \
  (BTA_DM_AUTH_FAIL_BASE + SMP_PASSKEY_ENTRY_FAIL)
#define BTA_DM_AUTH_SMP_OOB_FAIL (BTA_DM_AUTH_FAIL_BASE + SMP_OOB_FAIL)
#define BTA_DM_AUTH_SMP_PAIR_AUTH_FAIL \
  (BTA_DM_AUTH_FAIL_BASE + SMP_PAIR_AUTH_FAIL)
#define BTA_DM_AUTH_SMP_CONFIRM_VALUE_FAIL \
  (BTA_DM_AUTH_FAIL_BASE + SMP_CONFIRM_VALUE_ERR)
#define BTA_DM_AUTH_SMP_PAIR_NOT_SUPPORT \
  (BTA_DM_AUTH_FAIL_BASE + SMP_PAIR_NOT_SUPPORT)
#define BTA_DM_AUTH_SMP_ENC_KEY_SIZE (BTA_DM_AUTH_FAIL_BASE + SMP_ENC_KEY_SIZE)
#define BTA_DM_AUTH_SMP_INVALID_CMD (BTA_DM_AUTH_FAIL_BASE + SMP_INVALID_CMD)
#define BTA_DM_AUTH_SMP_UNKNOWN_ERR \
  (BTA_DM_AUTH_FAIL_BASE + SMP_PAIR_FAIL_UNKNOWN)
#define BTA_DM_AUTH_SMP_REPEATED_ATTEMPT \
  (BTA_DM_AUTH_FAIL_BASE + SMP_REPEATED_ATTEMPTS)
#define BTA_DM_AUTH_SMP_INVALID_PARAMETERS \
  (BTA_DM_AUTH_FAIL_BASE + SMP_INVALID_PARAMETERS)
#define BTA_DM_AUTH_SMP_INTERNAL_ERR \
  (BTA_DM_AUTH_FAIL_BASE + SMP_PAIR_INTERNAL_ERR)
#define BTA_DM_AUTH_SMP_UNKNOWN_IO (BTA_DM_AUTH_FAIL_BASE + SMP_UNKNOWN_IO_CAP)
#define BTA_DM_AUTH_SMP_INIT_FAIL (BTA_DM_AUTH_FAIL_BASE + SMP_INIT_FAIL)
#define BTA_DM_AUTH_SMP_CONFIRM_FAIL (BTA_DM_AUTH_FAIL_BASE + SMP_CONFIRM_FAIL)
#define BTA_DM_AUTH_SMP_BUSY (BTA_DM_AUTH_FAIL_BASE + SMP_BUSY)
#define BTA_DM_AUTH_SMP_ENC_FAIL (BTA_DM_AUTH_FAIL_BASE + SMP_ENC_FAIL)
#define BTA_DM_AUTH_SMP_RSP_TIMEOUT (BTA_DM_AUTH_FAIL_BASE + SMP_RSP_TIMEOUT)
#define BTA_DM_AUTH_SMP_CONN_TOUT (BTA_DM_AUTH_FAIL_BASE + SMP_CONN_TOUT)

/* connection parameter boundary value and dummy value */
#define BTA_DM_BLE_SCAN_INT_MIN BTM_BLE_SCAN_INT_MIN
#define BTA_DM_BLE_SCAN_INT_MAX BTM_BLE_SCAN_INT_MAX
#define BTA_DM_BLE_SCAN_WIN_MIN BTM_BLE_SCAN_WIN_MIN
#define BTA_DM_BLE_SCAN_WIN_MAX BTM_BLE_SCAN_WIN_MAX
#define BTA_DM_BLE_CONN_INT_MIN BTM_BLE_CONN_INT_MIN
#define BTA_DM_BLE_CONN_INT_MAX BTM_BLE_CONN_INT_MAX
#define BTA_DM_BLE_CONN_LATENCY_MAX BTM_BLE_CONN_LATENCY_MAX
#define BTA_DM_BLE_CONN_SUP_TOUT_MIN BTM_BLE_CONN_SUP_TOUT_MIN
#define BTA_DM_BLE_CONN_SUP_TOUT_MAX BTM_BLE_CONN_SUP_TOUT_MAX
#define BTA_DM_BLE_CONN_PARAM_UNDEF                                          \
  BTM_BLE_CONN_PARAM_UNDEF /* use this value when a specific value not to be \
                              overwritten */

#define BTA_LE_KEY_PENC \
  BTM_LE_KEY_PENC /* encryption information of peer device */
#define BTA_LE_KEY_PID BTM_LE_KEY_PID     /* identity key of the peer device */
#define BTA_LE_KEY_PCSRK BTM_LE_KEY_PCSRK /* peer SRK */
#define BTA_LE_KEY_LENC \
  BTM_LE_KEY_LENC                     /* master role security information:div */
#define BTA_LE_KEY_LID BTM_LE_KEY_LID /* master device ID key */
#define BTA_LE_KEY_LCSRK \
  BTM_LE_KEY_LCSRK                /* local CSRK has been deliver to peer */
typedef uint8_t tBTA_LE_KEY_TYPE; /* can be used as a bit mask */

typedef tBTM_LE_PENC_KEYS tBTA_LE_PENC_KEYS;
typedef tBTM_LE_PCSRK_KEYS tBTA_LE_PCSRK_KEYS;
typedef tBTM_LE_LENC_KEYS tBTA_LE_LENC_KEYS;
typedef tBTM_LE_LCSRK_KEYS tBTA_LE_LCSRK_KEYS;
typedef tBTM_LE_PID_KEYS tBTA_LE_PID_KEYS;

typedef union {
  tBTA_LE_PENC_KEYS penc_key;  /* received peer encryption key */
  tBTA_LE_PCSRK_KEYS psrk_key; /* received peer device SRK */
  tBTA_LE_PID_KEYS pid_key;    /* peer device ID key */
  tBTA_LE_LENC_KEYS
      lenc_key; /* local encryption reproduction keys LTK = = d1(ER,DIV,0)*/
  tBTA_LE_LCSRK_KEYS lcsrk_key; /* local device CSRK = d1(ER,DIV,1)*/
  tBTA_LE_PID_KEYS lid_key; /* local device ID key for the particular remote */
} tBTA_LE_KEY_VALUE;

#define BTA_BLE_LOCAL_KEY_TYPE_ID 1
#define BTA_BLE_LOCAL_KEY_TYPE_ER 2
typedef uint8_t tBTA_DM_BLE_LOCAL_KEY_MASK;

typedef struct {
  Octet16 ir;
  Octet16 irk;
  Octet16 dhk;
} tBTA_BLE_LOCAL_ID_KEYS;

#define BTA_DM_SEC_GRANTED BTA_SUCCESS
#define BTA_DM_SEC_PAIR_NOT_SPT BTA_DM_AUTH_SMP_PAIR_NOT_SUPPORT
#define BTA_DM_SEC_REP_ATTEMPTS BTA_DM_AUTH_SMP_REPEATED_ATTEMPT
typedef uint8_t tBTA_DM_BLE_SEC_GRANT;

/* Structure associated with BTA_DM_BLE_SEC_REQ_EVT */
typedef struct {
  RawAddress bd_addr; /* peer address */
  BD_NAME bd_name; /* peer device name */
} tBTA_DM_BLE_SEC_REQ;

typedef struct {
  RawAddress bd_addr; /* peer address */
  tBTM_LE_KEY_TYPE key_type;
  tBTM_LE_KEY_VALUE* p_key_value;
} tBTA_DM_BLE_KEY;

/* Structure associated with BTA_DM_AUTH_CMPL_EVT */
typedef struct {
  RawAddress bd_addr;  /* BD address peer device. */
  BD_NAME bd_name;     /* Name of peer device. */
  bool key_present;    /* Valid link key value in key element */
  LinkKey key;         /* Link key associated with peer device. */
  uint8_t key_type;    /* The type of Link Key */
  bool success;        /* true of authentication succeeded, false if failed. */
  uint8_t fail_reason; /* The HCI reason/error code for when success=false */
  tBLE_ADDR_TYPE addr_type; /* Peer device address type */
  tBT_DEVICE_TYPE dev_type;
} tBTA_DM_AUTH_CMPL;

/* Structure associated with BTA_DM_AUTHORIZE_EVT */
typedef struct {
  RawAddress bd_addr;      /* BD address peer device. */
  BD_NAME bd_name;         /* Name of peer device. */
  tBTA_SERVICE_ID service; /* Service ID to authorize. */
  DEV_CLASS dev_class;
} tBTA_DM_AUTHORIZE;

/* Structure associated with BTA_DM_LINK_UP_EVT */
typedef struct {
  RawAddress bd_addr; /* BD address peer device. */
  tBTA_TRANSPORT link_type;
} tBTA_DM_LINK_UP;

/* Structure associated with BTA_DM_LINK_DOWN_EVT */
typedef struct {
  RawAddress bd_addr; /* BD address peer device. */
  uint8_t status;  /* connection open/closed */
  bool is_removed; /* true if device is removed when link is down */
  tBTA_TRANSPORT link_type;
} tBTA_DM_LINK_DOWN;

/* Structure associated with BTA_DM_ROLE_CHG_EVT */
typedef struct {
  RawAddress bd_addr; /* BD address peer device. */
  uint8_t new_role; /* the new connection role */
} tBTA_DM_ROLE_CHG;

/* Structure associated with BTA_DM_BUSY_LEVEL_EVT */
typedef struct {
  uint8_t level;       /* when paging or inquiring, level is 10.
                          Otherwise, the number of ACL links */
  uint8_t level_flags; /* indicates individual flags */
} tBTA_DM_BUSY_LEVEL;

#define BTA_IO_CAP_OUT BTM_IO_CAP_OUT       /* 0 DisplayOnly */
#define BTA_IO_CAP_IO BTM_IO_CAP_IO         /* 1 DisplayYesNo */
#define BTA_IO_CAP_IN BTM_IO_CAP_IN         /* 2 KeyboardOnly */
#define BTA_IO_CAP_NONE BTM_IO_CAP_NONE     /* 3 NoInputNoOutput */
#define BTA_IO_CAP_KBDISP BTM_IO_CAP_KBDISP /* 4 Keyboard display */
typedef tBTM_IO_CAP tBTA_IO_CAP;

#define BTA_AUTH_SP_NO                                      \
  BTM_AUTH_SP_NO /* 0 MITM Protection Not Required - Single \
                   Profile/non-bonding                      \
                   Numeric comparison with automatic accept allowed */
#define BTA_AUTH_SP_YES                                                       \
  BTM_AUTH_SP_YES /* 1 MITM Protection Required - Single Profile/non-bonding  \
                    Use IO Capabilities to determine authentication procedure \
                    */
#define BTA_AUTH_AP_NO                                                      \
  BTM_AUTH_AP_NO /* 2 MITM Protection Not Required - All Profiles/dedicated \
                   bonding                                                  \
                   Numeric comparison with automatic accept allowed */
#define BTA_AUTH_AP_YES                                                       \
  BTM_AUTH_AP_YES /* 3 MITM Protection Required - All Profiles/dedicated      \
                    bonding                                                   \
                    Use IO Capabilities to determine authentication procedure \
                    */
#define BTA_AUTH_SPGB_NO                                                       \
  BTM_AUTH_SPGB_NO /* 4 MITM Protection Not Required - Single Profiles/general \
                     bonding                                                   \
                     Numeric comparison with automatic accept allowed */
#define BTA_AUTH_SPGB_YES                                                   \
  BTM_AUTH_SPGB_YES /* 5 MITM Protection Required - Single Profiles/general \
                      bonding                                               \
                      Use IO Capabilities to determine authentication       \
                      procedure */
typedef tBTM_AUTH_REQ tBTA_AUTH_REQ;

#define BTA_AUTH_DD_BOND \
  BTM_AUTH_DD_BOND /* 2 this bit is set for dedicated bonding */
#define BTA_AUTH_GEN_BOND \
  BTM_AUTH_SPGB_NO /* 4 this bit is set for general bonding */
#define BTA_AUTH_BONDS \
  BTM_AUTH_BONDS /* 6 the general/dedicated bonding bits  */

#define BTA_LE_AUTH_NO_BOND BTM_LE_AUTH_REQ_NO_BOND /* 0*/
#define BTA_LE_AUTH_BOND BTM_LE_AUTH_REQ_BOND       /* 1 << 0 */
#define BTA_LE_AUTH_REQ_MITM BTM_LE_AUTH_REQ_MITM   /* 1 << 2 */

#define BTA_LE_AUTH_REQ_SC_ONLY BTM_LE_AUTH_REQ_SC_ONLY           /* 1 << 3 */
#define BTA_LE_AUTH_REQ_SC_BOND BTM_LE_AUTH_REQ_SC_BOND           /* 1001 */
#define BTA_LE_AUTH_REQ_SC_MITM BTM_LE_AUTH_REQ_SC_MITM           /* 1100 */
#define BTA_LE_AUTH_REQ_SC_MITM_BOND BTM_LE_AUTH_REQ_SC_MITM_BOND /* 1101 */
typedef tBTM_LE_AUTH_REQ
    tBTA_LE_AUTH_REQ; /* combination of the above bit pattern */

#define BTA_OOB_NONE BTM_OOB_NONE
#define BTA_OOB_PRESENT BTM_OOB_PRESENT
#define BTA_OOB_UNKNOWN BTM_OOB_UNKNOWN

typedef tBTM_OOB_DATA tBTA_OOB_DATA;

/* Structure associated with BTA_DM_SP_CFM_REQ_EVT */
typedef struct {
  /* Note: First 3 data members must be, bd_addr, dev_class, and bd_name in
   * order */
  RawAddress bd_addr;  /* peer address */
  DEV_CLASS dev_class; /* peer CoD */
  BD_NAME bd_name;     /* peer device name */
  uint32_t num_val; /* the numeric value for comparison. If just_works, do not
                       show this number to UI */
  bool just_works;  /* true, if "Just Works" association model */
  tBTA_AUTH_REQ loc_auth_req; /* Authentication required for local device */
  tBTA_AUTH_REQ rmt_auth_req; /* Authentication required for peer device */
  tBTA_IO_CAP loc_io_caps;    /* IO Capabilities of local device */
  tBTA_AUTH_REQ rmt_io_caps;  /* IO Capabilities of remote device */
} tBTA_DM_SP_CFM_REQ;

enum {
  BTA_SP_KEY_STARTED, /* passkey entry started */
  BTA_SP_KEY_ENTERED, /* passkey digit entered */
  BTA_SP_KEY_ERASED,  /* passkey digit erased */
  BTA_SP_KEY_CLEARED, /* passkey cleared */
  BTA_SP_KEY_COMPLT   /* passkey entry completed */
};
typedef uint8_t tBTA_SP_KEY_TYPE;

/* Structure associated with BTA_DM_SP_KEYPRESS_EVT */
typedef struct {
  RawAddress bd_addr; /* peer address */
  tBTA_SP_KEY_TYPE notif_type;
} tBTA_DM_SP_KEY_PRESS;

/* Structure associated with BTA_DM_SP_KEY_NOTIF_EVT */
typedef struct {
  /* Note: First 3 data members must be, bd_addr, dev_class, and bd_name in
   * order */
  RawAddress bd_addr;  /* peer address */
  DEV_CLASS dev_class; /* peer CoD */
  BD_NAME bd_name;     /* peer device name */
  uint32_t passkey; /* the numeric value for comparison. If just_works, do not
                       show this number to UI */
} tBTA_DM_SP_KEY_NOTIF;

/* Structure associated with BTA_DM_SP_RMT_OOB_EVT */
typedef struct {
  /* Note: First 3 data members must be, bd_addr, dev_class, and bd_name in
   * order */
  RawAddress bd_addr;  /* peer address */
  DEV_CLASS dev_class; /* peer CoD */
  BD_NAME bd_name;     /* peer device name */
} tBTA_DM_SP_RMT_OOB;

/* Structure associated with BTA_DM_BOND_CANCEL_CMPL_EVT */
typedef struct {
  tBTA_STATUS result; /* true of bond cancel succeeded, false if failed. */
} tBTA_DM_BOND_CANCEL_CMPL;

/* Union of all security callback structures */
typedef union {
  tBTA_DM_ENABLE enable;          /* BTA enabled */
  tBTA_DM_PIN_REQ pin_req;        /* PIN request. */
  tBTA_DM_AUTH_CMPL auth_cmpl;    /* Authentication complete indication. */
  tBTA_DM_AUTHORIZE authorize;    /* Authorization request. */
  tBTA_DM_LINK_UP link_up;        /* ACL connection down event */
  tBTA_DM_LINK_DOWN link_down;    /* ACL connection down event */
  tBTA_DM_BUSY_LEVEL busy_level;  /* System busy level */
  tBTA_DM_SP_CFM_REQ cfm_req;     /* user confirm request */
  tBTA_DM_SP_KEY_NOTIF key_notif; /* passkey notification */
  tBTA_DM_SP_RMT_OOB rmt_oob;     /* remote oob */
  tBTA_DM_BOND_CANCEL_CMPL
      bond_cancel_cmpl;               /* Bond Cancel Complete indication */
  tBTA_DM_SP_KEY_PRESS key_press;     /* key press notification event */
  tBTA_DM_ROLE_CHG role_chg;          /* role change event */
  tBTA_DM_BLE_SEC_REQ ble_req;        /* BLE SMP related request */
  tBTA_DM_BLE_KEY ble_key;            /* BLE SMP keys used when pairing */
  tBTA_BLE_LOCAL_ID_KEYS ble_id_keys; /* IR event */
  Octet16 ble_er;                     /* ER event data */
} tBTA_DM_SEC;

/* Security callback */
typedef void(tBTA_DM_SEC_CBACK)(tBTA_DM_SEC_EVT event, tBTA_DM_SEC* p_data);

#define BTA_DM_BLE_PF_LIST_LOGIC_OR 1
#define BTA_DM_BLE_PF_FILT_LOGIC_OR 0

/* Search callback events */
#define BTA_DM_INQ_RES_EVT 0  /* Inquiry result for a peer device. */
#define BTA_DM_INQ_CMPL_EVT 1 /* Inquiry complete. */
#define BTA_DM_DISC_RES_EVT 2 /* Discovery result for a peer device. */
#define BTA_DM_DISC_BLE_RES_EVT \
  3 /* Discovery result for BLE GATT based servoce on a peer device. */
#define BTA_DM_DISC_CMPL_EVT 4          /* Discovery complete. */
#define BTA_DM_DI_DISC_CMPL_EVT 5       /* Discovery complete. */
#define BTA_DM_SEARCH_CANCEL_CMPL_EVT 6 /* Search cancelled */

typedef uint8_t tBTA_DM_SEARCH_EVT;

#define BTA_DM_INQ_RES_IGNORE_RSSI \
  BTM_INQ_RES_IGNORE_RSSI /* 0x7f RSSI value not supplied (ignore it) */

/* Structure associated with BTA_DM_INQ_RES_EVT */
typedef struct {
  RawAddress bd_addr;          /* BD address peer device. */
  DEV_CLASS dev_class;         /* Device class of peer device. */
  bool remt_name_not_required; /* Application sets this flag if it already knows
                                  the name of the device */
  /* If the device name is known to application BTA skips the remote name
   * request */
  bool is_limited; /* true, if the limited inquiry bit is set in the CoD */
  int8_t rssi;     /* The rssi value */
  uint8_t* p_eir;  /* received EIR */
  uint16_t eir_len; /* received EIR length */
  uint8_t inq_result_type;
  uint8_t ble_addr_type;
  uint16_t ble_evt_type;
  uint8_t ble_primary_phy;
  uint8_t ble_secondary_phy;
  uint8_t ble_advertising_sid;
  int8_t ble_tx_power;
  uint16_t ble_periodic_adv_int;
  tBT_DEVICE_TYPE device_type;
  uint8_t flag;
} tBTA_DM_INQ_RES;

/* Structure associated with BTA_DM_INQ_CMPL_EVT */
typedef struct {
  uint8_t num_resps; /* Number of inquiry responses. */
} tBTA_DM_INQ_CMPL;

/* Structure associated with BTA_DM_DI_DISC_CMPL_EVT */
typedef struct {
  RawAddress bd_addr; /* BD address peer device. */
  uint8_t num_record; /* Number of DI record */
  tBTA_STATUS result;
} tBTA_DM_DI_DISC_CMPL;

/* Structure associated with BTA_DM_DISC_RES_EVT */
typedef struct {
  RawAddress bd_addr;          /* BD address peer device. */
  BD_NAME bd_name;             /* Name of peer device. */
  tBTA_SERVICE_MASK services;  /* Services found on peer device. */
  uint8_t* p_raw_data;         /* Raw data for discovery DB */
  uint32_t raw_data_size;      /* size of raw data */
  tBT_DEVICE_TYPE device_type; /* device type in case it is BLE device */
  uint32_t num_uuids;
  bluetooth::Uuid* p_uuid_list;
  tBTA_STATUS result;
} tBTA_DM_DISC_RES;

/* Structure associated with tBTA_DM_DISC_BLE_RES */
typedef struct {
  RawAddress bd_addr; /* BD address peer device. */
  BD_NAME bd_name;  /* Name of peer device. */
  bluetooth::Uuid service; /* GATT based Services UUID found on peer device. */
} tBTA_DM_DISC_BLE_RES;

/* Union of all search callback structures */
typedef union {
  tBTA_DM_INQ_RES inq_res;   /* Inquiry result for a peer device. */
  tBTA_DM_INQ_CMPL inq_cmpl; /* Inquiry complete. */
  tBTA_DM_DISC_RES disc_res; /* Discovery result for a peer device. */
  tBTA_DM_DISC_BLE_RES
      disc_ble_res;             /* discovery result for GATT based service */
  tBTA_DM_DI_DISC_CMPL di_disc; /* DI discovery result for a peer device */

} tBTA_DM_SEARCH;

/* Search callback */
typedef void(tBTA_DM_SEARCH_CBACK)(tBTA_DM_SEARCH_EVT event,
                                   tBTA_DM_SEARCH* p_data);

/* Execute call back */
typedef void(tBTA_DM_EXEC_CBACK)(void* p_param);

/* Encryption callback*/
typedef void(tBTA_DM_ENCRYPT_CBACK)(const RawAddress& bd_addr,
                                    tBTA_TRANSPORT transport,
                                    tBTA_STATUS result);

#define BTA_DM_BLE_SEC_NONE BTM_BLE_SEC_NONE
#define BTA_DM_BLE_SEC_ENCRYPT BTM_BLE_SEC_ENCRYPT
#define BTA_DM_BLE_SEC_NO_MITM BTM_BLE_SEC_ENCRYPT_NO_MITM
#define BTA_DM_BLE_SEC_MITM BTM_BLE_SEC_ENCRYPT_MITM
typedef tBTM_BLE_SEC_ACT tBTA_DM_BLE_SEC_ACT;

typedef tBTM_BLE_TX_TIME_MS tBTA_DM_BLE_TX_TIME_MS;
typedef tBTM_BLE_RX_TIME_MS tBTA_DM_BLE_RX_TIME_MS;
typedef tBTM_BLE_IDLE_TIME_MS tBTA_DM_BLE_IDLE_TIME_MS;
typedef tBTM_BLE_ENERGY_USED tBTA_DM_BLE_ENERGY_USED;

#define BTA_DM_CONTRL_UNKNOWN 0 /* Unknown state */
#define BTA_DM_CONTRL_ACTIVE 1  /* ACL link on, SCO link ongoing, sniff mode */
#define BTA_DM_CONTRL_SCAN                   \
  2 /* Scan state - paging/inquiry/trying to \
       connect*/
#define BTA_DM_CONTRL_IDLE                           \
  3 /* Idle state - page scan, LE advt, inquiry scan \
       */

typedef uint8_t tBTA_DM_CONTRL_STATE;

typedef uint8_t tBTA_DM_BLE_ADV_STATE;
typedef uint8_t tBTA_DM_BLE_ADV_INFO_PRESENT;
typedef uint8_t tBTA_DM_BLE_RSSI_VALUE;
typedef uint16_t tBTA_DM_BLE_ADV_INFO_TIMESTAMP;

typedef void(tBTA_BLE_ENERGY_INFO_CBACK)(tBTA_DM_BLE_TX_TIME_MS tx_time,
                                         tBTA_DM_BLE_RX_TIME_MS rx_time,
                                         tBTA_DM_BLE_IDLE_TIME_MS idle_time,
                                         tBTA_DM_BLE_ENERGY_USED energy_used,
                                         tBTA_DM_CONTRL_STATE ctrl_state,
                                         tBTA_STATUS status);

/* Maximum service name length */
#define BTA_SERVICE_NAME_LEN 35
#define BTA_SERVICE_DESP_LEN BTA_SERVICE_NAME_LEN
#define BTA_PROVIDER_NAME_LEN BTA_SERVICE_NAME_LEN

/* link policy masks  */
#define BTA_DM_LP_SWITCH HCI_ENABLE_MASTER_SLAVE_SWITCH
#define BTA_DM_LP_HOLD HCI_ENABLE_HOLD_MODE
#define BTA_DM_LP_SNIFF HCI_ENABLE_SNIFF_MODE
#define BTA_DM_LP_PARK HCI_ENABLE_PARK_MODE
typedef uint16_t tBTA_DM_LP_MASK;

/* power mode actions  */
#define BTA_DM_PM_NO_ACTION 0x00 /* no change to the current pm setting */
#define BTA_DM_PM_PARK 0x10      /* prefers park mode */
#define BTA_DM_PM_SNIFF 0x20     /* prefers sniff mode */
#define BTA_DM_PM_SNIFF1 0x21    /* prefers sniff1 mode */
#define BTA_DM_PM_SNIFF2 0x22    /* prefers sniff2 mode */
#define BTA_DM_PM_SNIFF3 0x23    /* prefers sniff3 mode */
#define BTA_DM_PM_SNIFF4 0x24    /* prefers sniff4 mode */
#define BTA_DM_PM_SNIFF5 0x25    /* prefers sniff5 mode */
#define BTA_DM_PM_SNIFF6 0x26    /* prefers sniff6 mode */
#define BTA_DM_PM_SNIFF7 0x27    /* prefers sniff7 mode */
#define BTA_DM_PM_SNIFF_USER0 \
  0x28 /* prefers user-defined sniff0 mode (testtool only) */
#define BTA_DM_PM_SNIFF_USER1 \
  0x29 /* prefers user-defined sniff1 mode (testtool only) */
#define BTA_DM_PM_ACTIVE 0x40  /* prefers active mode */
#define BTA_DM_PM_RETRY 0x80   /* retry power mode based on current settings */
#define BTA_DM_PM_SUSPEND 0x04 /* prefers suspend mode */
#define BTA_DM_PM_NO_PREF                                                   \
  0x01 /* service has no prefernce on power mode setting. eg. connection to \
          service got closed */

typedef uint8_t tBTA_DM_PM_ACTION;

/* index to bta_dm_ssr_spec */
#define BTA_DM_PM_SSR0 0
#define BTA_DM_PM_SSR1                      \
  1 /* BTA_DM_PM_SSR1 will be dedicated for \
    HH SSR setting entry, no other profile can use it */
#define BTA_DM_PM_SSR2 2
#define BTA_DM_PM_SSR3 3
#define BTA_DM_PM_SSR4 4
#define BTA_DM_PM_SSR5 5
#define BTA_DM_PM_SSR6 6

#define BTA_DM_PM_NUM_EVTS 9

#ifndef BTA_DM_PM_PARK_IDX
#define BTA_DM_PM_PARK_IDX \
  6 /* the actual index to bta_dm_pm_md[] for PARK mode */
#endif

#ifndef BTA_DM_PM_SNIFF_A2DP_IDX
#define BTA_DM_PM_SNIFF_A2DP_IDX BTA_DM_PM_SNIFF
#endif

#ifndef BTA_DM_PM_SNIFF_HD_IDLE_IDX
#define BTA_DM_PM_SNIFF_HD_IDLE_IDX BTA_DM_PM_SNIFF2
#endif

#ifndef BTA_DM_PM_SNIFF_SCO_OPEN_IDX
#define BTA_DM_PM_SNIFF_SCO_OPEN_IDX BTA_DM_PM_SNIFF3
#endif

#ifndef BTA_DM_PM_SNIFF_HD_ACTIVE_IDX
#define BTA_DM_PM_SNIFF_HD_ACTIVE_IDX BTA_DM_PM_SNIFF4
#endif

#ifndef BTA_DM_PM_SNIFF_HH_OPEN_IDX
#define BTA_DM_PM_SNIFF_HH_OPEN_IDX BTA_DM_PM_SNIFF2
#endif

#ifndef BTA_DM_PM_SNIFF_HH_ACTIVE_IDX
#define BTA_DM_PM_SNIFF_HH_ACTIVE_IDX BTA_DM_PM_SNIFF2
#endif

#ifndef BTA_DM_PM_SNIFF_HH_IDLE_IDX
#define BTA_DM_PM_SNIFF_HH_IDLE_IDX BTA_DM_PM_SNIFF2
#endif

#ifndef BTA_DM_PM_HH_OPEN_DELAY
#define BTA_DM_PM_HH_OPEN_DELAY 30000
#endif

#ifndef BTA_DM_PM_HH_ACTIVE_DELAY
#define BTA_DM_PM_HH_ACTIVE_DELAY 30000
#endif

#ifndef BTA_DM_PM_HH_IDLE_DELAY
#define BTA_DM_PM_HH_IDLE_DELAY 30000
#endif

/* The Sniff Parameters defined below must be ordered from highest
 * latency (biggest interval) to lowest latency.  If there is a conflict
 * among the connected services the setting with the lowest latency will
 * be selected.  If a device should override a sniff parameter then it
 * must insure that order is maintained.
 */
#ifndef BTA_DM_PM_SNIFF_MAX
#define BTA_DM_PM_SNIFF_MAX 800
#define BTA_DM_PM_SNIFF_MIN 400
#define BTA_DM_PM_SNIFF_ATTEMPT 4
#define BTA_DM_PM_SNIFF_TIMEOUT 1
#endif

#ifndef BTA_DM_PM_SNIFF1_MAX
#define BTA_DM_PM_SNIFF1_MAX 400
#define BTA_DM_PM_SNIFF1_MIN 200
#define BTA_DM_PM_SNIFF1_ATTEMPT 4
#define BTA_DM_PM_SNIFF1_TIMEOUT 1
#endif

#ifndef BTA_DM_PM_SNIFF2_MAX
#define BTA_DM_PM_SNIFF2_MAX 54
#define BTA_DM_PM_SNIFF2_MIN 30
#define BTA_DM_PM_SNIFF2_ATTEMPT 4
#define BTA_DM_PM_SNIFF2_TIMEOUT 1
#endif

#ifndef BTA_DM_PM_SNIFF3_MAX
#define BTA_DM_PM_SNIFF3_MAX 150
#define BTA_DM_PM_SNIFF3_MIN 50
#define BTA_DM_PM_SNIFF3_ATTEMPT 4
#define BTA_DM_PM_SNIFF3_TIMEOUT 1
#endif

#ifndef BTA_DM_PM_SNIFF4_MAX
#define BTA_DM_PM_SNIFF4_MAX 18
#define BTA_DM_PM_SNIFF4_MIN 10
#define BTA_DM_PM_SNIFF4_ATTEMPT 4
#define BTA_DM_PM_SNIFF4_TIMEOUT 1
#endif

#ifndef BTA_DM_PM_SNIFF5_MAX
#define BTA_DM_PM_SNIFF5_MAX 36
#define BTA_DM_PM_SNIFF5_MIN 30
#define BTA_DM_PM_SNIFF5_ATTEMPT 2
#define BTA_DM_PM_SNIFF5_TIMEOUT 0
#endif

#ifndef BTA_DM_PM_SNIFF6_MAX
#define BTA_DM_PM_SNIFF6_MAX 18
#define BTA_DM_PM_SNIFF6_MIN 14
#define BTA_DM_PM_SNIFF6_ATTEMPT 1
#define BTA_DM_PM_SNIFF6_TIMEOUT 0
#endif

#ifndef BTA_DM_PM_PARK_MAX
#define BTA_DM_PM_PARK_MAX 800
#define BTA_DM_PM_PARK_MIN 400
#define BTA_DM_PM_PARK_ATTEMPT 0
#define BTA_DM_PM_PARK_TIMEOUT 0
#endif

/* Switch callback events */
#define BTA_DM_SWITCH_CMPL_EVT 0 /* Completion of the Switch API */

typedef uint8_t tBTA_DM_SWITCH_EVT;
typedef void(tBTA_DM_SWITCH_CBACK)(tBTA_DM_SWITCH_EVT event,
                                   tBTA_STATUS status);

/* Audio routing out configuration */
#define BTA_DM_ROUTE_NONE 0x00      /* No Audio output */
#define BTA_DM_ROUTE_DAC 0x01       /* routing over analog output */
#define BTA_DM_ROUTE_I2S 0x02       /* routing over digital (I2S) output */
#define BTA_DM_ROUTE_BT_MONO 0x04   /* routing over SCO */
#define BTA_DM_ROUTE_BT_STEREO 0x08 /* routing over BT Stereo */
#define BTA_DM_ROUTE_HOST 0x10      /* routing over Host */
#define BTA_DM_ROUTE_FMTX 0x20      /* routing over FMTX */
#define BTA_DM_ROUTE_FMRX 0x40      /* routing over FMRX */
#define BTA_DM_ROUTE_BTSNK 0x80     /* routing over BT SNK */

typedef uint8_t tBTA_DM_ROUTE_PATH;

/* Device Identification (DI) data structure
*/
/* Used to set the DI record */
typedef tSDP_DI_RECORD tBTA_DI_RECORD;
/* Used to get the DI record */
typedef tSDP_DI_GET_RECORD tBTA_DI_GET_RECORD;
/* SDP discovery database */
typedef tSDP_DISCOVERY_DB tBTA_DISCOVERY_DB;

#ifndef BTA_DI_NUM_MAX
#define BTA_DI_NUM_MAX 3
#endif

/* Device features mask definitions */
#define BTA_FEATURE_BYTES_PER_PAGE BTM_FEATURE_BYTES_PER_PAGE
#define BTA_EXT_FEATURES_PAGE_MAX BTM_EXT_FEATURES_PAGE_MAX
/* ACL type
*/
#define BTA_DM_LINK_TYPE_BR_EDR 0x01
#define BTA_DM_LINK_TYPE_LE 0x02
#define BTA_DM_LINK_TYPE_ALL 0xFF
typedef uint8_t tBTA_DM_LINK_TYPE;

#define IMMEDIATE_DELY_MODE 0x00
#define ONFOUND_DELY_MODE 0x01
#define BATCH_DELY_MODE 0x02
#define ALLOW_ALL_FILTER 0x00
#define LOWEST_RSSI_VALUE 129

/*****************************************************************************
 *  External Function Declarations
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         BTA_EnableBluetooth
 *
 * Description      This function initializes BTA and prepares BTA and the
 *                  Bluetooth protocol stack for use.  This function is
 *                  typically called at startup or when Bluetooth services
 *                  are required by the phone.  This function must be called
 *                  before calling any other API function.
 *
 *
 * Returns          BTA_SUCCESS if successful.
 *                  BTA_FAIL if internal failure.
 *
 ******************************************************************************/
extern tBTA_STATUS BTA_EnableBluetooth(tBTA_DM_SEC_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         BTA_DisableBluetooth
 *
 * Description      This function disables BTA and the Bluetooth protocol
 *                  stack.  It is called when BTA is no longer being used
 *                  by any application in the system.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern tBTA_STATUS BTA_DisableBluetooth(void);

/*******************************************************************************
 *
 * Function         BTA_EnableTestMode
 *
 * Description      Enables bluetooth device under test mode
 *
 *
 * Returns          tBTA_STATUS
 *
 ******************************************************************************/
extern void BTA_EnableTestMode(void);

/*******************************************************************************
 *
 * Function         BTA_DisableTestMode
 *
 * Description      Disable bluetooth device under test mode
 *
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_DisableTestMode(void);

/*******************************************************************************
 *
 * Function         BTA_DmSetDeviceName
 *
 * Description      This function sets the Bluetooth name of the local device.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSetDeviceName(char* p_name);

/*******************************************************************************
 *
 * Function         BTA_DmSetVisibility
 *
 * Description      This function sets the Bluetooth connectable,discoverable,
 *                  pairable and conn paired only modesmodes of the local
 *                  device.
 *                  This controls whether other Bluetooth devices can find and
 *                  connect to the local device.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSetVisibility(tBTA_DM_DISC disc_mode, tBTA_DM_CONN conn_mode,
                                uint8_t pairable_mode, uint8_t conn_filter);

/*******************************************************************************
 *
 * Function         BTA_DmSearch
 *
 * Description      This function searches for peer Bluetooth devices.  It
 *                  first performs an inquiry; for each device found from the
 *                  inquiry it gets the remote name of the device.  If
 *                  parameter services is nonzero, service discovery will be
 *                  performed on each device for the services specified.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSearch(tBTA_DM_INQ* p_dm_inq, tBTA_SERVICE_MASK services,
                         tBTA_DM_SEARCH_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         BTA_DmSearchCancel
 *
 * Description      This function cancels a search that has been initiated
 *                  by calling BTA_DmSearch().
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSearchCancel(void);

/*******************************************************************************
 *
 * Function         BTA_DmDiscover
 *
 * Description      This function performs service discovery for the services
 *                  of a particular peer device.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmDiscover(const RawAddress& bd_addr,
                           tBTA_SERVICE_MASK services,
                           tBTA_DM_SEARCH_CBACK* p_cback, bool sdp_search);

/*******************************************************************************
 *
 * Function         BTA_DmDiscoverUUID
 *
 * Description      This function performs service discovery for the services
 *                  of a particular peer device.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmDiscoverUUID(const RawAddress& bd_addr,
                               const bluetooth::Uuid& uuid,
                               tBTA_DM_SEARCH_CBACK* p_cback, bool sdp_search);

/*******************************************************************************
 *
 * Function         BTA_DmGetCachedRemoteName
 *
 * Description      Retieve cached remote name if available
 *
 * Returns          BTA_SUCCESS if cached name was retrieved
 *                  BTA_FAILURE if cached name is not available
 *
 ******************************************************************************/
tBTA_STATUS BTA_DmGetCachedRemoteName(const RawAddress& remote_device,
                                      uint8_t** pp_cached_name);

/*******************************************************************************
 *
 * Function         BTA_DmBond
 *
 * Description      This function initiates a bonding procedure with a peer
 *                  device.  The bonding procedure enables authentication
 *                  and optionally encryption on the Bluetooth link.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBond(const RawAddress& bd_addr);

/*******************************************************************************
 *
 * Function         BTA_DmBondByTransport
 *
 * Description      This function initiates a bonding procedure with a peer
 *                  device by designated transport.  The bonding procedure
 *                  enables authentication and optionally encryption on the
 *                  Bluetooth link.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBondByTransport(const RawAddress& bd_addr,
                                  tBTA_TRANSPORT transport);

/*******************************************************************************
 *
 * Function         BTA_DmBondCancel
 *
 * Description      This function cancels a bonding procedure with a peer
 *                  device.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBondCancel(const RawAddress& bd_addr);

/*******************************************************************************
 *
 * Function         BTA_DmPinReply
 *
 * Description      This function provides a PIN when one is requested by DM
 *                  during a bonding procedure.  The application should call
 *                  this function after the security callback is called with
 *                  a BTA_DM_PIN_REQ_EVT.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmPinReply(const RawAddress& bd_addr, bool accept,
                           uint8_t pin_len, uint8_t* p_pin);

/*******************************************************************************
 *
 * Function         BTA_DmLocalOob
 *
 * Description      This function retrieves the OOB data from local controller.
 *                  The result is reported by bta_dm_co_loc_oob().
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmLocalOob(void);

/*******************************************************************************
 *
 * Function         BTA_DmConfirm
 *
 * Description      This function accepts or rejects the numerical value of the
 *                  Simple Pairing process on BTA_DM_SP_CFM_REQ_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmConfirm(const RawAddress& bd_addr, bool accept);

/*******************************************************************************
 *
 * Function         BTA_DmAddDevice
 *
 * Description      This function adds a device to the security database list
 *                  of peer devices. This function would typically be called
 *                  at system startup to initialize the security database with
 *                  known peer devices.  This is a direct execution function
 *                  that may lock task scheduling on some platforms.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmAddDevice(const RawAddress& bd_addr, DEV_CLASS dev_class,
                            const LinkKey& link_key,
                            tBTA_SERVICE_MASK trusted_mask, bool is_trusted,
                            uint8_t key_type, tBTA_IO_CAP io_cap,
                            uint8_t pin_length);

/*******************************************************************************
 *
 * Function         BTA_DmRemoveDevice
 *
 * Description      This function removes a device from the security database.
 *                  This is a direct execution function that may lock task
 *                  scheduling on some platforms.
 *
 *
 * Returns          BTA_SUCCESS if successful.
 *                  BTA_FAIL if operation failed.
 *
 ******************************************************************************/
extern tBTA_STATUS BTA_DmRemoveDevice(const RawAddress& bd_addr);

/*******************************************************************************
 *
 * Function         BTA_GetEirService
 *
 * Description      This function is called to get BTA service mask from EIR.
 *
 * Parameters       p_eir - pointer of EIR significant part
 *                  eir_len - EIR length
 *                  p_services - return the BTA service mask
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GetEirService(uint8_t* p_eir, size_t eir_len,
                              tBTA_SERVICE_MASK* p_services);

/*******************************************************************************
 *
 * Function         BTA_DmGetConnectionState
 *
 * Description      Returns whether the remote device is currently connected.
 *
 * Returns          0 if the device is NOT connected.
 *
 ******************************************************************************/
extern uint16_t BTA_DmGetConnectionState(const RawAddress& bd_addr);

/*******************************************************************************
 *
 * Function         BTA_DmSetLocalDiRecord
 *
 * Description      This function adds a DI record to the local SDP database.
 *
 * Returns          BTA_SUCCESS if record set sucessfully, otherwise error code.
 *
 ******************************************************************************/
extern tBTA_STATUS BTA_DmSetLocalDiRecord(tBTA_DI_RECORD* p_device_info,
                                          uint32_t* p_handle);

/*******************************************************************************
 *
 *
 * Function         BTA_DmCloseACL
 *
 * Description      This function force to close an ACL connection and remove
 the
 *                  device from the security database list of known devices.
 *
 * Parameters:      bd_addr       - Address of the peer device
 *                  remove_dev    - remove device or not after link down
 *                  transport     - which transport to close

 *
 * Returns          void.
 *
 ******************************************************************************/
extern void BTA_DmCloseACL(const RawAddress& bd_addr, bool remove_dev,
                           tBTA_TRANSPORT transport);

/* BLE related API functions */
/*******************************************************************************
 *
 * Function         BTA_DmBleSecurityGrant
 *
 * Description      Grant security request access.
 *
 * Parameters:      bd_addr          - BD address of the peer
 *                  res              - security grant status.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleSecurityGrant(const RawAddress& bd_addr,
                                   tBTA_DM_BLE_SEC_GRANT res);

/*******************************************************************************
 *
 * Function         BTA_DmBlePasskeyReply
 *
 * Description      Send BLE SMP passkey reply.
 *
 * Parameters:      bd_addr          - BD address of the peer
 *                  accept           - passkey entry sucessful or declined.
 *                  passkey          - passkey value, must be a 6 digit number,
 *                                     can be lead by 0.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBlePasskeyReply(const RawAddress& bd_addr, bool accept,
                                  uint32_t passkey);

/*******************************************************************************
 *
 * Function         BTA_DmBleConfirmReply
 *
 * Description      Send BLE SMP SC user confirmation reply.
 *
 * Parameters:      bd_addr          - BD address of the peer
 *                  accept           - numbers to compare are the same or
 *                                     different.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleConfirmReply(const RawAddress& bd_addr, bool accept);

/*******************************************************************************
 *
 * Function         BTA_DmAddBleDevice
 *
 * Description      Add a BLE device.  This function will be normally called
 *                  during host startup to restore all required information
 *                  for a LE device stored in the NVRAM.
 *
 * Parameters:      bd_addr          - BD address of the peer
 *                  dev_type         - Remote device's device type.
 *                  addr_type        - LE device address type.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmAddBleDevice(const RawAddress& bd_addr,
                               tBLE_ADDR_TYPE addr_type,
                               tBT_DEVICE_TYPE dev_type);

/*******************************************************************************
 *
 * Function         BTA_DmAddBleKey
 *
 * Description      Add/modify LE device information.  This function will be
 *                  normally called during host startup to restore all required
 *                  information stored in the NVRAM.
 *
 * Parameters:      bd_addr          - BD address of the peer
 *                  p_le_key         - LE key values.
 *                  key_type         - LE SMP key type.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmAddBleKey(const RawAddress& bd_addr,
                            tBTA_LE_KEY_VALUE* p_le_key,
                            tBTA_LE_KEY_TYPE key_type);

/*******************************************************************************
 *
 * Function         BTA_DmSetBlePrefConnParams
 *
 * Description      This function is called to set the preferred connection
 *                  parameters when default connection parameter is not desired.
 *
 * Parameters:      bd_addr          - BD address of the peripheral
 *                  min_conn_int     - minimum preferred connection interval
 *                  max_conn_int     - maximum preferred connection interval
 *                  slave_latency    - preferred slave latency
 *                  supervision_tout - preferred supervision timeout
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSetBlePrefConnParams(const RawAddress& bd_addr,
                                       uint16_t min_conn_int,
                                       uint16_t max_conn_int,
                                       uint16_t slave_latency,
                                       uint16_t supervision_tout);

/*******************************************************************************
 *
 * Function         BTA_DmSetBleConnScanParams
 *
 * Description      This function is called to set scan parameters used in
 *                  BLE connection request
 *
 * Parameters:      scan_interval    - scan interval
 *                  scan_window      - scan window
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSetBleConnScanParams(uint32_t scan_interval,
                                       uint32_t scan_window);

/*******************************************************************************
 *
 * Function         BTA_DmSearchExt
 *
 * Description      This function searches for peer Bluetooth devices. It
 *                  performs an inquiry and gets the remote name for devices.
 *                  Service discovery is done if services is non zero
 *
 * Parameters       p_dm_inq: inquiry conditions
 *                  services: if service is not empty, service discovery will be
 *                            done.
 *                            for all GATT based service condition, put
 *                            num_uuid, and p_uuid is the pointer to the list of
 *                            UUID values.
 *                  p_cback: callback functino when search is completed.
 *
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmSearchExt(tBTA_DM_INQ* p_dm_inq,
                            tBTA_SERVICE_MASK_EXT* p_services,
                            tBTA_DM_SEARCH_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         BTA_DmDiscoverExt
 *
 * Description      This function does service discovery for services of a
 *                  peer device. When services.num_uuid is 0, it indicates all
 *                  GATT based services are to be searched; other wise a list of
 *                  UUID of interested services should be provided through
 *                  services.p_uuid.
 *
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmDiscoverExt(const RawAddress& bd_addr,
                              tBTA_SERVICE_MASK_EXT* p_services,
                              tBTA_DM_SEARCH_CBACK* p_cback, bool sdp_search);

/*******************************************************************************
 *
 * Function         BTA_DmDiscoverByTransport
 *
 * Description      This function does service discovery on particular transport
 *                  for services of a
 *                  peer device. When services.num_uuid is 0, it indicates all
 *                  GATT based services are to be searched; other wise a list of
 *                  UUID of interested services should be provided through
 *                  p_services->p_uuid.
 *
 *
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmDiscoverByTransport(const RawAddress& bd_addr,
                                      tBTA_SERVICE_MASK_EXT* p_services,
                                      tBTA_DM_SEARCH_CBACK* p_cback,
                                      bool sdp_search,
                                      tBTA_TRANSPORT transport);

/*******************************************************************************
 *
 * Function         BTA_DmSetEncryption
 *
 * Description      This function is called to ensure that connection is
 *                  encrypted.  Should be called only on an open connection.
 *                  Typically only needed for connections that first want to
 *                  bring up unencrypted links, then later encrypt them.
 *
 * Parameters:      bd_addr       - Address of the peer device
 *                  transport     - transport of the link to be encruypted
 *                  p_callback    - Pointer to callback function to indicat the
 *                                  link encryption status
 *                  sec_act       - This is the security action to indicate
 *                                  what kind of BLE security level is required
 *                                  for the BLE link if BLE is supported
 *                                  Note: This parameter is ignored for
 *                                        BR/EDR or if BLE is not supported.
 *
 * Returns          void
 *
 *
 ******************************************************************************/
extern void BTA_DmSetEncryption(const RawAddress& bd_addr,
                                tBTA_TRANSPORT transport,
                                tBTA_DM_ENCRYPT_CBACK* p_callback,
                                tBTA_DM_BLE_SEC_ACT sec_act);

/*******************************************************************************
 *
 * Function         BTA_DmBleObserve
 *
 * Description      This procedure keep the device listening for advertising
 *                  events from a broadcast device.
 *
 * Parameters       start: start or stop observe.
 *                  duration : Duration of the scan. Continuous scan if 0 is
 *                             passed
 *                  p_results_cb: Callback to be called with scan results
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleObserve(bool start, uint8_t duration,
                             tBTA_DM_SEARCH_CBACK* p_results_cb);

/*******************************************************************************
 *
 * Function         BTA_DmBleConfigLocalPrivacy
 *
 * Description      Enable/disable privacy on the local device
 *
 * Parameters:      privacy_enable   - enable/disabe privacy on remote device.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleConfigLocalPrivacy(bool privacy_enable);

/*******************************************************************************
 *
 * Function         BTA_DmBleEnableRemotePrivacy
 *
 * Description      Enable/disable privacy on a remote device
 *
 * Parameters:      bd_addr          - BD address of the peer
 *                  privacy_enable   - enable/disabe privacy on remote device.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleEnableRemotePrivacy(const RawAddress& bd_addr,
                                         bool privacy_enable);

/*******************************************************************************
 *
 * Function         BTA_DmBleUpdateConnectionParams
 *
 * Description      Update connection parameters, can only be used when
 *                  connection is up.
 *
 * Parameters:      bd_addr   - BD address of the peer
 *                  min_int   - minimum connection interval, [0x0004 ~ 0x4000]
 *                  max_int   - maximum connection interval, [0x0004 ~ 0x4000]
 *                  latency   - slave latency [0 ~ 500]
 *                  timeout   - supervision timeout [0x000a ~ 0xc80]
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleUpdateConnectionParams(const RawAddress& bd_addr,
                                            uint16_t min_int, uint16_t max_int,
                                            uint16_t latency, uint16_t timeout,
                                            uint16_t min_ce_len,
                                            uint16_t max_ce_len);

/*******************************************************************************
 *
 * Function         BTA_DmBleSetDataLength
 *
 * Description      This function is to set maximum LE data packet size
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleSetDataLength(const RawAddress& remote_device,
                                   uint16_t tx_data_length);

/*******************************************************************************
 *
 * Function         BTA_DmBleGetEnergyInfo
 *
 * Description      This function is called to obtain the energy info
 *
 * Parameters       p_cmpl_cback - Command complete callback
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_DmBleGetEnergyInfo(tBTA_BLE_ENERGY_INFO_CBACK* p_cmpl_cback);

/*******************************************************************************
 *
 * Function         BTA_BrcmInit
 *
 * Description      This function initializes Broadcom specific VS handler in
 *                  BTA
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_VendorInit(void);

/*******************************************************************************
 *
 * Function         BTA_BrcmCleanup
 *
 * Description      This function frees up Broadcom specific VS specific dynamic
 *                  memory
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_VendorCleanup(void);

#endif /* BTA_API_H */
