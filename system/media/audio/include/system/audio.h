/*
 * Copyright (C) 2011 The Android Open Source Project
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


#ifndef ANDROID_AUDIO_CORE_H
#define ANDROID_AUDIO_CORE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <cutils/bitops.h>

#include "audio-base.h"
#include "audio-base-utils.h"

/*
 * Annotation to tell clang that we intend to fall through from one case to
 * another in a switch (for c++ files). Sourced from android-base/macros.h.
 */
#ifndef FALLTHROUGH_INTENDED
#ifdef __cplusplus
#define FALLTHROUGH_INTENDED [[fallthrough]]
#else
#define FALLTHROUGH_INTENDED
#endif // __cplusplus
#endif // FALLTHROUGH_INTENDED

__BEGIN_DECLS

/* The enums were moved here mostly from
 * frameworks/base/include/media/AudioSystem.h
 */

/* represents an invalid uid for tracks; the calling or client uid is often substituted. */
#define AUDIO_UID_INVALID ((uid_t)-1)

/* device address used to refer to the standard remote submix */
#define AUDIO_REMOTE_SUBMIX_DEVICE_ADDRESS "0"

/* AudioFlinger and AudioPolicy services use I/O handles to identify audio sources and sinks */
typedef int audio_io_handle_t;

typedef uint32_t audio_flags_mask_t;

/* Do not change these values without updating their counterparts
 * in frameworks/base/media/java/android/media/AudioAttributes.java
 */
enum {
    AUDIO_FLAG_NONE                       = 0x0,
    AUDIO_FLAG_AUDIBILITY_ENFORCED        = 0x1,
    AUDIO_FLAG_SECURE                     = 0x2,
    AUDIO_FLAG_SCO                        = 0x4,
    AUDIO_FLAG_BEACON                     = 0x8,
    AUDIO_FLAG_HW_AV_SYNC                 = 0x10,
    AUDIO_FLAG_HW_HOTWORD                 = 0x20,
    AUDIO_FLAG_BYPASS_INTERRUPTION_POLICY = 0x40,
    AUDIO_FLAG_BYPASS_MUTE                = 0x80,
    AUDIO_FLAG_LOW_LATENCY                = 0x100,
    AUDIO_FLAG_DEEP_BUFFER                = 0x200,
    AUDIO_FLAG_NO_MEDIA_PROJECTION        = 0X400,
    AUDIO_FLAG_MUTE_HAPTIC                = 0x800,
    AUDIO_FLAG_NO_SYSTEM_CAPTURE          = 0X1000,
};

/* Audio attributes */
#define AUDIO_ATTRIBUTES_TAGS_MAX_SIZE 256
typedef struct {
    audio_content_type_t content_type;
    audio_usage_t        usage;
    audio_source_t       source;
    audio_flags_mask_t   flags;
    char                 tags[AUDIO_ATTRIBUTES_TAGS_MAX_SIZE]; /* UTF8 */
} __attribute__((packed)) audio_attributes_t; // sent through Binder;

static const audio_attributes_t AUDIO_ATTRIBUTES_INITIALIZER = {
    /* .content_type = */ AUDIO_CONTENT_TYPE_UNKNOWN,
    /* .usage = */ AUDIO_USAGE_UNKNOWN,
    /* .source = */ AUDIO_SOURCE_DEFAULT,
    /* .flags = */ AUDIO_FLAG_NONE,
    /* .tags = */ ""
};

static inline audio_attributes_t attributes_initializer(audio_usage_t usage)
{
    audio_attributes_t attributes = AUDIO_ATTRIBUTES_INITIALIZER;
    attributes.usage = usage;
    return attributes;
}

static inline void audio_flags_to_audio_output_flags(
                                           const audio_flags_mask_t audio_flags,
                                           audio_output_flags_t *flags)
{
    if ((audio_flags & AUDIO_FLAG_HW_AV_SYNC) != 0) {
        *flags = (audio_output_flags_t)(*flags |
            AUDIO_OUTPUT_FLAG_HW_AV_SYNC | AUDIO_OUTPUT_FLAG_DIRECT);
    }
    if ((audio_flags & AUDIO_FLAG_LOW_LATENCY) != 0) {
        *flags = (audio_output_flags_t)(*flags | AUDIO_OUTPUT_FLAG_FAST);
    }
    // check deep buffer after flags have been modified above
    if (*flags == AUDIO_OUTPUT_FLAG_NONE && (audio_flags & AUDIO_FLAG_DEEP_BUFFER) != 0) {
        *flags = AUDIO_OUTPUT_FLAG_DEEP_BUFFER;
    }
}


/* a unique ID allocated by AudioFlinger for use as an audio_io_handle_t, audio_session_t,
 * effect ID (int), audio_module_handle_t, and audio_patch_handle_t.
 * Audio port IDs (audio_port_handle_t) are allocated by AudioPolicy
 * in a different namespace than AudioFlinger unique IDs.
 */
typedef int audio_unique_id_t;

/* Possible uses for an audio_unique_id_t */
typedef enum {
    AUDIO_UNIQUE_ID_USE_UNSPECIFIED = 0,
    AUDIO_UNIQUE_ID_USE_SESSION = 1,    // for allocated sessions, not special AUDIO_SESSION_*
    AUDIO_UNIQUE_ID_USE_MODULE = 2,
    AUDIO_UNIQUE_ID_USE_EFFECT = 3,
    AUDIO_UNIQUE_ID_USE_PATCH = 4,
    AUDIO_UNIQUE_ID_USE_OUTPUT = 5,
    AUDIO_UNIQUE_ID_USE_INPUT = 6,
    AUDIO_UNIQUE_ID_USE_CLIENT = 7,  // client-side players and recorders
    AUDIO_UNIQUE_ID_USE_MAX = 8,  // must be a power-of-two
    AUDIO_UNIQUE_ID_USE_MASK = AUDIO_UNIQUE_ID_USE_MAX - 1
} audio_unique_id_use_t;

/* Return the use of an audio_unique_id_t */
static inline audio_unique_id_use_t audio_unique_id_get_use(audio_unique_id_t id)
{
    return (audio_unique_id_use_t) (id & AUDIO_UNIQUE_ID_USE_MASK);
}

/* Reserved audio_unique_id_t values.  FIXME: not a complete list. */
#define AUDIO_UNIQUE_ID_ALLOCATE AUDIO_SESSION_ALLOCATE

/* A channel mask per se only defines the presence or absence of a channel, not the order.
 * But see AUDIO_INTERLEAVE_* below for the platform convention of order.
 *
 * audio_channel_mask_t is an opaque type and its internal layout should not
 * be assumed as it may change in the future.
 * Instead, always use the functions declared in this header to examine.
 *
 * These are the current representations:
 *
 *   AUDIO_CHANNEL_REPRESENTATION_POSITION
 *     is a channel mask representation for position assignment.
 *     Each low-order bit corresponds to the spatial position of a transducer (output),
 *     or interpretation of channel (input).
 *     The user of a channel mask needs to know the context of whether it is for output or input.
 *     The constants AUDIO_CHANNEL_OUT_* or AUDIO_CHANNEL_IN_* apply to the bits portion.
 *     It is not permitted for no bits to be set.
 *
 *   AUDIO_CHANNEL_REPRESENTATION_INDEX
 *     is a channel mask representation for index assignment.
 *     Each low-order bit corresponds to a selected channel.
 *     There is no platform interpretation of the various bits.
 *     There is no concept of output or input.
 *     It is not permitted for no bits to be set.
 *
 * All other representations are reserved for future use.
 *
 * Warning: current representation distinguishes between input and output, but this will not the be
 * case in future revisions of the platform. Wherever there is an ambiguity between input and output
 * that is currently resolved by checking the channel mask, the implementer should look for ways to
 * fix it with additional information outside of the mask.
 */
typedef uint32_t audio_channel_mask_t;

/* log(2) of maximum number of representations, not part of public API */
#define AUDIO_CHANNEL_REPRESENTATION_LOG2   2

/* The return value is undefined if the channel mask is invalid. */
static inline uint32_t audio_channel_mask_get_bits(audio_channel_mask_t channel)
{
    return channel & ((1 << AUDIO_CHANNEL_COUNT_MAX) - 1);
}

typedef uint32_t audio_channel_representation_t;

/* The return value is undefined if the channel mask is invalid. */
static inline audio_channel_representation_t audio_channel_mask_get_representation(
        audio_channel_mask_t channel)
{
    // The right shift should be sufficient, but also "and" for safety in case mask is not 32 bits
    return (audio_channel_representation_t)
            ((channel >> AUDIO_CHANNEL_COUNT_MAX) & ((1 << AUDIO_CHANNEL_REPRESENTATION_LOG2) - 1));
}

