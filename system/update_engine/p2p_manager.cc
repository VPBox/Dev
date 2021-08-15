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

// This provides access to timestamps with nanosecond resolution in
// struct stat, See NOTES in stat(2) for details.
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include "update_engine/p2p_manager.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/falloc.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>

#include <algorithm>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/files/file_enumerator.h>
#include <base/files/file_path.h>
#include <base/format_macros.h>
#include <base/logging.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/subprocess.h"
#include "update_engine/common/utils.h"
#include "update_engine/update_manager/policy.h"
#include "update_engine/update_manager/update_manager.h"

using base::Bind;
using base::Callback;
using base::FilePath;
using base::StringPrintf;
using base::Time;
using base::TimeDelta;
using brillo::MessageLoop;
using chromeos_update_manager::EvalStatus;
using chromeos_update_manager::Policy;
using chromeos_update_manager::UpdateManager;
using std::map;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

namespace {

// The default p2p directory.
const char kDefaultP2PDir[] = "/var/cache/p2p";

// The p2p xattr used for conveying the final size of a file - see the
// p2p ddoc for details.
const char kCrosP2PFileSizeXAttrName[] = "user.cros-p2p-filesize";

}  // namespace

// The default P2PManager::Configuration implementation.
class ConfigurationImpl : public P2PManager::Configuration {
 public:
  ConfigurationImpl() {}

  FilePath GetP2PDir() override { return FilePath(kDefaultP2PDir); }

  vector<string> GetInitctlArgs(bool is_start) override {
    vector<string> args;
    args.push_back("initctl");
    args.push_back(is_start ? "start" : "stop");
    args.push_back("p2p");
    return args;
  }

  vector<string> GetP2PClientArgs(const string& file_id,
                                  size_t minimum_size) override {
    vector<string> args;
    args.push_back("p2p-client");
    args.push_back(string("--get-url=") + file_id);
    args.push_back(StringPrintf("--minimum-size=%" PRIuS, minimum_size));
    return args;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigurationImpl);
};

// The default P2PManager implementation.
class P2PManagerImpl : public P2PManager {
 public:
  P2PManagerImpl(Configuration* configuration,
                 ClockInterface* clock,
                 UpdateManager* update_manager,
                 const string& file_extension,
                 const int num_files_to_keep,
                 const TimeDelta& max_file_age);

  // P2PManager methods.
  void SetDevicePolicy(const policy::DevicePolicy* device_policy) override;
  bool IsP2PEnabled() override;
  bool EnsureP2PRunning() override;
  bool EnsureP2PNotRunning() override;
  bool PerformHousekeeping() override;
  void LookupUrlForFile(const string& file_id,
                        size_t minimum_size,
                        TimeDelta max_time_to_wait,
                        LookupCallback callback) override;
  bool FileShare(const string& file_id, size_t expected_size) override;
  FilePath FileGetPath(const string& file_id) override;
  ssize_t FileGetSize(const string& file_id) override;
  ssize_t FileGetExpectedSize(const string& file_id) override;
  bool FileGetVisible(const string& file_id, bool* out_result) override;
  bool FileMakeVisible(const string& file_id) override;
  int CountSharedFiles() override;

 private:
  // Enumeration for specifying visibility.
  enum Visibility { kVisible, kNonVisible };

  // Returns "." + |file_extension_| + ".p2p" if |visibility| is
  // |kVisible|. Returns the same concatenated with ".tmp" otherwise.
  string GetExt(Visibility visibility);

  // Gets the on-disk path for |file_id| depending on if the file
  // is visible or not.
  FilePath GetPath(const string& file_id, Visibility visibility);

  // Utility function used by EnsureP2PRunning() and EnsureP2PNotRunning().
  bool EnsureP2P(bool should_be_running);

  // Utility function to delete a file given by |path| and log the
  // path as well as |reason|. Returns false on failure.
  bool DeleteP2PFile(const FilePath& path, const string& reason);

