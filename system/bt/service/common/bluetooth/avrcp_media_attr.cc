//
//  Copyright 2017 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "bluetooth/avrcp_media_attr.h"

namespace bluetooth {

AvrcpMediaAttr::AvrcpMediaAttr() = default;
AvrcpMediaAttr::AvrcpMediaAttr(const AvrcpMediaAttr& other) = default;
AvrcpMediaAttr::AvrcpMediaAttr(const std::string& title,
                               const std::string& artist,
                               const std::string& album,
                               const std::string& genre, int track_num,
                               int num_tracks, int play_time)
    : title_(title),
      artist_(artist),
      album_(album),
      genre_(genre),
      track_num_(track_num),
      num_tracks_(num_tracks),
      play_time_(play_time) {}

AvrcpMediaAttr::~AvrcpMediaAttr() = default;

}  // namespace bluetooth
