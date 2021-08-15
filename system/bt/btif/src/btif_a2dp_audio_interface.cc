/******************************************************************************
 *
 *  Copyright (C) 2017 Google, Inc.
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

#define LOG_TAG "btif_a2dp_audio_interface"

#include "btif_a2dp_audio_interface.h"

#include <mutex>

#include <a2dp_vendor.h>
#include <a2dp_vendor_ldac_constants.h>
#include <android/hardware/bluetooth/a2dp/1.0/IBluetoothAudioHost.h>
#include <android/hardware/bluetooth/a2dp/1.0/IBluetoothAudioOffload.h>
#include <android/hardware/bluetooth/a2dp/1.0/types.h>
#include <base/logging.h>
#include <hwbinder/ProcessState.h>
#include <utils/RefBase.h>
#include "a2dp_sbc.h"
#include "bt_common.h"
#include "bta/av/bta_av_int.h"
#include "btif_a2dp.h"
#include "btif_a2dp_control.h"
#include "btif_a2dp_sink.h"
#include "btif_a2dp_source.h"
#include "btif_av.h"
#include "btif_av_co.h"
#include "btif_hf.h"
#include "common/metrics.h"
#include "common/time_util.h"
#include "osi/include/osi.h"
#include "stack/include/btu.h"

using bluetooth::common::A2dpSessionMetrics;
using bluetooth::common::BluetoothMetricsLogger;

using android::hardware::bluetooth::a2dp::V1_0::IBluetoothAudioOffload;
using android::hardware::bluetooth::a2dp::V1_0::IBluetoothAudioHost;
using android::hardware::bluetooth::a2dp::V1_0::Status;
using android::hardware::bluetooth::a2dp::V1_0::CodecConfiguration;
using android::hardware::bluetooth::a2dp::V1_0::CodecType;
using android::hardware::bluetooth::a2dp::V1_0::SampleRate;
using android::hardware::bluetooth::a2dp::V1_0::BitsPerSample;
using android::hardware::bluetooth::a2dp::V1_0::ChannelMode;
using android::hardware::ProcessState;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_death_recipient;
using ::android::hardware::hidl_vec;
using ::android::sp;
using ::android::wp;
android::sp<IBluetoothAudioOffload> btAudio;

#define CASE_RETURN_STR(const) \
  case const:                  \
    return #const;
static uint8_t a2dp_cmd_pending = A2DP_CTRL_CMD_NONE;
static Status mapToStatus(uint8_t resp);
uint8_t btif_a2dp_audio_process_request(uint8_t cmd);

static void btif_a2dp_audio_send_start_req();
static void btif_a2dp_audio_send_suspend_req();
static void btif_a2dp_audio_send_stop_req();
static void btif_a2dp_audio_interface_init();
static void btif_a2dp_audio_interface_deinit();
static void btif_a2dp_audio_interface_restart_session();
// Delay reporting
// static void btif_a2dp_audio_send_sink_latency();

class A2dpOffloadAudioStats {
 public:
  A2dpOffloadAudioStats() { Reset(); }
  void Reset() {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    ResetPreserveSession();
    codec_index_ = -1;
  }
  void ResetPreserveSession() {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    audio_start_time_ms_ = -1;
    audio_stop_time_ms_ = -1;
  }
  void StoreMetrics() {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    if (audio_start_time_ms_ < 0 || audio_stop_time_ms_ < 0) {
      return;
    }
    A2dpSessionMetrics metrics;
    metrics.codec_index = codec_index_;
    metrics.is_a2dp_offload = true;
    if (audio_stop_time_ms_ > audio_start_time_ms_) {
      metrics.audio_duration_ms = audio_stop_time_ms_ - audio_start_time_ms_;
    }
    BluetoothMetricsLogger::GetInstance()->LogA2dpSession(metrics);
  }
  void LogAudioStart() {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    audio_start_time_ms_ = bluetooth::common::time_get_os_boottime_ms();
  }
  void LogAudioStop() {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    audio_stop_time_ms_ = bluetooth::common::time_get_os_boottime_ms();
  }
  void LogAudioStopMetricsAndReset() {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    LogAudioStop();
    StoreMetrics();
    ResetPreserveSession();
  }
  void SetCodecIndex(int64_t codec_index) {
    std::lock_guard<std::recursive_mutex> lock(lock_);
    codec_index_ = codec_index;
  }

 private:
  std::recursive_mutex lock_;
  int64_t audio_start_time_ms_ = -1;
  int64_t audio_stop_time_ms_ = -1;
  int64_t codec_index_ = -1;
};

static A2dpOffloadAudioStats a2dp_offload_audio_stats;

class BluetoothAudioHost : public IBluetoothAudioHost {
 public:
  Return<void> startStream() {
    btif_a2dp_audio_send_start_req();
    return Void();
  }
  Return<void> suspendStream() {
    btif_a2dp_audio_send_suspend_req();
    return Void();
  }
  Return<void> stopStream() {
    btif_a2dp_audio_send_stop_req();
    return Void();
  }

  // TODO : Delay reporting
  /*    Return<void> a2dp_get_sink_latency() {
          LOG_INFO(LOG_TAG,"%s:start ", __func__);
          btif_a2dp_audio_send_sink_latency();
          return Void();
      }*/
};

