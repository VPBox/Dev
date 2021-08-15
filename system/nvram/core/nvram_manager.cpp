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

#include "nvram/core/nvram_manager.h"

extern "C" {
#include <inttypes.h>
#include <string.h>
}  // extern "C"

#include <nvram/core/logger.h>

#include "crypto.h"

using namespace nvram::storage;

namespace nvram {

namespace {

// Maximum size of a single space's contents.
constexpr size_t kMaxSpaceSize = 1024;

// Maximum authorization blob size;
constexpr size_t kMaxAuthSize = 32;

// The bitmask of all supported control flags.
constexpr uint32_t kSupportedControlsMask =
    (1 << NV_CONTROL_PERSISTENT_WRITE_LOCK) |
    (1 << NV_CONTROL_BOOT_WRITE_LOCK) |
    (1 << NV_CONTROL_BOOT_READ_LOCK) |
    (1 << NV_CONTROL_WRITE_AUTHORIZATION) |
    (1 << NV_CONTROL_READ_AUTHORIZATION) |
    (1 << NV_CONTROL_WRITE_EXTEND);

// Convert the |space.controls| bitmask to vector representation.
nvram_result_t GetControlsVector(const NvramSpace& space,
                                 Vector<nvram_control_t>* controls) {
  for (size_t control = 0; control < sizeof(uint32_t) * 8; ++control) {
    if (space.HasControl(control)) {
      if (!controls->Resize(controls->size() + 1)) {
        NVRAM_LOG_ERR("Allocation failure.");
        return NV_RESULT_INTERNAL_ERROR;
      }
      (*controls)[controls->size() - 1] = static_cast<nvram_control_t>(control);
    }
  }
  return NV_RESULT_SUCCESS;
}

// Constant time memory block comparison.
bool ConstantTimeEquals(const Blob& a, const Blob& b) {
  if (a.size() != b.size())
    return false;

  // The volatile qualifiers prevent the compiler from making assumptions that
  // allow shortcuts:
  //  * The entire array data must be read from memory.
  //  * Marking |result| volatile ensures the subsequent loop iterations must
  //    still store to |result|, thus avoiding the loop to exit early.
  // This achieves the desired constant-time behavior.
  volatile const uint8_t* data_a = a.data();
  volatile const uint8_t* data_b = b.data();
  volatile uint8_t result = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    result |= data_a[i] ^ data_b[i];
  }

  return result == 0;
}

// A standard minimum function.
template <typename Type>
const Type& min(const Type& a, const Type& b) {
  return (a < b) ? a : b;
}

// Filter status codes from the storage layer to only include known values.
// Anything outside the range will be mapped to the generic |kStorageError|.
storage::Status SanitizeStorageStatus(storage::Status status) {
  switch (status) {
    case storage::Status::kSuccess:
      return storage::Status::kSuccess;
    case storage::Status::kNotFound:
      return storage::Status::kNotFound;
    case storage::Status::kStorageError:
      return storage::Status::kStorageError;
  }
  NVRAM_LOG_ERR("Unknown status code %u!", status);
  return storage::Status::kStorageError;
}

}  // namespace

