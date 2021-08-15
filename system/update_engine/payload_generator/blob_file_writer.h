//
// Copyright (C) 2015 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_BLOB_FILE_WRITER_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_BLOB_FILE_WRITER_H_

#include <base/macros.h>

#include <base/synchronization/lock.h>
#include <brillo/secure_blob.h>

namespace chromeos_update_engine {

class BlobFileWriter {
 public:
  // Create the BlobFileWriter object that will manage the blobs stored to
  // |blob_fd| in a thread safe way.
  BlobFileWriter(int blob_fd, off_t* blob_file_size)
      : blob_fd_(blob_fd), blob_file_size_(blob_file_size) {}

  // Store the passed |blob| in the blob file. Returns the offset at which it
  // was stored, or -1 in case of failure.
  off_t StoreBlob(const brillo::Blob& blob);

  // The number of |total_blobs| is the number of blobs that will be stored but
  // is only used for logging purposes. If not set or set to 0, logging will be
  // skipped. This function will also reset the number of stored blobs to 0.
  void SetTotalBlobs(size_t total_blobs);

 private:
  size_t total_blobs_{0};
  size_t stored_blobs_{0};

  // The file and its size are protected with the |blob_mutex_|.
  int blob_fd_;
  off_t* blob_file_size_;

  base::Lock blob_mutex_;

  DISALLOW_COPY_AND_ASSIGN(BlobFileWriter);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_BLOB_FILE_WRITER_H_
