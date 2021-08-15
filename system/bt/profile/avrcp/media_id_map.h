/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once

#include <map>

namespace bluetooth {
namespace avrcp {

// A helper class to convert Media ID's (represented as strings) that are
// received from the AVRCP Media Interface layer into UID's to be used
// with connected devices.
class MediaIdMap {
 public:
  void clear() {
    media_id_to_uid_.clear();
    uid_to_media_id_.clear();
  }

  std::string get_media_id(uint64_t uid) {
    const auto& uid_it = uid_to_media_id_.find(uid);
    if (uid_it == uid_to_media_id_.end()) return "";
    return uid_it->second;
  }

  uint64_t get_uid(std::string media_id) {
    const auto& media_id_it = media_id_to_uid_.find(media_id);
    if (media_id_it == media_id_to_uid_.end()) return 0;
    return media_id_it->second;
  }

  uint64_t insert(std::string media_id) {
    if (media_id_to_uid_.find(media_id) != media_id_to_uid_.end()) {
      return media_id_to_uid_[media_id];
    }

    uint64_t uid = media_id_to_uid_.size() + 1;
    media_id_to_uid_.insert(std::pair<std::string, uint64_t>(media_id, uid));
    uid_to_media_id_.insert(std::pair<uint64_t, std::string>(uid, media_id));
    return uid;
  }

 private:
  std::map<std::string, uint64_t> media_id_to_uid_;
  std::map<uint64_t, std::string> uid_to_media_id_;
};

}  // namespace avrcp
}  // namespace bluetooth