class BluetoothAudioDeathRecipient : public hidl_death_recipient {
 public:
  virtual void serviceDied(
      uint64_t /*cookie*/,
      const wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
    LOG_ERROR(LOG_TAG, "%s", __func__);
    // Restart the session on the correct thread
    do_in_main_thread(FROM_HERE,
                      base::Bind(&btif_a2dp_audio_interface_restart_session));
  }
};
sp<BluetoothAudioDeathRecipient> bluetoothAudioDeathRecipient =
    new BluetoothAudioDeathRecipient();

static Status mapToStatus(uint8_t resp) {
  switch (resp) {
    case A2DP_CTRL_ACK_SUCCESS:
      return Status::SUCCESS;
      break;
    case A2DP_CTRL_ACK_PENDING:
      return Status::PENDING;
      break;
    case A2DP_CTRL_ACK_FAILURE:
    case A2DP_CTRL_ACK_INCALL_FAILURE:
    case A2DP_CTRL_ACK_DISCONNECT_IN_PROGRESS:
      return Status::FAILURE;
    default:
      APPL_TRACE_WARNING("%s: unknown status recevied :%d", __func__, resp);
      return Status::FAILURE;
      break;
  }
}

static void btif_a2dp_get_codec_configuration(
    CodecConfiguration* p_codec_info) {
  LOG_INFO(LOG_TAG, "%s", __func__);
  tBT_A2DP_OFFLOAD a2dp_offload;
  A2dpCodecConfig* a2dpCodecConfig = bta_av_get_a2dp_current_codec();
  a2dpCodecConfig->getCodecSpecificConfig(&a2dp_offload);
  btav_a2dp_codec_config_t codec_config;
  codec_config = a2dpCodecConfig->getCodecConfig();
  a2dp_offload_audio_stats.SetCodecIndex(a2dpCodecConfig->codecIndex());
  switch (codec_config.codec_type) {
    case BTAV_A2DP_CODEC_INDEX_SOURCE_SBC:
      p_codec_info->codecType =
          (::android::hardware::bluetooth::a2dp::V1_0::CodecType)
              BTA_AV_CODEC_TYPE_SBC;
      p_codec_info->codecSpecific.sbcData.codecParameters =
          a2dp_offload.codec_info[0];
      LOG_INFO(LOG_TAG, " %s: codec parameters =%d", __func__,
               a2dp_offload.codec_info[0]);
      p_codec_info->codecSpecific.sbcData.minBitpool =
          a2dp_offload.codec_info[1];
      p_codec_info->codecSpecific.sbcData.maxBitpool =
          a2dp_offload.codec_info[2];
      break;
    case BTAV_A2DP_CODEC_INDEX_SOURCE_AAC:
      p_codec_info->codecType =
          (::android::hardware::bluetooth::a2dp::V1_0::CodecType)
              BTA_AV_CODEC_TYPE_AAC;
      break;
    case BTAV_A2DP_CODEC_INDEX_SOURCE_APTX:
      p_codec_info->codecType =
          (::android::hardware::bluetooth::a2dp::V1_0::CodecType)
              BTA_AV_CODEC_TYPE_APTX;
      break;
    case BTAV_A2DP_CODEC_INDEX_SOURCE_APTX_HD:
      p_codec_info->codecType =
          (::android::hardware::bluetooth::a2dp::V1_0::CodecType)
              BTA_AV_CODEC_TYPE_APTXHD;
      break;
    case BTAV_A2DP_CODEC_INDEX_SOURCE_LDAC:
      p_codec_info->codecType =
          (::android::hardware::bluetooth::a2dp::V1_0::CodecType)
              BTA_AV_CODEC_TYPE_LDAC;
      p_codec_info->codecSpecific.ldacData.bitrateIndex =
          a2dp_offload.codec_info[6];
      break;
    default:
      APPL_TRACE_ERROR("%s: Unknown Codec type :%d ", __func__,
                       codec_config.codec_type);
  }

  // Obtain the MTU
  RawAddress peer_addr = btif_av_source_active_peer();
  tA2DP_ENCODER_INIT_PEER_PARAMS peer_param;
  bta_av_co_get_peer_params(peer_addr, &peer_param);
  int effectiveMtu = a2dpCodecConfig->getEffectiveMtu();
  if (effectiveMtu > 0 && effectiveMtu < peer_param.peer_mtu) {
    p_codec_info->peerMtu = effectiveMtu;
  } else {
    p_codec_info->peerMtu = peer_param.peer_mtu;
  }
  LOG_INFO(LOG_TAG, "%s: peer MTU: %d effective MTU: %d result MTU: %d",
           __func__, peer_param.peer_mtu, effectiveMtu, p_codec_info->peerMtu);

  p_codec_info->sampleRate =
      (::android::hardware::bluetooth::a2dp::V1_0::SampleRate)
          codec_config.sample_rate;
  p_codec_info->bitsPerSample =
      (::android::hardware::bluetooth::a2dp::V1_0::BitsPerSample)
          codec_config.bits_per_sample;
  p_codec_info->channelMode =
      (::android::hardware::bluetooth::a2dp::V1_0::ChannelMode)
          codec_config.channel_mode;
  p_codec_info->encodedAudioBitrate = a2dpCodecConfig->getTrackBitRate();
}

