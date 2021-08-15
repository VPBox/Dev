/******************************************************************************
 *
 *  Copyright 2003-2013 Broadcom Corporation
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
 *  This is the public interface file for BTA GATT.
 *
 ******************************************************************************/

#ifndef BTA_GATT_API_H
#define BTA_GATT_API_H

#include "bta/gatt/database.h"
#include "bta_api.h"
#include "gatt_api.h"

#include <base/callback_forward.h>
#include <vector>

#ifndef BTA_GATT_DEBUG
#define BTA_GATT_DEBUG false
#endif

/*****************************************************************************
 *  Constants and data types
 ****************************************************************************/
/**************************
 *  Common Definitions
 **************************/
/* GATT ID */
typedef struct {
  bluetooth::Uuid uuid; /* uuid of the attribute */
  uint8_t inst_id;      /* instance ID */
} __attribute__((packed)) tBTA_GATT_ID;

/* Client callback function events */
#define BTA_GATTC_DEREG_EVT 1        /* GATT client deregistered event */
#define BTA_GATTC_OPEN_EVT 2         /* GATTC open request status  event */
#define BTA_GATTC_CLOSE_EVT 5        /* GATTC  close request status event */
#define BTA_GATTC_SEARCH_CMPL_EVT 6  /* GATT discovery complete event */
#define BTA_GATTC_SEARCH_RES_EVT 7   /* GATT discovery result event */
#define BTA_GATTC_SRVC_DISC_DONE_EVT 8 /* GATT service discovery done event */
#define BTA_GATTC_NOTIF_EVT 10       /* GATT attribute notification event */
#define BTA_GATTC_EXEC_EVT 12        /* execute write complete event */
#define BTA_GATTC_ACL_EVT 13         /* ACL up event */
#define BTA_GATTC_CANCEL_OPEN_EVT 14 /* cancel open event */
#define BTA_GATTC_SRVC_CHG_EVT 15    /* service change event */
#define BTA_GATTC_ENC_CMPL_CB_EVT 17 /* encryption complete callback event */
#define BTA_GATTC_CFG_MTU_EVT 18     /* configure MTU complete event */
#define BTA_GATTC_CONGEST_EVT 24     /* Congestion event */
#define BTA_GATTC_PHY_UPDATE_EVT 25  /* PHY change event */
#define BTA_GATTC_CONN_UPDATE_EVT 26 /* Connection parameters update event */

typedef uint8_t tBTA_GATTC_EVT;

typedef struct {
  uint16_t unit;  /* as UUIUD defined by SIG */
  uint16_t descr; /* as UUID as defined by SIG */
  tGATT_FORMAT format;
  int8_t exp;
  uint8_t name_spc; /* The name space of the description */
} tBTA_GATT_CHAR_PRES;

/* Characteristic Aggregate Format attribute value
 */
#define BTA_GATT_AGGR_HANDLE_NUM_MAX 10
typedef struct {
  uint8_t num_handle;
  uint16_t handle_list[BTA_GATT_AGGR_HANDLE_NUM_MAX];
} tBTA_GATT_CHAR_AGGRE;

typedef struct {
  uint16_t len;
  uint8_t* p_value;
} tBTA_GATT_UNFMT;

#define BTA_GATT_CONN_NONE 0x0101 /* 0x0101 no connection to cancel  */
typedef uint16_t tBTA_GATT_REASON;

#define BTA_GATTC_MULTI_MAX GATT_MAX_READ_MULTI_HANDLES

typedef struct {
  uint8_t num_attr;
  uint16_t handles[BTA_GATTC_MULTI_MAX];
} tBTA_GATTC_MULTI;

/* callback data structure */
typedef struct {
  tGATT_STATUS status;
  tGATT_IF client_if;
} tBTA_GATTC_REG;

typedef struct {
  uint16_t conn_id;
  tGATT_STATUS status;
  uint16_t handle;
  uint16_t len;
  uint8_t value[GATT_MAX_ATTR_LEN];
} tBTA_GATTC_READ;

typedef struct {
  uint16_t conn_id;
  tGATT_STATUS status;
  uint16_t handle;
} tBTA_GATTC_WRITE;

