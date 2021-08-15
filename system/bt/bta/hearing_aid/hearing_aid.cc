/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include "bta_hearing_aid_api.h"

#include "bta_gatt_api.h"
#include "bta_gatt_queue.h"
#include "btm_int.h"
#include "device/include/controller.h"
#include "embdrv/g722/g722_enc_dec.h"
#include "gap_api.h"
#include "gatt_api.h"
#include "osi/include/properties.h"

#include <base/bind.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <hardware/bt_hearing_aid.h>
#include <vector>

using base::Closure;
using bluetooth::Uuid;
using bluetooth::hearing_aid::ConnectionState;

// The MIN_CE_LEN parameter for Connection Parameters based on the current
// Connection Interval
constexpr uint16_t MIN_CE_LEN_10MS_CI = 0x0006;
constexpr uint16_t MIN_CE_LEN_20MS_CI = 0x000C;
constexpr uint16_t CONNECTION_INTERVAL_10MS_PARAM = 0x0008;
constexpr uint16_t CONNECTION_INTERVAL_20MS_PARAM = 0x0010;

void btif_storage_add_hearing_aid(const HearingDevice& dev_info);
bool btif_storage_get_hearing_aid_prop(
    const RawAddress& address, uint8_t* capabilities, uint64_t* hi_sync_id,
    uint16_t* render_delay, uint16_t* preparation_delay, uint16_t* codecs);

constexpr uint8_t CODEC_G722_16KHZ = 0x01;
constexpr uint8_t CODEC_G722_24KHZ = 0x02;

// audio control point opcodes
constexpr uint8_t CONTROL_POINT_OP_START = 0x01;
constexpr uint8_t CONTROL_POINT_OP_STOP = 0x02;
constexpr uint8_t CONTROL_POINT_OP_STATE_CHANGE = 0x03;

constexpr uint8_t STATE_CHANGE_OTHER_SIDE_DISCONNECTED = 0x00;
constexpr uint8_t STATE_CHANGE_OTHER_SIDE_CONNECTED = 0x01;
constexpr uint8_t STATE_CHANGE_CONN_UPDATE = 0x02;

// used to mark current_volume as not yet known, or possibly old
constexpr int8_t VOLUME_UNKNOWN = 127;
constexpr int8_t VOLUME_MIN = -127;

// audio type
constexpr uint8_t AUDIOTYPE_UNKNOWN = 0x00;

// Status of the other side Hearing Aids device
constexpr uint8_t OTHER_SIDE_NOT_STREAMING = 0x00;
constexpr uint8_t OTHER_SIDE_IS_STREAMING = 0x01;

// This ADD_RENDER_DELAY_INTERVALS is the number of connection intervals when
// the audio data packet is send by Audio Engine to when the Hearing Aids device
// received it from the air. We assumed that there is 2 data buffer queued from
// audio subsystem to bluetooth chip. Then the estimated OTA delay is two
// connnection intervals.
constexpr uint16_t ADD_RENDER_DELAY_INTERVALS = 4;

namespace {

// clang-format off
Uuid HEARING_AID_UUID          = Uuid::FromString("FDF0");
Uuid READ_ONLY_PROPERTIES_UUID = Uuid::FromString("6333651e-c481-4a3e-9169-7c902aad37bb");
Uuid AUDIO_CONTROL_POINT_UUID  = Uuid::FromString("f0d4de7e-4a88-476c-9d9f-1937b0996cc0");
Uuid AUDIO_STATUS_UUID         = Uuid::FromString("38663f1a-e711-4cac-b641-326b56404837");
Uuid VOLUME_UUID               = Uuid::FromString("00e4ca9e-ab14-41e4-8823-f9e70c7e91df");
Uuid LE_PSM_UUID               = Uuid::FromString("2d410339-82b6-42aa-b34e-e2e01df8cc1a");
// clang-format on

void hearingaid_gattc_callback(tBTA_GATTC_EVT event, tBTA_GATTC* p_data);
void encryption_callback(const RawAddress*, tGATT_TRANSPORT, void*,
                         tBTM_STATUS);
void read_rssi_cb(void* p_void);

inline BT_HDR* malloc_l2cap_buf(uint16_t len) {
  BT_HDR* msg = (BT_HDR*)osi_malloc(BT_HDR_SIZE + L2CAP_MIN_OFFSET +
                                    len /* LE-only, no need for FCS here */);
  msg->offset = L2CAP_MIN_OFFSET;
  msg->len = len;
  return msg;
}

inline uint8_t* get_l2cap_sdu_start_ptr(BT_HDR* msg) {
  return (uint8_t*)(msg) + BT_HDR_SIZE + L2CAP_MIN_OFFSET;
}

class HearingAidImpl;
HearingAidImpl* instance;
HearingAidAudioReceiver* audioReceiver;

class HearingDevices {
 public:
  void Add(HearingDevice device) {
    if (FindByAddress(device.address) != nullptr) return;

    devices.push_back(device);
  }

  void Remove(const RawAddress& address) {
    for (auto it = devices.begin(); it != devices.end();) {
      if (it->address != address) {
        ++it;
        continue;
      }

      it = devices.erase(it);
      return;
    }
  }

  HearingDevice* FindByAddress(const RawAddress& address) {
    auto iter = std::find_if(devices.begin(), devices.end(),
                             [&address](const HearingDevice& device) {
                               return device.address == address;
                             });

    return (iter == devices.end()) ? nullptr : &(*iter);
  }

  HearingDevice* FindByConnId(uint16_t conn_id) {
    auto iter = std::find_if(devices.begin(), devices.end(),
                             [&conn_id](const HearingDevice& device) {
                               return device.conn_id == conn_id;
                             });

    return (iter == devices.end()) ? nullptr : &(*iter);
  }

  HearingDevice* FindByGapHandle(uint16_t gap_handle) {
    auto iter = std::find_if(devices.begin(), devices.end(),
                             [&gap_handle](const HearingDevice& device) {
                               return device.gap_handle == gap_handle;
                             });

    return (iter == devices.end()) ? nullptr : &(*iter);
  }

  bool IsAnyConnectionUpdateStarted() {
    for (const auto& d : devices) {
      if (d.connection_update_status == STARTED) return true;
    }

    return false;
  }

  void StartRssiLog() {
    int read_rssi_start_interval_count = 0;

    for (auto& d : devices) {
      VLOG(1) << __func__ << ": device=" << d.address << ", read_rssi_count=" << d.read_rssi_count;

      // Reset the count
      if (d.read_rssi_count <= 0) {
        d.read_rssi_count = READ_RSSI_NUM_TRIES;
        d.num_intervals_since_last_rssi_read = read_rssi_start_interval_count;

        // Spaced apart the Read RSSI commands to the BT controller.
        read_rssi_start_interval_count += PERIOD_TO_READ_RSSI_IN_INTERVALS / 2;
        read_rssi_start_interval_count %= PERIOD_TO_READ_RSSI_IN_INTERVALS;

        std::deque<rssi_log>& rssi_logs = d.audio_stats.rssi_history;
        if (rssi_logs.size() >= MAX_RSSI_HISTORY) {
          rssi_logs.pop_front();
        }
        rssi_logs.emplace_back();
      }
    }
  }

  size_t size() { return (devices.size()); }

  std::vector<HearingDevice> devices;
};

static void write_rpt_ctl_cfg_cb(uint16_t conn_id, tGATT_STATUS status,
                                 uint16_t handle, void* data) {
  if (status != GATT_SUCCESS) {
    LOG(ERROR) << __func__ << ": handle=" << handle << ", conn_id=" << conn_id
               << ", status=" << loghex(status);
  }
}

g722_encode_state_t* encoder_state_left = nullptr;
g722_encode_state_t* encoder_state_right = nullptr;

inline void encoder_state_init() {
  if (encoder_state_left != nullptr) {
    LOG(WARNING) << __func__ << ": encoder already initialized";
    return;
  }
  encoder_state_left = g722_encode_init(nullptr, 64000, G722_PACKED);
  encoder_state_right = g722_encode_init(nullptr, 64000, G722_PACKED);
}

inline void encoder_state_release() {
  if (encoder_state_left != nullptr) {
    g722_encode_release(encoder_state_left);
    encoder_state_left = nullptr;
    g722_encode_release(encoder_state_right);
    encoder_state_right = nullptr;
  }
}

class HearingAidImpl : public HearingAid {
 private:
  // Keep track of whether the Audio Service has resumed audio playback
  bool audio_running;
  // For Testing: overwrite the MIN_CE_LEN during connection parameter updates
  uint16_t overwrite_min_ce_len;

