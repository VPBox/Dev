/******************************************************************************
 *
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
 *  This file contains the GATT client discovery procedures and cache
 *  related functions.
 *
 ******************************************************************************/

#define LOG_TAG "bt_bta_gattc"

#include "bt_target.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sstream>

#include "bt_common.h"
#include "bta_gattc_int.h"
#include "bta_sys.h"
#include "btm_api.h"
#include "btm_ble_api.h"
#include "btm_int.h"
#include "database.h"
#include "database_builder.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "sdp_api.h"
#include "sdpdefs.h"
#include "utl.h"

using base::StringPrintf;
using bluetooth::Uuid;
using gatt::Characteristic;
using gatt::Database;
using gatt::DatabaseBuilder;
using gatt::Descriptor;
using gatt::IncludedService;
using gatt::Service;
using gatt::StoredAttribute;

static void bta_gattc_cache_write(const RawAddress& server_bda,
                                  const std::vector<StoredAttribute>& attr);
static tGATT_STATUS bta_gattc_sdp_service_disc(uint16_t conn_id,
                                               tBTA_GATTC_SERV* p_server_cb);
const Descriptor* bta_gattc_get_descriptor_srcb(tBTA_GATTC_SERV* p_srcb,
                                                uint16_t handle);
const Characteristic* bta_gattc_get_characteristic_srcb(tBTA_GATTC_SERV* p_srcb,
                                                        uint16_t handle);
static void bta_gattc_explore_srvc_finished(uint16_t conn_id,
                                            tBTA_GATTC_SERV* p_srvc_cb);

#define BTA_GATT_SDP_DB_SIZE 4096

#define GATT_CACHE_PREFIX "/data/misc/bluetooth/gatt_cache_"
#define GATT_CACHE_VERSION 5

static void bta_gattc_generate_cache_file_name(char* buffer, size_t buffer_len,
                                               const RawAddress& bda) {
  snprintf(buffer, buffer_len, "%s%02x%02x%02x%02x%02x%02x", GATT_CACHE_PREFIX,
           bda.address[0], bda.address[1], bda.address[2], bda.address[3],
           bda.address[4], bda.address[5]);
}

/*****************************************************************************
 *  Constants and data types
 ****************************************************************************/

typedef struct {
  tSDP_DISCOVERY_DB* p_sdp_db;
  uint16_t sdp_conn_id;
} tBTA_GATTC_CB_DATA;

#if (BTA_GATT_DEBUG == TRUE)
/* utility functions */

/* debug function to display the server cache */
static void bta_gattc_display_cache_server(const Database& database) {
  LOG(INFO) << "<================Start Server Cache =============>";
  std::istringstream iss(database.ToString());
  for (std::string line; std::getline(iss, line);) {
    LOG(INFO) << line;
  }
  LOG(INFO) << "<================End Server Cache =============>";
}

/** debug function to display the exploration list */
static void bta_gattc_display_explore_record(const DatabaseBuilder& database) {
  LOG(INFO) << "<================Start Explore Queue =============>";
  std::istringstream iss(database.ToString());
  for (std::string line; std::getline(iss, line);) {
    LOG(INFO) << line;
  }
  LOG(INFO) << "<================ End Explore Queue =============>";
}
#endif /* BTA_GATT_DEBUG == TRUE */

/** Initialize the database cache and discovery related resources */
void bta_gattc_init_cache(tBTA_GATTC_SERV* p_srvc_cb) {
  p_srvc_cb->gatt_database = gatt::Database();
  p_srvc_cb->pending_discovery.Clear();
}

const Service* bta_gattc_find_matching_service(
    const std::list<Service>& services, uint16_t handle) {
  for (const Service& service : services) {
    if (handle >= service.handle && handle <= service.end_handle)
      return &service;
  }

  return nullptr;
}

/** Start primary service discovery */
tGATT_STATUS bta_gattc_discover_pri_service(uint16_t conn_id,
                                            tBTA_GATTC_SERV* p_server_cb,
                                            uint8_t disc_type) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);
  if (!p_clcb) return GATT_ERROR;

  if (p_clcb->transport == BTA_TRANSPORT_LE) {
    return GATTC_Discover(conn_id, disc_type, 0x0001, 0xFFFF);
  }

  // only for Classic transport
  return bta_gattc_sdp_service_disc(conn_id, p_server_cb);
}