// Looks at |request| to determine the command to execute, then invokes
// the appropriate handler.
void NvramManager::Dispatch(const nvram::Request& request,
                            nvram::Response* response) {
  nvram_result_t result = NV_RESULT_INVALID_PARAMETER;
  const nvram::RequestUnion& input = request.payload;
  nvram::ResponseUnion* output = &response->payload;

  switch (input.which()) {
    case nvram::COMMAND_GET_INFO:
      result = GetInfo(*input.get<COMMAND_GET_INFO>(),
                       &output->Activate<COMMAND_GET_INFO>());
      break;
    case nvram::COMMAND_CREATE_SPACE:
      result = CreateSpace(*input.get<COMMAND_CREATE_SPACE>(),
                           &output->Activate<COMMAND_CREATE_SPACE>());
      break;
    case nvram::COMMAND_GET_SPACE_INFO:
      result = GetSpaceInfo(*input.get<COMMAND_GET_SPACE_INFO>(),
                            &output->Activate<COMMAND_GET_SPACE_INFO>());
      break;
    case nvram::COMMAND_DELETE_SPACE:
      result = DeleteSpace(*input.get<COMMAND_DELETE_SPACE>(),
                           &output->Activate<COMMAND_DELETE_SPACE>());
      break;
    case nvram::COMMAND_DISABLE_CREATE:
      result = DisableCreate(*input.get<COMMAND_DISABLE_CREATE>(),
                             &output->Activate<COMMAND_DISABLE_CREATE>());
      break;
    case nvram::COMMAND_WRITE_SPACE:
      result = WriteSpace(*input.get<COMMAND_WRITE_SPACE>(),
                          &output->Activate<COMMAND_WRITE_SPACE>());
      break;
    case nvram::COMMAND_READ_SPACE:
      result = ReadSpace(*input.get<COMMAND_READ_SPACE>(),
                         &output->Activate<COMMAND_READ_SPACE>());
      break;
    case nvram::COMMAND_LOCK_SPACE_WRITE:
      result = LockSpaceWrite(*input.get<COMMAND_LOCK_SPACE_WRITE>(),
                              &output->Activate<COMMAND_LOCK_SPACE_WRITE>());
      break;
    case nvram::COMMAND_LOCK_SPACE_READ:
      result = LockSpaceRead(*input.get<COMMAND_LOCK_SPACE_READ>(),
                             &output->Activate<COMMAND_LOCK_SPACE_READ>());
      break;
    case nvram::COMMAND_WIPE_STORAGE:
      result = WipeStorage(*input.get<COMMAND_WIPE_STORAGE>(),
                           &output->Activate<COMMAND_WIPE_STORAGE>());
      break;
    case nvram::COMMAND_DISABLE_WIPE:
      result = DisableWipe(*input.get<COMMAND_DISABLE_WIPE>(),
                           &output->Activate<COMMAND_DISABLE_WIPE>());
      break;
  }

  response->result = result;
}

nvram_result_t NvramManager::GetInfo(const GetInfoRequest& /* request */,
                                     GetInfoResponse* response) {
  NVRAM_LOG_INFO("GetInfo");

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  // TODO: Get better values for total and available size from the storage
  // layer.
  response->total_size = kMaxSpaceSize * kMaxSpaces;
  response->available_size = kMaxSpaceSize * (kMaxSpaces - num_spaces_);
  response->max_space_size = kMaxSpaceSize;
  response->max_spaces = kMaxSpaces;
  Vector<uint32_t>& space_list = response->space_list;
  if (!space_list.Resize(num_spaces_)) {
    NVRAM_LOG_ERR("Allocation failure.");
    return NV_RESULT_INTERNAL_ERROR;
  }
  for (size_t i = 0; i < num_spaces_; ++i) {
    space_list[i] = spaces_[i].index;
  }
  response->wipe_disabled = disable_wipe_;

  return NV_RESULT_SUCCESS;
}

