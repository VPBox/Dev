/******************************************************************************
 *
 *  Copyright 2017  The Android Open Source Project
 *  Copyright 2014  Broadcom Corporation
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

#include "bt_target.h"
#include "device/include/controller.h"
#include "osi/include/alarm.h"

#include "ble_advertiser.h"
#include "ble_advertiser_hci_interface.h"
#include "btm_int_types.h"

#include <string.h>
#include <queue>
#include <vector>

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/memory/weak_ptr.h>
#include <base/strings/string_number_conversions.h>
#include <base/time/time.h>

using base::Bind;
using base::TimeDelta;
using base::TimeTicks;
using RegisterCb =
    base::Callback<void(uint8_t /* inst_id */, uint8_t /* status */)>;
using IdTxPowerStatusCb = base::Callback<void(
    uint8_t /* inst_id */, int8_t /* tx_power */, uint8_t /* status */)>;
using SetEnableData = BleAdvertiserHciInterface::SetEnableData;
extern void btm_gen_resolvable_private_addr(
    base::Callback<void(const RawAddress& rpa)> cb);

constexpr int ADV_DATA_LEN_MAX = 251;

namespace {

bool is_connectable(uint16_t advertising_event_properties) {
  return advertising_event_properties & 0x01;
}

struct AdvertisingInstance {
  uint8_t inst_id;
  bool in_use;
  uint8_t advertising_event_properties;
  alarm_t* adv_raddr_timer;
  int8_t tx_power;
  uint16_t duration;  // 1 unit is 10ms
  uint8_t maxExtAdvEvents;
  alarm_t* timeout_timer;
  uint8_t own_address_type;
  RawAddress own_address;
  MultiAdvCb timeout_cb;
  bool address_update_required;
  bool periodic_enabled;
  uint32_t advertising_interval;  // 1 unit is 0.625 ms

  /* When true, advertising set is enabled, or last scheduled call to "LE Set
   * Extended Advertising Set Enable" is to enable this advertising set. Any
   * command scheduled when in this state will execute when the set is enabled,
   * unless enabling fails.
   *
   * When false, advertising set is disabled, or last scheduled call to "LE Set
   * Extended Advertising Set Enable" is to disable this advertising set. Any
   * command scheduled when in this state will execute when the set is disabled.
   */
  bool enable_status;
  TimeTicks enable_time;

  bool IsEnabled() { return enable_status; }

  bool IsConnectable() { return is_connectable(advertising_event_properties); }

  AdvertisingInstance(int inst_id)
      : inst_id(inst_id),
        in_use(false),
        advertising_event_properties(0),
        tx_power(0),
        duration(0),
        timeout_timer(nullptr),
        own_address_type(0),
        own_address(RawAddress::kEmpty),
        address_update_required(false),
        periodic_enabled(false),
        enable_status(false) {
    adv_raddr_timer = alarm_new_periodic("btm_ble.adv_raddr_timer");
  }

