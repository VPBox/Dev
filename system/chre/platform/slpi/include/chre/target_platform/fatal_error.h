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

#ifndef CHRE_PLATFORM_SLPI_FATAL_ERROR_H_
#define CHRE_PLATFORM_SLPI_FATAL_ERROR_H_

#include "err.h"

#include "chre/util/macros.h"

#define FATAL_ERROR_QUIT() \
  ERR_FATAL("CHRE fatal@" CHRE_FILENAME ":" STRINGIFY(__LINE__), 0, 0, 0)

#endif  // CHRE_PLATFORM_SLPI_FATAL_ERROR_H_