nvram_result_t NvramManager::CreateSpace(const CreateSpaceRequest& request,
                                         CreateSpaceResponse* /* response */) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("CreateSpace Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  if (disable_create_) {
    NVRAM_LOG_INFO("Creation of further spaces is disabled.");
    return NV_RESULT_OPERATION_DISABLED;
  }

  if (FindSpace(index) != kMaxSpaces) {
    NVRAM_LOG_INFO("Space 0x%" PRIx32 " already exists.", index);
    return NV_RESULT_SPACE_ALREADY_EXISTS;
  }

  if (num_spaces_ + 1 > kMaxSpaces) {
    NVRAM_LOG_INFO("Too many spaces.");
    return NV_RESULT_INVALID_PARAMETER;
  }

  if (request.size > kMaxSpaceSize) {
    NVRAM_LOG_INFO("Create request exceeds max space size.");
    return NV_RESULT_INVALID_PARAMETER;
  }

  if (request.authorization_value.size() > kMaxAuthSize) {
    NVRAM_LOG_INFO("Authorization blob too large.");
    return NV_RESULT_INVALID_PARAMETER;
  }

  uint32_t controls = 0;
  for (uint32_t control : request.controls) {
    controls |= (1 << control);
  }
  if ((controls & ~kSupportedControlsMask) != 0) {
    NVRAM_LOG_INFO("Bad controls.");
    return NV_RESULT_INVALID_PARAMETER;
  }
  if ((controls & (1 << NV_CONTROL_PERSISTENT_WRITE_LOCK)) != 0 &&
      (controls & (1 << NV_CONTROL_BOOT_WRITE_LOCK)) != 0) {
    NVRAM_LOG_INFO("Write lock controls are exclusive.");
    return NV_RESULT_INVALID_PARAMETER;
  }
  if ((controls & (1 << NV_CONTROL_WRITE_EXTEND)) != 0 &&
      request.size != crypto::kSHA256DigestSize) {
    NVRAM_LOG_INFO("Write-extended space size must be %zu.",
                   crypto::kSHA256DigestSize);
    return NV_RESULT_INVALID_PARAMETER;
  }

  // Mark the index as allocated.
  spaces_[num_spaces_].index = index;
  spaces_[num_spaces_].write_locked = false;
  spaces_[num_spaces_].read_locked = false;
  ++num_spaces_;

  // Create a space record.
  NvramSpace space;
  space.flags = 0;
  space.controls = controls;

  // Copy the auth blob.
  if (space.HasControl(NV_CONTROL_WRITE_AUTHORIZATION) ||
      space.HasControl(NV_CONTROL_READ_AUTHORIZATION)) {
    if (!space.authorization_value.Assign(request.authorization_value.data(),
                                          request.authorization_value.size())) {
      NVRAM_LOG_ERR("Allocation failure.");
      return NV_RESULT_INTERNAL_ERROR;
    }
  }

  // Initialize the space content.
  if (!space.contents.Resize(request.size)) {
    NVRAM_LOG_ERR("Allocation failure.");
    return NV_RESULT_INTERNAL_ERROR;
  }
  memset(space.contents.data(), 0, request.size);

  // Write the header before the space data. This ensures that all space
  // definitions present in storage are also recorded in the header. Thus, the
  // set of spaces present in the header is always a superset of the set of
  // spaces that have state in storage. If there's a crash after writing the
  // header but before writing the space information, the space data will be
  // missing in storage. The initialization code handles this by checking the
  // for the space data corresponding to the index marked as provisional in the
  // header.
  nvram_result_t result;
  if ((result = WriteHeader(Optional<uint32_t>(index))) != NV_RESULT_SUCCESS ||
      (result = WriteSpace(index, space)) != NV_RESULT_SUCCESS) {
    --num_spaces_;
  }
  return result;
}

nvram_result_t NvramManager::GetSpaceInfo(const GetSpaceInfoRequest& request,
                                          GetSpaceInfoResponse* response) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("GetSpaceInfo Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  SpaceRecord space_record;
  nvram_result_t result;
  if (!LoadSpaceRecord(index, &space_record, &result)) {
    return result;
  }

  response->size = space_record.persistent.contents.size();

  result = GetControlsVector(space_record.persistent, &response->controls);
  if (result != NV_RESULT_SUCCESS) {
    return NV_RESULT_INTERNAL_ERROR;
  }

  if (space_record.persistent.HasControl(NV_CONTROL_BOOT_READ_LOCK)) {
    response->read_locked = space_record.transient->read_locked;
  }

  if (space_record.persistent.HasControl(NV_CONTROL_PERSISTENT_WRITE_LOCK)) {
    response->write_locked =
        space_record.persistent.HasFlag(NvramSpace::kFlagWriteLocked);
  } else if (space_record.persistent.HasControl(NV_CONTROL_BOOT_WRITE_LOCK)) {
    response->write_locked = space_record.transient->write_locked;
  }

  return NV_RESULT_SUCCESS;
}

