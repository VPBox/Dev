/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "a2dp_vendor_ldac_encoder"
#define ATRACE_TAG ATRACE_TAG_AUDIO

#include "a2dp_vendor_ldac_encoder.h"

#ifndef OS_GENERIC
#include <cutils/trace.h>
#endif
#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <ldacBT.h>

#include "a2dp_vendor.h"
#include "a2dp_vendor_ldac.h"
#include "a2dp_vendor_ldac_abr.h"
#include "bt_common.h"
#include "common/time_util.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

//
// Encoder for LDAC Source Codec
//

//
// The LDAC encoder shared library, and the functions to use
//
static const char* LDAC_ENCODER_LIB_NAME = "libldacBT_enc.so";
static void* ldac_encoder_lib_handle = NULL;

static const char* LDAC_GET_HANDLE_NAME = "ldacBT_get_handle";
typedef HANDLE_LDAC_BT (*tLDAC_GET_HANDLE)(void);

static const char* LDAC_FREE_HANDLE_NAME = "ldacBT_free_handle";
typedef void (*tLDAC_FREE_HANDLE)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_CLOSE_HANDLE_NAME = "ldacBT_close_handle";
typedef void (*tLDAC_CLOSE_HANDLE)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_GET_VERSION_NAME = "ldacBT_get_version";
typedef int (*tLDAC_GET_VERSION)(void);

static const char* LDAC_GET_BITRATE_NAME = "ldacBT_get_bitrate";
typedef int (*tLDAC_GET_BITRATE)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_GET_SAMPLING_FREQ_NAME = "ldacBT_get_sampling_freq";
typedef int (*tLDAC_GET_SAMPLING_FREQ)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_INIT_HANDLE_ENCODE_NAME = "ldacBT_init_handle_encode";
typedef int (*tLDAC_INIT_HANDLE_ENCODE)(HANDLE_LDAC_BT hLdacParam, int mtu,
                                        int eqmid, int channel_mode,
                                        LDACBT_SMPL_FMT_T fmt,
                                        int sampling_freq);

static const char* LDAC_ENCODE_NAME = "ldacBT_encode";
typedef int (*tLDAC_ENCODE)(HANDLE_LDAC_BT hLdacParam, void* p_pcm,
                            int* p_pcm_encoded_byte, unsigned char* p_stream,
                            int* pframe_length_wrote, int* pframe_num);

static const char* LDAC_SET_EQMID_NAME = "ldacBT_set_eqmid";
typedef int (*tLDAC_SET_EQMID)(HANDLE_LDAC_BT hLdacParam, int eqmid);

static const char* LDAC_ALTER_EQMID_PRIORITY_NAME =
    "ldacBT_alter_eqmid_priority";
typedef int (*tLDAC_ALTER_EQMID_PRIORITY)(HANDLE_LDAC_BT hLdacParam,
                                          int priority);

static const char* LDAC_GET_EQMID_NAME = "ldacBT_get_eqmid";
typedef int (*tLDAC_GET_EQMID)(HANDLE_LDAC_BT hLdacParam);

static const char* LDAC_GET_ERROR_CODE_NAME = "ldacBT_get_error_code";
typedef int (*tLDAC_GET_ERROR_CODE)(HANDLE_LDAC_BT hLdacParam);

static tLDAC_GET_HANDLE ldac_get_handle_func;
static tLDAC_FREE_HANDLE ldac_free_handle_func;
static tLDAC_CLOSE_HANDLE ldac_close_handle_func;
static tLDAC_GET_VERSION ldac_get_version_func;
static tLDAC_GET_BITRATE ldac_get_bitrate_func;
static tLDAC_GET_SAMPLING_FREQ ldac_get_sampling_freq_func;
static tLDAC_INIT_HANDLE_ENCODE ldac_init_handle_encode_func;
static tLDAC_ENCODE ldac_encode_func;
static tLDAC_SET_EQMID ldac_set_eqmid_func;
static tLDAC_ALTER_EQMID_PRIORITY ldac_alter_eqmid_priority_func;
static tLDAC_GET_EQMID ldac_get_eqmid_func;
static tLDAC_GET_ERROR_CODE ldac_get_error_code_func;

// A2DP LDAC encoder interval in milliseconds
#define A2DP_LDAC_ENCODER_INTERVAL_MS 20
#define A2DP_LDAC_MEDIA_BYTES_PER_FRAME 128

// offset
#if (BTA_AV_CO_CP_SCMS_T == TRUE)
#define A2DP_LDAC_OFFSET (AVDT_MEDIA_OFFSET + A2DP_LDAC_MPL_HDR_LEN + 1)
#else
#define A2DP_LDAC_OFFSET (AVDT_MEDIA_OFFSET + A2DP_LDAC_MPL_HDR_LEN)
#endif