/* Returns true if the channel mask is valid,
 * or returns false for AUDIO_CHANNEL_NONE, AUDIO_CHANNEL_INVALID, and other invalid values.
 * This function is unable to determine whether a channel mask for position assignment
 * is invalid because an output mask has an invalid output bit set,
 * or because an input mask has an invalid input bit set.
 * All other APIs that take a channel mask assume that it is valid.
 */
static inline bool audio_channel_mask_is_valid(audio_channel_mask_t channel)
{
    uint32_t bits = audio_channel_mask_get_bits(channel);
    audio_channel_representation_t representation = audio_channel_mask_get_representation(channel);
    switch (representation) {
    case AUDIO_CHANNEL_REPRESENTATION_POSITION:
    case AUDIO_CHANNEL_REPRESENTATION_INDEX:
        break;
    default:
        bits = 0;
        break;
    }
    return bits != 0;
}

/* Not part of public API */
static inline audio_channel_mask_t audio_channel_mask_from_representation_and_bits(
        audio_channel_representation_t representation, uint32_t bits)
{
    return (audio_channel_mask_t) ((representation << AUDIO_CHANNEL_COUNT_MAX) | bits);
}

/**
 * Expresses the convention when stereo audio samples are stored interleaved
 * in an array.  This should improve readability by allowing code to use
 * symbolic indices instead of hard-coded [0] and [1].
 *
 * For multi-channel beyond stereo, the platform convention is that channels
 * are interleaved in order from least significant channel mask bit to most
 * significant channel mask bit, with unused bits skipped.  Any exceptions
 * to this convention will be noted at the appropriate API.
 */
enum {
    AUDIO_INTERLEAVE_LEFT = 0,
    AUDIO_INTERLEAVE_RIGHT = 1,
};

/* This enum is deprecated */
typedef enum {
    AUDIO_IN_ACOUSTICS_NONE          = 0,
    AUDIO_IN_ACOUSTICS_AGC_ENABLE    = 0x0001,
    AUDIO_IN_ACOUSTICS_AGC_DISABLE   = 0,
    AUDIO_IN_ACOUSTICS_NS_ENABLE     = 0x0002,
    AUDIO_IN_ACOUSTICS_NS_DISABLE    = 0,
    AUDIO_IN_ACOUSTICS_TX_IIR_ENABLE = 0x0004,
    AUDIO_IN_ACOUSTICS_TX_DISABLE    = 0,
} audio_in_acoustics_t;

typedef uint32_t audio_devices_t;
/**
 * Stub audio output device. Used in policy configuration file on platforms without audio outputs.
 * This alias value to AUDIO_DEVICE_OUT_DEFAULT is only used in the audio policy context.
 */
#define AUDIO_DEVICE_OUT_STUB AUDIO_DEVICE_OUT_DEFAULT
/**
 * Stub audio input device. Used in policy configuration file on platforms without audio inputs.
 * This alias value to AUDIO_DEVICE_IN_DEFAULT is only used in the audio policy context.
 */
#define AUDIO_DEVICE_IN_STUB AUDIO_DEVICE_IN_DEFAULT

/* Additional information about compressed streams offloaded to
 * hardware playback
 * The version and size fields must be initialized by the caller by using
 * one of the constants defined here.
 * Must be aligned to transmit as raw memory through Binder.
 */
typedef struct {
    uint16_t version;                   // version of the info structure
    uint16_t size;                      // total size of the structure including version and size
    uint32_t sample_rate;               // sample rate in Hz
    audio_channel_mask_t channel_mask;  // channel mask
    audio_format_t format;              // audio format
    audio_stream_type_t stream_type;    // stream type
    uint32_t bit_rate;                  // bit rate in bits per second
    int64_t duration_us;                // duration in microseconds, -1 if unknown
    bool has_video;                     // true if stream is tied to a video stream
    bool is_streaming;                  // true if streaming, false if local playback
    uint32_t bit_width;
    uint32_t offload_buffer_size;       // offload fragment size
    audio_usage_t usage;
} __attribute__((aligned(8))) audio_offload_info_t;

#define AUDIO_MAKE_OFFLOAD_INFO_VERSION(maj,min) \
            ((((maj) & 0xff) << 8) | ((min) & 0xff))

#define AUDIO_OFFLOAD_INFO_VERSION_0_1 AUDIO_MAKE_OFFLOAD_INFO_VERSION(0, 1)
#define AUDIO_OFFLOAD_INFO_VERSION_CURRENT AUDIO_OFFLOAD_INFO_VERSION_0_1

static const audio_offload_info_t AUDIO_INFO_INITIALIZER = {
    /* .version = */ AUDIO_OFFLOAD_INFO_VERSION_CURRENT,
    /* .size = */ sizeof(audio_offload_info_t),
    /* .sample_rate = */ 0,
    /* .channel_mask = */ 0,
    /* .format = */ AUDIO_FORMAT_DEFAULT,
    /* .stream_type = */ AUDIO_STREAM_VOICE_CALL,
    /* .bit_rate = */ 0,
    /* .duration_us = */ 0,
    /* .has_video = */ false,
    /* .is_streaming = */ false,
    /* .bit_width = */ 16,
    /* .offload_buffer_size = */ 0,
    /* .usage = */ AUDIO_USAGE_UNKNOWN
};

/* common audio stream configuration parameters
 * You should memset() the entire structure to zero before use to
 * ensure forward compatibility
 * Must be aligned to transmit as raw memory through Binder.
 */
struct __attribute__((aligned(8))) audio_config {
    uint32_t sample_rate;
    audio_channel_mask_t channel_mask;
    audio_format_t  format;
    audio_offload_info_t offload_info;
    uint32_t frame_count;
};
typedef struct audio_config audio_config_t;

static const audio_config_t AUDIO_CONFIG_INITIALIZER = {
    /* .sample_rate = */ 0,
    /* .channel_mask = */ AUDIO_CHANNEL_NONE,
    /* .format = */ AUDIO_FORMAT_DEFAULT,
    /* .offload_info = */ {
        /* .version = */ AUDIO_OFFLOAD_INFO_VERSION_CURRENT,
        /* .size = */ sizeof(audio_offload_info_t),
        /* .sample_rate = */ 0,
        /* .channel_mask = */ 0,
        /* .format = */ AUDIO_FORMAT_DEFAULT,
        /* .stream_type = */ AUDIO_STREAM_VOICE_CALL,
        /* .bit_rate = */ 0,
        /* .duration_us = */ 0,
        /* .has_video = */ false,
        /* .is_streaming = */ false,
        /* .bit_width = */ 16,
        /* .offload_buffer_size = */ 0,
        /* .usage = */ AUDIO_USAGE_UNKNOWN
    },
    /* .frame_count = */ 0,
};

struct audio_config_base {
    uint32_t sample_rate;
    audio_channel_mask_t channel_mask;
    audio_format_t  format;
};

typedef struct audio_config_base audio_config_base_t;

static const audio_config_base_t AUDIO_CONFIG_BASE_INITIALIZER = {
    /* .sample_rate = */ 0,
    /* .channel_mask = */ AUDIO_CHANNEL_NONE,
    /* .format = */ AUDIO_FORMAT_DEFAULT
};

/* audio hw module handle functions or structures referencing a module */
typedef int audio_module_handle_t;

/******************************
 *  Volume control
 *****************************/

/** 3 dB headroom are allowed on float samples (3db = 10^(3/20) = 1.412538).
* See: https://developer.android.com/reference/android/media/AudioTrack.html#write(float[], int, int, int)
*/
#define FLOAT_NOMINAL_RANGE_HEADROOM 1.412538

/* If the audio hardware supports gain control on some audio paths,
 * the platform can expose them in the audio_policy.conf file. The audio HAL
 * will then implement gain control functions that will use the following data
 * structures. */

typedef uint32_t audio_gain_mode_t;


/* An audio_gain struct is a representation of a gain stage.
 * A gain stage is always attached to an audio port. */
struct audio_gain  {
    audio_gain_mode_t    mode;          /* e.g. AUDIO_GAIN_MODE_JOINT */
    audio_channel_mask_t channel_mask;  /* channels which gain an be controlled.
                                           N/A if AUDIO_GAIN_MODE_CHANNELS is not supported */
    int                  min_value;     /* minimum gain value in millibels */
    int                  max_value;     /* maximum gain value in millibels */
    int                  default_value; /* default gain value in millibels */
    unsigned int         step_value;    /* gain step in millibels */
    unsigned int         min_ramp_ms;   /* minimum ramp duration in ms */
    unsigned int         max_ramp_ms;   /* maximum ramp duration in ms */
};