nvram_result_t NvramManager::DeleteSpace(const DeleteSpaceRequest& request,
                                         DeleteSpaceResponse* /* response */) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("DeleteSpace Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  SpaceRecord space_record;
  nvram_result_t result;
  if (!LoadSpaceRecord(index, &space_record, &result)) {
    return result;
  }

  result = space_record.CheckWriteAccess(request.authorization_value);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  // Delete the space. First mark the space as provisionally removed in the
  // header. Then, delete the space data from storage. This allows orphaned
  // space data be cleaned up after a crash.
  SpaceListEntry tmp = spaces_[space_record.array_index];
  spaces_[space_record.array_index] = spaces_[num_spaces_ - 1];
  --num_spaces_;
  result = WriteHeader(Optional<uint32_t>(index));
  if (result == NV_RESULT_SUCCESS) {
    switch (SanitizeStorageStatus(persistence::DeleteSpace(index))) {
      case storage::Status::kStorageError:
        NVRAM_LOG_ERR("Failed to delete space 0x%" PRIx32 " data.", index);
        result = NV_RESULT_INTERNAL_ERROR;
        break;
      case storage::Status::kNotFound:
        // The space was missing even if it shouldn't have been. Log an error,
        // but return success as we're in the desired state.
        NVRAM_LOG_ERR("Space 0x%" PRIx32 " data missing on deletion.", index);
        return NV_RESULT_SUCCESS;
      case storage::Status::kSuccess:
        return NV_RESULT_SUCCESS;
    }
  }

  // Failed to delete, re-add the transient state to |spaces_|.
  spaces_[num_spaces_] = tmp;
  ++num_spaces_;
  return result;
}

nvram_result_t NvramManager::DisableCreate(
    const DisableCreateRequest& /* request */,
    DisableCreateResponse* /* response */) {
  NVRAM_LOG_INFO("DisableCreate");

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  // Set the |disable_create_| flag and call |WriteHeader| to persist the flag
  // such that it remains effective after a reboot. Make sure to restore the
  // current value of |disable_create_| if the write call fails, as we return an
  // error in that case and client code would not expect state changes.
  bool disable_create_previous = disable_create_;
  disable_create_ = true;
  nvram_result_t result = WriteHeader(Optional<uint32_t>());
  if (result != NV_RESULT_SUCCESS) {
    disable_create_ = disable_create_previous;
  }
  return result;
}

nvram_result_t NvramManager::WriteSpace(const WriteSpaceRequest& request,
                                        WriteSpaceResponse* /* response */) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("WriteSpace Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  SpaceRecord space_record;
  nvram_result_t result;
  if (!LoadSpaceRecord(index, &space_record, &result)) {
    return result;
  }

  result = space_record.CheckWriteAccess(request.authorization_value);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  Blob& contents = space_record.persistent.contents;
  if (space_record.persistent.HasControl(NV_CONTROL_WRITE_EXTEND)) {
    // Concatenate the current space |contents| with the input data.
    Blob sha256_input;
    if (!sha256_input.Resize(contents.size() + request.buffer.size())) {
      return NV_RESULT_INTERNAL_ERROR;
    }
    memcpy(sha256_input.data(), contents.data(), contents.size());
    memcpy(sha256_input.data() + contents.size(), request.buffer.data(),
           request.buffer.size());

    // Compute the SHA-256 digest and write it back to |contents|.
    crypto::SHA256(sha256_input.data(), sha256_input.size(), contents.data(),
                   contents.size());
  } else {
    if (contents.size() < request.buffer.size()) {
      return NV_RESULT_INVALID_PARAMETER;
    }

    memcpy(contents.data(), request.buffer.data(), request.buffer.size());
    memset(contents.data() + request.buffer.size(), 0x0,
           contents.size() - request.buffer.size());
  }

  return WriteSpace(index, space_record.persistent);
}

