/******************************************************************************
 *
 *  Copyright 2008-2014 Broadcom Corporation
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
 *  This file contains functions for BLE GAP.
 *
 ******************************************************************************/

#define LOG_TAG "bt_btm_ble"

#include <base/bind.h>
#include <base/callback.h>
#include <base/strings/string_number_conversions.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <vector>

#include "bt_types.h"
#include "bt_utils.h"
#include "btm_ble_api.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "gap_api.h"
#include "hcimsgs.h"
#include "osi/include/osi.h"

#include "advertise_data_parser.h"
#include "btm_ble_int.h"
#include "gatt_int.h"
#include "gattdefs.h"
#include "l2c_int.h"
#include "osi/include/log.h"

#define BTM_BLE_NAME_SHORT 0x01
#define BTM_BLE_NAME_CMPL 0x02

#define BTM_BLE_FILTER_TARGET_UNKNOWN 0xff
#define BTM_BLE_POLICY_UNKNOWN 0xff

#define BTM_EXT_BLE_RMT_NAME_TIMEOUT_MS (30 * 1000)
#define MIN_ADV_LENGTH 2
#define BTM_VSC_CHIP_CAPABILITY_RSP_LEN 9
#define BTM_VSC_CHIP_CAPABILITY_RSP_LEN_L_RELEASE \
  BTM_VSC_CHIP_CAPABILITY_RSP_LEN
#define BTM_VSC_CHIP_CAPABILITY_RSP_LEN_M_RELEASE 15

namespace {

class AdvertisingCache {
 public:
  /* Set the data to |data| for device |addr_type, addr| */
  const std::vector<uint8_t>& Set(uint8_t addr_type, const RawAddress& addr,
                                  std::vector<uint8_t> data) {
    auto it = Find(addr_type, addr);
    if (it != items.end()) {
      it->data = std::move(data);
      return it->data;
    }

    if (items.size() > cache_max) {
      items.pop_back();
    }

    items.emplace_front(addr_type, addr, std::move(data));
    return items.front().data;
  }

  /* Append |data| for device |addr_type, addr| */
  const std::vector<uint8_t>& Append(uint8_t addr_type, const RawAddress& addr,
                                     std::vector<uint8_t> data) {
    auto it = Find(addr_type, addr);
    if (it != items.end()) {
      it->data.insert(it->data.end(), data.begin(), data.end());
      return it->data;
    }

    if (items.size() > cache_max) {
      items.pop_back();
    }

    items.emplace_front(addr_type, addr, std::move(data));
    return items.front().data;
  }

  /* Clear data for device |addr_type, addr| */
  void Clear(uint8_t addr_type, const RawAddress& addr) {
    auto it = Find(addr_type, addr);
    if (it != items.end()) {
      items.erase(it);
    }
  }

 private:
  struct Item {
    uint8_t addr_type;
    RawAddress addr;
    std::vector<uint8_t> data;

    Item(uint8_t addr_type, const RawAddress& addr, std::vector<uint8_t> data)
        : addr_type(addr_type), addr(addr), data(data) {}
  };

  std::list<Item>::iterator Find(uint8_t addr_type, const RawAddress& addr) {
    for (auto it = items.begin(); it != items.end(); it++) {
      if (it->addr_type == addr_type && it->addr == addr) {
        return it;
      }
    }
    return items.end();
  }

  /* we keep maximum 7 devices in the cache */
  const size_t cache_max = 7;
  std::list<Item> items;
};

/* Devices in this cache are waiting for eiter scan response, or chained packets
 * on secondary channel */
AdvertisingCache cache;

}  // namespace

#if (BLE_VND_INCLUDED == TRUE)
static tBTM_BLE_CTRL_FEATURES_CBACK* p_ctrl_le_feature_rd_cmpl_cback = NULL;
#endif

/*******************************************************************************
 *  Local functions
 ******************************************************************************/
static void btm_ble_update_adv_flag(uint8_t flag);
static void btm_ble_process_adv_pkt_cont(
    uint16_t evt_type, uint8_t addr_type, const RawAddress& bda,
    uint8_t primary_phy, uint8_t secondary_phy, uint8_t advertising_sid,
    int8_t tx_power, int8_t rssi, uint16_t periodic_adv_int, uint8_t data_len,
    uint8_t* data);
static uint8_t btm_set_conn_mode_adv_init_addr(tBTM_BLE_INQ_CB* p_cb,
                                               RawAddress& p_peer_addr_ptr,
                                               tBLE_ADDR_TYPE* p_peer_addr_type,
                                               tBLE_ADDR_TYPE* p_own_addr_type);
static void btm_ble_stop_observe(void);
static void btm_ble_fast_adv_timer_timeout(void* data);
static void btm_ble_start_slow_adv(void);
static void btm_ble_inquiry_timer_gap_limited_discovery_timeout(void* data);
static void btm_ble_inquiry_timer_timeout(void* data);
static void btm_ble_observer_timer_timeout(void* data);

#define BTM_BLE_INQ_RESULT 0x01
#define BTM_BLE_OBS_RESULT 0x02

bool ble_evt_type_is_connectable(uint16_t evt_type) {
  return evt_type & (1 << BLE_EVT_CONNECTABLE_BIT);
}

bool ble_evt_type_is_scannable(uint16_t evt_type) {
  return evt_type & (1 << BLE_EVT_SCANNABLE_BIT);
}

bool ble_evt_type_is_directed(uint16_t evt_type) {
  return evt_type & (1 << BLE_EVT_DIRECTED_BIT);
}

bool ble_evt_type_is_scan_resp(uint16_t evt_type) {
  return evt_type & (1 << BLE_EVT_SCAN_RESPONSE_BIT);
}

bool ble_evt_type_is_legacy(uint16_t evt_type) {
  return evt_type & (1 << BLE_EVT_LEGACY_BIT);
}

uint8_t ble_evt_type_data_status(uint16_t evt_type) {
  return (evt_type >> 5) & 3;
}

constexpr uint8_t UNSUPPORTED = 255;

/* LE states combo bit to check */
const uint8_t btm_le_state_combo_tbl[BTM_BLE_STATE_MAX][BTM_BLE_STATE_MAX] = {
    {
        /* single state support */
        HCI_LE_STATES_CONN_ADV_BIT, /* conn_adv */
        HCI_LE_STATES_INIT_BIT,     /* init */
        HCI_LE_STATES_INIT_BIT,     /* master */
        HCI_LE_STATES_SLAVE_BIT,    /* slave */
        UNSUPPORTED,                /* todo: lo du dir adv, not covered ? */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_BIT, /* hi duty dir adv */
        HCI_LE_STATES_NON_CONN_ADV_BIT,    /* non connectable adv */
        HCI_LE_STATES_PASS_SCAN_BIT,       /*  passive scan */
        HCI_LE_STATES_ACTIVE_SCAN_BIT,     /*   active scan */
        HCI_LE_STATES_SCAN_ADV_BIT         /* scanable adv */
    },
    {
        /* conn_adv =0 */
        UNSUPPORTED,                            /* conn_adv */
        HCI_LE_STATES_CONN_ADV_INIT_BIT,        /* init: 32 */
        HCI_LE_STATES_CONN_ADV_MASTER_BIT,      /* master: 35 */
        HCI_LE_STATES_CONN_ADV_SLAVE_BIT,       /* slave: 38,*/
        UNSUPPORTED,                            /* lo du dir adv */
        UNSUPPORTED,                            /* hi duty dir adv */
        UNSUPPORTED,                            /* non connectable adv */
        HCI_LE_STATES_CONN_ADV_PASS_SCAN_BIT,   /*  passive scan */
        HCI_LE_STATES_CONN_ADV_ACTIVE_SCAN_BIT, /*   active scan */
        UNSUPPORTED                             /* scanable adv */
    },
    {
        /* init */
        HCI_LE_STATES_CONN_ADV_INIT_BIT,        /* conn_adv: 32 */
        UNSUPPORTED,                            /* init */
        HCI_LE_STATES_INIT_MASTER_BIT,          /* master 28 */
        HCI_LE_STATES_INIT_MASTER_SLAVE_BIT,    /* slave 41 */
        HCI_LE_STATES_LO_DUTY_DIR_ADV_INIT_BIT, /* lo du dir adv 34 */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_INIT_BIT, /* hi duty dir adv 33 */
        HCI_LE_STATES_NON_CONN_INIT_BIT,        /*  non connectable adv */
        HCI_LE_STATES_PASS_SCAN_INIT_BIT,       /* passive scan */
        HCI_LE_STATES_ACTIVE_SCAN_INIT_BIT,     /*  active scan */
        HCI_LE_STATES_SCAN_ADV_INIT_BIT         /* scanable adv */

    },
    {
        /* master */
        HCI_LE_STATES_CONN_ADV_MASTER_BIT,        /* conn_adv: 35 */
        HCI_LE_STATES_INIT_MASTER_BIT,            /* init 28 */
        HCI_LE_STATES_INIT_MASTER_BIT,            /* master 28 */
        HCI_LE_STATES_CONN_ADV_INIT_BIT,          /* slave: 32 */
        HCI_LE_STATES_LO_DUTY_DIR_ADV_MASTER_BIT, /* lo duty cycle adv 37 */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_MASTER_BIT, /* hi duty cycle adv 36 */
        HCI_LE_STATES_NON_CONN_ADV_MASTER_BIT,    /*  non connectable adv*/
        HCI_LE_STATES_PASS_SCAN_MASTER_BIT,       /*  passive scan */
        HCI_LE_STATES_ACTIVE_SCAN_MASTER_BIT,     /*   active scan */
        HCI_LE_STATES_SCAN_ADV_MASTER_BIT         /*  scanable adv */

    },
    {
        /* slave */
        HCI_LE_STATES_CONN_ADV_SLAVE_BIT,        /* conn_adv: 38,*/
        HCI_LE_STATES_INIT_MASTER_SLAVE_BIT,     /* init 41 */
        HCI_LE_STATES_INIT_MASTER_SLAVE_BIT,     /* master 41 */
        HCI_LE_STATES_CONN_ADV_SLAVE_BIT,        /* slave: 38,*/
        HCI_LE_STATES_LO_DUTY_DIR_ADV_SLAVE_BIT, /* lo duty cycle adv 40 */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_SLAVE_BIT, /* hi duty cycle adv 39 */
        HCI_LE_STATES_NON_CONN_ADV_SLAVE_BIT,    /* non connectable adv */
        HCI_LE_STATES_PASS_SCAN_SLAVE_BIT,       /* passive scan */
        HCI_LE_STATES_ACTIVE_SCAN_SLAVE_BIT,     /*  active scan */
        HCI_LE_STATES_SCAN_ADV_SLAVE_BIT         /* scanable adv */

    },
    {
        /* lo duty cycle adv */
        UNSUPPORTED,                              /* conn_adv: 38,*/
        HCI_LE_STATES_LO_DUTY_DIR_ADV_INIT_BIT,   /* init 34 */
        HCI_LE_STATES_LO_DUTY_DIR_ADV_MASTER_BIT, /* master 37 */
        HCI_LE_STATES_LO_DUTY_DIR_ADV_SLAVE_BIT,  /* slave: 40 */
        UNSUPPORTED,                              /* lo duty cycle adv 40 */
        UNSUPPORTED,                              /* hi duty cycle adv 39 */
        UNSUPPORTED,                              /*  non connectable adv */
        UNSUPPORTED, /* TODO: passive scan, not covered? */
        UNSUPPORTED, /* TODO:  active scan, not covered? */
        UNSUPPORTED  /*  scanable adv */
    },
    {
        /* hi duty cycle adv */
        UNSUPPORTED,                                 /* conn_adv: 38,*/
        HCI_LE_STATES_HI_DUTY_DIR_ADV_INIT_BIT,      /* init 33 */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_MASTER_BIT,    /* master 36 */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_SLAVE_BIT,     /* slave: 39*/
        UNSUPPORTED,                                 /* lo duty cycle adv 40 */
        UNSUPPORTED,                                 /* hi duty cycle adv 39 */
        UNSUPPORTED,                                 /* non connectable adv */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_PASS_SCAN_BIT, /* passive scan */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_ACTIVE_SCAN_BIT, /* active scan */
        UNSUPPORTED                                    /* scanable adv */
    },
    {
        /* non connectable adv */
        UNSUPPORTED,                                /* conn_adv: */
        HCI_LE_STATES_NON_CONN_INIT_BIT,            /* init  */
        HCI_LE_STATES_NON_CONN_ADV_MASTER_BIT,      /* master  */
        HCI_LE_STATES_NON_CONN_ADV_SLAVE_BIT,       /* slave: */
        UNSUPPORTED,                                /* lo duty cycle adv */
        UNSUPPORTED,                                /* hi duty cycle adv */
        UNSUPPORTED,                                /* non connectable adv */
        HCI_LE_STATES_NON_CONN_ADV_PASS_SCAN_BIT,   /* passive scan */
        HCI_LE_STATES_NON_CONN_ADV_ACTIVE_SCAN_BIT, /* active scan */
        UNSUPPORTED                                 /* scanable adv */
    },
    {
        /* passive scan */
        HCI_LE_STATES_CONN_ADV_PASS_SCAN_BIT,        /* conn_adv: */
        HCI_LE_STATES_PASS_SCAN_INIT_BIT,            /* init  */
        HCI_LE_STATES_PASS_SCAN_MASTER_BIT,          /* master  */
        HCI_LE_STATES_PASS_SCAN_SLAVE_BIT,           /* slave: */
        UNSUPPORTED,                                 /* lo duty cycle adv */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_PASS_SCAN_BIT, /* hi duty cycle adv */
        HCI_LE_STATES_NON_CONN_ADV_PASS_SCAN_BIT,    /* non connectable adv */
        UNSUPPORTED,                                 /* passive scan */
        UNSUPPORTED,                                 /* active scan */
        HCI_LE_STATES_SCAN_ADV_PASS_SCAN_BIT         /* scanable adv */
    },
    {
        /* active scan */
        HCI_LE_STATES_CONN_ADV_ACTIVE_SCAN_BIT,        /* conn_adv: */
        HCI_LE_STATES_ACTIVE_SCAN_INIT_BIT,            /* init  */
        HCI_LE_STATES_ACTIVE_SCAN_MASTER_BIT,          /* master  */
        HCI_LE_STATES_ACTIVE_SCAN_SLAVE_BIT,           /* slave: */
        UNSUPPORTED,                                   /* lo duty cycle adv */
        HCI_LE_STATES_HI_DUTY_DIR_ADV_ACTIVE_SCAN_BIT, /* hi duty cycle adv */
        HCI_LE_STATES_NON_CONN_ADV_ACTIVE_SCAN_BIT, /*  non connectable adv */
        UNSUPPORTED,                                /* TODO: passive scan */
        UNSUPPORTED,                                /* TODO:  active scan */
        HCI_LE_STATES_SCAN_ADV_ACTIVE_SCAN_BIT      /*  scanable adv */
    },
    {
        /* scanable adv */
        UNSUPPORTED,                            /* conn_adv: */
        HCI_LE_STATES_SCAN_ADV_INIT_BIT,        /* init  */
        HCI_LE_STATES_SCAN_ADV_MASTER_BIT,      /* master  */
        HCI_LE_STATES_SCAN_ADV_SLAVE_BIT,       /* slave: */
        UNSUPPORTED,                            /* lo duty cycle adv */
        UNSUPPORTED,                            /* hi duty cycle adv */
        UNSUPPORTED,                            /* non connectable adv */
        HCI_LE_STATES_SCAN_ADV_PASS_SCAN_BIT,   /*  passive scan */
        HCI_LE_STATES_SCAN_ADV_ACTIVE_SCAN_BIT, /*  active scan */
        UNSUPPORTED                             /* scanable adv */
    }};

