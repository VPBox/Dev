/*
 * Copyright 2017 The Android Open Source Project
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

#define LOG_TAG "a2dp_vendor_ldac_abr"

#include "a2dp_vendor_ldac_abr.h"

#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "osi/include/log.h"

//
// LDAC ABR(Adaptive Bit Rate) Source Code
//

//
// The LDAC ABR shared library, and the functions to use
//
static const char* LDAC_ABR_LIB_NAME = "libldacBT_abr.so";
static void* ldac_abr_lib_handle = NULL;

static const char* LDAC_ABR_GET_HANDLE_NAME = "ldac_ABR_get_handle";
typedef HANDLE_LDAC_ABR (*tLDAC_ABR_GET_HANDLE)(void);

static const char* LDAC_ABR_FREE_HANDLE_NAME = "ldac_ABR_free_handle";
typedef void (*tLDAC_ABR_FREE_HANDLE)(HANDLE_LDAC_ABR hLdacAbr);

static const char* LDAC_ABR_INIT_NAME = "ldac_ABR_Init";
typedef int (*tLDAC_ABR_INIT)(HANDLE_LDAC_ABR hLdacAbr,
                              unsigned int interval_ms);

static const char* LDAC_ABR_SET_THRESHOLDS_NAME = "ldac_ABR_set_thresholds";
typedef int (*tLDAC_ABR_SET_THRESHOLDS)(HANDLE_LDAC_ABR hLdacAbr,
                                        unsigned int th_critical,
                                        unsigned int th_dangerous_trend,
                                        unsigned int th_safety_4hqsq);

static const char* LDAC_ABR_PROC_NAME = "ldac_ABR_Proc";
typedef int (*tLDAC_ABR_PROC)(HANDLE_LDAC_BT hLdacParam,
                              HANDLE_LDAC_ABR hLdacAbr, unsigned int txq_length,
                              unsigned int flag_enable);

static tLDAC_ABR_GET_HANDLE ldac_abr_get_handle_func;
static tLDAC_ABR_FREE_HANDLE ldac_abr_free_handle_func;
static tLDAC_ABR_INIT ldac_abr_init_func;
static tLDAC_ABR_SET_THRESHOLDS ldac_abr_set_thresholds_func;
static tLDAC_ABR_PROC ldac_abr_proc_func;

bool A2DP_VendorLoadLdacAbr(void) {
  if (ldac_abr_lib_handle != NULL) return true;  // Already loaded

  // Open the LDAC ABR library
  ldac_abr_lib_handle = dlopen(LDAC_ABR_LIB_NAME, RTLD_NOW);
  if (ldac_abr_lib_handle == NULL) {
    LOG_ERROR(LOG_TAG, "%s: cannot open LDAC ABR library %s: %s", __func__,
              LDAC_ABR_LIB_NAME, dlerror());
    A2DP_VendorUnloadLdacAbr();
    return false;
  }

  // Load all functions
  ldac_abr_get_handle_func = (tLDAC_ABR_GET_HANDLE)dlsym(
      ldac_abr_lib_handle, LDAC_ABR_GET_HANDLE_NAME);
  if (ldac_abr_get_handle_func == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the LDAC ABR library: %s",
              __func__, LDAC_ABR_GET_HANDLE_NAME, dlerror());
    A2DP_VendorUnloadLdacAbr();
    return false;
  }

  ldac_abr_free_handle_func = (tLDAC_ABR_FREE_HANDLE)dlsym(
      ldac_abr_lib_handle, LDAC_ABR_FREE_HANDLE_NAME);
  if (ldac_abr_free_handle_func == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the LDAC ABR library: %s",
              __func__, LDAC_ABR_FREE_HANDLE_NAME, dlerror());
    A2DP_VendorUnloadLdacAbr();
    return false;
  }

  ldac_abr_init_func =
      (tLDAC_ABR_INIT)dlsym(ldac_abr_lib_handle, LDAC_ABR_INIT_NAME);
  if (ldac_abr_init_func == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the LDAC ABR library: %s",
              __func__, LDAC_ABR_INIT_NAME, dlerror());
    A2DP_VendorUnloadLdacAbr();
    return false;
  }

  ldac_abr_set_thresholds_func = (tLDAC_ABR_SET_THRESHOLDS)dlsym(
      ldac_abr_lib_handle, LDAC_ABR_SET_THRESHOLDS_NAME);
  if (ldac_abr_set_thresholds_func == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the LDAC ABR library: %s",
              __func__, LDAC_ABR_SET_THRESHOLDS_NAME, dlerror());
    A2DP_VendorUnloadLdacAbr();
    return false;
  }

  ldac_abr_proc_func =
      (tLDAC_ABR_PROC)dlsym(ldac_abr_lib_handle, LDAC_ABR_PROC_NAME);
  if (ldac_abr_proc_func == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find function '%s' in the LDAC ABR library: %s",
              __func__, LDAC_ABR_PROC_NAME, dlerror());
    A2DP_VendorUnloadLdacAbr();
    return false;
  }

  return true;
}

void A2DP_VendorUnloadLdacAbr(void) {
  ldac_abr_get_handle_func = NULL;
  ldac_abr_free_handle_func = NULL;
  ldac_abr_init_func = NULL;
  ldac_abr_set_thresholds_func = NULL;
  ldac_abr_proc_func = NULL;

  if (ldac_abr_lib_handle != NULL) {
    dlclose(ldac_abr_lib_handle);
    ldac_abr_lib_handle = NULL;
  }
}

HANDLE_LDAC_ABR a2dp_ldac_abr_get_handle(void) {
  return ldac_abr_get_handle_func();
}

void a2dp_ldac_abr_free_handle(HANDLE_LDAC_ABR hLdacAbr) {
  return ldac_abr_free_handle_func(hLdacAbr);
}

int a2dp_ldac_abr_init(HANDLE_LDAC_ABR hLdacAbr, unsigned int interval_ms) {
  return ldac_abr_init_func(hLdacAbr, interval_ms);
}

int a2dp_ldac_abr_set_thresholds(HANDLE_LDAC_ABR hLdacAbr,
                                 unsigned int th_critical,
                                 unsigned int th_dangerous_trend,
                                 unsigned int th_4hqsq) {
  return ldac_abr_set_thresholds_func(hLdacAbr, th_critical, th_dangerous_trend,
                                      th_4hqsq);
}

int a2dp_ldac_abr_proc(HANDLE_LDAC_BT hLdacParam, HANDLE_LDAC_ABR hLdacAbr,
                       size_t transmit_queue_length, unsigned int flag_enable) {
  return ldac_abr_proc_func(hLdacParam, hLdacAbr, transmit_queue_length,
                            flag_enable);
}