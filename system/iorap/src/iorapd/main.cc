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

#include "binder/iiorap_impl.h"
#include "common/debug.h"
#include "manager/event_manager.h"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <binder/IPCThreadState.h>
#include <utils/Trace.h>

#include <stdio.h>

static constexpr const char* kServiceName = iorap::binder::IIorapImpl::getServiceName();

// Log to both Stderr and Logd for convenience when running this from the command line.
class StderrAndLogdLogger {
 public:
  explicit StderrAndLogdLogger(android::base::LogId default_log_id = android::base::MAIN)
    : logd_(default_log_id) {
  }

  void operator()(::android::base::LogId id,
                  ::android::base::LogSeverity sev,
                  const char* tag,
                  const char* file,
                  unsigned int line,
                  const char* message) {
    logd_(id, sev, tag, file, line, message);
    StderrLogger(id, sev, tag, file, line, message);
  }

 private:
  ::android::base::LogdLogger logd_;
};

int main(int /*argc*/, char** argv) {
  if (android::base::GetBoolProperty("iorapd.log.verbose", iorap::kIsDebugBuild)) {
    // Show verbose logs if the property is enabled or if we are a debug build.
    setenv("ANDROID_LOG_TAGS", "*:v", /*overwrite*/ 1);
  }

  // Logs go to system logcat.
  android::base::InitLogging(argv, StderrAndLogdLogger{android::base::SYSTEM});

  {
    android::ScopedTrace trace_main{ATRACE_TAG_PACKAGE_MANAGER, "main"};
    LOG(INFO) << kServiceName << " (the prefetchening) firing up";

    android::ScopedTrace trace_start{ATRACE_TAG_PACKAGE_MANAGER, "IorapNativeService::start"};

    // TODO: use fruit for this DI.
    auto /*std::shared_ptr<EventManager>*/ event_manager =
        iorap::manager::EventManager::Create();
    if (!iorap::binder::IIorapImpl::Start(std::move(event_manager))) {
      LOG(ERROR) << "Unable to start IorapNativeService";
      exit(1);
    }
  }

  // Block until something else shuts down the binder service.
  android::IPCThreadState::self()->joinThreadPool();
  LOG(INFO) << kServiceName << " shutting down";

  return 0;
}
