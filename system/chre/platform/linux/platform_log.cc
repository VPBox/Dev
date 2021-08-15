/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "chre/platform/shared/platform_log.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>

#include "chre/platform/fatal_error.h"

namespace chre {

void PlatformLogBase::logLooper() {
  while (1) {
    char *logMessage = nullptr;

    {
      std::unique_lock<std::mutex> lock(mMutex);
      mConditionVariable.wait(lock, [this]{
        return (!mLogQueue.empty() || mStopLogger);
      });

      if (!mLogQueue.empty()) {
        // Move the log message to avoid holding a lock for longer than
        // required.
        logMessage = mLogQueue.front();
        mLogQueue.pop();
      } else if (mStopLogger) {
        // The stop logger is checked in an else-if to allow the main log queue
        // to drain when the logger is stopping.
        break;
      }
    }

    // If we get here, there must be a log message to output. This is outside of
    // the context of the lock which means that the logging thread will only be
    // blocked for the minimum amount of time.
    std::cerr << logMessage << std::endl;
    free(logMessage);
  }
}

PlatformLog::PlatformLog() {
  mLoggerThread = std::thread(&PlatformLog::logLooper, this);
}

PlatformLog::~PlatformLog() {
  {
    std::unique_lock<std::mutex> lock(mMutex);
    mStopLogger = true;
    mConditionVariable.notify_one();
  }

  mLoggerThread.join();
}

void PlatformLog::log(const char *formatStr, ...) {
  char *formattedStr;
  va_list argList;
  va_start(argList, formatStr);
  int result = vasprintf(&formattedStr, formatStr, argList);
  va_end(argList);

  if (result >= 0) {
    std::unique_lock<std::mutex> lock(mMutex);
    mLogQueue.push(formattedStr);
    mConditionVariable.notify_one();
  } else {
    FATAL_ERROR("Failed to allocate log message");
  }
}

}  // namespace chre