nvram_result_t NvramManager::ReadSpace(const ReadSpaceRequest& request,
                                       ReadSpaceResponse* response) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("ReadSpace Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  SpaceRecord space_record;
  nvram_result_t result;
  if (!LoadSpaceRecord(index, &space_record, &result)) {
    return result;
  }

  result = space_record.CheckReadAccess(request.authorization_value);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  if (!response->buffer.Assign(space_record.persistent.contents.data(),
                               space_record.persistent.contents.size())) {
    NVRAM_LOG_ERR("Allocation failure.");
    return NV_RESULT_INTERNAL_ERROR;
  }

  return NV_RESULT_SUCCESS;
}

nvram_result_t NvramManager::LockSpaceWrite(
    const LockSpaceWriteRequest& request,
    LockSpaceWriteResponse* /* response */) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("LockSpaceWrite Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  SpaceRecord space_record;
  nvram_result_t result;
  if (!LoadSpaceRecord(index, &space_record, &result)) {
    return result;
  }

  result = space_record.CheckWriteAccess(request.authorization_value);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  if (space_record.persistent.HasControl(NV_CONTROL_PERSISTENT_WRITE_LOCK)) {
    space_record.persistent.SetFlag(NvramSpace::kFlagWriteLocked);
    return WriteSpace(index, space_record.persistent);
  } else if (space_record.persistent.HasControl(NV_CONTROL_BOOT_WRITE_LOCK)) {
    space_record.transient->write_locked = true;
    return NV_RESULT_SUCCESS;
  }

  NVRAM_LOG_ERR("Space not configured for write locking.");
  return NV_RESULT_INVALID_PARAMETER;
}

nvram_result_t NvramManager::LockSpaceRead(
    const LockSpaceReadRequest& request,
    LockSpaceReadResponse* /* response */) {
  const uint32_t index = request.index;
  NVRAM_LOG_INFO("LockSpaceRead Ox%" PRIx32, index);

  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

  SpaceRecord space_record;
  nvram_result_t result;
  if (!LoadSpaceRecord(index, &space_record, &result)) {
    return result;
  }

  result = space_record.CheckReadAccess(request.authorization_value);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  if (space_record.persistent.HasControl(NV_CONTROL_BOOT_READ_LOCK)) {
    space_record.transient->read_locked = true;
    return NV_RESULT_SUCCESS;
  }

  NVRAM_LOG_ERR("Space not configured for read locking.");
  return NV_RESULT_INVALID_PARAMETER;
}

