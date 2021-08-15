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

#ifndef CHRE_PAL_VERSION_H_
#define CHRE_PAL_VERSION_H_

/**
 * @file
 * Shared definitions related to CHRE PAL versioning.
 *
 * Versioning guidelines
 *
 * Like the CHRE nanoapp API, CHRE PAL APIs are versioned by a 32-bit integer
 * where the most significant byte represents the major version, the next most
 * significant byte represents the minor version, and the two least significant
 * bytes represent the implementation patch version in little endian order.
 *
 * Semantic versioning guidelines are followed, such that a new major version
 * indicates a compatibility-breaking change was introduced, a new minor version
 * indicates that new functionality was added in a backwards-compatible way, and
 * a new patch version indicates bug fixes in the implementation. The patch
 * version does not apply to the API itself, only a particular implementation of
 * the API.
 *
 * Note that although the PALs are generally closely related to the CHRE API,
 * they are versioned independently. For example, new features may be added to
 * the PAL APIs to support CHRE core system requirements that do not impact the
 * CHRE API directly. Also, a change may be made to the CHRE API that only
 * results in a minor version change due to compatibility layers between CHRE
 * nanoapps and the CHRE system, however this may require a major version bump
 * to the PAL in order to implement it. In general, this is done to keep the PAL
 * APIs simpler, as the components of the CHRE system are more tightly coupled
 * than the apps that run on top of it and are typically updated at the same
 * time. So we would want to ensure that a given nanoapp can run on multiple
 * versions of the CHRE API, but it is less important for PAL modules to work
 * with multiple versions of the core CHRE implementation, and vice versa.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Extracts only the API version component of a module version
 */
#define CHRE_PAL_GET_API_VERSION(moduleVersion) \
    ((moduleVersion) & UINT32_C(0xFFFF0000))

/**
 * Extracts only the major API version component of a module version
 */
#define CHRE_PAL_GET_API_MAJOR_VERSION(moduleVersion) \
    (((moduleVersion) & UINT32_C(0xFF000000)) >> 24)

/**
 * Extracts only the module patch version of a module version
 */
#define CHRE_PAL_GET_PATCH_VERSION(moduleVersion) \
    ((moduleVersion) & UINT32_C(0x0000FFFF))

/**
 * Constructs an API version from major & minor patch versions
 *
 * @param major Major version, valid range 0-255
 * @param minor Minor version, valid range 0-255
 */
#define CHRE_PAL_CREATE_API_VERSION(major, minor) \
    ((uint32_t) ((((major) & 0xFF) << 24) | (((minor) & 0xFF) << 16)))

/**
 * Constructs a module version from a complete API version and a module patch
 * version
 */
#define CHRE_PAL_CREATE_MODULE_VERSION(apiVersion, patchVersion) \
    ((uint32_t) (CHRE_PAL_GET_API_VERSION(apiVersion) | \
                 CHRE_PAL_GET_PATCH_VERSION(patchVersion)))

/**
 * Determines if a requested CHRE API version is compatible with the supplied
 * API version
 */
#define CHRE_PAL_VERSIONS_ARE_COMPATIBLE(apiVersion, requestedApiVersion) \
    (CHRE_PAL_GET_API_MAJOR_VERSION(apiVersion) == \
        CHRE_PAL_GET_API_MAJOR_VERSION(requestedApiVersion))

#ifdef __cplusplus
}
#endif

#endif  // CHRE_VERSION_H_