/* The gain configuration structure is used to get or set the gain values of a
 * given port */
struct audio_gain_config  {
    int                  index;             /* index of the corresponding audio_gain in the
                                               audio_port gains[] table */
    audio_gain_mode_t    mode;              /* mode requested for this command */
    audio_channel_mask_t channel_mask;      /* channels which gain value follows.
                                               N/A in joint mode */

    // note this "8" is not FCC_8, so it won't need to be changed for > 8 channels
    int                  values[sizeof(audio_channel_mask_t) * 8]; /* gain values in millibels
                                               for each channel ordered from LSb to MSb in
                                               channel mask. The number of values is 1 in joint
                                               mode or popcount(channel_mask) */
    unsigned int         ramp_duration_ms; /* ramp duration in ms */
};

/******************************
 *  Routing control
 *****************************/

/* Types defined here are used to describe an audio source or sink at internal
 * framework interfaces (audio policy, patch panel) or at the audio HAL.
 * Sink and sources are grouped in a concept of “audio port” representing an
 * audio end point at the edge of the system managed by the module exposing
 * the interface. */

/* Each port has a unique ID or handle allocated by policy manager */
typedef int audio_port_handle_t;

/* the maximum length for the human-readable device name */
#define AUDIO_PORT_MAX_NAME_LEN 128

/* a union to store port configuration flags. Declared as a type so can be reused
   in framework code */
union audio_io_flags {
    audio_input_flags_t  input;
    audio_output_flags_t output;
};

/* maximum audio device address length */
#define AUDIO_DEVICE_MAX_ADDRESS_LEN 32

/* extension for audio port configuration structure when the audio port is a
 * hardware device */
struct audio_port_config_device_ext {
    audio_module_handle_t hw_module;                /* module the device is attached to */
    audio_devices_t       type;                     /* device type (e.g AUDIO_DEVICE_OUT_SPEAKER) */
    char                  address[AUDIO_DEVICE_MAX_ADDRESS_LEN]; /* device address. "" if N/A */
};

/* extension for audio port configuration structure when the audio port is a
 * sub mix */
struct audio_port_config_mix_ext {
    audio_module_handle_t hw_module;    /* module the stream is attached to */
    audio_io_handle_t handle;           /* I/O handle of the input/output stream */
    union {
        //TODO: change use case for output streams: use strategy and mixer attributes
        audio_stream_type_t stream;
        audio_source_t      source;
    } usecase;
};

/* extension for audio port configuration structure when the audio port is an
 * audio session */
struct audio_port_config_session_ext {
    audio_session_t   session; /* audio session */
};

/* audio port configuration structure used to specify a particular configuration of
 * an audio port */
struct audio_port_config {
    audio_port_handle_t      id;           /* port unique ID */
    audio_port_role_t        role;         /* sink or source */
    audio_port_type_t        type;         /* device, mix ... */
    unsigned int             config_mask;  /* e.g AUDIO_PORT_CONFIG_ALL */
    unsigned int             sample_rate;  /* sampling rate in Hz */
    audio_channel_mask_t     channel_mask; /* channel mask if applicable */
    audio_format_t           format;       /* format if applicable */
    struct audio_gain_config gain;         /* gain to apply if applicable */
#ifndef AUDIO_NO_SYSTEM_DECLARATIONS
    union audio_io_flags     flags;        /* framework only: HW_AV_SYNC, DIRECT, ... */
#endif
    union {
        struct audio_port_config_device_ext  device;  /* device specific info */
        struct audio_port_config_mix_ext     mix;     /* mix specific info */
        struct audio_port_config_session_ext session; /* session specific info */
    } ext;
};


/* max number of sampling rates in audio port */
#define AUDIO_PORT_MAX_SAMPLING_RATES 32
/* max number of channel masks in audio port */
#define AUDIO_PORT_MAX_CHANNEL_MASKS 32
/* max number of audio formats in audio port */
#define AUDIO_PORT_MAX_FORMATS 32
/* max number of gain controls in audio port */
#define AUDIO_PORT_MAX_GAINS 16

/* extension for audio port structure when the audio port is a hardware device */
struct audio_port_device_ext {
    audio_module_handle_t hw_module;    /* module the device is attached to */
    audio_devices_t       type;         /* device type (e.g AUDIO_DEVICE_OUT_SPEAKER) */
    char                  address[AUDIO_DEVICE_MAX_ADDRESS_LEN];
};

/* extension for audio port structure when the audio port is a sub mix */
struct audio_port_mix_ext {
    audio_module_handle_t     hw_module;     /* module the stream is attached to */
    audio_io_handle_t         handle;        /* I/O handle of the input.output stream */
    audio_mix_latency_class_t latency_class; /* latency class */
    // other attributes: routing strategies
};

/* extension for audio port structure when the audio port is an audio session */
struct audio_port_session_ext {
    audio_session_t   session; /* audio session */
};

struct audio_port {
    audio_port_handle_t      id;                /* port unique ID */
    audio_port_role_t        role;              /* sink or source */
    audio_port_type_t        type;              /* device, mix ... */
    char                     name[AUDIO_PORT_MAX_NAME_LEN];
    unsigned int             num_sample_rates;  /* number of sampling rates in following array */
    unsigned int             sample_rates[AUDIO_PORT_MAX_SAMPLING_RATES];
    unsigned int             num_channel_masks; /* number of channel masks in following array */
    audio_channel_mask_t     channel_masks[AUDIO_PORT_MAX_CHANNEL_MASKS];
    unsigned int             num_formats;       /* number of formats in following array */
    audio_format_t           formats[AUDIO_PORT_MAX_FORMATS];
    unsigned int             num_gains;         /* number of gains in following array */
    struct audio_gain        gains[AUDIO_PORT_MAX_GAINS];
    struct audio_port_config active_config;     /* current audio port configuration */
    union {
        struct audio_port_device_ext  device;
        struct audio_port_mix_ext     mix;
        struct audio_port_session_ext session;
    } ext;
};

/* An audio patch represents a connection between one or more source ports and
 * one or more sink ports. Patches are connected and disconnected by audio policy manager or by
 * applications via framework APIs.
 * Each patch is identified by a handle at the interface used to create that patch. For instance,
 * when a patch is created by the audio HAL, the HAL allocates and returns a handle.
 * This handle is unique to a given audio HAL hardware module.
 * But the same patch receives another system wide unique handle allocated by the framework.
 * This unique handle is used for all transactions inside the framework.
 */
typedef int audio_patch_handle_t;

#define AUDIO_PATCH_PORTS_MAX   16

struct audio_patch {
    audio_patch_handle_t id;            /* patch unique ID */
    unsigned int      num_sources;      /* number of sources in following array */
    struct audio_port_config sources[AUDIO_PATCH_PORTS_MAX];
    unsigned int      num_sinks;        /* number of sinks in following array */
    struct audio_port_config sinks[AUDIO_PATCH_PORTS_MAX];
};



/* a HW synchronization source returned by the audio HAL */
typedef uint32_t audio_hw_sync_t;

/* an invalid HW synchronization source indicating an error */
#define AUDIO_HW_SYNC_INVALID 0

/** @TODO export from .hal */
typedef enum {
    NONE    = 0x0,
    /**
     * Only set this flag if applications can access the audio buffer memory
     * shared with the backend (usually DSP) _without_ security issue.
     *
     * Setting this flag also implies that Binder will allow passing the shared memory FD
     * to applications.
     *
     * That usually implies that the kernel will prevent any access to the
     * memory surrounding the audio buffer as it could lead to a security breach.
     *
     * For example, a "/dev/snd/" file descriptor generally is not shareable,
     * but an "anon_inode:dmabuffer" file descriptor is shareable.
     * See also Linux kernel's dma_buf.
     *
     * This flag is required to support AAudio exclusive mode:
     * See: https://source.android.com/devices/audio/aaudio
     */
    AUDIO_MMAP_APPLICATION_SHAREABLE    = 0x1,
} audio_mmap_buffer_flag;

/**
 * Mmap buffer descriptor returned by audio_stream->create_mmap_buffer().
 * note\ Used by streams opened in mmap mode.
 */
struct audio_mmap_buffer_info {
    void*   shared_memory_address;  /**< base address of mmap memory buffer.
                                         For use by local process only */
    int32_t shared_memory_fd;       /**< FD for mmap memory buffer */
    int32_t buffer_size_frames;     /**< total buffer size in frames */
    int32_t burst_size_frames;      /**< transfer size granularity in frames */
    audio_mmap_buffer_flag flags;   /**< Attributes describing the buffer. */
};

