/******************************************************************************
 *
 *  Copyright 1999-2012 Broadcom Corporation
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
 *  This file contains functions for BLE address management.
 *
 ******************************************************************************/

#include <base/bind.h>
#include <string.h>

#include "bt_types.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "gap_api.h"
#include "hcimsgs.h"

#include "btm_ble_int.h"
#include "stack/crypto_toolbox/crypto_toolbox.h"

/* This function generates Resolvable Private Address (RPA) from Identity
 * Resolving Key |irk| and |random|*/
RawAddress generate_rpa_from_irk_and_rand(const Octet16& irk,
                                          BT_OCTET8 random) {
  random[2] &= (~BLE_RESOLVE_ADDR_MASK);
  random[2] |= BLE_RESOLVE_ADDR_MSB;

  RawAddress address;
  address.address[2] = random[0];
  address.address[1] = random[1];
  address.address[0] = random[2];

  /* encrypt with IRK */
  Octet16 p = crypto_toolbox::aes_128(irk, random, 3);

  /* set hash to be LSB of rpAddress */
  address.address[5] = p[0];
  address.address[4] = p[1];
  address.address[3] = p[2];
  return address;
}

/** This function is called when random address for local controller was
 * generated */
void btm_gen_resolve_paddr_low(const RawAddress& address) {
  tBTM_LE_RANDOM_CB* p_cb = &btm_cb.ble_ctr_cb.addr_mgnt_cb;

  BTM_TRACE_EVENT("btm_gen_resolve_paddr_low");

  p_cb->private_addr = address;

  /* set it to controller */
  btm_ble_set_random_address(p_cb->private_addr);

  p_cb->own_addr_type = BLE_ADDR_RANDOM;

  /* start a periodical timer to refresh random addr */
  uint64_t interval_ms = BTM_BLE_PRIVATE_ADDR_INT_MS;
#if (BTM_BLE_CONFORMANCE_TESTING == TRUE)
  interval_ms = btm_cb.ble_ctr_cb.rpa_tout * 1000;
#endif
  alarm_set_on_mloop(p_cb->refresh_raddr_timer, interval_ms,
                     btm_ble_refresh_raddr_timer_timeout, NULL);
}

/** This function generate a resolvable private address using local IRK */
void btm_gen_resolvable_private_addr(
    base::Callback<void(const RawAddress&)> cb) {
  BTM_TRACE_EVENT("%s", __func__);
  /* generate 3B rand as BD LSB, SRK with it, get BD MSB */
  btsnd_hcic_ble_rand(base::Bind(
      [](base::Callback<void(const RawAddress&)> cb, BT_OCTET8 random) {
        const Octet16& irk = BTM_GetDeviceIDRoot();
        cb.Run(generate_rpa_from_irk_and_rand(irk, random));
      },
      std::move(cb)));
}

/*******************************************************************************
 *
 * Function         btm_gen_non_resolve_paddr_cmpl
 *
 * Description      This is the callback function when non-resolvable private
 *                  function is generated and write to controller.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_gen_non_resolve_paddr_cmpl(BT_OCTET8 rand) {
  tBTM_LE_RANDOM_CB* p_cb = &btm_cb.ble_ctr_cb.addr_mgnt_cb;
  tBTM_BLE_ADDR_CBACK* p_cback = p_cb->p_generate_cback;
  void* p_data = p_cb->p;
  uint8_t* pp;
  RawAddress static_random;

  BTM_TRACE_EVENT("btm_gen_non_resolve_paddr_cmpl");

  p_cb->p_generate_cback = NULL;
  pp = rand;
  STREAM_TO_BDADDR(static_random, pp);
  /* mask off the 2 MSB */
  static_random.address[0] &= BLE_STATIC_PRIVATE_MSB_MASK;

  /* report complete */
  if (p_cback) (*p_cback)(static_random, p_data);
}
/*******************************************************************************
 *
 * Function         btm_gen_non_resolvable_private_addr
 *
 * Description      This function generate a non-resolvable private address.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_gen_non_resolvable_private_addr(tBTM_BLE_ADDR_CBACK* p_cback,
                                         void* p) {
  tBTM_LE_RANDOM_CB* p_mgnt_cb = &btm_cb.ble_ctr_cb.addr_mgnt_cb;

  BTM_TRACE_EVENT("btm_gen_non_resolvable_private_addr");

  if (p_mgnt_cb->p_generate_cback != NULL) return;

  p_mgnt_cb->p_generate_cback = p_cback;
  p_mgnt_cb->p = p;
  btsnd_hcic_ble_rand(base::Bind(&btm_gen_non_resolve_paddr_cmpl));
}

/*******************************************************************************
 *  Utility functions for Random address resolving
 ******************************************************************************/

