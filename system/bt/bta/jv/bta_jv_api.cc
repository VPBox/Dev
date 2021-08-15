/******************************************************************************
 *
 *  Copyright 2006-2012 Broadcom Corporation
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
 *  This is the implementation of the JAVA API for Bluetooth Wireless
 *  Technology (JABWT) as specified by the JSR82 specificiation
 *
 ******************************************************************************/
#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/logging.h>
#include <string.h>

#include "bt_common.h"
#include "bta_api.h"
#include "bta_jv_api.h"
#include "bta_jv_int.h"
#include "bta_sys.h"
#include "gap_api.h"
#include "port_api.h"
#include "sdp_api.h"
#include "stack/include/btu.h"
#include "utl.h"

using base::Bind;
using bluetooth::Uuid;

namespace {
bool bta_jv_enabled = false;
}

/*******************************************************************************
 *
 * Function         BTA_JvEnable
 *
 * Description      Enable the Java I/F service. When the enable
 *                  operation is complete the callback function will be
 *                  called with a BTA_JV_ENABLE_EVT. This function must
 *                  be called before other function in the JV API are
 *                  called.
 *
 * Returns          BTA_JV_SUCCESS if successful.
 *                  BTA_JV_FAIL if internal failure.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvEnable(tBTA_JV_DM_CBACK* p_cback) {
  if (!p_cback || bta_jv_enabled) {
    LOG(ERROR) << __func__ << ": failure";
    return BTA_JV_FAILURE;
  }

  VLOG(2) << __func__;

  memset(&bta_jv_cb, 0, sizeof(tBTA_JV_CB));
  /* set handle to invalid value by default */
  for (int i = 0; i < BTA_JV_PM_MAX_NUM; i++) {
    bta_jv_cb.pm_cb[i].handle = BTA_JV_PM_HANDLE_CLEAR;
  }

  bta_jv_enabled = true;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_enable, p_cback));
  return BTA_JV_SUCCESS;
}

/** Disable the Java I/F */
void BTA_JvDisable(void) {
  VLOG(2) << __func__;

  bta_jv_enabled = false;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_disable));
}

/*******************************************************************************
 *
 * Function         BTA_JvIsEncrypted
 *
 * Description      This function checks if the link to peer device is encrypted
 *
 * Returns          true if encrypted.
 *                  false if not.
 *
 ******************************************************************************/
bool BTA_JvIsEncrypted(const RawAddress& bd_addr) {
  bool is_encrypted = false;
  uint8_t sec_flags, le_flags;

  if (BTM_GetSecurityFlags(bd_addr, &sec_flags) &&
      BTM_GetSecurityFlagsByTransport(bd_addr, &le_flags, BT_TRANSPORT_LE)) {
    if (sec_flags & BTM_SEC_FLAG_ENCRYPTED || le_flags & BTM_SEC_FLAG_ENCRYPTED)
      is_encrypted = true;
  }
  return is_encrypted;
}
/*******************************************************************************
 *
 * Function         BTA_JvGetChannelId
 *
 * Description      This function reserves a SCN (server channel number) for
 *                  applications running over RFCOMM, L2CAP of L2CAP_LE.
 *                  It is primarily called by server profiles/applications to
 *                  register their SCN into the SDP database. The SCN is
 *                  reported by the tBTA_JV_DM_CBACK callback with a
 *                  BTA_JV_GET_SCN_EVT for RFCOMM channels and
 *                  BTA_JV_GET_PSM_EVT for L2CAP and LE.
 *                  If the SCN/PSM reported is 0, that means all resources are
 *                  exhausted.
 * Parameters
 *   conn_type      one of BTA_JV_CONN_TYPE_
 *   user_data      Any uservalue - will be returned in the resulting event.
 *   channel        Only used for RFCOMM - to try to allocate a specific RFCOMM
 *                  channel.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_JvGetChannelId(int conn_type, uint32_t id, int32_t channel) {
  VLOG(2) << __func__ << ": conn_type=" << conn_type;

  if (conn_type != BTA_JV_CONN_TYPE_RFCOMM &&
      conn_type != BTA_JV_CONN_TYPE_L2CAP &&
      conn_type != BTA_JV_CONN_TYPE_L2CAP_LE) {
    CHECK(false) << "Invalid conn_type=" << conn_type;
  }

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_get_channel_id, conn_type, channel, id, id));
}

/*******************************************************************************
 *
 * Function         BTA_JvFreeChannel
 *
 * Description      This function frees a server channel number that was used
 *                  by an application running over RFCOMM.
 * Parameters
 *   channel        The channel to free
 *   conn_type      one of BTA_JV_CONN_TYPE_
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvFreeChannel(uint16_t channel, int conn_type) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_free_scn, conn_type, channel));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvStartDiscovery
 *
 * Description      This function performs service discovery for the services
 *                  provided by the given peer device. When the operation is
 *                  complete the tBTA_JV_DM_CBACK callback function will be
 *                  called with a BTA_JV_DISCOVERY_COMP_EVT.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvStartDiscovery(const RawAddress& bd_addr,
                                    uint16_t num_uuid, const Uuid* p_uuid_list,
                                    uint32_t rfcomm_slot_id) {
  VLOG(2) << __func__;

  Uuid* uuid_list_copy = new Uuid[num_uuid];
  memcpy(uuid_list_copy, p_uuid_list, num_uuid * sizeof(Uuid));

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_start_discovery, bd_addr, num_uuid,
                         base::Owned(uuid_list_copy), rfcomm_slot_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvCreateRecord
 *
 * Description      Create a service record in the local SDP database.
 *                  When the operation is complete the tBTA_JV_DM_CBACK callback
 *                  function will be called with a BTA_JV_CREATE_RECORD_EVT.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvCreateRecordByUser(uint32_t rfcomm_slot_id) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_create_record, rfcomm_slot_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvDeleteRecord
 *
 * Description      Delete a service record in the local SDP database.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvDeleteRecord(uint32_t handle) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_delete_record, handle));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capConnectLE
 *
 * Description      Initiate an LE connection as a L2CAP client to the given BD
 *                  Address.
 *                  When the connection is initiated or failed to initiate,
 *                  tBTA_JV_L2CAP_CBACK is called with BTA_JV_L2CAP_CL_INIT_EVT
 *                  When the connection is established or failed,
 *                  tBTA_JV_L2CAP_CBACK is called with BTA_JV_L2CAP_OPEN_EVT
 *
 ******************************************************************************/
