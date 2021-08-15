/******************************************************************************
 *
 *  Copyright 2016 Android Open Source Project
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
#include <base/command_line.h>
#include "main_int.h"

void init_cpp_logging(config_t* config) {
  // Command line and log level might be also configured in service/main.cpp
  // when running the bluetoothtbd daemon. If it's already configured, skip
  // configuring.
  if (base::CommandLine::InitializedForCurrentProcess()) return;

  const std::string* loggingV =
      config_get_string(*config, CONFIG_DEFAULT_SECTION, "LoggingV", NULL);
  const std::string* loggingVModule = config_get_string(
      *config, CONFIG_DEFAULT_SECTION, "LoggingVModule", NULL);

  int argc = 1;
  const char* argv[] = {"bt_stack", NULL, NULL};

  if (loggingV != NULL) {
    argv[argc] = loggingV->c_str();
    argc++;
  }

  if (loggingVModule != NULL) {
    argv[argc] = loggingVModule->c_str();
    argc++;
  }

  // Init command line object with logging switches
  base::CommandLine::Init(argc, argv);

  logging::LoggingSettings log_settings;
  if (!logging::InitLogging(log_settings)) {
    LOG(ERROR) << "Failed to set up logging";
  }

  // Android already logs thread_id, proc_id, timestamp, so disable those.
  logging::SetLogItems(false, false, false, false);
}
