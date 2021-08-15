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

#include "audio_hal_interface/hearing_aid_software_encoding.h"
#include "audio_hearing_aid_hw/include/audio_hearing_aid_hw.h"
#include "bta_hearing_aid_api.h"
#include "btu.h"
#include "osi/include/wakelock.h"
#include "uipc.h"

#include <base/files/file_util.h>
#include <include/hardware/bt_av.h>

#include "common/repeating_timer.h"
#include "common/time_util.h"

using base::FilePath;
extern const char* audio_ha_hw_dump_ctrl_event(tHEARING_AID_CTRL_CMD event);

namespace {
int bit_rate = -1;
int sample_rate = -1;
int data_interval_ms = -1;
int num_channels = 2;
bluetooth::common::RepeatingTimer audio_timer;
HearingAidAudioReceiver* localAudioReceiver = nullptr;
std::unique_ptr<tUIPC_STATE> uipc_hearing_aid = nullptr;

struct AudioHalStats {
  size_t media_read_total_underflow_bytes;
  size_t media_read_total_underflow_count;
  uint64_t media_read_last_underflow_us;

  AudioHalStats() { Reset(); }

  void Reset() {
    media_read_total_underflow_bytes = 0;
    media_read_total_underflow_count = 0;
    media_read_last_underflow_us = 0;
  }
};

AudioHalStats stats;

bool hearing_aid_on_resume_req(bool start_media_task);
bool hearing_aid_on_suspend_req();

void send_audio_data() {
  uint32_t bytes_per_tick =
      (num_channels * sample_rate * data_interval_ms * (bit_rate / 8)) / 1000;

  uint16_t event;
  uint8_t p_buf[bytes_per_tick];

  uint32_t bytes_read;
  if (bluetooth::audio::hearing_aid::is_hal_2_0_enabled()) {
    bytes_read = bluetooth::audio::hearing_aid::read(p_buf, bytes_per_tick);
  } else {
    bytes_read = UIPC_Read(*uipc_hearing_aid, UIPC_CH_ID_AV_AUDIO, &event,
                           p_buf, bytes_per_tick);
  }

  VLOG(2) << "bytes_read: " << bytes_read;
  if (bytes_read < bytes_per_tick) {
    stats.media_read_total_underflow_bytes += bytes_per_tick - bytes_read;
    stats.media_read_total_underflow_count++;
    stats.media_read_last_underflow_us =
        bluetooth::common::time_get_os_boottime_us();
  }

  std::vector<uint8_t> data(p_buf, p_buf + bytes_read);

  if (localAudioReceiver != nullptr) {
    localAudioReceiver->OnAudioDataReady(data);
  }
}

void hearing_aid_send_ack(tHEARING_AID_CTRL_ACK status) {
  uint8_t ack = status;
  DVLOG(2) << "Hearing Aid audio ctrl ack: " << status;
  UIPC_Send(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0, &ack, sizeof(ack));
}

void start_audio_ticks() {
  if (data_interval_ms != HA_INTERVAL_10_MS &&
      data_interval_ms != HA_INTERVAL_20_MS) {
    LOG(FATAL) << " Unsupported data interval: " << data_interval_ms;
  }

  wakelock_acquire();
  audio_timer.SchedulePeriodic(
      get_main_thread()->GetWeakPtr(), FROM_HERE, base::Bind(&send_audio_data),
      base::TimeDelta::FromMilliseconds(data_interval_ms));
  LOG(INFO) << __func__ << ": running with data interval: " << data_interval_ms;
}

void stop_audio_ticks() {
  LOG(INFO) << __func__ << ": stopped";
  audio_timer.CancelAndWait();
  wakelock_release();
}

void hearing_aid_data_cb(tUIPC_CH_ID, tUIPC_EVENT event) {
  DVLOG(2) << "Hearing Aid audio data event: " << event;
  switch (event) {
    case UIPC_OPEN_EVT:
      LOG(INFO) << __func__ << ": UIPC_OPEN_EVT";
      /*
       * Read directly from media task from here on (keep callback for
       * connection events.
       */
      UIPC_Ioctl(*uipc_hearing_aid, UIPC_CH_ID_AV_AUDIO,
                 UIPC_REG_REMOVE_ACTIVE_READSET, NULL);
      UIPC_Ioctl(*uipc_hearing_aid, UIPC_CH_ID_AV_AUDIO, UIPC_SET_READ_POLL_TMO,
                 reinterpret_cast<void*>(0));

      do_in_main_thread(FROM_HERE, base::BindOnce(start_audio_ticks));
      break;
    case UIPC_CLOSE_EVT:
      LOG(INFO) << __func__ << ": UIPC_CLOSE_EVT";
      hearing_aid_send_ack(HEARING_AID_CTRL_ACK_SUCCESS);
      do_in_main_thread(FROM_HERE, base::BindOnce(stop_audio_ticks));
      break;
    default:
      LOG(ERROR) << "Hearing Aid audio data event not recognized:" << event;
  }
}

void hearing_aid_recv_ctrl_data() {
  tHEARING_AID_CTRL_CMD cmd = HEARING_AID_CTRL_CMD_NONE;
  int n;

  uint8_t read_cmd = 0; /* The read command size is one octet */
  n = UIPC_Read(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, NULL, &read_cmd, 1);
  cmd = static_cast<tHEARING_AID_CTRL_CMD>(read_cmd);

  /* detach on ctrl channel means audioflinger process was terminated */
  if (n == 0) {
    LOG(WARNING) << __func__ << "CTRL CH DETACHED";
    UIPC_Close(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL);
    return;
  }

  LOG(INFO) << __func__ << " " << audio_ha_hw_dump_ctrl_event(cmd);
  //  a2dp_cmd_pending = cmd;

  tHEARING_AID_CTRL_ACK ctrl_ack_status;

  switch (cmd) {
    case HEARING_AID_CTRL_CMD_CHECK_READY:
      hearing_aid_send_ack(HEARING_AID_CTRL_ACK_SUCCESS);
      break;

    case HEARING_AID_CTRL_CMD_START:
      ctrl_ack_status = HEARING_AID_CTRL_ACK_SUCCESS;
      // timer is restarted in UIPC_Open
      if (!hearing_aid_on_resume_req(false)) {
        ctrl_ack_status = HEARING_AID_CTRL_ACK_FAILURE;
      } else {
        UIPC_Open(*uipc_hearing_aid, UIPC_CH_ID_AV_AUDIO, hearing_aid_data_cb,
                  HEARING_AID_DATA_PATH);
      }
      hearing_aid_send_ack(ctrl_ack_status);
      break;

    case HEARING_AID_CTRL_CMD_STOP:
      if (!hearing_aid_on_suspend_req()) {
        LOG(INFO) << __func__ << ":HEARING_AID_CTRL_CMD_STOP: hearing_aid_on_suspend_req() errs, but ignored.";
      }
      hearing_aid_send_ack(HEARING_AID_CTRL_ACK_SUCCESS);
      break;

    case HEARING_AID_CTRL_CMD_SUSPEND:
      ctrl_ack_status = HEARING_AID_CTRL_ACK_SUCCESS;
      if (!hearing_aid_on_suspend_req()) {
        ctrl_ack_status = HEARING_AID_CTRL_ACK_FAILURE;
      }
      hearing_aid_send_ack(ctrl_ack_status);
      break;

    case HEARING_AID_CTRL_GET_OUTPUT_AUDIO_CONFIG: {
      btav_a2dp_codec_config_t codec_config;
      btav_a2dp_codec_config_t codec_capability;
      if (sample_rate == 16000) {
        codec_config.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_16000;
        codec_capability.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_16000;
      } else if (sample_rate == 24000) {
        codec_config.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_24000;
        codec_capability.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_24000;
      } else {
        LOG(FATAL) << "unsupported sample rate: " << sample_rate;
      }

      codec_config.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;
      codec_capability.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_16;

      codec_config.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;
      codec_capability.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_STEREO;

      hearing_aid_send_ack(HEARING_AID_CTRL_ACK_SUCCESS);
      // Send the current codec config
      UIPC_Send(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                reinterpret_cast<const uint8_t*>(&codec_config.sample_rate),
                sizeof(btav_a2dp_codec_sample_rate_t));
      UIPC_Send(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                reinterpret_cast<const uint8_t*>(&codec_config.bits_per_sample),
                sizeof(btav_a2dp_codec_bits_per_sample_t));
      UIPC_Send(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                reinterpret_cast<const uint8_t*>(&codec_config.channel_mode),
                sizeof(btav_a2dp_codec_channel_mode_t));
      // Send the current codec capability
      UIPC_Send(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                reinterpret_cast<const uint8_t*>(&codec_capability.sample_rate),
                sizeof(btav_a2dp_codec_sample_rate_t));
      UIPC_Send(
          *uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
          reinterpret_cast<const uint8_t*>(&codec_capability.bits_per_sample),
          sizeof(btav_a2dp_codec_bits_per_sample_t));
      UIPC_Send(
          *uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
          reinterpret_cast<const uint8_t*>(&codec_capability.channel_mode),
          sizeof(btav_a2dp_codec_channel_mode_t));
      break;
    }

    case HEARING_AID_CTRL_SET_OUTPUT_AUDIO_CONFIG: {
      // TODO: we only support one config for now!
      btav_a2dp_codec_config_t codec_config;
      codec_config.sample_rate = BTAV_A2DP_CODEC_SAMPLE_RATE_NONE;
      codec_config.bits_per_sample = BTAV_A2DP_CODEC_BITS_PER_SAMPLE_NONE;
      codec_config.channel_mode = BTAV_A2DP_CODEC_CHANNEL_MODE_NONE;

      hearing_aid_send_ack(HEARING_AID_CTRL_ACK_SUCCESS);
      // Send the current codec config
      if (UIPC_Read(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                    reinterpret_cast<uint8_t*>(&codec_config.sample_rate),
                    sizeof(btav_a2dp_codec_sample_rate_t)) !=
          sizeof(btav_a2dp_codec_sample_rate_t)) {
        LOG(ERROR) << __func__ << "Error reading sample rate from audio HAL";
        break;
      }
      if (UIPC_Read(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                    reinterpret_cast<uint8_t*>(&codec_config.bits_per_sample),
                    sizeof(btav_a2dp_codec_bits_per_sample_t)) !=
          sizeof(btav_a2dp_codec_bits_per_sample_t)) {
        LOG(ERROR) << __func__
                   << "Error reading bits per sample from audio HAL";

        break;
      }
      if (UIPC_Read(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, 0,
                    reinterpret_cast<uint8_t*>(&codec_config.channel_mode),
                    sizeof(btav_a2dp_codec_channel_mode_t)) !=
          sizeof(btav_a2dp_codec_channel_mode_t)) {
        LOG(ERROR) << __func__ << "Error reading channel mode from audio HAL";

        break;
      }
      LOG(INFO) << __func__ << " HEARING_AID_CTRL_SET_OUTPUT_AUDIO_CONFIG: "
                << "sample_rate=" << codec_config.sample_rate
                << "bits_per_sample=" << codec_config.bits_per_sample
                << "channel_mode=" << codec_config.channel_mode;
      break;
    }

    default:
      LOG(ERROR) << __func__ << "UNSUPPORTED CMD: " << cmd;
      hearing_aid_send_ack(HEARING_AID_CTRL_ACK_FAILURE);
      break;
  }
  LOG(INFO) << __func__
            << " a2dp-ctrl-cmd : " << audio_ha_hw_dump_ctrl_event(cmd)
            << " DONE";
}

void hearing_aid_ctrl_cb(tUIPC_CH_ID, tUIPC_EVENT event) {
  VLOG(2) << "Hearing Aid audio ctrl event: " << event;
  switch (event) {
    case UIPC_OPEN_EVT:
      break;
    case UIPC_CLOSE_EVT:
      /* restart ctrl server unless we are shutting down */
      if (HearingAid::IsHearingAidRunning()) {
        UIPC_Open(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, hearing_aid_ctrl_cb,
                  HEARING_AID_CTRL_PATH);
      }
      break;
    case UIPC_RX_DATA_READY_EVT:
      hearing_aid_recv_ctrl_data();
      break;
    default:
      LOG(ERROR) << "Hearing Aid audio ctrl unrecognized event: " << event;
  }
}

bool hearing_aid_on_resume_req(bool start_media_task) {
  if (localAudioReceiver == nullptr) {
    LOG(ERROR) << __func__
               << ": HEARING_AID_CTRL_CMD_START: audio receiver not started";
    return false;
  }
  bt_status_t status;
  if (start_media_task) {
    status = do_in_main_thread(
        FROM_HERE, base::BindOnce(&HearingAidAudioReceiver::OnAudioResume,
                                  base::Unretained(localAudioReceiver),
                                  start_audio_ticks));
  } else {
    auto start_dummy_ticks = []() {
      LOG(INFO) << "start_audio_ticks: waiting for data path opened";
    };
    status = do_in_main_thread(
        FROM_HERE, base::BindOnce(&HearingAidAudioReceiver::OnAudioResume,
                                  base::Unretained(localAudioReceiver),
                                  start_dummy_ticks));
  }
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << __func__
               << ": HEARING_AID_CTRL_CMD_START: do_in_main_thread err="
               << status;
    return false;
  }
  return true;
}