/* check LE combo state supported */
inline bool BTM_LE_STATES_SUPPORTED(const uint8_t* x, uint8_t bit_num) {
  uint8_t mask = 1 << (bit_num % 8);
  uint8_t offset = bit_num / 8;
  return ((x)[offset] & mask);
}

/*******************************************************************************
 *
 * Function         BTM_BleUpdateAdvFilterPolicy
 *
 * Description      This function update the filter policy of advertiser.
 *
 * Parameter        adv_policy: advertising filter policy
 *
 * Return           void
 ******************************************************************************/
void BTM_BleUpdateAdvFilterPolicy(tBTM_BLE_AFP adv_policy) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;
  tBLE_ADDR_TYPE init_addr_type = BLE_ADDR_PUBLIC;
  RawAddress adv_address = RawAddress::kEmpty;
  uint8_t adv_mode = p_cb->adv_mode;

  BTM_TRACE_EVENT("BTM_BleUpdateAdvFilterPolicy");

  if (!controller_get_interface()->supports_ble()) return;

  if (p_cb->afp != adv_policy) {
    p_cb->afp = adv_policy;

    /* if adv active, stop and restart */
    btm_ble_stop_adv();

    if (p_cb->connectable_mode & BTM_BLE_CONNECTABLE)
      p_cb->evt_type = btm_set_conn_mode_adv_init_addr(
          p_cb, adv_address, &init_addr_type, &p_cb->adv_addr_type);

    btsnd_hcic_ble_write_adv_params(
        (uint16_t)(p_cb->adv_interval_min ? p_cb->adv_interval_min
                                          : BTM_BLE_GAP_ADV_SLOW_INT),
        (uint16_t)(p_cb->adv_interval_max ? p_cb->adv_interval_max
                                          : BTM_BLE_GAP_ADV_SLOW_INT),
        p_cb->evt_type, p_cb->adv_addr_type, init_addr_type, adv_address,
        p_cb->adv_chnl_map, p_cb->afp);

    if (adv_mode == BTM_BLE_ADV_ENABLE) btm_ble_start_adv();
  }
}

/*******************************************************************************
 *
 * Function         BTM_BleObserve
 *
 * Description      This procedure keep the device listening for advertising
 *                  events from a broadcast device.
 *
 * Parameters       start: start or stop observe.
 *                  white_list: use white list in observer mode or not.
 *
 * Returns          void
 *
 ******************************************************************************/
tBTM_STATUS BTM_BleObserve(bool start, uint8_t duration,
                           tBTM_INQ_RESULTS_CB* p_results_cb,
                           tBTM_CMPL_CB* p_cmpl_cb) {
  tBTM_BLE_INQ_CB* p_inq = &btm_cb.ble_ctr_cb.inq_var;
  tBTM_STATUS status = BTM_WRONG_MODE;

  uint32_t scan_interval =
      !p_inq->scan_interval ? BTM_BLE_GAP_DISC_SCAN_INT : p_inq->scan_interval;
  uint32_t scan_window =
      !p_inq->scan_window ? BTM_BLE_GAP_DISC_SCAN_WIN : p_inq->scan_window;

  BTM_TRACE_EVENT("%s : scan_type:%d, %d, %d", __func__,
                  btm_cb.btm_inq_vars.scan_type, p_inq->scan_interval,
                  p_inq->scan_window);

  if (!controller_get_interface()->supports_ble()) return BTM_ILLEGAL_VALUE;

  if (start) {
    /* shared inquiry database, do not allow observe if any inquiry is active */
    if (BTM_BLE_IS_OBS_ACTIVE(btm_cb.ble_ctr_cb.scan_activity)) {
      BTM_TRACE_ERROR("%s Observe Already Active", __func__);
      return status;
    }

    btm_cb.ble_ctr_cb.p_obs_results_cb = p_results_cb;
    btm_cb.ble_ctr_cb.p_obs_cmpl_cb = p_cmpl_cb;
    status = BTM_CMD_STARTED;

    /* scan is not started */
    if (!BTM_BLE_IS_SCAN_ACTIVE(btm_cb.ble_ctr_cb.scan_activity)) {
      /* allow config of scan type */
      p_inq->scan_type = (p_inq->scan_type == BTM_BLE_SCAN_MODE_NONE)
                             ? BTM_BLE_SCAN_MODE_ACTI
                             : p_inq->scan_type;
/* assume observe always not using white list */
#if (defined BLE_PRIVACY_SPT && BLE_PRIVACY_SPT == TRUE)
      /* enable resolving list */
      btm_ble_enable_resolving_list_for_platform(BTM_BLE_RL_SCAN);
#endif

      btm_send_hci_set_scan_params(
          p_inq->scan_type, (uint16_t)scan_interval, (uint16_t)scan_window,
          btm_cb.ble_ctr_cb.addr_mgnt_cb.own_addr_type, BTM_BLE_DEFAULT_SFP);

      p_inq->scan_duplicate_filter = BTM_BLE_DUPLICATE_DISABLE;
      status = btm_ble_start_scan();
    }

    if (status == BTM_CMD_STARTED) {
      btm_cb.ble_ctr_cb.scan_activity |= BTM_LE_OBSERVE_ACTIVE;
      if (duration != 0) {
        /* start observer timer */
        uint64_t duration_ms = duration * 1000;
        alarm_set_on_mloop(btm_cb.ble_ctr_cb.observer_timer, duration_ms,
                           btm_ble_observer_timer_timeout, NULL);
      }
    }
  } else if (BTM_BLE_IS_OBS_ACTIVE(btm_cb.ble_ctr_cb.scan_activity)) {
    status = BTM_CMD_STARTED;
    btm_ble_stop_observe();
  } else {
    BTM_TRACE_ERROR("%s Observe not active", __func__);
  }

  return status;
}

#if (BLE_VND_INCLUDED == TRUE)
/*******************************************************************************
 *
 * Function         btm_vsc_brcm_features_complete
 *
 * Description      Command Complete callback for HCI_BLE_VENDOR_CAP_OCF
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_ble_vendor_capability_vsc_cmpl_cback(
    tBTM_VSC_CMPL* p_vcs_cplt_params) {
  uint8_t status = 0xFF;
  uint8_t* p;

  BTM_TRACE_DEBUG("%s", __func__);

  /* Check status of command complete event */
  CHECK(p_vcs_cplt_params->opcode == HCI_BLE_VENDOR_CAP_OCF);
  CHECK(p_vcs_cplt_params->param_len > 0);

  p = p_vcs_cplt_params->p_param_buf;
  STREAM_TO_UINT8(status, p);

  if (status != HCI_SUCCESS) {
    BTM_TRACE_DEBUG("%s: Status = 0x%02x (0 is success)", __func__, status);
    return;
  }
  CHECK(p_vcs_cplt_params->param_len > BTM_VSC_CHIP_CAPABILITY_RSP_LEN);
  STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.adv_inst_max, p);
  STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.rpa_offloading, p);
  STREAM_TO_UINT16(btm_cb.cmn_ble_vsc_cb.tot_scan_results_strg, p);
  STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.max_irk_list_sz, p);
  STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.filter_support, p);
  STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.max_filter, p);
  STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.energy_support, p);

  if (p_vcs_cplt_params->param_len >
      BTM_VSC_CHIP_CAPABILITY_RSP_LEN_L_RELEASE) {
    STREAM_TO_UINT16(btm_cb.cmn_ble_vsc_cb.version_supported, p);
  } else {
    btm_cb.cmn_ble_vsc_cb.version_supported = BTM_VSC_CHIP_CAPABILITY_L_VERSION;
  }

  if (btm_cb.cmn_ble_vsc_cb.version_supported >=
      BTM_VSC_CHIP_CAPABILITY_M_VERSION) {
    CHECK(p_vcs_cplt_params->param_len >= BTM_VSC_CHIP_CAPABILITY_RSP_LEN_M_RELEASE);
    STREAM_TO_UINT16(btm_cb.cmn_ble_vsc_cb.total_trackable_advertisers, p);
    STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.extended_scan_support, p);
    STREAM_TO_UINT8(btm_cb.cmn_ble_vsc_cb.debug_logging_supported, p);
  }
  btm_cb.cmn_ble_vsc_cb.values_read = true;

  BTM_TRACE_DEBUG(
      "%s: stat=%d, irk=%d, ADV ins:%d, rpa=%d, ener=%d, ext_scan=%d", __func__,
      status, btm_cb.cmn_ble_vsc_cb.max_irk_list_sz,
      btm_cb.cmn_ble_vsc_cb.adv_inst_max, btm_cb.cmn_ble_vsc_cb.rpa_offloading,
      btm_cb.cmn_ble_vsc_cb.energy_support,
      btm_cb.cmn_ble_vsc_cb.extended_scan_support);

  btm_ble_adv_init();

  if (btm_cb.cmn_ble_vsc_cb.max_filter > 0) btm_ble_adv_filter_init();

#if (BLE_PRIVACY_SPT == TRUE)
  /* VS capability included and non-4.2 device */
  if (btm_cb.cmn_ble_vsc_cb.max_irk_list_sz > 0 &&
      controller_get_interface()->get_ble_resolving_list_max_size() == 0)
    btm_ble_resolving_list_init(btm_cb.cmn_ble_vsc_cb.max_irk_list_sz);
#endif /* (BLE_PRIVACY_SPT == TRUE) */

  if (btm_cb.cmn_ble_vsc_cb.tot_scan_results_strg > 0) btm_ble_batchscan_init();

  if (p_ctrl_le_feature_rd_cmpl_cback != NULL)
    p_ctrl_le_feature_rd_cmpl_cback(status);
}
#endif /* (BLE_VND_INCLUDED == TRUE) */

/*******************************************************************************
 *
 * Function         BTM_BleGetVendorCapabilities
 *
 * Description      This function reads local LE features
 *
 * Parameters       p_cmn_vsc_cb : Locala LE capability structure
 *
 * Returns          void
 *
 ******************************************************************************/
extern void BTM_BleGetVendorCapabilities(tBTM_BLE_VSC_CB* p_cmn_vsc_cb) {
  BTM_TRACE_DEBUG("BTM_BleGetVendorCapabilities");

  if (NULL != p_cmn_vsc_cb) {
    *p_cmn_vsc_cb = btm_cb.cmn_ble_vsc_cb;
  }
}

/******************************************************************************
 *
 * Function         BTM_BleReadControllerFeatures
 *
 * Description      Reads BLE specific controller features
 *
 * Parameters:      tBTM_BLE_CTRL_FEATURES_CBACK : Callback to notify when
 *                  features are read
 *
 * Returns          void
 *
 ******************************************************************************/