/** start exploring next service, or finish discovery if no more services left
 */
static void bta_gattc_explore_next_service(uint16_t conn_id,
                                           tBTA_GATTC_SERV* p_srvc_cb) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);
  if (!p_clcb) {
    LOG(ERROR) << "unknown conn_id=" << loghex(conn_id);
    return;
  }

  if (!p_srvc_cb->pending_discovery.StartNextServiceExploration()) {
    bta_gattc_explore_srvc_finished(conn_id, p_srvc_cb);
    return;
  }

  const auto& service = p_srvc_cb->pending_discovery.CurrentlyExploredService();
  VLOG(1) << "Start service discovery";

  /* start discovering included services */
  GATTC_Discover(conn_id, GATT_DISC_INC_SRVC, service.first, service.second);
}

static void bta_gattc_explore_srvc_finished(uint16_t conn_id,
                                            tBTA_GATTC_SERV* p_srvc_cb) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);
  if (!p_clcb) {
    LOG(ERROR) << "unknown conn_id=" << loghex(conn_id);
    return;
  }

  /* no service found at all, the end of server discovery*/
  LOG(INFO) << __func__ << ": service discovery finished";

  p_srvc_cb->gatt_database = p_srvc_cb->pending_discovery.Build();

#if (BTA_GATT_DEBUG == TRUE)
  bta_gattc_display_cache_server(p_srvc_cb->gatt_database);
#endif
  /* save cache to NV */
  p_clcb->p_srcb->state = BTA_GATTC_SERV_SAVE;

  if (btm_sec_is_a_bonded_dev(p_srvc_cb->server_bda)) {
    bta_gattc_cache_write(p_clcb->p_srcb->server_bda,
                          p_clcb->p_srcb->gatt_database.Serialize());
  }

  bta_gattc_reset_discover_st(p_clcb->p_srcb, GATT_SUCCESS);
}

/** Start discovery for characteristic descriptor */
void bta_gattc_start_disc_char_dscp(uint16_t conn_id,
                                    tBTA_GATTC_SERV* p_srvc_cb) {
  VLOG(1) << "starting discover characteristics descriptor";

  std::pair<uint16_t, uint16_t> range =
      p_srvc_cb->pending_discovery.NextDescriptorRangeToExplore();
  if (range == DatabaseBuilder::EXPLORE_END) {
    goto descriptor_discovery_done;
  }

  if (GATTC_Discover(conn_id, GATT_DISC_CHAR_DSCPT, range.first,
                     range.second) != 0) {
    goto descriptor_discovery_done;
  }
  return;

descriptor_discovery_done:
  /* all characteristic has been explored, start with next service if any */
  DVLOG(3) << "all characteristics explored";

  bta_gattc_explore_next_service(conn_id, p_srvc_cb);
  return;
}