  // Schedules an async request for tracking changes in P2P enabled status.
  void ScheduleEnabledStatusChange();

  // An async callback used by the above.
  void OnEnabledStatusChange(EvalStatus status, const bool& result);

  // The device policy being used or null if no policy is being used.
  const policy::DevicePolicy* device_policy_ = nullptr;

  // Configuration object.
  unique_ptr<Configuration> configuration_;

  // Object for telling the time.
  ClockInterface* clock_;

  // A pointer to the global Update Manager.
  UpdateManager* update_manager_;

  // A short string unique to the application (for example "cros_au")
  // used to mark a file as being owned by a particular application.
  const string file_extension_;

  // If non-zero, this number denotes how many files in /var/cache/p2p
  // owned by the application (cf. |file_extension_|) to keep after
  // performing housekeeping.
  const int num_files_to_keep_;

  // If non-zero, files older than this will not be kept after
  // performing housekeeping.
  const TimeDelta max_file_age_;

  // The string ".p2p".
  static const char kP2PExtension[];

  // The string ".tmp".
  static const char kTmpExtension[];

  // Whether P2P service may be running; initially, we assume it may be.
  bool may_be_running_ = true;

  // The current known enabled status of the P2P feature (initialized lazily),
  // and whether an async status check has been scheduled.
  bool is_enabled_;
  bool waiting_for_enabled_status_change_ = false;

  DISALLOW_COPY_AND_ASSIGN(P2PManagerImpl);
};

const char P2PManagerImpl::kP2PExtension[] = ".p2p";

const char P2PManagerImpl::kTmpExtension[] = ".tmp";

P2PManagerImpl::P2PManagerImpl(Configuration* configuration,
                               ClockInterface* clock,
                               UpdateManager* update_manager,
                               const string& file_extension,
                               const int num_files_to_keep,
                               const TimeDelta& max_file_age)
    : clock_(clock),
      update_manager_(update_manager),
      file_extension_(file_extension),
      num_files_to_keep_(num_files_to_keep),
      max_file_age_(max_file_age) {
  configuration_.reset(configuration != nullptr ? configuration
                                                : new ConfigurationImpl());
}

void P2PManagerImpl::SetDevicePolicy(
    const policy::DevicePolicy* device_policy) {
  device_policy_ = device_policy;
}

bool P2PManagerImpl::IsP2PEnabled() {
  if (!waiting_for_enabled_status_change_) {
    // Get and store an initial value.
    if (update_manager_->PolicyRequest(&Policy::P2PEnabled, &is_enabled_) ==
        EvalStatus::kFailed) {
      is_enabled_ = false;
      LOG(ERROR) << "Querying P2P enabled status failed, disabling.";
    }

    // Track future changes (async).
    ScheduleEnabledStatusChange();
  }

  return is_enabled_;
}

bool P2PManagerImpl::EnsureP2P(bool should_be_running) {
  int return_code = 0;
  string output;

  may_be_running_ = true;  // Unless successful, we must be conservative.

  vector<string> args = configuration_->GetInitctlArgs(should_be_running);
  if (!Subprocess::SynchronousExec(args, &return_code, &output)) {
    LOG(ERROR) << "Error spawning " << utils::StringVectorToString(args);
    return false;
  }

  // If initctl(8) does not exit normally (exit status other than zero), ensure
  // that the error message is not benign by scanning stderr; this is a
  // necessity because initctl does not offer actions such as "start if not
  // running" or "stop if running".
  // TODO(zeuthen,chromium:277051): Avoid doing this.
  if (return_code != 0) {
    const char* expected_error_message =
        should_be_running ? "initctl: Job is already running: p2p\n"
                          : "initctl: Unknown instance \n";
    if (output != expected_error_message)
      return false;
  }

  may_be_running_ = should_be_running;  // Successful after all.
  return true;
}