/**
 * Mmap buffer read/write position returned by audio_stream->get_mmap_position().
 * note\ Used by streams opened in mmap mode.
 */
struct audio_mmap_position {
    int64_t  time_nanoseconds; /**< timestamp in ns, CLOCK_MONOTONIC */
    int32_t  position_frames;  /**< increasing 32 bit frame count reset when stream->stop()
                                    is called */
};

/** Metadata of a playback track for an in stream. */
typedef struct playback_track_metadata {
    audio_usage_t usage;
    audio_content_type_t content_type;
    float gain; // Normalized linear volume. 0=silence, 1=0dbfs...
} playback_track_metadata_t;

/** Metadata of a record track for an out stream. */
typedef struct record_track_metadata {
    audio_source_t source;
    float gain; // Normalized linear volume. 0=silence, 1=0dbfs...
    // For record tracks originating from a software patch, the dest_device
    // fields provide information about the downstream device.
    audio_devices_t dest_device;
    char dest_device_address[AUDIO_DEVICE_MAX_ADDRESS_LEN];
} record_track_metadata_t;


/******************************
 *  Helper functions
 *****************************/

static inline bool audio_is_output_device(audio_devices_t device)
{
    if (((device & AUDIO_DEVICE_BIT_IN) == 0) &&
            (popcount(device) == 1) && ((device & ~AUDIO_DEVICE_OUT_ALL) == 0))
        return true;
    else
        return false;
}

static inline bool audio_is_input_device(audio_devices_t device)
{
    if ((device & AUDIO_DEVICE_BIT_IN) != 0) {
        device &= ~AUDIO_DEVICE_BIT_IN;
        if ((popcount(device) == 1) && ((device & ~AUDIO_DEVICE_IN_ALL) == 0))
            return true;
    }
    return false;
}

static inline bool audio_is_output_devices(audio_devices_t device)
{
    return (device & AUDIO_DEVICE_BIT_IN) == 0;
}

static inline bool audio_is_a2dp_in_device(audio_devices_t device)
{
    if ((device & AUDIO_DEVICE_BIT_IN) != 0) {
        device &= ~AUDIO_DEVICE_BIT_IN;
        if ((popcount(device) == 1) && (device & AUDIO_DEVICE_IN_BLUETOOTH_A2DP))
            return true;
    }
    return false;
}

static inline bool audio_is_a2dp_out_device(audio_devices_t device)
{
    if ((popcount(device) == 1) && (device & AUDIO_DEVICE_OUT_ALL_A2DP))
        return true;
    else
        return false;
}

// Deprecated - use audio_is_a2dp_out_device() instead
static inline bool audio_is_a2dp_device(audio_devices_t device)
{
    return audio_is_a2dp_out_device(device);
}

static inline bool audio_is_bluetooth_sco_device(audio_devices_t device)
{
    if ((device & AUDIO_DEVICE_BIT_IN) == 0) {
        if ((popcount(device) == 1) && ((device & ~AUDIO_DEVICE_OUT_ALL_SCO) == 0))
            return true;
    } else {
        device &= ~AUDIO_DEVICE_BIT_IN;
        if ((popcount(device) == 1) && ((device & ~AUDIO_DEVICE_IN_BLUETOOTH_SCO_HEADSET) == 0))
            return true;
    }

    return false;
}

static inline bool audio_is_hearing_aid_out_device(audio_devices_t device)
{
    return device == AUDIO_DEVICE_OUT_HEARING_AID;
}

static inline bool audio_is_usb_out_device(audio_devices_t device)
{
    return ((popcount(device) == 1) && (device & AUDIO_DEVICE_OUT_ALL_USB));
}

static inline bool audio_is_usb_in_device(audio_devices_t device)
{
    if ((device & AUDIO_DEVICE_BIT_IN) != 0) {
        device &= ~AUDIO_DEVICE_BIT_IN;
        if (popcount(device) == 1 && (device & AUDIO_DEVICE_IN_ALL_USB) != 0)
            return true;
    }
    return false;
}

/* OBSOLETE - use audio_is_usb_out_device() instead. */
static inline bool audio_is_usb_device(audio_devices_t device)
{
    return audio_is_usb_out_device(device);
}

static inline bool audio_is_remote_submix_device(audio_devices_t device)
{
    if ((audio_is_output_devices(device) &&
         (device & AUDIO_DEVICE_OUT_REMOTE_SUBMIX) == AUDIO_DEVICE_OUT_REMOTE_SUBMIX)
        || (!audio_is_output_devices(device) &&
         (device & AUDIO_DEVICE_IN_REMOTE_SUBMIX) == AUDIO_DEVICE_IN_REMOTE_SUBMIX))
        return true;
    else
        return false;
}

/* Returns true if:
 *  representation is valid, and
 *  there is at least one channel bit set which _could_ correspond to an input channel, and
 *  there are no channel bits set which could _not_ correspond to an input channel.
 * Otherwise returns false.
 */
static inline bool audio_is_input_channel(audio_channel_mask_t channel)
{
    uint32_t bits = audio_channel_mask_get_bits(channel);
    switch (audio_channel_mask_get_representation(channel)) {
    case AUDIO_CHANNEL_REPRESENTATION_POSITION:
        if (bits & ~AUDIO_CHANNEL_IN_ALL) {
            bits = 0;
        }
        FALLTHROUGH_INTENDED;
    case AUDIO_CHANNEL_REPRESENTATION_INDEX:
        return bits != 0;
    default:
        return false;
    }
}

/* Returns true if:
 *  representation is valid, and
 *  there is at least one channel bit set which _could_ correspond to an output channel, and
 *  there are no channel bits set which could _not_ correspond to an output channel.
 * Otherwise returns false.
 */
static inline bool audio_is_output_channel(audio_channel_mask_t channel)
{
    uint32_t bits = audio_channel_mask_get_bits(channel);
    switch (audio_channel_mask_get_representation(channel)) {
    case AUDIO_CHANNEL_REPRESENTATION_POSITION:
        if (bits & ~AUDIO_CHANNEL_OUT_ALL) {
            bits = 0;
        }
        FALLTHROUGH_INTENDED;
    case AUDIO_CHANNEL_REPRESENTATION_INDEX:
        return bits != 0;
    default:
        return false;
    }
}

/* Returns the number of channels from an input channel mask,
 * used in the context of audio input or recording.
 * If a channel bit is set which could _not_ correspond to an input channel,
 * it is excluded from the count.
 * Returns zero if the representation is invalid.
 */
static inline uint32_t audio_channel_count_from_in_mask(audio_channel_mask_t channel)
{
    uint32_t bits = audio_channel_mask_get_bits(channel);
    switch (audio_channel_mask_get_representation(channel)) {
    case AUDIO_CHANNEL_REPRESENTATION_POSITION:
        // TODO: We can now merge with from_out_mask and remove anding
        bits &= AUDIO_CHANNEL_IN_ALL;
        FALLTHROUGH_INTENDED;
    case AUDIO_CHANNEL_REPRESENTATION_INDEX:
        return popcount(bits);
    default:
        return 0;
    }
}

/* Returns the number of channels from an output channel mask,
 * used in the context of audio output or playback.
 * If a channel bit is set which could _not_ correspond to an output channel,
 * it is excluded from the count.
 * Returns zero if the representation is invalid.
 */
static inline uint32_t audio_channel_count_from_out_mask(audio_channel_mask_t channel)
{
    uint32_t bits = audio_channel_mask_get_bits(channel);
    switch (audio_channel_mask_get_representation(channel)) {
    case AUDIO_CHANNEL_REPRESENTATION_POSITION:
        // TODO: We can now merge with from_in_mask and remove anding
        bits &= AUDIO_CHANNEL_OUT_ALL;
        FALLTHROUGH_INTENDED;
    case AUDIO_CHANNEL_REPRESENTATION_INDEX:
        return popcount(bits);
    default:
        return 0;
    }
}

/* Derive a channel mask for index assignment from a channel count.
 * Returns the matching channel mask,
 * or AUDIO_CHANNEL_NONE if the channel count is zero,
 * or AUDIO_CHANNEL_INVALID if the channel count exceeds AUDIO_CHANNEL_COUNT_MAX.
 */
