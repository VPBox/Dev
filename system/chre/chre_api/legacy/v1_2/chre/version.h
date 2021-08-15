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

#ifndef _CHRE_VERSION_H_
#define _CHRE_VERSION_H_

/**
 * @file
 * Definitions and methods for the versioning of the Context Hub Runtime
 * Environment.
 *
 * The CHRE API versioning pertains to all header files in the CHRE API.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Value for version 0.1 of the Context Hub Runtime Environment API interface.
 *
 * This is a legacy version.  Version 1.0 is considered the first official
 * version of the API.
 *
 * @see CHRE_API_VERSION
 */
#define CHRE_API_VERSION_0_1  UINT32_C(0x00010000)

/**
 * Value for version 1.0 of the Context Hub Runtime Environment API interface.
 *
 * The version of the CHRE API which shipped with the Android Nougat release.
 *
 * @see CHRE_API_VERSION
 */
#define CHRE_API_VERSION_1_0  UINT32_C(0x01000000)

/**
 * Value for version 1.1 of the Context Hub Runtime Environment API interface.
 *
 * The version of the CHRE API shipped with the Android O release. It adds
 * initial support for new GNSS, WiFi, and WWAN modules.
 *
 * @see CHRE_API_VERSION
 */
#define CHRE_API_VERSION_1_1  UINT32_C(0x01010000)

/**
 * Value for version 1.2 of the Context Hub Runtime Environment API interface.
 *
 * The version of the CHRE API shipped with the Android P release. It adds
 * initial support for the new audio module.
 *
 * @see CHRE_API_VERSION
 */
#define CHRE_API_VERSION_1_2  UINT32_C(0x01020000)

/**
 * Major and Minor Version of this Context Hub Runtime Environment API.
 *
 * The major version changes when there is an incompatible API change.
 *
 * The minor version changes when there is an addition in functionality
 * in a backwards-compatible manner.
 *
 * We define the version number as an unsigned 32-bit value.  The most
 * significant byte is the Major Version.  The second-most significant byte
 * is the Minor Version.  The two least significant bytes are the Patch
 * Version.  The Patch Version is not defined by this header API, but
 * is provided by a specific CHRE implementation (see chreGetVersion()).
 *
 * Note that version numbers can always be numerically compared with
 * expected results, so 1.0.0 < 1.0.4 < 1.1.0 < 2.0.300 < 3.5.0.
 */
#define CHRE_API_VERSION CHRE_API_VERSION_1_2

/**
 * Utility macro to extract only the API major version of a composite CHRE
 * version.
 *
 * @param version A uint32_t version, e.g. the value returned by
 *     chreGetApiVersion()
 *
 * @returns The API major version in the least significant byte, e.g. 0x01
 */
#define CHRE_EXTRACT_MAJOR_VERSION(version) \
    (((version) & UINT32_C(0xFF000000)) >> 24)

/**
 * Utility macro to extract only the API minor version of a composite CHRE
 * version.
 *
 * @param version A uint32_t version, e.g. the CHRE_API_VERSION constant
 *
 * @returns The API minor version in the least significant byte, e.g. 0x01
 */
#define CHRE_EXTRACT_MINOR_VERSION(version) \
    (((version) & UINT32_C(0x00FF0000)) >> 16)

/**
 * Utility macro to extract only the API minor version of a composite CHRE
 * version.
 *
 * @param version A complete uint32_t version, e.g. the value returned by
 *     chreGetVersion()
 *
 * @returns The implementation patch version in the least significant two bytes,
 *     e.g. 0x0123, with all other bytes set to 0
 */
#define CHRE_EXTRACT_PATCH_VERSION(version)  ((version) & UINT32_C(0xFFFF))


/**
 * Get the API version the CHRE implementation was compiled against.
 *
 * This is not necessarily the CHRE_API_VERSION in the header the nanoapp was
 * built against, and indeed may not have even appeared in the context_hub_os.h
 * header which this nanoapp was built against.
 *
 * By definition, this will have the two least significant bytes set to 0,
 * and only contain the major and minor version number.
 *
 * @returns The API version.
 */
uint32_t chreGetApiVersion(void);

/**
 * Get the version of this CHRE implementation.
 *
 * By definition, ((chreGetApiVersion() & UINT32_C(0xFFFF0000)) ==
 *                 (chreGetVersion()    & UINT32_C(0xFFFF0000))).
 *
 * The Patch Version, in the lower two bytes, only have meaning in context
 * of this specific platform ID.  It is increased by the platform every time
 * a backwards-compatible bug fix is released.
 *
 * @returns The version.
 *
 * @see chreGetPlatformId()
 */
uint32_t chreGetVersion(void);

/**
 * Get the Platform ID of this CHRE.
 *
 * The most significant five bytes are the vendor ID as set out by the
 * NANOAPP_VENDOR convention in the original context hub HAL header file
 * (context_hub.h), also used by nanoapp IDs.
 *
 * The least significant three bytes are set by the vendor, but must be
 * unique for each different CHRE implementation/hardware that the vendor
 * supplies.
 *
 * The idea is that in the case of known bugs in the field, a new nanoapp could
 * be shipped with a workaround that would use this value, and chreGetVersion(),
 * to have code that can conditionally work around the bug on a buggy version.
 * Thus, we require this uniqueness to allow such a setup to work.
 *
 * @returns The platform ID.
 *
 * @see CHRE_EXTRACT_VENDOR_ID
 */
uint64_t chreGetPlatformId(void);


#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_VERSION_H_ */