bool P2PManagerImpl::EnsureP2PRunning() {
  return EnsureP2P(true);
}

bool P2PManagerImpl::EnsureP2PNotRunning() {
  return EnsureP2P(false);
}

// Returns True if the timestamp in the first pair is greater than the
// timestamp in the latter. If used with std::sort() this will yield a
// sequence of elements where newer (high timestamps) elements precede
// older ones (low timestamps).
static bool MatchCompareFunc(const pair<FilePath, Time>& a,
                             const pair<FilePath, Time>& b) {
  return a.second > b.second;
}

string P2PManagerImpl::GetExt(Visibility visibility) {
  string ext = string(".") + file_extension_ + kP2PExtension;
  switch (visibility) {
    case kVisible:
      break;
    case kNonVisible:
      ext += kTmpExtension;
      break;
      // Don't add a default case to let the compiler warn about newly
      // added enum values.
  }
  return ext;
}

FilePath P2PManagerImpl::GetPath(const string& file_id, Visibility visibility) {
  return configuration_->GetP2PDir().Append(file_id + GetExt(visibility));
}

bool P2PManagerImpl::DeleteP2PFile(const FilePath& path, const string& reason) {
  LOG(INFO) << "Deleting p2p file " << path.value() << " (reason: " << reason
            << ")";
  if (unlink(path.value().c_str()) != 0) {
    PLOG(ERROR) << "Error deleting p2p file " << path.value();
    return false;
  }
  return true;
}

bool P2PManagerImpl::PerformHousekeeping() {
  // Open p2p dir.
  FilePath p2p_dir = configuration_->GetP2PDir();
  const string ext_visible = GetExt(kVisible);
  const string ext_non_visible = GetExt(kNonVisible);

  bool deletion_failed = false;
  vector<pair<FilePath, Time>> matches;

  base::FileEnumerator dir(p2p_dir, false, base::FileEnumerator::FILES);
  // Go through all files and collect their mtime.
  for (FilePath name = dir.Next(); !name.empty(); name = dir.Next()) {
    if (!(base::EndsWith(
              name.value(), ext_visible, base::CompareCase::SENSITIVE) ||
          base::EndsWith(
              name.value(), ext_non_visible, base::CompareCase::SENSITIVE))) {
      continue;
    }

    Time time = dir.GetInfo().GetLastModifiedTime();

    // If instructed to keep only files younger than a given age
    // (|max_file_age_| != 0), delete files satisfying this criteria
    // right now. Otherwise add it to a list we'll consider for later.
    if (clock_ != nullptr && max_file_age_ != TimeDelta() &&
        clock_->GetWallclockTime() - time > max_file_age_) {
      if (!DeleteP2PFile(name, "file too old"))
        deletion_failed = true;
    } else {
      matches.push_back(std::make_pair(name, time));
    }
  }

  // If instructed to only keep N files (|max_files_to_keep_ != 0),
  // sort list of matches, newest (biggest time) to oldest (lowest
  // time). Then delete starting at element |num_files_to_keep_|.
  if (num_files_to_keep_ > 0) {
    std::sort(matches.begin(), matches.end(), MatchCompareFunc);
    vector<pair<FilePath, Time>>::const_iterator i;
    for (i = matches.begin() + num_files_to_keep_; i < matches.end(); ++i) {
      if (!DeleteP2PFile(i->first, "too many files"))
        deletion_failed = true;
    }
  }

  return !deletion_failed;
}

// Helper class for implementing LookupUrlForFile().
class LookupData {
 public:
  explicit LookupData(P2PManager::LookupCallback callback)
      : callback_(callback) {}

  ~LookupData() {
    if (timeout_task_ != MessageLoop::kTaskIdNull)
      MessageLoop::current()->CancelTask(timeout_task_);
    if (child_pid_)
      Subprocess::Get().KillExec(child_pid_);
  }