static inline audio_channel_mask_t audio_channel_mask_for_index_assignment_from_count(
        uint32_t channel_count)
{
    if (channel_count == 0) {
        return AUDIO_CHANNEL_NONE;
    }
    if (channel_count > AUDIO_CHANNEL_COUNT_MAX) {
        return AUDIO_CHANNEL_INVALID;
    }
    uint32_t bits = (1 << channel_count) - 1;
    return audio_channel_mask_from_representation_and_bits(
            AUDIO_CHANNEL_REPRESENTATION_INDEX, bits);
}

/* Derive an output channel mask for position assignment from a channel count.
 * This is to be used when the content channel mask is unknown. The 1, 2, 4, 5, 6, 7 and 8 channel
 * cases are mapped to the standard game/home-theater layouts, but note that 4 is mapped to quad,
 * and not stereo + FC + mono surround. A channel count of 3 is arbitrarily mapped to stereo + FC
 * for continuity with stereo.
 * Returns the matching channel mask,
 * or AUDIO_CHANNEL_NONE if the channel count is zero,
 * or AUDIO_CHANNEL_INVALID if the channel count exceeds that of the
 * configurations for which a default output channel mask is defined.
 */
static inline audio_channel_mask_t audio_channel_out_mask_from_count(uint32_t channel_count)
{
    uint32_t bits;
    switch (channel_count) {
    case 0:
        return AUDIO_CHANNEL_NONE;
    case 1:
        bits = AUDIO_CHANNEL_OUT_MONO;
        break;
    case 2:
        bits = AUDIO_CHANNEL_OUT_STEREO;
        break;
    case 3: // 2.1
        bits = AUDIO_CHANNEL_OUT_STEREO | AUDIO_CHANNEL_OUT_LOW_FREQUENCY;
        break;
    case 4: // 4.0
        bits = AUDIO_CHANNEL_OUT_QUAD;
        break;
    case 5: // 5.0
        bits = AUDIO_CHANNEL_OUT_QUAD | AUDIO_CHANNEL_OUT_FRONT_CENTER;
        break;
    case 6: // 5.1
        bits = AUDIO_CHANNEL_OUT_5POINT1;
        break;
    case 7: // 6.1
        bits = AUDIO_CHANNEL_OUT_5POINT1 | AUDIO_CHANNEL_OUT_BACK_CENTER;
        break;
    case 8:
        bits = AUDIO_CHANNEL_OUT_7POINT1;
        break;
    // FIXME FCC_8
    default:
        return AUDIO_CHANNEL_INVALID;
    }
    return audio_channel_mask_from_representation_and_bits(
            AUDIO_CHANNEL_REPRESENTATION_POSITION, bits);
}

/* Derive a default input channel mask from a channel count.
 * Assumes a position mask for mono and stereo, or an index mask for channel counts > 2.
 * Returns the matching channel mask,
 * or AUDIO_CHANNEL_NONE if the channel count is zero,
 * or AUDIO_CHANNEL_INVALID if the channel count exceeds that of the
 * configurations for which a default input channel mask is defined.
 */
static inline audio_channel_mask_t audio_channel_in_mask_from_count(uint32_t channel_count)
{
    uint32_t bits;
    switch (channel_count) {
    case 0:
        return AUDIO_CHANNEL_NONE;
    case 1:
        bits = AUDIO_CHANNEL_IN_MONO;
        break;
    case 2:
        bits = AUDIO_CHANNEL_IN_STEREO;
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        // FIXME FCC_8
        return audio_channel_mask_for_index_assignment_from_count(channel_count);
    default:
        return AUDIO_CHANNEL_INVALID;
    }
    return audio_channel_mask_from_representation_and_bits(
            AUDIO_CHANNEL_REPRESENTATION_POSITION, bits);
}

/* Derive a default haptic channel mask from a channel count.
 */
static inline audio_channel_mask_t haptic_channel_mask_from_count(uint32_t channel_count)
{
    switch(channel_count) {
    case 0:
        return AUDIO_CHANNEL_NONE;
    case 1:
        return AUDIO_CHANNEL_OUT_HAPTIC_A;
    case 2:
        return AUDIO_CHANNEL_OUT_HAPTIC_AB;
    default:
        return AUDIO_CHANNEL_INVALID;
    }
}

static inline audio_channel_mask_t audio_channel_mask_in_to_out(audio_channel_mask_t in)
{
    switch (in) {
    case AUDIO_CHANNEL_IN_MONO:
        return AUDIO_CHANNEL_OUT_MONO;
    case AUDIO_CHANNEL_IN_STEREO:
        return AUDIO_CHANNEL_OUT_STEREO;
    case AUDIO_CHANNEL_IN_5POINT1:
        return AUDIO_CHANNEL_OUT_5POINT1;
    case AUDIO_CHANNEL_IN_3POINT1POINT2:
        return AUDIO_CHANNEL_OUT_3POINT1POINT2;
    case AUDIO_CHANNEL_IN_3POINT0POINT2:
        return AUDIO_CHANNEL_OUT_3POINT0POINT2;
    case AUDIO_CHANNEL_IN_2POINT1POINT2:
        return AUDIO_CHANNEL_OUT_2POINT1POINT2;
    case AUDIO_CHANNEL_IN_2POINT0POINT2:
        return AUDIO_CHANNEL_OUT_2POINT0POINT2;
    default:
        return AUDIO_CHANNEL_INVALID;
    }
}

static inline audio_channel_mask_t audio_channel_mask_out_to_in(audio_channel_mask_t out)
{
    switch (out) {
    case AUDIO_CHANNEL_OUT_MONO:
        return AUDIO_CHANNEL_IN_MONO;
    case AUDIO_CHANNEL_OUT_STEREO:
        return AUDIO_CHANNEL_IN_STEREO;
    case AUDIO_CHANNEL_OUT_5POINT1:
        return AUDIO_CHANNEL_IN_5POINT1;
    case AUDIO_CHANNEL_OUT_3POINT1POINT2:
        return AUDIO_CHANNEL_IN_3POINT1POINT2;
    case AUDIO_CHANNEL_OUT_3POINT0POINT2:
        return AUDIO_CHANNEL_IN_3POINT0POINT2;
    case AUDIO_CHANNEL_OUT_2POINT1POINT2:
        return AUDIO_CHANNEL_IN_2POINT1POINT2;
    case AUDIO_CHANNEL_OUT_2POINT0POINT2:
        return AUDIO_CHANNEL_IN_2POINT0POINT2;
    default:
        return AUDIO_CHANNEL_INVALID;
    }
}

static inline bool audio_channel_position_mask_is_out_canonical(audio_channel_mask_t channelMask)
{
    if (audio_channel_mask_get_representation(channelMask)
            != AUDIO_CHANNEL_REPRESENTATION_POSITION) {
        return false;
    }
    const uint32_t audioChannelCount = audio_channel_count_from_out_mask(
            channelMask & ~AUDIO_CHANNEL_HAPTIC_ALL);
    const uint32_t hapticChannelCount = audio_channel_count_from_out_mask(
            channelMask & AUDIO_CHANNEL_HAPTIC_ALL);
    return channelMask == (audio_channel_out_mask_from_count(audioChannelCount) |
            haptic_channel_mask_from_count(hapticChannelCount));
}

