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

#ifndef CHRE_PLATFORM_SLPI_FASTRPC_H
#define CHRE_PLATFORM_SLPI_FASTRPC_H

/**
 * @file
 * Shared definitions used in FastRPC communication with the host. This file is
 * included by both the SLPI-side and host-side code.
 */

#ifdef __cplusplus
extern "C" {
#endif

//! FastRPC mandates that return value of 0 indicates success. Any other value
//! is considered an error and will result in skipping transfer of output
//! parameters.
#define CHRE_FASTRPC_SUCCESS (0)

//! General failure.
#define CHRE_FASTRPC_ERROR (-1)

//! The call failed because CHRE is shutting down; the host should not call this
//! function again until CHRE is restarted.
#define CHRE_FASTRPC_ERROR_SHUTTING_DOWN (-2)

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // CHRE_PLATFORM_SLPI_FASTRPC_H