typedef struct {
  uint32_t sample_rate;
  uint8_t channel_mode;
  uint8_t bits_per_sample;
  int quality_mode_index;
  int pcm_wlength;
  LDACBT_SMPL_FMT_T pcm_fmt;
} tA2DP_LDAC_ENCODER_PARAMS;

typedef struct {
  uint32_t counter;
  uint32_t bytes_per_tick; /* pcm bytes read each media task tick */
  uint64_t last_frame_us;
} tA2DP_LDAC_FEEDING_STATE;

typedef struct {
  uint64_t session_start_us;

  size_t media_read_total_expected_packets;
  size_t media_read_total_expected_reads_count;
  size_t media_read_total_expected_read_bytes;

  size_t media_read_total_dropped_packets;
  size_t media_read_total_actual_reads_count;
  size_t media_read_total_actual_read_bytes;
} a2dp_ldac_encoder_stats_t;

typedef struct {
  a2dp_source_read_callback_t read_callback;
  a2dp_source_enqueue_callback_t enqueue_callback;
  uint16_t TxAaMtuSize;
  size_t TxQueueLength;

  bool use_SCMS_T;
  bool is_peer_edr;          // True if the peer device supports EDR
  bool peer_supports_3mbps;  // True if the peer device supports 3Mbps EDR
  uint16_t peer_mtu;         // MTU of the A2DP peer
  uint32_t timestamp;        // Timestamp for the A2DP frames

  HANDLE_LDAC_BT ldac_handle;
  bool has_ldac_handle;  // True if ldac_handle is valid

  HANDLE_LDAC_ABR ldac_abr_handle;
  bool has_ldac_abr_handle;
  int last_ldac_abr_eqmid;
  size_t ldac_abr_adjustments;

  tA2DP_FEEDING_PARAMS feeding_params;
  tA2DP_LDAC_ENCODER_PARAMS ldac_encoder_params;
  tA2DP_LDAC_FEEDING_STATE ldac_feeding_state;

  a2dp_ldac_encoder_stats_t stats;
} tA2DP_LDAC_ENCODER_CB;

static bool ldac_abr_loaded = false;

static tA2DP_LDAC_ENCODER_CB a2dp_ldac_encoder_cb;

static void a2dp_vendor_ldac_encoder_update(uint16_t peer_mtu,
                                            A2dpCodecConfig* a2dp_codec_config,
                                            bool* p_restart_input,
                                            bool* p_restart_output,
                                            bool* p_config_updated);
static void a2dp_ldac_get_num_frame_iteration(uint8_t* num_of_iterations,
                                              uint8_t* num_of_frames,
                                              uint64_t timestamp_us);
static void a2dp_ldac_encode_frames(uint8_t nb_frame);
static bool a2dp_ldac_read_feeding(uint8_t* read_buffer, uint32_t* bytes_read);
static std::string quality_mode_index_to_name(int quality_mode_index);

static void* load_func(const char* func_name) {
  void* func_ptr = dlsym(ldac_encoder_lib_handle, func_name);
  if (func_ptr == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the encoder library: %s",
              __func__, func_name, dlerror());
    A2DP_VendorUnloadEncoderLdac();
    return NULL;
  }
  return func_ptr;
}

bool A2DP_VendorLoadEncoderLdac(void) {
  if (ldac_encoder_lib_handle != NULL) return true;  // Already loaded

  // Initialize the control block
  memset(&a2dp_ldac_encoder_cb, 0, sizeof(a2dp_ldac_encoder_cb));

  // Open the encoder library
  ldac_encoder_lib_handle = dlopen(LDAC_ENCODER_LIB_NAME, RTLD_NOW);
  if (ldac_encoder_lib_handle == NULL) {
    LOG_ERROR(LOG_TAG, "%s: cannot open LDAC encoder library %s: %s", __func__,
              LDAC_ENCODER_LIB_NAME, dlerror());
    return false;
  }

  // Load all functions
  ldac_get_handle_func = (tLDAC_GET_HANDLE)load_func(LDAC_GET_HANDLE_NAME);
  if (ldac_get_handle_func == NULL) return false;
  ldac_free_handle_func = (tLDAC_FREE_HANDLE)load_func(LDAC_FREE_HANDLE_NAME);
  if (ldac_free_handle_func == NULL) return false;
  ldac_close_handle_func =
      (tLDAC_CLOSE_HANDLE)load_func(LDAC_CLOSE_HANDLE_NAME);
  if (ldac_close_handle_func == NULL) return false;
  ldac_get_version_func = (tLDAC_GET_VERSION)load_func(LDAC_GET_VERSION_NAME);
  if (ldac_get_version_func == NULL) return false;
  ldac_get_bitrate_func = (tLDAC_GET_BITRATE)load_func(LDAC_GET_BITRATE_NAME);
  if (ldac_get_bitrate_func == NULL) return false;
  ldac_get_sampling_freq_func =
      (tLDAC_GET_SAMPLING_FREQ)load_func(LDAC_GET_SAMPLING_FREQ_NAME);
  if (ldac_get_sampling_freq_func == NULL) return false;
  ldac_init_handle_encode_func =
      (tLDAC_INIT_HANDLE_ENCODE)load_func(LDAC_INIT_HANDLE_ENCODE_NAME);
  if (ldac_init_handle_encode_func == NULL) return false;
  ldac_encode_func = (tLDAC_ENCODE)load_func(LDAC_ENCODE_NAME);
  if (ldac_encode_func == NULL) return false;
  ldac_set_eqmid_func = (tLDAC_SET_EQMID)load_func(LDAC_SET_EQMID_NAME);
  if (ldac_set_eqmid_func == NULL) return false;
  ldac_alter_eqmid_priority_func =
      (tLDAC_ALTER_EQMID_PRIORITY)load_func(LDAC_ALTER_EQMID_PRIORITY_NAME);
  if (ldac_alter_eqmid_priority_func == NULL) return false;
  ldac_get_eqmid_func = (tLDAC_GET_EQMID)load_func(LDAC_GET_EQMID_NAME);
  if (ldac_get_eqmid_func == NULL) return false;
  ldac_get_error_code_func =
      (tLDAC_GET_ERROR_CODE)load_func(LDAC_GET_ERROR_CODE_NAME);
  if (ldac_get_error_code_func == NULL) return false;

  if (!A2DP_VendorLoadLdacAbr()) {
    LOG_WARN(LOG_TAG, "%s: cannot load the LDAC ABR library", __func__);
    ldac_abr_loaded = false;
  } else {
    ldac_abr_loaded = true;
  }
  return true;
}