static inline bool audio_is_valid_format(audio_format_t format)
{
    switch (format & AUDIO_FORMAT_MAIN_MASK) {
    case AUDIO_FORMAT_PCM:
        switch (format) {
        case AUDIO_FORMAT_PCM_16_BIT:
        case AUDIO_FORMAT_PCM_8_BIT:
        case AUDIO_FORMAT_PCM_32_BIT:
        case AUDIO_FORMAT_PCM_8_24_BIT:
        case AUDIO_FORMAT_PCM_FLOAT:
        case AUDIO_FORMAT_PCM_24_BIT_PACKED:
            return true;
        default:
            return false;
        }
        /* not reached */
    case AUDIO_FORMAT_MP3:
    case AUDIO_FORMAT_AMR_NB:
    case AUDIO_FORMAT_AMR_WB:
        return true;
    case AUDIO_FORMAT_AAC:
        switch (format) {
        case AUDIO_FORMAT_AAC:
        case AUDIO_FORMAT_AAC_MAIN:
        case AUDIO_FORMAT_AAC_LC:
        case AUDIO_FORMAT_AAC_SSR:
        case AUDIO_FORMAT_AAC_LTP:
        case AUDIO_FORMAT_AAC_HE_V1:
        case AUDIO_FORMAT_AAC_SCALABLE:
        case AUDIO_FORMAT_AAC_ERLC:
        case AUDIO_FORMAT_AAC_LD:
        case AUDIO_FORMAT_AAC_HE_V2:
        case AUDIO_FORMAT_AAC_ELD:
        case AUDIO_FORMAT_AAC_XHE:
            return true;
        default:
            return false;
        }
        /* not reached */
    case AUDIO_FORMAT_HE_AAC_V1:
    case AUDIO_FORMAT_HE_AAC_V2:
    case AUDIO_FORMAT_VORBIS:
    case AUDIO_FORMAT_OPUS:
    case AUDIO_FORMAT_AC3:
        return true;
    case AUDIO_FORMAT_E_AC3:
        switch (format) {
        case AUDIO_FORMAT_E_AC3:
        case AUDIO_FORMAT_E_AC3_JOC:
            return true;
        default:
            return false;
        }
        /* not reached */
    case AUDIO_FORMAT_DTS:
    case AUDIO_FORMAT_DTS_HD:
    case AUDIO_FORMAT_IEC61937:
    case AUDIO_FORMAT_DOLBY_TRUEHD:
    case AUDIO_FORMAT_EVRC:
    case AUDIO_FORMAT_EVRCB:
    case AUDIO_FORMAT_EVRCWB:
    case AUDIO_FORMAT_EVRCNW:
    case AUDIO_FORMAT_AAC_ADIF:
    case AUDIO_FORMAT_WMA:
    case AUDIO_FORMAT_WMA_PRO:
    case AUDIO_FORMAT_AMR_WB_PLUS:
    case AUDIO_FORMAT_MP2:
    case AUDIO_FORMAT_QCELP:
    case AUDIO_FORMAT_DSD:
    case AUDIO_FORMAT_FLAC:
    case AUDIO_FORMAT_ALAC:
    case AUDIO_FORMAT_APE:
        return true;
    case AUDIO_FORMAT_AAC_ADTS:
        switch (format) {
        case AUDIO_FORMAT_AAC_ADTS:
        case AUDIO_FORMAT_AAC_ADTS_MAIN:
        case AUDIO_FORMAT_AAC_ADTS_LC:
        case AUDIO_FORMAT_AAC_ADTS_SSR:
        case AUDIO_FORMAT_AAC_ADTS_LTP:
        case AUDIO_FORMAT_AAC_ADTS_HE_V1:
        case AUDIO_FORMAT_AAC_ADTS_SCALABLE:
        case AUDIO_FORMAT_AAC_ADTS_ERLC:
        case AUDIO_FORMAT_AAC_ADTS_LD:
        case AUDIO_FORMAT_AAC_ADTS_HE_V2:
        case AUDIO_FORMAT_AAC_ADTS_ELD:
        case AUDIO_FORMAT_AAC_ADTS_XHE:
            return true;
        default:
            return false;
        }
        /* not reached */
    case AUDIO_FORMAT_SBC:
    case AUDIO_FORMAT_APTX:
    case AUDIO_FORMAT_APTX_HD:
    case AUDIO_FORMAT_AC4:
    case AUDIO_FORMAT_LDAC:
        return true;
    case AUDIO_FORMAT_MAT:
        switch (format) {
        case AUDIO_FORMAT_MAT:
        case AUDIO_FORMAT_MAT_1_0:
        case AUDIO_FORMAT_MAT_2_0:
        case AUDIO_FORMAT_MAT_2_1:
            return true;
        default:
            return false;
        }
        /* not reached */
    case AUDIO_FORMAT_AAC_LATM:
        switch (format) {
        case AUDIO_FORMAT_AAC_LATM:
        case AUDIO_FORMAT_AAC_LATM_LC:
        case AUDIO_FORMAT_AAC_LATM_HE_V1:
        case AUDIO_FORMAT_AAC_LATM_HE_V2:
            return true;
        default:
            return false;
        }
        /* not reached */
    case AUDIO_FORMAT_CELT:
    case AUDIO_FORMAT_APTX_ADAPTIVE:
    case AUDIO_FORMAT_LHDC:
    case AUDIO_FORMAT_LHDC_LL:
    case AUDIO_FORMAT_APTX_TWSP:
        return true;
    default:
        return false;
    }
}

/**
 * Extract the primary format, eg. PCM, AC3, etc.
 */
static inline audio_format_t audio_get_main_format(audio_format_t format)
{
    return (audio_format_t)(format & AUDIO_FORMAT_MAIN_MASK);
}

/**
 * Is the data plain PCM samples that can be scaled and mixed?
 */
static inline bool audio_is_linear_pcm(audio_format_t format)
{
    return (audio_get_main_format(format) == AUDIO_FORMAT_PCM);
}

/**
 * For this format, is the number of PCM audio frames directly proportional
 * to the number of data bytes?
 *
 * In other words, is the format transported as PCM audio samples,
 * but not necessarily scalable or mixable.
 * This returns true for real PCM, but also for AUDIO_FORMAT_IEC61937,
 * which is transported as 16 bit PCM audio, but where the encoded data
 * cannot be mixed or scaled.
 */
static inline bool audio_has_proportional_frames(audio_format_t format)
{
    audio_format_t mainFormat = audio_get_main_format(format);
    return (mainFormat == AUDIO_FORMAT_PCM
            || mainFormat == AUDIO_FORMAT_IEC61937);
}

static inline size_t audio_bytes_per_sample(audio_format_t format)
{
    size_t size = 0;

    switch (format) {
    case AUDIO_FORMAT_PCM_32_BIT:
    case AUDIO_FORMAT_PCM_8_24_BIT:
        size = sizeof(int32_t);
        break;
    case AUDIO_FORMAT_PCM_24_BIT_PACKED:
        size = sizeof(uint8_t) * 3;
        break;
    case AUDIO_FORMAT_PCM_16_BIT:
    case AUDIO_FORMAT_IEC61937:
        size = sizeof(int16_t);
        break;
    case AUDIO_FORMAT_PCM_8_BIT:
        size = sizeof(uint8_t);
        break;
    case AUDIO_FORMAT_PCM_FLOAT:
        size = sizeof(float);
        break;
    default:
        break;
    }
    return size;
}

static inline size_t audio_bytes_per_frame(uint32_t channel_count, audio_format_t format)
{
    // cannot overflow for reasonable channel_count
    return channel_count * audio_bytes_per_sample(format);
}

/* converts device address to string sent to audio HAL via set_parameters */
static inline char *audio_device_address_to_parameter(audio_devices_t device, const char *address)
{
    const size_t kSize = AUDIO_DEVICE_MAX_ADDRESS_LEN + sizeof("a2dp_sink_address=");
    char param[kSize];

    if ((device & AUDIO_DEVICE_BIT_IN) != 0) {
        device &= ~AUDIO_DEVICE_BIT_IN;
        if (device & AUDIO_DEVICE_IN_BLUETOOTH_A2DP)
            snprintf(param, kSize, "%s=%s", "a2dp_source_address", address);
        else if (device & AUDIO_DEVICE_IN_REMOTE_SUBMIX)
            snprintf(param, kSize, "%s=%s", "mix", address);
        else
            snprintf(param, kSize, "%s", address);
    } else {
        if (device & AUDIO_DEVICE_OUT_ALL_A2DP)
            snprintf(param, kSize, "%s=%s", "a2dp_sink_address", address);
        else if (device & AUDIO_DEVICE_OUT_REMOTE_SUBMIX)
            snprintf(param, kSize, "%s=%s", "mix", address);
        else
            snprintf(param, kSize, "%s", address);
    }
    return strdup(param);
}

static inline bool audio_device_is_digital(audio_devices_t device) {
    if ((device & AUDIO_DEVICE_BIT_IN) != 0) {
        // input
        return (~AUDIO_DEVICE_BIT_IN & device & (AUDIO_DEVICE_IN_ALL_USB |
                          AUDIO_DEVICE_IN_HDMI |
                          AUDIO_DEVICE_IN_HDMI_ARC |
                          AUDIO_DEVICE_IN_SPDIF |
                          AUDIO_DEVICE_IN_IP |
                          AUDIO_DEVICE_IN_BUS)) != 0;
    } else {
        // output
        return (device & (AUDIO_DEVICE_OUT_ALL_USB |
                          AUDIO_DEVICE_OUT_HDMI |
                          AUDIO_DEVICE_OUT_HDMI_ARC |
                          AUDIO_DEVICE_OUT_SPDIF |
                          AUDIO_DEVICE_OUT_IP |
                          AUDIO_DEVICE_OUT_BUS)) != 0;
    }
}

#ifndef AUDIO_NO_SYSTEM_DECLARATIONS