/*******************************************************************************
 *
 * Function         btm_ble_init_pseudo_addr
 *
 * Description      This function is used to initialize pseudo address.
 *                  If pseudo address is not available, use dummy address
 *
 * Returns          true is updated; false otherwise.
 *
 ******************************************************************************/
bool btm_ble_init_pseudo_addr(tBTM_SEC_DEV_REC* p_dev_rec,
                              const RawAddress& new_pseudo_addr) {
  if (p_dev_rec->ble.pseudo_addr.IsEmpty()) {
    p_dev_rec->ble.pseudo_addr = new_pseudo_addr;
    return true;
  }

  return false;
}

/* Return true if given Resolvable Privae Address |rpa| matches Identity
 * Resolving Key |irk| */
static bool rpa_matches_irk(const RawAddress& rpa, const Octet16& irk) {
  /* use the 3 MSB of bd address as prand */
  uint8_t rand[3];
  rand[0] = rpa.address[2];
  rand[1] = rpa.address[1];
  rand[2] = rpa.address[0];

  /* generate X = E irk(R0, R1, R2) and R is random address 3 LSO */
  Octet16 x = crypto_toolbox::aes_128(irk, &rand[0], 3);

  rand[0] = rpa.address[5];
  rand[1] = rpa.address[4];
  rand[2] = rpa.address[3];

  if (memcmp(x.data(), &rand[0], 3) == 0) {
    // match
    return true;
  }
  // not a match
  return false;
}

/** This function checks if a RPA is resolvable by the device key.
 *  Returns true is resolvable; false otherwise.
 */
bool btm_ble_addr_resolvable(const RawAddress& rpa,
                             tBTM_SEC_DEV_REC* p_dev_rec) {
  if (!BTM_BLE_IS_RESOLVE_BDA(rpa)) return false;

  if ((p_dev_rec->device_type & BT_DEVICE_TYPE_BLE) &&
      (p_dev_rec->ble.key_type & BTM_LE_KEY_PID)) {
    BTM_TRACE_DEBUG("%s try to resolve", __func__);

    if (rpa_matches_irk(rpa, p_dev_rec->ble.keys.irk)) {
      btm_ble_init_pseudo_addr(p_dev_rec, rpa);
      return true;
    }
  }
  return false;
}

/** This function match the random address to the appointed device record,
 * starting from calculating IRK. If the record index exceeds the maximum record
 * number, matching failed and send a callback. */
static bool btm_ble_match_random_bda(void* data, void* context) {
  BTM_TRACE_EVENT("%s next iteration", __func__);
  RawAddress* random_bda = (RawAddress*)context;

  tBTM_SEC_DEV_REC* p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(data);

  BTM_TRACE_DEBUG("sec_flags = %02x device_type = %d", p_dev_rec->sec_flags,
                  p_dev_rec->device_type);

  if (!(p_dev_rec->device_type & BT_DEVICE_TYPE_BLE) ||
      !(p_dev_rec->ble.key_type & BTM_LE_KEY_PID))
    return true;

  if (rpa_matches_irk(*random_bda, p_dev_rec->ble.keys.irk)) {
    BTM_TRACE_EVENT("match is found");
    // if it was match, finish iteration, otherwise continue
    return false;
  }

  // not a match, continue iteration
  return true;
}