void A2DP_VendorUnloadEncoderLdac(void) {
  // Cleanup any LDAC-related state
  if (a2dp_ldac_encoder_cb.has_ldac_handle && ldac_free_handle_func != NULL)
    ldac_free_handle_func(a2dp_ldac_encoder_cb.ldac_handle);
  memset(&a2dp_ldac_encoder_cb, 0, sizeof(a2dp_ldac_encoder_cb));

  ldac_get_handle_func = NULL;
  ldac_free_handle_func = NULL;
  ldac_close_handle_func = NULL;
  ldac_get_version_func = NULL;
  ldac_get_bitrate_func = NULL;
  ldac_get_sampling_freq_func = NULL;
  ldac_init_handle_encode_func = NULL;
  ldac_encode_func = NULL;
  ldac_set_eqmid_func = NULL;
  ldac_alter_eqmid_priority_func = NULL;
  ldac_get_eqmid_func = NULL;
  ldac_get_error_code_func = NULL;

  if (ldac_encoder_lib_handle != NULL) {
    dlclose(ldac_encoder_lib_handle);
    ldac_encoder_lib_handle = NULL;
  }
}

void a2dp_vendor_ldac_encoder_init(
    const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
    A2dpCodecConfig* a2dp_codec_config,
    a2dp_source_read_callback_t read_callback,
    a2dp_source_enqueue_callback_t enqueue_callback) {
  if (a2dp_ldac_encoder_cb.has_ldac_handle)
    ldac_free_handle_func(a2dp_ldac_encoder_cb.ldac_handle);
  if (a2dp_ldac_encoder_cb.has_ldac_abr_handle)
    a2dp_ldac_abr_free_handle(a2dp_ldac_encoder_cb.ldac_abr_handle);
  memset(&a2dp_ldac_encoder_cb, 0, sizeof(a2dp_ldac_encoder_cb));

  a2dp_ldac_encoder_cb.stats.session_start_us =
      bluetooth::common::time_get_os_boottime_us();

  a2dp_ldac_encoder_cb.read_callback = read_callback;
  a2dp_ldac_encoder_cb.enqueue_callback = enqueue_callback;
  a2dp_ldac_encoder_cb.is_peer_edr = p_peer_params->is_peer_edr;
  a2dp_ldac_encoder_cb.peer_supports_3mbps = p_peer_params->peer_supports_3mbps;
  a2dp_ldac_encoder_cb.peer_mtu = p_peer_params->peer_mtu;
  a2dp_ldac_encoder_cb.timestamp = 0;
  a2dp_ldac_encoder_cb.ldac_abr_handle = NULL;
  a2dp_ldac_encoder_cb.has_ldac_abr_handle = false;
  a2dp_ldac_encoder_cb.last_ldac_abr_eqmid = -1;
  a2dp_ldac_encoder_cb.ldac_abr_adjustments = 0;

  a2dp_ldac_encoder_cb.use_SCMS_T = false;  // TODO: should be a parameter
#if (BTA_AV_CO_CP_SCMS_T == TRUE)
  a2dp_ldac_encoder_cb.use_SCMS_T = true;
#endif

  // NOTE: Ignore the restart_input / restart_output flags - this initization
  // happens when the connection is (re)started.
  bool restart_input = false;
  bool restart_output = false;
  bool config_updated = false;
  a2dp_vendor_ldac_encoder_update(a2dp_ldac_encoder_cb.peer_mtu,
                                  a2dp_codec_config, &restart_input,
                                  &restart_output, &config_updated);
}