#if (BLE_VND_INCLUDED == TRUE)
extern void BTM_BleReadControllerFeatures(
    tBTM_BLE_CTRL_FEATURES_CBACK* p_vsc_cback) {
  if (btm_cb.cmn_ble_vsc_cb.values_read) return;

  BTM_TRACE_DEBUG("BTM_BleReadControllerFeatures");

  p_ctrl_le_feature_rd_cmpl_cback = p_vsc_cback;
  BTM_VendorSpecificCommand(HCI_BLE_VENDOR_CAP_OCF, 0, NULL,
                            btm_ble_vendor_capability_vsc_cmpl_cback);
}
#else
extern void BTM_BleReadControllerFeatures(
    UNUSED_ATTR tBTM_BLE_CTRL_FEATURES_CBACK* p_vsc_cback) {}
#endif

/*******************************************************************************
 *
 * Function         BTM_BleEnableMixedPrivacyMode
 *
 * Description      This function is called to enabled Mixed mode if privacy 1.2
 *                  is applicable in controller.
 *
 * Parameters       mixed_on:  mixed mode to be used or not.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_BleEnableMixedPrivacyMode(bool mixed_on) {
#if (BLE_PRIVACY_SPT == TRUE)
  btm_cb.ble_ctr_cb.mixed_mode = mixed_on;

/* TODO: send VSC to enabled mixed mode */
#endif
}

/*******************************************************************************
 *
 * Function         BTM_BleConfigPrivacy
 *
 * Description      This function is called to enable or disable the privacy in
 *                   LE channel of the local device.
 *
 * Parameters       privacy_mode:  privacy mode on or off.
 *
 * Returns          bool    privacy mode set success; otherwise failed.
 *
 ******************************************************************************/
bool BTM_BleConfigPrivacy(bool privacy_mode) {
#if (BLE_PRIVACY_SPT == TRUE)
  tBTM_BLE_CB* p_cb = &btm_cb.ble_ctr_cb;

  BTM_TRACE_EVENT("%s", __func__);

  /* if LE is not supported, return error */
  if (!controller_get_interface()->supports_ble()) return false;

  tGAP_BLE_ATTR_VALUE gap_ble_attr_value;
  gap_ble_attr_value.addr_resolution = 0;
  if (!privacy_mode) /* if privacy disabled, always use public address */
  {
    p_cb->addr_mgnt_cb.own_addr_type = BLE_ADDR_PUBLIC;
    p_cb->privacy_mode = BTM_PRIVACY_NONE;
  } else /* privacy is turned on*/
  {
    /* always set host random address, used when privacy 1.1 or priavcy 1.2 is
     * disabled */
    p_cb->addr_mgnt_cb.own_addr_type = BLE_ADDR_RANDOM;
    btm_gen_resolvable_private_addr(base::Bind(&btm_gen_resolve_paddr_low));

    /* 4.2 controller only allow privacy 1.2 or mixed mode, resolvable private
     * address in controller */
    if (controller_get_interface()->supports_ble_privacy()) {
      gap_ble_attr_value.addr_resolution = 1;
      /* check vendor specific capability */
      p_cb->privacy_mode =
          btm_cb.ble_ctr_cb.mixed_mode ? BTM_PRIVACY_MIXED : BTM_PRIVACY_1_2;
    } else /* 4.1/4.0 controller */
      p_cb->privacy_mode = BTM_PRIVACY_1_1;
  }

  GAP_BleAttrDBUpdate(GATT_UUID_GAP_CENTRAL_ADDR_RESOL, &gap_ble_attr_value);

  return true;
#else
  return false;
#endif
}

/*******************************************************************************
 *
 * Function          BTM_BleMaxMultiAdvInstanceCount
 *
 * Description        Returns max number of multi adv instances supported by
 *                  controller
 *
 * Returns          Max multi adv instance count
 *
 ******************************************************************************/
extern uint8_t BTM_BleMaxMultiAdvInstanceCount(void) {
  return btm_cb.cmn_ble_vsc_cb.adv_inst_max < BTM_BLE_MULTI_ADV_MAX
             ? btm_cb.cmn_ble_vsc_cb.adv_inst_max
             : BTM_BLE_MULTI_ADV_MAX;
}

/*******************************************************************************
 *
 * Function         BTM_BleLocalPrivacyEnabled
 *
 * Description        Checks if local device supports private address
 *
 * Returns          Return true if local privacy is enabled else false
 *
 ******************************************************************************/
bool BTM_BleLocalPrivacyEnabled(void) {
#if (BLE_PRIVACY_SPT == TRUE)
  return (btm_cb.ble_ctr_cb.privacy_mode != BTM_PRIVACY_NONE);
#else
  return false;
#endif
}

/*******************************************************************************
 *
 * Function         BTM_BleSetConnectableMode
 *
 * Description      This function is called to set BLE connectable mode for a
 *                  peripheral device.
 *
 * Parameters       conn_mode:  directed connectable mode, or non-directed. It
 *                              can be BTM_BLE_CONNECT_EVT,
 *                              BTM_BLE_CONNECT_DIR_EVT or
 *                              BTM_BLE_CONNECT_LO_DUTY_DIR_EVT
 *
 * Returns          BTM_ILLEGAL_VALUE if controller does not support BLE.
 *                  BTM_SUCCESS is status set successfully; otherwise failure.
 *
 ******************************************************************************/
tBTM_STATUS BTM_BleSetConnectableMode(tBTM_BLE_CONN_MODE connectable_mode) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;

  BTM_TRACE_EVENT("%s connectable_mode = %d ", __func__, connectable_mode);
  if (!controller_get_interface()->supports_ble()) return BTM_ILLEGAL_VALUE;

  p_cb->directed_conn = connectable_mode;
  return btm_ble_set_connectability(p_cb->connectable_mode);
}

#if (BLE_PRIVACY_SPT == TRUE)
static bool is_resolving_list_bit_set(void* data, void* context) {
  tBTM_SEC_DEV_REC* p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(data);

  if ((p_dev_rec->ble.in_controller_list & BTM_RESOLVING_LIST_BIT) != 0)
    return false;

  return true;
}
#endif

/*******************************************************************************
 *
 * Function         btm_set_conn_mode_adv_init_addr
 *
 * Description      set initator address type and local address type based on
 *                  adv mode.
 *
 *
 ******************************************************************************/
static uint8_t btm_set_conn_mode_adv_init_addr(
    tBTM_BLE_INQ_CB* p_cb, RawAddress& p_peer_addr_ptr,
    tBLE_ADDR_TYPE* p_peer_addr_type, tBLE_ADDR_TYPE* p_own_addr_type) {
  uint8_t evt_type;
#if (BLE_PRIVACY_SPT == TRUE)
  tBTM_SEC_DEV_REC* p_dev_rec;
#endif

  evt_type =
      (p_cb->connectable_mode == BTM_BLE_NON_CONNECTABLE)
          ? ((p_cb->scan_rsp) ? BTM_BLE_DISCOVER_EVT : BTM_BLE_NON_CONNECT_EVT)
          : BTM_BLE_CONNECT_EVT;

  if (evt_type == BTM_BLE_CONNECT_EVT) {
    evt_type = p_cb->directed_conn;

    if (p_cb->directed_conn == BTM_BLE_CONNECT_DIR_EVT ||
        p_cb->directed_conn == BTM_BLE_CONNECT_LO_DUTY_DIR_EVT) {
#if (BLE_PRIVACY_SPT == TRUE)
      /* for privacy 1.2, convert peer address as static, own address set as ID
       * addr */
      if (btm_cb.ble_ctr_cb.privacy_mode == BTM_PRIVACY_1_2 ||
          btm_cb.ble_ctr_cb.privacy_mode == BTM_PRIVACY_MIXED) {
        /* only do so for bonded device */
        if ((p_dev_rec = btm_find_or_alloc_dev(p_cb->direct_bda.bda)) != NULL &&
            p_dev_rec->ble.in_controller_list & BTM_RESOLVING_LIST_BIT) {
          btm_ble_enable_resolving_list(BTM_BLE_RL_ADV);
          p_peer_addr_ptr = p_dev_rec->ble.identity_addr;
          *p_peer_addr_type = p_dev_rec->ble.identity_addr_type;
          *p_own_addr_type = BLE_ADDR_RANDOM_ID;
          return evt_type;
        }
        /* otherwise fall though as normal directed adv */
        else {
          btm_ble_disable_resolving_list(BTM_BLE_RL_ADV, true);
        }
      }
#endif
      /* direct adv mode does not have privacy, if privacy is not enabled  */
      *p_peer_addr_type = p_cb->direct_bda.type;
      p_peer_addr_ptr = p_cb->direct_bda.bda;
      return evt_type;
    }
  }

/* undirect adv mode or non-connectable mode*/
#if (BLE_PRIVACY_SPT == TRUE)
  /* when privacy 1.2 privacy only mode is used, or mixed mode */
  if ((btm_cb.ble_ctr_cb.privacy_mode == BTM_PRIVACY_1_2 &&
       p_cb->afp != AP_SCAN_CONN_ALL) ||
      btm_cb.ble_ctr_cb.privacy_mode == BTM_PRIVACY_MIXED) {
    list_node_t* n =
        list_foreach(btm_cb.sec_dev_rec, is_resolving_list_bit_set, NULL);
    if (n) {
      /* if enhanced privacy is required, set Identity address and matching IRK
       * peer */
      tBTM_SEC_DEV_REC* p_dev_rec =
          static_cast<tBTM_SEC_DEV_REC*>(list_node(n));
      p_peer_addr_ptr = p_dev_rec->ble.identity_addr;
      *p_peer_addr_type = p_dev_rec->ble.identity_addr_type;

      *p_own_addr_type = BLE_ADDR_RANDOM_ID;
    } else {
      /* resolving list is empty, not enabled */
      *p_own_addr_type = BLE_ADDR_RANDOM;
    }
  }
  /* privacy 1.1, or privacy 1.2, general discoverable/connectable mode, disable
     privacy in */
  /* controller fall back to host based privacy */
  else if (btm_cb.ble_ctr_cb.privacy_mode != BTM_PRIVACY_NONE) {
    *p_own_addr_type = BLE_ADDR_RANDOM;
  }
#endif

  /* if no privacy,do not set any peer address,*/
  /* local address type go by global privacy setting */
  return evt_type;
}

/**
 * This function is called to set scan parameters. |cb| is called with operation
 * status
 **/
void BTM_BleSetScanParams(uint32_t scan_interval, uint32_t scan_window,
                          tBLE_SCAN_MODE scan_mode,
                          base::Callback<void(uint8_t)> cb) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;
  uint32_t max_scan_interval;
  uint32_t max_scan_window;

  BTM_TRACE_EVENT("%s", __func__);
  if (!controller_get_interface()->supports_ble()) return;

  /* If not supporting extended scan support, use the older range for checking
   */
  if (btm_cb.cmn_ble_vsc_cb.extended_scan_support == 0) {
    max_scan_interval = BTM_BLE_SCAN_INT_MAX;
    max_scan_window = BTM_BLE_SCAN_WIN_MAX;
  } else {
    /* If supporting extended scan support, use the new extended range for
     * checking */
    max_scan_interval = BTM_BLE_EXT_SCAN_INT_MAX;
    max_scan_window = BTM_BLE_EXT_SCAN_WIN_MAX;
  }

  if (BTM_BLE_ISVALID_PARAM(scan_interval, BTM_BLE_SCAN_INT_MIN,
                            max_scan_interval) &&
      BTM_BLE_ISVALID_PARAM(scan_window, BTM_BLE_SCAN_WIN_MIN,
                            max_scan_window) &&
      (scan_mode == BTM_BLE_SCAN_MODE_ACTI ||
       scan_mode == BTM_BLE_SCAN_MODE_PASS)) {
    p_cb->scan_type = scan_mode;
    p_cb->scan_interval = scan_interval;
    p_cb->scan_window = scan_window;

    cb.Run(BTM_SUCCESS);
  } else {
    cb.Run(BTM_ILLEGAL_VALUE);

    BTM_TRACE_ERROR("Illegal params: scan_interval = %d scan_window = %d",
                    scan_interval, scan_window);
  }
}

/*******************************************************************************
 *
 * Function         BTM_BleWriteScanRsp
 *
 * Description      This function is called to write LE scan response.
 *
 * Parameters:      p_scan_rsp: scan response information.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_BleWriteScanRsp(uint8_t* data, uint8_t length,
                         tBTM_BLE_ADV_DATA_CMPL_CBACK* p_adv_data_cback) {
  BTM_TRACE_EVENT("%s: length: %d", __func__, length);
  if (!controller_get_interface()->supports_ble()) {
    p_adv_data_cback(BTM_ILLEGAL_VALUE);
    return;
  }

  btsnd_hcic_ble_set_scan_rsp_data(length, data);

  if (length != 0)
    btm_cb.ble_ctr_cb.inq_var.scan_rsp = true;
  else
    btm_cb.ble_ctr_cb.inq_var.scan_rsp = false;

  p_adv_data_cback(BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM__BLEReadDiscoverability
 *
 * Description      This function is called to read the current LE
 *                  discoverability mode of the device.
 *
 * Returns          BTM_BLE_NON_DISCOVERABLE ,BTM_BLE_LIMITED_DISCOVERABLE or
 *                     BTM_BLE_GENRAL_DISCOVERABLE
 *
 ******************************************************************************/
