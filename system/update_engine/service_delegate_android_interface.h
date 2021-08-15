//
// Copyright (C) 2016 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_SERVICE_DELEGATE_ANDROID_INTERFACE_H_
#define UPDATE_ENGINE_SERVICE_DELEGATE_ANDROID_INTERFACE_H_

#include <inttypes.h>

#include <string>
#include <vector>

#include <brillo/errors/error.h>

namespace chromeos_update_engine {

// This class defines the interface exposed by the Android version of the
// daemon service. This interface only includes the method calls that such
// daemon exposes. For asynchronous events initiated by a class implementing
// this interface see the ServiceObserverInterface class.
class ServiceDelegateAndroidInterface {
 public:
  virtual ~ServiceDelegateAndroidInterface() = default;

  // Start an update attempt to download an apply the provided |payload_url| if
  // no other update is running. The extra |key_value_pair_headers| will be
  // included when fetching the payload. Returns whether the update was started
  // successfully, which means that no other update was running and the passed
  // parameters were correct, but not necessarily that the update finished
  // correctly.
  virtual bool ApplyPayload(
      const std::string& payload_url,
      int64_t payload_offset,
      int64_t payload_size,
      const std::vector<std::string>& key_value_pair_headers,
      brillo::ErrorPtr* error) = 0;

  // Suspend an ongoing update. Returns true if there was an update ongoing and
  // it was suspended. In case of failure, it returns false and sets |error|
  // accordingly.
  virtual bool SuspendUpdate(brillo::ErrorPtr* error) = 0;

  // Resumes an update suspended with SuspendUpdate(). The update can't be
  // suspended after it finished and this method will fail in that case.
  // Returns whether the resume operation was successful, which only implies
  // that there was a suspended update. In case of error, returns false and sets
  // |error| accordingly.
  virtual bool ResumeUpdate(brillo::ErrorPtr* error) = 0;

  // Cancel the ongoing update. The update could be running or suspended, but it
  // can't be canceled after it was done. In case of error, returns false and
  // sets |error| accordingly.
  virtual bool CancelUpdate(brillo::ErrorPtr* error) = 0;

  // Reset the already applied update back to an idle state. This method can
  // only be called when no update attempt is going on, and it will reset the
  // status back to idle, deleting the currently applied update if any. In case
  // of error, returns false and sets |error| accordingly.
  virtual bool ResetStatus(brillo::ErrorPtr* error) = 0;

  // Verifies whether a payload (delegated by the payload metadata) can be
  // applied to the current device. Returns whether the payload is applicable.
  // In case of error, returns false and sets |error| accordingly.
  virtual bool VerifyPayloadApplicable(const std::string& metadata_filename,
                                       brillo::ErrorPtr* error) = 0;

 protected:
  ServiceDelegateAndroidInterface() = default;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_SERVICE_DELEGATE_ANDROID_INTERFACE_H_