/* Process the discovery result from sdp */
void bta_gattc_sdp_callback(uint16_t sdp_status, void* user_data) {
  tBTA_GATTC_CB_DATA* cb_data = (tBTA_GATTC_CB_DATA*)user_data;
  tBTA_GATTC_SERV* p_srvc_cb = bta_gattc_find_scb_by_cid(cb_data->sdp_conn_id);

  if (p_srvc_cb == nullptr) {
    LOG(ERROR) << "GATT service discovery is done on unknown connection";
    /* allocated in bta_gattc_sdp_service_disc */
    osi_free(cb_data);
    return;
  }

  if ((sdp_status != SDP_SUCCESS) && (sdp_status != SDP_DB_FULL)) {
    bta_gattc_explore_srvc_finished(cb_data->sdp_conn_id, p_srvc_cb);

    /* allocated in bta_gattc_sdp_service_disc */
    osi_free(cb_data);
    return;
  }

  bool no_pending_disc = !p_srvc_cb->pending_discovery.InProgress();

  tSDP_DISC_REC* p_sdp_rec = SDP_FindServiceInDb(cb_data->p_sdp_db, 0, nullptr);
  while (p_sdp_rec != nullptr) {
    /* find a service record, report it */
    Uuid service_uuid;
    if (!SDP_FindServiceUUIDInRec(p_sdp_rec, &service_uuid)) continue;

    tSDP_PROTOCOL_ELEM pe;
    if (!SDP_FindProtocolListElemInRec(p_sdp_rec, UUID_PROTOCOL_ATT, &pe))
      continue;

    uint16_t start_handle = (uint16_t)pe.params[0];
    uint16_t end_handle = (uint16_t)pe.params[1];

#if (BTA_GATT_DEBUG == TRUE)
    VLOG(1) << "Found ATT service uuid=" << service_uuid
            << ", s_handle=" << loghex(start_handle)
            << ", e_handle=" << loghex(end_handle);
#endif

    if (!GATT_HANDLE_IS_VALID(start_handle) ||
        !GATT_HANDLE_IS_VALID(end_handle)) {
      LOG(ERROR) << "invalid start_handle=" << loghex(start_handle)
                 << ", end_handle=" << loghex(end_handle);
      continue;
    }

    /* discover services result, add services into a service list */
    p_srvc_cb->pending_discovery.AddService(start_handle, end_handle,
                                            service_uuid, true);

    p_sdp_rec = SDP_FindServiceInDb(cb_data->p_sdp_db, 0, p_sdp_rec);
  }

  // If discovery is already pending, no need to call
  // bta_gattc_explore_next_service. Next service will be picked up to discovery
  // once current one is discovered. If discovery is not pending, start one
  if (no_pending_disc) {
    bta_gattc_explore_next_service(cb_data->sdp_conn_id, p_srvc_cb);
  }

  /* allocated in bta_gattc_sdp_service_disc */
  osi_free(cb_data);
}

/* Start DSP Service Discovery */
static tGATT_STATUS bta_gattc_sdp_service_disc(uint16_t conn_id,
                                               tBTA_GATTC_SERV* p_server_cb) {
  uint16_t num_attrs = 2;
  uint16_t attr_list[2];

  /*
   * On success, cb_data will be freed inside bta_gattc_sdp_callback,
   * otherwise it will be freed within this function.
   */
  tBTA_GATTC_CB_DATA* cb_data = (tBTA_GATTC_CB_DATA*)osi_malloc(
      sizeof(tBTA_GATTC_CB_DATA) + BTA_GATT_SDP_DB_SIZE);

  cb_data->p_sdp_db = (tSDP_DISCOVERY_DB*)(cb_data + 1);
  attr_list[0] = ATTR_ID_SERVICE_CLASS_ID_LIST;
  attr_list[1] = ATTR_ID_PROTOCOL_DESC_LIST;

  Uuid uuid = Uuid::From16Bit(UUID_PROTOCOL_ATT);
  SDP_InitDiscoveryDb(cb_data->p_sdp_db, BTA_GATT_SDP_DB_SIZE, 1, &uuid,
                      num_attrs, attr_list);

  if (!SDP_ServiceSearchAttributeRequest2(p_server_cb->server_bda,
                                          cb_data->p_sdp_db,
                                          &bta_gattc_sdp_callback, cb_data)) {
    osi_free(cb_data);
    return GATT_ERROR;
  }

  cb_data->sdp_conn_id = conn_id;
  return GATT_SUCCESS;
}

/** callback function to GATT client stack */
void bta_gattc_disc_res_cback(uint16_t conn_id, tGATT_DISC_TYPE disc_type,
                              tGATT_DISC_RES* p_data) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);
  tBTA_GATTC_SERV* p_srvc_cb = bta_gattc_find_scb_by_cid(conn_id);

  if (!p_srvc_cb || !p_clcb || p_clcb->state != BTA_GATTC_DISCOVER_ST) return;

  switch (disc_type) {
    case GATT_DISC_SRVC_ALL:
    case GATT_DISC_SRVC_BY_UUID:
      p_srvc_cb->pending_discovery.AddService(
          p_data->handle, p_data->value.group_value.e_handle,
          p_data->value.group_value.service_type, true);
      break;

    case GATT_DISC_INC_SRVC:
      p_srvc_cb->pending_discovery.AddIncludedService(
          p_data->handle, p_data->value.incl_service.service_type,
          p_data->value.incl_service.s_handle,
          p_data->value.incl_service.e_handle);
      break;

    case GATT_DISC_CHAR:
      p_srvc_cb->pending_discovery.AddCharacteristic(
          p_data->handle, p_data->value.dclr_value.val_handle,
          p_data->value.dclr_value.char_uuid,
          p_data->value.dclr_value.char_prop);
      break;

    case GATT_DISC_CHAR_DSCPT:
      p_srvc_cb->pending_discovery.AddDescriptor(p_data->handle, p_data->type);
      break;
  }
}