bool A2dpCodecConfigLdacSource::updateEncoderUserConfig(
    const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params, bool* p_restart_input,
    bool* p_restart_output, bool* p_config_updated) {
  a2dp_ldac_encoder_cb.is_peer_edr = p_peer_params->is_peer_edr;
  a2dp_ldac_encoder_cb.peer_supports_3mbps = p_peer_params->peer_supports_3mbps;
  a2dp_ldac_encoder_cb.peer_mtu = p_peer_params->peer_mtu;
  a2dp_ldac_encoder_cb.timestamp = 0;

  if (a2dp_ldac_encoder_cb.peer_mtu == 0) {
    LOG_ERROR(LOG_TAG,
              "%s: Cannot update the codec encoder for %s: "
              "invalid peer MTU",
              __func__, name().c_str());
    return false;
  }

  a2dp_vendor_ldac_encoder_update(a2dp_ldac_encoder_cb.peer_mtu, this,
                                  p_restart_input, p_restart_output,
                                  p_config_updated);
  return true;
}

// Update the A2DP LDAC encoder.
// |peer_mtu| is the peer MTU.
// |a2dp_codec_config| is the A2DP codec to use for the update.
static void a2dp_vendor_ldac_encoder_update(uint16_t peer_mtu,
                                            A2dpCodecConfig* a2dp_codec_config,
                                            bool* p_restart_input,
                                            bool* p_restart_output,
                                            bool* p_config_updated) {
  tA2DP_LDAC_ENCODER_PARAMS* p_encoder_params =
      &a2dp_ldac_encoder_cb.ldac_encoder_params;
  uint8_t codec_info[AVDT_CODEC_SIZE];

  *p_restart_input = false;
  *p_restart_output = false;
  *p_config_updated = false;

  if (!a2dp_ldac_encoder_cb.has_ldac_handle) {
    a2dp_ldac_encoder_cb.ldac_handle = ldac_get_handle_func();
    if (a2dp_ldac_encoder_cb.ldac_handle == NULL) {
      LOG_ERROR(LOG_TAG, "%s: Cannot get LDAC encoder handle", __func__);
      return;  // TODO: Return an error?
    }
    a2dp_ldac_encoder_cb.has_ldac_handle = true;
  }
  CHECK(a2dp_ldac_encoder_cb.ldac_handle != nullptr);

  if (!a2dp_codec_config->copyOutOtaCodecConfig(codec_info)) {
    LOG_ERROR(LOG_TAG,
              "%s: Cannot update the codec encoder for %s: "
              "invalid codec config",
              __func__, a2dp_codec_config->name().c_str());
    return;
  }
  const uint8_t* p_codec_info = codec_info;
  btav_a2dp_codec_config_t codec_config = a2dp_codec_config->getCodecConfig();

  // The feeding parameters
  tA2DP_FEEDING_PARAMS* p_feeding_params = &a2dp_ldac_encoder_cb.feeding_params;
  p_feeding_params->sample_rate =
      A2DP_VendorGetTrackSampleRateLdac(p_codec_info);
  p_feeding_params->bits_per_sample =
      a2dp_codec_config->getAudioBitsPerSample();
  p_feeding_params->channel_count =
      A2DP_VendorGetTrackChannelCountLdac(p_codec_info);
  LOG_DEBUG(LOG_TAG, "%s: sample_rate=%u bits_per_sample=%u channel_count=%u",
            __func__, p_feeding_params->sample_rate,
            p_feeding_params->bits_per_sample, p_feeding_params->channel_count);
  a2dp_vendor_ldac_feeding_reset();

  // The codec parameters
  p_encoder_params->sample_rate =
      a2dp_ldac_encoder_cb.feeding_params.sample_rate;
  p_encoder_params->channel_mode =
      A2DP_VendorGetChannelModeCodeLdac(p_codec_info);

  uint16_t mtu_size =
      BT_DEFAULT_BUFFER_SIZE - A2DP_LDAC_OFFSET - sizeof(BT_HDR);
  if (mtu_size < peer_mtu) {
    a2dp_ldac_encoder_cb.TxAaMtuSize = mtu_size;
  } else {
    a2dp_ldac_encoder_cb.TxAaMtuSize = peer_mtu;
  }

  // Set the quality mode index
  int old_quality_mode_index = p_encoder_params->quality_mode_index;
  if (codec_config.codec_specific_1 != 0) {
    p_encoder_params->quality_mode_index = codec_config.codec_specific_1 % 10;
    LOG_DEBUG(LOG_TAG, "%s: setting quality mode to %s", __func__,
              quality_mode_index_to_name(p_encoder_params->quality_mode_index)
                  .c_str());
  } else {
    p_encoder_params->quality_mode_index = A2DP_LDAC_QUALITY_ABR;
    LOG_DEBUG(LOG_TAG, "%s: setting quality mode to default %s", __func__,
              quality_mode_index_to_name(p_encoder_params->quality_mode_index)
                  .c_str());
  }

  int ldac_eqmid = LDAC_ABR_MODE_EQMID;
  if (p_encoder_params->quality_mode_index == A2DP_LDAC_QUALITY_ABR) {
    if (!ldac_abr_loaded) {
      p_encoder_params->quality_mode_index = A2DP_LDAC_QUALITY_MID;
      LOG_WARN(
          LOG_TAG,
          "%s: LDAC ABR library is not loaded, resetting quality mode to %s",
          __func__,
          quality_mode_index_to_name(p_encoder_params->quality_mode_index)
              .c_str());
    } else {
      LOG_DEBUG(LOG_TAG, "%s: changing mode from %s to %s", __func__,
                quality_mode_index_to_name(old_quality_mode_index).c_str(),
                quality_mode_index_to_name(p_encoder_params->quality_mode_index)
                    .c_str());
      if (a2dp_ldac_encoder_cb.ldac_abr_handle != NULL) {
        LOG_DEBUG(LOG_TAG, "%s: already in LDAC ABR mode, do nothing.",
                  __func__);
      } else {
        LOG_DEBUG(LOG_TAG, "%s: get and init LDAC ABR handle.", __func__);
        a2dp_ldac_encoder_cb.ldac_abr_handle = a2dp_ldac_abr_get_handle();
        if (a2dp_ldac_encoder_cb.ldac_abr_handle != NULL) {
          a2dp_ldac_encoder_cb.has_ldac_abr_handle = true;
          a2dp_ldac_encoder_cb.last_ldac_abr_eqmid = -1;
          a2dp_ldac_encoder_cb.ldac_abr_adjustments = 0;
          a2dp_ldac_abr_init(a2dp_ldac_encoder_cb.ldac_abr_handle,
                             A2DP_LDAC_ENCODER_INTERVAL_MS);
        } else {
          p_encoder_params->quality_mode_index = A2DP_LDAC_QUALITY_MID;
          LOG_DEBUG(
              LOG_TAG,
              "%s: get LDAC ABR handle failed, resetting quality mode to %s.",
              __func__,
              quality_mode_index_to_name(p_encoder_params->quality_mode_index)
                  .c_str());
        }
      }
    }
  } else {
    ldac_eqmid = p_encoder_params->quality_mode_index;
    LOG_DEBUG(LOG_TAG, "%s: in %s mode, free LDAC ABR handle.", __func__,
              quality_mode_index_to_name(ldac_eqmid).c_str());
    if (a2dp_ldac_encoder_cb.has_ldac_abr_handle) {
      a2dp_ldac_abr_free_handle(a2dp_ldac_encoder_cb.ldac_abr_handle);
      a2dp_ldac_encoder_cb.ldac_abr_handle = NULL;
      a2dp_ldac_encoder_cb.has_ldac_abr_handle = false;
      a2dp_ldac_encoder_cb.last_ldac_abr_eqmid = -1;
      a2dp_ldac_encoder_cb.ldac_abr_adjustments = 0;
    }
  }

  if (p_encoder_params->quality_mode_index != old_quality_mode_index)
    *p_config_updated = true;

  p_encoder_params->pcm_wlength =
      a2dp_ldac_encoder_cb.feeding_params.bits_per_sample >> 3;
  // Set the Audio format from pcm_wlength
  p_encoder_params->pcm_fmt = LDACBT_SMPL_FMT_S16;
  if (p_encoder_params->pcm_wlength == 2)
    p_encoder_params->pcm_fmt = LDACBT_SMPL_FMT_S16;
  else if (p_encoder_params->pcm_wlength == 3)
    p_encoder_params->pcm_fmt = LDACBT_SMPL_FMT_S24;
  else if (p_encoder_params->pcm_wlength == 4)
    p_encoder_params->pcm_fmt = LDACBT_SMPL_FMT_S32;

  LOG_DEBUG(LOG_TAG, "%s: MTU=%d, peer_mtu=%d", __func__,
            a2dp_ldac_encoder_cb.TxAaMtuSize, peer_mtu);
  LOG_DEBUG(LOG_TAG,
            "%s: sample_rate: %d channel_mode: %d "
            "quality_mode_index: %d pcm_wlength: %d pcm_fmt: %d",
            __func__, p_encoder_params->sample_rate,
            p_encoder_params->channel_mode,
            p_encoder_params->quality_mode_index, p_encoder_params->pcm_wlength,
            p_encoder_params->pcm_fmt);

  // Initialize the encoder.
  // NOTE: MTU in the initialization must include the AVDT media header size.
  int result = ldac_init_handle_encode_func(
      a2dp_ldac_encoder_cb.ldac_handle,
      a2dp_ldac_encoder_cb.TxAaMtuSize + AVDT_MEDIA_HDR_SIZE, ldac_eqmid,
      p_encoder_params->channel_mode, p_encoder_params->pcm_fmt,
      p_encoder_params->sample_rate);
  if (result != 0) {
    int err_code = ldac_get_error_code_func(a2dp_ldac_encoder_cb.ldac_handle);
    LOG_ERROR(LOG_TAG,
              "%s: error initializing the LDAC encoder: %d api_error = %d "
              "handle_error = %d block_error = %d error_code = 0x%x",
              __func__, result, LDACBT_API_ERR(err_code),
              LDACBT_HANDLE_ERR(err_code), LDACBT_BLOCK_ERR(err_code),
              err_code);
  }
}

