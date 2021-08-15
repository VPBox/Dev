/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#define LOG_TAG "bt_stack_config"

#include "stack_config.h"

#include <base/logging.h>

#include "osi/include/future.h"
#include "osi/include/log.h"

namespace {
const char* TRACE_CONFIG_ENABLED_KEY = "TraceConf";
const char* PTS_AVRCP_TEST = "PTS_AvrcpTest";
const char* PTS_SECURE_ONLY_MODE = "PTS_SecurePairOnly";
const char* PTS_LE_CONN_UPDATED_DISABLED = "PTS_DisableConnUpdates";
const char* PTS_DISABLE_SDP_LE_PAIR = "PTS_DisableSDPOnLEPair";
const char* PTS_SMP_PAIRING_OPTIONS_KEY = "PTS_SmpOptions";
const char* PTS_SMP_FAILURE_CASE_KEY = "PTS_SmpFailureCase";

static std::unique_ptr<config_t> config;
}  // namespace

// Module lifecycle functions

static future_t* init() {
// TODO(armansito): Find a better way than searching by a hardcoded path.
#if defined(OS_GENERIC)
  const char* path = "bt_stack.conf";
#else  // !defined(OS_GENERIC)
  const char* path = "/etc/bluetooth/bt_stack.conf";
#endif  // defined(OS_GENERIC)
  CHECK(path != NULL);

  LOG_INFO(LOG_TAG, "%s attempt to load stack conf from %s", __func__, path);

  config = config_new(path);
  if (!config) {
    LOG_INFO(LOG_TAG, "%s file >%s< not found", __func__, path);
    config = config_new_empty();
  }

  return future_new_immediate(FUTURE_SUCCESS);
}

static future_t* clean_up() {
  config.reset();
  return future_new_immediate(FUTURE_SUCCESS);
}

EXPORT_SYMBOL extern const module_t stack_config_module = {
    .name = STACK_CONFIG_MODULE,
    .init = init,
    .start_up = NULL,
    .shut_down = NULL,
    .clean_up = clean_up,
    .dependencies = {NULL}};

// Interface functions
static bool get_trace_config_enabled(void) {
  return config_get_bool(*config, CONFIG_DEFAULT_SECTION,
                         TRACE_CONFIG_ENABLED_KEY, false);
}

static bool get_pts_avrcp_test(void) {
  return config_get_bool(*config, CONFIG_DEFAULT_SECTION, PTS_AVRCP_TEST,
                         false);
}

static bool get_pts_secure_only_mode(void) {
  return config_get_bool(*config, CONFIG_DEFAULT_SECTION, PTS_SECURE_ONLY_MODE,
                         false);
}

static bool get_pts_conn_updates_disabled(void) {
  return config_get_bool(*config, CONFIG_DEFAULT_SECTION,
                         PTS_LE_CONN_UPDATED_DISABLED, false);
}

static bool get_pts_crosskey_sdp_disable(void) {
  return config_get_bool(*config, CONFIG_DEFAULT_SECTION,
                         PTS_DISABLE_SDP_LE_PAIR, false);
}

static const std::string* get_pts_smp_options(void) {
  return config_get_string(*config, CONFIG_DEFAULT_SECTION,
                           PTS_SMP_PAIRING_OPTIONS_KEY, NULL);
}

static int get_pts_smp_failure_case(void) {
  return config_get_int(*config, CONFIG_DEFAULT_SECTION,
                        PTS_SMP_FAILURE_CASE_KEY, 0);
}

static config_t* get_all(void) { return config.get(); }

const stack_config_t interface = {
    get_trace_config_enabled,     get_pts_avrcp_test,
    get_pts_secure_only_mode,     get_pts_conn_updates_disabled,
    get_pts_crosskey_sdp_disable, get_pts_smp_options,
    get_pts_smp_failure_case,     get_all};

const stack_config_t* stack_config_get_interface(void) { return &interface; }