void bta_gattc_disc_cmpl_cback(uint16_t conn_id, tGATT_DISC_TYPE disc_type,
                               tGATT_STATUS status) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);

  if (p_clcb && (status != GATT_SUCCESS || p_clcb->status != GATT_SUCCESS)) {
    if (status == GATT_SUCCESS) p_clcb->status = status;
    bta_gattc_sm_execute(p_clcb, BTA_GATTC_DISCOVER_CMPL_EVT, NULL);
    return;
  }

  tBTA_GATTC_SERV* p_srvc_cb = bta_gattc_find_scb_by_cid(conn_id);
  if (!p_srvc_cb) return;

  switch (disc_type) {
    case GATT_DISC_SRVC_ALL:
    case GATT_DISC_SRVC_BY_UUID:
// definition of all services are discovered, now it's time to discover
// their content
#if (BTA_GATT_DEBUG == TRUE)
      bta_gattc_display_explore_record(p_srvc_cb->pending_discovery);
#endif
      bta_gattc_explore_next_service(conn_id, p_srvc_cb);
      break;

    case GATT_DISC_INC_SRVC: {
      auto& service = p_srvc_cb->pending_discovery.CurrentlyExploredService();
      /* start discovering characteristic */
      GATTC_Discover(conn_id, GATT_DISC_CHAR, service.first, service.second);
      break;
    }

    case GATT_DISC_CHAR: {
#if (BTA_GATT_DEBUG == TRUE)
      bta_gattc_display_explore_record(p_srvc_cb->pending_discovery);
#endif
      bta_gattc_start_disc_char_dscp(conn_id, p_srvc_cb);
      break;
    }

    case GATT_DISC_CHAR_DSCPT:
      /* start discovering next characteristic for char descriptor */
      bta_gattc_start_disc_char_dscp(conn_id, p_srvc_cb);
      break;
  }
}

/** search local cache for matching service record */
void bta_gattc_search_service(tBTA_GATTC_CLCB* p_clcb, Uuid* p_uuid) {
  for (const Service& service : p_clcb->p_srcb->gatt_database.Services()) {
    if (p_uuid && *p_uuid != service.uuid) continue;

#if (BTA_GATT_DEBUG == TRUE)
    VLOG(1) << __func__ << "found service " << service.uuid
            << " handle:" << +service.handle;
#endif
    if (!p_clcb->p_rcb->p_cback) continue;

    tBTA_GATTC cb_data;
    memset(&cb_data, 0, sizeof(tBTA_GATTC));
    cb_data.srvc_res.conn_id = p_clcb->bta_conn_id;
    cb_data.srvc_res.service_uuid.inst_id = service.handle;
    cb_data.srvc_res.service_uuid.uuid = service.uuid;

    (*p_clcb->p_rcb->p_cback)(BTA_GATTC_SEARCH_RES_EVT, &cb_data);
  }
}

const std::list<Service>* bta_gattc_get_services_srcb(tBTA_GATTC_SERV* p_srcb) {
  if (!p_srcb || p_srcb->gatt_database.IsEmpty()) return NULL;

  return &p_srcb->gatt_database.Services();
}

const std::list<Service>* bta_gattc_get_services(uint16_t conn_id) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);

  if (p_clcb == NULL) return NULL;

  tBTA_GATTC_SERV* p_srcb = p_clcb->p_srcb;

  return bta_gattc_get_services_srcb(p_srcb);
}

const Service* bta_gattc_get_service_for_handle_srcb(tBTA_GATTC_SERV* p_srcb,
                                                     uint16_t handle) {
  const std::list<Service>* services = bta_gattc_get_services_srcb(p_srcb);
  if (services == NULL) return NULL;
  return bta_gattc_find_matching_service(*services, handle);
}

