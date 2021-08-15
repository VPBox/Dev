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

#ifndef UPDATE_ENGINE_COMMON_TEST_UTILS_H_
#define UPDATE_ENGINE_COMMON_TEST_UTILS_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Streams used for gtest's PrintTo() functions.
#include <iostream>  // NOLINT(readability/streams)
#include <memory>
#include <string>
#include <vector>

#include <base/files/file_path.h>
#include <base/files/scoped_temp_dir.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "update_engine/common/action.h"
#include "update_engine/common/utils.h"
#include "update_engine/update_metadata.pb.h"

// These are some handy functions for unittests.

namespace chromeos_update_engine {

// PrintTo() functions are used by gtest to log these objects. These PrintTo()
// functions must be defined in the same namespace as the first argument.
void PrintTo(const Extent& extent, ::std::ostream* os);
void PrintTo(const ErrorCode& error_code, ::std::ostream* os);

namespace test_utils {

// 300 byte pseudo-random string. Not null terminated.
// This does not gzip compress well.
extern const uint8_t kRandomString[300];

// Writes the data passed to path. The file at path will be overwritten if it
// exists. Returns true on success, false otherwise.
bool WriteFileVector(const std::string& path, const brillo::Blob& data);
bool WriteFileString(const std::string& path, const std::string& data);

// Binds provided |filename| to an unused loopback device, whose name is written
// to the string pointed to by |out_lo_dev_name|. The new loop device will be
// read-only unless |writable| is set to true. Returns true on success, false
// otherwise (along with corresponding test failures), in which case the content
// of |out_lo_dev_name| is unknown.
bool BindToUnusedLoopDevice(const std::string& filename,
                            bool writable,
                            std::string* out_lo_dev_name);
bool UnbindLoopDevice(const std::string& lo_dev_name);

// Returns true iff a == b
bool ExpectVectorsEq(const brillo::Blob& a, const brillo::Blob& b);

inline int System(const std::string& cmd) {
  return system(cmd.c_str());
}

// Reads a symlink from disk. Returns empty string on failure.
std::string Readlink(const std::string& path);

void FillWithData(brillo::Blob* buffer);

// Compare the value of native array for download source parameter.
MATCHER_P(DownloadSourceMatcher, source_array, "") {
  return std::equal(source_array, source_array + kNumDownloadSources, arg);
}

// Class to unmount FS when object goes out of scope
class ScopedFilesystemUnmounter {
 public:
  explicit ScopedFilesystemUnmounter(const std::string& mountpoint)
      : mountpoint_(mountpoint), should_unmount_(true) {}
  ~ScopedFilesystemUnmounter() {
    if (should_unmount_) {
      utils::UnmountFilesystem(mountpoint_);
    }
  }
  void set_should_unmount(bool unmount) { should_unmount_ = unmount; }

 private:
  const std::string mountpoint_;
  bool should_unmount_;
  DISALLOW_COPY_AND_ASSIGN(ScopedFilesystemUnmounter);
};

class ScopedLoopbackDeviceBinder {
 public:
  ScopedLoopbackDeviceBinder(const std::string& file,
                             bool writable,
                             std::string* dev) {
    is_bound_ = BindToUnusedLoopDevice(file, writable, &dev_);
    EXPECT_TRUE(is_bound_);

    if (is_bound_ && dev)
      *dev = dev_;
  }

  ~ScopedLoopbackDeviceBinder() {
    if (!is_bound_)
      return;

    for (int retry = 0; retry < 5; retry++) {
      if (UnbindLoopDevice(dev_))
        return;
      sleep(1);
    }
    ADD_FAILURE();
  }

  const std::string& dev() const {
    EXPECT_TRUE(is_bound_);
    return dev_;
  }

  bool is_bound() const { return is_bound_; }

 private:
  std::string dev_;
  bool is_bound_;
  DISALLOW_COPY_AND_ASSIGN(ScopedLoopbackDeviceBinder);
};

class ScopedTempFile {
 public:
  ScopedTempFile() : ScopedTempFile("update_engine_test_temp_file.XXXXXX") {}

  explicit ScopedTempFile(const std::string& pattern) {
    EXPECT_TRUE(utils::MakeTempFile(pattern, &path_, nullptr));
    unlinker_.reset(new ScopedPathUnlinker(path_));
  }

  const std::string& path() const { return path_; }

 private:
  std::string path_;
  std::unique_ptr<ScopedPathUnlinker> unlinker_;
};

class ScopedLoopMounter {
 public:
  explicit ScopedLoopMounter(const std::string& file_path,
                             std::string* mnt_path,
                             unsigned long flags);  // NOLINT(runtime/int)

 private:
  // These objects must be destructed in the following order:
  //   ScopedFilesystemUnmounter (the file system must be unmounted first)
  //   ScopedLoopbackDeviceBinder (then the loop device can be deleted)
  //   ScopedDirRemover (then the mount point can be deleted)
  base::ScopedTempDir temp_dir_;
  std::unique_ptr<ScopedLoopbackDeviceBinder> loop_binder_;
  std::unique_ptr<ScopedFilesystemUnmounter> unmounter_;
};

// Returns the path where the build artifacts are stored. This is the directory
// where the unittest executable is being run from.
base::FilePath GetBuildArtifactsPath();
// Returns the path of the build artifact specified in |relative_path|.
std::string GetBuildArtifactsPath(const std::string& relative_path);

}  // namespace test_utils

// Useful actions for test. These need to be defined in the
// chromeos_update_engine namespace.

class NoneType;

template <typename T>
class ObjectFeederAction;

template <typename T>
class ActionTraits<ObjectFeederAction<T>> {
 public:
  typedef T OutputObjectType;
  typedef NoneType InputObjectType;
};

// This is a simple Action class for testing. It feeds an object into
// another action.
template <typename T>
class ObjectFeederAction : public Action<ObjectFeederAction<T>> {
 public:
  typedef NoneType InputObjectType;
  typedef T OutputObjectType;
  void PerformAction() {
    LOG(INFO) << "feeder running!";
    CHECK(this->processor_);
    if (this->HasOutputPipe()) {
      this->SetOutputObject(out_obj_);
    }
    this->processor_->ActionComplete(this, ErrorCode::kSuccess);
  }
  static std::string StaticType() { return "ObjectFeederAction"; }
  std::string Type() const { return StaticType(); }
  void set_obj(const T& out_obj) { out_obj_ = out_obj; }

 private:
  T out_obj_;
};

template <typename T>
class ObjectCollectorAction;

template <typename T>
class ActionTraits<ObjectCollectorAction<T>> {
 public:
  typedef NoneType OutputObjectType;
  typedef T InputObjectType;
};

// This is a simple Action class for testing. It receives an object from
// another action.
template <typename T>
class ObjectCollectorAction : public Action<ObjectCollectorAction<T>> {
 public:
  typedef T InputObjectType;
  typedef NoneType OutputObjectType;
  void PerformAction() {
    LOG(INFO) << "collector running!";
    ASSERT_TRUE(this->processor_);
    if (this->HasInputObject()) {
      object_ = this->GetInputObject();
    }
    this->processor_->ActionComplete(this, ErrorCode::kSuccess);
  }
  static std::string StaticType() { return "ObjectCollectorAction"; }
  std::string Type() const { return StaticType(); }
  const T& object() const { return object_; }

 private:
  T object_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_TEST_UTILS_H_