  ~AdvertisingInstance() {
    alarm_free(adv_raddr_timer);
    adv_raddr_timer = nullptr;
    if (timeout_timer) {
      alarm_free(timeout_timer);
      timeout_timer = nullptr;
    }
  }
};

void btm_ble_adv_raddr_timer_timeout(void* data);

struct closure_data {
  base::Closure user_task;
  base::Location posted_from;
};

static void alarm_closure_cb(void* p) {
  closure_data* data = (closure_data*)p;
  VLOG(1) << "executing timer scheduled at %s" << data->posted_from.ToString();
  data->user_task.Run();
  delete data;
}

// Periodic alarms are not supported, because we clean up data in callback
void alarm_set_closure(const base::Location& posted_from, alarm_t* alarm,
                       uint64_t interval_ms, base::Closure user_task) {
  closure_data* data = new closure_data;
  data->posted_from = posted_from;
  data->user_task = std::move(user_task);
  VLOG(1) << "scheduling timer %s" << data->posted_from.ToString();
  alarm_set_on_mloop(alarm, interval_ms, alarm_closure_cb, data);
}

class BleAdvertisingManagerImpl;

/* a temporary type for holding all the data needed in callbacks below*/
struct CreatorParams {
  uint8_t inst_id;
  base::WeakPtr<BleAdvertisingManagerImpl> self;
  IdTxPowerStatusCb cb;
  tBTM_BLE_ADV_PARAMS params;
  std::vector<uint8_t> advertise_data;
  std::vector<uint8_t> scan_response_data;
  tBLE_PERIODIC_ADV_PARAMS periodic_params;
  std::vector<uint8_t> periodic_data;
  uint16_t duration;
  uint8_t maxExtAdvEvents;
  RegisterCb timeout_cb;
};

using c_type = std::unique_ptr<CreatorParams>;

BleAdvertisingManager* instance;
base::WeakPtr<BleAdvertisingManagerImpl> instance_weakptr;

class BleAdvertisingManagerImpl
    : public BleAdvertisingManager,
      public BleAdvertiserHciInterface::AdvertisingEventObserver {
 public:
  BleAdvertisingManagerImpl(BleAdvertiserHciInterface* interface)
      : hci_interface(interface), weak_factory_(this) {
    hci_interface->ReadInstanceCount(
        base::Bind(&BleAdvertisingManagerImpl::ReadInstanceCountCb,
                   weak_factory_.GetWeakPtr()));
  }

  ~BleAdvertisingManagerImpl() { adv_inst.clear(); }

  void GetOwnAddress(uint8_t inst_id, GetAddressCallback cb) override {
    cb.Run(adv_inst[inst_id].own_address_type, adv_inst[inst_id].own_address);
  }

  void ReadInstanceCountCb(uint8_t instance_count) {
    this->inst_count = instance_count;
    adv_inst.reserve(inst_count);
    /* Initialize adv instance indices and IDs. */
    for (uint8_t i = 0; i < inst_count; i++) {
      adv_inst.emplace_back(i);
    }
  }

  void GenerateRpa(base::Callback<void(const RawAddress&)> cb) {
    btm_gen_resolvable_private_addr(std::move(cb));
  }

  void ConfigureRpa(AdvertisingInstance* p_inst, MultiAdvCb configuredCb) {
    /* Connectable advertising set must be disabled when updating RPA */
    bool restart = p_inst->IsEnabled() && p_inst->IsConnectable();

    // If there is any form of timeout on the set, schedule address update when
    // the set stops, because there is no good way to compute new timeout value.
    // Maximum duration value is around 10 minutes, so this is safe.
    if (restart && (p_inst->duration || p_inst->maxExtAdvEvents)) {
      p_inst->address_update_required = true;
      configuredCb.Run(0x01);
      return;
    }

    GenerateRpa(Bind(
        [](AdvertisingInstance* p_inst, MultiAdvCb configuredCb,
           const RawAddress& bda) {
          /* Connectable advertising set must be disabled when updating RPA */
          bool restart = p_inst->IsEnabled() && p_inst->IsConnectable();

          if (!instance_weakptr.get()) return;
          auto hci_interface = instance_weakptr.get()->GetHciInterface();

          if (restart) {
            p_inst->enable_status = false;
            hci_interface->Enable(false, p_inst->inst_id, 0x00, 0x00,
                                  base::DoNothing());
          }

          /* set it to controller */
          hci_interface->SetRandomAddress(
              p_inst->inst_id, bda,
              Bind(
                  [](AdvertisingInstance* p_inst, RawAddress bda,
                     MultiAdvCb configuredCb, uint8_t status) {
                    p_inst->own_address = bda;
                    configuredCb.Run(0x00);
                  },
                  p_inst, bda, configuredCb));

          if (restart) {
            p_inst->enable_status = true;
            hci_interface->Enable(true, p_inst->inst_id, 0x00, 0x00,
                                  base::DoNothing());
          }
        },
        p_inst, std::move(configuredCb)));
  }

  void RegisterAdvertiser(
      base::Callback<void(uint8_t /* inst_id */, uint8_t /* status */)> cb)
      override {
    AdvertisingInstance* p_inst = &adv_inst[0];
    for (uint8_t i = 0; i < inst_count; i++, p_inst++) {
      if (p_inst->in_use) continue;

      p_inst->in_use = true;

      // set up periodic timer to update address.
      if (BTM_BleLocalPrivacyEnabled()) {
        p_inst->own_address_type = BLE_ADDR_RANDOM;
        GenerateRpa(Bind(
            [](AdvertisingInstance* p_inst,
               base::Callback<void(uint8_t /* inst_id */, uint8_t /* status */)>
                   cb,
               const RawAddress& bda) {
              p_inst->own_address = bda;

              alarm_set_on_mloop(p_inst->adv_raddr_timer,
                                 BTM_BLE_PRIVATE_ADDR_INT_MS,
                                 btm_ble_adv_raddr_timer_timeout, p_inst);
              cb.Run(p_inst->inst_id, BTM_BLE_MULTI_ADV_SUCCESS);
            },
            p_inst, cb));
      } else {
        p_inst->own_address_type = BLE_ADDR_PUBLIC;
        p_inst->own_address = *controller_get_interface()->get_address();

        cb.Run(p_inst->inst_id, BTM_BLE_MULTI_ADV_SUCCESS);
      }
      return;
    }

    LOG(INFO) << "no free advertiser instance";
    cb.Run(0xFF, ADVERTISE_FAILED_TOO_MANY_ADVERTISERS);
  }

  void StartAdvertising(uint8_t advertiser_id, MultiAdvCb cb,
                        tBTM_BLE_ADV_PARAMS* params,
                        std::vector<uint8_t> advertise_data,
                        std::vector<uint8_t> scan_response_data, int duration,
                        MultiAdvCb timeout_cb) override {
    /* a temporary type for holding all the data needed in callbacks below*/
    struct CreatorParams {
      uint8_t inst_id;
      base::WeakPtr<BleAdvertisingManagerImpl> self;
      MultiAdvCb cb;
      tBTM_BLE_ADV_PARAMS params;
      std::vector<uint8_t> advertise_data;
      std::vector<uint8_t> scan_response_data;
      int duration;
      MultiAdvCb timeout_cb;
    };

    std::unique_ptr<CreatorParams> c;
    c.reset(new CreatorParams());

    c->self = weak_factory_.GetWeakPtr();
    c->cb = std::move(cb);
    c->params = *params;
    c->advertise_data = std::move(advertise_data);
    c->scan_response_data = std::move(scan_response_data);
    c->duration = duration;
    c->timeout_cb = std::move(timeout_cb);
    c->inst_id = advertiser_id;

    using c_type = std::unique_ptr<CreatorParams>;

    // this code is intentionally left formatted this way to highlight the
    // asynchronous flow
    // clang-format off
    c->self->SetParameters(c->inst_id, &c->params, Bind(
      [](c_type c, uint8_t status, int8_t tx_power) {
        if (!c->self) {
          LOG(INFO) << "Stack was shut down";
          return;
        }

        if (status) {
          LOG(ERROR) << "setting parameters failed, status: " << +status;
          c->cb.Run(status);
          return;
        }

        c->self->adv_inst[c->inst_id].tx_power = tx_power;

        const RawAddress& rpa = c->self->adv_inst[c->inst_id].own_address;
        c->self->GetHciInterface()->SetRandomAddress(c->inst_id, rpa, Bind(
          [](c_type c, uint8_t status) {
            if (!c->self) {
              LOG(INFO) << "Stack was shut down";
              return;
            }

            if (status != 0) {
              LOG(ERROR) << "setting random address failed, status: " << +status;
              c->cb.Run(status);
              return;
            }

            c->self->SetData(c->inst_id, false, std::move(c->advertise_data), Bind(
              [](c_type c, uint8_t status) {
                if (!c->self) {
                  LOG(INFO) << "Stack was shut down";
                  return;
                }

                if (status != 0) {
                  LOG(ERROR) << "setting advertise data failed, status: " << +status;
                  c->cb.Run(status);
                  return;
                }

                c->self->SetData(c->inst_id, true, std::move(c->scan_response_data), Bind(
                  [](c_type c, uint8_t status) {
                    if (!c->self) {
                      LOG(INFO) << "Stack was shut down";
                      return;
                    }

                    if (status != 0) {
                      LOG(ERROR) << "setting scan response data failed, status: " << +status;
                      c->cb.Run(status);
                      return;
                    }

                    c->self->Enable(c->inst_id, true, c->cb, c->duration, 0, std::move(c->timeout_cb));

                }, base::Passed(&c)));
            }, base::Passed(&c)));
        }, base::Passed(&c)));
    }, base::Passed(&c)));
    // clang-format on
  }

  void StartAdvertisingSet(IdTxPowerStatusCb cb, tBTM_BLE_ADV_PARAMS* params,
                           std::vector<uint8_t> advertise_data,
                           std::vector<uint8_t> scan_response_data,
                           tBLE_PERIODIC_ADV_PARAMS* periodic_params,
                           std::vector<uint8_t> periodic_data,
                           uint16_t duration, uint8_t maxExtAdvEvents,
                           RegisterCb timeout_cb) override {
    std::unique_ptr<CreatorParams> c;
    c.reset(new CreatorParams());

    c->self = weak_factory_.GetWeakPtr();
    c->cb = std::move(cb);
    c->params = *params;
    c->advertise_data = std::move(advertise_data);
    c->scan_response_data = std::move(scan_response_data);
    c->periodic_params = *periodic_params;
    c->periodic_data = std::move(periodic_data);
    c->duration = duration;
    c->maxExtAdvEvents = maxExtAdvEvents;
    c->timeout_cb = std::move(timeout_cb);

    // this code is intentionally left formatted this way to highlight the
    // asynchronous flow
    // clang-format off
    c->self->RegisterAdvertiser(Bind(
      [](c_type c, uint8_t advertiser_id, uint8_t status) {
        if (!c->self) {
          LOG(INFO) << "Stack was shut down";
          return;
        }

        if (status != 0) {
          LOG(ERROR) << " failed, status: " << +status;
          c->cb.Run(0, 0, status);
          return;
        }

        c->inst_id = advertiser_id;

        c->self->SetParameters(c->inst_id, &c->params, Bind(
          [](c_type c, uint8_t status, int8_t tx_power) {
            if (!c->self) {
              LOG(INFO) << "Stack was shut down";
              return;
            }

            if (status != 0) {
              c->self->Unregister(c->inst_id);
              LOG(ERROR) << "setting parameters failed, status: " << +status;
              c->cb.Run(0, 0, status);
              return;
            }

            c->self->adv_inst[c->inst_id].tx_power = tx_power;

            if (c->self->adv_inst[c->inst_id].own_address_type == BLE_ADDR_PUBLIC) {
              c->self->StartAdvertisingSetAfterAddressPart(std::move(c));
              return;
            }

            //own_address_type == BLE_ADDR_RANDOM
            const RawAddress& rpa = c->self->adv_inst[c->inst_id].own_address;
            c->self->GetHciInterface()->SetRandomAddress(c->inst_id, rpa, Bind(
              [](c_type c, uint8_t status) {
                if (!c->self) {
                  LOG(INFO) << "Stack was shut down";
                  return;
                }

                if (status != 0) {
                  c->self->Unregister(c->inst_id);
                  LOG(ERROR) << "setting random address failed, status: " << +status;
                  c->cb.Run(0, 0, status);
                  return;
                }

                c->self->StartAdvertisingSetAfterAddressPart(std::move(c));
          }, base::Passed(&c)));
        }, base::Passed(&c)));
    }, base::Passed(&c)));
    // clang-format on
  }

  void StartAdvertisingSetAfterAddressPart(c_type c) {
    c->self->SetData(
        c->inst_id, false, std::move(c->advertise_data),
        Bind(
            [](c_type c, uint8_t status) {
              if (!c->self) {
                LOG(INFO) << "Stack was shut down";
                return;
              }

              if (status != 0) {
                c->self->Unregister(c->inst_id);
                LOG(ERROR) << "setting advertise data failed, status: "
                           << +status;
                c->cb.Run(0, 0, status);
                return;
              }

              c->self->SetData(
                  c->inst_id, true, std::move(c->scan_response_data),
                  Bind(
                      [](c_type c, uint8_t status) {
                        if (!c->self) {
                          LOG(INFO) << "Stack was shut down";
                          return;
                        }

                        if (status != 0) {
                          c->self->Unregister(c->inst_id);
                          LOG(ERROR)
                              << "setting scan response data failed, status: "
                              << +status;
                          c->cb.Run(0, 0, status);
                          return;
                        }

                        if (c->periodic_params.enable) {
                          c->self->StartAdvertisingSetPeriodicPart(
                              std::move(c));
                        } else {
                          c->self->StartAdvertisingSetFinish(std::move(c));
                        }
                      },
                      base::Passed(&c)));
            },
            base::Passed(&c)));
  }

  void StartAdvertisingSetPeriodicPart(c_type c) {
    // this code is intentionally left formatted this way to highlight the
    // asynchronous flow
    // clang-format off
    c->self->SetPeriodicAdvertisingParameters(c->inst_id, &c->periodic_params, Bind(
      [](c_type c, uint8_t status) {
        if (!c->self) {
          LOG(INFO) << "Stack was shut down";
          return;
        }

        if (status != 0) {
          c->self->Unregister(c->inst_id);
          LOG(ERROR) << "setting periodic parameters failed, status: " << +status;
          c->cb.Run(0, 0, status);
          return;
        }

        c->self->SetPeriodicAdvertisingData(c->inst_id, std::move(c->periodic_data), Bind(
          [](c_type c, uint8_t status) {
            if (!c->self) {
              LOG(INFO) << "Stack was shut down";
              return;
            }

            if (status != 0) {
              c->self->Unregister(c->inst_id);
              LOG(ERROR) << "setting periodic parameters failed, status: " << +status;
              c->cb.Run(0, 0, status);
              return;
            }

            c->self->SetPeriodicAdvertisingEnable(c->inst_id, true, Bind(
              [](c_type c, uint8_t status) {
                if (!c->self) {
                  LOG(INFO) << "Stack was shut down";
                  return;
                }

                if (status != 0) {
                  c->self->Unregister(c->inst_id);
                  LOG(ERROR) << "enabling periodic advertising failed, status: " << +status;
                  c->cb.Run(0, 0, status);
                  return;
                }

                c->self->StartAdvertisingSetFinish(std::move(c));

              }, base::Passed(&c)));
        }, base::Passed(&c)));
    }, base::Passed(&c)));
    // clang-format on
  }

  void StartAdvertisingSetFinish(c_type c) {
    uint8_t inst_id = c->inst_id;
    uint16_t duration = c->duration;
    uint8_t maxExtAdvEvents = c->maxExtAdvEvents;
    RegisterCb timeout_cb = std::move(c->timeout_cb);
    base::WeakPtr<BleAdvertisingManagerImpl> self = c->self;
    MultiAdvCb enable_cb = Bind(
        [](c_type c, uint8_t status) {
          if (!c->self) {
            LOG(INFO) << "Stack was shut down";
            return;
          }

          if (status != 0) {
            c->self->Unregister(c->inst_id);
            LOG(ERROR) << "enabling advertiser failed, status: " << +status;
            c->cb.Run(0, 0, status);
            return;
          }
          int8_t tx_power = c->self->adv_inst[c->inst_id].tx_power;
          c->cb.Run(c->inst_id, tx_power, status);
        },
        base::Passed(&c));

    self->Enable(inst_id, true, std::move(enable_cb), duration, maxExtAdvEvents,
                 Bind(std::move(timeout_cb), inst_id));
  }

  void EnableWithTimerCb(uint8_t inst_id, MultiAdvCb enable_cb, int duration,
                         MultiAdvCb timeout_cb, uint8_t status) {
    VLOG(1) << __func__ << " inst_id: " << +inst_id;
    AdvertisingInstance* p_inst = &adv_inst[inst_id];

    // Run the regular enable callback
    enable_cb.Run(status);

    p_inst->timeout_timer = alarm_new("btm_ble.adv_timeout");

    base::Closure cb = Bind(
        &BleAdvertisingManagerImpl::Enable, weak_factory_.GetWeakPtr(), inst_id,
        0 /* disable */, std::move(timeout_cb), 0, 0, base::DoNothing());

    // schedule disable when the timeout passes
    alarm_set_closure(FROM_HERE, p_inst->timeout_timer, duration * 10,
                      std::move(cb));
  }

  void Enable(uint8_t inst_id, bool enable, MultiAdvCb cb, uint16_t duration,
              uint8_t maxExtAdvEvents, MultiAdvCb timeout_cb) override {
    VLOG(1) << __func__ << " inst_id: " << +inst_id;
    if (inst_id >= inst_count) {
      LOG(ERROR) << "bad instance id " << +inst_id;
      return;
    }

    AdvertisingInstance* p_inst = &adv_inst[inst_id];
    VLOG(1) << __func__ << " enable: " << enable << ", duration: " << +duration;
    if (!p_inst->in_use) {
      LOG(ERROR) << "Invalid or no active instance";
      cb.Run(BTM_BLE_MULTI_ADV_FAILURE);
      return;
    }

    if (enable && (duration || maxExtAdvEvents)) {
      p_inst->timeout_cb = std::move(timeout_cb);
    }

    p_inst->duration = duration;
    p_inst->maxExtAdvEvents = maxExtAdvEvents;

    if (enable && p_inst->address_update_required) {
      p_inst->address_update_required = false;
      ConfigureRpa(p_inst, base::Bind(&BleAdvertisingManagerImpl::EnableFinish,
                                      weak_factory_.GetWeakPtr(), p_inst,
                                      enable, std::move(cb)));
      return;
    }

    EnableFinish(p_inst, enable, std::move(cb), 0);
  }

  void EnableFinish(AdvertisingInstance* p_inst, bool enable, MultiAdvCb cb,
                    uint8_t status) {
    MultiAdvCb myCb;
    if (enable && p_inst->duration) {
      // TODO(jpawlowski): HCI implementation that can't do duration should
      // emulate it, not EnableWithTimerCb.
      myCb = Bind(&BleAdvertisingManagerImpl::EnableWithTimerCb,
                  weak_factory_.GetWeakPtr(), p_inst->inst_id, std::move(cb),
                  p_inst->duration, p_inst->timeout_cb);
    } else {
      myCb = std::move(cb);

      if (p_inst->timeout_timer) {
        alarm_cancel(p_inst->timeout_timer);
        alarm_free(p_inst->timeout_timer);
        p_inst->timeout_timer = nullptr;
      }
    }

    if (enable) p_inst->enable_time = TimeTicks::Now();
    p_inst->enable_status = enable;
    GetHciInterface()->Enable(enable, p_inst->inst_id, p_inst->duration,
                              p_inst->maxExtAdvEvents, std::move(myCb));
  }

  void SetParameters(uint8_t inst_id, tBTM_BLE_ADV_PARAMS* p_params,
                     ParametersCb cb) override {
    VLOG(1) << __func__ << " inst_id: " << +inst_id;
    if (inst_id >= inst_count) {
      LOG(ERROR) << "bad instance id " << +inst_id;
      return;
    }

    AdvertisingInstance* p_inst = &adv_inst[inst_id];
    if (!p_inst->in_use) {
      LOG(ERROR) << "adv instance not in use" << +inst_id;
      cb.Run(BTM_BLE_MULTI_ADV_FAILURE, 0);
      return;
    }

    // TODO: disable only if was enabled, currently no use scenario needs
    // that,
    // we always set parameters before enabling
    // GetHciInterface()->Enable(false, inst_id, base::DoNothing());
    p_inst->advertising_event_properties =
        p_params->advertising_event_properties;
    p_inst->tx_power = p_params->tx_power;
    p_inst->advertising_interval = p_params->adv_int_min;
    const RawAddress& peer_address = RawAddress::kEmpty;

    GetHciInterface()->SetParameters(
        p_inst->inst_id, p_params->advertising_event_properties,
        p_params->adv_int_min, p_params->adv_int_max, p_params->channel_map,
        p_inst->own_address_type, p_inst->own_address, 0x00, peer_address,
        p_params->adv_filter_policy, p_inst->tx_power,
        p_params->primary_advertising_phy, 0x00,
        p_params->secondary_advertising_phy, 0x01 /* TODO: proper SID */,
        p_params->scan_request_notification_enable, cb);

    // TODO: re-enable only if it was enabled, properly call
    // SetParamsCallback
    // currently no use scenario needs that
    // GetHciInterface()->Enable(true, inst_id, BTM_BleUpdateAdvInstParamCb);
  }

  void SetData(uint8_t inst_id, bool is_scan_rsp, std::vector<uint8_t> data,
               MultiAdvCb cb) override {
    VLOG(1) << __func__ << " inst_id: " << +inst_id;
    if (inst_id >= inst_count) {
      LOG(ERROR) << "bad instance id " << +inst_id;
      return;
    }

    AdvertisingInstance* p_inst = &adv_inst[inst_id];
    VLOG(1) << "is_scan_rsp = " << is_scan_rsp;

    if (!is_scan_rsp && is_connectable(p_inst->advertising_event_properties)) {
      uint8_t flags_val = BTM_GENERAL_DISCOVERABLE;

      if (p_inst->duration) flags_val = BTM_LIMITED_DISCOVERABLE;

      std::vector<uint8_t> flags;
      flags.push_back(2);  // length
      flags.push_back(HCI_EIR_FLAGS_TYPE);
      flags.push_back(flags_val);

      data.insert(data.begin(), flags.begin(), flags.end());
    }

    // Find and fill TX Power with the correct value
    if (data.size()) {
      size_t i = 0;
      while (i < data.size()) {
        uint8_t type = data[i + 1];
        if (type == HCI_EIR_TX_POWER_LEVEL_TYPE) {
          data[i + 2] = adv_inst[inst_id].tx_power;
        }
        i += data[i] + 1;
      }
    }

    VLOG(1) << "data is: " << base::HexEncode(data.data(), data.size());
    DivideAndSendData(
        inst_id, data, cb,
        base::Bind(&BleAdvertisingManagerImpl::SetDataAdvDataSender,
                   weak_factory_.GetWeakPtr(), is_scan_rsp));
  }

  void SetDataAdvDataSender(uint8_t is_scan_rsp, uint8_t inst_id,
                            uint8_t operation, uint8_t length, uint8_t* data,
                            MultiAdvCb cb) {
    if (is_scan_rsp)
      GetHciInterface()->SetScanResponseData(inst_id, operation, 0x01, length,
                                             data, cb);
    else
      GetHciInterface()->SetAdvertisingData(inst_id, operation, 0x01, length,
                                            data, cb);
  }

  using DataSender = base::Callback<void(
      uint8_t /*inst_id*/, uint8_t /* operation */, uint8_t /* length */,
      uint8_t* /* data */, MultiAdvCb /* done */)>;

  void DivideAndSendData(int inst_id, std::vector<uint8_t> data,
                         MultiAdvCb done_cb, DataSender sender) {
    DivideAndSendDataRecursively(true, inst_id, std::move(data), 0,
                                 std::move(done_cb), std::move(sender), 0);
  }

  static void DivideAndSendDataRecursively(bool isFirst, int inst_id,
                                           std::vector<uint8_t> data,
                                           int offset, MultiAdvCb done_cb,
                                           DataSender sender, uint8_t status) {
    constexpr uint8_t INTERMEDIATE =
        0x00;                        // Intermediate fragment of fragmented data
    constexpr uint8_t FIRST = 0x01;  // First fragment of fragmented data
    constexpr uint8_t LAST = 0x02;   // Last fragment of fragmented data
    constexpr uint8_t COMPLETE = 0x03;  // Complete extended advertising data

    int dataSize = (int)data.size();
    if (status != 0 || (!isFirst && offset == dataSize)) {
      /* if we got error writing data, or reached the end of data */
      done_cb.Run(status);
      return;
    }

    bool moreThanOnePacket = dataSize - offset > ADV_DATA_LEN_MAX;
    uint8_t operation = isFirst ? moreThanOnePacket ? FIRST : COMPLETE
                                : moreThanOnePacket ? INTERMEDIATE : LAST;
    int length = moreThanOnePacket ? ADV_DATA_LEN_MAX : dataSize - offset;
    int newOffset = offset + length;

    sender.Run(
        inst_id, operation, length, data.data() + offset,
        Bind(&BleAdvertisingManagerImpl::DivideAndSendDataRecursively, false,
             inst_id, std::move(data), newOffset, std::move(done_cb), sender));
  }

  void SetPeriodicAdvertisingParameters(uint8_t inst_id,
                                        tBLE_PERIODIC_ADV_PARAMS* params,
                                        MultiAdvCb cb) override {
    VLOG(1) << __func__ << " inst_id: " << +inst_id;

    GetHciInterface()->SetPeriodicAdvertisingParameters(
        inst_id, params->min_interval, params->max_interval,
        params->periodic_advertising_properties, cb);
  }

  void SetPeriodicAdvertisingData(uint8_t inst_id, std::vector<uint8_t> data,
                                  MultiAdvCb cb) override {
    VLOG(1) << __func__ << " inst_id: " << +inst_id;

    VLOG(1) << "data is: " << base::HexEncode(data.data(), data.size());

    DivideAndSendData(
        inst_id, data, cb,
        base::Bind(&BleAdvertiserHciInterface::SetPeriodicAdvertisingData,
                   base::Unretained(GetHciInterface())));
  }

  void SetPeriodicAdvertisingEnable(uint8_t inst_id, uint8_t enable,
                                    MultiAdvCb cb) override {
    VLOG(1) << __func__ << " inst_id: " << +inst_id << ", enable: " << +enable;

    AdvertisingInstance* p_inst = &adv_inst[inst_id];
    if (!p_inst->in_use) {
      LOG(ERROR) << "Invalid or not active instance";
      cb.Run(BTM_BLE_MULTI_ADV_FAILURE);
      return;
    }

    MultiAdvCb enable_cb = Bind(
        [](AdvertisingInstance* p_inst, uint8_t enable, MultiAdvCb cb,
           uint8_t status) {
          VLOG(1) << "periodc adv enable cb: inst_id: " << +p_inst->inst_id
                  << ", enable: " << +enable << ", status: " << std::hex
                  << +status;
          if (!status) p_inst->periodic_enabled = enable;

          cb.Run(status);
        },
        p_inst, enable, std::move(cb));

    GetHciInterface()->SetPeriodicAdvertisingEnable(enable, inst_id,
                                                    std::move(enable_cb));
  }

  void Unregister(uint8_t inst_id) override {
    AdvertisingInstance* p_inst = &adv_inst[inst_id];

    VLOG(1) << __func__ << " inst_id: " << +inst_id;
    if (inst_id >= inst_count) {
      LOG(ERROR) << "bad instance id " << +inst_id;
      return;
    }

    if (adv_inst[inst_id].IsEnabled()) {
      p_inst->enable_status = false;
      GetHciInterface()->Enable(false, inst_id, 0x00, 0x00, base::DoNothing());
    }

    if (p_inst->periodic_enabled) {
      p_inst->periodic_enabled = false;
      GetHciInterface()->SetPeriodicAdvertisingEnable(false, inst_id,
                                                      base::DoNothing());
    }

    alarm_cancel(p_inst->adv_raddr_timer);
    p_inst->in_use = false;
    GetHciInterface()->RemoveAdvertisingSet(inst_id, base::DoNothing());
    p_inst->address_update_required = false;
  }

  void RecomputeTimeout(AdvertisingInstance* inst, TimeTicks now) {
    TimeDelta duration = now - inst->enable_time;
    bool cb_fired = false;
    if (inst->duration) {
      int durationDone = (duration.InMilliseconds() / 10);
      if (durationDone + 1 >= inst->duration) {
        inst->enable_status = false;
        inst->timeout_cb.Run(0 /* TODO: STATUS HERE?*/);
        cb_fired = true;
      } else {
        inst->duration = inst->duration - durationDone;
      }
    }

    if (inst->maxExtAdvEvents && !cb_fired) {
      int eventsDone =
          (duration.InMilliseconds() / (inst->advertising_interval * 5 / 8));

      if (eventsDone + 1 >= inst->maxExtAdvEvents) {
        inst->enable_status = false;
        inst->timeout_cb.Run(0 /* TODO: STATUS HERE?*/);
      } else {
        inst->maxExtAdvEvents = inst->maxExtAdvEvents - eventsDone;
      }
    }
  }

  void Suspend() override {
    std::vector<SetEnableData> sets;

    for (AdvertisingInstance& inst : adv_inst) {
      if (!inst.in_use || !inst.enable_status) continue;

      if (inst.duration || inst.maxExtAdvEvents)
        RecomputeTimeout(&inst, TimeTicks::Now());

      sets.emplace_back(SetEnableData{.handle = inst.inst_id});
    }

    if (!sets.empty())
      GetHciInterface()->Enable(false, sets, base::DoNothing());
  }

  void Resume() override {
    std::vector<SetEnableData> sets;

    for (const AdvertisingInstance& inst : adv_inst) {
      if (inst.in_use && inst.enable_status) {
        sets.emplace_back(SetEnableData{
            .handle = inst.inst_id,
            .duration = inst.duration,
            .max_extended_advertising_events = inst.maxExtAdvEvents});
      }
    }

    if (!sets.empty()) GetHciInterface()->Enable(true, sets, base::DoNothing());
  }

  void OnAdvertisingSetTerminated(
      uint8_t status, uint8_t advertising_handle, uint16_t connection_handle,
      uint8_t num_completed_extended_adv_events) override {
    AdvertisingInstance* p_inst = &adv_inst[advertising_handle];
    VLOG(1) << __func__ << "status: " << loghex(status)
            << ", advertising_handle: " << loghex(advertising_handle)
            << ", connection_handle: " << loghex(connection_handle);

    if (status == HCI_ERR_LIMIT_REACHED ||
        status == HCI_ERR_ADVERTISING_TIMEOUT) {
      // either duration elapsed, or maxExtAdvEvents reached
      p_inst->enable_status = false;

      if (p_inst->timeout_cb.is_null()) {
        LOG(INFO) << __func__ << "No timeout callback";
        return;
      }

      p_inst->timeout_cb.Run(status);
      return;
    }

    if (BTM_BleLocalPrivacyEnabled() &&
        advertising_handle <= BTM_BLE_MULTI_ADV_MAX) {
      btm_acl_update_conn_addr(connection_handle, p_inst->own_address);
    }

    VLOG(1) << "reneabling advertising";

    if (p_inst->in_use) {
      // TODO(jpawlowski): we don't really allow to do directed advertising
      // right now. This should probably be removed, check with Andre.
      if ((p_inst->advertising_event_properties & 0x0C) == 0) {
        /* directed advertising bits not set */

        RecomputeTimeout(p_inst, TimeTicks::Now());
        if (p_inst->enable_status) {
          GetHciInterface()->Enable(true, advertising_handle, p_inst->duration,
                                    p_inst->maxExtAdvEvents, base::DoNothing());
        }

      } else {
        /* mark directed adv as disabled if adv has been stopped */
        p_inst->in_use = false;
      }
    }
  }

  base::WeakPtr<BleAdvertisingManagerImpl> GetWeakPtr() {
    return weak_factory_.GetWeakPtr();
  }

  void CancelAdvAlarms() {
    AdvertisingInstance* p_inst = &adv_inst[0];
    for (uint8_t i = 0; i < inst_count; i++, p_inst++) {
      if (p_inst->timeout_timer) {
        alarm_cancel(p_inst->timeout_timer);
      }
      if (p_inst->adv_raddr_timer) {
        alarm_cancel(p_inst->adv_raddr_timer);
      }
    }
  }

 private:
  BleAdvertiserHciInterface* GetHciInterface() { return hci_interface; }

  BleAdvertiserHciInterface* hci_interface = nullptr;
  std::vector<AdvertisingInstance> adv_inst;
  uint8_t inst_count;

  // Member variables should appear before the WeakPtrFactory, to ensure
  // that any WeakPtrs are invalidated before its members
  // variable's destructors are executed, rendering them invalid.
  base::WeakPtrFactory<BleAdvertisingManagerImpl> weak_factory_;
};

void btm_ble_adv_raddr_timer_timeout(void* data) {
  BleAdvertisingManagerImpl* ptr = instance_weakptr.get();
  if (ptr) ptr->ConfigureRpa((AdvertisingInstance*)data, base::DoNothing());
}
}  // namespace