static inline bool audio_gain_config_are_equal(
        const struct audio_gain_config *lhs, const struct audio_gain_config *rhs) {
    if (lhs->mode != rhs->mode) return false;
    switch (lhs->mode) {
    case AUDIO_GAIN_MODE_JOINT:
        if (lhs->values[0] != rhs->values[0]) return false;
        break;
    case AUDIO_GAIN_MODE_CHANNELS:
    case AUDIO_GAIN_MODE_RAMP:
        if (lhs->channel_mask != rhs->channel_mask) return false;
        for (int i = 0; i < popcount(lhs->channel_mask); ++i) {
            if (lhs->values[i] != rhs->values[i]) return false;
        }
        break;
    default: return false;
    }
    return lhs->ramp_duration_ms == rhs->ramp_duration_ms;
}

static inline bool audio_port_config_has_input_direction(const struct audio_port_config *port_cfg) {
    switch (port_cfg->type) {
    case AUDIO_PORT_TYPE_DEVICE:
        switch (port_cfg->role) {
        case AUDIO_PORT_ROLE_SOURCE: return true;
        case AUDIO_PORT_ROLE_SINK: return false;
        default: return false;
        }
    case AUDIO_PORT_TYPE_MIX:
        switch (port_cfg->role) {
        case AUDIO_PORT_ROLE_SOURCE: return false;
        case AUDIO_PORT_ROLE_SINK: return true;
        default: return false;
        }
    default: return false;
    }
}

static inline bool audio_port_configs_are_equal(
        const struct audio_port_config *lhs, const struct audio_port_config *rhs) {
    if (lhs->role != rhs->role || lhs->type != rhs->type) return false;
    switch (lhs->type) {
    case AUDIO_PORT_TYPE_NONE: break;
    case AUDIO_PORT_TYPE_DEVICE:
        if (lhs->ext.device.hw_module != rhs->ext.device.hw_module ||
                lhs->ext.device.type != rhs->ext.device.type ||
                strncmp(lhs->ext.device.address, rhs->ext.device.address,
                        AUDIO_DEVICE_MAX_ADDRESS_LEN) != 0) {
            return false;
        }
        break;
    case AUDIO_PORT_TYPE_MIX:
        if (lhs->ext.mix.hw_module != rhs->ext.mix.hw_module ||
                lhs->ext.mix.handle != rhs->ext.mix.handle) return false;
        if (lhs->role == AUDIO_PORT_ROLE_SOURCE &&
                lhs->ext.mix.usecase.stream != rhs->ext.mix.usecase.stream) return false;
        else if (lhs->role == AUDIO_PORT_ROLE_SINK &&
                lhs->ext.mix.usecase.source != rhs->ext.mix.usecase.source) return false;
        break;
    case AUDIO_PORT_TYPE_SESSION:
        if (lhs->ext.session.session != rhs->ext.session.session) return false;
        break;
    default: return false;
    }
    return lhs->config_mask == rhs->config_mask &&
            ((lhs->config_mask & AUDIO_PORT_CONFIG_SAMPLE_RATE) == 0 ||
                    lhs->sample_rate == rhs->sample_rate) &&
            ((lhs->config_mask & AUDIO_PORT_CONFIG_CHANNEL_MASK) == 0 ||
                    lhs->channel_mask == rhs->channel_mask) &&
            ((lhs->config_mask & AUDIO_PORT_CONFIG_FORMAT) == 0 ||
                    lhs->format == rhs->format) &&
            ((lhs->config_mask & AUDIO_PORT_CONFIG_GAIN) == 0 ||
                    audio_gain_config_are_equal(&lhs->gain, &rhs->gain)) &&
            ((lhs->config_mask & AUDIO_PORT_CONFIG_FLAGS) == 0 ||
                    (audio_port_config_has_input_direction(lhs) ?
                            lhs->flags.input == rhs->flags.input :
                            lhs->flags.output == rhs->flags.output));
}

static inline bool audio_port_config_has_hw_av_sync(const struct audio_port_config *port_cfg) {
    if (!(port_cfg->config_mask & AUDIO_PORT_CONFIG_FLAGS)) {
        return false;
    }
    return audio_port_config_has_input_direction(port_cfg) ?
            port_cfg->flags.input & AUDIO_INPUT_FLAG_HW_AV_SYNC
            : port_cfg->flags.output & AUDIO_OUTPUT_FLAG_HW_AV_SYNC;
}

static inline bool audio_patch_has_hw_av_sync(const struct audio_patch *patch) {
    for (unsigned int i = 0; i < patch->num_sources; ++i) {
        if (audio_port_config_has_hw_av_sync(&patch->sources[i])) return true;
    }
    for (unsigned int i = 0; i < patch->num_sinks; ++i) {
        if (audio_port_config_has_hw_av_sync(&patch->sinks[i])) return true;
    }
    return false;
}

static inline bool audio_patch_is_valid(const struct audio_patch *patch) {
    // Note that patch can have no sinks.
    return patch->num_sources != 0 && patch->num_sources <= AUDIO_PATCH_PORTS_MAX &&
            patch->num_sinks <= AUDIO_PATCH_PORTS_MAX;
}

// Note that when checking for equality the order of ports must match.
// Patches will not be equivalent if they contain the same ports but they are permuted differently.
static inline bool audio_patches_are_equal(
        const struct audio_patch *lhs, const struct audio_patch *rhs) {
    if (!audio_patch_is_valid(lhs) || !audio_patch_is_valid(rhs)) return false;
    if (lhs->num_sources != rhs->num_sources || lhs->num_sinks != rhs->num_sinks) return false;
    for (unsigned int i = 0; i < lhs->num_sources; ++i) {
        if (!audio_port_configs_are_equal(&lhs->sources[i], &rhs->sources[i])) return false;
    }
    for (unsigned int i = 0; i < lhs->num_sinks; ++i) {
        if (!audio_port_configs_are_equal(&lhs->sinks[i], &rhs->sinks[i])) return false;
    }
    return true;
}

#endif

// Unique effect ID (can be generated from the following site:
//  http://www.itu.int/ITU-T/asn1/uuid.html)
// This struct is used for effects identification and in soundtrigger.
typedef struct audio_uuid_s {
    uint32_t timeLow;
    uint16_t timeMid;
    uint16_t timeHiAndVersion;
    uint16_t clockSeq;
    uint8_t node[6];
} audio_uuid_t;

//TODO: audio_microphone_location_t need to move to HAL v4.0
typedef enum {
    AUDIO_MICROPHONE_LOCATION_UNKNOWN = 0,
    AUDIO_MICROPHONE_LOCATION_MAINBODY = 1,
    AUDIO_MICROPHONE_LOCATION_MAINBODY_MOVABLE = 2,
    AUDIO_MICROPHONE_LOCATION_PERIPHERAL = 3,
    AUDIO_MICROPHONE_LOCATION_CNT = 4,
} audio_microphone_location_t;

//TODO: audio_microphone_directionality_t need to move to HAL v4.0
typedef enum {
    AUDIO_MICROPHONE_DIRECTIONALITY_UNKNOWN = 0,
    AUDIO_MICROPHONE_DIRECTIONALITY_OMNI = 1,
    AUDIO_MICROPHONE_DIRECTIONALITY_BI_DIRECTIONAL = 2,
    AUDIO_MICROPHONE_DIRECTIONALITY_CARDIOID = 3,
    AUDIO_MICROPHONE_DIRECTIONALITY_HYPER_CARDIOID = 4,
    AUDIO_MICROPHONE_DIRECTIONALITY_SUPER_CARDIOID = 5,
    AUDIO_MICROPHONE_DIRECTIONALITY_CNT = 6,
} audio_microphone_directionality_t;

/* A 3D point which could be used to represent geometric location
 * or orientation of a microphone.
 */
struct audio_microphone_coordinate {
    float x;
    float y;
    float z;
};

/* An number to indicate which group the microphone locate. Main body is
 * usually group 0. Developer could use this value to group the microphones
 * that locate on the same peripheral or attachments.
 */
typedef int audio_microphone_group_t;

typedef enum {
    AUDIO_MICROPHONE_CHANNEL_MAPPING_UNUSED = 0,
    AUDIO_MICROPHONE_CHANNEL_MAPPING_DIRECT = 1,
    AUDIO_MICROPHONE_CHANNEL_MAPPING_PROCESSED = 2,
    AUDIO_MICROPHONE_CHANNEL_MAPPING_CNT = 3,
} audio_microphone_channel_mapping_t;

