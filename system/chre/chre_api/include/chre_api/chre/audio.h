/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef _CHRE_AUDIO_H_
#define _CHRE_AUDIO_H_

/**
 * @file
 * The API for requesting audio in the Context Hub Runtime Environment.
 *
 * This includes the definition of audio data structures and the ability to
 * request audio streams.
 */

#include <chre/event.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The current compatibility version of the chreAudioDataEvent structure.
 */
#define CHRE_AUDIO_DATA_EVENT_VERSION  UINT8_C(1)

/**
 * Produce an event ID in the block of IDs reserved for audio
 * @param offset Index into audio event ID block; valid range [0,15]
 */
#define CHRE_AUDIO_EVENT_ID(offset)  (CHRE_EVENT_AUDIO_FIRST_EVENT + (offset))

/**
 * nanoappHandleEvent argument: struct chreAudioSourceStatusEvent
 *
 * Indicates a change in the format and/or rate of audio data provided to a
 * nanoapp.
 */
#define CHRE_EVENT_AUDIO_SAMPLING_CHANGE  CHRE_AUDIO_EVENT_ID(0)

/**
 * nanoappHandleEvent argument: struct chreAudioDataEvent
 *
 * Provides a buffer of audio data to a nanoapp.
 */
#define CHRE_EVENT_AUDIO_DATA  CHRE_AUDIO_EVENT_ID(1)

/**
 * The maximum size of the name of an audio source including the
 * null-terminator.
 */
#define CHRE_AUDIO_SOURCE_NAME_MAX_SIZE  (40)

/**
 * Helper values for sample rates.
 *
 * @defgroup CHRE_AUDIO_SAMPLE_RATES
 * @{
 */

//! 16kHz Audio Sample Data
#define CHRE_AUDIO_SAMPLE_RATE_16KHZ  (16000)

/** @} */

/**
 * Formats for audio that can be provided to a nanoapp.
 */
enum chreAudioDataFormat {
  /**
   * Unsigned, 8-bit u-Law encoded data as specified by ITU-T G.711.
   */
  CHRE_AUDIO_DATA_FORMAT_8_BIT_U_LAW = 0,

  /**
   * Signed, 16-bit linear PCM data. Endianness must be native to the local
   * processor.
   */
  CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM = 1,
};

/**
 * A description of an audio source available to a nanoapp.
 *
 * This provides a description of an audio source with a name and a
 * description of the format of the provided audio data.
 */
struct chreAudioSource {
  /**
   * A human readable name for this audio source. This is a C-style,
   * null-terminated string. The length must be less than or equal to
   * CHRE_AUDIO_SOURCE_NAME_MAX_SIZE bytes (including the null-terminator) and
   * is expected to describe the source of the audio in US English. All
   * characters must be printable (i.e.: isprint would return true for all
   * characters in the name for the EN-US locale). The typical use of this field
   * is for a nanoapp to log the name of the audio source that it is using.
   *
   * Example: "Camcorder Microphone"
   */
  const char *name;

  /**
   * The sampling rate in hertz of this mode. This value is rounded to the
   * nearest integer. Typical values might include 16000, 44100 and 44800.
   *
   * If the requested audio source is preempted by another feature of the system
   * (e.g. hotword), a gap may occur in received audio data. This is indicated
   * to the client by posting a CHRE_EVENT_AUDIO_SAMPLING_CHANGE event. The
   * nanoapp will then receive another CHRE_EVENT_AUDIO_SAMPLING_CHANGE event
   * once the audio source is available again.
   */
  uint32_t sampleRate;

  /**
   * The minimum amount of time that this audio source can be buffered, in
   * nanoseconds. Audio data is delivered to nanoapps in buffers. This specifies
   * the minimum amount of data that can be delivered to a nanoapp without
   * losing data. A request for a buffer that is smaller than this will fail.
   */
  uint64_t minBufferDuration;

  /**
   * The maximum amount of time that this audio source can be buffered, in
   * nanoseconds. Audio data is delivered to nanoapps in buffers. This specifies
   * the maximum amount of data that can be stored by the system in one event
   * without losing data. A request for a buffer that is larger than this will
   * fail.
   */
  uint64_t maxBufferDuration;

  /**
   * The format for data provided to the nanoapp. This will be assigned to one
   * of the enum chreAudioDataFormat values.
   */
  uint8_t format;
};

/**
 * The current status of an audio source.
 */
struct chreAudioSourceStatus {
  /**
   * Set to true if the audio source is currently enabled by this nanoapp. If
   * this struct is provided by a CHRE_EVENT_AUDIO_SAMPLING_CHANGE event, it
   * must necessarily be set to true because sampling change events are only
   * sent for sources which this nanoapp has actively subscribed to. If this
   * struct is obtained from the chreAudioGetStatus API, it may be set to true
   * or false depending on if audio is currently enabled.
   */
  bool enabled;

