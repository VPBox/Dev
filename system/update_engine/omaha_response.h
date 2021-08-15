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

#ifndef UPDATE_ENGINE_OMAHA_RESPONSE_H_
#define UPDATE_ENGINE_OMAHA_RESPONSE_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <limits>
#include <string>
#include <vector>

namespace chromeos_update_engine {

// This struct encapsulates the data Omaha's response for the request.
// The strings in this struct are not XML escaped.
struct OmahaResponse {
  // True iff there is an update to be downloaded.
  bool update_exists = false;

  // If non-zero, server-dictated poll interval in seconds.
  int poll_interval = 0;

  // These are only valid if update_exists is true:
  std::string version;
  std::string system_version;

  struct Package {
    // The ordered list of URLs in the Omaha response. Each item is a complete
    // URL (i.e. in terms of Omaha XML, each value is a urlBase + packageName)
    std::vector<std::string> payload_urls;
    uint64_t size = 0;
    uint64_t metadata_size = 0;
    std::string metadata_signature;
    std::string hash;
    // True if the payload described in this response is a delta payload.
    // False if it's a full payload.
    bool is_delta = false;
  };
  std::vector<Package> packages;

  std::string more_info_url;
  std::string deadline;
  int max_days_to_scatter = 0;
  // The number of URL-related failures to tolerate before moving on to the
  // next URL in the current pass. This is a configurable value from the
  // Omaha Response attribute, if ever we need to fine tune the behavior.
  uint32_t max_failure_count_per_url = 0;
  bool prompt = false;

  // True if the Omaha rule instructs us to disable the back-off logic
  // on the client altogether. False otherwise.
  bool disable_payload_backoff = false;

  // True if the Omaha rule instructs us to disable p2p for downloading.
  bool disable_p2p_for_downloading = false;

  // True if the Omaha rule instructs us to disable p2p for sharing.
  bool disable_p2p_for_sharing = false;

  // True if the Omaha rule instructs us to powerwash.
  bool powerwash_required = false;

  // If not blank, a base-64 encoded representation of the PEM-encoded
  // public key in the response.
  std::string public_key_rsa;

  // If not -1, the number of days since the epoch Jan 1, 2007 0:00
  // PST, according to the Omaha Server's clock and timezone (PST8PDT,
  // aka "Pacific Time".)
  int install_date_days = -1;

  // True if the returned image is a rollback for the device.
  bool is_rollback = false;

  struct RollbackKeyVersion {
    // Kernel key version. 0xffff if the value is unknown.
    uint16_t kernel_key = std::numeric_limits<uint16_t>::max();
    // Kernel version. 0xffff if the value is unknown.
    uint16_t kernel = std::numeric_limits<uint16_t>::max();
    // Firmware key verison. 0xffff if the value is unknown.
    uint16_t firmware_key = std::numeric_limits<uint16_t>::max();
    // Firmware version. 0xffff if the value is unknown.
    uint16_t firmware = std::numeric_limits<uint16_t>::max();
  };

  // Key versions of the returned rollback image. Values are 0xffff if the
  // image not a rollback, or the fields were not present.
  RollbackKeyVersion rollback_key_version;
};
static_assert(sizeof(off_t) == 8, "off_t not 64 bit");

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_OMAHA_RESPONSE_H_
