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

#define LOG_TAG "sysprop_cpp"

#include <android-base/logging.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <getopt.h>

#include "CppGen.h"

namespace {

struct Arguments {
  std::string input_file_path;
  std::string header_dir;
  std::string system_header_dir;
  std::string source_dir;
  std::string include_name;
};

[[noreturn]] void PrintUsage(const char* exe_name) {
  std::printf(
      "Usage: %s --header-dir dir --source-dir dir "
      "--include-name name --system-header-dir dir "
      "sysprop_file\n",
      exe_name);
  std::exit(EXIT_FAILURE);
}

bool ParseArgs(int argc, char* argv[], Arguments* args, std::string* err) {
  for (;;) {
    static struct option long_options[] = {
        {"header-dir", required_argument, 0, 'h'},
        {"system-header-dir", required_argument, 0, 's'},
        {"source-dir", required_argument, 0, 'c'},
        {"include-name", required_argument, 0, 'n'},
    };

    int opt = getopt_long_only(argc, argv, "", long_options, nullptr);
    if (opt == -1) break;

    switch (opt) {
      case 'h':
        args->header_dir = optarg;
        break;
      case 's':
        args->system_header_dir = optarg;
        break;
      case 'c':
        args->source_dir = optarg;
        break;
      case 'n':
        args->include_name = optarg;
        break;
      default:
        PrintUsage(argv[0]);
    }
  }

  if (optind >= argc) {
    *err = "No input file specified";
    return false;
  }

  if (optind + 1 < argc) {
    *err = "More than one input file";
    return false;
  }

  if (args->header_dir.empty() || args->system_header_dir.empty() ||
      args->source_dir.empty() || args->include_name.empty()) {
    PrintUsage(argv[0]);
  }

  args->input_file_path = argv[optind];

  return true;
}

}  // namespace

int main(int argc, char* argv[]) {
  Arguments args;
  std::string err;
  if (!ParseArgs(argc, argv, &args, &err)) {
    std::fprintf(stderr, "%s: %s\n", argv[0], err.c_str());
    PrintUsage(argv[0]);
  }

  if (!GenerateCppFiles(args.input_file_path, args.header_dir,
                        args.system_header_dir, args.source_dir,
                        args.include_name, &err)) {
    LOG(FATAL) << "Error during generating cpp sysprop from "
               << args.input_file_path << ": " << err;
  }
}
