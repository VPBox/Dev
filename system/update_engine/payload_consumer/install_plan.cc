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

#include "update_engine/payload_consumer/install_plan.h"

#include <base/format_macros.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"

using std::string;

namespace chromeos_update_engine {

string InstallPayloadTypeToString(InstallPayloadType type) {
  switch (type) {
    case InstallPayloadType::kUnknown:
      return "unknown";
    case InstallPayloadType::kFull:
      return "full";
    case InstallPayloadType::kDelta:
      return "delta";
  }
  return "invalid type";
}

bool InstallPlan::operator==(const InstallPlan& that) const {
  return ((is_resume == that.is_resume) &&
          (download_url == that.download_url) && (payloads == that.payloads) &&
          (source_slot == that.source_slot) &&
          (target_slot == that.target_slot) && (partitions == that.partitions));
}

bool InstallPlan::operator!=(const InstallPlan& that) const {
  return !((*this) == that);
}

void InstallPlan::Dump() const {
  string partitions_str;
  for (const auto& partition : partitions) {
    partitions_str +=
        base::StringPrintf(", part: %s (source_size: %" PRIu64
                           ", target_size %" PRIu64 ", postinst:%s)",
                           partition.name.c_str(),
                           partition.source_size,
                           partition.target_size,
                           utils::ToString(partition.run_postinstall).c_str());
  }
  string payloads_str;
  for (const auto& payload : payloads) {
    payloads_str += base::StringPrintf(
        ", payload: (size: %" PRIu64 ", metadata_size: %" PRIu64
        ", metadata signature: %s, hash: %s, payload type: %s)",
        payload.size,
        payload.metadata_size,
        payload.metadata_signature.c_str(),
        base::HexEncode(payload.hash.data(), payload.hash.size()).c_str(),
        InstallPayloadTypeToString(payload.type).c_str());
  }

  string version_str = base::StringPrintf(", version: %s", version.c_str());
  if (!system_version.empty()) {
    version_str +=
        base::StringPrintf(", system_version: %s", system_version.c_str());
  }

  LOG(INFO) << "InstallPlan: " << (is_resume ? "resume" : "new_update")
            << version_str
            << ", source_slot: " << BootControlInterface::SlotName(source_slot)
            << ", target_slot: " << BootControlInterface::SlotName(target_slot)
            << ", url: " << download_url << payloads_str << partitions_str
            << ", hash_checks_mandatory: "
            << utils::ToString(hash_checks_mandatory)
            << ", powerwash_required: " << utils::ToString(powerwash_required)
            << ", switch_slot_on_reboot: "
            << utils::ToString(switch_slot_on_reboot)
            << ", run_post_install: " << utils::ToString(run_post_install)
            << ", is_rollback: " << utils::ToString(is_rollback)
            << ", write_verity: " << utils::ToString(write_verity);
}

bool InstallPlan::LoadPartitionsFromSlots(BootControlInterface* boot_control) {
  bool result = true;
  for (Partition& partition : partitions) {
    if (source_slot != BootControlInterface::kInvalidSlot &&
        partition.source_size > 0) {
      result = boot_control->GetPartitionDevice(
                   partition.name, source_slot, &partition.source_path) &&
               result;
    } else {
      partition.source_path.clear();
    }

    if (target_slot != BootControlInterface::kInvalidSlot &&
        partition.target_size > 0) {
      result = boot_control->GetPartitionDevice(
                   partition.name, target_slot, &partition.target_path) &&
               result;
    } else {
      partition.target_path.clear();
    }
  }
  return result;
}

bool InstallPlan::Partition::operator==(
    const InstallPlan::Partition& that) const {
  return (name == that.name && source_path == that.source_path &&
          source_size == that.source_size && source_hash == that.source_hash &&
          target_path == that.target_path && target_size == that.target_size &&
          target_hash == that.target_hash &&
          run_postinstall == that.run_postinstall &&
          postinstall_path == that.postinstall_path &&
          filesystem_type == that.filesystem_type &&
          postinstall_optional == that.postinstall_optional);
}

}  // namespace chromeos_update_engine
