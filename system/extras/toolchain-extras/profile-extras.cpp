/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // For POSIX basename().

// Use _system_properties.h to use __system_property_wait_any()
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "profile-extras.h"

extern "C" {

void __gcov_flush(void);

static void gcov_signal_handler(__unused int signum) {
  __gcov_flush();
}

static const char kCoveragePropName[] = "debug.coverage.flush";

// In a loop, wait for any change to sysprops and trigger a __gcov_flush when
// <kCoveragePropName> sysprop transistions to "1" after a transistion to "0".
void *property_watch_loop(__unused void *arg) {
  uint32_t serial = 0;

  // __gcov_flush is called on a state transition from 0 to 1.  Initialize state
  // to 1 so a process spinning up when the sysprop is already set does not
  // immediately dump its coverage.
  int previous_state = 1;

  while (true) {
    // Use deprecated __system_property_wait_any for backward compatibility.
    serial = __system_property_wait_any(serial);
    const struct prop_info *pi = __system_property_find(kCoveragePropName);
    if (!pi)
      continue;

    char value[PROP_VALUE_MAX];
    __system_property_read(pi, nullptr, value);
    if (strcmp(value, "0") == 0) {
      previous_state = 0;
    } else if (strcmp(value, "1") == 0) {
      if (previous_state == 0) {
        __gcov_flush();
      }
      previous_state = 1;
    }
  }
}

// Initialize libprofile-extras:
// - Install a signal handler that triggers __gcov_flush on <GCOV_FLUSH_SIGNAL>.
// - Create a thread that calls __gcov_flush when <kCoveragePropName> sysprop
// transistions to "1" after a transistion to "0".
//
// We want this initiazlier to run during load time.
//
// Just marking init_profile_extras() with __attribute__((constructor)) isn't
// enough since the linker drops it from its output since no other symbol from
// this static library is referenced.
//
// We force the linker to include init_profile_extras() by passing
// '-uinit_profile_extras' to the linker (in build/soong).
__attribute__((constructor)) int init_profile_extras(void) {
  sighandler_t ret1 = signal(GCOV_FLUSH_SIGNAL, gcov_signal_handler);
  if (ret1 == SIG_ERR) {
    return -1;
  }

  // Do not create thread running property_watch_loop for zygote (it can get
  // invoked as zygote or app_process).  This check is only needed for the
  // platform, but can be done on any version after Android L, when
  // getprogname() was added.
#if defined(__ANDROID_API__) && __ANDROID_API__ >= __ANDROID_API_L__
  const char *prog_basename = basename(getprogname());
  if (strncmp(prog_basename, "zygote", strlen("zygote")) == 0) {
    return 0;
  }
  if (strncmp(prog_basename, "app_process", strlen("app_process")) == 0) {
    return 0;
  }
#endif

  pthread_t thread;
  int error = pthread_create(&thread, nullptr, property_watch_loop, nullptr);
  if (error != 0) {
    return -1;
  }
  return 0;
}
}
