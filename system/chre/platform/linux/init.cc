/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "chre/core/event.h"
#include "chre/core/event_loop.h"
#include "chre/core/event_loop_manager.h"
#include "chre/core/init.h"
#include "chre/core/nanoapp.h"
#include "chre/core/static_nanoapps.h"
#ifdef CHRE_AUDIO_SUPPORT_ENABLED
#include "chre/platform/platform_audio.h"
#endif  // CHRE_AUDIO_SUPPORT_ENABLED
#include "chre/platform/context.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/shared/platform_log.h"
#include "chre/platform/system_timer.h"
#include "chre/util/time.h"

#include <csignal>
#include <tclap/CmdLine.h>
#include <thread>

using chre::EventLoopManagerSingleton;
using chre::Milliseconds;

//! A description of the simulator.
constexpr char kSimDescription[] = "A simulation environment for the Context "
                                   "Hub Runtime Environment (CHRE)";

//! The version of the simulator. This is not super important but is assigned by
//! rules of semantic versioning.
constexpr char kSimVersion[] = "0.1.0";

namespace {

extern "C" void signalHandler(int sig) {
  (void) sig;
  LOGI("Stop request received");
  EventLoopManagerSingleton::get()->getEventLoop().stop();
}

}

int main(int argc, char **argv) {
  try {
    // Parse command-line arguments.
    TCLAP::CmdLine cmd(kSimDescription, ' ', kSimVersion);
    TCLAP::SwitchArg noStaticNanoappsArg("", "no_static_nanoapps",
        "disable running static nanoapps", cmd, false);
    TCLAP::MultiArg<std::string> nanoappsArg("", "nanoapp",
        "nanoapp shared object to load and execute", false, "path", cmd);
#ifdef CHRE_AUDIO_SUPPORT_ENABLED
    TCLAP::ValueArg<std::string> audioFileArg("", "audio_file",
        "WAV file to open for audio simulation", false, "", "path", cmd);
    TCLAP::ValueArg<double> minAudioBufSizeArg("", "min_audio_buf_size",
        "min buffer size for audio simulation", false, 1.0, "seconds", cmd);
    TCLAP::ValueArg<double> maxAudioBufSizeArg("", "max_audio_buf_size",
        "max buffer size for audio simulation", false, 10.0, "seconds", cmd);
#endif  // CHRE_AUDIO_SUPPORT_ENABLED
    cmd.parse(argc, argv);

    // Initialize logging.
    chre::PlatformLogSingleton::init();

#ifdef CHRE_AUDIO_SUPPORT_ENABLED
    // Initialize audio sources.
    if (!audioFileArg.getValue().empty()) {
      auto audioSource = chre::MakeUnique<chre::AudioSource>(
          audioFileArg.getValue(), minAudioBufSizeArg.getValue(),
          maxAudioBufSizeArg.getValue());
      chre::PlatformAudio::addAudioSource(audioSource);
    }

    // TODO(P1-d24c82): Add another command line argument that takes a json
    // configuration to support multiple sources.
#endif  // CHRE_AUDIO_SUPPORT_ENABLED

    // Initialize the system.
    chre::init();

    // Register a signal handler.
    std::signal(SIGINT, signalHandler);

    // Load any static nanoapps and start the event loop.
    std::thread chreThread([&]() {
      EventLoopManagerSingleton::get()->lateInit();

      // Load static nanoapps unless they are disabled by a command-line flag.
      if (!noStaticNanoappsArg.getValue()) {
        chre::loadStaticNanoapps();
      }

      // Load dynamic nanoapps specified on the command-line.
      chre::DynamicVector<chre::UniquePtr<chre::Nanoapp>> dynamicNanoapps;
      for (const auto& nanoapp : nanoappsArg.getValue()) {
        dynamicNanoapps.push_back(chre::MakeUnique<chre::Nanoapp>());
        dynamicNanoapps.back()->loadFromFile(nanoapp);
        EventLoopManagerSingleton::get()->getEventLoop()
            .startNanoapp(dynamicNanoapps.back());
      }

      EventLoopManagerSingleton::get()->getEventLoop().run();
    });
    chreThread.join();

    chre::deinit();
    chre::PlatformLogSingleton::deinit();
  } catch (TCLAP::ExitException) {}

  return 0;
}