void BTA_JvL2capConnectLE(uint16_t remote_chan, const RawAddress& peer_bd_addr,
                          tBTA_JV_L2CAP_CBACK* p_cback,
                          uint32_t l2cap_socket_id) {
  VLOG(2) << __func__;
  CHECK(p_cback);
  do_in_main_thread(FROM_HERE, Bind(&bta_jv_l2cap_connect_le, remote_chan,
                                    peer_bd_addr, p_cback, l2cap_socket_id));
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capConnect
 *
 * Description      Initiate a connection as a L2CAP client to the given BD
 *                  Address.
 *                  When the connection is initiated or failed to initiate,
 *                  tBTA_JV_L2CAP_CBACK is called with BTA_JV_L2CAP_CL_INIT_EVT
 *                  When the connection is established or failed,
 *                  tBTA_JV_L2CAP_CBACK is called with BTA_JV_L2CAP_OPEN_EVT
 *
 ******************************************************************************/
void BTA_JvL2capConnect(int conn_type, tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                        std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info,
                        uint16_t remote_psm, uint16_t rx_mtu,
                        std::unique_ptr<tL2CAP_CFG_INFO> cfg,
                        const RawAddress& peer_bd_addr,
                        tBTA_JV_L2CAP_CBACK* p_cback,
                        uint32_t l2cap_socket_id) {
  VLOG(2) << __func__;
  CHECK(p_cback);

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_l2cap_connect, conn_type, sec_mask, role,
                         remote_psm, rx_mtu, peer_bd_addr, base::Passed(&cfg),
                         base::Passed(&ertm_info), p_cback, l2cap_socket_id));
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capClose
 *
 * Description      This function closes an L2CAP client connection
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capClose(uint32_t handle) {
  VLOG(2) << __func__;

  if (handle >= BTA_JV_MAX_L2C_CONN || !bta_jv_cb.l2c_cb[handle].p_cback)
    return BTA_JV_FAILURE;

  do_in_main_thread(
      FROM_HERE, Bind(&bta_jv_l2cap_close, handle, &bta_jv_cb.l2c_cb[handle]));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capCloseLE
 *
 * Description      This function closes an L2CAP client connection for Fixed
 *                  Channels Function is idempotent and no callbacks are called!
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capCloseLE(uint32_t handle) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_l2cap_close_fixed, handle));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capStartServer
 *
 * Description      This function starts an L2CAP server and listens for an
 *                  L2CAP connection from a remote Bluetooth device.  When the
 *                  server is started successfully, tBTA_JV_L2CAP_CBACK is
 *                  called with BTA_JV_L2CAP_START_EVT.  When the connection is
 *                  established tBTA_JV_L2CAP_CBACK is called with
 *                  BTA_JV_L2CAP_OPEN_EVT.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_JvL2capStartServer(int conn_type, tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                            std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info,
                            uint16_t local_psm, uint16_t rx_mtu,
                            std::unique_ptr<tL2CAP_CFG_INFO> cfg,
                            tBTA_JV_L2CAP_CBACK* p_cback,
                            uint32_t l2cap_socket_id) {
  VLOG(2) << __func__;
  CHECK(p_cback);

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_l2cap_start_server, conn_type, sec_mask, role,
                         local_psm, rx_mtu, base::Passed(&cfg),
                         base::Passed(&ertm_info), p_cback, l2cap_socket_id));
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capStartServerLE
 *
 * Description      This function starts an LE L2CAP server and listens for an
 *                  L2CAP connection from a remote Bluetooth device.  When the
 *                  server is started successfully, tBTA_JV_L2CAP_CBACK is
 *                  called with BTA_JV_L2CAP_START_EVT.  When the connection is
 *                  established, tBTA_JV_L2CAP_CBACK is called with
 *                  BTA_JV_L2CAP_OPEN_EVT.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTA_JvL2capStartServerLE(uint16_t local_chan, tBTA_JV_L2CAP_CBACK* p_cback,
                              uint32_t l2cap_socket_id) {
  VLOG(2) << __func__;
  CHECK(p_cback);
  do_in_main_thread(FROM_HERE, Bind(&bta_jv_l2cap_start_server_le, local_chan,
                                    p_cback, l2cap_socket_id));
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capStopServer
 *
 * Description      This function stops the L2CAP server. If the server has an
 *                  active connection, it would be closed.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capStopServer(uint16_t local_psm,
                                     uint32_t l2cap_socket_id) {
  VLOG(2) << __func__;

  do_in_main_thread(
      FROM_HERE, Bind(&bta_jv_l2cap_stop_server, local_psm, l2cap_socket_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capStopServerLE
 *
 * Description      This function stops the LE L2CAP server. If the server has
 *                  an active connection, it would be closed.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capStopServerLE(uint16_t local_chan,
                                       uint32_t l2cap_socket_id) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_l2cap_stop_server_le, local_chan));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capRead
 *
 * Description      This function reads data from an L2CAP connection
 *                  When the operation is complete, tBTA_JV_L2CAP_CBACK is
 *                  called with BTA_JV_L2CAP_READ_EVT.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capRead(uint32_t handle, uint32_t req_id,
                               uint8_t* p_data, uint16_t len) {
  VLOG(2) << __func__;

  if (handle >= BTA_JV_MAX_L2C_CONN || !bta_jv_cb.l2c_cb[handle].p_cback)
    return BTA_JV_FAILURE;

  tBTA_JV_L2CAP_READ evt_data;
  evt_data.status = BTA_JV_FAILURE;
  evt_data.handle = handle;
  evt_data.req_id = req_id;
  evt_data.p_data = p_data;
  evt_data.len = 0;

  if (BT_PASS ==
      GAP_ConnReadData((uint16_t)handle, p_data, len, &evt_data.len)) {
    evt_data.status = BTA_JV_SUCCESS;
  }
  bta_jv_cb.l2c_cb[handle].p_cback(BTA_JV_L2CAP_READ_EVT, (tBTA_JV*)&evt_data,
                                   bta_jv_cb.l2c_cb[handle].l2cap_socket_id);
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capReady
 *
 * Description      This function determined if there is data to read from
 *                    an L2CAP connection
 *
 * Returns          BTA_JV_SUCCESS, if data queue size is in *p_data_size.
 *                  BTA_JV_FAILURE, if error.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capReady(uint32_t handle, uint32_t* p_data_size) {
  tBTA_JV_STATUS status = BTA_JV_FAILURE;

  VLOG(2) << __func__ << ": handle=" << handle;
  if (p_data_size && handle < BTA_JV_MAX_L2C_CONN &&
      bta_jv_cb.l2c_cb[handle].p_cback) {
    *p_data_size = 0;
    if (BT_PASS == GAP_GetRxQueueCnt((uint16_t)handle, p_data_size)) {
      status = BTA_JV_SUCCESS;
    }
  }

  return (status);
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capWrite
 *
 * Description      This function writes data to an L2CAP connection
 *                  When the operation is complete, tBTA_JV_L2CAP_CBACK is
 *                  called with BTA_JV_L2CAP_WRITE_EVT. Works for
 *                  PSM-based connections. This function takes ownership of
 *                  p_data, and will osi_free it. Data length must be smaller
 *                  than remote maximum SDU size.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvL2capWrite(uint32_t handle, uint32_t req_id, BT_HDR* msg,
                                uint32_t user_id) {
  VLOG(2) << __func__;

  if (handle >= BTA_JV_MAX_L2C_CONN || !bta_jv_cb.l2c_cb[handle].p_cback) {
    osi_free(msg);
    return BTA_JV_FAILURE;
  }

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_l2cap_write, handle, req_id, msg,
                                    user_id, &bta_jv_cb.l2c_cb[handle]));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvL2capWriteFixed
 *
 * Description      This function writes data to an L2CAP connection
 *                  When the operation is complete, tBTA_JV_L2CAP_CBACK is
 *                  called with BTA_JV_L2CAP_WRITE_EVT. Works for
 *                  fixed-channel connections. This function takes ownership of
 *                  p_data, and will osi_free it.
 *
 ******************************************************************************/
void BTA_JvL2capWriteFixed(uint16_t channel, const RawAddress& addr,
                           uint32_t req_id, tBTA_JV_L2CAP_CBACK* p_cback,
                           BT_HDR* msg, uint32_t user_id) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE, Bind(&bta_jv_l2cap_write_fixed, channel, addr,
                                    req_id, msg, user_id, p_cback));
}

/*******************************************************************************
 *
 * Function         BTA_JvRfcommConnect
 *
 * Description      This function makes an RFCOMM conection to a remote BD
 *                  Address.
 *                  When the connection is initiated or failed to initiate,
 *                  tBTA_JV_RFCOMM_CBACK is called with
 *                  BTA_JV_RFCOMM_CL_INIT_EVT
 *                  When the connection is established or failed,
 *                  tBTA_JV_RFCOMM_CBACK is called with BTA_JV_RFCOMM_OPEN_EVT
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvRfcommConnect(tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                                   uint8_t remote_scn,
                                   const RawAddress& peer_bd_addr,
                                   tBTA_JV_RFCOMM_CBACK* p_cback,
                                   uint32_t rfcomm_slot_id) {
  VLOG(2) << __func__;

  if (!p_cback) return BTA_JV_FAILURE; /* Nothing to do */

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_rfcomm_connect, sec_mask, role, remote_scn,
                         peer_bd_addr, p_cback, rfcomm_slot_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvRfcommClose
 *
 * Description      This function closes an RFCOMM connection
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvRfcommClose(uint32_t handle, uint32_t rfcomm_slot_id) {
  uint32_t hi = ((handle & BTA_JV_RFC_HDL_MASK) & ~BTA_JV_RFCOMM_MASK) - 1;
  uint32_t si = BTA_JV_RFC_HDL_TO_SIDX(handle);

  VLOG(2) << __func__;

  if (hi >= BTA_JV_MAX_RFC_CONN || !bta_jv_cb.rfc_cb[hi].p_cback ||
      si >= BTA_JV_MAX_RFC_SR_SESSION || !bta_jv_cb.rfc_cb[hi].rfc_hdl[si])
    return BTA_JV_FAILURE;

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_rfcomm_close, handle, rfcomm_slot_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvRfcommStartServer
 *
 * Description      This function starts listening for an RFCOMM connection
 *                  request from a remote Bluetooth device.  When the server is
 *                  started successfully, tBTA_JV_RFCOMM_CBACK is called
 *                  with BTA_JV_RFCOMM_START_EVT.
 *                  When the connection is established, tBTA_JV_RFCOMM_CBACK
 *                  is called with BTA_JV_RFCOMM_OPEN_EVT.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvRfcommStartServer(tBTA_SEC sec_mask, tBTA_JV_ROLE role,
                                       uint8_t local_scn, uint8_t max_session,
                                       tBTA_JV_RFCOMM_CBACK* p_cback,
                                       uint32_t rfcomm_slot_id) {
  VLOG(2) << __func__;

  if (p_cback == NULL) return BTA_JV_FAILURE; /* Nothing to do */

  if (max_session == 0) max_session = 1;
  if (max_session > BTA_JV_MAX_RFC_SR_SESSION) {
    LOG(INFO) << __func__ << "max_session is too big. use max "
              << BTA_JV_MAX_RFC_SR_SESSION;
    max_session = BTA_JV_MAX_RFC_SR_SESSION;
  }

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_rfcomm_start_server, sec_mask, role, local_scn,
                         max_session, p_cback, rfcomm_slot_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvRfcommStopServer
 *
 * Description      This function stops the RFCOMM server. If the server has an
 *                  active connection, it would be closed.
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvRfcommStopServer(uint32_t handle,
                                      uint32_t rfcomm_slot_id) {
  VLOG(2) << __func__;

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_rfcomm_stop_server, handle, rfcomm_slot_id));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function         BTA_JvRfcommGetPortHdl
 *
 * Description    This function fetches the rfcomm port handle
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
uint16_t BTA_JvRfcommGetPortHdl(uint32_t handle) {
  uint32_t hi = ((handle & BTA_JV_RFC_HDL_MASK) & ~BTA_JV_RFCOMM_MASK) - 1;
  uint32_t si = BTA_JV_RFC_HDL_TO_SIDX(handle);

  if (hi < BTA_JV_MAX_RFC_CONN && si < BTA_JV_MAX_RFC_SR_SESSION &&
      bta_jv_cb.rfc_cb[hi].rfc_hdl[si])
    return bta_jv_cb.port_cb[bta_jv_cb.rfc_cb[hi].rfc_hdl[si] - 1].port_handle;
  else
    return 0xffff;
}

/*******************************************************************************
 *
 * Function         BTA_JvRfcommWrite
 *
 * Description      This function writes data to an RFCOMM connection
 *
 * Returns          BTA_JV_SUCCESS, if the request is being processed.
 *                  BTA_JV_FAILURE, otherwise.
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvRfcommWrite(uint32_t handle, uint32_t req_id) {
  uint32_t hi = ((handle & BTA_JV_RFC_HDL_MASK) & ~BTA_JV_RFCOMM_MASK) - 1;
  uint32_t si = BTA_JV_RFC_HDL_TO_SIDX(handle);

  VLOG(2) << __func__;

  VLOG(2) << __func__ << "handle=" << loghex(handle) << ", hi=" << hi
          << ", si=" << si;
  if (hi >= BTA_JV_MAX_RFC_CONN || !bta_jv_cb.rfc_cb[hi].p_cback ||
      si >= BTA_JV_MAX_RFC_SR_SESSION || !bta_jv_cb.rfc_cb[hi].rfc_hdl[si]) {
    return BTA_JV_FAILURE;
  }

  VLOG(2) << "write ok";

  tBTA_JV_RFC_CB* p_cb = &bta_jv_cb.rfc_cb[hi];
  do_in_main_thread(FROM_HERE, Bind(&bta_jv_rfcomm_write, handle, req_id, p_cb,
                                    &bta_jv_cb.port_cb[p_cb->rfc_hdl[si] - 1]));
  return BTA_JV_SUCCESS;
}

/*******************************************************************************
 *
 * Function    BTA_JVSetPmProfile
 *
 * Description: This function set or free power mode profile for different JV
 *              application.
 *
 * Parameters:  handle,  JV handle from RFCOMM or L2CAP
 *              app_id:  app specific pm ID, can be BTA_JV_PM_ALL, see
 *                       bta_dm_cfg.c for details
 *              BTA_JV_PM_ID_CLEAR: removes pm management on the handle. init_st
 *              is ignored and BTA_JV_CONN_CLOSE is called implicitly
 *              init_st:  state after calling this API. typically it should be
 *                        BTA_JV_CONN_OPEN
 *
 * Returns      BTA_JV_SUCCESS, if the request is being processed.
 *              BTA_JV_FAILURE, otherwise.
 *
 * NOTE:        BTA_JV_PM_ID_CLEAR: In general no need to be called as jv pm
 *                                  calls automatically
 *              BTA_JV_CONN_CLOSE to remove in case of connection close!
 *
 ******************************************************************************/
tBTA_JV_STATUS BTA_JvSetPmProfile(uint32_t handle, tBTA_JV_PM_ID app_id,
                                  tBTA_JV_CONN_STATE init_st) {
  VLOG(2) << __func__ << " handle=" << loghex(handle) << ", app_id:" << app_id;

  do_in_main_thread(FROM_HERE,
                    Bind(&bta_jv_set_pm_profile, handle, app_id, init_st));
  return BTA_JV_SUCCESS;
}
