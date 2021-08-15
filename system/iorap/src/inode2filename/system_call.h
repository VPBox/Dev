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

#ifndef IORAP_SRC_INODE2FILENAME_SYSTEM_CALL_H_
#define IORAP_SRC_INODE2FILENAME_SYSTEM_CALL_H_

#include <fruit/fruit.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Abstract out the system calls behind a virtual interface:
// This enables us to use dependency injection to provide mock implementations
// during tests.
struct SystemCall {
  // stat(2)
  virtual int stat(const char *pathname, struct stat *statbuf) = 0;

  // opendir(3)
  virtual DIR *opendir(const char *name) = 0;

  // readdir(3)
  virtual struct dirent *readdir(DIR *dirp) = 0;

  // closedir(3)
  virtual int closedir(DIR *dirp) = 0;

  virtual ~SystemCall() {}
};

// "Live" implementation that calls down to libc.
struct SystemCallImpl : public SystemCall {
  // Marks this constructor as the one to use for injection.
  INJECT(SystemCallImpl()) = default;

  // stat(2)
  virtual int stat(const char *pathname, struct stat *statbuf) override {
    return ::stat(pathname, statbuf);
  }

  // opendir(3)
  virtual DIR *opendir(const char *name) override {
    return ::opendir(name);
  }

  // readdir(3)
  virtual struct dirent *readdir(DIR *dirp) override {
    return ::readdir(dirp);
  }

  // closedir(3)
  virtual int closedir(DIR *dirp) override {
    return ::closedir(dirp);
  }

  virtual ~SystemCallImpl() {}
};

#endif  // IORAP_SRC_INODE2FILENAME_SYSTEM_CALL_H_

