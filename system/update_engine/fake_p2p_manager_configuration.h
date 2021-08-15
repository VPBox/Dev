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

#ifndef UPDATE_ENGINE_FAKE_P2P_MANAGER_CONFIGURATION_H_
#define UPDATE_ENGINE_FAKE_P2P_MANAGER_CONFIGURATION_H_

#include "update_engine/p2p_manager.h"

#include <string>
#include <vector>

#include <base/files/scoped_temp_dir.h>
#include <base/strings/string_util.h>
#include <gtest/gtest.h>

namespace chromeos_update_engine {

// Configuration for P2PManager for use in unit tests. Instead of
// /var/cache/p2p, a temporary directory is used.
class FakeP2PManagerConfiguration : public P2PManager::Configuration {
 public:
  FakeP2PManagerConfiguration() { EXPECT_TRUE(p2p_dir_.CreateUniqueTempDir()); }

  // P2PManager::Configuration override
  base::FilePath GetP2PDir() override { return p2p_dir_.GetPath(); }

  // P2PManager::Configuration override
  std::vector<std::string> GetInitctlArgs(bool is_start) override {
    return is_start ? initctl_start_args_ : initctl_stop_args_;
  }

  // P2PManager::Configuration override
  std::vector<std::string> GetP2PClientArgs(const std::string& file_id,
                                            size_t minimum_size) override {
    std::vector<std::string> formatted_command = p2p_client_cmd_format_;
    // Replace {variable} on the passed string.
    std::string str_minimum_size = std::to_string(minimum_size);
    for (std::string& arg : formatted_command) {
      base::ReplaceSubstringsAfterOffset(&arg, 0, "{file_id}", file_id);
      base::ReplaceSubstringsAfterOffset(
          &arg, 0, "{minsize}", str_minimum_size);
    }
    return formatted_command;
  }

  // Use |command_line| instead of "initctl start p2p" when attempting
  // to start the p2p service.
  void SetInitctlStartCommand(const std::vector<std::string>& command) {
    initctl_start_args_ = command;
  }

  // Use |command_line| instead of "initctl stop p2p" when attempting
  // to stop the p2p service.
  void SetInitctlStopCommand(const std::vector<std::string>& command) {
    initctl_stop_args_ = command;
  }

  // Use |command_format| instead of "p2p-client --get-url={file_id}
  // --minimum-size={minsize}" when attempting to look up a file using
  // p2p-client(1).
  //
  // The passed |command_format| argument can have "{file_id}" and "{minsize}"
  // as substrings of any of its elements, that will be replaced by the
  // corresponding values passed to GetP2PClientArgs().
  void SetP2PClientCommand(const std::vector<std::string>& command_format) {
    p2p_client_cmd_format_ = command_format;
  }

 private:
  // The temporary directory used for p2p.
  base::ScopedTempDir p2p_dir_;

  // Argument vector for starting p2p.
  std::vector<std::string> initctl_start_args_{"initctl", "start", "p2p"};

  // Argument vector for stopping p2p.
  std::vector<std::string> initctl_stop_args_{"initctl", "stop", "p2p"};

  // A string for generating the p2p-client command. See the
  // SetP2PClientCommandLine() for details.
  std::vector<std::string> p2p_client_cmd_format_{
      "p2p-client", "--get-url={file_id}", "--minimum-size={minsize}"};

  DISALLOW_COPY_AND_ASSIGN(FakeP2PManagerConfiguration);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_FAKE_P2P_MANAGER_CONFIGURATION_H_
