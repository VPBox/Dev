//
//  Copyright (C) 2017 Google, Inc.
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

#pragma once

#include <string>

namespace bluetooth {

class AvrcpMediaAttr {
 public:
  AvrcpMediaAttr();
  AvrcpMediaAttr(const AvrcpMediaAttr& other);
  AvrcpMediaAttr(const std::string& title, const std::string& artist,
                 const std::string& album, const std::string& genre,
                 int track_num, int num_tracks, int play_time);
  ~AvrcpMediaAttr();

  const std::string& title() const { return title_; }
  const std::string& artist() const { return artist_; }
  const std::string& album() const { return album_; }
  const std::string& genre() const { return genre_; }
  int track_num() const { return track_num_; }
  int num_tracks() const { return num_tracks_; }
  int play_time() const { return play_time_; }

 protected:
  std::string title_;
  std::string artist_;
  std::string album_;
  std::string genre_;
  int track_num_ = -1;
  int num_tracks_ = -1;
  int play_time_ = -1;
};

}  // namespace bluetooth