void a2dp_vendor_ldac_encoder_cleanup(void) {
  if (a2dp_ldac_encoder_cb.has_ldac_abr_handle)
    a2dp_ldac_abr_free_handle(a2dp_ldac_encoder_cb.ldac_abr_handle);
  if (a2dp_ldac_encoder_cb.has_ldac_handle)
    ldac_free_handle_func(a2dp_ldac_encoder_cb.ldac_handle);
  memset(&a2dp_ldac_encoder_cb, 0, sizeof(a2dp_ldac_encoder_cb));
}

void a2dp_vendor_ldac_feeding_reset(void) {
  /* By default, just clear the entire state */
  memset(&a2dp_ldac_encoder_cb.ldac_feeding_state, 0,
         sizeof(a2dp_ldac_encoder_cb.ldac_feeding_state));

  a2dp_ldac_encoder_cb.ldac_feeding_state.bytes_per_tick =
      (a2dp_ldac_encoder_cb.feeding_params.sample_rate *
       a2dp_ldac_encoder_cb.feeding_params.bits_per_sample / 8 *
       a2dp_ldac_encoder_cb.feeding_params.channel_count *
       A2DP_LDAC_ENCODER_INTERVAL_MS) /
      1000;

  LOG_DEBUG(LOG_TAG, "%s: PCM bytes per tick %u", __func__,
            a2dp_ldac_encoder_cb.ldac_feeding_state.bytes_per_tick);
}

