/*
 * Copyright (C) 2017 The Android Open Source Project
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
 * See the License for the specic language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_LIBPERFMGR_REQUESTGROUP_H_
#define ANDROID_LIBPERFMGR_REQUESTGROUP_H_

#include <chrono>
#include <map>
#include <string>
#include <utility>

namespace android {
namespace perfmgr {

using ReqTime = std::chrono::time_point<std::chrono::steady_clock>;

// The RequestGroup type represents the set of requests for a given value on a
// particular sysfs node, and the interface is simple: there is a function to
// add requests, a function to remove requests, and a function to check for the
// next expiration time if there is an outstanding request, and a function to
// check the requested value. There may only be one request per PowerHint, so
// the representation is simple: a map from PowerHint to the expiration time for
// that hint.
class RequestGroup {
  public:
    RequestGroup(std::string request_value)  // NOLINT(runtime/explicit)
        : request_value_(std::move(request_value)) {}

    // Remove expired request in the map and return true when request_map_ is
    // not empty, false when request_map_ is empty; also update expire_time with
    // nearest timeout in request_map_ or std::chrono::milliseconds::max() when
    // request_map_ is empty.
    bool GetExpireTime(std::chrono::milliseconds* expire_time);
    // Return the request value.
    const std::string& GetRequestValue() const;
    // Return true for adding request, false for extending expire time of
    // existing active request on given hint_type.
    bool AddRequest(const std::string& hint_type, ReqTime end_time);
    // Return true for removing request, false if request is not active on given
    // hint_type. If request exits and the new end_time is less than the active
    // time, expire time will not be updated; also returns false.
    bool RemoveRequest(const std::string& hint_type);

  private:
    const std::string request_value_;
    std::map<std::string, ReqTime> request_map_;
};

}  // namespace perfmgr
}  // namespace android

#endif  // ANDROID_LIBPERFMGR_REQUESTGROUP_H_
