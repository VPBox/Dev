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

#ifndef UPDATE_ENGINE_COMMON_BOOT_CONTROL_INTERFACE_H_
#define UPDATE_ENGINE_COMMON_BOOT_CONTROL_INTERFACE_H_

#include <climits>
#include <map>
#include <string>
#include <vector>

#include <base/callback.h>
#include <base/macros.h>

namespace chromeos_update_engine {

// The abstract boot control interface defines the interaction with the
// platform's bootloader hiding vendor-specific details from the rest of
// update_engine. This interface is used for controlling where the device should
// boot from.
class BootControlInterface {
 public:
  using Slot = unsigned int;

  struct PartitionMetadata {
    struct Partition {
      std::string name;
      uint64_t size;
    };
    struct Group {
      std::string name;
      uint64_t size;
      std::vector<Partition> partitions;
    };
    std::vector<Group> groups;
  };

  static const Slot kInvalidSlot = UINT_MAX;

  virtual ~BootControlInterface() = default;

  // Return the number of update slots in the system. A system will normally
  // have two slots, named "A" and "B" in the documentation, but sometimes
  // images running from other media can have only one slot, like some USB
  // image. Systems with only one slot won't be able to update.
  virtual unsigned int GetNumSlots() const = 0;

  // Return the slot where we are running the system from. On success, the
  // result is a number between 0 and GetNumSlots() - 1. Otherwise, log an error
  // and return kInvalidSlot.
  virtual Slot GetCurrentSlot() const = 0;

  // Determines the block device for the given partition name and slot number.
  // The |slot| number must be between 0 and GetNumSlots() - 1 and the
  // |partition_name| is a platform-specific name that identifies a partition on
  // every slot. In order to access the dynamic partitions in the target slot,
  // InitPartitionMetadata() must be called (once per payload) prior to calling
  // this function. On success, returns true and stores the block device in
  // |device|.
  virtual bool GetPartitionDevice(const std::string& partition_name,
                                  Slot slot,
                                  std::string* device) const = 0;

  // Returns whether the passed |slot| is marked as bootable. Returns false if
  // the slot is invalid.
  virtual bool IsSlotBootable(Slot slot) const = 0;

  // Mark the specified slot unbootable. No other slot flags are modified.
  // Returns true on success.
  virtual bool MarkSlotUnbootable(Slot slot) = 0;

  // Set the passed |slot| as the preferred boot slot. Returns whether it
  // succeeded setting the active slot. If succeeded, on next boot the
  // bootloader will attempt to load the |slot| marked as active. Note that this
  // method doesn't change the value of GetCurrentSlot() on the current boot.
  virtual bool SetActiveBootSlot(Slot slot) = 0;

  // Mark the current slot as successfully booted asynchronously. No other slot
  // flags are modified. Returns false if it was not able to schedule the
  // operation, otherwise, returns true and calls the |callback| with the result
  // of the operation.
  virtual bool MarkBootSuccessfulAsync(base::Callback<void(bool)> callback) = 0;

  // Initializes the metadata of the underlying partitions for a given |slot|
  // and sets up the states for accessing dynamic partitions.
  // |partition_metadata| will be written to the specified |slot| if
  // |update_metadata| is set.
  virtual bool InitPartitionMetadata(
      Slot slot,
      const PartitionMetadata& partition_metadata,
      bool update_metadata) = 0;

  // Do necessary clean-up operations after the whole update.
  virtual void Cleanup() = 0;

  // Return a human-readable slot name used for logging.
  static std::string SlotName(Slot slot) {
    if (slot == kInvalidSlot)
      return "INVALID";
    if (slot < 26)
      return std::string(1, 'A' + slot);
    return "TOO_BIG";
  }

 protected:
  BootControlInterface() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(BootControlInterface);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_BOOT_CONTROL_INTERFACE_H_
