/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef SIMPLE_PERF_COMMAND_H_
#define SIMPLE_PERF_COMMAND_H_

#include <functional>
#include <memory>
#include <limits>
#include <string>
#include <vector>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/parseint.h>

class Command {
 public:
  Command(const std::string& name, const std::string& short_help_string,
          const std::string& long_help_string)
      : name_(name), short_help_string_(short_help_string), long_help_string_(long_help_string) {
  }

  virtual ~Command() {
  }

  const std::string& Name() const {
    return name_;
  }

  const std::string& ShortHelpString() const {
    return short_help_string_;
  }

  const std::string LongHelpString() const {
    return long_help_string_;
  }

  virtual bool Run(const std::vector<std::string>& args) = 0;

  template <typename T>
  bool GetUintOption(const std::vector<std::string>& args, size_t* pi, T* value, uint64_t min = 0,
                     uint64_t max = std::numeric_limits<T>::max(), bool allow_suffixes = false) {
    if (!NextArgumentOrError(args, pi)) {
      return false;
    }
    uint64_t tmp_value;
    if (!android::base::ParseUint(args[*pi], &tmp_value, max, allow_suffixes) || tmp_value < min) {
      LOG(ERROR) << "Invalid argument for option " << args[*pi - 1] << ": " << args[*pi];
      return false;
    }
    *value = static_cast<T>(tmp_value);
    return true;
  }

  bool GetDoubleOption(const std::vector<std::string>& args, size_t* pi, double* value,
                       double min = 0, double max = std::numeric_limits<double>::max());

 protected:
  bool NextArgumentOrError(const std::vector<std::string>& args, size_t* pi);
  void ReportUnknownOption(const std::vector<std::string>& args, size_t i);

 private:
  const std::string name_;
  const std::string short_help_string_;
  const std::string long_help_string_;

  DISALLOW_COPY_AND_ASSIGN(Command);
};

void RegisterCommand(const std::string& cmd_name,
                     const std::function<std::unique_ptr<Command>(void)>& callback);
void UnRegisterCommand(const std::string& cmd_name);
std::unique_ptr<Command> CreateCommandInstance(const std::string& cmd_name);
const std::vector<std::string> GetAllCommandNames();
bool RunSimpleperfCmd(int argc, char** argv);

#endif  // SIMPLE_PERF_COMMAND_H_