typedef struct {
  uint16_t conn_id;
  tGATT_STATUS status;
} tBTA_GATTC_EXEC_CMPL;

typedef struct {
  uint16_t conn_id;
  tGATT_STATUS status;
} tBTA_GATTC_SEARCH_CMPL;

typedef struct {
  uint16_t conn_id;
  tBTA_GATT_ID service_uuid;
} tBTA_GATTC_SRVC_RES;

typedef struct {
  uint16_t conn_id;
  tGATT_STATUS status;
  uint16_t mtu;
} tBTA_GATTC_CFG_MTU;

typedef struct {
  tGATT_STATUS status;
  uint16_t conn_id;
  tGATT_IF client_if;
  RawAddress remote_bda;
  tBTA_TRANSPORT transport;
  uint16_t mtu;
} tBTA_GATTC_OPEN;

typedef struct {
  tGATT_STATUS status;
  uint16_t conn_id;
  tGATT_IF client_if;
  RawAddress remote_bda;
  tBTA_GATT_REASON reason; /* disconnect reason code, not useful when connect
                              event is reported */
} tBTA_GATTC_CLOSE;

typedef struct {
  uint16_t conn_id;
  RawAddress bda;
  uint16_t handle;
  uint16_t len;
  uint8_t value[GATT_MAX_ATTR_LEN];
  bool is_notify;
} tBTA_GATTC_NOTIFY;

typedef struct {
  uint16_t conn_id;
  bool congested; /* congestion indicator */
} tBTA_GATTC_CONGEST;

typedef struct {
  tGATT_STATUS status;
  tGATT_IF client_if;
  uint16_t conn_id;
  RawAddress remote_bda;
} tBTA_GATTC_OPEN_CLOSE;

typedef struct {
  tGATT_IF client_if;
  RawAddress remote_bda;
} tBTA_GATTC_ENC_CMPL_CB;

typedef struct {
  tGATT_IF server_if;
  uint16_t conn_id;
  uint8_t tx_phy;
  uint8_t rx_phy;
  tGATT_STATUS status;
} tBTA_GATTC_PHY_UPDATE;

typedef struct {
  tGATT_IF server_if;
  uint16_t conn_id;
  uint16_t interval;
  uint16_t latency;
  uint16_t timeout;
  tGATT_STATUS status;
} tBTA_GATTC_CONN_UPDATE;

typedef union {
  tGATT_STATUS status;

  tBTA_GATTC_SEARCH_CMPL search_cmpl; /* discovery complete */
  tBTA_GATTC_SRVC_RES srvc_res;       /* discovery result */
  tBTA_GATTC_REG reg_oper;            /* registration data */
  tBTA_GATTC_OPEN open;
  tBTA_GATTC_CLOSE close;
  tBTA_GATTC_READ read;           /* read attribute/descriptor data */
  tBTA_GATTC_WRITE write;         /* write complete data */
  tBTA_GATTC_EXEC_CMPL exec_cmpl; /*  execute complete */
  tBTA_GATTC_NOTIFY notify;       /* notification/indication event data */
  tBTA_GATTC_ENC_CMPL_CB enc_cmpl;
  RawAddress remote_bda;      /* service change event */
  tBTA_GATTC_CFG_MTU cfg_mtu; /* configure MTU operation */
  tBTA_GATTC_CONGEST congest;
  tBTA_GATTC_PHY_UPDATE phy_update;
  tBTA_GATTC_CONN_UPDATE conn_update;
} tBTA_GATTC;

/* GATTC enable callback function */
typedef void(tBTA_GATTC_ENB_CBACK)(tGATT_STATUS status);

/* Client callback function */
typedef void(tBTA_GATTC_CBACK)(tBTA_GATTC_EVT event, tBTA_GATTC* p_data);

/* GATT Server Data Structure */
/* Server callback function events */
#define BTA_GATTS_REG_EVT 0
#define BTA_GATTS_READ_CHARACTERISTIC_EVT \
  GATTS_REQ_TYPE_READ_CHARACTERISTIC                                 /* 1 */
#define BTA_GATTS_READ_DESCRIPTOR_EVT GATTS_REQ_TYPE_READ_DESCRIPTOR /* 2 */
#define BTA_GATTS_WRITE_CHARACTERISTIC_EVT \
  GATTS_REQ_TYPE_WRITE_CHARACTERISTIC                                  /* 3 */