bool hearing_aid_on_suspend_req() {
  if (localAudioReceiver == nullptr) {
    LOG(ERROR) << __func__
               << ": HEARING_AID_CTRL_CMD_SUSPEND: audio receiver not started";
    return false;
  }
  bt_status_t status = do_in_main_thread(
      FROM_HERE,
      base::BindOnce(&HearingAidAudioReceiver::OnAudioSuspend,
                     base::Unretained(localAudioReceiver), stop_audio_ticks));
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << __func__
               << ": HEARING_AID_CTRL_CMD_SUSPEND: do_in_main_thread err="
               << status;
    return false;
  }
  return true;
}
}  // namespace

void HearingAidAudioSource::Start(const CodecConfiguration& codecConfiguration,
                                  HearingAidAudioReceiver* audioReceiver,
                                  uint16_t remote_delay_ms) {
  LOG(INFO) << __func__ << ": Hearing Aid Source Open";

  bit_rate = codecConfiguration.bit_rate;
  sample_rate = codecConfiguration.sample_rate;
  data_interval_ms = codecConfiguration.data_interval_ms;

  stats.Reset();

  if (bluetooth::audio::hearing_aid::is_hal_2_0_enabled()) {
    bluetooth::audio::hearing_aid::start_session();
    bluetooth::audio::hearing_aid::set_remote_delay(remote_delay_ms);
  }
  localAudioReceiver = audioReceiver;
}