uint16_t BTM_BleReadDiscoverability() {
  BTM_TRACE_API("%s", __func__);

  return (btm_cb.ble_ctr_cb.inq_var.discoverable_mode);
}

/*******************************************************************************
 *
 * Function         BTM__BLEReadConnectability
 *
 * Description      This function is called to read the current LE
 *                  connectability mode of the device.
 *
 * Returns          BTM_BLE_NON_CONNECTABLE or BTM_BLE_CONNECTABLE
 *
 ******************************************************************************/
uint16_t BTM_BleReadConnectability() {
  BTM_TRACE_API("%s", __func__);

  return (btm_cb.ble_ctr_cb.inq_var.connectable_mode);
}

/*******************************************************************************
 *
 * Function         btm_ble_select_adv_interval
 *
 * Description      select adv interval based on device mode
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_select_adv_interval(tBTM_BLE_INQ_CB* p_cb, uint8_t evt_type,
                                 uint16_t* p_adv_int_min,
                                 uint16_t* p_adv_int_max) {
  if (p_cb->adv_interval_min && p_cb->adv_interval_max) {
    *p_adv_int_min = p_cb->adv_interval_min;
    *p_adv_int_max = p_cb->adv_interval_max;
  } else {
    switch (evt_type) {
      case BTM_BLE_CONNECT_EVT:
      case BTM_BLE_CONNECT_LO_DUTY_DIR_EVT:
        *p_adv_int_min = *p_adv_int_max = BTM_BLE_GAP_ADV_FAST_INT_1;
        break;

      case BTM_BLE_NON_CONNECT_EVT:
      case BTM_BLE_DISCOVER_EVT:
        *p_adv_int_min = *p_adv_int_max = BTM_BLE_GAP_ADV_FAST_INT_2;
        break;

      /* connectable directed event */
      case BTM_BLE_CONNECT_DIR_EVT:
        *p_adv_int_min = BTM_BLE_GAP_ADV_DIR_MIN_INT;
        *p_adv_int_max = BTM_BLE_GAP_ADV_DIR_MAX_INT;
        break;

      default:
        *p_adv_int_min = *p_adv_int_max = BTM_BLE_GAP_ADV_SLOW_INT;
        break;
    }
  }
  return;
}

/*******************************************************************************
 *
 * Function         btm_ble_update_dmt_flag_bits
 *
 * Description      Obtain updated adv flag value based on connect and
 *                  discoverability mode. Also, setup DMT support value in the
 *                  flag based on whether the controller supports both LE and
 *                  BR/EDR.
 *
 * Parameters:      flag_value (Input / Output) - flag value
 *                  connect_mode (Input) - Connect mode value
 *                  disc_mode (Input) - discoverability mode
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_update_dmt_flag_bits(uint8_t* adv_flag_value,
                                  const uint16_t connect_mode,
                                  const uint16_t disc_mode) {
  /* BR/EDR non-discoverable , non-connectable */
  if ((disc_mode & BTM_DISCOVERABLE_MASK) == 0 &&
      (connect_mode & BTM_CONNECTABLE_MASK) == 0)
    *adv_flag_value |= BTM_BLE_BREDR_NOT_SPT;
  else
    *adv_flag_value &= ~BTM_BLE_BREDR_NOT_SPT;

  /* if local controller support, mark both controller and host support in flag
   */
  if (controller_get_interface()->supports_simultaneous_le_bredr())
    *adv_flag_value |= (BTM_BLE_DMT_CONTROLLER_SPT | BTM_BLE_DMT_HOST_SPT);
  else
    *adv_flag_value &= ~(BTM_BLE_DMT_CONTROLLER_SPT | BTM_BLE_DMT_HOST_SPT);
}

/*******************************************************************************
 *
 * Function         btm_ble_set_adv_flag
 *
 * Description      Set adv flag in adv data.
 *
 * Parameters:      connect_mode (Input)- Connect mode value
 *                  disc_mode (Input) - discoverability mode
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_set_adv_flag(uint16_t connect_mode, uint16_t disc_mode) {
  uint8_t flag = 0, old_flag = 0;
  tBTM_BLE_LOCAL_ADV_DATA* p_adv_data = &btm_cb.ble_ctr_cb.inq_var.adv_data;

  if (p_adv_data->p_flags != NULL) flag = old_flag = *(p_adv_data->p_flags);

  btm_ble_update_dmt_flag_bits(&flag, connect_mode, disc_mode);

  LOG_DEBUG(LOG_TAG, "disc_mode %04x", disc_mode);
  /* update discoverable flag */
  if (disc_mode & BTM_BLE_LIMITED_DISCOVERABLE) {
    flag &= ~BTM_BLE_GEN_DISC_FLAG;
    flag |= BTM_BLE_LIMIT_DISC_FLAG;
  } else if (disc_mode & BTM_BLE_GENERAL_DISCOVERABLE) {
    flag |= BTM_BLE_GEN_DISC_FLAG;
    flag &= ~BTM_BLE_LIMIT_DISC_FLAG;
  } else /* remove all discoverable flags */
  {
    flag &= ~(BTM_BLE_LIMIT_DISC_FLAG | BTM_BLE_GEN_DISC_FLAG);
  }

  if (flag != old_flag) {
    btm_ble_update_adv_flag(flag);
  }
}
/*******************************************************************************
 *
 * Function         btm_ble_set_discoverability
 *
 * Description      This function is called to set BLE discoverable mode.
 *
 * Parameters:      combined_mode: discoverability mode.
 *
 * Returns          BTM_SUCCESS is status set successfully; otherwise failure.
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_set_discoverability(uint16_t combined_mode) {
  tBTM_LE_RANDOM_CB* p_addr_cb = &btm_cb.ble_ctr_cb.addr_mgnt_cb;
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;
  uint16_t mode = (combined_mode & BTM_BLE_DISCOVERABLE_MASK);
  uint8_t new_mode = BTM_BLE_ADV_ENABLE;
  uint8_t evt_type;
  tBTM_STATUS status = BTM_SUCCESS;
  RawAddress address = RawAddress::kEmpty;
  tBLE_ADDR_TYPE init_addr_type = BLE_ADDR_PUBLIC,
                 own_addr_type = p_addr_cb->own_addr_type;
  uint16_t adv_int_min, adv_int_max;

  BTM_TRACE_EVENT("%s mode=0x%0x combined_mode=0x%x", __func__, mode,
                  combined_mode);

  /*** Check mode parameter ***/
  if (mode > BTM_BLE_MAX_DISCOVERABLE) return (BTM_ILLEGAL_VALUE);

  p_cb->discoverable_mode = mode;

  evt_type = btm_set_conn_mode_adv_init_addr(p_cb, address, &init_addr_type,
                                             &own_addr_type);

  if (p_cb->connectable_mode == BTM_BLE_NON_CONNECTABLE &&
      mode == BTM_BLE_NON_DISCOVERABLE)
    new_mode = BTM_BLE_ADV_DISABLE;

  btm_ble_select_adv_interval(p_cb, evt_type, &adv_int_min, &adv_int_max);

  alarm_cancel(p_cb->fast_adv_timer);

  /* update adv params if start advertising */
  BTM_TRACE_EVENT("evt_type=0x%x p-cb->evt_type=0x%x ", evt_type,
                  p_cb->evt_type);

  if (new_mode == BTM_BLE_ADV_ENABLE) {
    btm_ble_set_adv_flag(btm_cb.btm_inq_vars.connectable_mode, combined_mode);

    if (evt_type != p_cb->evt_type || p_cb->adv_addr_type != own_addr_type ||
        !p_cb->fast_adv_on) {
      btm_ble_stop_adv();

      /* update adv params */
      btsnd_hcic_ble_write_adv_params(adv_int_min, adv_int_max, evt_type,
                                      own_addr_type, init_addr_type, address,
                                      p_cb->adv_chnl_map, p_cb->afp);
      p_cb->evt_type = evt_type;
      p_cb->adv_addr_type = own_addr_type;
    }
  }

  if (status == BTM_SUCCESS && p_cb->adv_mode != new_mode) {
    if (new_mode == BTM_BLE_ADV_ENABLE)
      status = btm_ble_start_adv();
    else
      status = btm_ble_stop_adv();
  }

  if (p_cb->adv_mode == BTM_BLE_ADV_ENABLE) {
    p_cb->fast_adv_on = true;
    /* start initial GAP mode adv timer */
    alarm_set_on_mloop(p_cb->fast_adv_timer, BTM_BLE_GAP_FAST_ADV_TIMEOUT_MS,
                       btm_ble_fast_adv_timer_timeout, NULL);
  } else {
#if (BLE_PRIVACY_SPT == TRUE)
    btm_ble_disable_resolving_list(BTM_BLE_RL_ADV, true);
#endif
  }

  /* set up stop advertising timer */
  if (status == BTM_SUCCESS && mode == BTM_BLE_LIMITED_DISCOVERABLE) {
    BTM_TRACE_EVENT("start timer for limited disc mode duration=%d ms",
                    BTM_BLE_GAP_LIM_TIMEOUT_MS);
    /* start Tgap(lim_timeout) */
    alarm_set_on_mloop(p_cb->inquiry_timer, BTM_BLE_GAP_LIM_TIMEOUT_MS,
                       btm_ble_inquiry_timer_gap_limited_discovery_timeout,
                       NULL);
  }
  return status;
}

/*******************************************************************************
 *
 * Function         btm_ble_set_connectability
 *
 * Description      This function is called to set BLE connectability mode.
 *
 * Parameters:      combined_mode: connectability mode.
 *
 * Returns          BTM_SUCCESS is status set successfully; otherwise failure.
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_set_connectability(uint16_t combined_mode) {
  tBTM_LE_RANDOM_CB* p_addr_cb = &btm_cb.ble_ctr_cb.addr_mgnt_cb;
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;
  uint16_t mode = (combined_mode & BTM_BLE_CONNECTABLE_MASK);
  uint8_t new_mode = BTM_BLE_ADV_ENABLE;
  uint8_t evt_type;
  tBTM_STATUS status = BTM_SUCCESS;
  RawAddress address = RawAddress::kEmpty;
  tBLE_ADDR_TYPE peer_addr_type = BLE_ADDR_PUBLIC,
                 own_addr_type = p_addr_cb->own_addr_type;
  uint16_t adv_int_min, adv_int_max;

  BTM_TRACE_EVENT("%s mode=0x%0x combined_mode=0x%x", __func__, mode,
                  combined_mode);

  /*** Check mode parameter ***/
  if (mode > BTM_BLE_MAX_CONNECTABLE) return (BTM_ILLEGAL_VALUE);

  p_cb->connectable_mode = mode;

  evt_type = btm_set_conn_mode_adv_init_addr(p_cb, address, &peer_addr_type,
                                             &own_addr_type);

  if (mode == BTM_BLE_NON_CONNECTABLE &&
      p_cb->discoverable_mode == BTM_BLE_NON_DISCOVERABLE)
    new_mode = BTM_BLE_ADV_DISABLE;

  btm_ble_select_adv_interval(p_cb, evt_type, &adv_int_min, &adv_int_max);

  alarm_cancel(p_cb->fast_adv_timer);
  /* update adv params if needed */
  if (new_mode == BTM_BLE_ADV_ENABLE) {
    btm_ble_set_adv_flag(combined_mode, btm_cb.btm_inq_vars.discoverable_mode);
    if (p_cb->evt_type != evt_type ||
        p_cb->adv_addr_type != p_addr_cb->own_addr_type || !p_cb->fast_adv_on) {
      btm_ble_stop_adv();

      btsnd_hcic_ble_write_adv_params(adv_int_min, adv_int_max, evt_type,
                                      own_addr_type, peer_addr_type, address,
                                      p_cb->adv_chnl_map, p_cb->afp);
      p_cb->evt_type = evt_type;
      p_cb->adv_addr_type = own_addr_type;
    }
  }

  /* update advertising mode */
  if (status == BTM_SUCCESS && new_mode != p_cb->adv_mode) {
    if (new_mode == BTM_BLE_ADV_ENABLE)
      status = btm_ble_start_adv();
    else
      status = btm_ble_stop_adv();
  }

  if (p_cb->adv_mode == BTM_BLE_ADV_ENABLE) {
    p_cb->fast_adv_on = true;
    /* start initial GAP mode adv timer */
    alarm_set_on_mloop(p_cb->fast_adv_timer, BTM_BLE_GAP_FAST_ADV_TIMEOUT_MS,
                       btm_ble_fast_adv_timer_timeout, NULL);
  } else {
#if (BLE_PRIVACY_SPT == TRUE)
    btm_ble_disable_resolving_list(BTM_BLE_RL_ADV, true);
#endif
  }
  return status;
}

void btm_send_hci_scan_enable(uint8_t enable, uint8_t filter_duplicates) {
  if (controller_get_interface()->supports_ble_extended_advertising()) {
    btsnd_hcic_ble_set_extended_scan_enable(enable, filter_duplicates, 0x0000,
                                            0x0000);
  } else {
    btsnd_hcic_ble_set_scan_enable(enable, filter_duplicates);
  }
}