#define BTA_GATTS_WRITE_DESCRIPTOR_EVT GATTS_REQ_TYPE_WRITE_DESCRIPTOR /* 4 */
#define BTA_GATTS_EXEC_WRITE_EVT GATTS_REQ_TYPE_WRITE_EXEC             /* 5 */
#define BTA_GATTS_MTU_EVT GATTS_REQ_TYPE_MTU                           /* 6 */
#define BTA_GATTS_CONF_EVT GATTS_REQ_TYPE_CONF                         /* 7 */
#define BTA_GATTS_DEREG_EVT 8
#define BTA_GATTS_DELELTE_EVT 11
#define BTA_GATTS_STOP_EVT 13
#define BTA_GATTS_CONNECT_EVT 14
#define BTA_GATTS_DISCONNECT_EVT 15
#define BTA_GATTS_OPEN_EVT 16
#define BTA_GATTS_CANCEL_OPEN_EVT 17
#define BTA_GATTS_CLOSE_EVT 18
#define BTA_GATTS_CONGEST_EVT 20
#define BTA_GATTS_PHY_UPDATE_EVT 21
#define BTA_GATTS_CONN_UPDATE_EVT 22

typedef uint8_t tBTA_GATTS_EVT;

#define BTA_GATTS_INVALID_APP 0xff

#define BTA_GATTS_INVALID_IF 0

#ifndef BTA_GATTC_CHAR_DESCR_MAX
#define BTA_GATTC_CHAR_DESCR_MAX 7
#endif

/***********************  NV callback Data Definitions   **********************
 */
typedef struct {
  bluetooth::Uuid app_uuid128;
  bluetooth::Uuid svc_uuid;
  uint16_t svc_inst;
  uint16_t s_handle;
  uint16_t e_handle;
  bool is_primary; /* primary service or secondary */
} tBTA_GATTS_HNDL_RANGE;

typedef struct {
  tGATT_STATUS status;
  RawAddress remote_bda;
  uint32_t trans_id;
  uint16_t conn_id;
  tGATTS_DATA* p_data;
} tBTA_GATTS_REQ;

typedef struct {
  tGATT_IF server_if;
  tGATT_STATUS status;
  bluetooth::Uuid uuid;
} tBTA_GATTS_REG_OPER;

typedef struct {
  tGATT_IF server_if;
  uint16_t service_id;
  uint16_t svc_instance;
  bool is_primary;
  tGATT_STATUS status;
  bluetooth::Uuid uuid;
} tBTA_GATTS_CREATE;

typedef struct {
  tGATT_IF server_if;
  uint16_t service_id;
  tGATT_STATUS status;
} tBTA_GATTS_SRVC_OPER;

typedef struct {
  tGATT_IF server_if;
  RawAddress remote_bda;
  uint16_t conn_id;
  tBTA_GATT_REASON reason; /* report disconnect reason */
  tGATT_TRANSPORT transport;
} tBTA_GATTS_CONN;

typedef struct {
  uint16_t conn_id;
  bool congested; /* report channel congestion indicator */
} tBTA_GATTS_CONGEST;

typedef struct {
  uint16_t conn_id;        /* connection ID */
  tGATT_STATUS status;     /* notification/indication status */
} tBTA_GATTS_CONF;

typedef struct {
  tGATT_IF server_if;
  uint16_t conn_id;
  uint8_t tx_phy;
  uint8_t rx_phy;
  tGATT_STATUS status;
} tBTA_GATTS_PHY_UPDATE;

typedef struct {
  tGATT_IF server_if;
  uint16_t conn_id;
  uint16_t interval;
  uint16_t latency;
  uint16_t timeout;
  tGATT_STATUS status;
} tBTA_GATTS_CONN_UPDATE;

