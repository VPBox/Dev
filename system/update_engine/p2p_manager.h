//
// Copyright (C) 2013 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_P2P_MANAGER_H_
#define UPDATE_ENGINE_P2P_MANAGER_H_

#include <string>
#include <vector>

#include <base/callback.h>
#include <base/files/file_path.h>
#include <base/memory/ref_counted.h>
#include <base/time/time.h>
#include <policy/device_policy.h>
#include <policy/libpolicy.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/update_manager/update_manager.h"

namespace chromeos_update_engine {

// Interface for sharing and discovering files via p2p.
class P2PManager {
 public:
  // Interface used for P2PManager implementations. The sole reason
  // for this interface is unit testing.
  class Configuration {
   public:
    virtual ~Configuration() {}

    // Gets the path to the p2p dir being used, e.g. /var/cache/p2p.
    virtual base::FilePath GetP2PDir() = 0;

    // Gets the argument vector for starting (if |is_start| is True)
    // resp. stopping (if |is_start| is False) the p2p service
    // e.g. ["initctl", "start", "p2p"] or ["initctl", "stop", "p2p"].
    virtual std::vector<std::string> GetInitctlArgs(bool is_start) = 0;

    // Gets the argument vector for invoking p2p-client, e.g.
    // "p2p-client --get-url=file_id_we_want --minimum-size=42123".
    virtual std::vector<std::string> GetP2PClientArgs(
        const std::string& file_id, size_t minimum_size) = 0;
  };

  virtual ~P2PManager() {}

  // The type for the callback used in LookupUrlForFile().
  // If the lookup failed, |url| is empty.
  typedef base::Callback<void(const std::string& url)> LookupCallback;

  // Use the device policy specified by |device_policy|. If this is
  // null, then no device policy is used.
  virtual void SetDevicePolicy(const policy::DevicePolicy* device_policy) = 0;

  // Returns true iff P2P is currently allowed for use on this device. This
  // value is determined and maintained by the Update Manager.
  virtual bool IsP2PEnabled() = 0;

  // Ensures that the p2p subsystem is running (e.g. starts it if it's
  // not already running) and blocks until this is so. Returns false
  // if an error occurred.
  virtual bool EnsureP2PRunning() = 0;

  // Ensures that the p2p subsystem is not running (e.g. stops it if
  // it's running) and blocks until this is so. Returns false if an
  // error occurred.
  virtual bool EnsureP2PNotRunning() = 0;

  // Cleans up files in /var/cache/p2p owned by this application as
  // per the |file_extension| and |num_files_to_keep| values passed
  // when the object was constructed. This may be called even if
  // the p2p subsystem is not running.
  virtual bool PerformHousekeeping() = 0;

  // Asynchronously finds a peer that serves the file identified by
  // |file_id|. If |minimum_size| is non-zero, will find a peer that
  // has at least that many bytes. When the result is ready |callback|
  // is called from the current message loop.
  //
  // This operation may take a very long time to complete because part
  // of the p2p protocol involves waiting for the LAN-wide sum of all
  // num-connections to drop below a given threshold. However, if
  // |max_time_to_wait| is non-zero, the operation is guaranteed to
  // not exceed this duration.
  //
  // If the file is not available on the LAN (or if mDNS/DNS-SD is
  // filtered), this is guaranteed to not take longer than 5 seconds.
  virtual void LookupUrlForFile(const std::string& file_id,
                                size_t minimum_size,
                                base::TimeDelta max_time_to_wait,
                                LookupCallback callback) = 0;

  // Shares a file identified by |file_id| in the directory
  // /var/cache/p2p. Initially the file will not be visible, that is,
  // it will have a .tmp extension and not be shared via p2p. Use the
  // FileMakeVisible() method to change this.
  //
  // If you know the final size of the file, pass it in the
  // |expected_size| parameter. Otherwise pass zero. If non-zero, the
  // amount of free space in /var/cache/p2p is checked and if there is
  // less than twice the amount of space available, this method
  // fails. Additionally, disk space will be reserved via fallocate(2)
  // and |expected_size| is written to the user.cros-p2p-filesize
  // xattr of the created file.
  //
  // If the file already exists, true is returned. Any on-disk xattr
  // is not updated.
  virtual bool FileShare(const std::string& file_id, size_t expected_size) = 0;

  // Gets a fully qualified path for the file identified by |file_id|.
  // If the file has not been shared already using the FileShare()
  // method, an empty base::FilePath is returned - use FilePath::empty() to
  // find out.
  virtual base::FilePath FileGetPath(const std::string& file_id) = 0;

  // Gets the actual size of the file identified by |file_id|. This is
  // equivalent to reading the value of the st_size field of the
  // struct stat on the file given by FileGetPath(). Returns -1 if an
  // error occurs.
  //
  // For a file just created with FileShare() this will return 0.
  virtual ssize_t FileGetSize(const std::string& file_id) = 0;

  // Gets the expected size of the file identified by |file_id|. This
  // is equivalent to reading the value of the user.cros-p2p-filesize
  // xattr on the file given by FileGetPath(). Returns -1 if an error
  // occurs.
  //
  // For a file just created with FileShare() this will return the
  // value of the |expected_size| parameter passed to that method.
  virtual ssize_t FileGetExpectedSize(const std::string& file_id) = 0;

  // Gets whether the file identified by |file_id| is publicly
  // visible. If |out_result| is not null, the result is returned
  // there. Returns false if an error occurs.
  virtual bool FileGetVisible(const std::string& file_id, bool* out_result) = 0;

  // Makes the file identified by |file_id| publicly visible
  // (e.g. removes the .tmp extension). If the file is already
  // visible, this method does nothing. Returns False if
  // the method fails or there is no file for |file_id|.
  virtual bool FileMakeVisible(const std::string& file_id) = 0;

  // Counts the number of shared files used by this application
  // (cf. the |file_extension parameter|. Returns -1 if an error
  // occurred.
  virtual int CountSharedFiles() = 0;

  // Creates a suitable P2PManager instance and initializes the object
  // so it's ready for use. The |file_extension| parameter is used to
  // identify your application, use e.g. "cros_au".  If
  // |configuration| is non-null, the P2PManager will take ownership
  // of the Configuration object and use it (hence, it must be
  // heap-allocated).
  //
  // The |num_files_to_keep| parameter specifies how many files to
  // keep after performing housekeeping (cf. the PerformHousekeeping()
  // method) - pass zero to allow infinitely many files. The
  // |max_file_age| parameter specifies the maximum file age after
  // performing housekeeping (pass zero to allow files of any age).
  static P2PManager* Construct(
      Configuration* configuration,
      ClockInterface* clock,
      chromeos_update_manager::UpdateManager* update_manager,
      const std::string& file_extension,
      const int num_files_to_keep,
      const base::TimeDelta& max_file_age);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_P2P_MANAGER_H_
