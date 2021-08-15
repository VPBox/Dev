/*
**
** Copyright 2015, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <string.h>

#include <android-base/logging.h>

#include "config.h"
#include "perfprofd_binder.h"
#include "perfprofd_cmdline.h"
#include "perfprofdcore.h"

extern int perfprofd_main(int argc, char** argv, Config* config);

int main(int argc, char** argv)
{
  if (argc > 1 && strcmp(argv[1], "--binder") == 0) {
    return android::perfprofd::binder::Main();
  }

  struct PosixSleepConfig : public Config {
    void Sleep(size_t seconds) override {
      sleep(seconds);
    }
    bool IsProfilingEnabled() const override {
      //
      // Check for existence of semaphore file in config directory
      //
      if (access(config_directory.c_str(), F_OK) == -1) {
        PLOG(WARNING) << "unable to open config directory " << config_directory;
        return false;
      }

      // Check for existence of semaphore file
      std::string semaphore_filepath = config_directory
          + "/" + SEMAPHORE_FILENAME;
      if (access(semaphore_filepath.c_str(), F_OK) == -1) {
        return false;
      }

      return true;
    }
  };
  PosixSleepConfig config;
  return perfprofd_main(argc, argv, &config);
}