/* GATTS callback data */
typedef union {
  tBTA_GATTS_REG_OPER reg_oper;
  tBTA_GATTS_CREATE create;
  tBTA_GATTS_SRVC_OPER srvc_oper;
  tGATT_STATUS status; /* BTA_GATTS_LISTEN_EVT */
  tBTA_GATTS_REQ req_data;
  tBTA_GATTS_CONN conn;             /* BTA_GATTS_CONN_EVT */
  tBTA_GATTS_CONGEST congest;       /* BTA_GATTS_CONGEST_EVT callback data */
  tBTA_GATTS_CONF confirm;          /* BTA_GATTS_CONF_EVT callback data */
  tBTA_GATTS_PHY_UPDATE phy_update; /* BTA_GATTS_PHY_UPDATE_EVT callback data */
  tBTA_GATTS_CONN_UPDATE
      conn_update; /* BTA_GATTS_CONN_UPDATE_EVT callback data */
} tBTA_GATTS;

/* GATTS enable callback function */
typedef void(tBTA_GATTS_ENB_CBACK)(tGATT_STATUS status);

/* Server callback function */
typedef void(tBTA_GATTS_CBACK)(tBTA_GATTS_EVT event, tBTA_GATTS* p_data);

/*****************************************************************************
 *  External Function Declarations
 ****************************************************************************/

/**************************
 *  Client Functions
 **************************/

/*******************************************************************************
 *
 * Function         BTA_GATTC_Disable
 *
 * Description      This function is called to disable the GATTC module
 *
 * Parameters       None.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_Disable(void);

using BtaAppRegisterCallback =
    base::Callback<void(uint8_t /* app_id */, uint8_t /* status */)>;

/**
 * This function is called to register application callbacks with BTA GATTC
 *module.
 * p_client_cb - pointer to the application callback function.
 **/
extern void BTA_GATTC_AppRegister(tBTA_GATTC_CBACK* p_client_cb,
                                  BtaAppRegisterCallback cb);

/*******************************************************************************
 *
 * Function         BTA_GATTC_AppDeregister
 *
 * Description      This function is called to deregister an application
 *                  from BTA GATTC module.
 *
 * Parameters       client_if - client interface identifier.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_AppDeregister(tGATT_IF client_if);

/*******************************************************************************
 *
 * Function         BTA_GATTC_Open
 *
 * Description      Open a direct connection or add a background auto connection
 *                  bd address
 *
 * Parameters       client_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *                  initiating_phys: LE PHY to use, optional
 *
 ******************************************************************************/
extern void BTA_GATTC_Open(tGATT_IF client_if, const RawAddress& remote_bda,
                           bool is_direct, tGATT_TRANSPORT transport,
                           bool opportunistic);
extern void BTA_GATTC_Open(tGATT_IF client_if, const RawAddress& remote_bda,
                           bool is_direct, tGATT_TRANSPORT transport,
                           bool opportunistic, uint8_t initiating_phys);

/*******************************************************************************
 *
 * Function         BTA_GATTC_CancelOpen
 *
 * Description      Open a direct connection or add a background auto connection
 *                  bd address
 *
 * Parameters       client_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTC_CancelOpen(tGATT_IF client_if,
                                 const RawAddress& remote_bda, bool is_direct);

/*******************************************************************************
 *
 * Function         BTA_GATTC_Close
 *
 * Description      Close a connection to a GATT server.
 *
 * Parameters       conn_id: connectino ID to be closed.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTC_Close(uint16_t conn_id);

/*******************************************************************************
 *
 * Function         BTA_GATTC_ServiceSearchRequest
 *
 * Description      This function is called to request a GATT service discovery
 *                  on a GATT server. This function report service search result
 *                  by a callback event, and followed by a service search
 *                  complete event.
 *
 * Parameters       conn_id: connection ID.
 *                  p_srvc_uuid: a UUID of the service application is interested
 *                               in. If Null, discover for all services.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_ServiceSearchRequest(uint16_t conn_id,
                                           bluetooth::Uuid* p_srvc_uuid);

/**
 * This function is called to send "Find service by UUID" request. Used only for
 * PTS tests.
 */
extern void BTA_GATTC_DiscoverServiceByUuid(uint16_t conn_id,
                                            const bluetooth::Uuid& srvc_uuid);

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetServices
 *
 * Description      This function is called to find the services on the given
 *                  server.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *
 * Returns          returns list of gatt::Service or NULL.
 *
 ******************************************************************************/