const Service* bta_gattc_get_service_for_handle(uint16_t conn_id,
                                                uint16_t handle) {
  const std::list<Service>* services = bta_gattc_get_services(conn_id);
  if (services == NULL) return NULL;

  return bta_gattc_find_matching_service(*services, handle);
}

const Characteristic* bta_gattc_get_characteristic_srcb(tBTA_GATTC_SERV* p_srcb,
                                                        uint16_t handle) {
  const Service* service =
      bta_gattc_get_service_for_handle_srcb(p_srcb, handle);

  if (!service) return NULL;

  for (const Characteristic& charac : service->characteristics) {
    if (handle == charac.value_handle) return &charac;
  }

  return NULL;
}

const Characteristic* bta_gattc_get_characteristic(uint16_t conn_id,
                                                   uint16_t handle) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);

  if (p_clcb == NULL) return NULL;

  tBTA_GATTC_SERV* p_srcb = p_clcb->p_srcb;
  return bta_gattc_get_characteristic_srcb(p_srcb, handle);
}

const Descriptor* bta_gattc_get_descriptor_srcb(tBTA_GATTC_SERV* p_srcb,
                                                uint16_t handle) {
  const Service* service =
      bta_gattc_get_service_for_handle_srcb(p_srcb, handle);

  if (!service) {
    return NULL;
  }

  for (const Characteristic& charac : service->characteristics) {
    for (const Descriptor& desc : charac.descriptors) {
      if (handle == desc.handle) return &desc;
    }
  }

  return NULL;
}

const Descriptor* bta_gattc_get_descriptor(uint16_t conn_id, uint16_t handle) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);

  if (p_clcb == NULL) return NULL;

  tBTA_GATTC_SERV* p_srcb = p_clcb->p_srcb;
  return bta_gattc_get_descriptor_srcb(p_srcb, handle);
}

const Characteristic* bta_gattc_get_owning_characteristic_srcb(
    tBTA_GATTC_SERV* p_srcb, uint16_t handle) {
  const Service* service =
      bta_gattc_get_service_for_handle_srcb(p_srcb, handle);

  if (!service) return NULL;

  for (const Characteristic& charac : service->characteristics) {
    for (const Descriptor& desc : charac.descriptors) {
      if (handle == desc.handle) return &charac;
    }
  }

  return NULL;
}

const Characteristic* bta_gattc_get_owning_characteristic(uint16_t conn_id,
                                                          uint16_t handle) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);
  if (!p_clcb) return NULL;

  return bta_gattc_get_owning_characteristic_srcb(p_clcb->p_srcb, handle);
}

/*******************************************************************************
 *
 * Function         bta_gattc_fill_gatt_db_el
 *
 * Description      fill a btgatt_db_element_t value
 *
 * Returns          None.
 *
 ******************************************************************************/
void bta_gattc_fill_gatt_db_el(btgatt_db_element_t* p_attr,
                               bt_gatt_db_attribute_type_t type,
                               uint16_t att_handle, uint16_t s_handle,
                               uint16_t e_handle, uint16_t id, const Uuid& uuid,
                               uint8_t prop) {
  p_attr->type = type;
  p_attr->attribute_handle = att_handle;
  p_attr->start_handle = s_handle;
  p_attr->end_handle = e_handle;
  p_attr->id = id;
  p_attr->properties = prop;

  // Permissions are not discoverable using the attribute protocol.
  // Core 5.0, Part F, 3.2.5 Attribute Permissions
  p_attr->permissions = 0;
  p_attr->uuid = uuid;
}

/*******************************************************************************
 * Returns          number of elements inside db from start_handle to end_handle
 ******************************************************************************/
static size_t bta_gattc_get_db_size(const std::list<Service>& services,
                                    uint16_t start_handle,
                                    uint16_t end_handle) {
  if (services.empty()) return 0;

  size_t db_size = 0;

  for (const Service& service : services) {
    if (service.handle < start_handle) continue;

    if (service.end_handle > end_handle) break;

    db_size++;

    for (const Characteristic& charac : service.characteristics) {
      db_size++;

      db_size += charac.descriptors.size();
    }

    db_size += service.included_services.size();
  }

  return db_size;
}