nvram_result_t NvramManager::WipeStorage(
    const WipeStorageRequest& /* request */,
    WipeStorageResponse* /* response */) {
  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

#ifdef NVRAM_WIPE_STORAGE_SUPPORT
  if (disable_wipe_) {
    return NV_RESULT_OPERATION_DISABLED;
  }

  // Go through all spaces and wipe the corresponding data. Note that the header
  // is only updated once all space data is gone. This will "break" all spaces
  // that are left declared but don't have data. This situation can be observed
  // if we crash somewhere during the wiping process before clearing the header.
  //
  // Note that we deliberately choose this wiping sequence so we can never end
  // up in a state where the header appears clean but existing space data
  // remains.
  //
  // As a final note, the ideal solution would be to atomically clear the header
  // and delete all space data. While more desirable from an operational point
  // of view, this would drastically complicate storage layer requirements to
  // support cross-object atomicity instead of per-object atomicity.
  for (size_t i = 0; i < num_spaces_; ++i) {
    const uint32_t index = spaces_[i].index;
    switch (SanitizeStorageStatus(persistence::DeleteSpace(index))) {
      case storage::Status::kStorageError:
        NVRAM_LOG_ERR("Failed to wipe space 0x%" PRIx32 " data.", index);
        return NV_RESULT_INTERNAL_ERROR;
      case storage::Status::kNotFound:
        // The space was missing even if it shouldn't have been. This may occur
        // if a previous wiping attempt was aborted half-way. Log an error, but
        // return success as we're in the desired state.
        NVRAM_LOG_WARN("Space 0x%" PRIx32 " data missing on wipe.", index);
        break;
      case storage::Status::kSuccess:
        break;
    }
  }

  // All spaces are gone, clear the header.
  num_spaces_ = 0;
  return WriteHeader(Optional<uint32_t>());
#else  // NVRAM_WIPE_STORAGE_SUPPORT
  // We're not accessing the flag member, so prevent a compiler warning. The
  // alternative of conditionally including the member in the class declaration
  // looks cleaner at first sight, but comes with the risk of
  // NVRAM_WIPE_STORAGE_SUPPORT polarity mismatches between compilation units,
  // which is more subtly dangerous, so we rather keep the member even for the
  // case in which it is not used.
  (void)disable_wipe_;
  return NV_RESULT_OPERATION_DISABLED;
#endif  // NVRAM_WIPE_STORAGE_SUPPORT
}

nvram_result_t NvramManager::DisableWipe(
    const DisableWipeRequest& /* request */,
    DisableWipeResponse* /* response */) {
  if (!Initialize())
    return NV_RESULT_INTERNAL_ERROR;

#ifdef NVRAM_WIPE_STORAGE_SUPPORT
  disable_wipe_ = true;
  return NV_RESULT_SUCCESS;
#else  // NVRAM_WIPE_STORAGE_SUPPORT
  return NV_RESULT_OPERATION_DISABLED;
#endif  // NVRAM_WIPE_STORAGE_SUPPORT
}

nvram_result_t NvramManager::SpaceRecord::CheckWriteAccess(
    const Blob& authorization_value) {
  if (persistent.HasControl(NV_CONTROL_PERSISTENT_WRITE_LOCK)) {
    if (persistent.HasFlag(NvramSpace::kFlagWriteLocked)) {
      NVRAM_LOG_INFO("Attempt to write persistently locked space 0x%" PRIx32
                     ".",
                     transient->index);
      return NV_RESULT_OPERATION_DISABLED;
    }
  } else if (persistent.HasControl(NV_CONTROL_BOOT_WRITE_LOCK)) {
    if (transient->write_locked) {
      NVRAM_LOG_INFO("Attempt to write per-boot locked space 0x%" PRIx32 ".",
                     transient->index);
      return NV_RESULT_OPERATION_DISABLED;
    }
  }

  if (persistent.HasControl(NV_CONTROL_WRITE_AUTHORIZATION) &&
      !ConstantTimeEquals(persistent.authorization_value,
                          authorization_value)) {
    NVRAM_LOG_INFO(
        "Authorization value mismatch for write access to space 0x%" PRIx32 ".",
        transient->index);
    return NV_RESULT_ACCESS_DENIED;
  }

  // All checks passed, allow the write.
  return NV_RESULT_SUCCESS;
}

nvram_result_t NvramManager::SpaceRecord::CheckReadAccess(
    const Blob& authorization_value) {
  if (persistent.HasControl(NV_CONTROL_BOOT_READ_LOCK)) {
    if (transient->read_locked) {
      NVRAM_LOG_INFO("Attempt to read per-boot locked space 0x%" PRIx32 ".",
                     transient->index);
      return NV_RESULT_OPERATION_DISABLED;
    }
  }

  if (persistent.HasControl(NV_CONTROL_READ_AUTHORIZATION) &&
      !ConstantTimeEquals(persistent.authorization_value,
                          authorization_value)) {
    NVRAM_LOG_INFO(
        "Authorization value mismatch for read access to space 0x%" PRIx32 ".",
        transient->index);
    return NV_RESULT_ACCESS_DENIED;
  }

  // All checks passed, allow the read.
  return NV_RESULT_SUCCESS;
}

