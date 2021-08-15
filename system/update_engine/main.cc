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

#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <xz.h>

#include <algorithm>
#include <string>
#include <vector>

#include <base/at_exit.h>
#include <base/command_line.h>
#include <base/files/dir_reader_posix.h>
#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <brillo/flag_helper.h>

#include "update_engine/common/terminator.h"
#include "update_engine/common/utils.h"
#include "update_engine/daemon.h"

using std::string;

namespace chromeos_update_engine {
namespace {

string GetTimeAsString(time_t utime) {
  struct tm tm;
  CHECK_EQ(localtime_r(&utime, &tm), &tm);
  char str[16];
  CHECK_EQ(strftime(str, sizeof(str), "%Y%m%d-%H%M%S", &tm), 15u);
  return str;
}

#ifdef __ANDROID__
constexpr char kSystemLogsRoot[] = "/data/misc/update_engine_log";
constexpr size_t kLogCount = 5;

// Keep the most recent |kLogCount| logs but remove the old ones in
// "/data/misc/update_engine_log/".
void DeleteOldLogs(const string& kLogsRoot) {
  base::DirReaderPosix reader(kLogsRoot.c_str());
  if (!reader.IsValid()) {
    LOG(ERROR) << "Failed to read " << kLogsRoot;
    return;
  }

  std::vector<string> old_logs;
  while (reader.Next()) {
    if (reader.name()[0] == '.')
      continue;

    // Log files are in format "update_engine.%Y%m%d-%H%M%S",
    // e.g. update_engine.20090103-231425
    uint64_t date;
    uint64_t local_time;
    if (sscanf(reader.name(),
               "update_engine.%" PRIu64 "-%" PRIu64 "",
               &date,
               &local_time) == 2) {
      old_logs.push_back(reader.name());
    } else {
      LOG(WARNING) << "Unrecognized log file " << reader.name();
    }
  }

  std::sort(old_logs.begin(), old_logs.end(), std::greater<string>());
  for (size_t i = kLogCount; i < old_logs.size(); i++) {
    string log_path = kLogsRoot + "/" + old_logs[i];
    if (unlink(log_path.c_str()) == -1) {
      PLOG(WARNING) << "Failed to unlink " << log_path;
    }
  }
}

string SetupLogFile(const string& kLogsRoot) {
  DeleteOldLogs(kLogsRoot);

  return base::StringPrintf("%s/update_engine.%s",
                            kLogsRoot.c_str(),
                            GetTimeAsString(::time(nullptr)).c_str());
}
#else
constexpr char kSystemLogsRoot[] = "/var/log";

void SetupLogSymlink(const string& symlink_path, const string& log_path) {
  // TODO(petkov): To ensure a smooth transition between non-timestamped and
  // timestamped logs, move an existing log to start the first timestamped
  // one. This code can go away once all clients are switched to this version or
  // we stop caring about the old-style logs.
  if (utils::FileExists(symlink_path.c_str()) &&
      !utils::IsSymlink(symlink_path.c_str())) {
    base::ReplaceFile(
        base::FilePath(symlink_path), base::FilePath(log_path), nullptr);
  }
  base::DeleteFile(base::FilePath(symlink_path), true);
  if (symlink(log_path.c_str(), symlink_path.c_str()) == -1) {
    PLOG(ERROR) << "Unable to create symlink " << symlink_path
                << " pointing at " << log_path;
  }
}

string SetupLogFile(const string& kLogsRoot) {
  const string kLogSymlink = kLogsRoot + "/update_engine.log";
  const string kLogsDir = kLogsRoot + "/update_engine";
  const string kLogPath =
      base::StringPrintf("%s/update_engine.%s",
                         kLogsDir.c_str(),
                         GetTimeAsString(::time(nullptr)).c_str());
  mkdir(kLogsDir.c_str(), 0755);
  SetupLogSymlink(kLogSymlink, kLogPath);
  return kLogSymlink;
}
#endif  // __ANDROID__

void SetupLogging(bool log_to_system, bool log_to_file) {
  logging::LoggingSettings log_settings;
  log_settings.lock_log = logging::DONT_LOCK_LOG_FILE;
  log_settings.logging_dest = static_cast<logging::LoggingDestination>(
      (log_to_system ? logging::LOG_TO_SYSTEM_DEBUG_LOG : 0) |
      (log_to_file ? logging::LOG_TO_FILE : 0));
  log_settings.log_file = nullptr;

  string log_file;
  if (log_to_file) {
    log_file = SetupLogFile(kSystemLogsRoot);
    log_settings.delete_old = logging::APPEND_TO_OLD_LOG_FILE;
    log_settings.log_file = log_file.c_str();
  }
  logging::InitLogging(log_settings);

#ifdef __ANDROID__
  // The log file will have AID_LOG as group ID; this GID is inherited from the
  // parent directory "/data/misc/update_engine_log" which sets the SGID bit.
  chmod(log_file.c_str(), 0640);
#endif
}

}  // namespace
}  // namespace chromeos_update_engine

int main(int argc, char** argv) {
  DEFINE_bool(logtofile, false, "Write logs to a file in log_dir.");
  DEFINE_bool(logtostderr,
              false,
              "Write logs to stderr instead of to a file in log_dir.");
  DEFINE_bool(foreground, false, "Don't daemon()ize; run in foreground.");

  chromeos_update_engine::Terminator::Init();
  brillo::FlagHelper::Init(argc, argv, "A/B Update Engine");

  // We have two logging flags "--logtostderr" and "--logtofile"; and the logic
  // to choose the logging destination is:
  // 1. --logtostderr --logtofile -> logs to both
  // 2. --logtostderr             -> logs to system debug
  // 3. --logtofile or no flags   -> logs to file
  bool log_to_system = FLAGS_logtostderr;
  bool log_to_file = FLAGS_logtofile || !FLAGS_logtostderr;
  chromeos_update_engine::SetupLogging(log_to_system, log_to_file);
  if (!FLAGS_foreground)
    PLOG_IF(FATAL, daemon(0, 0) == 1) << "daemon() failed";

  LOG(INFO) << "A/B Update Engine starting";

  // xz-embedded requires to initialize its CRC-32 table once on startup.
  xz_crc32_init();

  // Ensure that all written files have safe permissions.
  // This is a mask, so we _block_ all permissions for the group owner and other
  // users but allow all permissions for the user owner. We allow execution
  // for the owner so we can create directories.
  // Done _after_ log file creation.
  umask(S_IRWXG | S_IRWXO);

  chromeos_update_engine::UpdateEngineDaemon update_engine_daemon;
  int exit_code = update_engine_daemon.Run();

  chromeos_update_engine::Subprocess::Get().FlushBufferedLogsAtExit();

  LOG(INFO) << "A/B Update Engine terminating with exit code " << exit_code;
  return exit_code;
}