  /**
   * Set to true if the audio source is currently suspended and no audio data
   * will be received from this source.
   */
  bool suspended;
};

/**
 * The nanoappHandleEvent argument for CHRE_EVENT_AUDIO_SAMPLING_CHANGE.
 */
struct chreAudioSourceStatusEvent {
  /**
   * The audio source which has completed a status change.
   */
  uint32_t handle;

  /**
   * The status of this audio source.
   */
  struct chreAudioSourceStatus status;
};

/**
 * The nanoappHandleEvent argument for CHRE_EVENT_AUDIO_DATA.
 *
 * One example of the sequence of events for a nanoapp to receive audio data is:
 *
 * 1. CHRE_EVENT_AUDIO_SAMPLING_CHANGE - Indicates that audio data is not
 *                                       suspended.
 * 2. CHRE_EVENT_AUDIO_DATA - One buffer of audio samples. Potentially repeated.
 * 3. CHRE_EVENT_AUDIO_SAMPLING_CHANGE - Indicates that audio data has suspended
 *                                       which indicates a gap in the audio.
 * 4. CHRE_EVENT_AUDIO_SAMPLING_CHANGE - Indicates that audio data has resumed
 *                                       and that audio data may be delivered
 *                                       again if enough samples are buffered.
 * 5. CHRE_EVENT_AUDIO_DATA - One buffer of audio samples. Potentially repeated.
 *                            The nanoapp must tolerate a gap in the timestamps.
 *
 * This process repeats for as long as an active request is made for an audio
 * source. A CHRE_EVENT_AUDIO_SAMPLING_CHANGE does not guarantee that the next
 * event will be a CHRE_EVENT_AUDIO_DATA event when suspended is set to false.
 * It may happen that the audio source is suspended before a complete buffer can
 * be captured. This will cause another CHRE_EVENT_AUDIO_SAMPLING_CHANGE event
 * to be dispatched with suspended set to true before a buffer is delivered.
 *
 * Audio events must be delivered to a nanoapp in order.
 */
struct chreAudioDataEvent {
  /**
   * Indicates the version of the structure, for compatibility purposes. Clients
   * do not normally need to worry about this field; the CHRE implementation
   * guarantees that the client only receives the structure version it expects.
   */
  uint8_t version;

  /**
   * Additional bytes reserved for future use; must be set to 0.
   */
  uint8_t reserved[3];

  /**
   * The handle for which this audio data originated from.
   */
  uint32_t handle;

  /**
   * The base timestamp for this buffer of audio data, from the same time base
   * as chreGetTime() (in nanoseconds). The audio API does not provide
   * timestamps for each audio sample. This timestamp corresponds to the first
   * sample of the buffer. Even though the value is expressed in nanoseconds,
   * there is an expectation that the sample clock may drift and nanosecond
   * level accuracy may not be possible. The goal is to be as accurate as
   * possible within reasonable limitations of a given system.
   */
  uint64_t timestamp;

  /**
   * The sample rate for this buffer of data in hertz, rounded to the nearest
   * integer. Fractional sampling rates are not supported. Typical values might
   * include 16000, 44100 and 48000.
   */
  uint32_t sampleRate;

  /**
   * The number of samples provided with this buffer.
   */
  uint32_t sampleCount;

  /**
   * The format of this audio data. This enumeration and union of pointers below
   * form a tagged struct. The consumer of this API must use this enum to
   * determine which samples pointer below to dereference. This will be assigned
   * to one of the enum chreAudioDataFormat values.
   */
  uint8_t format;

  /**
   * A union of pointers to various formats of sample data. These correspond to
   * the valid chreAudioDataFormat values.
   */
  union {
    const uint8_t *samplesULaw8;
    const int16_t *samplesS16;
  };
};

/**
 * Retrieves information about an audio source supported by the current CHRE
 * implementation. The source returned by the runtime must not change for the
 * entire lifecycle of the Nanoapp and hot-pluggable audio sources are not
 * supported.
 *
 * A simple example of iterating all available audio sources is provided here:
 *
 * struct chreAudioSource audioSource;
 * for (uint32_t i = 0; chreAudioGetSource(i, &audioSource); i++) {
 *     chreLog(CHRE_LOG_INFO, "Found audio source: %s", audioSource.name);
 * }
 *
 * Handles provided to this API must be a stable value for the entire duration
 * of a nanoapp. Handles for all audio sources must be zero-indexed and
 * contiguous. The following are examples of handles that could be provided to
 * this API:
 *
 *   Valid: 0
 *   Valid: 0, 1, 2, 3
 * Invalid: 1, 2, 3
 * Invalid: 0, 2
 *
 * @param handle The handle for an audio source to obtain details for. The
 *     range of acceptable handles must be zero-indexed and contiguous.
 * @param audioSource A struct to populate with details of the audio source.
 * @return true if the query was successful, false if the provided handle is
 *     invalid or the supplied audioSource is NULL.
 *
 * @since v1.2
 */