void a2dp_vendor_ldac_feeding_flush(void) {
  a2dp_ldac_encoder_cb.ldac_feeding_state.counter = 0;
}

uint64_t a2dp_vendor_ldac_get_encoder_interval_ms(void) {
  return A2DP_LDAC_ENCODER_INTERVAL_MS;
}

void a2dp_vendor_ldac_send_frames(uint64_t timestamp_us) {
  uint8_t nb_frame = 0;
  uint8_t nb_iterations = 0;

  a2dp_ldac_get_num_frame_iteration(&nb_iterations, &nb_frame, timestamp_us);
  LOG_VERBOSE(LOG_TAG, "%s: Sending %d frames per iteration, %d iterations",
              __func__, nb_frame, nb_iterations);
  if (nb_frame == 0) return;

  for (uint8_t counter = 0; counter < nb_iterations; counter++) {
    if (a2dp_ldac_encoder_cb.has_ldac_abr_handle) {
      int flag_enable = 1;
      int prev_eqmid = a2dp_ldac_encoder_cb.last_ldac_abr_eqmid;
      a2dp_ldac_encoder_cb.last_ldac_abr_eqmid =
          a2dp_ldac_abr_proc(a2dp_ldac_encoder_cb.ldac_handle,
                             a2dp_ldac_encoder_cb.ldac_abr_handle,
                             a2dp_ldac_encoder_cb.TxQueueLength, flag_enable);
      if (prev_eqmid != a2dp_ldac_encoder_cb.last_ldac_abr_eqmid)
        a2dp_ldac_encoder_cb.ldac_abr_adjustments++;
#ifndef OS_GENERIC
      ATRACE_INT("LDAC ABR level", a2dp_ldac_encoder_cb.last_ldac_abr_eqmid);
#endif
    }
    // Transcode frame and enqueue
    a2dp_ldac_encode_frames(nb_frame);
  }
}