/*******************************************************************************
 *
 * Function         bta_gattc_get_gatt_db_impl
 *
 * Description      copy the server GATT database into db parameter.
 *
 * Parameters       p_srvc_cb: server.
 *                  db: output parameter which will contain GATT database copy.
 *                      Caller is responsible for freeing it.
 *                  count: output parameter which will contain number of
 *                  elements in database.
 *
 * Returns          None.
 *
 ******************************************************************************/
static void bta_gattc_get_gatt_db_impl(tBTA_GATTC_SERV* p_srvc_cb,
                                       uint16_t start_handle,
                                       uint16_t end_handle,
                                       btgatt_db_element_t** db, int* count) {
  VLOG(1) << __func__
          << StringPrintf(": start_handle 0x%04x, end_handle 0x%04x",
                          start_handle, end_handle);

  if (p_srvc_cb->gatt_database.IsEmpty()) {
    *count = 0;
    *db = NULL;
    return;
  }

  size_t db_size = bta_gattc_get_db_size(p_srvc_cb->gatt_database.Services(),
                                         start_handle, end_handle);

  void* buffer = osi_malloc(db_size * sizeof(btgatt_db_element_t));
  btgatt_db_element_t* curr_db_attr = (btgatt_db_element_t*)buffer;

  for (const Service& service : p_srvc_cb->gatt_database.Services()) {
    if (service.handle < start_handle) continue;

    if (service.end_handle > end_handle) break;

    bta_gattc_fill_gatt_db_el(curr_db_attr,
                              service.is_primary ? BTGATT_DB_PRIMARY_SERVICE
                                                 : BTGATT_DB_SECONDARY_SERVICE,
                              0 /* att_handle */, service.handle,
                              service.end_handle, service.handle, service.uuid,
                              0 /* prop */);
    curr_db_attr++;

    for (const Characteristic& charac : service.characteristics) {
      bta_gattc_fill_gatt_db_el(curr_db_attr, BTGATT_DB_CHARACTERISTIC,
                                charac.value_handle, 0 /* s_handle */,
                                0 /* e_handle */, charac.value_handle,
                                charac.uuid, charac.properties);
      curr_db_attr++;

      for (const Descriptor& desc : charac.descriptors) {
        bta_gattc_fill_gatt_db_el(
            curr_db_attr, BTGATT_DB_DESCRIPTOR, desc.handle, 0 /* s_handle */,
            0 /* e_handle */, desc.handle, desc.uuid, 0 /* property */);
        curr_db_attr++;
      }
    }

    for (const IncludedService& p_isvc : service.included_services) {
      bta_gattc_fill_gatt_db_el(curr_db_attr, BTGATT_DB_INCLUDED_SERVICE,
                                p_isvc.handle, p_isvc.start_handle,
                                0 /* e_handle */, p_isvc.handle, p_isvc.uuid,
                                0 /* property */);
      curr_db_attr++;
    }
  }

  *db = (btgatt_db_element_t*)buffer;
  *count = db_size;
}

/*******************************************************************************
 *
 * Function         bta_gattc_get_gatt_db
 *
 * Description      copy the server GATT database into db parameter.
 *
 * Parameters       conn_id: connection ID which identify the server.
 *                  db: output parameter which will contain GATT database copy.
 *                      Caller is responsible for freeing it.
 *                  count: number of elements in database.
 *
 * Returns          None.
 *
 ******************************************************************************/
void bta_gattc_get_gatt_db(uint16_t conn_id, uint16_t start_handle,
                           uint16_t end_handle, btgatt_db_element_t** db,
                           int* count) {
  tBTA_GATTC_CLCB* p_clcb = bta_gattc_find_clcb_by_conn_id(conn_id);

  LOG_DEBUG(LOG_TAG, "%s", __func__);
  if (p_clcb == NULL) {
    LOG(ERROR) << "Unknown conn_id=" << loghex(conn_id);
    return;
  }

  if (p_clcb->state != BTA_GATTC_CONN_ST) {
    LOG(ERROR) << "server cache not available, CLCB state=" << +p_clcb->state;
    return;
  }

  if (!p_clcb->p_srcb || p_clcb->p_srcb->pending_discovery.InProgress() ||
      p_clcb->p_srcb->gatt_database.IsEmpty()) {
    LOG(ERROR) << "No server cache available";
    return;
  }

  bta_gattc_get_gatt_db_impl(p_clcb->p_srcb, start_handle, end_handle, db,
                             count);
}