 public:
  virtual ~HearingAidImpl() = default;

  HearingAidImpl(bluetooth::hearing_aid::HearingAidCallbacks* callbacks,
                 Closure initCb)
      : audio_running(false),
        overwrite_min_ce_len(0),
        gatt_if(0),
        seq_counter(0),
        current_volume(VOLUME_UNKNOWN),
        callbacks(callbacks),
        codec_in_use(0) {
    default_data_interval_ms = (uint16_t)osi_property_get_int32(
        "persist.bluetooth.hearingaid.interval", (int32_t)HA_INTERVAL_20_MS);
    if ((default_data_interval_ms != HA_INTERVAL_10_MS) &&
        (default_data_interval_ms != HA_INTERVAL_20_MS)) {
      LOG(ERROR) << __func__
                 << ": invalid interval=" << default_data_interval_ms
                 << "ms. Overwriting back to default";
      default_data_interval_ms = HA_INTERVAL_20_MS;
    }
    VLOG(2) << __func__
            << ", default_data_interval_ms=" << default_data_interval_ms;

    overwrite_min_ce_len = (uint16_t)osi_property_get_int32(
        "persist.bluetooth.hearingaidmincelen", 0);
    if (overwrite_min_ce_len) {
      LOG(INFO) << __func__
                << ": Overwrites MIN_CE_LEN=" << overwrite_min_ce_len;
    }

    BTA_GATTC_AppRegister(
        hearingaid_gattc_callback,
        base::Bind(
            [](Closure initCb, uint8_t client_id, uint8_t status) {
              if (status != GATT_SUCCESS) {
                LOG(ERROR) << "Can't start Hearing Aid profile - no gatt "
                              "clients left!";
                return;
              }
              instance->gatt_if = client_id;
              initCb.Run();
            },
            initCb));
  }

  uint16_t UpdateBleConnParams(const RawAddress& address) {
    /* List of parameters that depends on the chosen Connection Interval */
    uint16_t min_ce_len;
    uint16_t connection_interval;

    switch (default_data_interval_ms) {
      case HA_INTERVAL_10_MS:
        min_ce_len = MIN_CE_LEN_10MS_CI;
        connection_interval = CONNECTION_INTERVAL_10MS_PARAM;
        break;
      case HA_INTERVAL_20_MS:
        min_ce_len = MIN_CE_LEN_20MS_CI;
        connection_interval = CONNECTION_INTERVAL_20MS_PARAM;
        break;
      default:
        LOG(ERROR) << __func__ << ":Error: invalid default_data_interval_ms="
                   << default_data_interval_ms;
        min_ce_len = MIN_CE_LEN_10MS_CI;
        connection_interval = CONNECTION_INTERVAL_10MS_PARAM;
    }

    if (overwrite_min_ce_len != 0) {
      VLOG(2) << __func__ << ": min_ce_len=" << min_ce_len
              << " is overwritten to " << overwrite_min_ce_len;
      min_ce_len = overwrite_min_ce_len;
    }

    L2CA_UpdateBleConnParams(address, connection_interval, connection_interval,
                             0x000A, 0x0064 /*1s*/, min_ce_len, min_ce_len);
    return connection_interval;
  }

  void Connect(const RawAddress& address) override {
    DVLOG(2) << __func__ << " " << address;
    hearingDevices.Add(HearingDevice(address, true));
    BTA_GATTC_Open(gatt_if, address, true, GATT_TRANSPORT_LE, false);
  }

  void AddToWhiteList(const RawAddress& address) override {
    VLOG(2) << __func__ << " address: " << address;
    hearingDevices.Add(HearingDevice(address, true));
    BTA_GATTC_Open(gatt_if, address, false, GATT_TRANSPORT_LE, false);
  }

  void AddFromStorage(const HearingDevice& dev_info, uint16_t is_white_listed) {
    DVLOG(2) << __func__ << " " << dev_info.address
             << ", hiSyncId=" << loghex(dev_info.hi_sync_id)
             << ", isWhiteListed=" << is_white_listed;
    if (is_white_listed) {
      hearingDevices.Add(dev_info);

      // TODO: we should increase the scanning window for few seconds, to get
      // faster initial connection, same after hearing aid disconnects, i.e.
      // BTM_BleSetConnScanParams(2048, 1024);

      /* add device into BG connection to accept remote initiated connection */
      BTA_GATTC_Open(gatt_if, dev_info.address, false, GATT_TRANSPORT_LE,
                     false);
    }

    callbacks->OnDeviceAvailable(dev_info.capabilities, dev_info.hi_sync_id,
                                 dev_info.address);
  }

  int GetDeviceCount() { return (hearingDevices.size()); }

  void OnGattConnected(tGATT_STATUS status, uint16_t conn_id,
                       tGATT_IF client_if, RawAddress address,
                       tBTA_TRANSPORT transport, uint16_t mtu) {
    VLOG(2) << __func__ << ": address=" << address << ", conn_id=" << conn_id;

    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      /* When Hearing Aid is quickly disabled and enabled in settings, this case
       * might happen */
      LOG(WARNING) << "Closing connection to non hearing-aid device, address="
                   << address;
      BTA_GATTC_Close(conn_id);
      return;
    }

    if (status != GATT_SUCCESS) {
      if (!hearingDevice->connecting_actively) {
        // whitelist connection failed, that's ok.
        return;
      }

      LOG(INFO) << "Failed to connect to Hearing Aid device";
      hearingDevices.Remove(address);
      callbacks->OnConnectionState(ConnectionState::DISCONNECTED, address);
      return;
    }

    hearingDevice->connecting_actively = false;
    hearingDevice->conn_id = conn_id;

    /* We must update connection parameters one at a time, otherwise anchor
     * point (start of connection event) for two devices can be too close to
     * each other. Here, by setting min_ce_len=max_ce_len=X, we force controller
     * to move anchor point of both connections away from each other, to make
     * sure we'll be able to fit all the data we want in one connection event.
     */
    bool any_update_pending = hearingDevices.IsAnyConnectionUpdateStarted();
    // mark the device as pending connection update. If we don't start the
    // update now, it'll be started once current device finishes.
    if (!any_update_pending) {
      hearingDevice->connection_update_status = STARTED;
      hearingDevice->requested_connection_interval =
          UpdateBleConnParams(address);
    } else {
      hearingDevice->connection_update_status = AWAITING;
    }

    tACL_CONN* p_acl = btm_bda_to_acl(address, BT_TRANSPORT_LE);
    if (p_acl != nullptr && controller_get_interface()->supports_ble_2m_phy() &&
        HCI_LE_2M_PHY_SUPPORTED(p_acl->peer_le_features)) {
      LOG(INFO) << address << " set preferred PHY to 2M";
      BTM_BleSetPhy(address, PHY_LE_2M, PHY_LE_2M, 0);
    }

    // Set data length
    // TODO(jpawlowski: for 16khz only 87 is required, optimize
    BTM_SetBleDataLength(address, 167);

    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(address);
    if (p_dev_rec) {
      if (p_dev_rec->sec_state == BTM_SEC_STATE_ENCRYPTING ||
          p_dev_rec->sec_state == BTM_SEC_STATE_AUTHENTICATING) {
        /* if security collision happened, wait for encryption done
         * (BTA_GATTC_ENC_CMPL_CB_EVT) */
        return;
      }
    }

    /* verify bond */
    uint8_t sec_flag = 0;
    BTM_GetSecurityFlagsByTransport(address, &sec_flag, BT_TRANSPORT_LE);

    if (sec_flag & BTM_SEC_FLAG_ENCRYPTED) {
      /* if link has been encrypted */
      OnEncryptionComplete(address, true);
      return;
    }

    if (sec_flag & BTM_SEC_FLAG_LKEY_KNOWN) {
      /* if bonded and link not encrypted */
      sec_flag = BTM_BLE_SEC_ENCRYPT;
      BTM_SetEncryption(address, BTA_TRANSPORT_LE, encryption_callback, nullptr,
                        sec_flag);
      return;
    }

    /* otherwise let it go through */
    OnEncryptionComplete(address, true);
  }