/** This function is called to resolve a random address.
 * Returns pointer to the security record of the device whom a random address is
 * matched to.
 */
tBTM_SEC_DEV_REC* btm_ble_resolve_random_addr(const RawAddress& random_bda) {
  BTM_TRACE_EVENT("%s", __func__);

  /* start to resolve random address */
  /* check for next security record */

  list_node_t* n = list_foreach(btm_cb.sec_dev_rec, btm_ble_match_random_bda,
                                (void*)&random_bda);
  tBTM_SEC_DEV_REC* p_dev_rec = nullptr;
  if (n != nullptr) p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(list_node(n));

  BTM_TRACE_EVENT("%s:  %sresolved", __func__,
                  (p_dev_rec == nullptr ? "not " : ""));
  return p_dev_rec;
}

/*******************************************************************************
 *  address mapping between pseudo address and real connection address
 ******************************************************************************/
/** Find the security record whose LE identity address is matching */
tBTM_SEC_DEV_REC* btm_find_dev_by_identity_addr(const RawAddress& bd_addr,
                                                uint8_t addr_type) {
#if (BLE_PRIVACY_SPT == TRUE)
  list_node_t* end = list_end(btm_cb.sec_dev_rec);
  for (list_node_t* node = list_begin(btm_cb.sec_dev_rec); node != end;
       node = list_next(node)) {
    tBTM_SEC_DEV_REC* p_dev_rec =
        static_cast<tBTM_SEC_DEV_REC*>(list_node(node));
    if (p_dev_rec->ble.identity_addr == bd_addr) {
      if ((p_dev_rec->ble.identity_addr_type & (~BLE_ADDR_TYPE_ID_BIT)) !=
          (addr_type & (~BLE_ADDR_TYPE_ID_BIT)))
        BTM_TRACE_WARNING(
            "%s find pseudo->random match with diff addr type: %d vs %d",
            __func__, p_dev_rec->ble.identity_addr_type, addr_type);

      /* found the match */
      return p_dev_rec;
    }
  }
#endif

  return NULL;
}

/*******************************************************************************
 *
 * Function         btm_identity_addr_to_random_pseudo
 *
 * Description      This function map a static BD address to a pseudo random
 *                  address in security database.
 *
 ******************************************************************************/
bool btm_identity_addr_to_random_pseudo(RawAddress* bd_addr,
                                        uint8_t* p_addr_type, bool refresh) {
#if (BLE_PRIVACY_SPT == TRUE)
  tBTM_SEC_DEV_REC* p_dev_rec =
      btm_find_dev_by_identity_addr(*bd_addr, *p_addr_type);

  BTM_TRACE_EVENT("%s", __func__);
  /* evt reported on static address, map static address to random pseudo */
  if (p_dev_rec != NULL) {
    /* if RPA offloading is supported, or 4.2 controller, do RPA refresh */
    if (refresh &&
        controller_get_interface()->get_ble_resolving_list_max_size() != 0)
      btm_ble_read_resolving_list_entry(p_dev_rec);

    /* assign the original address to be the current report address */
    if (!btm_ble_init_pseudo_addr(p_dev_rec, *bd_addr))
      *bd_addr = p_dev_rec->ble.pseudo_addr;

    *p_addr_type = p_dev_rec->ble.ble_addr_type;
    return true;
  }
#endif
  return false;
}

/*******************************************************************************
 *
 * Function         btm_random_pseudo_to_identity_addr
 *
 * Description      This function map a random pseudo address to a public
 *                  address. random_pseudo is input and output parameter
 *
 ******************************************************************************/
bool btm_random_pseudo_to_identity_addr(RawAddress* random_pseudo,
                                        uint8_t* p_identity_addr_type) {
#if (BLE_PRIVACY_SPT == TRUE)
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(*random_pseudo);

  if (p_dev_rec != NULL) {
    if (p_dev_rec->ble.in_controller_list & BTM_RESOLVING_LIST_BIT) {
      *p_identity_addr_type = p_dev_rec->ble.identity_addr_type;
      *random_pseudo = p_dev_rec->ble.identity_addr;
      if (controller_get_interface()->supports_ble_privacy())
        *p_identity_addr_type |= BLE_ADDR_TYPE_ID_BIT;
      return true;
    }
  }
#endif
  return false;
}

