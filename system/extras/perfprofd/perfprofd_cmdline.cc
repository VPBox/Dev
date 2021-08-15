/*
 *
 * Copyright 2015, The Android Open Source Project
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

#include "perfprofd_cmdline.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <set>
#include <string>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/stringprintf.h>

#include "perfprofd_record.pb.h"

#include "configreader.h"
#include "dropbox.h"
#include "perfprofdcore.h"
#include "perfprofd_io.h"

//
// Perf profiling daemon -- collects system-wide profiles using
//
//       simpleperf record -a
//
// and encodes them so that they can be uploaded by a separate service.
//

//

//
// Output file from 'perf record'.
//
#define PERF_OUTPUT "perf.data"

//
// Path to the perf file to convert and exit? Empty value is the default, daemon mode.
//
static std::string perf_file_to_convert = "";

//
// SIGHUP handler. Sending SIGHUP to the daemon can be used to break it
// out of a sleep() call so as to trigger a new collection (debugging)
//
static void sig_hup(int /* signum */)
{
  LOG(WARNING) << "SIGHUP received";
}

//
// Parse command line args. Currently supported flags:
// *  "-c PATH" sets the path of the config file to PATH.
// *  "-x PATH" reads PATH as a perf data file and saves it as a file in
//    perf_profile.proto format. ".encoded" suffix is appended to PATH to form
//    the output file path.
//
static void parse_args(int argc, char** argv)
{
  int ac;

  for (ac = 1; ac < argc; ++ac) {
    if (!strcmp(argv[ac], "-c")) {
      if (ac >= argc-1) {
        LOG(ERROR) << "malformed command line: -c option requires argument)";
        continue;
      }
      ConfigReader::setConfigFilePath(argv[ac+1]);
      ++ac;
    } else if (!strcmp(argv[ac], "-x")) {
      if (ac >= argc-1) {
        LOG(ERROR) << "malformed command line: -x option requires argument)";
        continue;
      }
      perf_file_to_convert = argv[ac+1];
      ++ac;
    } else {
      LOG(ERROR) << "malformed command line: unknown option or arg " <<  argv[ac] << ")";
      continue;
    }
  }
}

//
// Post-processes after profile is collected and converted to protobuf.
// * GMS core stores processed file sequence numbers in
//   /data/data/com.google.android.gms/files/perfprofd_processed.txt
// * Update /data/misc/perfprofd/perfprofd_produced.txt to remove the sequence
//   numbers that have been processed and append the current seq number
// Returns true if the current_seq should increment.
//
static bool post_process(const Config& config, int current_seq)
{
  const std::string& dest_dir = config.destination_directory;
  std::string processed_file_path =
      config.config_directory + "/" + PROCESSED_FILENAME;
  std::string produced_file_path = dest_dir + "/" + PRODUCED_FILENAME;


  std::set<int> processed;
  FILE *fp = fopen(processed_file_path.c_str(), "r");
  if (fp != NULL) {
    int seq;
    while(fscanf(fp, "%d\n", &seq) > 0) {
      if (remove(android::base::StringPrintf(
          "%s/perf.data.encoded.%d", dest_dir.c_str(),seq).c_str()) == 0) {
        processed.insert(seq);
      }
    }
    fclose(fp);
  }

  std::set<int> produced;
  fp = fopen(produced_file_path.c_str(), "r");
  if (fp != NULL) {
    int seq;
    while(fscanf(fp, "%d\n", &seq) > 0) {
      if (processed.find(seq) == processed.end()) {
        produced.insert(seq);
      }
    }
    fclose(fp);
  }

  uint32_t maxLive = config.max_unprocessed_profiles;
  if (produced.size() >= maxLive) {
    return false;
  }

  produced.insert(current_seq);
  fp = fopen(produced_file_path.c_str(), "w");
  if (fp == NULL) {
    PLOG(WARNING) << "Cannot write " <<  produced_file_path;
    return false;
  }
  for (std::set<int>::const_iterator iter = produced.begin();
       iter != produced.end(); ++iter) {
    fprintf(fp, "%d\n", *iter);
  }
  fclose(fp);
  chmod(produced_file_path.c_str(),
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  return true;
}

//
// Initialization
//

static void init(ConfigReader &config)
{
  if (!config.readFile()) {
    LOG(ERROR) << "unable to open configuration file " << config.getConfigFilePath();
  }

  CommonInit(static_cast<uint32_t>(config.getUnsignedValue("use_fixed_seed")),
             config.getStringValue("destination_directory").c_str());

  signal(SIGHUP, sig_hup);
}

//
// Main routine:
// 1. parse cmd line args
// 2. read config file
// 3. loop: {
//       sleep for a while
//       perform a profile collection
//    }
//
int perfprofd_main(int argc, char** argv, Config* config)
{
  LOG(INFO) << "starting Android Wide Profiling daemon";

  parse_args(argc, argv);
  {
    ConfigReader config_reader;
    init(config_reader);
    config_reader.FillConfig(config);
  }
  GlobalInit(config->perf_path);

  if (!perf_file_to_convert.empty()) {
    std::string encoded_path = perf_file_to_convert + ".encoded";
    encode_to_proto(perf_file_to_convert, encoded_path.c_str(), *config, 0, nullptr);
    return 0;
  }

  // Early exit if we're not supposed to run on this build flavor
  if (!IsDebugBuild() && config->only_debug_build) {
    LOG(INFO) << "early exit due to inappropriate build type";
    return 0;
  }

  auto config_fn = [config]() {
    return config;
  };
  auto reread_config = [config]() {
    // Reread config file -- the uploader may have rewritten it.
    ConfigReader config_reader;
    if (config_reader.readFile()) {
      config_reader.FillConfig(config);
    }
  };
  int seq = 0;
  auto handler = [&seq](android::perfprofd::PerfprofdRecord* proto, Config* handler_config) {
    if (proto == nullptr) {
      return false;
    }
    if (handler_config->send_to_dropbox) {
      std::string error_msg;
      if (!android::perfprofd::dropbox::SendToDropbox(proto,
                                                      handler_config->destination_directory,
                                                      &error_msg)) {
        LOG(ERROR) << "Failed dropbox submission: " << error_msg;
        return false;
      }
    } else {
      std::string data_file_path(handler_config->destination_directory);
      data_file_path += "/";
      data_file_path += PERF_OUTPUT;
      std::string path = android::base::StringPrintf("%s.encoded.%d", data_file_path.c_str(), seq);
      if (!android::perfprofd::SerializeProtobuf(proto, path.c_str(), handler_config->compress)) {
        return false;
      }
      if (!post_process(*handler_config, seq)) {
        return false;
      }
    }
    seq++;
    return true;
  };
  ProfilingLoop(config_fn, reread_config, handler);

  LOG(INFO) << "finishing Android Wide Profiling daemon";
  return 0;
}