  void OnConnectionUpdateComplete(uint16_t conn_id, tBTA_GATTC* p_data) {
    HearingDevice* hearingDevice = hearingDevices.FindByConnId(conn_id);
    if (!hearingDevice) {
      DVLOG(2) << "Skipping unknown device, conn_id=" << loghex(conn_id);
      return;
    }

    if (p_data) {
      if (p_data->conn_update.status == 0) {
        bool same_conn_interval =
            (hearingDevice->requested_connection_interval ==
             p_data->conn_update.interval);

        switch (hearingDevice->connection_update_status) {
          case COMPLETED:
            if (!same_conn_interval) {
              LOG(WARNING) << __func__
                           << ": Unexpected change. Redo. connection interval="
                           << p_data->conn_update.interval << ", expected="
                           << hearingDevice->requested_connection_interval
                           << ", conn_id=" << conn_id
                           << ", connection_update_status="
                           << hearingDevice->connection_update_status;
              // Redo this connection interval change.
              hearingDevice->connection_update_status = AWAITING;
            }
            break;
          case STARTED:
            if (same_conn_interval) {
              LOG(INFO) << __func__
                        << ": Connection update completed. conn_id=" << conn_id
                        << ", device=" << hearingDevice->address;
              hearingDevice->connection_update_status = COMPLETED;
            } else {
              LOG(WARNING) << __func__
                           << ": Ignored. Different connection interval="
                           << p_data->conn_update.interval << ", expected="
                           << hearingDevice->requested_connection_interval
                           << ", conn_id=" << conn_id
                           << ", connection_update_status="
                           << hearingDevice->connection_update_status;
              // Wait for the right Connection Update Completion.
              return;
            }
            break;
          case AWAITING:
          case NONE:
            break;
        }

        // Inform this side and other side device (if any) of Connection
        // Updates.
        std::vector<uint8_t> conn_update(
            {CONTROL_POINT_OP_STATE_CHANGE, STATE_CHANGE_CONN_UPDATE,
             (uint8_t)p_data->conn_update.interval});
        send_state_change_to_other_side(hearingDevice, conn_update);
        send_state_change(hearingDevice, conn_update);
      } else {
        LOG(INFO) << __func__
                  << ": error status=" << loghex(p_data->conn_update.status)
                  << ", conn_id=" << conn_id
                  << ", device=" << hearingDevice->address
                  << ", connection_update_status="
                  << hearingDevice->connection_update_status;

        if (hearingDevice->connection_update_status == STARTED) {
          // Redo this connection interval change.
          LOG(ERROR) << __func__ << ": Redo Connection Interval change";
          hearingDevice->connection_update_status = AWAITING;
        }
      }
    } else {
      hearingDevice->connection_update_status = NONE;
    }

    for (auto& device : hearingDevices.devices) {
      if (device.conn_id && (device.connection_update_status == AWAITING)) {
        device.connection_update_status = STARTED;
        device.requested_connection_interval =
            UpdateBleConnParams(device.address);
        return;
      }
    }
  }

  // Completion Callback for the RSSI read operation.
  void OnReadRssiComplete(const RawAddress& address, int8_t rssi_value) {
    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      LOG(INFO) << "Skipping unknown device" << address;
      return;
    }

    VLOG(1) << __func__ << ": device=" << address << ", rssi=" << (int)rssi_value;

    if (hearingDevice->read_rssi_count <= 0) {
      LOG(ERROR) << __func__ << ": device=" << address
                 << ", invalid read_rssi_count=" << hearingDevice->read_rssi_count;
      return;
    }

    rssi_log& last_log_set = hearingDevice->audio_stats.rssi_history.back();

    if (hearingDevice->read_rssi_count == READ_RSSI_NUM_TRIES) {
      // Store the timestamp only for the first one after packet flush
      clock_gettime(CLOCK_REALTIME, &last_log_set.timestamp);
      LOG(INFO) << __func__ << ": store time. device=" << address << ", rssi=" << (int)rssi_value;
    }