/*******************************************************************************
 *
 * Function         btm_ble_refresh_peer_resolvable_private_addr
 *
 * Description      This function refresh the currently used resolvable remote
 *                  private address into security database and set active
 *                  connection address.
 *
 ******************************************************************************/
void btm_ble_refresh_peer_resolvable_private_addr(const RawAddress& pseudo_bda,
                                                  const RawAddress& rpa,
                                                  uint8_t rra_type) {
#if (BLE_PRIVACY_SPT == TRUE)
  uint8_t rra_dummy = false;
  if (rpa.IsEmpty()) rra_dummy = true;

  /* update security record here, in adv event or connection complete process */
  tBTM_SEC_DEV_REC* p_sec_rec = btm_find_dev(pseudo_bda);
  if (p_sec_rec != NULL) {
    p_sec_rec->ble.cur_rand_addr = rpa;

    /* unknown, if dummy address, set to static */
    if (rra_type == BTM_BLE_ADDR_PSEUDO)
      p_sec_rec->ble.active_addr_type =
          rra_dummy ? BTM_BLE_ADDR_STATIC : BTM_BLE_ADDR_RRA;
    else
      p_sec_rec->ble.active_addr_type = rra_type;
  } else {
    BTM_TRACE_ERROR("No matching known device in record");
    return;
  }

  BTM_TRACE_DEBUG("%s: active_addr_type: %d ", __func__,
                  p_sec_rec->ble.active_addr_type);

  /* connection refresh remote address */
  tACL_CONN* p_acl = btm_bda_to_acl(p_sec_rec->bd_addr, BT_TRANSPORT_LE);
  if (p_acl == NULL)
    p_acl = btm_bda_to_acl(p_sec_rec->ble.pseudo_addr, BT_TRANSPORT_LE);

  if (p_acl != NULL) {
    if (rra_type == BTM_BLE_ADDR_PSEUDO) {
      /* use identity address, resolvable_private_addr is empty */
      if (rra_dummy) {
        p_acl->active_remote_addr_type = p_sec_rec->ble.identity_addr_type;
        p_acl->active_remote_addr = p_sec_rec->ble.identity_addr;
      } else {
        p_acl->active_remote_addr_type = BLE_ADDR_RANDOM;
        p_acl->active_remote_addr = rpa;
      }
    } else {
      p_acl->active_remote_addr_type = rra_type;
      p_acl->active_remote_addr = rpa;
    }

    BTM_TRACE_DEBUG("p_acl->active_remote_addr_type: %d ",
                    p_acl->active_remote_addr_type);
    VLOG(1) << __func__ << " conn_addr: " << p_acl->active_remote_addr;
  }
#endif
}

/*******************************************************************************
 *
 * Function         btm_ble_refresh_local_resolvable_private_addr
 *
 * Description      This function refresh the currently used resolvable private
 *                  address for the active link to the remote device
 *
 ******************************************************************************/
void btm_ble_refresh_local_resolvable_private_addr(
    const RawAddress& pseudo_addr, const RawAddress& local_rpa) {
#if (BLE_PRIVACY_SPT == TRUE)
  tACL_CONN* p = btm_bda_to_acl(pseudo_addr, BT_TRANSPORT_LE);

  if (p != NULL) {
    if (btm_cb.ble_ctr_cb.privacy_mode != BTM_PRIVACY_NONE) {
      p->conn_addr_type = BLE_ADDR_RANDOM;
      if (!local_rpa.IsEmpty())
        p->conn_addr = local_rpa;
      else
        p->conn_addr = btm_cb.ble_ctr_cb.addr_mgnt_cb.private_addr;
    } else {
      p->conn_addr_type = BLE_ADDR_PUBLIC;
      p->conn_addr = *controller_get_interface()->get_address();
    }
  }
#endif
}
