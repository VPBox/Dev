//
// Copyright (C) 2012 The Android Open Source Project
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
//

// This is a simple program used to test interaction with update_engine when
// executing other programs. This program receives pre-programmed actions in the
// command line and executes them in order.

#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#define EX_USAGE_ERROR 100

void usage(const char* program, const char* error) {
  if (error)
    fprintf(stderr, "ERROR: %s\n", error);
  fprintf(stderr, "Usage: %s <cmd> [args..]\n", program);
  exit(EX_USAGE_ERROR);
}

int main(int argc, char** argv, char** envp) {
  if (argc < 2)
    usage(argv[0], "No command passed");

  std::string cmd(argv[1]);
  if (cmd == "fstat") {
    // Call fstat on the passed file descriptor number
    if (argc < 3)
      usage(argv[0], "No fd passed to fstat");
    int fd = atoi(argv[2]);
    struct stat buf;
    int rc = fstat(fd, &buf);
    if (rc < 0) {
      int ret = errno;
      perror("fstat");
      return ret;
    }
    return 0;
  }

  usage(argv[0], "Unknown command");
}
