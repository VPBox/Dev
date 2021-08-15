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

#include <chre.h>

#include <cinttypes>
#include <cstdio>
#include <cstdlib>

/**
 * @file
 *
 * This file supplies implementations for functions used by container_support.h
 * to allow CHRE containers to be used standalone.
 */

void *chreHeapAlloc(uint32_t bytes) {
  return malloc(bytes);
}

void chreHeapFree(void *ptr) {
  free(ptr);
}

void chreAbort(uint32_t abortCode) {
  // We do not use exit() here because it accepts a signed int and we really
  // want abort() semantics. Log the failure and abort().
  fprintf(stderr, "Aborting with code %" PRIu32 "\n", abortCode);
  abort();
}
