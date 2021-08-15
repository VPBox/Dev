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

#ifndef CHRE_PLATFORM_SHARED_NANOAPP_SUPPORT_LIB_DSO_H_
#define CHRE_PLATFORM_SHARED_NANOAPP_SUPPORT_LIB_DSO_H_

/**
 * @file
 * This provides the interface that the dynamic shared object (DSO) nanoapp
 * nanoapp support library (NSL) uses to interface with the underlying CHRE
 * implementation in a compatible manner.
 *
 * This header file must retain compatibility with C, and have minimal or no
 * dependencies on other CHRE system header files, as it will be used when
 * compiling external/dynamic nanoapps.
 */

#include "chre/util/entry_points.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Special magic value to uniquely identify the nanoapp info structure
#define CHRE_NSL_NANOAPP_INFO_MAGIC  UINT32_C(0x50e69977)

//! The minor version in the nanoapp info structure to determine which fields
//! are available to support backwards compatibility.
#define CHRE_NSL_NANOAPP_INFO_STRUCT_MINOR_VERSION  UINT8_C(2)

//! The symbol name expected from the nanoapp's definition of its info struct
#define CHRE_NSL_DSO_NANOAPP_INFO_SYMBOL_NAME  "_chreNslDsoNanoappInfo"

//! Maximum length of vendor and name strings
#define CHRE_NSL_DSO_NANOAPP_STRING_MAX_LEN  (32)

/**
 * DSO-based nanoapps must expose this struct under a symbol whose name is given
 * by CHRE_NSL_DSO_NANOAPP_INFO_SYMBOL_NAME. When the nanoapp is loaded, dlsym()
 * will be used to locate this symbol to register the nanoapp with the system.
 */
struct chreNslNanoappInfo {
  //! @see CHRE_NSL_NANOAPP_INFO_MAGIC
  uint32_t magic;

  //! @see CHRE_NSL_NANOAPP_INFO_STRUCT_MINOR_VERSION
  uint8_t structMinorVersion;

  //! Set to 1 if this nanoapp is a "system nanoapp" that should not show up in
  //! the context hub HAL, likely because it implements some device
  //! functionality beneath the HAL.
  uint8_t isSystemNanoapp:1;

  //! Set to 1 if this nanoapp runs in tightly coupled memory. This flag is only
  //! relevant to platforms that have the ability to run nanoapps within tightly
  //! coupled memory.
  //!
  //! @since minor version 1
  uint8_t isTcmNanoapp:1;

  //! Reserved for future use, set to 0. Assignment of this field to some use
  //! must be accompanied by an increase of the struct minor version.
  uint8_t reservedFlags:6;
  uint8_t reserved;

  //! The CHRE API version that the nanoapp was compiled against
  uint32_t targetApiVersion;

  //! A human-friendly name of the nanoapp vendor (null-terminated string,
  //! maximum length CHRE_NSL_DSO_NANOAPP_STRING_MAX_LEN)
  const char *vendor;

  //! A human-friendly name for the nanoapp (null-terminated string, maximum
  //! length CHRE_NSL_DSO_NANOAPP_STRING_MAX_LEN)
  const char *name;

  //! Identifies the vendor (most significant 5 bytes) and application
  uint64_t appId;

  //! Application-specific version number
  uint32_t appVersion;

  struct {
    chreNanoappStartFunction *start;
    chreNanoappHandleEventFunction *handleEvent;
    chreNanoappEndFunction *end;
  } entryPoints;

  //! Application-specific verison string. This might contain a commit hash at
  //! which the app was built, but is up to the app itself.
  //!
  //! @since minor version 2
  const char *appVersionString;
};

/**
 * Defined as a placeholder to enable future functionality extension.
 *
 * @param apiId
 * @param apiHandle If this function returns true, this will be set to a pointer
 *        to the associated structure containing the API
 *
 * @return true if the requested API is supported, false otherwise
 */
bool chreNslDsoGetApi(uint32_t apiId, void **apiHandle);

#ifdef __cplusplus
}
#endif

#endif  // CHRE_PLATFORM_SHARED_NANOAPP_SUPPORT_LIB_DSO_H_
