/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_PAL_WWAN_H_
#define CHRE_PAL_WWAN_H_

/**
 * @file
 * Defines the interface between the common CHRE core system and the
 * platform-specific WWAN module.
 */

#include <stdbool.h>
#include <stdint.h>

#include "chre_api/chre/wwan.h"
#include "chre/pal/system.h"
#include "chre/pal/version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initial version of the CHRE WWAN PAL, tied to CHRE API v1.1.
 */
#define CHRE_PAL_WWAN_API_V1_0  CHRE_PAL_CREATE_API_VERSION(1, 0)

/**
 * The version of the CHRE WWAN PAL defined in this header file.
 */
#define CHRE_PAL_WWAN_API_CURRENT_VERSION  CHRE_PAL_WWAN_API_V1_0

struct chrePalWwanCallbacks {
    /**
     * Callback invoked to provide the result of a prior request to
     * requestCellInfo in struct chrePalWwanApi.
     *
     * This function call passes ownership of the result's memory to the core
     * CHRE system, i.e. the PAL module must not modify or free the provided
     * data until the associated API function is called to release the memory.
     *
     * @param result
     *
     * @see chrePalWwanApi.requestCellInfo
     * @see chrePalWwanApi.releaseCellInfoResult
     */
    void (*cellInfoResultCallback)(struct chreWwanCellInfoResult *result);
};

struct chrePalWwanApi {
    /**
     * Version of the module providing this API. This value should be
     * constructed from CHRE_PAL_CREATE_MODULE_VERSION using the supported
     * API version constant (CHRE_PAL_WWAN_API_*) and the module-specific patch
     * version.
     */
    uint32_t moduleVersion;

    /**
     * Initializes the WWAN module. Initialization must complete synchronously.
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
                 const struct chrePalWwanCallbacks *callbacks);

    /**
     * Performs clean shutdown of the WWAN module, usually done in preparation
     * for stopping the CHRE. The WWAN module must ensure that it will not
     * invoke any callbacks past this point, and complete any relevant teardown
     * activities before returning from this function.
     */
    void (*close)(void);

    /**
     * Retrieves information about the features supported by this module. The
     * value returned from this function must not change for the duration of
     * execution.
     *
     * @return See chreWwanGetCapabilities()
     *
     * @see chreWwanGetCapabilities()
     */
    uint32_t (*getCapabilities)(void);

    /**
     * Initiates a request for information about the current serving cell and
     * its neighbors.
     *
     * @return true if the request was accepted, in which case a subsequent call
     *         to cellInfoResultCallback will be used to communicate the result
     *
     * @see chreWwanGetCellInfoAsync()
     * @see chrePalWwanCallbacks.cellInfoResultCallback
     */
    bool (*requestCellInfo)(void);

    /**
     * Invoked when the core CHRE system no longer needs a raw measurement event
     * structure that was provided to it via measurementEventCallback(). The
     * GNSS module may use this to free associated memory, etc.
     *
     * @param result Result data to release
     */
    void (*releaseCellInfoResult)(struct chreWwanCellInfoResult *result);
};

/**
 * Retrieve a handle for the CHRE WWAN PAL.
 *
 * @param requestedApiVersion The implementation of this function must return a
 *        pointer to a structure with the same major version as requested.
 *
 * @return Pointer to API handle, or NULL if a compatible API version is not
 *         supported by the module, or the API as a whole is not implemented. If
 *         non-NULL, the returned API handle must be valid as long as this
 *         module is loaded.
 */
const struct chrePalWwanApi *chrePalWwanGetApi(uint32_t requestedApiVersion);

#ifdef __cplusplus
}
#endif

#endif  // CHRE_PAL_WWAN_H_