    last_log_set.rssi.emplace_back(rssi_value);
    hearingDevice->read_rssi_count--;
  }

  void OnEncryptionComplete(const RawAddress& address, bool success) {
    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      DVLOG(2) << "Skipping unknown device" << address;
      return;
    }

    if (!success) {
      LOG(ERROR) << "encryption failed";
      BTA_GATTC_Close(hearingDevice->conn_id);
      if (hearingDevice->first_connection) {
        callbacks->OnConnectionState(ConnectionState::DISCONNECTED, address);
      }
      return;
    }

    DVLOG(2) << __func__ << " " << address;

    if (hearingDevice->audio_control_point_handle &&
        hearingDevice->audio_status_handle &&
        hearingDevice->audio_status_ccc_handle &&
        hearingDevice->volume_handle && hearingDevice->read_psm_handle) {
      // Use cached data, jump to read PSM
      ReadPSM(hearingDevice);
    } else {
      hearingDevice->first_connection = true;
      BTA_GATTC_ServiceSearchRequest(hearingDevice->conn_id, &HEARING_AID_UUID);
    }
  }

  void OnServiceChangeEvent(const RawAddress& address) {
    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      VLOG(2) << "Skipping unknown device" << address;
      return;
    }
    LOG(INFO) << __func__ << ": address=" << address;
    hearingDevice->first_connection = true;
    hearingDevice->service_changed_rcvd = true;
    BtaGattQueue::Clean(hearingDevice->conn_id);
    if (hearingDevice->gap_handle) {
      GAP_ConnClose(hearingDevice->gap_handle);
      hearingDevice->gap_handle = 0;
    }
  }

  void OnServiceDiscDoneEvent(const RawAddress& address) {
    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      VLOG(2) << "Skipping unknown device" << address;
      return;
    }
    if (hearingDevice->service_changed_rcvd) {
      BTA_GATTC_ServiceSearchRequest(hearingDevice->conn_id, &HEARING_AID_UUID);
    }
  }

  void OnServiceSearchComplete(uint16_t conn_id, tGATT_STATUS status) {
    HearingDevice* hearingDevice = hearingDevices.FindByConnId(conn_id);
    if (!hearingDevice) {
      DVLOG(2) << "Skipping unknown device, conn_id=" << loghex(conn_id);
      return;
    }

    // Known device, nothing to do.
    if (!hearingDevice->first_connection) return;

    if (status != GATT_SUCCESS) {
      /* close connection and report service discovery complete with error */
      LOG(ERROR) << "Service discovery failed";
      if (hearingDevice->first_connection) {
        callbacks->OnConnectionState(ConnectionState::DISCONNECTED,
                                     hearingDevice->address);
      }
      return;
    }

    const std::list<gatt::Service>* services = BTA_GATTC_GetServices(conn_id);

    const gatt::Service* service = nullptr;
    for (const gatt::Service& tmp : *services) {
      if (tmp.uuid == Uuid::From16Bit(UUID_SERVCLASS_GATT_SERVER)) {
        LOG(INFO) << "Found UUID_SERVCLASS_GATT_SERVER, handle="
                  << loghex(tmp.handle);
        const gatt::Service* service_changed_service = &tmp;
        find_server_changed_ccc_handle(conn_id, service_changed_service);
      } else if (tmp.uuid == HEARING_AID_UUID) {
        LOG(INFO) << "Found Hearing Aid service, handle=" << loghex(tmp.handle);
        service = &tmp;
      }
    }

    if (!service) {
      LOG(ERROR) << "No Hearing Aid service found";
      callbacks->OnConnectionState(ConnectionState::DISCONNECTED,
                                   hearingDevice->address);
      return;
    }

    for (const gatt::Characteristic& charac : service->characteristics) {
      if (charac.uuid == READ_ONLY_PROPERTIES_UUID) {
        if (!btif_storage_get_hearing_aid_prop(
                hearingDevice->address, &hearingDevice->capabilities,
                &hearingDevice->hi_sync_id, &hearingDevice->render_delay,
                &hearingDevice->preparation_delay, &hearingDevice->codecs)) {
          VLOG(2) << "Reading read only properties "
                  << loghex(charac.value_handle);
          BtaGattQueue::ReadCharacteristic(
              conn_id, charac.value_handle,
              HearingAidImpl::OnReadOnlyPropertiesReadStatic, nullptr);
        }
      } else if (charac.uuid == AUDIO_CONTROL_POINT_UUID) {
        hearingDevice->audio_control_point_handle = charac.value_handle;
        // store audio control point!
      } else if (charac.uuid == AUDIO_STATUS_UUID) {
        hearingDevice->audio_status_handle = charac.value_handle;

        hearingDevice->audio_status_ccc_handle =
            find_ccc_handle(conn_id, charac.value_handle);
        if (!hearingDevice->audio_status_ccc_handle) {
          LOG(ERROR) << __func__ << ": cannot find Audio Status CCC descriptor";
          continue;
        }

        LOG(INFO) << __func__
                  << ": audio_status_handle=" << loghex(charac.value_handle)
                  << ", ccc=" << loghex(hearingDevice->audio_status_ccc_handle);
      } else if (charac.uuid == VOLUME_UUID) {
        hearingDevice->volume_handle = charac.value_handle;
      } else if (charac.uuid == LE_PSM_UUID) {
        hearingDevice->read_psm_handle = charac.value_handle;
      } else {
        LOG(WARNING) << "Unknown characteristic found:" << charac.uuid;
      }
    }

    if (hearingDevice->service_changed_rcvd) {
      hearingDevice->service_changed_rcvd = false;
    }

    ReadPSM(hearingDevice);
  }

  void ReadPSM(HearingDevice* hearingDevice) {
    if (hearingDevice->read_psm_handle) {
      LOG(INFO) << "Reading PSM " << loghex(hearingDevice->read_psm_handle)
                << ", device=" << hearingDevice->address;
      BtaGattQueue::ReadCharacteristic(
          hearingDevice->conn_id, hearingDevice->read_psm_handle,
          HearingAidImpl::OnPsmReadStatic, nullptr);
    }
  }

  void OnNotificationEvent(uint16_t conn_id, uint16_t handle, uint16_t len,
                           uint8_t* value) {
    HearingDevice* device = hearingDevices.FindByConnId(conn_id);
    if (!device) {
      LOG(INFO) << __func__
                << ": Skipping unknown device, conn_id=" << loghex(conn_id);
      return;
    }

    if (device->audio_status_handle != handle) {
      LOG(INFO) << __func__ << ": Mismatched handle, "
                << loghex(device->audio_status_handle)
                << "!=" << loghex(handle);
      return;
    }

    if (len < 1) {
      LOG(ERROR) << __func__ << ": Data Length too small, len=" << len
                 << ", expecting at least 1";
      return;
    }

    if (value[0] != 0) {
      LOG(INFO) << __func__
                << ": Invalid returned status. data=" << loghex(value[0]);
      return;
    }

    LOG(INFO) << __func__
              << ": audio status success notification. command_acked="
              << device->command_acked;
    device->command_acked = true;
  }

  void OnReadOnlyPropertiesRead(uint16_t conn_id, tGATT_STATUS status,
                                uint16_t handle, uint16_t len, uint8_t* value,
                                void* data) {
    HearingDevice* hearingDevice = hearingDevices.FindByConnId(conn_id);
    if (!hearingDevice) {
      DVLOG(2) << __func__ << "unknown conn_id=" << loghex(conn_id);
      return;
    }

    VLOG(2) << __func__ << " " << base::HexEncode(value, len);

    uint8_t* p = value;

    uint8_t version;
    STREAM_TO_UINT8(version, p);

    if (version != 0x01) {
      LOG(WARNING) << "Unknown version: " << loghex(version);
      return;
    }

    // version 0x01 of read only properties:
    if (len < 17) {
      LOG(WARNING) << "Read only properties too short: " << loghex(len);
      return;
    }
    uint8_t capabilities;
    STREAM_TO_UINT8(capabilities, p);
    hearingDevice->capabilities = capabilities;
    bool side = capabilities & CAPABILITY_SIDE;
    bool standalone = capabilities & CAPABILITY_BINAURAL;
    VLOG(2) << __func__ << " capabilities: " << (side ? "right" : "left")
            << ", " << (standalone ? "binaural" : "monaural");

    if (capabilities & CAPABILITY_RESERVED) {
      LOG(WARNING) << __func__ << " reserved capabilities are set";
    }

    STREAM_TO_UINT64(hearingDevice->hi_sync_id, p);
    VLOG(2) << __func__ << " hiSyncId: " << loghex(hearingDevice->hi_sync_id);
    uint8_t feature_map;
    STREAM_TO_UINT8(feature_map, p);

    STREAM_TO_UINT16(hearingDevice->render_delay, p);
    VLOG(2) << __func__
            << " render delay: " << loghex(hearingDevice->render_delay);

    STREAM_TO_UINT16(hearingDevice->preparation_delay, p);
    VLOG(2) << __func__ << " preparation delay: "
            << loghex(hearingDevice->preparation_delay);

    uint16_t codecs;
    STREAM_TO_UINT16(codecs, p);
    hearingDevice->codecs = codecs;
    VLOG(2) << __func__ << " supported codecs: " << loghex(codecs);
    if (codecs & (1 << CODEC_G722_16KHZ)) VLOG(2) << "\tG722@16kHz";
    if (codecs & (1 << CODEC_G722_24KHZ)) VLOG(2) << "\tG722@24kHz";

    if (!(codecs & (1 << CODEC_G722_16KHZ))) {
      LOG(WARNING) << __func__ << " Mandatory codec, G722@16kHz not supported";
    }
  }

  uint16_t CalcCompressedAudioPacketSize(uint16_t codec_type,
                                         int connection_interval) {
    int sample_rate;

    const int sample_bit_rate = 16;  /* 16 bits per sample */
    const int compression_ratio = 4; /* G.722 has a 4:1 compression ratio */
    if (codec_type == CODEC_G722_24KHZ) {
      sample_rate = 24000;
    } else {
      sample_rate = 16000;
    }

    // compressed_data_packet_size is the size in bytes of the compressed audio
    // data buffer that is generated for each connection interval.
    uint32_t compressed_data_packet_size =
        (sample_rate * connection_interval * (sample_bit_rate / 8) /
         compression_ratio) /
        1000;
    return ((uint16_t)compressed_data_packet_size);
  }

  void ChooseCodec(const HearingDevice& hearingDevice) {
    if (codec_in_use) return;

    // use the best codec available for this pair of devices.
    uint16_t codecs = hearingDevice.codecs;
    if (hearingDevice.hi_sync_id != 0) {
      for (const auto& device : hearingDevices.devices) {
        if (device.hi_sync_id != hearingDevice.hi_sync_id) continue;

        codecs &= device.codecs;
      }
    }

    if ((codecs & (1 << CODEC_G722_24KHZ)) &&
        controller_get_interface()->supports_ble_2m_phy() &&
        default_data_interval_ms == HA_INTERVAL_10_MS) {
      codec_in_use = CODEC_G722_24KHZ;
    } else if (codecs & (1 << CODEC_G722_16KHZ)) {
      codec_in_use = CODEC_G722_16KHZ;
    }
  }

  void OnAudioStatus(uint16_t conn_id, tGATT_STATUS status, uint16_t handle,
                     uint16_t len, uint8_t* value, void* data) {
    LOG(INFO) << __func__ << " " << base::HexEncode(value, len);
  }

  void OnPsmRead(uint16_t conn_id, tGATT_STATUS status, uint16_t handle,
                 uint16_t len, uint8_t* value, void* data) {
    HearingDevice* hearingDevice = hearingDevices.FindByConnId(conn_id);
    if (!hearingDevice) {
      DVLOG(2) << "Skipping unknown read event, conn_id=" << loghex(conn_id);
      return;
    }

    if (status != GATT_SUCCESS) {
      LOG(ERROR) << "Error reading PSM for device" << hearingDevice->address;
      return;
    }

    if (len > 2) {
      LOG(ERROR) << "Bad PSM length";
      return;
    }

    uint16_t psm = *((uint16_t*)value);
    VLOG(2) << "read psm:" << loghex(psm);

    ConnectSocket(hearingDevice, psm);
  }

  void ConnectSocket(HearingDevice* hearingDevice, uint16_t psm) {
    tL2CAP_CFG_INFO cfg_info = tL2CAP_CFG_INFO{.mtu = 512};

    SendEnableServiceChangedInd(hearingDevice);

    uint8_t service_id = hearingDevice->isLeft()
                             ? BTM_SEC_SERVICE_HEARING_AID_LEFT
                             : BTM_SEC_SERVICE_HEARING_AID_RIGHT;
    uint16_t gap_handle = GAP_ConnOpen(
        "", service_id, false, &hearingDevice->address, psm, 514 /* MPS */,
        &cfg_info, nullptr, BTM_SEC_NONE /* TODO: request security ? */,
        L2CAP_FCR_LE_COC_MODE, HearingAidImpl::GapCallbackStatic,
        BT_TRANSPORT_LE);
    if (gap_handle == GAP_INVALID_HANDLE) {
      LOG(ERROR) << "UNABLE TO GET gap_handle";
      return;
    }

    hearingDevice->gap_handle = gap_handle;
    LOG(INFO) << "Successfully sent GAP connect request";
  }

  static void OnReadOnlyPropertiesReadStatic(uint16_t conn_id,
                                             tGATT_STATUS status,
                                             uint16_t handle, uint16_t len,
                                             uint8_t* value, void* data) {
    if (instance)
      instance->OnReadOnlyPropertiesRead(conn_id, status, handle, len, value,
                                         data);
  }
  static void OnAudioStatusStatic(uint16_t conn_id, tGATT_STATUS status,
                                  uint16_t handle, uint16_t len, uint8_t* value,
                                  void* data) {
    if (instance)
      instance->OnAudioStatus(conn_id, status, handle, len, value, data);
  }

  static void OnPsmReadStatic(uint16_t conn_id, tGATT_STATUS status,
                              uint16_t handle, uint16_t len, uint8_t* value,
                              void* data) {
    if (instance)
      instance->OnPsmRead(conn_id, status, handle, len, value, data);
  }

  /* CoC Socket is ready */
  void OnGapConnection(const RawAddress& address) {
    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      LOG(INFO) << "Device not connected to profile" << address;
      return;
    }

    if (hearingDevice->first_connection) {
      /* add device into BG connection to accept remote initiated connection */
      BTA_GATTC_Open(gatt_if, address, false, GATT_TRANSPORT_LE, false);

      btif_storage_add_hearing_aid(*hearingDevice);

      hearingDevice->first_connection = false;
    }

    LOG(INFO) << __func__ << ": audio_status_handle="
              << loghex(hearingDevice->audio_status_handle)
              << ", audio_status_ccc_handle="
              << loghex(hearingDevice->audio_status_ccc_handle);

    /* Register and enable the Audio Status Notification */
    tGATT_STATUS register_status;
    register_status = BTA_GATTC_RegisterForNotifications(
        gatt_if, address, hearingDevice->audio_status_handle);
    if (register_status != GATT_SUCCESS) {
      LOG(ERROR) << __func__
                 << ": BTA_GATTC_RegisterForNotifications failed, status="
                 << loghex(register_status);
      return;
    }
    std::vector<uint8_t> value(2);
    uint8_t* ptr = value.data();
    UINT16_TO_STREAM(ptr, GATT_CHAR_CLIENT_CONFIG_NOTIFICATION);
    BtaGattQueue::WriteDescriptor(
        hearingDevice->conn_id, hearingDevice->audio_status_ccc_handle,
        std::move(value), GATT_WRITE, write_rpt_ctl_cfg_cb, nullptr);

    ChooseCodec(*hearingDevice);

    SendStart(hearingDevice);

    if (audio_running) {
      // Inform the other side (if any) of this connection
      std::vector<uint8_t> inform_conn_state(
          {CONTROL_POINT_OP_STATE_CHANGE, STATE_CHANGE_OTHER_SIDE_CONNECTED});
      send_state_change_to_other_side(hearingDevice, inform_conn_state);
    }

    hearingDevice->accepting_audio = true;
    LOG(INFO) << __func__ << ": address=" << address
              << ", hi_sync_id=" << loghex(hearingDevice->hi_sync_id)
              << ", codec_in_use=" << loghex(codec_in_use)
              << ", audio_running=" << audio_running;

    StartSendingAudio(*hearingDevice);

    callbacks->OnDeviceAvailable(hearingDevice->capabilities,
                                 hearingDevice->hi_sync_id, address);
    callbacks->OnConnectionState(ConnectionState::CONNECTED, address);
  }

  void StartSendingAudio(const HearingDevice& hearingDevice) {
    VLOG(0) << __func__ << ": device=" << hearingDevice.address;

    if (encoder_state_left == nullptr) {
      encoder_state_init();
      seq_counter = 0;

      // use the best codec avaliable for this pair of devices.
      uint16_t codecs = hearingDevice.codecs;
      if (hearingDevice.hi_sync_id != 0) {
        for (const auto& device : hearingDevices.devices) {
          if (device.hi_sync_id != hearingDevice.hi_sync_id) continue;

          codecs &= device.codecs;
        }
      }

      CodecConfiguration codec;
      if (codec_in_use == CODEC_G722_24KHZ) {
        codec.sample_rate = 24000;
      } else {
        codec.sample_rate = 16000;
      }
      codec.bit_rate = 16;
      codec.data_interval_ms = default_data_interval_ms;

      uint16_t delay_report_ms = 0;
      if (hearingDevice.render_delay != 0) {
        delay_report_ms =
            hearingDevice.render_delay +
            (ADD_RENDER_DELAY_INTERVALS * default_data_interval_ms);
      }

      HearingAidAudioSource::Start(codec, audioReceiver, delay_report_ms);
    }
  }

  void OnAudioSuspend(const std::function<void()>& stop_audio_ticks) {
    CHECK(stop_audio_ticks) << "stop_audio_ticks is empty";

    if (!audio_running) {
      LOG(WARNING) << __func__ << ": Unexpected audio suspend";
    } else {
      LOG(INFO) << __func__ << ": audio_running=" << audio_running;
    }
    audio_running = false;
    stop_audio_ticks();

    std::vector<uint8_t> stop({CONTROL_POINT_OP_STOP});
    for (auto& device : hearingDevices.devices) {
      if (!device.accepting_audio) continue;

      if (!device.playback_started) {
        LOG(WARNING) << __func__
                     << ": Playback not started, skip send Stop cmd, device="
                     << device.address;
      } else {
        LOG(INFO) << __func__ << ": send Stop cmd, device=" << device.address;
        device.playback_started = false;
        device.command_acked = false;
        BtaGattQueue::WriteCharacteristic(device.conn_id,
                                          device.audio_control_point_handle,
                                          stop, GATT_WRITE, nullptr, nullptr);
      }
    }
  }

  void OnAudioResume(const std::function<void()>& start_audio_ticks) {
    CHECK(start_audio_ticks) << "start_audio_ticks is empty";

    if (audio_running) {
      LOG(ERROR) << __func__ << ": Unexpected Audio Resume";
    } else {
      LOG(INFO) << __func__ << ": audio_running=" << audio_running;
    }

    for (auto& device : hearingDevices.devices) {
      if (!device.accepting_audio) continue;
      audio_running = true;
      SendStart(&device);
    }

    if (!audio_running) {
      LOG(INFO) << __func__ << ": No device (0/" << GetDeviceCount()
                << ") ready to start";
      return;
    }

    // TODO: shall we also reset the encoder ?
    encoder_state_release();
    encoder_state_init();
    seq_counter = 0;

    start_audio_ticks();
  }

  uint8_t GetOtherSideStreamStatus(HearingDevice* this_side_device) {
    for (auto& device : hearingDevices.devices) {
      if ((device.address == this_side_device->address) ||
          (device.hi_sync_id != this_side_device->hi_sync_id)) {
        continue;
      }
      if (audio_running && (device.conn_id != 0)) {
        return (OTHER_SIDE_IS_STREAMING);
      } else {
        return (OTHER_SIDE_NOT_STREAMING);
      }
    }
    return (OTHER_SIDE_NOT_STREAMING);
  }

  void SendEnableServiceChangedInd(HearingDevice* device) {
    VLOG(2) << __func__ << " Enable " << device->address
            << "service changed ind.";
    std::vector<uint8_t> value(2);
    uint8_t* ptr = value.data();
    UINT16_TO_STREAM(ptr, GATT_CHAR_CLIENT_CONFIG_INDICTION);
    BtaGattQueue::WriteDescriptor(
        device->conn_id, device->service_changed_ccc_handle, std::move(value),
        GATT_WRITE, nullptr, nullptr);
  }

  void SendStart(HearingDevice* device) {
    std::vector<uint8_t> start({CONTROL_POINT_OP_START, codec_in_use,
                                AUDIOTYPE_UNKNOWN, (uint8_t)current_volume,
                                OTHER_SIDE_NOT_STREAMING});

    if (!audio_running) {
      if (!device->playback_started) {
        LOG(INFO) << __func__
                  << ": Skip Send Start since audio is not running, device="
                  << device->address;
      } else {
        LOG(ERROR) << __func__
                   << ": Audio not running but Playback has started, device="
                   << device->address;
      }
      return;
    }

    if (current_volume == VOLUME_UNKNOWN) start[3] = (uint8_t)VOLUME_MIN;

    if (device->playback_started) {
      LOG(ERROR) << __func__
                 << ": Playback already started, skip send Start cmd, device="
                 << device->address;
    } else {
      start[4] = GetOtherSideStreamStatus(device);
      LOG(INFO) << __func__ << ": send Start cmd, volume=" << loghex(start[3])
                << ", audio type=" << loghex(start[2])
                << ", device=" << device->address
                << ", other side streaming=" << loghex(start[4]);
      device->playback_started = true;
      device->command_acked = false;
      BtaGattQueue::WriteCharacteristic(device->conn_id,
                                        device->audio_control_point_handle,
                                        start, GATT_WRITE, nullptr, nullptr);
    }
  }

  void OnAudioDataReady(const std::vector<uint8_t>& data) {
    /* For now we assume data comes in as 16bit per sample 16kHz PCM stereo */
    DVLOG(2) << __func__;

    int num_samples =
        data.size() / (2 /*bytes_per_sample*/ * 2 /*number of channels*/);

    // The G.722 codec accept only even number of samples for encoding
    if (num_samples % 2 != 0)
      LOG(FATAL) << "num_samples is not even: " << num_samples;

    // TODO: we should cache left/right and current state, instad of recomputing
    // it for each packet, 100 times a second.
    HearingDevice* left = nullptr;
    HearingDevice* right = nullptr;
    for (auto& device : hearingDevices.devices) {
      if (!device.accepting_audio) continue;

      if (device.isLeft())
        left = &device;
      else
        right = &device;
    }

    if (left == nullptr && right == nullptr) {
      LOG(WARNING) << __func__ << ": No more (0/" << GetDeviceCount()
                   << ") devices ready";
      DoDisconnectAudioStop();
      return;
    }

    std::vector<uint16_t> chan_left;
    std::vector<uint16_t> chan_right;
    if (left == nullptr || right == nullptr) {
      for (int i = 0; i < num_samples; i++) {
        const uint8_t* sample = data.data() + i * 4;

        int16_t left = (int16_t)((*(sample + 1) << 8) + *sample) >> 1;

        sample += 2;
        int16_t right = (int16_t)((*(sample + 1) << 8) + *sample) >> 1;

        uint16_t mono_data = (int16_t)(((uint32_t)left + (uint32_t)right) >> 1);
        chan_left.push_back(mono_data);
        chan_right.push_back(mono_data);
      }
    } else {
      for (int i = 0; i < num_samples; i++) {
        const uint8_t* sample = data.data() + i * 4;

        uint16_t left = (int16_t)((*(sample + 1) << 8) + *sample) >> 1;
        chan_left.push_back(left);

        sample += 2;
        uint16_t right = (int16_t)((*(sample + 1) << 8) + *sample) >> 1;
        chan_right.push_back(right);
      }
    }

    // TODO: monural, binarual check

    // divide encoded data into packets, add header, send.

    // TODO: make those buffers static and global to prevent constant
    // reallocations
    // TODO: this should basically fit the encoded data, tune the size later
    std::vector<uint8_t> encoded_data_left;
    if (left) {
      // TODO: instead of a magic number, we need to figure out the correct
      // buffer size
      encoded_data_left.resize(4000);
      int encoded_size =
          g722_encode(encoder_state_left, encoded_data_left.data(),
                      (const int16_t*)chan_left.data(), chan_left.size());
      encoded_data_left.resize(encoded_size);

      uint16_t cid = GAP_ConnGetL2CAPCid(left->gap_handle);
      uint16_t packets_to_flush = L2CA_FlushChannel(cid, L2CAP_FLUSH_CHANS_GET);
      if (packets_to_flush) {
        VLOG(2) << left->address << " skipping " << packets_to_flush
                << " packets";
        left->audio_stats.packet_flush_count += packets_to_flush;
        left->audio_stats.frame_flush_count++;
        hearingDevices.StartRssiLog();
      }
      // flush all packets stuck in queue
      L2CA_FlushChannel(cid, 0xffff);
      check_and_do_rssi_read(left);
    }

    std::vector<uint8_t> encoded_data_right;
    if (right) {
      // TODO: instead of a magic number, we need to figure out the correct
      // buffer size
      encoded_data_right.resize(4000);
      int encoded_size =
          g722_encode(encoder_state_right, encoded_data_right.data(),
                      (const int16_t*)chan_right.data(), chan_right.size());
      encoded_data_right.resize(encoded_size);

      uint16_t cid = GAP_ConnGetL2CAPCid(right->gap_handle);
      uint16_t packets_to_flush = L2CA_FlushChannel(cid, L2CAP_FLUSH_CHANS_GET);
      if (packets_to_flush) {
        VLOG(2) << right->address << " skipping " << packets_to_flush
                << " packets";
        right->audio_stats.packet_flush_count += packets_to_flush;
        right->audio_stats.frame_flush_count++;
        hearingDevices.StartRssiLog();
      }
      // flush all packets stuck in queue
      L2CA_FlushChannel(cid, 0xffff);
      check_and_do_rssi_read(right);
    }

    size_t encoded_data_size =
        std::max(encoded_data_left.size(), encoded_data_right.size());

    uint16_t packet_size =
        CalcCompressedAudioPacketSize(codec_in_use, default_data_interval_ms);

    for (size_t i = 0; i < encoded_data_size; i += packet_size) {
      if (left) {
        left->audio_stats.packet_send_count++;
        SendAudio(encoded_data_left.data() + i, packet_size, left);
      }
      if (right) {
        right->audio_stats.packet_send_count++;
        SendAudio(encoded_data_right.data() + i, packet_size, right);
      }
      seq_counter++;
    }
    if (left) left->audio_stats.frame_send_count++;
    if (right) right->audio_stats.frame_send_count++;
  }

  void SendAudio(uint8_t* encoded_data, uint16_t packet_size,
                 HearingDevice* hearingAid) {
    if (!hearingAid->playback_started || !hearingAid->command_acked) {
      VLOG(2) << __func__
              << ": Playback stalled, device=" << hearingAid->address
              << ", cmd send=" << hearingAid->playback_started
              << ", cmd acked=" << hearingAid->command_acked;
      return;
    }

    BT_HDR* audio_packet = malloc_l2cap_buf(packet_size + 1);
    uint8_t* p = get_l2cap_sdu_start_ptr(audio_packet);
    *p = seq_counter;
    p++;
    memcpy(p, encoded_data, packet_size);

    DVLOG(2) << hearingAid->address << " : " << base::HexEncode(p, packet_size);

    uint16_t result = GAP_ConnWriteData(hearingAid->gap_handle, audio_packet);

    if (result != BT_PASS) {
      LOG(ERROR) << " Error sending data: " << loghex(result);
    }
  }

  void GapCallback(uint16_t gap_handle, uint16_t event, tGAP_CB_DATA* data) {
    HearingDevice* hearingDevice = hearingDevices.FindByGapHandle(gap_handle);
    if (!hearingDevice) {
      LOG(INFO) << "Skipping unknown device, gap_handle=" << gap_handle;
      return;
    }

    switch (event) {
      case GAP_EVT_CONN_OPENED: {
        RawAddress address = *GAP_ConnGetRemoteAddr(gap_handle);
        uint16_t tx_mtu = GAP_ConnGetRemMtuSize(gap_handle);

        LOG(INFO) << "GAP_EVT_CONN_OPENED " << address << ", tx_mtu=" << tx_mtu;
        OnGapConnection(address);
        break;
      }

      // TODO: handle properly!
      case GAP_EVT_CONN_CLOSED:
        LOG(INFO) << __func__
                  << ": GAP_EVT_CONN_CLOSED: " << hearingDevice->address
                  << ", playback_started=" << hearingDevice->playback_started;
        hearingDevice->accepting_audio = false;
        hearingDevice->gap_handle = 0;
        hearingDevice->playback_started = false;
        hearingDevice->command_acked = false;
        break;
      case GAP_EVT_CONN_DATA_AVAIL: {
        DVLOG(2) << "GAP_EVT_CONN_DATA_AVAIL";

        // only data we receive back from hearing aids are some stats, not
        // really important, but useful now for debugging.
        uint32_t bytes_to_read = 0;
        GAP_GetRxQueueCnt(gap_handle, &bytes_to_read);
        std::vector<uint8_t> buffer(bytes_to_read);

        uint16_t bytes_read = 0;
        // TODO:GAP_ConnReadData should accpet uint32_t for length!
        GAP_ConnReadData(gap_handle, buffer.data(), buffer.size(), &bytes_read);

        if (bytes_read < 4) {
          LOG(WARNING) << " Wrong data length";
          return;
        }

        uint8_t* p = buffer.data();

        DVLOG(1) << "stats from the hearing aid:";
        for (size_t i = 0; i + 4 <= buffer.size(); i += 4) {
          uint16_t event_counter, frame_index;
          STREAM_TO_UINT16(event_counter, p);
          STREAM_TO_UINT16(frame_index, p);
          DVLOG(1) << "event_counter=" << event_counter
                   << " frame_index: " << frame_index;
        }
        break;
      }

      case GAP_EVT_TX_EMPTY:
        DVLOG(2) << "GAP_EVT_TX_EMPTY";
        break;
      case GAP_EVT_CONN_CONGESTED:
        DVLOG(2) << "GAP_EVT_CONN_CONGESTED";

        // TODO: make it into function
        HearingAidAudioSource::Stop();
        // TODO: kill the encoder only if all hearing aids are down.
        // g722_encode_release(encoder_state);
        // encoder_state_left = nulllptr;
        // encoder_state_right = nulllptr;
        break;
      case GAP_EVT_CONN_UNCONGESTED:
        DVLOG(2) << "GAP_EVT_CONN_UNCONGESTED";
        break;

      case GAP_EVT_LE_COC_CREDITS: {
        auto& tmp = data->coc_credits;
        DVLOG(2) << "GAP_EVT_LE_COC_CREDITS, for device: "
                 << hearingDevice->address << " added" << tmp.credits_received
                 << " credit_count: " << tmp.credit_count;
        break;
      }
    }
  }

  static void GapCallbackStatic(uint16_t gap_handle, uint16_t event,
                                tGAP_CB_DATA* data) {
    if (instance) instance->GapCallback(gap_handle, event, data);
  }

  void DumpRssi(int fd, const HearingDevice& device) {
    const struct AudioStats* stats = &device.audio_stats;

    if (stats->rssi_history.size() <= 0) {
      dprintf(fd, "  No RSSI history for %s:\n", device.address.ToString().c_str());
      return;
    }
    dprintf(fd, "  RSSI history for %s:\n", device.address.ToString().c_str());

    dprintf(fd, "    Time of RSSI    0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9\n");
    for (auto& rssi_logs : stats->rssi_history) {
      if (rssi_logs.rssi.size() <= 0) {
        break;
      }

      char eventtime[20];
      char temptime[20];
      struct tm* tstamp = localtime(&rssi_logs.timestamp.tv_sec);
      if (!strftime(temptime, sizeof(temptime), "%H:%M:%S", tstamp)) {
        LOG(ERROR) << __func__ << ": strftime fails. tm_sec=" << tstamp->tm_sec << ", tm_min=" << tstamp->tm_min
                   << ", tm_hour=" << tstamp->tm_hour;
        strlcpy(temptime, "UNKNOWN TIME", sizeof(temptime));
      }
      snprintf(eventtime, sizeof(eventtime), "%s.%03ld", temptime, rssi_logs.timestamp.tv_nsec / 1000000);

      dprintf(fd, "    %s: ", eventtime);

      for (auto rssi_value : rssi_logs.rssi) {
        dprintf(fd, " %04d", rssi_value);
      }
      dprintf(fd, "\n");
    }
  }

  void Dump(int fd) {
    std::stringstream stream;
    for (const auto& device : hearingDevices.devices) {
      bool side = device.capabilities & CAPABILITY_SIDE;
      bool standalone = device.capabilities & CAPABILITY_BINAURAL;
      stream << "  " << device.address.ToString() << " "
             << (device.accepting_audio ? "" : "not ") << "connected"
             << "\n    " << (standalone ? "binaural" : "monaural") << " "
             << (side ? "right" : "left") << " " << loghex(device.hi_sync_id)
             << std::endl;
      stream
          << "    Packet counts (enqueued/flushed)                        : "
          << device.audio_stats.packet_send_count << " / "
          << device.audio_stats.packet_flush_count
          << "\n    Frame counts (enqueued/flushed)                         : "
          << device.audio_stats.frame_send_count << " / "
          << device.audio_stats.frame_flush_count << std::endl;

      DumpRssi(fd, device);
    }
    dprintf(fd, "%s", stream.str().c_str());
  }

  void Disconnect(const RawAddress& address) override {
    DVLOG(2) << __func__;
    HearingDevice* hearingDevice = hearingDevices.FindByAddress(address);
    if (!hearingDevice) {
      LOG(INFO) << "Device not connected to profile" << address;
      return;
    }

    VLOG(2) << __func__ << ": " << address;

    bool connected = hearingDevice->accepting_audio;

    LOG(INFO) << "GAP_EVT_CONN_CLOSED: " << hearingDevice->address
              << ", playback_started=" << hearingDevice->playback_started
              << ", accepting_audio=" << hearingDevice->accepting_audio;

    if (hearingDevice->connecting_actively) {
      // cancel pending direct connect
      BTA_GATTC_CancelOpen(gatt_if, address, true);
    }

    // Removes all registrations for connection.
    BTA_GATTC_CancelOpen(0, address, false);

    // Inform the other side (if any) of this disconnection
    std::vector<uint8_t> inform_disconn_state(
        {CONTROL_POINT_OP_STATE_CHANGE, STATE_CHANGE_OTHER_SIDE_DISCONNECTED});
    send_state_change_to_other_side(hearingDevice, inform_disconn_state);

    DoDisconnectCleanUp(hearingDevice);

    hearingDevices.Remove(address);

    if (!connected) {
      return;
    }

    callbacks->OnConnectionState(ConnectionState::DISCONNECTED, address);
    for (const auto& device : hearingDevices.devices) {
      if (device.accepting_audio) return;
    }
    LOG(INFO) << __func__ << ": No more (0/" << GetDeviceCount()
              << ") devices ready";
    DoDisconnectAudioStop();
  }

  void OnGattDisconnected(tGATT_STATUS status, uint16_t conn_id,
                          tGATT_IF client_if, RawAddress remote_bda,
                          tBTA_GATT_REASON reason) {
    HearingDevice* hearingDevice = hearingDevices.FindByConnId(conn_id);
    if (!hearingDevice) {
      VLOG(2) << "Skipping unknown device disconnect, conn_id="
              << loghex(conn_id);
      return;
    }
    VLOG(2) << __func__ << ": conn_id=" << loghex(conn_id)
            << ", reason=" << loghex(reason) << ", remote_bda=" << remote_bda;

    // Inform the other side (if any) of this disconnection
    std::vector<uint8_t> inform_disconn_state(
        {CONTROL_POINT_OP_STATE_CHANGE, STATE_CHANGE_OTHER_SIDE_DISCONNECTED});
    send_state_change_to_other_side(hearingDevice, inform_disconn_state);

    DoDisconnectCleanUp(hearingDevice);

    // Keep this hearing aid in the list, and allow to reconnect back.

    callbacks->OnConnectionState(ConnectionState::DISCONNECTED, remote_bda);

    for (const auto& device : hearingDevices.devices) {
      if (device.accepting_audio) return;
    }
    LOG(INFO) << __func__ << ": No more (0/" << GetDeviceCount()
              << ") devices ready";
    DoDisconnectAudioStop();
  }

  void DoDisconnectCleanUp(HearingDevice* hearingDevice) {
    if (hearingDevice->connection_update_status != COMPLETED) {
      LOG(INFO) << __func__ << ": connection update not completed. Current="
                << hearingDevice->connection_update_status
                << ", device=" << hearingDevice->address;

      if (hearingDevice->connection_update_status == STARTED) {
        OnConnectionUpdateComplete(hearingDevice->conn_id, NULL);
      }
    }
    hearingDevice->connection_update_status = NONE;

    if (hearingDevice->conn_id) {
      BtaGattQueue::Clean(hearingDevice->conn_id);
      BTA_GATTC_Close(hearingDevice->conn_id);
      hearingDevice->conn_id = 0;
    }

    if (hearingDevice->gap_handle) {
      GAP_ConnClose(hearingDevice->gap_handle);
      hearingDevice->gap_handle = 0;
    }

    hearingDevice->accepting_audio = false;
    LOG(INFO) << __func__ << ": device=" << hearingDevice->address
              << ", playback_started=" << hearingDevice->playback_started;
    hearingDevice->playback_started = false;
    hearingDevice->command_acked = false;
  }

  void DoDisconnectAudioStop() {
    HearingAidAudioSource::Stop();
    audio_running = false;
    encoder_state_release();
    current_volume = VOLUME_UNKNOWN;
  }

  void SetVolume(int8_t volume) override {
    VLOG(2) << __func__ << ": " << +volume;
    current_volume = volume;
    for (HearingDevice& device : hearingDevices.devices) {
      if (!device.accepting_audio) continue;

      std::vector<uint8_t> volume_value({static_cast<unsigned char>(volume)});
      BtaGattQueue::WriteCharacteristic(device.conn_id, device.volume_handle,
                                        volume_value, GATT_WRITE_NO_RSP,
                                        nullptr, nullptr);
    }
  }

  void CleanUp() {
    BTA_GATTC_AppDeregister(gatt_if);
    for (HearingDevice& device : hearingDevices.devices) {
      DoDisconnectCleanUp(&device);
    }

    hearingDevices.devices.clear();

    encoder_state_release();
  }

 private:
  uint8_t gatt_if;
  uint8_t seq_counter;
  /* current volume gain for the hearing aids*/
  int8_t current_volume;
  bluetooth::hearing_aid::HearingAidCallbacks* callbacks;

  /* currently used codec */
  uint8_t codec_in_use;

  uint16_t default_data_interval_ms;

  HearingDevices hearingDevices;

  void find_server_changed_ccc_handle(uint16_t conn_id,
                                      const gatt::Service* service) {
    HearingDevice* hearingDevice = hearingDevices.FindByConnId(conn_id);
    if (!hearingDevice) {
      DVLOG(2) << "Skipping unknown device, conn_id=" << loghex(conn_id);
      return;
    }
    for (const gatt::Characteristic& charac : service->characteristics) {
      if (charac.uuid == Uuid::From16Bit(GATT_UUID_GATT_SRV_CHGD)) {
        hearingDevice->service_changed_ccc_handle =
            find_ccc_handle(conn_id, charac.value_handle);
        if (!hearingDevice->service_changed_ccc_handle) {
          LOG(ERROR) << __func__
                     << ": cannot find service changed CCC descriptor";
          continue;
        }
        LOG(INFO) << __func__ << " service_changed_ccc="
                  << loghex(hearingDevice->service_changed_ccc_handle);
        break;
      }
    }
  }

  // Find the handle for the client characteristics configuration of a given
  // characteristics
  uint16_t find_ccc_handle(uint16_t conn_id, uint16_t char_handle) {
    const gatt::Characteristic* p_char =
        BTA_GATTC_GetCharacteristic(conn_id, char_handle);

    if (!p_char) {
      LOG(WARNING) << __func__ << ": No such characteristic: " << char_handle;
      return 0;
    }

    for (const gatt::Descriptor& desc : p_char->descriptors) {
      if (desc.uuid == Uuid::From16Bit(GATT_UUID_CHAR_CLIENT_CONFIG))
        return desc.handle;
    }

    return 0;
  }

  void send_state_change(HearingDevice* device, std::vector<uint8_t> payload) {
    if (device->conn_id != 0) {
      if (device->service_changed_rcvd) {
        LOG(INFO)
            << __func__
            << ": service discover is in progress, skip send State Change cmd.";
        return;
      }
      // Send the data packet
      LOG(INFO) << __func__ << ": Send State Change. device=" << device->address
                << ", status=" << loghex(payload[1]);
      BtaGattQueue::WriteCharacteristic(
          device->conn_id, device->audio_control_point_handle, payload,
          GATT_WRITE_NO_RSP, nullptr, nullptr);
    }
  }

  void send_state_change_to_other_side(HearingDevice* this_side_device,
                                       std::vector<uint8_t> payload) {
    for (auto& device : hearingDevices.devices) {
      if ((device.address == this_side_device->address) ||
          (device.hi_sync_id != this_side_device->hi_sync_id)) {
        continue;
      }
      send_state_change(&device, payload);
    }
  }

  void check_and_do_rssi_read(HearingDevice* device) {
    if (device->read_rssi_count > 0) {
      device->num_intervals_since_last_rssi_read++;
      if (device->num_intervals_since_last_rssi_read >= PERIOD_TO_READ_RSSI_IN_INTERVALS) {
        device->num_intervals_since_last_rssi_read = 0;
        VLOG(1) << __func__ << ": device=" << device->address;
        BTM_ReadRSSI(device->address, read_rssi_cb);
      }
    }
  }
};