void btm_send_hci_set_scan_params(uint8_t scan_type, uint16_t scan_int,
                                  uint16_t scan_win, uint8_t addr_type_own,
                                  uint8_t scan_filter_policy) {
  if (controller_get_interface()->supports_ble_extended_advertising()) {
    scanning_phy_cfg phy_cfg;
    phy_cfg.scan_type = scan_type;
    phy_cfg.scan_int = scan_int;
    phy_cfg.scan_win = scan_win;

    btsnd_hcic_ble_set_extended_scan_params(addr_type_own, scan_filter_policy,
                                            1, &phy_cfg);
  } else {
    btsnd_hcic_ble_set_scan_params(scan_type, scan_int, scan_win, addr_type_own,
                                   scan_filter_policy);
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_start_inquiry
 *
 * Description      This function is called to start BLE inquiry procedure.
 *                  If the duration is zero, the periodic inquiry mode is
 *                  cancelled.
 *
 * Parameters:      mode - GENERAL or LIMITED inquiry
 *                  p_inq_params - pointer to the BLE inquiry parameter.
 *                  p_results_cb - callback returning pointer to results
 *                                 (tBTM_INQ_RESULTS)
 *                  p_cmpl_cb - callback indicating the end of an inquiry
 *
 *
 *
 * Returns          BTM_CMD_STARTED if successfully started
 *                  BTM_NO_RESOURCES if could not allocate a message buffer
 *                  BTM_BUSY - if an inquiry is already active
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_start_inquiry(uint8_t mode, uint8_t duration) {
  tBTM_STATUS status = BTM_CMD_STARTED;
  tBTM_BLE_CB* p_ble_cb = &btm_cb.ble_ctr_cb;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  BTM_TRACE_DEBUG("btm_ble_start_inquiry: mode = %02x inq_active = 0x%02x",
                  mode, btm_cb.btm_inq_vars.inq_active);

  /* if selective connection is active, or inquiry is already active, reject it
   */
  if (BTM_BLE_IS_INQ_ACTIVE(p_ble_cb->scan_activity)) {
    BTM_TRACE_ERROR("LE Inquiry is active, can not start inquiry");
    return (BTM_BUSY);
  }

  if (!BTM_BLE_IS_SCAN_ACTIVE(p_ble_cb->scan_activity)) {
    btm_send_hci_set_scan_params(
        BTM_BLE_SCAN_MODE_ACTI, BTM_BLE_LOW_LATENCY_SCAN_INT,
        BTM_BLE_LOW_LATENCY_SCAN_WIN,
        btm_cb.ble_ctr_cb.addr_mgnt_cb.own_addr_type, SP_ADV_ALL);
#if (BLE_PRIVACY_SPT == TRUE)
    /* enable IRK list */
    btm_ble_enable_resolving_list_for_platform(BTM_BLE_RL_SCAN);
#endif
    p_ble_cb->inq_var.scan_duplicate_filter = BTM_BLE_DUPLICATE_DISABLE;
    status = btm_ble_start_scan();
  } else if ((p_ble_cb->inq_var.scan_interval !=
              BTM_BLE_LOW_LATENCY_SCAN_INT) ||
             (p_ble_cb->inq_var.scan_window != BTM_BLE_LOW_LATENCY_SCAN_WIN)) {
    BTM_TRACE_DEBUG("%s, restart LE scan with low latency scan params",
                    __func__);
    btm_send_hci_scan_enable(BTM_BLE_SCAN_DISABLE, BTM_BLE_DUPLICATE_ENABLE);
    btm_send_hci_set_scan_params(
        BTM_BLE_SCAN_MODE_ACTI, BTM_BLE_LOW_LATENCY_SCAN_INT,
        BTM_BLE_LOW_LATENCY_SCAN_WIN,
        btm_cb.ble_ctr_cb.addr_mgnt_cb.own_addr_type, SP_ADV_ALL);
    btm_send_hci_scan_enable(BTM_BLE_SCAN_ENABLE, BTM_BLE_DUPLICATE_DISABLE);
  }

  if (status == BTM_CMD_STARTED) {
    p_inq->inq_active |= mode;
    p_ble_cb->scan_activity |= mode;

    BTM_TRACE_DEBUG("btm_ble_start_inquiry inq_active = 0x%02x",
                    p_inq->inq_active);

    if (duration != 0) {
      /* start inquiry timer */
      uint64_t duration_ms = duration * 1000;
      alarm_set_on_mloop(p_ble_cb->inq_var.inquiry_timer, duration_ms,
                         btm_ble_inquiry_timer_timeout, NULL);
    }
  }

  return status;
}

/*******************************************************************************
 *
 * Function         btm_ble_read_remote_name_cmpl
 *
 * Description      This function is called when BLE remote name is received.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_read_remote_name_cmpl(bool status, const RawAddress& bda,
                                   uint16_t length, char* p_name) {
  uint8_t hci_status = HCI_SUCCESS;
  BD_NAME bd_name;

  memset(bd_name, 0, (BD_NAME_LEN + 1));
  if (length > BD_NAME_LEN) {
    length = BD_NAME_LEN;
  }
  memcpy((uint8_t*)bd_name, p_name, length);

  if ((!status) || (length == 0)) {
    hci_status = HCI_ERR_HOST_TIMEOUT;
  }

  btm_process_remote_name(&bda, bd_name, length + 1, hci_status);
  btm_sec_rmt_name_request_complete(&bda, (uint8_t*)p_name, hci_status);
}

/*******************************************************************************
 *
 * Function         btm_ble_read_remote_name
 *
 * Description      This function read remote LE device name using GATT read
 *                  procedure.
 *
 * Parameters:       None.
 *
 * Returns          void
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_read_remote_name(const RawAddress& remote_bda,
                                     tBTM_CMPL_CB* p_cb) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  if (!controller_get_interface()->supports_ble()) return BTM_ERR_PROCESSING;

  tINQ_DB_ENT* p_i = btm_inq_db_find(remote_bda);
  if (p_i && !ble_evt_type_is_connectable(p_i->inq_info.results.ble_evt_type)) {
    BTM_TRACE_DEBUG("name request to non-connectable device failed.");
    return BTM_ERR_PROCESSING;
  }

  /* read remote device name using GATT procedure */
  if (p_inq->remname_active) return BTM_BUSY;

  if (!GAP_BleReadPeerDevName(remote_bda, btm_ble_read_remote_name_cmpl))
    return BTM_BUSY;

  p_inq->p_remname_cmpl_cb = p_cb;
  p_inq->remname_active = true;
  p_inq->remname_bda = remote_bda;

  alarm_set_on_mloop(p_inq->remote_name_timer, BTM_EXT_BLE_RMT_NAME_TIMEOUT_MS,
                     btm_inq_remote_name_timer_timeout, NULL);

  return BTM_CMD_STARTED;
}

/*******************************************************************************
 *
 * Function         btm_ble_cancel_remote_name
 *
 * Description      This function cancel read remote LE device name.
 *
 * Parameters:       None.
 *
 * Returns          void
 *
 ******************************************************************************/
bool btm_ble_cancel_remote_name(const RawAddress& remote_bda) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  bool status;

  status = GAP_BleCancelReadPeerDevName(remote_bda);

  p_inq->remname_active = false;
  p_inq->remname_bda = RawAddress::kEmpty;
  alarm_cancel(p_inq->remote_name_timer);

  return status;
}

/*******************************************************************************
 *
 * Function         btm_ble_update_adv_flag
 *
 * Description      This function update the limited discoverable flag in the
 *                  adv data.
 *
 * Parameters:       None.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_ble_update_adv_flag(uint8_t flag) {
  tBTM_BLE_LOCAL_ADV_DATA* p_adv_data = &btm_cb.ble_ctr_cb.inq_var.adv_data;
  uint8_t* p;

  BTM_TRACE_DEBUG("btm_ble_update_adv_flag new=0x%x", flag);

  if (p_adv_data->p_flags != NULL) {
    BTM_TRACE_DEBUG("btm_ble_update_adv_flag old=0x%x", *p_adv_data->p_flags);
    *p_adv_data->p_flags = flag;
  } else /* no FLAGS in ADV data*/
  {
    p = (p_adv_data->p_pad == NULL) ? p_adv_data->ad_data : p_adv_data->p_pad;
    /* need 3 bytes space to stuff in the flags, if not */
    /* erase all written data, just for flags */
    if ((BTM_BLE_AD_DATA_LEN - (p - p_adv_data->ad_data)) < 3) {
      p = p_adv_data->p_pad = p_adv_data->ad_data;
      memset(p_adv_data->ad_data, 0, BTM_BLE_AD_DATA_LEN);
    }

    *p++ = 2;
    *p++ = BTM_BLE_AD_TYPE_FLAG;
    p_adv_data->p_flags = p;
    *p++ = flag;
    p_adv_data->p_pad = p;
  }

  btsnd_hcic_ble_set_adv_data(
      (uint8_t)(p_adv_data->p_pad - p_adv_data->ad_data), p_adv_data->ad_data);
  p_adv_data->data_mask |= BTM_BLE_AD_BIT_FLAGS;
}

/**
 * Check ADV flag to make sure device is discoverable and match the search
 * condition
 */
uint8_t btm_ble_is_discoverable(const RawAddress& bda,
                                std::vector<uint8_t> const& adv_data) {
  uint8_t flag = 0, rt = 0;
  uint8_t data_len;
  tBTM_INQ_PARMS* p_cond = &btm_cb.btm_inq_vars.inqparms;

  /* for observer, always "discoverable */
  if (BTM_BLE_IS_OBS_ACTIVE(btm_cb.ble_ctr_cb.scan_activity))
    rt |= BTM_BLE_OBS_RESULT;

  /* does not match filter condition */
  if (p_cond->filter_cond_type == BTM_FILTER_COND_BD_ADDR &&
      bda != p_cond->filter_cond.bdaddr_cond) {
    BTM_TRACE_DEBUG("BD ADDR does not meet filter condition");
    return rt;
  }

  if (!adv_data.empty()) {
    const uint8_t* p_flag = AdvertiseDataParser::GetFieldByType(
        adv_data, BTM_BLE_AD_TYPE_FLAG, &data_len);
    if (p_flag != NULL && data_len != 0) {
      flag = *p_flag;

      if ((btm_cb.btm_inq_vars.inq_active & BTM_BLE_GENERAL_INQUIRY) &&
          (flag & (BTM_BLE_LIMIT_DISC_FLAG | BTM_BLE_GEN_DISC_FLAG)) != 0) {
        BTM_TRACE_DEBUG("Find Generable Discoverable device");
        rt |= BTM_BLE_INQ_RESULT;
      }

      else if (btm_cb.btm_inq_vars.inq_active & BTM_BLE_LIMITED_INQUIRY &&
               (flag & BTM_BLE_LIMIT_DISC_FLAG) != 0) {
        BTM_TRACE_DEBUG("Find limited discoverable device");
        rt |= BTM_BLE_INQ_RESULT;
      }
    }
  }
  return rt;
}

