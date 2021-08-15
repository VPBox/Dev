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

#include "command.h"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <android-base/logging.h>
#include <android-base/parsedouble.h>
#include <android-base/parseint.h>
#include <android-base/quick_exit.h>

#include "utils.h"

bool Command::NextArgumentOrError(const std::vector<std::string>& args, size_t* pi) {
  if (*pi + 1 == args.size()) {
    LOG(ERROR) << "No argument following " << args[*pi] << " option. Try `simpleperf help " << name_
               << "`";
    return false;
  }
  ++*pi;
  return true;
}

bool Command::GetDoubleOption(const std::vector<std::string>& args, size_t* pi, double* value,
                              double min, double max) {
  if (!NextArgumentOrError(args, pi)) {
    return false;
  }
  if (!android::base::ParseDouble(args[*pi].c_str(), value, min, max)) {
    LOG(ERROR) << "Invalid argument for option " << args[*pi - 1] << ": " << args[*pi];
    return false;
  }
  return true;
}

void Command::ReportUnknownOption(const std::vector<std::string>& args, size_t i) {
  LOG(ERROR) << "Unknown option for " << name_ << " command: '" << args[i]
             << "'. Try `simpleperf help " << name_ << "`";
}

typedef std::function<std::unique_ptr<Command>(void)> callback_t;

static std::map<std::string, callback_t>& CommandMap() {
  // commands is used in the constructor of Command. Defining it as a static
  // variable in a function makes sure it is initialized before use.
  static std::map<std::string, callback_t> command_map;
  return command_map;
}

void RegisterCommand(const std::string& cmd_name,
                     const std::function<std::unique_ptr<Command>(void)>& callback) {
  CommandMap().insert(std::make_pair(cmd_name, callback));
}

void UnRegisterCommand(const std::string& cmd_name) {
  CommandMap().erase(cmd_name);
}

std::unique_ptr<Command> CreateCommandInstance(const std::string& cmd_name) {
  auto it = CommandMap().find(cmd_name);
  return (it == CommandMap().end()) ? nullptr : (it->second)();
}

const std::vector<std::string> GetAllCommandNames() {
  std::vector<std::string> names;
  for (const auto& pair : CommandMap()) {
    names.push_back(pair.first);
  }
  return names;
}

extern void RegisterDumpRecordCommand();
extern void RegisterHelpCommand();
extern void RegisterListCommand();
extern void RegisterKmemCommand();
extern void RegisterRecordCommand();
extern void RegisterReportCommand();
extern void RegisterReportSampleCommand();
extern void RegisterStatCommand();
extern void RegisterDebugUnwindCommand();
extern void RegisterTraceSchedCommand();
extern void RegisterAPICommands();

class CommandRegister {
 public:
  CommandRegister() {
    RegisterDumpRecordCommand();
    RegisterHelpCommand();
    RegisterKmemCommand();
    RegisterReportCommand();
    RegisterReportSampleCommand();
#if defined(__linux__)
    RegisterListCommand();
    RegisterRecordCommand();
    RegisterStatCommand();
    RegisterDebugUnwindCommand();
    RegisterTraceSchedCommand();
#if defined(__ANDROID__)
    RegisterAPICommands();
#endif
#endif
  }
};

CommandRegister command_register;

static void StderrLogger(android::base::LogId, android::base::LogSeverity severity,
                         const char*, const char* file, unsigned int line, const char* message) {
  static const char log_characters[] = "VDIWEFF";
  char severity_char = log_characters[severity];
  fprintf(stderr, "simpleperf %c %s:%u] %s\n", severity_char, file, line, message);
}

bool RunSimpleperfCmd(int argc, char** argv) {
  android::base::InitLogging(argv, StderrLogger);
  std::vector<std::string> args;
  android::base::LogSeverity log_severity = android::base::INFO;

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      args.insert(args.begin(), "help");
    } else if (strcmp(argv[i], "--log") == 0) {
      if (i + 1 < argc) {
        ++i;
        if (!GetLogSeverity(argv[i], &log_severity)) {
          LOG(ERROR) << "Unknown log severity: " << argv[i];
          return false;
        }
      } else {
        LOG(ERROR) << "Missing argument for --log option.\n";
        return false;
      }
#if defined(__ANDROID__)
    } else if (strcmp(argv[i], "--log-to-android-buffer") == 0) {
      android::base::SetLogger(android::base::LogdLogger());
#endif
    } else if (strcmp(argv[i], "--version") == 0) {
      LOG(INFO) << "Simpleperf version " << GetSimpleperfVersion();
      return true;
    } else {
      args.push_back(argv[i]);
    }
  }
  android::base::ScopedLogSeverity severity(log_severity);

  if (args.empty()) {
    args.push_back("help");
  }
  std::unique_ptr<Command> command = CreateCommandInstance(args[0]);
  if (command == nullptr) {
    LOG(ERROR) << "malformed command line: unknown command " << args[0];
    return false;
  }
  std::string command_name = args[0];
  args.erase(args.begin());

  LOG(DEBUG) << "command '" << command_name << "' starts running";
  bool result = command->Run(args);
  LOG(DEBUG) << "command '" << command_name << "' "
             << (result ? "finished successfully" : "failed");
  // Quick exit to avoid cost freeing memory and closing files.
  fflush(stdout);
  fflush(stderr);
  android::base::quick_exit(result ? 0 : 1);
  return result;
}