/* the maximum length for the microphone id */
#define AUDIO_MICROPHONE_ID_MAX_LEN 32
/* max number of frequency responses in a frequency response table */
#define AUDIO_MICROPHONE_MAX_FREQUENCY_RESPONSES 256
/* max number of microphone */
#define AUDIO_MICROPHONE_MAX_COUNT 32
/* the value of unknown spl */
#define AUDIO_MICROPHONE_SPL_UNKNOWN -FLT_MAX
/* the value of unknown sensitivity */
#define AUDIO_MICROPHONE_SENSITIVITY_UNKNOWN -FLT_MAX
/* the value of unknown coordinate */
#define AUDIO_MICROPHONE_COORDINATE_UNKNOWN -FLT_MAX
/* the value used as address when the address of bottom microphone is empty */
#define AUDIO_BOTTOM_MICROPHONE_ADDRESS "bottom"
/* the value used as address when the address of back microphone is empty */
#define AUDIO_BACK_MICROPHONE_ADDRESS "back"

struct audio_microphone_characteristic_t {
    char                               device_id[AUDIO_MICROPHONE_ID_MAX_LEN];
    audio_port_handle_t                id;
    audio_devices_t                    device;
    char                               address[AUDIO_DEVICE_MAX_ADDRESS_LEN];
    audio_microphone_channel_mapping_t channel_mapping[AUDIO_CHANNEL_COUNT_MAX];
    audio_microphone_location_t        location;
    audio_microphone_group_t           group;
    unsigned int                       index_in_the_group;
    float                              sensitivity;
    float                              max_spl;
    float                              min_spl;
    audio_microphone_directionality_t  directionality;
    unsigned int                       num_frequency_responses;
    float frequency_responses[2][AUDIO_MICROPHONE_MAX_FREQUENCY_RESPONSES];
    struct audio_microphone_coordinate geometric_location;
    struct audio_microphone_coordinate orientation;
};

__END_DECLS

/**
 * List of known audio HAL modules. This is the base name of the audio HAL
 * library composed of the "audio." prefix, one of the base names below and
 * a suffix specific to the device.
 * e.g: audio.primary.goldfish.so or audio.a2dp.default.so
 *
 * The same module names are used in audio policy configuration files.
 */

#define AUDIO_HARDWARE_MODULE_ID_PRIMARY "primary"
#define AUDIO_HARDWARE_MODULE_ID_A2DP "a2dp"
#define AUDIO_HARDWARE_MODULE_ID_USB "usb"
#define AUDIO_HARDWARE_MODULE_ID_REMOTE_SUBMIX "r_submix"
#define AUDIO_HARDWARE_MODULE_ID_CODEC_OFFLOAD "codec_offload"
#define AUDIO_HARDWARE_MODULE_ID_STUB "stub"
#define AUDIO_HARDWARE_MODULE_ID_HEARING_AID "hearing_aid"
#define AUDIO_HARDWARE_MODULE_ID_MSD "msd"

/**
 * Multi-Stream Decoder (MSD) HAL service name. MSD HAL is used to mix
 * encoded streams together with PCM streams, producing re-encoded
 * streams or PCM streams.
 *
 * The service must register itself using this name, and audioserver
 * tries to instantiate a device factory using this name as well.
 * Note that the HIDL implementation library file name *must* have the
 * suffix "msd" in order to be picked up by HIDL that is:
 *
 *   android.hardware.audio@x.x-implmsd.so
 */
#define AUDIO_HAL_SERVICE_NAME_MSD "msd"

/**
 * Parameter definitions.
 * Note that in the framework code it's recommended to use AudioParameter.h
 * instead of these preprocessor defines, and for sure avoid just copying
 * the constant values.
 */

#define AUDIO_PARAMETER_VALUE_ON "on"
#define AUDIO_PARAMETER_VALUE_OFF "off"

/**
 *  audio device parameters
 */

/* BT SCO Noise Reduction + Echo Cancellation parameters */
#define AUDIO_PARAMETER_KEY_BT_NREC "bt_headset_nrec"

/* Get a new HW synchronization source identifier.
 * Return a valid source (positive integer) or AUDIO_HW_SYNC_INVALID if an error occurs
 * or no HW sync is available. */
#define AUDIO_PARAMETER_HW_AV_SYNC "hw_av_sync"

/* Screen state */
#define AUDIO_PARAMETER_KEY_SCREEN_STATE "screen_state"

/* User's preferred audio language setting (in ISO 639-2/T three-letter string code)
 * used to select a specific language presentation for next generation audio codecs. */
#define AUDIO_PARAMETER_KEY_AUDIO_LANGUAGE_PREFERRED "audio_language_preferred"

/**
 *  audio stream parameters
 */

#define AUDIO_PARAMETER_STREAM_ROUTING "routing"             /* audio_devices_t */
#define AUDIO_PARAMETER_STREAM_FORMAT "format"               /* audio_format_t */
#define AUDIO_PARAMETER_STREAM_CHANNELS "channels"           /* audio_channel_mask_t */
#define AUDIO_PARAMETER_STREAM_FRAME_COUNT "frame_count"     /* size_t */
#define AUDIO_PARAMETER_STREAM_INPUT_SOURCE "input_source"   /* audio_source_t */
#define AUDIO_PARAMETER_STREAM_SAMPLING_RATE "sampling_rate" /* uint32_t */

/* Request the presentation id to be decoded by a next gen audio decoder */
#define AUDIO_PARAMETER_STREAM_PRESENTATION_ID "presentation_id" /* int32_t */

/* Request the program id to be decoded by a next gen audio decoder */
#define AUDIO_PARAMETER_STREAM_PROGRAM_ID "program_id"           /* int32_t */

#define AUDIO_PARAMETER_DEVICE_CONNECT "connect"            /* audio_devices_t */
#define AUDIO_PARAMETER_DEVICE_DISCONNECT "disconnect"      /* audio_devices_t */

/* Enable mono audio playback if 1, else should be 0. */
#define AUDIO_PARAMETER_MONO_OUTPUT "mono_output"

/* Set the HW synchronization source for an output stream. */
#define AUDIO_PARAMETER_STREAM_HW_AV_SYNC "hw_av_sync"

/* Query supported formats. The response is a '|' separated list of strings from
 * audio_format_t enum e.g: "sup_formats=AUDIO_FORMAT_PCM_16_BIT" */
#define AUDIO_PARAMETER_STREAM_SUP_FORMATS "sup_formats"
/* Query supported channel masks. The response is a '|' separated list of strings from
 * audio_channel_mask_t enum e.g: "sup_channels=AUDIO_CHANNEL_OUT_STEREO|AUDIO_CHANNEL_OUT_MONO" */
#define AUDIO_PARAMETER_STREAM_SUP_CHANNELS "sup_channels"
/* Query supported sampling rates. The response is a '|' separated list of integer values e.g:
 * "sup_sampling_rates=44100|48000" */
#define AUDIO_PARAMETER_STREAM_SUP_SAMPLING_RATES "sup_sampling_rates"

#define AUDIO_PARAMETER_VALUE_LIST_SEPARATOR "|"

/* Reconfigure offloaded A2DP codec */
#define AUDIO_PARAMETER_RECONFIG_A2DP "reconfigA2dp"
/* Query if HwModule supports reconfiguration of offloaded A2DP codec */
#define AUDIO_PARAMETER_A2DP_RECONFIG_SUPPORTED "isReconfigA2dpSupported"

/**
 * audio codec parameters
 */

#define AUDIO_OFFLOAD_CODEC_PARAMS "music_offload_codec_param"
#define AUDIO_OFFLOAD_CODEC_BIT_PER_SAMPLE "music_offload_bit_per_sample"
#define AUDIO_OFFLOAD_CODEC_BIT_RATE "music_offload_bit_rate"
#define AUDIO_OFFLOAD_CODEC_AVG_BIT_RATE "music_offload_avg_bit_rate"
#define AUDIO_OFFLOAD_CODEC_ID "music_offload_codec_id"
#define AUDIO_OFFLOAD_CODEC_BLOCK_ALIGN "music_offload_block_align"
#define AUDIO_OFFLOAD_CODEC_SAMPLE_RATE "music_offload_sample_rate"
#define AUDIO_OFFLOAD_CODEC_ENCODE_OPTION "music_offload_encode_option"
#define AUDIO_OFFLOAD_CODEC_NUM_CHANNEL  "music_offload_num_channels"
#define AUDIO_OFFLOAD_CODEC_DOWN_SAMPLING  "music_offload_down_sampling"
#define AUDIO_OFFLOAD_CODEC_DELAY_SAMPLES  "delay_samples"
#define AUDIO_OFFLOAD_CODEC_PADDING_SAMPLES  "padding_samples"

#endif  // ANDROID_AUDIO_CORE_H