extern const std::list<gatt::Service>* BTA_GATTC_GetServices(uint16_t conn_id);

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetCharacteristic
 *
 * Description      This function is called to find the characteristic on the
 *                  given server.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *                  handle: characteristic handle
 *
 * Returns          returns pointer to gatt::Characteristic or NULL.
 *
 ******************************************************************************/
extern const gatt::Characteristic* BTA_GATTC_GetCharacteristic(uint16_t conn_id,
                                                               uint16_t handle);

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetDescriptor
 *
 * Description      This function is called to find the characteristic on the
 *                  given server.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *                  handle: descriptor handle
 *
 * Returns          returns pointer to gatt::Descriptor or NULL.
 *
 ******************************************************************************/
extern const gatt::Descriptor* BTA_GATTC_GetDescriptor(uint16_t conn_id,
                                                       uint16_t handle);

/* Return characteristic that owns descriptor with handle equal to |handle|, or
 * NULL */
extern const gatt::Characteristic* BTA_GATTC_GetOwningCharacteristic(
    uint16_t conn_id, uint16_t handle);

/* Return service that owns descriptor or characteristic with handle equal to
 * |handle|, or NULL */
extern const gatt::Service* BTA_GATTC_GetOwningService(uint16_t conn_id,
                                                       uint16_t handle);

/*******************************************************************************
 *
 * Function         BTA_GATTC_GetGattDb
 *
 * Description      This function is called to get gatt db.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *                  db: output parameter which will contain gatt db copy.
 *                      Caller is responsible for freeing it.
 *                  count: number of elements in db.
 *
 ******************************************************************************/
extern void BTA_GATTC_GetGattDb(uint16_t conn_id, uint16_t start_handle,
                                uint16_t end_handle, btgatt_db_element_t** db,
                                int* count);

typedef void (*GATT_READ_OP_CB)(uint16_t conn_id, tGATT_STATUS status,
                                uint16_t handle, uint16_t len, uint8_t* value,
                                void* data);
typedef void (*GATT_WRITE_OP_CB)(uint16_t conn_id, tGATT_STATUS status,
                                 uint16_t handle, void* data);

/*******************************************************************************
 *
 * Function         BTA_GATTC_ReadCharacteristic
 *
 * Description      This function is called to read a characteristics value
 *
 * Parameters       conn_id - connectino ID.
 *                  handle - characteritic handle to read.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ReadCharacteristic(uint16_t conn_id, uint16_t handle,
                                  tGATT_AUTH_REQ auth_req,
                                  GATT_READ_OP_CB callback, void* cb_data);

/**
 * This function is called to read a value of characteristic with uuid equal to
 * |uuid|
 */
void BTA_GATTC_ReadUsingCharUuid(uint16_t conn_id, const bluetooth::Uuid& uuid,
                                 uint16_t s_handle, uint16_t e_handle,
                                 tGATT_AUTH_REQ auth_req,
                                 GATT_READ_OP_CB callback, void* cb_data);

/*******************************************************************************
 *
 * Function         BTA_GATTC_ReadCharDescr
 *
 * Description      This function is called to read a descriptor value.
 *
 * Parameters       conn_id - connection ID.
 *                  handle - descriptor handle to read.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_ReadCharDescr(uint16_t conn_id, uint16_t handle,
                             tGATT_AUTH_REQ auth_req, GATT_READ_OP_CB callback,
                             void* cb_data);

/*******************************************************************************
 *
 * Function         BTA_GATTC_WriteCharValue
 *
 * Description      This function is called to write characteristic value.
 *
 * Parameters       conn_id - connection ID.
 *                  handle - characteristic handle to write.
 *                  write_type - type of write.
 *                  value - the value to be written.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_WriteCharValue(uint16_t conn_id, uint16_t handle,
                              tGATT_WRITE_TYPE write_type,
                              std::vector<uint8_t> value,
                              tGATT_AUTH_REQ auth_req,
                              GATT_WRITE_OP_CB callback, void* cb_data);

/*******************************************************************************
 *
 * Function         BTA_GATTC_WriteCharDescr
 *
 * Description      This function is called to write descriptor value.
 *
 * Parameters       conn_id - connection ID
 *                  handle - descriptor handle to write.
 *                  value - the value to be written.
 *
 * Returns          None
 *
 ******************************************************************************/