void read_rssi_cb(void* p_void) {
  tBTM_RSSI_RESULT* p_result = (tBTM_RSSI_RESULT*)p_void;

  if (!p_result) return;

  if ((instance) && (p_result->status == BTM_SUCCESS)) {
    instance->OnReadRssiComplete(p_result->rem_bda, p_result->rssi);
  }
}

void hearingaid_gattc_callback(tBTA_GATTC_EVT event, tBTA_GATTC* p_data) {
  VLOG(2) << __func__ << " event = " << +event;

  if (p_data == nullptr) return;

  switch (event) {
    case BTA_GATTC_DEREG_EVT:
      break;

    case BTA_GATTC_OPEN_EVT: {
      if (!instance) return;
      tBTA_GATTC_OPEN& o = p_data->open;
      instance->OnGattConnected(o.status, o.conn_id, o.client_if, o.remote_bda,
                                o.transport, o.mtu);
      break;
    }

    case BTA_GATTC_CLOSE_EVT: {
      if (!instance) return;
      tBTA_GATTC_CLOSE& c = p_data->close;
      instance->OnGattDisconnected(c.status, c.conn_id, c.client_if,
                                   c.remote_bda, c.reason);
    } break;

    case BTA_GATTC_SEARCH_CMPL_EVT:
      if (!instance) return;
      instance->OnServiceSearchComplete(p_data->search_cmpl.conn_id,
                                        p_data->search_cmpl.status);
      break;

    case BTA_GATTC_NOTIF_EVT:
      if (!instance) return;
      if (!p_data->notify.is_notify || p_data->notify.len > GATT_MAX_ATTR_LEN) {
        LOG(ERROR) << __func__ << ": rejected BTA_GATTC_NOTIF_EVT. is_notify="
                   << p_data->notify.is_notify
                   << ", len=" << p_data->notify.len;
        break;
      }
      instance->OnNotificationEvent(p_data->notify.conn_id,
                                    p_data->notify.handle, p_data->notify.len,
                                    p_data->notify.value);
      break;

    case BTA_GATTC_ENC_CMPL_CB_EVT:
      if (!instance) return;
      instance->OnEncryptionComplete(p_data->enc_cmpl.remote_bda, true);
      break;

    case BTA_GATTC_CONN_UPDATE_EVT:
      if (!instance) return;
      instance->OnConnectionUpdateComplete(p_data->conn_update.conn_id, p_data);
      break;

    case BTA_GATTC_SRVC_CHG_EVT:
      if (!instance) return;
      instance->OnServiceChangeEvent(p_data->remote_bda);
      break;

    case BTA_GATTC_SRVC_DISC_DONE_EVT:
      if (!instance) return;
      instance->OnServiceDiscDoneEvent(p_data->remote_bda);
      break;

    default:
      break;
  }
}