void BleAdvertisingManager::Initialize(BleAdvertiserHciInterface* interface) {
  instance = new BleAdvertisingManagerImpl(interface);
  instance_weakptr = ((BleAdvertisingManagerImpl*)instance)->GetWeakPtr();
}

bool BleAdvertisingManager::IsInitialized() { return instance; }

base::WeakPtr<BleAdvertisingManager> BleAdvertisingManager::Get() {
  return instance_weakptr;
};

void BleAdvertisingManager::CleanUp() {
  if (instance_weakptr.get()) instance_weakptr.get()->CancelAdvAlarms();

  delete instance;
  instance = nullptr;
};

/**
 * This function initialize the advertising manager.
 **/
void btm_ble_adv_init() {
  BleAdvertiserHciInterface::Initialize();
  BleAdvertisingManager::Initialize(BleAdvertiserHciInterface::Get());
  BleAdvertiserHciInterface::Get()->SetAdvertisingEventObserver(
      (BleAdvertisingManagerImpl*)BleAdvertisingManager::Get().get());

  if (BleAdvertiserHciInterface::Get()->QuirkAdvertiserZeroHandle()) {
    // If handle 0 can't be used, register advertiser for it, but never use it.
    BleAdvertisingManager::Get().get()->RegisterAdvertiser(base::DoNothing());
  }
}