static void btif_a2dp_audio_interface_init() {
  LOG_INFO(LOG_TAG, "%s", __func__);

  btAudio = IBluetoothAudioOffload::getService();
  CHECK(btAudio != nullptr);

  auto death_link = btAudio->linkToDeath(bluetoothAudioDeathRecipient, 0);
  if (!death_link.isOk()) {
    LOG_ERROR(LOG_TAG, "%s: Cannot observe the Bluetooth Audio HAL's death",
              __func__);
  }

  LOG_DEBUG(
      LOG_TAG, "%s: IBluetoothAudioOffload::getService() returned %p (%s)",
      __func__, btAudio.get(), (btAudio->isRemote() ? "remote" : "local"));

  LOG_INFO(LOG_TAG, "%s:Init returned", __func__);
}

static void btif_a2dp_audio_interface_deinit() {
  LOG_INFO(LOG_TAG, "%s: start", __func__);
  if (btAudio != nullptr) {
    auto death_unlink = btAudio->unlinkToDeath(bluetoothAudioDeathRecipient);
    if (!death_unlink.isOk()) {
      LOG_ERROR(LOG_TAG,
                "%s: Error unlinking death observer from Bluetooth Audio HAL",
                __func__);
    }
  }
  btAudio = nullptr;
}

void btif_a2dp_audio_interface_start_session() {
  LOG_INFO(LOG_TAG, "%s", __func__);
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionStart(
      bluetooth::common::CONNECTION_TECHNOLOGY_TYPE_BREDR, 0);
  a2dp_offload_audio_stats.Reset();
  btif_a2dp_audio_interface_init();
  CHECK(btAudio != nullptr);
  CodecConfiguration codec_info;
  btif_a2dp_get_codec_configuration(&codec_info);
  android::sp<IBluetoothAudioHost> host_if = new BluetoothAudioHost();
  btAudio->startSession(host_if, codec_info);
}

void btif_a2dp_audio_interface_end_session() {
  LOG_INFO(LOG_TAG, "%s", __func__);
  a2dp_offload_audio_stats.LogAudioStopMetricsAndReset();
  BluetoothMetricsLogger::GetInstance()->LogBluetoothSessionEnd(
      bluetooth::common::DISCONNECT_REASON_UNKNOWN, 0);
  a2dp_offload_audio_stats.Reset();
  if (btAudio == nullptr) return;
  auto ret = btAudio->endSession();
  if (!ret.isOk()) {
    LOG_ERROR(LOG_TAG, "HAL server is dead");
  }
  btif_a2dp_audio_interface_deinit();
}