static void btm_ble_appearance_to_cod(uint16_t appearance, uint8_t* dev_class) {
  dev_class[0] = 0;

  switch (appearance) {
    case BTM_BLE_APPEARANCE_GENERIC_PHONE:
      dev_class[1] = BTM_COD_MAJOR_PHONE;
      dev_class[2] = BTM_COD_MINOR_UNCLASSIFIED;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_COMPUTER:
      dev_class[1] = BTM_COD_MAJOR_COMPUTER;
      dev_class[2] = BTM_COD_MINOR_UNCLASSIFIED;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_REMOTE:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_REMOTE_CONTROL;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_THERMOMETER:
    case BTM_BLE_APPEARANCE_THERMOMETER_EAR:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_THERMOMETER;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_HEART_RATE:
    case BTM_BLE_APPEARANCE_HEART_RATE_BELT:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_HEART_PULSE_MONITOR;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE:
    case BTM_BLE_APPEARANCE_BLOOD_PRESSURE_ARM:
    case BTM_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_BLOOD_MONITOR;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER:
    case BTM_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP:
    case BTM_BLE_APPEARANCE_PULSE_OXIMETER_WRIST:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_PULSE_OXIMETER;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_GLUCOSE:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_GLUCOSE_METER;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_WEIGHT:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_WEIGHING_SCALE;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_WALKING:
    case BTM_BLE_APPEARANCE_WALKING_IN_SHOE:
    case BTM_BLE_APPEARANCE_WALKING_ON_SHOE:
    case BTM_BLE_APPEARANCE_WALKING_ON_HIP:
      dev_class[1] = BTM_COD_MAJOR_HEALTH;
      dev_class[2] = BTM_COD_MINOR_STEP_COUNTER;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_WATCH:
    case BTM_BLE_APPEARANCE_SPORTS_WATCH:
      dev_class[1] = BTM_COD_MAJOR_WEARABLE;
      dev_class[2] = BTM_COD_MINOR_WRIST_WATCH;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_EYEGLASSES:
      dev_class[1] = BTM_COD_MAJOR_WEARABLE;
      dev_class[2] = BTM_COD_MINOR_GLASSES;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_DISPLAY:
      dev_class[1] = BTM_COD_MAJOR_IMAGING;
      dev_class[2] = BTM_COD_MINOR_DISPLAY;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER:
      dev_class[1] = BTM_COD_MAJOR_AUDIO;
      dev_class[2] = BTM_COD_MINOR_UNCLASSIFIED;
      break;
    case BTM_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER:
    case BTM_BLE_APPEARANCE_HID_BARCODE_SCANNER:
    case BTM_BLE_APPEARANCE_GENERIC_HID:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_UNCLASSIFIED;
      break;
    case BTM_BLE_APPEARANCE_HID_KEYBOARD:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_KEYBOARD;
      break;
    case BTM_BLE_APPEARANCE_HID_MOUSE:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_POINTING;
      break;
    case BTM_BLE_APPEARANCE_HID_JOYSTICK:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_JOYSTICK;
      break;
    case BTM_BLE_APPEARANCE_HID_GAMEPAD:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_GAMEPAD;
      break;
    case BTM_BLE_APPEARANCE_HID_DIGITIZER_TABLET:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_DIGITIZING_TABLET;
      break;
    case BTM_BLE_APPEARANCE_HID_CARD_READER:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_CARD_READER;
      break;
    case BTM_BLE_APPEARANCE_HID_DIGITAL_PEN:
      dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
      dev_class[2] = BTM_COD_MINOR_DIGITAL_PAN;
      break;
    case BTM_BLE_APPEARANCE_UKNOWN:
    case BTM_BLE_APPEARANCE_GENERIC_CLOCK:
    case BTM_BLE_APPEARANCE_GENERIC_TAG:
    case BTM_BLE_APPEARANCE_GENERIC_KEYRING:
    case BTM_BLE_APPEARANCE_GENERIC_CYCLING:
    case BTM_BLE_APPEARANCE_CYCLING_COMPUTER:
    case BTM_BLE_APPEARANCE_CYCLING_SPEED:
    case BTM_BLE_APPEARANCE_CYCLING_CADENCE:
    case BTM_BLE_APPEARANCE_CYCLING_POWER:
    case BTM_BLE_APPEARANCE_CYCLING_SPEED_CADENCE:
    case BTM_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS:
    case BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION:
    case BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV:
    case BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD:
    case BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV:
    default:
      dev_class[1] = BTM_COD_MAJOR_UNCLASSIFIED;
      dev_class[2] = BTM_COD_MINOR_UNCLASSIFIED;
  };
}

/**
 * Update adv packet information into inquiry result.
 */
void btm_ble_update_inq_result(tINQ_DB_ENT* p_i, uint8_t addr_type,
                               const RawAddress& bda, uint16_t evt_type,
                               uint8_t primary_phy, uint8_t secondary_phy,
                               uint8_t advertising_sid, int8_t tx_power,
                               int8_t rssi, uint16_t periodic_adv_int,
                               std::vector<uint8_t> const& data) {
  tBTM_INQ_RESULTS* p_cur = &p_i->inq_info.results;
  uint8_t len;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  /* Save the info */
  p_cur->inq_result_type = BTM_INQ_RESULT_BLE;
  p_cur->ble_addr_type = addr_type;
  p_cur->rssi = rssi;
  p_cur->ble_primary_phy = primary_phy;
  p_cur->ble_secondary_phy = secondary_phy;
  p_cur->ble_advertising_sid = advertising_sid;
  p_cur->ble_tx_power = tx_power;
  p_cur->ble_periodic_adv_int = periodic_adv_int;

  if (btm_cb.ble_ctr_cb.inq_var.scan_type == BTM_BLE_SCAN_MODE_ACTI &&
      ble_evt_type_is_scannable(evt_type) &&
      !ble_evt_type_is_scan_resp(evt_type)) {
    p_i->scan_rsp = false;
  } else
    p_i->scan_rsp = true;

  if (p_i->inq_count != p_inq->inq_counter)
    p_cur->device_type = BT_DEVICE_TYPE_BLE;
  else
    p_cur->device_type |= BT_DEVICE_TYPE_BLE;

  if (evt_type != BTM_BLE_SCAN_RSP_EVT) p_cur->ble_evt_type = evt_type;

  p_i->inq_count = p_inq->inq_counter; /* Mark entry for current inquiry */

  if (!data.empty()) {
    const uint8_t* p_flag =
        AdvertiseDataParser::GetFieldByType(data, BTM_BLE_AD_TYPE_FLAG, &len);
    if (p_flag != NULL && len != 0) p_cur->flag = *p_flag;
  }

  if (!data.empty()) {
    /* Check to see the BLE device has the Appearance UUID in the advertising
     * data.  If it does
     * then try to convert the appearance value to a class of device value
     * Bluedroid can use.
     * Otherwise fall back to trying to infer if it is a HID device based on the
     * service class.
     */
    const uint8_t* p_uuid16 = AdvertiseDataParser::GetFieldByType(
        data, BTM_BLE_AD_TYPE_APPEARANCE, &len);
    if (p_uuid16 && len == 2) {
      btm_ble_appearance_to_cod((uint16_t)p_uuid16[0] | (p_uuid16[1] << 8),
                                p_cur->dev_class);
    } else {
      p_uuid16 = AdvertiseDataParser::GetFieldByType(
          data, BTM_BLE_AD_TYPE_16SRV_CMPL, &len);
      if (p_uuid16 != NULL) {
        uint8_t i;
        for (i = 0; i + 2 <= len; i = i + 2) {
          /* if this BLE device support HID over LE, set HID Major in class of
           * device */
          if ((p_uuid16[i] | (p_uuid16[i + 1] << 8)) == UUID_SERVCLASS_LE_HID) {
            p_cur->dev_class[0] = 0;
            p_cur->dev_class[1] = BTM_COD_MAJOR_PERIPHERAL;
            p_cur->dev_class[2] = 0;
            break;
          }
        }
      }
    }
  }

  /* if BR/EDR not supported is not set, assume is a DUMO device */
  if ((p_cur->flag & BTM_BLE_BREDR_NOT_SPT) == 0 &&
      !ble_evt_type_is_directed(evt_type)) {
    if (p_cur->ble_addr_type != BLE_ADDR_RANDOM) {
      BTM_TRACE_DEBUG("BR/EDR NOT support bit not set, treat as DUMO");
      p_cur->device_type |= BT_DEVICE_TYPE_DUMO;
    } else {
      BTM_TRACE_DEBUG("Random address, treating device as LE only");
    }
  } else {
    BTM_TRACE_DEBUG("BR/EDR NOT SUPPORT bit set, LE only device");
  }
}

/*******************************************************************************
 *
 * Function         btm_clear_all_pending_le_entry
 *
 * Description      This function is called to clear all LE pending entry in
 *                  inquiry database.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_clear_all_pending_le_entry(void) {
  uint16_t xx;
  tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

  for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
    /* mark all pending LE entry as unused if an LE only device has scan
     * response outstanding */
    if ((p_ent->in_use) &&
        (p_ent->inq_info.results.device_type == BT_DEVICE_TYPE_BLE) &&
        !p_ent->scan_rsp)
      p_ent->in_use = false;
  }
}

void btm_ble_process_adv_addr(RawAddress& bda, uint8_t* addr_type) {
#if (BLE_PRIVACY_SPT == TRUE)
  /* map address to security record */
  bool match = btm_identity_addr_to_random_pseudo(&bda, addr_type, false);

  VLOG(1) << __func__ << ": bda=" << bda;
  /* always do RRA resolution on host */
  if (!match && BTM_BLE_IS_RESOLVE_BDA(bda)) {
    tBTM_SEC_DEV_REC* match_rec = btm_ble_resolve_random_addr(bda);
    if (match_rec) {
      match_rec->ble.active_addr_type = BTM_BLE_ADDR_RRA;
      match_rec->ble.cur_rand_addr = bda;

      if (btm_ble_init_pseudo_addr(match_rec, bda)) {
        bda = match_rec->bd_addr;
      } else {
        // Assign the original address to be the current report address
        bda = match_rec->ble.pseudo_addr;
        *addr_type = match_rec->ble.ble_addr_type;
      }
    }
  }
#endif
}

/**
 * This function is called when extended advertising report event is received .
 * It updates the inquiry database. If the inquiry database is full, the oldest
 * entry is discarded.
 */
void btm_ble_process_ext_adv_pkt(uint8_t data_len, uint8_t* data) {
  RawAddress bda, direct_address;
  uint8_t* p = data;
  uint8_t addr_type, num_reports, pkt_data_len, primary_phy, secondary_phy,
      advertising_sid;
  int8_t rssi, tx_power;
  uint16_t event_type, periodic_adv_int, direct_address_type;

  /* Only process the results if the inquiry is still active */
  if (!BTM_BLE_IS_SCAN_ACTIVE(btm_cb.ble_ctr_cb.scan_activity)) return;

  /* Extract the number of reports in this event. */
  STREAM_TO_UINT8(num_reports, p);

  while (num_reports--) {
    if (p > data + data_len) {
      // TODO(jpawlowski): we should crash the stack here
      BTM_TRACE_ERROR(
          "Malformed LE Extended Advertising Report Event from controller - "
          "can't loop the data");
      return;
    }

    /* Extract inquiry results */
    STREAM_TO_UINT16(event_type, p);
    STREAM_TO_UINT8(addr_type, p);
    STREAM_TO_BDADDR(bda, p);
    STREAM_TO_UINT8(primary_phy, p);
    STREAM_TO_UINT8(secondary_phy, p);
    STREAM_TO_UINT8(advertising_sid, p);
    STREAM_TO_INT8(tx_power, p);
    STREAM_TO_INT8(rssi, p);
    STREAM_TO_UINT16(periodic_adv_int, p);
    STREAM_TO_UINT8(direct_address_type, p);
    STREAM_TO_BDADDR(direct_address, p);
    STREAM_TO_UINT8(pkt_data_len, p);

    uint8_t* pkt_data = p;
    p += pkt_data_len; /* Advance to the the next packet*/
    if (p > data + data_len) {
      LOG(ERROR) << "Invalid pkt_data_len: " << +pkt_data_len;
      return;
    }

    if (rssi >= 21 && rssi <= 126) {
      BTM_TRACE_ERROR("%s: bad rssi value in advertising report: %d", __func__,
                      rssi);
    }

    if (addr_type != BLE_ADDR_ANONYMOUS) {
      btm_ble_process_adv_addr(bda, &addr_type);
    }

    btm_ble_process_adv_pkt_cont(event_type, addr_type, bda, primary_phy,
                                 secondary_phy, advertising_sid, tx_power, rssi,
                                 periodic_adv_int, pkt_data_len, pkt_data);
  }
}

/**
 * This function is called when advertising report event is received. It updates
 * the inquiry database. If the inquiry database is full, the oldest entry is
 * discarded.
 */
void btm_ble_process_adv_pkt(uint8_t data_len, uint8_t* data) {
  RawAddress bda;
  uint8_t* p = data;
  uint8_t legacy_evt_type, addr_type, num_reports, pkt_data_len;
  int8_t rssi;

  /* Only process the results if the inquiry is still active */
  if (!BTM_BLE_IS_SCAN_ACTIVE(btm_cb.ble_ctr_cb.scan_activity)) return;

  /* Extract the number of reports in this event. */
  STREAM_TO_UINT8(num_reports, p);

  while (num_reports--) {
    if (p > data + data_len) {
      // TODO(jpawlowski): we should crash the stack here
      BTM_TRACE_ERROR("Malformed LE Advertising Report Event from controller");
      return;
    }

    /* Extract inquiry results */
    STREAM_TO_UINT8(legacy_evt_type, p);
    STREAM_TO_UINT8(addr_type, p);
    STREAM_TO_BDADDR(bda, p);
    STREAM_TO_UINT8(pkt_data_len, p);

    uint8_t* pkt_data = p;
    p += pkt_data_len; /* Advance to the the rssi byte */
    if (p > data + data_len - sizeof(rssi)) {
      LOG(ERROR) << "Invalid pkt_data_len: " << +pkt_data_len;
      return;
    }

    STREAM_TO_INT8(rssi, p);

    if (rssi >= 21 && rssi <= 126) {
      BTM_TRACE_ERROR("%s: bad rssi value in advertising report: ", __func__,
                      pkt_data_len, rssi);
    }

    btm_ble_process_adv_addr(bda, &addr_type);

    uint16_t event_type;
    if (legacy_evt_type == 0x00) {  // ADV_IND;
      event_type = 0x0013;
    } else if (legacy_evt_type == 0x01) {  // ADV_DIRECT_IND;
      event_type = 0x0015;
    } else if (legacy_evt_type == 0x02) {  // ADV_SCAN_IND;
      event_type = 0x0012;
    } else if (legacy_evt_type == 0x03) {  // ADV_NONCONN_IND;
      event_type = 0x0010;
    } else if (legacy_evt_type == 0x04) {  // SCAN_RSP;
      // We can't distinguish between "SCAN_RSP to an ADV_IND", and "SCAN_RSP to
      // an ADV_SCAN_IND", so always return "SCAN_RSP to an ADV_IND"
      event_type = 0x001B;
    } else {
      BTM_TRACE_ERROR(
          "Malformed LE Advertising Report Event - unsupported "
          "legacy_event_type 0x%02x",
          legacy_evt_type);
      return;
    }

    btm_ble_process_adv_pkt_cont(
        event_type, addr_type, bda, PHY_LE_1M, PHY_LE_NO_PACKET, NO_ADI_PRESENT,
        TX_POWER_NOT_PRESENT, rssi, 0x00 /* no periodic adv */, pkt_data_len,
        pkt_data);
  }
}