// Obtains the number of frames to send and number of iterations
// to be used. |num_of_iterations| and |num_of_frames| parameters
// are used as output param for returning the respective values.
static void a2dp_ldac_get_num_frame_iteration(uint8_t* num_of_iterations,
                                              uint8_t* num_of_frames,
                                              uint64_t timestamp_us) {
  uint32_t result = 0;
  uint8_t nof = 0;
  uint8_t noi = 1;

  uint32_t pcm_bytes_per_frame =
      A2DP_LDAC_MEDIA_BYTES_PER_FRAME *
      a2dp_ldac_encoder_cb.feeding_params.channel_count *
      a2dp_ldac_encoder_cb.feeding_params.bits_per_sample / 8;
  LOG_VERBOSE(LOG_TAG, "%s: pcm_bytes_per_frame %u", __func__,
              pcm_bytes_per_frame);

  uint32_t us_this_tick = A2DP_LDAC_ENCODER_INTERVAL_MS * 1000;
  uint64_t now_us = timestamp_us;
  if (a2dp_ldac_encoder_cb.ldac_feeding_state.last_frame_us != 0)
    us_this_tick =
        (now_us - a2dp_ldac_encoder_cb.ldac_feeding_state.last_frame_us);
  a2dp_ldac_encoder_cb.ldac_feeding_state.last_frame_us = now_us;

  a2dp_ldac_encoder_cb.ldac_feeding_state.counter +=
      a2dp_ldac_encoder_cb.ldac_feeding_state.bytes_per_tick * us_this_tick /
      (A2DP_LDAC_ENCODER_INTERVAL_MS * 1000);

  result =
      a2dp_ldac_encoder_cb.ldac_feeding_state.counter / pcm_bytes_per_frame;
  a2dp_ldac_encoder_cb.ldac_feeding_state.counter -=
      result * pcm_bytes_per_frame;
  nof = result;

  LOG_VERBOSE(LOG_TAG, "%s: effective num of frames %u, iterations %u",
              __func__, nof, noi);

  *num_of_frames = nof;
  *num_of_iterations = noi;
}

static void a2dp_ldac_encode_frames(uint8_t nb_frame) {
  tA2DP_LDAC_ENCODER_PARAMS* p_encoder_params =
      &a2dp_ldac_encoder_cb.ldac_encoder_params;
  uint8_t remain_nb_frame = nb_frame;
  uint16_t ldac_frame_size;
  uint8_t read_buffer[LDACBT_MAX_LSU * 4 /* byte/sample */ * 2 /* ch */];

  switch (p_encoder_params->sample_rate) {
    case 176400:
    case 192000:
      ldac_frame_size = 512;  // sample/ch
      break;
    case 88200:
    case 96000:
      ldac_frame_size = 256;  // sample/ch
      break;
    case 44100:
    case 48000:
    default:
      ldac_frame_size = 128;  // sample/ch
      break;
  }

  uint32_t count;
  int32_t encode_count = 0;
  int32_t out_frames = 0;
  int written = 0;

  uint32_t bytes_read = 0;
  while (nb_frame) {
    BT_HDR* p_buf = (BT_HDR*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);
    p_buf->offset = A2DP_LDAC_OFFSET;
    p_buf->len = 0;
    p_buf->layer_specific = 0;
    a2dp_ldac_encoder_cb.stats.media_read_total_expected_packets++;

    count = 0;
    do {
      //
      // Read the PCM data and encode it
      //
      uint32_t temp_bytes_read = 0;
      if (a2dp_ldac_read_feeding(read_buffer, &temp_bytes_read)) {
        bytes_read += temp_bytes_read;
        uint8_t* packet = (uint8_t*)(p_buf + 1) + p_buf->offset + p_buf->len;
        if (a2dp_ldac_encoder_cb.ldac_handle == NULL) {
          LOG_ERROR(LOG_TAG, "%s: invalid LDAC handle", __func__);
          a2dp_ldac_encoder_cb.stats.media_read_total_dropped_packets++;
          osi_free(p_buf);
          return;
        }
        int result = ldac_encode_func(
            a2dp_ldac_encoder_cb.ldac_handle, read_buffer, (int*)&encode_count,
            packet + count, (int*)&written, (int*)&out_frames);
        if (result != 0) {
          int err_code =
              ldac_get_error_code_func(a2dp_ldac_encoder_cb.ldac_handle);
          LOG_ERROR(LOG_TAG,
                    "%s: LDAC encoding error: %d api_error = %d "
                    "handle_error = %d block_error = %d error_code = 0x%x",
                    __func__, result, LDACBT_API_ERR(err_code),
                    LDACBT_HANDLE_ERR(err_code), LDACBT_BLOCK_ERR(err_code),
                    err_code);
          a2dp_ldac_encoder_cb.stats.media_read_total_dropped_packets++;
          osi_free(p_buf);
          return;
        }
        count += written;
        p_buf->len += written;
        nb_frame--;
        p_buf->layer_specific += out_frames;  // added a frame to the buffer
      } else {
        LOG_WARN(LOG_TAG, "%s: underflow %d", __func__, nb_frame);
        a2dp_ldac_encoder_cb.ldac_feeding_state.counter +=
            nb_frame * LDACBT_ENC_LSU *
            a2dp_ldac_encoder_cb.feeding_params.channel_count *
            a2dp_ldac_encoder_cb.feeding_params.bits_per_sample / 8;

        // no more pcm to read
        nb_frame = 0;
      }
    } while ((written == 0) && nb_frame);

    if (p_buf->len) {
      /*
       * Timestamp of the media packet header represent the TS of the
       * first frame, i.e the timestamp before including this frame.
       */
      *((uint32_t*)(p_buf + 1)) = a2dp_ldac_encoder_cb.timestamp;

      a2dp_ldac_encoder_cb.timestamp += p_buf->layer_specific * ldac_frame_size;

      uint8_t done_nb_frame = remain_nb_frame - nb_frame;
      remain_nb_frame = nb_frame;
      if (!a2dp_ldac_encoder_cb.enqueue_callback(p_buf, done_nb_frame,
                                                 bytes_read))
        return;
    } else {
      // NOTE: Unlike the execution path for other codecs, it is normal for
      // LDAC to NOT write encoded data to the last buffer if there wasn't
      // enough data to write to. That data is accumulated internally by
      // the codec and included in the next iteration. Therefore, here we
      // don't increment the "media_read_total_dropped_packets" counter.
      osi_free(p_buf);
    }
  }
}