// Conditionally restart the session only if it was started before
static void btif_a2dp_audio_interface_restart_session() {
  LOG_INFO(LOG_TAG, "%s", __func__);
  if (btAudio == nullptr) {
    LOG_INFO(LOG_TAG, "%s: nothing to restart - session was not started",
             __func__);
    return;
  }
  btAudio = nullptr;
  btif_a2dp_audio_interface_start_session();
}

void btif_a2dp_audio_on_started(tBTA_AV_STATUS status) {
  LOG_INFO(LOG_TAG, "%s: status = %d", __func__, status);
  if (btAudio != nullptr) {
    if (a2dp_cmd_pending == A2DP_CTRL_CMD_START) {
      if (status != A2DP_CTRL_ACK_PENDING) {
        a2dp_cmd_pending = A2DP_CTRL_CMD_NONE;
      }
      LOG_INFO(LOG_TAG, "%s: calling method onStarted", __func__);
      auto hal_status = mapToStatus(status);
      btAudio->streamStarted(hal_status);
      if (hal_status == Status::SUCCESS) {
        a2dp_offload_audio_stats.LogAudioStart();
      }
    }
  }
}

void btif_a2dp_audio_on_suspended(tBTA_AV_STATUS status) {
  LOG_INFO(LOG_TAG, "%s: status = %d", __func__, status);
  if (btAudio != nullptr) {
    if (a2dp_cmd_pending == A2DP_CTRL_CMD_SUSPEND) {
      if (status != A2DP_CTRL_ACK_PENDING) {
        a2dp_cmd_pending = A2DP_CTRL_CMD_NONE;
      }
      LOG_INFO(LOG_TAG, "calling method onSuspended");
      auto hal_status = mapToStatus(status);
      btAudio->streamSuspended(hal_status);
      if (hal_status == Status::SUCCESS) {
        a2dp_offload_audio_stats.LogAudioStopMetricsAndReset();
      }
    }
  }
}

void btif_a2dp_audio_on_stopped(tBTA_AV_STATUS status) {
  LOG_INFO(LOG_TAG, "%s: status = %d", __func__, status);
  if (btAudio != nullptr && a2dp_cmd_pending == A2DP_CTRL_CMD_START) {
    a2dp_cmd_pending = A2DP_CTRL_CMD_NONE;
    LOG_INFO(LOG_TAG, "%s: Remote disconnected when start under progress",
             __func__);
    btAudio->streamStarted(mapToStatus(A2DP_CTRL_ACK_DISCONNECT_IN_PROGRESS));
    a2dp_offload_audio_stats.LogAudioStopMetricsAndReset();
  }
}
void btif_a2dp_audio_send_start_req() {
  LOG_INFO(LOG_TAG, "%s", __func__);
  uint8_t resp;
  resp = btif_a2dp_audio_process_request(A2DP_CTRL_CMD_START);
  if (btAudio != nullptr) {
    auto status = mapToStatus(resp);
    auto ret = btAudio->streamStarted(status);
    if (status == Status::SUCCESS) {
      a2dp_offload_audio_stats.LogAudioStart();
    }
    if (!ret.isOk()) LOG_ERROR(LOG_TAG, "HAL server died");
  }
}
void btif_a2dp_audio_send_suspend_req() {
  LOG_INFO(LOG_TAG, "%s", __func__);
  uint8_t resp;
  resp = btif_a2dp_audio_process_request(A2DP_CTRL_CMD_SUSPEND);
  if (btAudio != nullptr) {
    auto status = mapToStatus(resp);
    auto ret = btAudio->streamSuspended(status);
    if (status == Status::SUCCESS) {
      a2dp_offload_audio_stats.LogAudioStopMetricsAndReset();
    }
    if (!ret.isOk()) LOG_ERROR(LOG_TAG, "HAL server died");
  }
}

void btif_a2dp_audio_send_stop_req() {
  LOG_INFO(LOG_TAG, "%s", __func__);
  btif_a2dp_audio_process_request(A2DP_CTRL_CMD_STOP);
  a2dp_offload_audio_stats.LogAudioStopMetricsAndReset();
}