/**
 * This function is called after random address resolution is done, and proceed
 * to process adv packet.
 */
static void btm_ble_process_adv_pkt_cont(
    uint16_t evt_type, uint8_t addr_type, const RawAddress& bda,
    uint8_t primary_phy, uint8_t secondary_phy, uint8_t advertising_sid,
    int8_t tx_power, int8_t rssi, uint16_t periodic_adv_int, uint8_t data_len,
    uint8_t* data) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  bool update = true;

  std::vector<uint8_t> tmp;
  if (data_len != 0) tmp.insert(tmp.begin(), data, data + data_len);

  bool is_scannable = ble_evt_type_is_scannable(evt_type);
  bool is_scan_resp = ble_evt_type_is_scan_resp(evt_type);

  bool is_start =
      ble_evt_type_is_legacy(evt_type) && is_scannable && !is_scan_resp;

  if (ble_evt_type_is_legacy(evt_type))
    AdvertiseDataParser::RemoveTrailingZeros(tmp);

  // We might have send scan request to this device before, but didn't get the
  // response. In such case make sure data is put at start, not appended to
  // already existing data.
  std::vector<uint8_t> const& adv_data =
      is_start ? cache.Set(addr_type, bda, std::move(tmp))
               : cache.Append(addr_type, bda, std::move(tmp));

  bool data_complete = (ble_evt_type_data_status(evt_type) != 0x01);

  if (!data_complete) {
    // If we didn't receive whole adv data yet, don't report the device.
    DVLOG(1) << "Data not complete yet, waiting for more " << bda;
    return;
  }

  bool is_active_scan =
      btm_cb.ble_ctr_cb.inq_var.scan_type == BTM_BLE_SCAN_MODE_ACTI;
  if (is_active_scan && is_scannable && !is_scan_resp) {
    // If we didn't receive scan response yet, don't report the device.
    DVLOG(1) << " Waiting for scan response " << bda;
    return;
  }

  if (!AdvertiseDataParser::IsValid(adv_data)) {
    DVLOG(1) << __func__ << "Dropping bad advertisement packet: "
             << base::HexEncode(adv_data.data(), adv_data.size());
    return;
  }

  tINQ_DB_ENT* p_i = btm_inq_db_find(bda);

  /* Check if this address has already been processed for this inquiry */
  if (btm_inq_find_bdaddr(bda)) {
    /* never been report as an LE device */
    if (p_i && (!(p_i->inq_info.results.device_type & BT_DEVICE_TYPE_BLE) ||
                /* scan repsonse to be updated */
                (!p_i->scan_rsp))) {
      update = true;
    } else if (BTM_BLE_IS_OBS_ACTIVE(btm_cb.ble_ctr_cb.scan_activity)) {
      update = false;
    } else {
      /* if yes, skip it */
      return; /* assumption: one result per event */
    }
  }
  /* If existing entry, use that, else get  a new one (possibly reusing the
   * oldest) */
  if (p_i == NULL) {
    p_i = btm_inq_db_new(bda);
    if (p_i != NULL) {
      p_inq->inq_cmpl_info.num_resp++;
    } else
      return;
  } else if (p_i->inq_count !=
             p_inq->inq_counter) /* first time seen in this inquiry */
  {
    p_inq->inq_cmpl_info.num_resp++;
  }

  /* update the LE device information in inquiry database */
  btm_ble_update_inq_result(p_i, addr_type, bda, evt_type, primary_phy,
                            secondary_phy, advertising_sid, tx_power, rssi,
                            periodic_adv_int, adv_data);

  uint8_t result = btm_ble_is_discoverable(bda, adv_data);
  if (result == 0) {
    cache.Clear(addr_type, bda);
    LOG_WARN(LOG_TAG,
             "%s device no longer discoverable, discarding advertising packet",
             __func__);
    return;
  }

  if (!update) result &= ~BTM_BLE_INQ_RESULT;
  /* If the number of responses found and limited, issue a cancel inquiry */
  if (p_inq->inqparms.max_resps &&
      p_inq->inq_cmpl_info.num_resp == p_inq->inqparms.max_resps) {
    /* new device */
    if (p_i == NULL ||
        /* assume a DUMO device, BR/EDR inquiry is always active */
        (p_i &&
         (p_i->inq_info.results.device_type & BT_DEVICE_TYPE_BLE) ==
             BT_DEVICE_TYPE_BLE &&
         p_i->scan_rsp)) {
      BTM_TRACE_WARNING(
          "INQ RES: Extra Response Received...cancelling inquiry..");

      /* if is non-periodic inquiry active, cancel now */
      if ((p_inq->inq_active & BTM_BR_INQ_ACTIVE_MASK) != 0 &&
          (p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) == 0)
        btsnd_hcic_inq_cancel();

      btm_ble_stop_inquiry();

      btm_acl_update_busy_level(BTM_BLI_INQ_DONE_EVT);
    }
  }

  tBTM_INQ_RESULTS_CB* p_inq_results_cb = p_inq->p_inq_results_cb;
  if (p_inq_results_cb && (result & BTM_BLE_INQ_RESULT)) {
    (p_inq_results_cb)((tBTM_INQ_RESULTS*)&p_i->inq_info.results,
                       const_cast<uint8_t*>(adv_data.data()), adv_data.size());
  }

  tBTM_INQ_RESULTS_CB* p_obs_results_cb = btm_cb.ble_ctr_cb.p_obs_results_cb;
  if (p_obs_results_cb && (result & BTM_BLE_OBS_RESULT)) {
    (p_obs_results_cb)((tBTM_INQ_RESULTS*)&p_i->inq_info.results,
                       const_cast<uint8_t*>(adv_data.data()), adv_data.size());
  }

  cache.Clear(addr_type, bda);
}

void btm_ble_process_phy_update_pkt(uint8_t len, uint8_t* data) {
  uint8_t status, tx_phy, rx_phy;
  uint16_t handle;

  LOG_ASSERT(len == 5);
  uint8_t* p = data;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  handle = handle & 0x0FFF;
  STREAM_TO_UINT8(tx_phy, p);
  STREAM_TO_UINT8(rx_phy, p);

  gatt_notify_phy_updated(status, handle, tx_phy, rx_phy);
}

/*******************************************************************************
 *
 * Function         btm_ble_start_scan
 *
 * Description      Start the BLE scan.
 *
 * Returns          void
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_start_scan(void) {
  tBTM_BLE_INQ_CB* p_inq = &btm_cb.ble_ctr_cb.inq_var;
  /* start scan, disable duplicate filtering */
  btm_send_hci_scan_enable(BTM_BLE_SCAN_ENABLE, p_inq->scan_duplicate_filter);

  if (p_inq->scan_type == BTM_BLE_SCAN_MODE_ACTI)
    btm_ble_set_topology_mask(BTM_BLE_STATE_ACTIVE_SCAN_BIT);
  else
    btm_ble_set_topology_mask(BTM_BLE_STATE_PASSIVE_SCAN_BIT);

  return BTM_CMD_STARTED;
}

/*******************************************************************************
 *
 * Function         btm_ble_stop_scan
 *
 * Description      Stop the BLE scan.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_stop_scan(void) {
  BTM_TRACE_EVENT("btm_ble_stop_scan ");

  if (btm_cb.ble_ctr_cb.inq_var.scan_type == BTM_BLE_SCAN_MODE_ACTI)
    btm_ble_clear_topology_mask(BTM_BLE_STATE_ACTIVE_SCAN_BIT);
  else
    btm_ble_clear_topology_mask(BTM_BLE_STATE_PASSIVE_SCAN_BIT);

  /* Clear the inquiry callback if set */
  btm_cb.ble_ctr_cb.inq_var.scan_type = BTM_BLE_SCAN_MODE_NONE;

  /* stop discovery now */
  btm_send_hci_scan_enable(BTM_BLE_SCAN_DISABLE, BTM_BLE_DUPLICATE_ENABLE);

  btm_update_scanner_filter_policy(SP_ADV_ALL);
}
/*******************************************************************************
 *
 * Function         btm_ble_stop_inquiry
 *
 * Description      Stop the BLE Inquiry.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_stop_inquiry(void) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tBTM_BLE_CB* p_ble_cb = &btm_cb.ble_ctr_cb;

  alarm_cancel(p_ble_cb->inq_var.inquiry_timer);

  p_ble_cb->scan_activity &= ~BTM_BLE_INQUIRY_MASK;

  /* If no more scan activity, stop LE scan now */
  if (!BTM_BLE_IS_SCAN_ACTIVE(p_ble_cb->scan_activity))
    btm_ble_stop_scan();
  else if ((p_ble_cb->inq_var.scan_interval != BTM_BLE_LOW_LATENCY_SCAN_INT) ||
           (p_ble_cb->inq_var.scan_window != BTM_BLE_LOW_LATENCY_SCAN_WIN)) {
    BTM_TRACE_DEBUG("%s: setting default params for ongoing observe", __func__);
    btm_ble_stop_scan();
    btm_ble_start_scan();
  }

  /* If we have a callback registered for inquiry complete, call it */
  BTM_TRACE_DEBUG("BTM Inq Compl Callback: status 0x%02x, num results %d",
                  p_inq->inq_cmpl_info.status, p_inq->inq_cmpl_info.num_resp);

  btm_process_inq_complete(
      HCI_SUCCESS, (uint8_t)(p_inq->inqparms.mode & BTM_BLE_INQUIRY_MASK));
}

/*******************************************************************************
 *
 * Function         btm_ble_stop_observe
 *
 * Description      Stop the BLE Observe.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_ble_stop_observe(void) {
  tBTM_BLE_CB* p_ble_cb = &btm_cb.ble_ctr_cb;
  tBTM_CMPL_CB* p_obs_cb = p_ble_cb->p_obs_cmpl_cb;

  alarm_cancel(p_ble_cb->observer_timer);

  p_ble_cb->scan_activity &= ~BTM_LE_OBSERVE_ACTIVE;

  p_ble_cb->p_obs_results_cb = NULL;
  p_ble_cb->p_obs_cmpl_cb = NULL;

  if (!BTM_BLE_IS_SCAN_ACTIVE(p_ble_cb->scan_activity)) btm_ble_stop_scan();

  if (p_obs_cb) (p_obs_cb)(&btm_cb.btm_inq_vars.inq_cmpl_info);
}
/*******************************************************************************
 *
 * Function         btm_ble_adv_states_operation
 *
 * Description      Set or clear adv states in topology mask
 *
 * Returns          operation status. true if sucessful, false otherwise.
 *
 ******************************************************************************/
typedef bool(BTM_TOPOLOGY_FUNC_PTR)(tBTM_BLE_STATE_MASK);
static bool btm_ble_adv_states_operation(BTM_TOPOLOGY_FUNC_PTR* p_handler,
                                         uint8_t adv_evt) {
  bool rt = false;

  switch (adv_evt) {
    case BTM_BLE_CONNECT_EVT:
      rt = (*p_handler)(BTM_BLE_STATE_CONN_ADV_BIT);
      break;

    case BTM_BLE_NON_CONNECT_EVT:
      rt = (*p_handler)(BTM_BLE_STATE_NON_CONN_ADV_BIT);
      break;
    case BTM_BLE_CONNECT_DIR_EVT:
      rt = (*p_handler)(BTM_BLE_STATE_HI_DUTY_DIR_ADV_BIT);
      break;

    case BTM_BLE_DISCOVER_EVT:
      rt = (*p_handler)(BTM_BLE_STATE_SCAN_ADV_BIT);
      break;

    case BTM_BLE_CONNECT_LO_DUTY_DIR_EVT:
      rt = (*p_handler)(BTM_BLE_STATE_LO_DUTY_DIR_ADV_BIT);
      break;

    default:
      BTM_TRACE_ERROR("unknown adv event : %d", adv_evt);
      break;
  }

  return rt;
}

/*******************************************************************************
 *
 * Function         btm_ble_start_adv
 *
 * Description      start the BLE advertising.
 *
 * Returns          void
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_start_adv(void) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;

  if (!btm_ble_adv_states_operation(btm_ble_topology_check, p_cb->evt_type))
    return BTM_WRONG_MODE;

#if (BLE_PRIVACY_SPT == TRUE)
  /* To relax resolving list,  always have resolving list enabled, unless
   * directed adv */
  if (p_cb->evt_type != BTM_BLE_CONNECT_LO_DUTY_DIR_EVT &&
      p_cb->evt_type != BTM_BLE_CONNECT_DIR_EVT)
    /* enable resolving list is desired */
    btm_ble_enable_resolving_list_for_platform(BTM_BLE_RL_ADV);
