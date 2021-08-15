// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common/debug.h"
#include "common/expected.h"
#include "inode2filename/search_directories.h"

using namespace iorap::inode2filename;  // NOLINT

#if defined(IORAP_INODE2FILENAME_MAIN)

void Usage(char** argv) {
  std::cerr << "Usage: " << argv[0] << " <options> <<inode_syntax>> [inode_syntax1 inode_syntax2 ...]" << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "  Block until all inodes have been read in, then begin searching for filenames for those inodes." << std::endl;
  std::cerr << "  Results are written immediately as they are available, and once all inodes are found, " << std::endl;
  std::cerr << "  the program will terminate." << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "    Inode syntax:     ('dev_t@inode' | 'major:minor:inode')" << std::endl;
  std::cerr << "    --help,-h         Print this Usage." << std::endl;
  std::cerr << "    --root,-r         Add root directory (default '.'). Repeatable." << std::endl;
  std::cerr << "    --verbose,-v      Set verbosity (default off)." << std::endl;
  std::cerr << "    --wait,-w         Wait for key stroke before continuing (default off)." << std::endl;
  exit(1);
}

static fruit::Component<SearchDirectories> GetSearchDirectoriesComponent() {
    return fruit::createComponent().bind<SystemCall, SystemCallImpl>();
}

int main(int argc, char** argv) {
  android::base::InitLogging(argv);
  android::base::SetLogger(android::base::StderrLogger);

  bool wait_for_keystroke = false;
  bool enable_verbose = false;
  std::vector<std::string> root_directories;
  std::vector<Inode> inode_list;

  if (argc == 1) {
    Usage(argv);
  }

  for (int arg = 1; arg < argc; ++arg) {
    std::string argstr = argv[arg];
    bool has_arg_next = (arg+1)<argc;
    std::string arg_next = has_arg_next ? argv[arg+1] : "";

    if (argstr == "--help" || argstr == "-h") {
      Usage(argv);
    } else if (argstr == "--root" || argstr == "-r") {
      if (!has_arg_next) {
        std::cerr << "Missing --root <value>" << std::endl;
        return 1;
      }
      root_directories.push_back(arg_next);
      ++arg;
    } else if (argstr == "--verbose" || argstr == "-v") {
      enable_verbose = true;
    } else if (argstr == "--wait" || argstr == "-w") {
      wait_for_keystroke = true;
    } else {
      Inode maybe_inode{};

      std::string error_msg;
      if (Inode::Parse(argstr, /*out*/&maybe_inode, /*out*/&error_msg)) {
        inode_list.push_back(maybe_inode);
      } else {
        if (argstr.size() >= 1) {
          if (argstr[0] == '-') {
            std::cerr << "Unrecognized flag: " << argstr << std::endl;
            return 1;
          }
        }

        std::cerr << "Failed to parse inode (" << argstr << ") because: " << error_msg << std::endl;
        return 1;
      }
    }
  }

  if (root_directories.size() == 0) {
    root_directories.push_back(".");
  }

  if (inode_list.size() == 0) {
    DCHECK_EQ(true, false);
    std::cerr << "Provide at least one inode." << std::endl;
    return 1;
  }

  if (enable_verbose) {
    android::base::SetMinimumLogSeverity(android::base::VERBOSE);

    LOG(VERBOSE) << "Verbose check";
    LOG(VERBOSE) << "Debug check: " << ::iorap::kIsDebugBuild;

    for (auto& inode_num : inode_list) {
      LOG(VERBOSE) << "Searching for inode " << inode_num;
    }
  }

  // Useful to attach a debugger...
  // 1) $> inode2filename -w <args>
  // 2) $> gdbclient <pid>
  if (wait_for_keystroke) {
    LOG(INFO) << "Self pid: " << getpid();
    LOG(INFO) << "Press any key to continue...";
    std::cin >> wait_for_keystroke;
  }

  fruit::Injector<SearchDirectories> injector(GetSearchDirectoriesComponent);
  SearchDirectories* search_directories = injector.get<SearchDirectories*>();

  auto/*observable[2]*/ [inode_results, connectable] =
      search_directories->FindFilenamesFromInodesPair(
          std::move(root_directories),
          std::move(inode_list),
          SearchMode::kInProcessDirect);

  int return_code = 1;
  inode_results.subscribe([&return_code](const InodeResult& result) {
    if (result) {
      LOG(DEBUG) << "Inode match: " << result.inode << ", " << result.data.value();
      std::cout << "Inode match: " << result.inode << ", " << result.data.value() << std::endl;
      return_code = 0;
    } else {
      LOG(WARNING) << "Failed to match inode: " << result.inode;
    }
  });

  // Normally #subscribe would start emitting items immediately, but this does nothing yet
  // because one of the nodes in the flow graph was published. Published streams make the entire
  // downstream inert until #connect is called.
  connectable->connect();

  // 0 -> found at least a single match, 1 -> could not find any matches.
  return return_code;
}

#endif