bool chreAudioGetSource(uint32_t handle, struct chreAudioSource *audioSource);

/**
 * Configures delivery of audio data to the current nanoapp. Note that this may
 * not fully disable the audio source if it is used by other clients in the
 * system but it will halt data delivery to the nanoapp.
 *
 * The bufferDuration and deliveryInterval parameters as described below are
 * used together to determine both how much and how often to deliver data to a
 * nanoapp, respectively. A nanoapp will always be provided the requested
 * amount of data at the requested interval, even if another nanoapp in CHRE
 * requests larger/more frequent buffers or smaller/less frequent buffers.
 * These two buffering parameters allow describing the duty cycle of captured
 * audio data. If a nanoapp wishes to receive all available audio data, it will
 * specify a bufferDuration and deliveryInterval that are equal. A 50% duty
 * cycle would be achieved by specifying a deliveryInterval that is double the
 * value of the bufferDuration provided. These parameters allow the audio
 * subsystem to operate at less than 100% duty cycle and permits use of
 * incomplete audio data without periodic reconfiguration of the source.
 *
 * Two examples are illustrated below:
 *
 * Target duty cycle: 50%
 * bufferDuration:    2
 * deliveryInterval:  4
 *
 * Time       0   1   2   3   4   5   6   7
 * Batch                  A               B
 * Sample    --  --  a1  a2  --  --  b1  b2
 * Duration          [    ]          [    ]
 * Interval  [            ]  [            ]
 *
 *
 * Target duty cycle: 100%
 * bufferDuration:    4
 * deliveryInterval:  4
 *
 * Time       0   1   2   3   4   5   6   7
 * Batch                  A               B
 * Sample    a1  a2  a3  a4  b1  b2  b3  b4
 * Duration  [            ]  [            ]
 * Interval  [            ]  [            ]
 *
 *
 * This is expected to reduce power overall.
 *
 * The first audio buffer supplied to the nanoapp may contain data captured
 * prior to the request. This could happen if the microphone was already enabled
 * and reading into a buffer prior to the nanoapp requesting audio data for
 * itself. The nanoapp must tolerate this.
 *
 * It is important to note that multiple logical audio sources (e.g. different
 * sample rate, format, etc.) may map to one physical audio source. It is
 * possible for a nanoapp to request audio data from more than one logical
 * source at a time. Audio data may be suspended for either the current or other
 * requests. The CHRE_EVENT_AUDIO_SAMPLING_CHANGE will be posted to all clients
 * if such a change occurs. It is also possible for the request to succeed and
 * all audio sources are serviced simultaneously. This is implementation defined
 * but at least one audio source must function correctly if it is advertised,
 * under normal conditions (e.g. not required for some other system function,
 * such as hotword).
 *
 * @param handle The handle for this audio source. The handle for the desired
 *     audio source can be determined using chreAudioGetSource().
 * @param enable true if enabling the source, false otherwise. When passed as
 *     false, the bufferDuration and deliveryInterval parameters are ignored.
 * @param bufferDuration The amount of time to capture audio samples from this
 *     audio source, in nanoseconds per delivery interval. This value must be
 *     in the range of minBufferDuration/maxBufferDuration for this source or
 *     the request will fail. The number of samples captured per buffer will be
 *     derived from the sample rate of the source and the requested duration and
 *     rounded down to the nearest sample boundary.
 * @param deliveryInterval Desired time between each CHRE_EVENT_AUDIO_DATA
 *     event. This allows specifying the commplete duty cycle of a request
 *     for audio data, in nanoseconds. This value must be greater than or equal
 *     to bufferDuration or the request will fail due to an invalid
 *     configuration.
 * @return true if the configuration was successful, false if invalid parameters
 *     were provided (non-existent handle, invalid buffering configuration).
 *
 * @since v1.2
 */
bool chreAudioConfigureSource(uint32_t handle, bool enable,
                              uint64_t bufferDuration,
                              uint64_t deliveryInterval);

/**
 * Gets the current chreAudioSourceStatus struct for a given audio handle.
 *
 * @param handle The handle for the audio source to query. The provided handle
 *     is obtained from a chreAudioSource which is requested from the
 *     chreAudioGetSource API.
 * @param status The current status of the supplied audio source.
 * @return true if the provided handle is valid and the status was obtained
 *     successfully, false if the handle was invalid or status is NULL.
 *
 * @since v1.2
 */
bool chreAudioGetStatus(uint32_t handle, struct chreAudioSourceStatus *status);

#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_AUDIO_H_ */