/*******************************************************************************
 *
 * Function         btm_ble_multi_adv_cleanup
 *
 * Description      This function cleans up multi adv control block.
 *
 * Parameters
 * Returns          void
 *
 ******************************************************************************/
void btm_ble_multi_adv_cleanup(void) {
  BleAdvertisingManager::CleanUp();
  BleAdvertiserHciInterface::CleanUp();
}

// TODO(jpawlowski): Find a nicer way to test RecomputeTimeout without exposing
// AdvertisingInstance
bool timeout_triggered = false;
void test_timeout_cb(uint8_t status) { timeout_triggered = true; }

// verify that if duration passed, or is about to pass, recomputation will shut
// down the advertiser completly
void testRecomputeTimeout1() {
  auto manager = (BleAdvertisingManagerImpl*)BleAdvertisingManager::Get().get();

  TimeTicks start = TimeTicks::Now();
  TimeTicks end = start + TimeDelta::FromMilliseconds(111);
  AdvertisingInstance test1(0);
  test1.enable_status = true;
  test1.enable_time = start;
  test1.duration = 12 /*120ms*/;
  test1.timeout_cb = Bind(&test_timeout_cb);

  manager->RecomputeTimeout(&test1, end);

  CHECK(timeout_triggered);
  timeout_triggered = false;
  CHECK(!test1.enable_status);
}

