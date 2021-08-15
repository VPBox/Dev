/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <android-base/unique_fd.h>

#include "LineBuffer.h"
#include "NativeInfo.h"

// This function is not re-entrant since it uses a static buffer for
// the line data.
void GetNativeInfo(int smaps_fd, size_t* pss_bytes, size_t* va_bytes) {
  static char map_buffer[65535];
  LineBuffer line_buf(smaps_fd, map_buffer, sizeof(map_buffer));
  char* line;
  size_t total_pss_bytes = 0;
  size_t total_va_bytes = 0;
  size_t line_len;
  bool native_map = false;
  while (line_buf.GetLine(&line, &line_len)) {
    uintptr_t start, end;
    int name_pos;
    size_t native_pss_kB;
    if (sscanf(line, "%" SCNxPTR "-%" SCNxPTR " %*4s %*x %*x:%*x %*d %n",
        &start, &end, &name_pos) == 2) {
      if (strcmp(line + name_pos, "[anon:libc_malloc]") == 0 ||
          strcmp(line + name_pos, "[heap]") == 0) {
        total_va_bytes += end - start;
        native_map = true;
      } else {
        native_map = false;
      }
    } else if (native_map && sscanf(line, "Pss: %zu", &native_pss_kB) == 1) {
      total_pss_bytes += native_pss_kB * 1024;
    }
  }
  *pss_bytes = total_pss_bytes;
  *va_bytes = total_va_bytes;
}

void PrintNativeInfo(const char* preamble) {
  size_t pss_bytes;
  size_t va_bytes;

  android::base::unique_fd smaps_fd(open("/proc/self/smaps", O_RDONLY));
  if (smaps_fd == -1) {
    err(1, "Cannot open /proc/self/smaps: %s\n", strerror(errno));
  }

  GetNativeInfo(smaps_fd, &pss_bytes, &va_bytes);
  printf("%sNative PSS: %zu bytes %0.2fMB\n", preamble, pss_bytes, pss_bytes/(1024*1024.0));
  printf("%sNative VA Space: %zu bytes %0.2fMB\n", preamble, va_bytes, va_bytes/(1024*1024.0));
  fflush(stdout);
}
