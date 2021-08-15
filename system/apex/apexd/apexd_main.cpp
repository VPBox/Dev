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

#define LOG_TAG "apexd"

#include <strings.h>

#include <android-base/logging.h>

#include "apexd.h"
#include "apexd_checkpoint_vold.h"
#include "apexd_prepostinstall.h"
#include "apexd_prop.h"
#include "apexservice.h"
#include "status_or.h"

#include <android-base/properties.h>

namespace {

int HandleSubcommand(char** argv) {
  if (strcmp("--pre-install", argv[1]) == 0) {
    LOG(INFO) << "Preinstall subcommand detected";
    return android::apex::RunPreInstall(argv);
  }

  if (strcmp("--post-install", argv[1]) == 0) {
    LOG(INFO) << "Postinstall subcommand detected";
    return android::apex::RunPostInstall(argv);
  }

  if (strcmp("--bootstrap", argv[1]) == 0) {
    LOG(INFO) << "Bootstrap subcommand detected";
    return android::apex::onBootstrap();
  }

  LOG(ERROR) << "Unknown subcommand: " << argv[1];
  return 1;
}

struct CombinedLogger {
  android::base::LogdLogger logd;

  CombinedLogger() {}

  void operator()(android::base::LogId id, android::base::LogSeverity severity,
                  const char* tag, const char* file, unsigned int line,
                  const char* message) {
    logd(id, severity, tag, file, line, message);
    KernelLogger(id, severity, tag, file, line, message);
  }
};

}  // namespace

int main(int /*argc*/, char** argv) {
  // Use CombinedLogger to also log to the kernel log.
  android::base::InitLogging(argv, CombinedLogger());

  if (argv[1] != nullptr) {
    return HandleSubcommand(argv);
  }
  // TODO: add a -v flag or an external setting to change LogSeverity.
  android::base::SetMinimumLogSeverity(android::base::VERBOSE);

  android::apex::StatusOr<android::apex::VoldCheckpointInterface>
      vold_service_st = android::apex::VoldCheckpointInterface::Create();
  android::apex::VoldCheckpointInterface* vold_service = nullptr;
  if (!vold_service_st.Ok()) {
    LOG(ERROR) << "Could not retrieve vold service: "
               << vold_service_st.ErrorMessage();
  } else {
    vold_service = &*vold_service_st;
  }

  android::apex::onStart(vold_service);
  android::apex::binder::CreateAndRegisterService();
  android::apex::binder::StartThreadPool();

  // Notify other components (e.g. init) that all APEXs are correctly mounted
  // and are ready to be used. Note that it's important that the binder service
  // is registered at this point, since other system services might depend on
  // it.
  android::apex::onAllPackagesReady();

  android::apex::waitForBootStatus(
      android::apex::rollbackActiveSessionAndReboot,
      android::apex::unmountDanglingMounts);

  android::apex::binder::JoinThreadPool();
  return 1;
}