// verify that duration and maxExtAdvEvents are properly adjusted when
// recomputing.
void testRecomputeTimeout2() {
  auto manager = (BleAdvertisingManagerImpl*)BleAdvertisingManager::Get().get();

  TimeTicks start = TimeTicks::Now();
  TimeTicks end = start + TimeDelta::FromMilliseconds(250);
  AdvertisingInstance test1(0);
  test1.enable_status = true;
  test1.enable_time = start;
  test1.duration = 50 /*500ms*/;
  test1.maxExtAdvEvents = 50;
  test1.advertising_interval = 16 /* 10 ms */;
  test1.timeout_cb = Bind(&test_timeout_cb);

  manager->RecomputeTimeout(&test1, end);

  CHECK(!timeout_triggered);
  CHECK(test1.enable_status);
  CHECK(test1.duration == 25);
  CHECK(test1.maxExtAdvEvents == 25);
}

// verify that if maxExtAdvEvents were sent, or are close to end, recomputation
// wil shut down the advertiser completly
void testRecomputeTimeout3() {
  auto manager = (BleAdvertisingManagerImpl*)BleAdvertisingManager::Get().get();

  TimeTicks start = TimeTicks::Now();
  TimeTicks end = start + TimeDelta::FromMilliseconds(495);
  AdvertisingInstance test1(0);
  test1.enable_status = true;
  test1.enable_time = start;
  test1.maxExtAdvEvents = 50;
  test1.advertising_interval = 16 /* 10 ms */;
  test1.timeout_cb = Bind(&test_timeout_cb);

  manager->RecomputeTimeout(&test1, end);

  CHECK(timeout_triggered);
  timeout_triggered = false;
  CHECK(!test1.enable_status);
}
