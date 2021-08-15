/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aidl.h"
#include "aidl_apicheck.h"
#include "io_delegate.h"
#include "logging.h"
#include "options.h"

#include <iostream>
#include <memory>

using android::aidl::Options;

// aidl is leaky. Turn off LeakSanitizer by default. b/37749857
extern "C" const char* __asan_default_options() {
  return "detect_leaks=0";
}

int main(int argc, char* argv[]) {
  android::base::InitLogging(argv);
  LOG(DEBUG) << "aidl starting";
  Options options(argc, argv, Options::Language::JAVA);
  if (!options.Ok()) {
    std::cerr << options.GetErrorMessage();
    std::cerr << options.GetUsage();
    return 1;
  }

  android::aidl::IoDelegate io_delegate;
  switch (options.GetTask()) {
    case Options::Task::COMPILE:
      return android::aidl::compile_aidl(options, io_delegate);
    case Options::Task::PREPROCESS:
      return android::aidl::preprocess_aidl(options, io_delegate) ? 0 : 1;
    case Options::Task::DUMP_API:
      return android::aidl::dump_api(options, io_delegate) ? 0 : 1;
    case Options::Task::CHECK_API:
      return android::aidl::check_api(options, io_delegate) ? 0 : 1;
    case Options::Task::DUMP_MAPPINGS:
      return android::aidl::dump_mappings(options, io_delegate) ? 0 : 1;
    default:
      LOG(FATAL) << "aidl: internal error" << std::endl;
      return 1;
  }
}