/*******************************************************************************
 *
 * Function         bta_gattc_cache_load
 *
 * Description      Load GATT cache from storage for server.
 *
 * Parameter        p_srcb: pointer to server cache, that will
 *                          be filled from storage
 * Returns          true on success, false otherwise
 *
 ******************************************************************************/
bool bta_gattc_cache_load(tBTA_GATTC_SERV* p_srcb) {
  char fname[255] = {0};
  bta_gattc_generate_cache_file_name(fname, sizeof(fname), p_srcb->server_bda);

  FILE* fd = fopen(fname, "rb");
  if (!fd) {
    LOG(ERROR) << __func__ << ": can't open GATT cache file " << fname
               << " for reading, error: " << strerror(errno);
    return false;
  }

  uint16_t cache_ver = 0;
  bool success = false;
  uint16_t num_attr = 0;

  if (fread(&cache_ver, sizeof(uint16_t), 1, fd) != 1) {
    LOG(ERROR) << __func__ << ": can't read GATT cache version from: " << fname;
    goto done;
  }

  if (cache_ver != GATT_CACHE_VERSION) {
    LOG(ERROR) << __func__ << ": wrong GATT cache version: " << fname;
    goto done;
  }

  if (fread(&num_attr, sizeof(uint16_t), 1, fd) != 1) {
    LOG(ERROR) << __func__
               << ": can't read number of GATT attributes: " << fname;
    goto done;
  }

  {
    std::vector<StoredAttribute> attr(num_attr);

    if (fread(attr.data(), sizeof(StoredAttribute), num_attr, fd) != num_attr) {
      LOG(ERROR) << __func__ << "s: can't read GATT attributes: " << fname;
      goto done;
    }

    p_srcb->gatt_database = gatt::Database::Deserialize(attr, &success);
  }

done:
  fclose(fd);
  return success;
}

/*******************************************************************************
 *
 * Function         bta_gattc_cache_write
 *
 * Description      This callout function is executed by GATT when a server
 *                  cache is available to save.
 *
 * Parameter        server_bda: server bd address of this cache belongs to
 *                  attr: attributes to save.
 * Returns
 *
 ******************************************************************************/
static void bta_gattc_cache_write(const RawAddress& server_bda,
                                  const std::vector<StoredAttribute>& attr) {
  char fname[255] = {0};
  bta_gattc_generate_cache_file_name(fname, sizeof(fname), server_bda);

  FILE* fd = fopen(fname, "wb");
  if (!fd) {
    LOG(ERROR) << __func__
               << ": can't open GATT cache file for writing: " << fname;
    return;
  }

  uint16_t cache_ver = GATT_CACHE_VERSION;
  if (fwrite(&cache_ver, sizeof(uint16_t), 1, fd) != 1) {
    LOG(ERROR) << __func__ << ": can't write GATT cache version: " << fname;
    fclose(fd);
    return;
  }

  uint16_t num_attr = attr.size();
  if (fwrite(&num_attr, sizeof(uint16_t), 1, fd) != 1) {
    LOG(ERROR) << __func__
               << ": can't write GATT cache attribute count: " << fname;
    fclose(fd);
    return;
  }

  if (fwrite(attr.data(), sizeof(StoredAttribute), num_attr, fd) != num_attr) {
    LOG(ERROR) << __func__ << ": can't write GATT cache attributes: " << fname;
    fclose(fd);
    return;
  }

  fclose(fd);
}

/*******************************************************************************
 *
 * Function         bta_gattc_cache_reset
 *
 * Description      This callout function is executed by GATTC to reset cache in
 *                  application
 *
 * Parameter        server_bda: server bd address of this cache belongs to
 *
 * Returns          void.
 *
 ******************************************************************************/
void bta_gattc_cache_reset(const RawAddress& server_bda) {
  VLOG(1) << __func__;
  char fname[255] = {0};
  bta_gattc_generate_cache_file_name(fname, sizeof(fname), server_bda);
  unlink(fname);
}
