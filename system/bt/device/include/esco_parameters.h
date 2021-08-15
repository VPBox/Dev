/******************************************************************************
 *
 *  Copyright 2015 Broadcom Corporation
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

#pragma once

#include <stdint.h>

/*******************
 * SCO Codec Types
 *******************/
typedef enum {
  SCO_CODEC_NONE = 0x0000,
  SCO_CODEC_CVSD = 0x0001,
  SCO_CODEC_MSBC = 0x0002,
} sco_codec_t;

typedef enum {
  ESCO_CODEC_CVSD = 0,
  ESCO_CODEC_MSBC_T1,
  ESCO_CODEC_MSBC_T2,
} esco_codec_t;

#define ESCO_NUM_CODECS 3

// Coding Formats (BT 4.1 or later Assigned numbers)
#define ESCO_CODING_FORMAT_ULAW ((uint8_t)0x00)     /* u-Law log    */
#define ESCO_CODING_FORMAT_ALAW ((uint8_t)0x01)     /* A-Law log    */
#define ESCO_CODING_FORMAT_CVSD ((uint8_t)0x02)     /* CVSD         */
#define ESCO_CODING_FORMAT_TRANSPNT ((uint8_t)0x03) /* Transparent  */
#define ESCO_CODING_FORMAT_LINEAR ((uint8_t)0x04)   /* Linear PCM   */
#define ESCO_CODING_FORMAT_MSBC ((uint8_t)0x05)     /* MSBC PCM   */
#define ESCO_CODING_FORMAT_VS ((uint8_t)0xFF)       /* Specifies VSC used */
typedef uint8_t esco_coding_format_t;

// PCM Data Formats (BT 4.1 or later Assigned numbers)
#define ESCO_PCM_DATA_FORMAT_NA \
  ((uint8_t)0x00) /* N/A to coding format in use */
#define ESCO_PCM_DATA_FORMAT_1_COMP ((uint8_t)0x01) /* 1's complement   */
#define ESCO_PCM_DATA_FORMAT_2_COMP ((uint8_t)0x02) /* 2's complement   */
#define ESCO_PCM_DATA_FORMAT_SIGN ((uint8_t)0x03)   /* Sign-magnitude   */
#define ESCO_PCM_DATA_FORMAT_UNSIGN ((uint8_t)0x04) /* Unsigned         */
typedef uint8_t esco_pcm_data_format_t;

// SCO Data Path
#define ESCO_DATA_PATH_PCM 1                /* 0x01-0xFE (PCM Chan) */
#define ESCO_DATA_PATH_HCI ((uint8_t)0x00)  /* HCI-0, 0x01-0xFE (PCM Chan) */
#define ESCO_DATA_PATH_TEST ((uint8_t)0xFF) /* 0xFF-Audio Test */
typedef uint8_t esco_data_path_t;

// eSCO constants
#define TXRX_64KBITS_RATE 0x00001f40  /* 64 kbits/sec data rate */
#define TXRX_128KBITS_RATE 0x00003E80 /* 128 kbits/sec data rate */
typedef uint32_t esco_txrx_bandwidth_t;

#define INPUT_OUTPUT_64K_RATE 0x00003E80  /* 16000 Bytes/sec over transport */
#define INPUT_OUTPUT_128K_RATE 0x00007D00 /* 32000 Bytes/sec over transport */
typedef uint32_t esco_io_bandwidth_t;

// Retransmission effort
#define ESCO_RETRANSMISSION_OFF 0
#define ESCO_RETRANSMISSION_POWER 1
#define ESCO_RETRANSMISSION_QUALITY 2
#define ESCO_RETRANSMISSION_DONTCARE 0xff
typedef uint8_t esco_retransmission_effort_t;

// Definitions for eSCO packet type masks (BT1.2 and BT2.0 definitions)
#define ESCO_PKT_TYPES_MASK_HV1 0x0001
#define ESCO_PKT_TYPES_MASK_HV2 0x0002
#define ESCO_PKT_TYPES_MASK_HV3 0x0004
#define ESCO_PKT_TYPES_MASK_EV3 0x0008
#define ESCO_PKT_TYPES_MASK_EV4 0x0010
#define ESCO_PKT_TYPES_MASK_EV5 0x0020
#define ESCO_PKT_TYPES_MASK_NO_2_EV3 0x0040
#define ESCO_PKT_TYPES_MASK_NO_3_EV3 0x0080
#define ESCO_PKT_TYPES_MASK_NO_2_EV5 0x0100
#define ESCO_PKT_TYPES_MASK_NO_3_EV5 0x0200
typedef uint16_t esco_packet_types_t;

// type definition
typedef struct {
  esco_coding_format_t coding_format; /* Coding Format*/
  uint16_t company_id; /* Company ID from BT SIG Assigned Numbers */
  uint16_t vendor_specific_codec_id; /* Vendor Specific Codec ID */
} esco_coding_id_format_t;

// Enhanced setup/accept synchronous connection See BT 4.1 or later HCI spec for
// details
typedef struct {
  esco_txrx_bandwidth_t
      transmit_bandwidth; /* Transmit Bandwidth (in octets/second) */
  esco_txrx_bandwidth_t receive_bandwidth; /* RX BW (# of octets/second) */
  esco_coding_id_format_t transmit_coding_format; /* TX coding format */
  esco_coding_id_format_t receive_coding_format;  /* RX coding format */
  uint16_t transmit_codec_frame_size; /* TX CODEC frame size (OTA frame size) */
  uint16_t receive_codec_frame_size;  /* RX CODEC frame size (OTA frame size) */
  esco_io_bandwidth_t input_bandwidth; /* Input BW (nominal rate octets/sec) */
  esco_io_bandwidth_t
      output_bandwidth; /* Output BW (nominal rate octets/sec) */
  esco_coding_id_format_t input_coding_format;  /* Input coding format */
  esco_coding_id_format_t output_coding_format; /* Output coding format */
  uint16_t input_coded_data_size;  /* Input coded data size (in bits) */
  uint16_t output_coded_data_size; /* Output coded data size (in bits) */
  esco_pcm_data_format_t
      input_pcm_data_format; /* Input PCM data format (see hcidefs.h) */
  esco_pcm_data_format_t
      output_pcm_data_format; /* Output PCM data format (see hcidefs.h) */
  uint8_t input_pcm_payload_msb_position;  /* Input PCM sample payload MSB
                                              position */
  uint8_t output_pcm_payload_msb_position; /* Output PCM sample payload MSB
                                              position */
  esco_data_path_t input_data_path;   /* 0x00 - HCI, or 0x01-0xFE for VS) */
  esco_data_path_t output_data_path;  /* 0x00 - HCI, or 0x01-0xFE for VS) */
  uint8_t input_transport_unit_size;  /* Input transport unit size */
  uint8_t output_transport_unit_size; /* Output transport unit size */
  uint16_t max_latency_ms;          /* Maximum latency (0x4-0xFFFE in msecs) */
  esco_packet_types_t packet_types; /* Packet Types */
  esco_retransmission_effort_t
      retransmission_effort; /* 0x00-0x02, 0xFF don't care */
} enh_esco_params_t;

// Get the enhanced eSCO configuration parameters for the provided |codec|
enh_esco_params_t esco_parameters_for_codec(esco_codec_t codec);