void BTA_GATTC_WriteCharDescr(uint16_t conn_id, uint16_t handle,
                              std::vector<uint8_t> value,
                              tGATT_AUTH_REQ auth_req,
                              GATT_WRITE_OP_CB callback, void* cb_data);

/*******************************************************************************
 *
 * Function         BTA_GATTC_SendIndConfirm
 *
 * Description      This function is called to send handle value confirmation.
 *
 * Parameters       conn_id - connection ID.
 *                  handle - characteristic handle to confirm.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_SendIndConfirm(uint16_t conn_id, uint16_t handle);

/*******************************************************************************
 *
 * Function         BTA_GATTC_RegisterForNotifications
 *
 * Description      This function is called to register for notification of a
 *                  service.
 *
 * Parameters       client_if - client interface.
 *                  remote_bda - target GATT server.
 *                  handle - GATT characteristic handle.
 *
 * Returns          OK if registration succeed, otherwise failed.
 *
 ******************************************************************************/
extern tGATT_STATUS BTA_GATTC_RegisterForNotifications(
    tGATT_IF client_if, const RawAddress& remote_bda, uint16_t handle);

/*******************************************************************************
 *
 * Function         BTA_GATTC_DeregisterForNotifications
 *
 * Description      This function is called to de-register for notification of a
 *                  service.
 *
 * Parameters       client_if - client interface.
 *                  remote_bda - target GATT server.
 *                  handle - GATT characteristic handle.
 *
 * Returns          OK if deregistration succeed, otherwise failed.
 *
 ******************************************************************************/
extern tGATT_STATUS BTA_GATTC_DeregisterForNotifications(
    tGATT_IF client_if, const RawAddress& remote_bda, uint16_t handle);

/*******************************************************************************
 *
 * Function         BTA_GATTC_PrepareWrite
 *
 * Description      This function is called to prepare write a characteristic
 *                  value.
 *
 * Parameters       conn_id - connection ID.
 *                  handle - GATT characteritic handle.
 *                  offset - offset of the write value.
 *                  value - the value to be written.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_PrepareWrite(uint16_t conn_id, uint16_t handle,
                                   uint16_t offset, std::vector<uint8_t> value,
                                   tGATT_AUTH_REQ auth_req,
                                   GATT_WRITE_OP_CB callback, void* cb_data);

/*******************************************************************************
 *
 * Function         BTA_GATTC_ExecuteWrite
 *
 * Description      This function is called to execute write a prepare write
 *                  sequence.
 *
 * Parameters       conn_id - connection ID.
 *                    is_execute - execute or cancel.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_ExecuteWrite(uint16_t conn_id, bool is_execute);

/*******************************************************************************
 *
 * Function         BTA_GATTC_ReadMultiple
 *
 * Description      This function is called to read multiple characteristic or
 *                  characteristic descriptors.
 *
 * Parameters       conn_id - connectino ID.
 *                    p_read_multi - read multiple parameters.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTC_ReadMultiple(uint16_t conn_id,
                                   tBTA_GATTC_MULTI* p_read_multi,
                                   tGATT_AUTH_REQ auth_req);

/*******************************************************************************
 *
 * Function         BTA_GATTC_Refresh
 *
 * Description      Refresh the server cache of the remote device
 *
 * Parameters       remote_bda: remote device BD address.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTC_Refresh(const RawAddress& remote_bda);

/*******************************************************************************
 *
 * Function         BTA_GATTC_ConfigureMTU
 *
 * Description      Configure the MTU size in the GATT channel. This can be done
 *                  only once per connection.
 *
 * Parameters       conn_id: connection ID.
 *                  mtu: desired MTU size to use.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTC_ConfigureMTU(uint16_t conn_id, uint16_t mtu);

/*******************************************************************************
 *  BTA GATT Server API
 ******************************************************************************/