  void InitiateLookup(const vector<string>& cmd, TimeDelta timeout) {
    // NOTE: if we fail early (i.e. in this method), we need to schedule
    // an idle to report the error. This is because we guarantee that
    // the callback is always called from the message loop (this
    // guarantee is useful for testing).

    // We expect to run just "p2p-client" and find it in the path.
    child_pid_ = Subprocess::Get().ExecFlags(
        cmd,
        Subprocess::kSearchPath,
        {},
        Bind(&LookupData::OnLookupDone, base::Unretained(this)));

    if (!child_pid_) {
      LOG(ERROR) << "Error spawning " << utils::StringVectorToString(cmd);
      ReportErrorAndDeleteInIdle();
      return;
    }

    if (timeout > TimeDelta()) {
      timeout_task_ = MessageLoop::current()->PostDelayedTask(
          FROM_HERE,
          Bind(&LookupData::OnTimeout, base::Unretained(this)),
          timeout);
    }
  }

 private:
  void ReportErrorAndDeleteInIdle() {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        Bind(&LookupData::OnIdleForReportErrorAndDelete,
             base::Unretained(this)));
  }

  void OnIdleForReportErrorAndDelete() {
    ReportError();
    delete this;
  }

  void IssueCallback(const string& url) {
    if (!callback_.is_null())
      callback_.Run(url);
  }

  void ReportError() {
    if (reported_)
      return;
    IssueCallback("");
    reported_ = true;
  }

  void ReportSuccess(const string& output) {
    if (reported_)
      return;
    string url = output;
    size_t newline_pos = url.find('\n');
    if (newline_pos != string::npos)
      url.resize(newline_pos);

    // Since p2p-client(1) is constructing this URL itself strictly
    // speaking there's no need to validate it... but, anyway, can't
    // hurt.
    if (url.compare(0, 7, "http://") == 0) {
      IssueCallback(url);
    } else {
      LOG(ERROR) << "p2p URL '" << url << "' does not look right. Ignoring.";
      ReportError();
    }
    reported_ = true;
  }

  void OnLookupDone(int return_code, const string& output) {
    child_pid_ = 0;
    if (return_code != 0) {
      LOG(INFO) << "Child exited with non-zero exit code " << return_code;
      ReportError();
    } else {
      ReportSuccess(output);
    }
    delete this;
  }

  void OnTimeout() {
    timeout_task_ = MessageLoop::kTaskIdNull;
    ReportError();
    delete this;
  }

  P2PManager::LookupCallback callback_;

  // The Subprocess tag of the running process. A value of 0 means that the
  // process is not running.
  pid_t child_pid_{0};

  // The timeout task_id we are waiting on, if any.
  MessageLoop::TaskId timeout_task_{MessageLoop::kTaskIdNull};

  bool reported_{false};
};

void P2PManagerImpl::LookupUrlForFile(const string& file_id,
                                      size_t minimum_size,
                                      TimeDelta max_time_to_wait,
                                      LookupCallback callback) {
  LookupData* lookup_data = new LookupData(callback);
  string file_id_with_ext = file_id + "." + file_extension_;
  vector<string> args =
      configuration_->GetP2PClientArgs(file_id_with_ext, minimum_size);
  lookup_data->InitiateLookup(args, max_time_to_wait);
}

