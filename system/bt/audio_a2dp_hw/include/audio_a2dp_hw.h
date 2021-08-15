/******************************************************************************
 *
 *  Copyright 2009-2012 Broadcom Corporation
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

/*****************************************************************************
 *
 *  Filename:      audio_a2dp_hw.h
 *
 *  Description:
 *
 *****************************************************************************/

#ifndef AUDIO_A2DP_HW_H
#define AUDIO_A2DP_HW_H

#include <stdint.h>

#include <hardware/bt_av.h>

/*****************************************************************************
 *  Constants & Macros
 *****************************************************************************/

#define A2DP_AUDIO_HARDWARE_INTERFACE "audio.a2dp"
#define A2DP_CTRL_PATH "/data/misc/bluedroid/.a2dp_ctrl"
#define A2DP_DATA_PATH "/data/misc/bluedroid/.a2dp_data"

// AUDIO_STREAM_OUTPUT_BUFFER_SZ controls the size of the audio socket buffer.
// If one assumes the write buffer is always full during normal BT playback,
// then increasing this value increases our playback latency.
//
// FIXME: The BT HAL should consume data at a constant rate.
// AudioFlinger assumes that the HAL draws data at a constant rate, which is
// true for most audio devices; however, the BT engine reads data at a variable
// rate (over the short term), which confuses both AudioFlinger as well as
// applications which deliver data at a (generally) fixed rate.
//
// 20 * 512 is not sufficient to smooth the variability for some BT devices,
// resulting in mixer sleep and throttling. We increase this to 28 * 512 to help
// reduce the effect of variable data consumption.
#define AUDIO_STREAM_OUTPUT_BUFFER_SZ (28 * 512)
#define AUDIO_STREAM_CONTROL_OUTPUT_BUFFER_SZ 256

// AUDIO_STREAM_OUTPUT_BUFFER_PERIODS controls how the socket buffer is divided
// for AudioFlinger data delivery. The AudioFlinger mixer delivers data in
// chunks of AUDIO_STREAM_OUTPUT_BUFFER_SZ / AUDIO_STREAM_OUTPUT_BUFFER_PERIODS.
// If the number of periods is 2, the socket buffer represents "double
// buffering" of the AudioFlinger mixer buffer.
//
// In general, AUDIO_STREAM_OUTPUT_BUFFER_PERIODS * 16 * 4 should be a divisor
// of AUDIO_STREAM_OUTPUT_BUFFER_SZ.
//
// These values should be chosen such that
//
// AUDIO_STREAM_BUFFER_SIZE * 1000 / (AUDIO_STREAM_OUTPUT_BUFFER_PERIODS
//         * AUDIO_STREAM_DEFAULT_RATE * 4) > 20 (ms)
//
// to avoid introducing the FastMixer in AudioFlinger. Using the FastMixer
// results in unnecessary latency and CPU overhead for Bluetooth.
#define AUDIO_STREAM_OUTPUT_BUFFER_PERIODS 2

#define AUDIO_SKT_DISCONNECTED (-1)

typedef enum {
  A2DP_CTRL_CMD_NONE,
  A2DP_CTRL_CMD_CHECK_READY,
  A2DP_CTRL_CMD_START,
  A2DP_CTRL_CMD_STOP,
  A2DP_CTRL_CMD_SUSPEND,
  A2DP_CTRL_GET_INPUT_AUDIO_CONFIG,
  A2DP_CTRL_GET_OUTPUT_AUDIO_CONFIG,
  A2DP_CTRL_SET_OUTPUT_AUDIO_CONFIG,
  A2DP_CTRL_CMD_OFFLOAD_START,
  A2DP_CTRL_GET_PRESENTATION_POSITION,
} tA2DP_CTRL_CMD;

typedef enum {
  A2DP_CTRL_ACK_SUCCESS,
  A2DP_CTRL_ACK_FAILURE,
  A2DP_CTRL_ACK_INCALL_FAILURE, /* Failure when in Call*/
  A2DP_CTRL_ACK_UNSUPPORTED,
  A2DP_CTRL_ACK_PENDING,
  A2DP_CTRL_ACK_DISCONNECT_IN_PROGRESS,
} tA2DP_CTRL_ACK;

typedef uint32_t tA2DP_SAMPLE_RATE;
typedef uint8_t tA2DP_CHANNEL_COUNT;
typedef uint8_t tA2DP_BITS_PER_SAMPLE;

/*****************************************************************************
 *  Type definitions for callback functions
 *****************************************************************************/

/*****************************************************************************
 *  Type definitions and return values
 *****************************************************************************/

/*****************************************************************************
 *  Extern variables and functions
 *****************************************************************************/

/*****************************************************************************
 *  Functions
 *****************************************************************************/

// Computes the Audio A2DP HAL output buffer size.
// |codec_sample_rate| is the sample rate of the output stream.
// |codec_bits_per_sample| is the number of bits per sample of the output
// stream.
// |codec_channel_mode| is the channel mode of the output stream.
//
// The buffer size is computed by using the following formula:
//
// AUDIO_STREAM_OUTPUT_BUFFER_SIZE =
//    (TIME_PERIOD_MS * AUDIO_STREAM_OUTPUT_BUFFER_PERIODS *
//     SAMPLE_RATE_HZ * NUMBER_OF_CHANNELS * (BITS_PER_SAMPLE / 8)) / 1000
//
// AUDIO_STREAM_OUTPUT_BUFFER_PERIODS controls how the socket buffer is
// divided for AudioFlinger data delivery. The AudioFlinger mixer delivers
// data in chunks of
// (AUDIO_STREAM_OUTPUT_BUFFER_SIZE / AUDIO_STREAM_OUTPUT_BUFFER_PERIODS) .
// If the number of periods is 2, the socket buffer represents "double
// buffering" of the AudioFlinger mixer buffer.
//
// Furthermore, the AudioFlinger expects the buffer size to be a multiple
// of 16 frames.
//
// NOTE: Currently, the computation uses the conservative 20ms time period.
//
// Returns the computed buffer size. If any of the input parameters is
// invalid, the return value is the default |AUDIO_STREAM_OUTPUT_BUFFER_SZ|.
size_t audio_a2dp_hw_stream_compute_buffer_size(
    btav_a2dp_codec_sample_rate_t codec_sample_rate,
    btav_a2dp_codec_bits_per_sample_t codec_bits_per_sample,
    btav_a2dp_codec_channel_mode_t codec_channel_mode);

// Returns whether the delay reporting property is set.
bool delay_reporting_enabled();

// Returns a string representation of |event|.
const char* audio_a2dp_hw_dump_ctrl_event(tA2DP_CTRL_CMD event);

#endif /* A2DP_AUDIO_HW_H */
