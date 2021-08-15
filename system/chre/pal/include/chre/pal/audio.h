/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef CHRE_PAL_AUDIO_H_
#define CHRE_PAL_AUDIO_H_

#include <stdbool.h>
#include <stdint.h>

#include "chre_api/chre/audio.h"
#include "chre/pal/system.h"
#include "chre/pal/version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initial version of the CHRE audio PAL, tied to CHRE API v1.2.
 */
#define CHRE_PAL_AUDIO_API_V1_2  CHRE_PAL_CREATE_API_VERSION(1, 2)

// v1.0 and v1.1 skipped to avoid confusion with older versions of the CHRE API.

#define CHRE_PAL_AUDIO_API_CURRENT_VERSION CHRE_PAL_AUDIO_API_V1_2

struct chrePalAudioCallbacks {
    /**
     * Callback used to pass audio data events to the core CHRE system, which
     * distributes it to clients (nanoapps). These events are only delivered
     * while an audio request is open.
     *
     * This function call passes ownership of the event memory to the core CHRE
     * system, i.e. the PAL module must not modify the referenced data until the
     * associated API function is called to release the memory.
     *
     * @param event Event data to distribute to clients. The audio module
     *        must ensure that this memory remains accessible until it is passed
     *        to the releaseAudioDataEvent function in struct chrePalAudioApi.
     *
     */
    void (*audioDataEventCallback)(struct chreAudioDataEvent *event);

    /**
     * Callback used to notify that audio availability for a specific handle has
     * changed.
     *
     * @param handle The audio source handle for which audio availability has
     *        changed.
     * @param available true when the source is available to provide audio
     *        data events, false otherwise.
     */
    void (*audioAvailabilityCallback)(uint32_t handle, bool available);
};

struct chrePalAudioApi {
    /**
     * Version of the module providing this API. This value should be
     * constructed from CHRE_PAL_CREATE_MODULE_VERSION using the supported
     * API version constant (CHRE_PAL_AUDIO_API_*) and the module-specific patch
     * version.
     */
    uint32_t moduleVersion;

    /**
     * Initializes the audio module. Initialization must complete synchronously.
     *
     * @param systemApi Structure containing CHRE system function pointers which
     *        the PAL implementation should prefer to use over equivalent
     *        functionality exposed by the underlying platform. The module does
     *        not need to deep-copy this structure; its memory remains
     *        accessible at least until after close() is called.
     * @param callbacks Structure containing entry points to the core CHRE
     *        system. The module does not need to deep-copy this structure; its
     *        memory remains accessible at least until after close() is called.
     *
     * @return true if initialization was successful, false otherwise
     */
    bool (*open)(const struct chrePalSystemApi *systemApi,
                 const struct chrePalAudioCallbacks *callbacks);

    /**
     * Performs clean shutdown of the audio module, usually done in preparation
     * for stopping the CHRE. The audio module must end any active requests to
     * ensure that it will not invoke any callbacks past this point, and
     * complete any relevant teardown activities before returning from this
     * function.
     */
    void (*close)(void);

    /**
     * Requests an audio data event from the platform for the provided handle. A
     * call to this method must cancel any previous request.
     *
     * The event requested here may contain data from previously posted events.
     * The concept is to allow the platform to manage its own buffers for audio
     * data. If a request comes in for 8000 samples of data and the most recent
     * request was for 4000 samples of data, the platform implementation may
     * reuse the existing 4000 samples of data and append 4000 samples of new
     * data (assuming that the arguments passed here allow that).
     *
     * Once a request for a given source has been made, the platform
     * implementation must maintain a buffer of previously collected audio
     * samples to provide when a request comes in for data in the past (up to
     * the maximum buffer size for this source). This happens when numSamples at
     * the source sample rate is a greater amount of time than eventDelay. This
     * buffer can be released once cancelAudioDataEventRequest has been invoked
     * for a given source.
     *
     * The event is provided to the client through the audioDataEventCallback.
     *
     * @param handle The handle for which an audio event is requested.
     * @param numSamples The number of samples to send once the request has been
     *        completed.
     * @param eventDelayNs The amount of time that must pass before providing
     *        the data event via the audioDataEventCallback.
     */
    bool (*requestAudioDataEvent)(uint32_t handle, uint32_t numSamples,
                                  uint64_t eventDelayNs);

    /**
     * Cancels the previous call to requestAudioDataEvent. No audio data is
     * allowed to be posted to CHRE after this function has been called and
     * before the next call to requestAudioDataEvent for the supplied handle.
     *
     * @param handle The handle for which the most recent call to
     *        requestAudioDataEvent will be cancelled.
     */
    void (*cancelAudioDataEvent)(uint32_t handle);

    /**
     * Releases a previously posted audio event. This will be invoked by CHRE to
     * say that all nanoapps have processed the previously posted data event.
     *
     * @param event An audio data event that was previously provided to
     *        CHRE as a result of a request for audio data.
     */
    void (*releaseAudioDataEvent)(struct chreAudioDataEvent *event);

    /**
     * @return the number of sources supported by the implementation. The
     * returned value must be exactly one greater than the maximum supported
     * audio handle.
     */
    uint32_t (*getSourceCount)(void);

    /**
     * Obtains the audio source description for a given handle.
     *
     * @param handle the handle for the requested audio source.
     * @param audioSource the chreAudioSource to populate with details of the
     *     audio source. This pointer must never be null.
     */
    bool (*getAudioSource)(uint32_t handle,
                           struct chreAudioSource *audioSource);
};

/**
 * Retrieve a handle for the CHRE audio PAL.
 *
 * @param requestedApiVersion The implementation of this function must return a
 *        pointer to a structure with the same major version as requested.
 *
 * @return Pointer to API handle, or NULL if a compatible API version is not
 *         supported by the module, or the API as a whole is not implemented. If
 *         non-NULL, the returned API handle must be valid as long as this
 *         module is loaded.
 */
const struct chrePalAudioApi *chrePalAudioGetApi(uint32_t requestedApiVersion);

#ifdef __cplusplus
}
#endif

#endif  // CHRE_PAL_AUDIO_H_