bool P2PManagerImpl::FileShare(const string& file_id, size_t expected_size) {
  // Check if file already exist.
  FilePath path = FileGetPath(file_id);
  if (!path.empty()) {
    // File exists - double check its expected size though.
    ssize_t file_expected_size = FileGetExpectedSize(file_id);
    if (file_expected_size == -1 ||
        static_cast<size_t>(file_expected_size) != expected_size) {
      LOG(ERROR) << "Existing p2p file " << path.value()
                 << " with expected_size=" << file_expected_size
                 << " does not match the passed in"
                 << " expected_size=" << expected_size;
      return false;
    }
    return true;
  }

  // Before creating the file, bail if statvfs(3) indicates that at
  // least twice the size is not available in P2P_DIR.
  struct statvfs statvfsbuf;
  FilePath p2p_dir = configuration_->GetP2PDir();
  if (statvfs(p2p_dir.value().c_str(), &statvfsbuf) != 0) {
    PLOG(ERROR) << "Error calling statvfs() for dir " << p2p_dir.value();
    return false;
  }
  size_t free_bytes =
      static_cast<size_t>(statvfsbuf.f_bsize) * statvfsbuf.f_bavail;
  if (free_bytes < 2 * expected_size) {
    // This can easily happen and is worth reporting.
    LOG(INFO) << "Refusing to allocate p2p file of " << expected_size
              << " bytes since the directory " << p2p_dir.value()
              << " only has " << free_bytes
              << " bytes available and this is less than twice the"
              << " requested size.";
    return false;
  }

  // Okie-dokey looks like enough space is available - create the file.
  path = GetPath(file_id, kNonVisible);
  int fd = open(path.value().c_str(), O_CREAT | O_RDWR, 0644);
  if (fd == -1) {
    PLOG(ERROR) << "Error creating file with path " << path.value();
    return false;
  }
  ScopedFdCloser fd_closer(&fd);

  // If the final size is known, allocate the file (e.g. reserve disk
  // space) and set the user.cros-p2p-filesize xattr.
  if (expected_size != 0) {
    if (fallocate(fd,
                  FALLOC_FL_KEEP_SIZE,  // Keep file size as 0.
                  0,
                  expected_size) != 0) {
      if (errno == ENOSYS || errno == EOPNOTSUPP) {
        // If the filesystem doesn't support the fallocate, keep
        // going. This is helpful when running unit tests on build
        // machines with ancient filesystems and/or OSes.
        PLOG(WARNING) << "Ignoring fallocate(2) failure";
      } else {
        // ENOSPC can happen (funky race though, cf. the statvfs() check
        // above), handle it gracefully, e.g. use logging level INFO.
        PLOG(INFO) << "Error allocating " << expected_size << " bytes for file "
                   << path.value();
        if (unlink(path.value().c_str()) != 0) {
          PLOG(ERROR) << "Error deleting file with path " << path.value();
        }
        return false;
      }
    }

    string decimal_size = std::to_string(expected_size);
    if (fsetxattr(fd,
                  kCrosP2PFileSizeXAttrName,
                  decimal_size.c_str(),
                  decimal_size.size(),
                  0) != 0) {
      PLOG(ERROR) << "Error setting xattr " << path.value();
      return false;
    }
  }

  return true;
}

FilePath P2PManagerImpl::FileGetPath(const string& file_id) {
  struct stat statbuf;
  FilePath path;

  path = GetPath(file_id, kVisible);
  if (stat(path.value().c_str(), &statbuf) == 0) {
    return path;
  }

  path = GetPath(file_id, kNonVisible);
  if (stat(path.value().c_str(), &statbuf) == 0) {
    return path;
  }

  path.clear();
  return path;
}

bool P2PManagerImpl::FileGetVisible(const string& file_id, bool* out_result) {
  FilePath path = FileGetPath(file_id);
  if (path.empty()) {
    LOG(ERROR) << "No file for id " << file_id;
    return false;
  }
  if (out_result != nullptr)
    *out_result = path.MatchesExtension(kP2PExtension);
  return true;
}

bool P2PManagerImpl::FileMakeVisible(const string& file_id) {
  FilePath path = FileGetPath(file_id);
  if (path.empty()) {
    LOG(ERROR) << "No file for id " << file_id;
    return false;
  }

  // Already visible?
  if (path.MatchesExtension(kP2PExtension))
    return true;

  LOG_ASSERT(path.MatchesExtension(kTmpExtension));
  FilePath new_path = path.RemoveExtension();
  LOG_ASSERT(new_path.MatchesExtension(kP2PExtension));
  if (rename(path.value().c_str(), new_path.value().c_str()) != 0) {
    PLOG(ERROR) << "Error renaming " << path.value() << " to "
                << new_path.value();
    return false;
  }

  return true;
}