#endif

  btsnd_hcic_ble_set_adv_enable(BTM_BLE_ADV_ENABLE);
  p_cb->adv_mode = BTM_BLE_ADV_ENABLE;
  btm_ble_adv_states_operation(btm_ble_set_topology_mask, p_cb->evt_type);
  return BTM_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btm_ble_stop_adv
 *
 * Description      Stop the BLE advertising.
 *
 * Returns          void
 *
 ******************************************************************************/
tBTM_STATUS btm_ble_stop_adv(void) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;

  if (p_cb->adv_mode == BTM_BLE_ADV_ENABLE) {
    btsnd_hcic_ble_set_adv_enable(BTM_BLE_ADV_DISABLE);

    p_cb->fast_adv_on = false;
    p_cb->adv_mode = BTM_BLE_ADV_DISABLE;

    /* clear all adv states */
    btm_ble_clear_topology_mask(BTM_BLE_STATE_ALL_ADV_MASK);
  }
  return BTM_SUCCESS;
}

static void btm_ble_fast_adv_timer_timeout(UNUSED_ATTR void* data) {
  /* fast adv is completed, fall back to slow adv interval */
  btm_ble_start_slow_adv();
}

/*******************************************************************************
 *
 * Function         btm_ble_start_slow_adv
 *
 * Description      Restart adv with slow adv interval
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_ble_start_slow_adv(void) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;

  if (p_cb->adv_mode == BTM_BLE_ADV_ENABLE) {
    tBTM_LE_RANDOM_CB* p_addr_cb = &btm_cb.ble_ctr_cb.addr_mgnt_cb;
    RawAddress address = RawAddress::kEmpty;
    tBLE_ADDR_TYPE init_addr_type = BLE_ADDR_PUBLIC;
    tBLE_ADDR_TYPE own_addr_type = p_addr_cb->own_addr_type;

    btm_ble_stop_adv();

    p_cb->evt_type = btm_set_conn_mode_adv_init_addr(
        p_cb, address, &init_addr_type, &own_addr_type);

    /* slow adv mode never goes into directed adv */
    btsnd_hcic_ble_write_adv_params(
        BTM_BLE_GAP_ADV_SLOW_INT, BTM_BLE_GAP_ADV_SLOW_INT, p_cb->evt_type,
        own_addr_type, init_addr_type, address, p_cb->adv_chnl_map, p_cb->afp);

    btm_ble_start_adv();
  }
}

static void btm_ble_inquiry_timer_gap_limited_discovery_timeout(
    UNUSED_ATTR void* data) {
  /* lim_timeout expired, limited discovery should exit now */
  btm_cb.btm_inq_vars.discoverable_mode &= ~BTM_BLE_LIMITED_DISCOVERABLE;
  btm_ble_set_adv_flag(btm_cb.btm_inq_vars.connectable_mode,
                       btm_cb.btm_inq_vars.discoverable_mode);
}

static void btm_ble_inquiry_timer_timeout(UNUSED_ATTR void* data) {
  btm_ble_stop_inquiry();
}

static void btm_ble_observer_timer_timeout(UNUSED_ATTR void* data) {
  btm_ble_stop_observe();
}

void btm_ble_refresh_raddr_timer_timeout(UNUSED_ATTR void* data) {
  if (btm_cb.ble_ctr_cb.addr_mgnt_cb.own_addr_type == BLE_ADDR_RANDOM) {
    /* refresh the random addr */
    btm_gen_resolvable_private_addr(base::Bind(&btm_gen_resolve_paddr_low));
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_read_remote_features_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the read LE remote feature
 *                  supported complete event.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_read_remote_features_complete(uint8_t* p) {
  BTM_TRACE_EVENT("%s", __func__);

  uint16_t handle;
  uint8_t status;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT16(handle, p);
  handle = handle & 0x0FFF;  // only 12 bits meaningful

  if (status != HCI_SUCCESS) {
    BTM_TRACE_ERROR("%s: failed for handle: 0x%04d, status 0x%02x", __func__,
                    handle, status);
    if (status != HCI_ERR_UNSUPPORTED_REM_FEATURE) return;
  }

  int idx = btm_handle_to_acl_index(handle);
  if (idx == MAX_L2CAP_LINKS) {
    BTM_TRACE_ERROR("%s: can't find acl for handle: 0x%04d", __func__, handle);
    return;
  }

  if (status == HCI_SUCCESS) {
    STREAM_TO_ARRAY(btm_cb.acl_db[idx].peer_le_features, p, BD_FEATURES_LEN);
  }

  btsnd_hcic_rmt_ver_req(handle);
}

/*******************************************************************************
 *
 * Function         btm_ble_write_adv_enable_complete
 *
 * Description      This function process the write adv enable command complete.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_write_adv_enable_complete(uint8_t* p) {
  tBTM_BLE_INQ_CB* p_cb = &btm_cb.ble_ctr_cb.inq_var;

  /* if write adv enable/disbale not succeed */
  if (*p != HCI_SUCCESS) {
    /* toggle back the adv mode */
    p_cb->adv_mode = !p_cb->adv_mode;
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_dir_adv_tout
 *
 * Description      when directed adv time out
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_dir_adv_tout(void) {
  btm_cb.ble_ctr_cb.inq_var.adv_mode = BTM_BLE_ADV_DISABLE;

  /* make device fall back into undirected adv mode by default */
  btm_cb.ble_ctr_cb.inq_var.directed_conn = false;
}

/*******************************************************************************
 *
 * Function         btm_ble_set_topology_mask
 *
 * Description      set BLE topology mask
 *
 * Returns          true is request is allowed, false otherwise.
 *
 ******************************************************************************/
bool btm_ble_set_topology_mask(tBTM_BLE_STATE_MASK request_state_mask) {
  request_state_mask &= BTM_BLE_STATE_ALL_MASK;
  btm_cb.ble_ctr_cb.cur_states |= (request_state_mask & BTM_BLE_STATE_ALL_MASK);
  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_clear_topology_mask
 *
 * Description      Clear BLE topology bit mask
 *
 * Returns          true is request is allowed, false otherwise.
 *
 ******************************************************************************/
bool btm_ble_clear_topology_mask(tBTM_BLE_STATE_MASK request_state_mask) {
  request_state_mask &= BTM_BLE_STATE_ALL_MASK;
  btm_cb.ble_ctr_cb.cur_states &= ~request_state_mask;
  return true;
}

/*******************************************************************************
 *
 * Function         btm_ble_update_link_topology_mask
 *
 * Description      This function update the link topology mask
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_update_link_topology_mask(uint8_t link_role, bool increase) {
  btm_ble_clear_topology_mask(BTM_BLE_STATE_ALL_CONN_MASK);

  if (increase)
    btm_cb.ble_ctr_cb.link_count[link_role]++;
  else if (btm_cb.ble_ctr_cb.link_count[link_role] > 0)
    btm_cb.ble_ctr_cb.link_count[link_role]--;

  if (btm_cb.ble_ctr_cb.link_count[HCI_ROLE_MASTER])
    btm_ble_set_topology_mask(BTM_BLE_STATE_MASTER_BIT);

  if (btm_cb.ble_ctr_cb.link_count[HCI_ROLE_SLAVE])
    btm_ble_set_topology_mask(BTM_BLE_STATE_SLAVE_BIT);

  if (link_role == HCI_ROLE_SLAVE && increase) {
    btm_cb.ble_ctr_cb.inq_var.adv_mode = BTM_BLE_ADV_DISABLE;
    /* make device fall back into undirected adv mode by default */
    btm_cb.ble_ctr_cb.inq_var.directed_conn = BTM_BLE_CONNECT_EVT;
    /* clear all adv states */
    btm_ble_clear_topology_mask(BTM_BLE_STATE_ALL_ADV_MASK);
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_update_mode_operation
 *
 * Description      This function update the GAP role operation when a link
 *                  status is updated.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_update_mode_operation(uint8_t link_role, const RawAddress* bd_addr,
                                   uint8_t status) {
  if (status == HCI_ERR_ADVERTISING_TIMEOUT) {
    btm_cb.ble_ctr_cb.inq_var.adv_mode = BTM_BLE_ADV_DISABLE;
    /* make device fall back into undirected adv mode by default */
    btm_cb.ble_ctr_cb.inq_var.directed_conn = BTM_BLE_CONNECT_EVT;
    /* clear all adv states */
    btm_ble_clear_topology_mask(BTM_BLE_STATE_ALL_ADV_MASK);
  }

  if (btm_cb.ble_ctr_cb.inq_var.connectable_mode == BTM_BLE_CONNECTABLE) {
    btm_ble_set_connectability(btm_cb.btm_inq_vars.connectable_mode |
                               btm_cb.ble_ctr_cb.inq_var.connectable_mode);
  }

  /* in case of disconnected, we must cancel bgconn and restart
     in order to add back device to white list in order to reconnect */
  if (bd_addr) btm_ble_bgconn_cancel_if_disconnected(*bd_addr);

  /* when no connection is attempted, and controller is not rejecting last
     request
     due to resource limitation, start next direct connection or background
     connection
     now in order */
  if (btm_ble_get_conn_st() == BLE_CONN_IDLE &&
      status != HCI_ERR_HOST_REJECT_RESOURCES &&
      status != HCI_ERR_MAX_NUM_OF_CONNECTIONS) {
    btm_ble_resume_bg_conn();
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_init
 *
 * Description      Initialize the control block variable values.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_init(void) {
  tBTM_BLE_CB* p_cb = &btm_cb.ble_ctr_cb;

  BTM_TRACE_DEBUG("%s", __func__);

  alarm_free(p_cb->observer_timer);
  alarm_free(p_cb->inq_var.fast_adv_timer);
  memset(p_cb, 0, sizeof(tBTM_BLE_CB));
  memset(&(btm_cb.cmn_ble_vsc_cb), 0, sizeof(tBTM_BLE_VSC_CB));
  btm_cb.cmn_ble_vsc_cb.values_read = false;

  p_cb->observer_timer = alarm_new("btm_ble.observer_timer");
  p_cb->cur_states = 0;

  p_cb->inq_var.adv_mode = BTM_BLE_ADV_DISABLE;
  p_cb->inq_var.scan_type = BTM_BLE_SCAN_MODE_NONE;
  p_cb->inq_var.adv_chnl_map = BTM_BLE_DEFAULT_ADV_CHNL_MAP;
  p_cb->inq_var.afp = BTM_BLE_DEFAULT_AFP;
  p_cb->inq_var.sfp = BTM_BLE_DEFAULT_SFP;
  p_cb->inq_var.connectable_mode = BTM_BLE_NON_CONNECTABLE;
  p_cb->inq_var.discoverable_mode = BTM_BLE_NON_DISCOVERABLE;
  p_cb->inq_var.fast_adv_timer = alarm_new("btm_ble_inq.fast_adv_timer");
  p_cb->inq_var.inquiry_timer = alarm_new("btm_ble_inq.inquiry_timer");

  /* for background connection, reset connection params to be undefined */
  p_cb->scan_int = p_cb->scan_win = BTM_BLE_SCAN_PARAM_UNDEF;

  p_cb->inq_var.evt_type = BTM_BLE_NON_CONNECT_EVT;

  p_cb->addr_mgnt_cb.refresh_raddr_timer =
      alarm_new("btm_ble_addr.refresh_raddr_timer");

#if (BLE_VND_INCLUDED == FALSE)
  btm_ble_adv_filter_init();
#endif
}

/*******************************************************************************
 *
 * Function         btm_ble_topology_check
 *
 * Description      check to see requested state is supported. One state check
 *                  at a time is supported
 *
 * Returns          true is request is allowed, false otherwise.
 *
 ******************************************************************************/
bool btm_ble_topology_check(tBTM_BLE_STATE_MASK request_state_mask) {
  bool rt = false;

  uint8_t state_offset = 0;
  uint16_t cur_states = btm_cb.ble_ctr_cb.cur_states;
  uint8_t request_state = 0;

  /* check only one bit is set and within valid range */
  if (request_state_mask == BTM_BLE_STATE_INVALID ||
      request_state_mask > BTM_BLE_STATE_SCAN_ADV_BIT ||
      (request_state_mask & (request_state_mask - 1)) != 0) {
    BTM_TRACE_ERROR("illegal state requested: %d", request_state_mask);
    return rt;
  }

  while (request_state_mask) {
    request_state_mask >>= 1;
    request_state++;
  }

  /* check if the requested state is supported or not */
  uint8_t bit_num = btm_le_state_combo_tbl[0][request_state - 1];
  const uint8_t* ble_supported_states =
      controller_get_interface()->get_ble_supported_states();

  if (!BTM_LE_STATES_SUPPORTED(ble_supported_states, bit_num)) {
    BTM_TRACE_ERROR("state requested not supported: %d", request_state);
    return rt;
  }

  rt = true;
  /* make sure currently active states are all supported in conjunction with the
     requested state. If the bit in table is UNSUPPORTED, the combination is not
     supported */
  while (cur_states != 0) {
    if (cur_states & 0x01) {
      uint8_t bit_num = btm_le_state_combo_tbl[request_state][state_offset];
      if (bit_num != UNSUPPORTED) {
        if (!BTM_LE_STATES_SUPPORTED(ble_supported_states, bit_num)) {
          rt = false;
          break;
        }
      }
    }
    cur_states >>= 1;
    state_offset++;
  }
  return rt;
}