static bool a2dp_ldac_read_feeding(uint8_t* read_buffer, uint32_t* bytes_read) {
  uint32_t read_size = LDACBT_ENC_LSU *
                       a2dp_ldac_encoder_cb.feeding_params.channel_count *
                       a2dp_ldac_encoder_cb.feeding_params.bits_per_sample / 8;

  a2dp_ldac_encoder_cb.stats.media_read_total_expected_reads_count++;
  a2dp_ldac_encoder_cb.stats.media_read_total_expected_read_bytes += read_size;

  /* Read Data from UIPC channel */
  uint32_t nb_byte_read =
      a2dp_ldac_encoder_cb.read_callback(read_buffer, read_size);
  a2dp_ldac_encoder_cb.stats.media_read_total_actual_read_bytes += nb_byte_read;

  if (nb_byte_read < read_size) {
    if (nb_byte_read == 0) return false;

    /* Fill the unfilled part of the read buffer with silence (0) */
    memset(((uint8_t*)read_buffer) + nb_byte_read, 0, read_size - nb_byte_read);
    nb_byte_read = read_size;
  }
  a2dp_ldac_encoder_cb.stats.media_read_total_actual_reads_count++;

  *bytes_read = nb_byte_read;
  return true;
}

static std::string quality_mode_index_to_name(int quality_mode_index) {
  switch (quality_mode_index) {
    case A2DP_LDAC_QUALITY_HIGH:
      return "HIGH";
    case A2DP_LDAC_QUALITY_MID:
      return "MID";
    case A2DP_LDAC_QUALITY_LOW:
      return "LOW";
    case A2DP_LDAC_QUALITY_ABR:
      return "ABR";
    default:
      return "Unknown";
  }
}

void a2dp_vendor_ldac_set_transmit_queue_length(size_t transmit_queue_length) {
  a2dp_ldac_encoder_cb.TxQueueLength = transmit_queue_length;
}

uint64_t A2dpCodecConfigLdacSource::encoderIntervalMs() const {
  return a2dp_vendor_ldac_get_encoder_interval_ms();
}

int A2dpCodecConfigLdacSource::getEffectiveMtu() const {
  return a2dp_ldac_encoder_cb.TxAaMtuSize;
}

void A2dpCodecConfigLdacSource::debug_codec_dump(int fd) {
  a2dp_ldac_encoder_stats_t* stats = &a2dp_ldac_encoder_cb.stats;
  tA2DP_LDAC_ENCODER_PARAMS* p_encoder_params =
      &a2dp_ldac_encoder_cb.ldac_encoder_params;

  A2dpCodecConfig::debug_codec_dump(fd);

  dprintf(fd,
          "  Packet counts (expected/dropped)                        : %zu / "
          "%zu\n",
          stats->media_read_total_expected_packets,
          stats->media_read_total_dropped_packets);

  dprintf(fd,
          "  PCM read counts (expected/actual)                       : %zu / "
          "%zu\n",
          stats->media_read_total_expected_reads_count,
          stats->media_read_total_actual_reads_count);

  dprintf(fd,
          "  PCM read bytes (expected/actual)                        : %zu / "
          "%zu\n",
          stats->media_read_total_expected_read_bytes,
          stats->media_read_total_actual_read_bytes);

  dprintf(
      fd, "  LDAC quality mode                                       : %s\n",
      quality_mode_index_to_name(p_encoder_params->quality_mode_index).c_str());

  dprintf(fd,
          "  LDAC transmission bitrate (Kbps)                        : %d\n",
          ldac_get_bitrate_func(a2dp_ldac_encoder_cb.ldac_handle));

  dprintf(fd,
          "  LDAC saved transmit queue length                        : %zu\n",
          a2dp_ldac_encoder_cb.TxQueueLength);
  if (a2dp_ldac_encoder_cb.has_ldac_abr_handle) {
    dprintf(fd,
            "  LDAC adaptive bit rate encode quality mode index        : %d\n",
            a2dp_ldac_encoder_cb.last_ldac_abr_eqmid);
    dprintf(fd,
            "  LDAC adaptive bit rate adjustments                      : %zu\n",
            a2dp_ldac_encoder_cb.ldac_abr_adjustments);
  }
}
