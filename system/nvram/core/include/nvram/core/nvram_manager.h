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

#ifndef NVRAM_CORE_NVRAM_MANAGER_H_
#define NVRAM_CORE_NVRAM_MANAGER_H_

#include <nvram/messages/nvram_messages.h>

#include <nvram/core/persistence.h>

namespace nvram {

// |NvramManager| implements the core functionality of the access-controlled
// NVRAM HAL backend. It keeps track of the allocated spaces and their state,
// including the transient state that is held per boot. It provides operations
// for querying, creating, deleting, reading and writing spaces. It deals with
// persistent storage objects in the form of |NvramHeader| and |NvramSpace|
// objects and uses the persistence layer to read and write them from persistent
// storage.
class NvramManager {
 public:
  // Looks at |request| to determine the command to execute, extracts the
  // request parameters and invokes the correct handler function. Stores status
  // and output parameters in |response|.
  void Dispatch(const Request& request, Response* response);

  nvram_result_t GetInfo(const GetInfoRequest& request,
                         GetInfoResponse* response);
  nvram_result_t CreateSpace(const CreateSpaceRequest& request,
                             CreateSpaceResponse* response);
  nvram_result_t GetSpaceInfo(const GetSpaceInfoRequest& request,
                              GetSpaceInfoResponse* response);
  nvram_result_t DeleteSpace(const DeleteSpaceRequest& request,
                             DeleteSpaceResponse* response);
  nvram_result_t DisableCreate(const DisableCreateRequest& request,
                               DisableCreateResponse* response);
  nvram_result_t WriteSpace(const WriteSpaceRequest& request,
                            WriteSpaceResponse* response);
  nvram_result_t ReadSpace(const ReadSpaceRequest& request,
                           ReadSpaceResponse* response);
  nvram_result_t LockSpaceWrite(const LockSpaceWriteRequest& request,
                                LockSpaceWriteResponse* response);
  nvram_result_t LockSpaceRead(const LockSpaceReadRequest& request,
                               LockSpaceReadResponse* response);

  // The wipe functions are meant for use by firmware after determining the
  // device's mode of operation. These can be used to clear access-controlled
  // NVRAM when a user invokes a full hardware reset. Note that in regular
  // operation, the user *MUST BE PREVENTED* from wiping access-controlled
  // NVRAM.
  //
  // If a full hardware reset can conveniently clear the access-controlled NVRAM
  // storage area out of band, it's fine to do so. In this case, the
  // wiping-related commands should not be exposed at all. Note that this is the
  // default behavior - the reference implementation will ignore all wipe
  // requests unless compiled with NVRAM_WIPE_STORAGE_SUPPORT=1.
  //
  // For devices where firmware doesn't have direct control over the storage
  // area used by access-controlled NVRAM, the wiping commands are provided to
  // facilitate clearing storage:
  //   1. Determine boot mode.
  //   2. If not in recovery mode, call DisableWipe(). All further wipe requests
  //      will be rejected. A reboot (or TEE restart for that matter) is
  //      required before a new decision can be made.
  //   3. If operating in recovery mode, forgo calling DisableWipe(). The
  //      recovery process will then be able to invoke WipeStorage() later as
  //      needed.
  nvram_result_t WipeStorage(const WipeStorageRequest& request,
                             WipeStorageResponse* response);
  nvram_result_t DisableWipe(const DisableWipeRequest& request,
                             DisableWipeResponse* response);

 private:
  // Holds transient state corresponding to an allocated NVRAM space, i.e. meta
  // data valid for a single boot. One instance of this struct is kept in memory
  // in the |spaces_| array for each of the spaces that are currently allocated.
  struct SpaceListEntry {
    uint32_t index;
    bool write_locked = false;
    bool read_locked = false;
  };

  // |SpaceRecord| holds all information known about a space. It includes both
  // an index and pointer to the transient information held in the
  // |SpaceListEntry| in the |spaces_| array and the persistent |NvramSpace|
  // state held in permanent storage. We only load the persistent space data
  // from storage when it is needed for an operation, such as reading and
  // writing space contents.
  struct SpaceRecord {
    // Access control check for write access to the space. The
    // |authorization_value| is only relevant if the space was configured to
    // require authorization. Returns RESULT_SUCCESS if write access is
    // permitted and a suitable result code to return to the client on failure.
    nvram_result_t CheckWriteAccess(const Blob& authorization_value);

    // Access control check for read access to the space. The
    // |authorization_value| is only relevant if the space was configured to
    // require authorization. Returns RESULT_SUCCESS if write access is
    // permitted and a suitable result code to return the client on failure.
    nvram_result_t CheckReadAccess(const Blob& authorization_value);

    size_t array_index = 0;
    SpaceListEntry* transient = nullptr;
    NvramSpace persistent;
  };

  // Initializes |header_| from storage if that hasn't happened already. Returns
  // true if NvramManager object is initialized and ready to serve requests. May
  // be called again after failure to attempt initialization again.
  bool Initialize();

  // Finds the array index in |spaces_| that corresponds to |space_index|.
  // Returns |kMaxSpaces| if there is no matching space.
  size_t FindSpace(uint32_t space_index);

  // Loads space data for |index|. Fills in |space_record| and returns true if
  // successful. Returns false and sets |result| on error.
  bool LoadSpaceRecord(uint32_t index,
                       SpaceRecord* space_record,
                       nvram_result_t* result);

  // Writes the header to storage and returns a suitable status code.
  nvram_result_t WriteHeader(Optional<uint32_t> provisional_index);

  // Write |space| data for |index|.
  nvram_result_t WriteSpace(uint32_t index, const NvramSpace& space);

  // Maximum number of NVRAM spaces we're willing to allocate.
  static constexpr size_t kMaxSpaces = 32;

  bool initialized_ = false;
  bool disable_create_ = false;
  bool disable_wipe_ = false;

  // Bookkeeping information for allocated spaces.
  size_t num_spaces_ = 0;
  SpaceListEntry spaces_[kMaxSpaces];
};

}  // namespace nvram

#endif  // NVRAM_CORE_NVRAM_MANAGER_H_