ssize_t P2PManagerImpl::FileGetSize(const string& file_id) {
  FilePath path = FileGetPath(file_id);
  if (path.empty())
    return -1;

  return utils::FileSize(path.value());
}

ssize_t P2PManagerImpl::FileGetExpectedSize(const string& file_id) {
  FilePath path = FileGetPath(file_id);
  if (path.empty())
    return -1;

  char ea_value[64] = {0};
  ssize_t ea_size;
  ea_size = getxattr(path.value().c_str(),
                     kCrosP2PFileSizeXAttrName,
                     &ea_value,
                     sizeof(ea_value) - 1);
  if (ea_size == -1) {
    PLOG(ERROR) << "Error calling getxattr() on file " << path.value();
    return -1;
  }

  char* endp = nullptr;
  long long int val = strtoll(ea_value, &endp, 0);  // NOLINT(runtime/int)
  if (*endp != '\0') {
    LOG(ERROR) << "Error parsing the value '" << ea_value << "' of the xattr "
               << kCrosP2PFileSizeXAttrName << " as an integer";
    return -1;
  }

  return val;
}

int P2PManagerImpl::CountSharedFiles() {
  int num_files = 0;

  FilePath p2p_dir = configuration_->GetP2PDir();
  const string ext_visible = GetExt(kVisible);
  const string ext_non_visible = GetExt(kNonVisible);

  base::FileEnumerator dir(p2p_dir, false, base::FileEnumerator::FILES);
  for (FilePath name = dir.Next(); !name.empty(); name = dir.Next()) {
    if (base::EndsWith(
            name.value(), ext_visible, base::CompareCase::SENSITIVE) ||
        base::EndsWith(
            name.value(), ext_non_visible, base::CompareCase::SENSITIVE)) {
      num_files += 1;
    }
  }

  return num_files;
}

void P2PManagerImpl::ScheduleEnabledStatusChange() {
  if (waiting_for_enabled_status_change_)
    return;

  Callback<void(EvalStatus, const bool&)> callback =
      Bind(&P2PManagerImpl::OnEnabledStatusChange, base::Unretained(this));
  update_manager_->AsyncPolicyRequest(
      callback, &Policy::P2PEnabledChanged, is_enabled_);
  waiting_for_enabled_status_change_ = true;
}

void P2PManagerImpl::OnEnabledStatusChange(EvalStatus status,
                                           const bool& result) {
  waiting_for_enabled_status_change_ = false;

  if (status == EvalStatus::kSucceeded) {
    if (result == is_enabled_) {
      LOG(WARNING) << "P2P enabled status did not change, which means that it "
                      "is permanent; not scheduling further checks.";
      waiting_for_enabled_status_change_ = true;
      return;
    }

    is_enabled_ = result;

    // If P2P is running but shouldn't be, make sure it isn't.
    if (may_be_running_ && !is_enabled_ && !EnsureP2PNotRunning()) {
      LOG(WARNING) << "Failed to stop P2P service.";
    }
  } else {
    LOG(WARNING)
        << "P2P enabled tracking failed (possibly timed out); retrying.";
  }

  ScheduleEnabledStatusChange();
}

P2PManager* P2PManager::Construct(Configuration* configuration,
                                  ClockInterface* clock,
                                  UpdateManager* update_manager,
                                  const string& file_extension,
                                  const int num_files_to_keep,
                                  const TimeDelta& max_file_age) {
  return new P2PManagerImpl(configuration,
                            clock,
                            update_manager,
                            file_extension,
                            num_files_to_keep,
                            max_file_age);
}

}  // namespace chromeos_update_engine