/*******************************************************************************
 *
 * Function         BTA_GATTS_Init
 *
 * Description      This function is called to initalize GATTS module
 *
 * Parameters       None
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTS_Init();

/*******************************************************************************
 *
 * Function         BTA_GATTS_Disable
 *
 * Description      This function is called to disable GATTS module
 *
 * Parameters       None.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTS_Disable(void);

/*******************************************************************************
 *
 * Function         BTA_GATTS_AppRegister
 *
 * Description      This function is called to register application callbacks
 *                    with BTA GATTS module.
 *
 * Parameters       p_app_uuid - applicaiton UUID
 *                  p_cback - pointer to the application callback function.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTS_AppRegister(const bluetooth::Uuid& app_uuid,
                                  tBTA_GATTS_CBACK* p_cback);

/*******************************************************************************
 *
 * Function         BTA_GATTS_AppDeregister
 *
 * Description      De-register with BTA GATT Server.
 *
 * Parameters       server_if: server interface
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTS_AppDeregister(tGATT_IF server_if);

/*******************************************************************************
 *
 * Function         BTA_GATTS_AddService
 *
 * Description      Add the given |service| and all included elements to the
 *                  GATT database. a |BTA_GATTS_ADD_SRVC_EVT| is triggered to
 *                  report the status and attribute handles.
 *
 * Parameters       server_if: server interface.
 *                  service: pointer to vector describing service.
 *
 * Returns          Returns |GATT_SUCCESS| on success or |GATT_ERROR| if the
 *                  service cannot be added.
 *
 ******************************************************************************/
typedef base::Callback<void(uint8_t status, int server_if,
                            std::vector<btgatt_db_element_t> service)>
    BTA_GATTS_AddServiceCb;

extern void BTA_GATTS_AddService(tGATT_IF server_if,
                                 std::vector<btgatt_db_element_t> service,
                                 BTA_GATTS_AddServiceCb cb);

/*******************************************************************************
 *
 * Function         BTA_GATTS_DeleteService
 *
 * Description      This function is called to delete a service. When this is
 *                  done, a callback event BTA_GATTS_DELETE_EVT is report with
 *                  the status.
 *
 * Parameters       service_id: service_id to be deleted.
 *
 * Returns          returns none.
 *
 ******************************************************************************/
extern void BTA_GATTS_DeleteService(uint16_t service_id);

/*******************************************************************************
 *
 * Function         BTA_GATTS_StopService
 *
 * Description      This function is called to stop a service.
 *
 * Parameters       service_id - service to be topped.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTS_StopService(uint16_t service_id);

/*******************************************************************************
 *
 * Function         BTA_GATTS_HandleValueIndication
 *
 * Description      This function is called to read a characteristics
 *                  descriptor.
 *
 * Parameters       conn_id - connection identifier.
 *                  attr_id - attribute ID to indicate.
 *                  value - data to indicate.
 *                  need_confirm - if this indication expects a confirmation or
 *                                 not.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTS_HandleValueIndication(uint16_t conn_id, uint16_t attr_id,
                                            std::vector<uint8_t> value,
                                            bool need_confirm);

/*******************************************************************************
 *
 * Function         BTA_GATTS_SendRsp
 *
 * Description      This function is called to send a response to a request.
 *
 * Parameters       conn_id - connection identifier.
 *                  trans_id - transaction ID.
 *                  status - response status
 *                  p_msg - response data.
 *
 * Returns          None
 *
 ******************************************************************************/
extern void BTA_GATTS_SendRsp(uint16_t conn_id, uint32_t trans_id,
                              tGATT_STATUS status, tGATTS_RSP* p_msg);

/*******************************************************************************
 *
 * Function         BTA_GATTS_Open
 *
 * Description      Open a direct open connection or add a background auto
 *                  connection bd address
 *
 * Parameters       server_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTS_Open(tGATT_IF server_if, const RawAddress& remote_bda,
                           bool is_direct, tGATT_TRANSPORT transport);

/*******************************************************************************
 *
 * Function         BTA_GATTS_CancelOpen
 *
 * Description      Cancel a direct open connection or remove a background auto
 *                  connection bd address
 *
 * Parameters       server_if: server interface.
 *                  remote_bda: remote device BD address.
 *                  is_direct: direct connection or background auto connection
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTS_CancelOpen(tGATT_IF server_if,
                                 const RawAddress& remote_bda, bool is_direct);

/*******************************************************************************
 *
 * Function         BTA_GATTS_Close
 *
 * Description      Close a connection  a remote device.
 *
 * Parameters       conn_id: connectino ID to be closed.
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTA_GATTS_Close(uint16_t conn_id);

#endif /* BTA_GATT_API_H */