void HearingAidAudioSource::Stop() {
  LOG(INFO) << __func__ << ": Hearing Aid Source Close";

  localAudioReceiver = nullptr;
  if (bluetooth::audio::hearing_aid::is_hal_2_0_enabled()) {
    bluetooth::audio::hearing_aid::end_session();
  }

  stop_audio_ticks();
}

void HearingAidAudioSource::Initialize() {
  auto stream_cb = bluetooth::audio::hearing_aid::StreamCallbacks{
      .on_resume_ = hearing_aid_on_resume_req,
      .on_suspend_ = hearing_aid_on_suspend_req,
  };
  if (!bluetooth::audio::hearing_aid::init(stream_cb, get_main_thread())) {
    LOG(WARNING) << __func__ << ": Using legacy HAL";
    uipc_hearing_aid = UIPC_Init();
    UIPC_Open(*uipc_hearing_aid, UIPC_CH_ID_AV_CTRL, hearing_aid_ctrl_cb, HEARING_AID_CTRL_PATH);
  }
}

void HearingAidAudioSource::CleanUp() {
  if (bluetooth::audio::hearing_aid::is_hal_2_0_enabled()) {
    bluetooth::audio::hearing_aid::cleanup();
  } else {
    UIPC_Close(*uipc_hearing_aid, UIPC_CH_ID_ALL);
    uipc_hearing_aid = nullptr;
  }
}

void HearingAidAudioSource::DebugDump(int fd) {
  uint64_t now_us = bluetooth::common::time_get_os_boottime_us();
  std::stringstream stream;
  stream << "  Hearing Aid Audio HAL:"
         << "\n    Counts (underflow)                                      : "
         << stats.media_read_total_underflow_count
         << "\n    Bytes (underflow)                                       : "
         << stats.media_read_total_underflow_bytes
         << "\n    Last update time ago in ms (underflow)                  : "
         << (stats.media_read_last_underflow_us > 0
                 ? (unsigned long long)(now_us -
                                        stats.media_read_last_underflow_us) /
                       1000
                 : 0)
         << std::endl;
  dprintf(fd, "%s", stream.str().c_str());
}
