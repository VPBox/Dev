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

#ifndef _CHRE_COMMON_H_
#define _CHRE_COMMON_H_

/**
 * @file
 * Definitions shared across multiple CHRE header files
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Mask of the 5 most significant bytes in a 64-bit nanoapp or CHRE platform
 * identifier, which represents the vendor ID portion of the ID.
 */
#define CHRE_VENDOR_ID_MASK  UINT64_C(0xFFFFFFFFFF000000)

/**
 * Vendor ID "Googl".  Used in nanoapp IDs and CHRE platform IDs developed and
 * released by Google.
 */
#define CHRE_VENDOR_ID_GOOGLE  UINT64_C(0x476F6F676C000000)

/**
 * Vendor ID "GoogT".  Used for nanoapp IDs associated with testing done by
 * Google.
 */
#define CHRE_VENDOR_ID_GOOGLE_TEST  UINT64_C(0x476F6F6754000000)

/**
 * Helper macro to mask off all bytes other than the vendor ID (most significant
 * 5 bytes) in 64-bit nanoapp and CHRE platform identifiers.
 *
 * @see chreGetNanoappInfo()
 * @see chreGetPlatformId()
 */
#define CHRE_EXTRACT_VENDOR_ID(id)  ((id) & CHRE_VENDOR_ID_MASK)

/**
 * Number of nanoseconds in one second, represented as an unsigned 64-bit
 * integer
 */
#define CHRE_NSEC_PER_SEC  UINT64_C(1000000000)

/**
 * General timeout for asynchronous API requests. Unless specified otherwise, a
 * function call that returns data asynchronously via an event, such as
 * CHRE_EVENT_ASYNC_GNSS_RESULT, must do so within this amount of time.
 */
#define CHRE_ASYNC_RESULT_TIMEOUT_NS  (5 * CHRE_NSEC_PER_SEC)


/**
 * A generic listing of error codes for use in {@link #chreAsyncResult} and
 * elsewhere. In general, module-specific error codes may be added to this enum,
 * but effort should be made to come up with a generic name that still captures
 * the meaning of the error.
 */
enum chreError {
    //! No error occurred
    CHRE_ERROR_NONE = 0,

    //! An unspecified failure occurred
    CHRE_ERROR = 1,

    //! One or more supplied arguments are invalid
    CHRE_ERROR_INVALID_ARGUMENT = 2,

    //! Unable to satisfy request because the system is busy
    CHRE_ERROR_BUSY = 3,

    //! Unable to allocate memory
    CHRE_ERROR_NO_MEMORY = 4,

    //! The requested feature is not supported
    CHRE_ERROR_NOT_SUPPORTED = 5,

    //! A timeout occurred while processing the request
    CHRE_ERROR_TIMEOUT = 6,

    //! The relevant capability is disabled, for example due to a user
    //! configuration that takes precedence over this request
    CHRE_ERROR_FUNCTION_DISABLED = 7,

    //! The request was rejected due to internal rate limiting of the requested
    //! functionality - the client may try its request again after waiting an
    //! unspecified amount of time
    CHRE_ERROR_REJECTED_RATE_LIMIT = 8,

    //! The requested functionality is not currently accessible from the CHRE,
    //! because another master, such as the main applications processor, is
    //! currently controlling it.
    CHRE_ERROR_FUNCTION_RESTRICTED_TO_OTHER_MASTER = 9,

    //!< Do not exceed this value when adding new error codes
    CHRE_ERROR_LAST = UINT8_MAX,
};

/**
 * Generic data structure to indicate the result of an asynchronous operation.
 *
 * @note
 * The general model followed by CHRE for asynchronous operations is that a
 * request function returns a boolean value that indicates whether the request
 * was accepted for further processing. The actual result of the operation is
 * provided in a subsequent event sent with an event type that is defined in the
 * specific API. Typically, a "cookie" parameter is supplied to allow the client
 * to tie the response to a specific request, or pass data through, etc. The
 * response is expected to be delivered within CHRE_ASYNC_RESULT_TIMEOUT_NS if
 * not specified otherwise.
 *
 * The CHRE implementation must allow for multiple asynchronous requests to be
 * outstanding at a given time, under reasonable resource constraints. Further,
 * requests must be processed in the same order as supplied by the client of the
 * API in order to maintain causality. Using GNSS as an example, if a client
 * calls chreGnssLocationSessionStartAsync() and then immediately calls
 * chreGnssLocationSessionStopAsync(), the final result must be that the
 * location session is stopped. Whether requests always complete in the
 * order that they are given is implementation-defined. For example, if a client
 * calls chreGnssLocationSessionStart() and then immediately calls
 * chreGnssMeasurementSessionStart(), it is possible for the
 * CHRE_EVENT_GNSS_RESULT associated with the measurement session to be
 * delivered before the one for the location session.
 */
struct chreAsyncResult {
    //! Indicates the request associated with this result. The interpretation of
    //! values in this field is dependent upon the event type provided when this
    //! result was delivered.
    uint8_t requestType;

    //! Set to true if the request was successfully processed
    bool success;

    //! If the request failed (success is false), this is set to a value from
    //! enum chreError (other than CHRE_ERROR_NONE), which may provide
    //! additional information about the nature of the failure.
    //! @see #chreError
    uint8_t errorCode;

    //! Reserved for future use, set to 0
    uint8_t reserved;

    //! Set to the cookie parameter given to the request function tied to this
    //! result
    const void *cookie;
};


#ifdef __cplusplus
}
#endif

#endif /* _CHRE_COMMON_H_ */
