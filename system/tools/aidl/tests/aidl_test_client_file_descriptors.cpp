/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "aidl_test_client_file_descriptors.h"

#include <iostream>
#include <vector>

 #include <errno.h>
#include <string.h>
#include <unistd.h>

#include <android-base/unique_fd.h>
#include <binder/ParcelFileDescriptor.h>

// libbase
using android::base::unique_fd;

// libutils:
using android::sp;

// libbinder:
using android::binder::Status;

// generated
using android::aidl::tests::ITestService;

using android::os::ParcelFileDescriptor;

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace android {
namespace aidl {
namespace tests {
namespace client {

#define FdByName(_fd) #_fd, _fd

bool DoWrite(const string& name, const unique_fd& fd, const string& buf) {
  int wrote;

  while ((wrote = write(fd.get(), buf.data(), buf.size())) < 0 && errno == EINTR);

  if (wrote == (signed)buf.size()) {
    return true;
  }

  if (wrote < 0) {
    cerr << "Error writing to file descriptor '" << name << "': "
        << strerror(errno) << endl;
  } else {
    cerr << "File descriptor '" << name << "'accepted short data." << endl;
  }

  return false;
}

bool DoRead(const string& name, const unique_fd& fd, const string& expected) {
  size_t length = expected.size();
  int got;
  string buf;
  buf.resize(length);

  while ((got = read(fd.get(), &buf[0], length)) < 0 && errno == EINTR);

  if (got < 0) {
    cerr << "Error reading from '" << name << "': " << strerror(errno) << endl;
    return false;
  }

  if (buf != expected) {
    cerr << "Expected '" << expected << "' got '" << buf << "'" << endl;
    return false;
  }

  return true;
}

bool DoPipe(unique_fd* read_side, unique_fd* write_side) {
  int fds[2];
  unique_fd return_fd;

  if (pipe(fds)) {
    cout << "Error creating pipes: " << strerror(errno) << endl;
    return false;
  }

  read_side->reset(fds[0]);
  write_side->reset(fds[1]);
  return true;
}

bool ConfirmFileDescriptors(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning file descriptors works." << endl;

  unique_fd return_fd;
  unique_fd read_fd;
  unique_fd write_fd;

  if (!DoPipe(&read_fd, &write_fd)) {
    return false;
  }

  status = s->RepeatFileDescriptor(write_fd, &return_fd);

  if (!status.isOk()) {
    cerr << "Could not repeat file descriptors." << endl;
    return false;
  }

  /* A note on some of the spookier stuff going on here: IIUC writes to pipes
   * should be atomic and non-blocking so long as the total size doesn't exceed
   * PIPE_BUF. We thus play a bit fast and loose with failure modes here.
   */

  bool ret =
      DoWrite(FdByName(return_fd), "ReturnString") &&
      DoRead(FdByName(read_fd), "ReturnString");

  return ret;
}

bool ConfirmFileDescriptorArrays(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning file descriptor arrays works." << endl;

  vector<unique_fd> array;
  array.resize(2);

  if (!DoPipe(&array[0], &array[1])) {
    return false;
  }

  vector<unique_fd> repeated;
  vector<unique_fd> reversed;

  status = s->ReverseFileDescriptorArray(array, &repeated, &reversed);

  if (!status.isOk()) {
    cerr << "Could not reverse file descriptor array." << endl;
    return false;
  }

  bool ret =
      DoWrite(FdByName(array[1]), "First") &&
      DoWrite(FdByName(repeated[1]), "Second") &&
      DoWrite(FdByName(reversed[0]), "Third") &&
      DoRead(FdByName(reversed[1]), "FirstSecondThird");

  return ret;
}

bool ConfirmParcelFileDescriptors(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning parcel file descriptors works." << endl;

  unique_fd read_fd;
  unique_fd write_fd;

  if (!DoPipe(&read_fd, &write_fd)) {
    return false;
  }

  ParcelFileDescriptor return_fd;

  status = s->RepeatParcelFileDescriptor(ParcelFileDescriptor(std::move(write_fd)), &return_fd);

  if (!status.isOk()) {
    cerr << "Could not repeat parcel file descriptors." << endl;
    return false;
  }

  /* A note on some of the spookier stuff going on here: IIUC writes to pipes
   * should be atomic and non-blocking so long as the total size doesn't exceed
   * PIPE_BUF. We thus play a bit fast and loose with failure modes here.
   */

  bool ret = DoWrite(FdByName(return_fd.release()), "ReturnString") &&
             DoRead(FdByName(read_fd), "ReturnString");

  return ret;
}

bool ConfirmParcelFileDescriptorArrays(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning parcel file descriptor arrays works." << endl;

  vector<unique_fd> array;
  array.resize(2);

  if (!DoPipe(&array[0], &array[1])) {
    return false;
  }

  vector<ParcelFileDescriptor> input;
  for (auto& fd : array) {
    input.push_back(ParcelFileDescriptor(std::move(fd)));
  }

  vector<ParcelFileDescriptor> repeated;
  vector<ParcelFileDescriptor> reversed;

  status = s->ReverseParcelFileDescriptorArray(input, &repeated, &reversed);

  if (!status.isOk()) {
    cerr << "Could not reverse file descriptor array." << endl;
    return false;
  }

  bool ret = DoWrite(FdByName(input[1].release()), "First") &&
             DoWrite(FdByName(repeated[1].release()), "Second") &&
             DoWrite(FdByName(reversed[0].release()), "Third") &&
             DoRead(FdByName(input[0].release()), "FirstSecondThird");

  return ret;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