bool NvramManager::Initialize() {
  if (initialized_)
    return true;

  NvramHeader header;
  switch (SanitizeStorageStatus(persistence::LoadHeader(&header))) {
    case storage::Status::kStorageError:
      NVRAM_LOG_ERR("Init failed to load header.");
      return false;
    case storage::Status::kNotFound:
      // No header in storage. This happens the very first time we initialize
      // on a fresh device where the header isn't present yet. The first write
      // will flush the fresh header to storage.
      initialized_ = true;
      return true;
    case storage::Status::kSuccess:
      if (header.version > NvramHeader::kVersion) {
        NVRAM_LOG_ERR("Storage format %" PRIu32 " is more recent than %" PRIu32
                      ", aborting.",
                      header.version, NvramHeader::kVersion);
        return false;
      }
      break;
  }

  // Check the state of the provisional space if applicable.
  const Optional<uint32_t>& provisional_index = header.provisional_index;
  bool provisional_space_in_storage = false;
  if (provisional_index.valid()) {
    NvramSpace space;
    switch (SanitizeStorageStatus(
        persistence::LoadSpace(provisional_index.value(), &space))) {
      case storage::Status::kStorageError:
        // Log an error but leave the space marked as allocated. This will allow
        // initialization to complete, so other spaces can be accessed.
        // Operations on the bad space will fail however. The choice of keeping
        // the bad space around (as opposed to dropping it) is intentional:
        //  * Failing noisily reduces the chances of bugs going undetected.
        //  * Keeping the index allocated prevents it from being accidentally
        //    clobbered due to appearing absent after transient storage errors.
        NVRAM_LOG_ERR("Failed to load provisional space 0x%" PRIx32 ".",
                      provisional_index.value());
        provisional_space_in_storage = true;
        break;
      case storage::Status::kNotFound:
        break;
      case storage::Status::kSuccess:
        provisional_space_in_storage = true;
        break;
    }
  }

  // If there are more spaces allocated than this build supports, fail
  // initialization. This may seem a bit drastic, but the alternatives aren't
  // acceptable:
  //  * If we continued with just a subset of the spaces, that may lead to wrong
  //    conclusions about the system state in consumers. Furthermore, consumers
  //    might delete a space to make room and then create a space that appears
  //    free but is present in storage. This would clobber the existing space
  //    data and potentially violate its access control rules.
  //  * We could just try to allocate more memory to hold the larger number of
  //    spaces. That'd render the memory footprint of the NVRAM implementation
  //    unpredictable. One variation that may work is to allow a maximum number
  //    of existing spaces larger than kMaxSpaces, but still within sane limits.
  if (header.allocated_indices.size() > kMaxSpaces) {
    NVRAM_LOG_ERR("Excess spaces %zu in header.",
                  header.allocated_indices.size());
    return false;
  }

  // Initialize the transient space bookkeeping data.
  bool delete_provisional_space = provisional_index.valid();
  for (uint32_t index : header.allocated_indices) {
    if (provisional_index.valid() && provisional_index.value() == index) {
      // The provisional space index refers to a created space. If it isn't
      // valid, pretend it was never created.
      if (!provisional_space_in_storage) {
        continue;
      }

      // The provisional space index corresponds to a created space that is
      // present in storage. Retain the space.
      delete_provisional_space = false;
    }

    spaces_[num_spaces_].index = index;
    spaces_[num_spaces_].write_locked = false;
    spaces_[num_spaces_].read_locked = false;
    ++num_spaces_;
  }

  // If the provisional space data is present in storage, but the index wasn't
  // in |header.allocated_indices|, it refers to half-deleted space. Destroy the
  // space in that case.
  if (delete_provisional_space) {
    switch (SanitizeStorageStatus(
        persistence::DeleteSpace(provisional_index.value()))) {
      case storage::Status::kStorageError:
        NVRAM_LOG_ERR("Failed to delete provisional space 0x%" PRIx32 " data.",
                      provisional_index.value());
        return false;
      case storage::Status::kNotFound:
        // The space isn't present in storage. This may happen if the space
        // deletion succeeded, but the header wasn't written subsequently.
        break;
      case storage::Status::kSuccess:
        break;
    }
  }

  disable_create_ = header.HasFlag(NvramHeader::kFlagDisableCreate);
  initialized_ = true;

  // Write the header to clear the provisional index if necessary. It's actually
  // not a problem if this fails, because the state is consistent regardless. We
  // still do this opportunistically in order to avoid loading the provisional
  // space data for each reboot after a crash.
  if (provisional_index.valid()) {
    WriteHeader(Optional<uint32_t>());
  }

  return true;
}

