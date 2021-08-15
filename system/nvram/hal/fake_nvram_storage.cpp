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

#include <nvram/core/storage.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>

#include <nvram/core/logger.h>

// An NVRAM storage layer implementation backed by the file system.
//
// NOTE: This does not meet the tamper evidence requirements for
// access-controlled NVRAM implementations, since the file system can't provide
// sufficient protection against tampering by attackers.

namespace {

// Name of the storage object holding the header.
const char kHeaderFileName[] = "header";

// Pattern for space data storage object names.
const char kSpaceDataFileNamePattern[] = "space_%08x";

// Temporary file name used in write-rename atomic write operations.
const char kTempFileName[] = "temp";

// Maximum size of objects we're willing to read and write.
const off_t kMaxFileSize = 2048;

// Buffer size for formatting names.
using NameBuffer = char[16];

// Global data directory descriptor.
int g_data_dir_fd = -1;

// Formats the storage object name for the given space index.
bool FormatSpaceFileName(NameBuffer name, uint32_t index) {
  int ret =
      snprintf(name, sizeof(NameBuffer), kSpaceDataFileNamePattern, index);
  return ret >= 0 && ret < static_cast<int>(sizeof(NameBuffer));
};

nvram::storage::Status DeleteFile(const char* name) {
  if (TEMP_FAILURE_RETRY(unlinkat(g_data_dir_fd, name, 0))) {
    if (errno == ENOENT) {
      return nvram::storage::Status::kNotFound;
    }
    PLOG(ERROR) << "Failed to remove " << name;
    return nvram::storage::Status::kStorageError;
  }

  return nvram::storage::Status::kSuccess;
}

// Loads the storage object identified by |name|.
nvram::storage::Status LoadFile(const char* name, nvram::Blob* blob) {
  android::base::unique_fd data_file_fd(
      TEMP_FAILURE_RETRY(openat(g_data_dir_fd, name, O_RDONLY)));
  if (data_file_fd.get() < 0) {
    if (errno == ENOENT) {
      return nvram::storage::Status::kNotFound;
    }
    PLOG(ERROR) << "Failed to open " << name;
    return nvram::storage::Status::kStorageError;
  }

  struct stat data_file_stat;
  if (TEMP_FAILURE_RETRY(fstat(data_file_fd.get(), &data_file_stat))) {
    PLOG(ERROR) << "Failed to stat " << name;
    return nvram::storage::Status::kStorageError;
  }

  if (data_file_stat.st_size > kMaxFileSize) {
    LOG(ERROR) << "Bad size for " << name << ":" << data_file_stat.st_size;
    return nvram::storage::Status::kStorageError;
  }

  if (!blob->Resize(data_file_stat.st_size)) {
    LOG(ERROR) << "Failed to allocate read buffer for " << name;
    return nvram::storage::Status::kStorageError;
  }

  if (!android::base::ReadFully(data_file_fd.get(), blob->data(),
                                blob->size())) {
    PLOG(ERROR) << "Failed to read " << name;
    return nvram::storage::Status::kStorageError;
  }

  return nvram::storage::Status::kSuccess;
}

// Writes blob to the storage object indicated by |name|.
nvram::storage::Status StoreFile(const char* name, const nvram::Blob& blob) {
  android::base::unique_fd data_file_fd(TEMP_FAILURE_RETRY(
      openat(g_data_dir_fd, kTempFileName, O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR | S_IWUSR)));
  if (data_file_fd.get() < 0) {
    if (errno == ENOENT) {
      return nvram::storage::Status::kNotFound;
    }
    PLOG(ERROR) << "Failed to open " << kTempFileName;
    return nvram::storage::Status::kStorageError;
  }

  if (!android::base::WriteFully(data_file_fd.get(), blob.data(),
                                 blob.size())) {
    PLOG(ERROR) << "Failed to write " << kTempFileName;
    DeleteFile(kTempFileName);
    return nvram::storage::Status::kStorageError;
  }

  // Force the file contents to be written to disk.
  if (TEMP_FAILURE_RETRY(fdatasync(data_file_fd.get()))) {
    PLOG(ERROR) << "Failed to sync " << kTempFileName;
    DeleteFile(kTempFileName);
    return nvram::storage::Status::kStorageError;
  }

  data_file_fd.reset();

  // Move the file into place.
  if (TEMP_FAILURE_RETRY(
          renameat(g_data_dir_fd, kTempFileName, g_data_dir_fd, name))) {
    PLOG(ERROR) << "Failed to move " << kTempFileName << " to " << name;
    DeleteFile(kTempFileName);
    return nvram::storage::Status::kStorageError;
  }

  // Force the directory meta data to be written to disk.
  if (TEMP_FAILURE_RETRY(fsync(g_data_dir_fd))) {
    PLOG(ERROR) << "Failed to sync data directory";
    return nvram::storage::Status::kStorageError;
  }

  return nvram::storage::Status::kSuccess;
}

}  // namespace

// Initializes the storage layer with the provided data directory descriptor.
void InitStorage(int data_dir_fd) {
  g_data_dir_fd = data_dir_fd;
}

namespace nvram {
namespace storage {

Status LoadHeader(Blob* blob) {
  return LoadFile(kHeaderFileName, blob);
}

Status StoreHeader(const Blob& blob) {
  return StoreFile(kHeaderFileName, blob);
}

Status LoadSpace(uint32_t index, Blob* blob) {
  NameBuffer name;
  if (!FormatSpaceFileName(name, index)) {
    return Status::kStorageError;
  }
  return LoadFile(name, blob);
}

Status StoreSpace(uint32_t index, const Blob& blob) {
  NameBuffer name;
  if (!FormatSpaceFileName(name, index)) {
    return Status::kStorageError;
  }
  return StoreFile(name, blob);
}

Status DeleteSpace(uint32_t index) {
  NameBuffer name;
  if (!FormatSpaceFileName(name, index)) {
    return Status::kStorageError;
  }

  return DeleteFile(name);
}

}  // namespace storage
}  // namespace nvram
