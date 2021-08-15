//
//  Copyright 2015 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#define LOG_TAG "hal_util"

#include <base/logging.h>
#include <base/strings/stringprintf.h>
#include <hardware/bluetooth.h>

#include <dlfcn.h>
#include <errno.h>
#include <string.h>

#include "btcore/include/hal_util.h"
#include "osi/include/log.h"

using base::StringPrintf;

#define BLUETOOTH_LIBRARY_NAME "libbluetooth.so"

int hal_util_load_bt_library(const bt_interface_t** interface) {
  const char* sym = BLUETOOTH_INTERFACE_STRING;
  bt_interface_t* itf = nullptr;

  // Always try to load the default Bluetooth stack on GN builds.
  void* handle = dlopen(BLUETOOTH_LIBRARY_NAME, RTLD_NOW);
  if (!handle) {
    const char* err_str = dlerror();
    LOG(ERROR) << __func__ << ": failed to load bluetooth library, error="
               << (err_str ? err_str : "error unknown");
    goto error;
  }

  // Get the address of the bt_interface_t.
  itf = (bt_interface_t*)dlsym(handle, sym);
  if (!itf) {
    LOG(ERROR) << __func__ << ": failed to load symbol from Bluetooth library "
               << sym;
    goto error;
  }

  // Success.
  LOG(INFO) << __func__ << " loaded HAL path=" << BLUETOOTH_LIBRARY_NAME
            << " btinterface=" << itf << " handle=" << handle;

  *interface = itf;
  return 0;

error:
  *interface = NULL;
  if (handle) dlclose(handle);

  return -EINVAL;
}