void encryption_callback(const RawAddress* address, tGATT_TRANSPORT, void*,
                         tBTM_STATUS status) {
  if (instance) {
    instance->OnEncryptionComplete(*address,
                                   status == BTM_SUCCESS ? true : false);
  }
}

class HearingAidAudioReceiverImpl : public HearingAidAudioReceiver {
 public:
  void OnAudioDataReady(const std::vector<uint8_t>& data) override {
    if (instance) instance->OnAudioDataReady(data);
  }
  void OnAudioSuspend(const std::function<void()>& stop_audio_ticks) override {
    if (instance) instance->OnAudioSuspend(stop_audio_ticks);
  }
  void OnAudioResume(const std::function<void()>& start_audio_ticks) override {
    if (instance) instance->OnAudioResume(start_audio_ticks);
  }
};

HearingAidAudioReceiverImpl audioReceiverImpl;

}  // namespace

void HearingAid::Initialize(
    bluetooth::hearing_aid::HearingAidCallbacks* callbacks, Closure initCb) {
  if (instance) {
    LOG(ERROR) << "Already initialized!";
  }

  audioReceiver = &audioReceiverImpl;
  instance = new HearingAidImpl(callbacks, initCb);
  HearingAidAudioSource::Initialize();
}

bool HearingAid::IsHearingAidRunning() { return instance; }

HearingAid* HearingAid::Get() {
  CHECK(instance);
  return instance;
};

void HearingAid::AddFromStorage(const HearingDevice& dev_info,
                                uint16_t is_white_listed) {
  if (!instance) {
    LOG(ERROR) << "Not initialized yet";
  }

  instance->AddFromStorage(dev_info, is_white_listed);
};

int HearingAid::GetDeviceCount() {
  if (!instance) {
    LOG(INFO) << __func__ << ": Not initialized yet";
    return 0;
  }

  return (instance->GetDeviceCount());
}

void HearingAid::CleanUp() {
  // Must stop audio source to make sure it doesn't call any of callbacks on our
  // soon to be  null instance
  HearingAidAudioSource::Stop();

  HearingAidImpl* ptr = instance;
  instance = nullptr;
  HearingAidAudioSource::CleanUp();

  ptr->CleanUp();

  delete ptr;
};

void HearingAid::DebugDump(int fd) {
  dprintf(fd, "Hearing Aid Manager:\n");
  if (instance) instance->Dump(fd);
  HearingAidAudioSource::DebugDump(fd);
  dprintf(fd, "\n");
}