/*void btif_a2dp_audio_send_sink_latency()
{
  LOG_INFO(LOG_TAG, "%s", __func__);
  uint16_t sink_latency = btif_av_get_sink_latency();
  if (btAudio != nullptr) {
    auto ret = btAudio->a2dp_on_get_sink_latency(sink_latency);
    if (!ret.isOk()) LOG_ERROR(LOG_TAG, "server died");
  }
}*/

uint8_t btif_a2dp_audio_process_request(uint8_t cmd) {
  LOG_INFO(LOG_TAG, "%s: cmd: %s", __func__,
           audio_a2dp_hw_dump_ctrl_event((tA2DP_CTRL_CMD)cmd));
  uint8_t status;
  switch (cmd) {
    case A2DP_CTRL_CMD_START:
      /*
       * Don't send START request to stack while we are in a call.
       * Some headsets such as "Sony MW600", don't allow AVDTP START
       * while in a call, and respond with BAD_STATE.
       */
      if (!bluetooth::headset::IsCallIdle()) {
        APPL_TRACE_WARNING("%s: A2DP command %s failed as call state is busy",
                           __func__,
                           audio_a2dp_hw_dump_ctrl_event((tA2DP_CTRL_CMD)cmd));
        status = A2DP_CTRL_ACK_INCALL_FAILURE;
        break;
      }
      if (btif_av_stream_started_ready()) {
        /*
         * Already started, setup audio data channel listener and ACK
         * back immediately.
         */
        status = A2DP_CTRL_ACK_SUCCESS;
        break;
      }
      if (btif_av_stream_ready()) {
        /*
         * Post start event and wait for audio path to open.
         * If we are the source, the ACK will be sent after the start
         * procedure is completed, othewise send it now.
         */
        btif_av_stream_start();
        if (btif_av_get_peer_sep() == AVDT_TSEP_SRC) {
          status = A2DP_CTRL_ACK_SUCCESS;
          break;
        }
        /*Return pending and ack when start stream cfm received from remote*/
        status = A2DP_CTRL_ACK_PENDING;
        break;
      }

      APPL_TRACE_WARNING("%s: A2DP command %s while AV stream is not ready",
                         __func__,
                         audio_a2dp_hw_dump_ctrl_event((tA2DP_CTRL_CMD)cmd));
      status = A2DP_CTRL_ACK_FAILURE;
      break;

    case A2DP_CTRL_CMD_STOP:
      if (btif_av_get_peer_sep() == AVDT_TSEP_SNK &&
          !btif_a2dp_source_is_streaming()) {
        /* We are already stopped, just ack back */
        status = A2DP_CTRL_ACK_SUCCESS;
        break;
      }
      btif_av_stream_stop(RawAddress::kEmpty);
      status = A2DP_CTRL_ACK_SUCCESS;
      break;

    case A2DP_CTRL_CMD_SUSPEND:
      /* Local suspend */
      if (btif_av_stream_started_ready()) {
        btif_av_stream_suspend();
        status = A2DP_CTRL_ACK_PENDING;
        break;
      }
      /* If we are not in started state, just ack back ok and let
       * audioflinger close the channel. This can happen if we are
       * remotely suspended, clear REMOTE SUSPEND flag.
       */
      btif_av_clear_remote_suspend_flag();
      status = A2DP_CTRL_ACK_SUCCESS;
      break;

    case A2DP_CTRL_CMD_OFFLOAD_START:
      btif_av_stream_start_offload();
      status = A2DP_CTRL_ACK_PENDING;
      break;

    default:
      APPL_TRACE_ERROR("UNSUPPORTED CMD (%d)", cmd);
      status = A2DP_CTRL_ACK_FAILURE;
      break;
  }
  LOG_INFO(LOG_TAG, "a2dp-ctrl-cmd : %s DONE returning status %d",
           audio_a2dp_hw_dump_ctrl_event((tA2DP_CTRL_CMD)cmd), status);
  if (status == A2DP_CTRL_ACK_PENDING) {
    a2dp_cmd_pending = cmd;
  } else {
    a2dp_cmd_pending = A2DP_CTRL_CMD_NONE;
  }
  return status;
}