size_t NvramManager::FindSpace(uint32_t space_index) {
  for (size_t i = 0; i < num_spaces_; ++i) {
    if (spaces_[i].index == space_index) {
      return i;
    }
  }

  return kMaxSpaces;
}

bool NvramManager::LoadSpaceRecord(uint32_t index,
                                   SpaceRecord* space_record,
                                   nvram_result_t* result) {
  space_record->array_index = FindSpace(index);
  if (space_record->array_index == kMaxSpaces) {
    *result = NV_RESULT_SPACE_DOES_NOT_EXIST;
    return false;
  }

  space_record->transient = &spaces_[space_record->array_index];

  switch (SanitizeStorageStatus(
      persistence::LoadSpace(index, &space_record->persistent))) {
    case storage::Status::kStorageError:
      NVRAM_LOG_ERR("Failed to load space 0x%" PRIx32 " data.", index);
      *result = NV_RESULT_INTERNAL_ERROR;
      return false;
    case storage::Status::kNotFound:
      // This should never happen if the header contains the index.
      NVRAM_LOG_ERR("Space index 0x%" PRIx32
                    " present in header, but data missing.",
                    index);
      *result = NV_RESULT_INTERNAL_ERROR;
      return false;
    case storage::Status::kSuccess:
      *result = NV_RESULT_SUCCESS;
      return true;
  }

  *result = NV_RESULT_INTERNAL_ERROR;
  return false;
}

nvram_result_t NvramManager::WriteHeader(Optional<uint32_t> provisional_index) {
  NvramHeader header;
  header.version = NvramHeader::kVersion;
  if (disable_create_) {
    header.SetFlag(NvramHeader::kFlagDisableCreate);
  }

  if (!header.allocated_indices.Resize(num_spaces_)) {
    NVRAM_LOG_ERR("Allocation failure.");
    return NV_RESULT_INTERNAL_ERROR;
  }
  for (size_t i = 0; i < num_spaces_; ++i) {
    header.allocated_indices[i] = spaces_[i].index;
  }

  header.provisional_index = provisional_index;

  if (SanitizeStorageStatus(persistence::StoreHeader(header)) !=
      storage::Status::kSuccess) {
    NVRAM_LOG_ERR("Failed to store header.");
    return NV_RESULT_INTERNAL_ERROR;
  }

  return NV_RESULT_SUCCESS;
}

nvram_result_t NvramManager::WriteSpace(uint32_t index,
                                        const NvramSpace& space) {
  if (SanitizeStorageStatus(persistence::StoreSpace(index, space)) !=
      storage::Status::kSuccess) {
    NVRAM_LOG_ERR("Failed to store space 0x%" PRIx32 ".", index);
    return NV_RESULT_INTERNAL_ERROR;
  }

  return NV_RESULT_SUCCESS;
}

}  // namespace nvram
