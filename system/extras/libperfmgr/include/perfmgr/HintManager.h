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

#ifndef ANDROID_LIBPERFMGR_HINTMANAGER_H_
#define ANDROID_LIBPERFMGR_HINTMANAGER_H_

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "perfmgr/NodeLooperThread.h"

namespace android {
namespace perfmgr {

// HintManager is the external interface of the library to be used by PowerHAL
// to do power hints with sysfs nodes. HintManager maintains a representation of
// the actions that are parsed from the configuration file as a mapping from a
// PowerHint to the set of actions that are performed for that PowerHint.
class HintManager {
  public:
    HintManager(sp<NodeLooperThread> nm,
                const std::map<std::string, std::vector<NodeAction>>& actions)
        : nm_(std::move(nm)), actions_(actions) {}
    ~HintManager() {
        if (nm_.get() != nullptr) nm_->Stop();
    }

    // Return true if the sysfs manager thread is running.
    bool IsRunning() const;

    // Do hint based on hint_type which defined as PowerHint in the actions
    // section of the JSON config. Return true with valid hint_type and also
    // NodeLooperThread::Request succeeds; otherwise return false.
    bool DoHint(const std::string& hint_type);

    // Do hint with the override time for all actions defined for the given
    // hint_type.  Return true with valid hint_type and also
    // NodeLooperThread::Request succeeds; otherwise return false.
    bool DoHint(const std::string& hint_type,
                std::chrono::milliseconds timeout_ms_override);

    // End hint early. Return true with valid hint_type and also
    // NodeLooperThread::Cancel succeeds; otherwise return false.
    bool EndHint(const std::string& hint_type);

    // Static method to construct HintManager from the JSON config file.
    static std::unique_ptr<HintManager> GetFromJSON(
        const std::string& config_path);

    // Return available hints managed by HintManager
    std::vector<std::string> GetHints() const;

    // Dump internal status to fd
    void DumpToFd(int fd);

  protected:
    static std::vector<std::unique_ptr<Node>> ParseNodes(
        const std::string& json_doc);
    static std::map<std::string, std::vector<NodeAction>> ParseActions(
        const std::string& json_doc,
        const std::vector<std::unique_ptr<Node>>& nodes);

  private:
    HintManager(HintManager const&) = delete;
    void operator=(HintManager const&) = delete;
    bool ValidateHint(const std::string& hint_type) const;

    sp<NodeLooperThread> nm_;
    const std::map<std::string, std::vector<NodeAction>> actions_;
};

}  // namespace perfmgr
}  // namespace android

#endif  // ANDROID_LIBPERFMGR_HINTMANAGER_H_
