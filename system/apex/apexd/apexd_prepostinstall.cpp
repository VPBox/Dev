/*
 * Copyright (C) 2018 The Android Open Source Project
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

#define LOG_TAG "apexd"

#include "apexd_prepostinstall.h"

#include <algorithm>
#include <vector>

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/scopeguard.h>
#include <android-base/strings.h>

#include "apex_file.h"
#include "apexd.h"
#include "apexd_private.h"
#include "apexd_utils.h"
#include "string_log.h"

namespace android {
namespace apex {

namespace {

void CloseSTDDescriptors() {
  // exec()d process will reopen STD* file descriptors as
  // /dev/null
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}

template <typename Fn>
Status StageFnInstall(const std::vector<ApexFile>& apexes, Fn fn,
                      const char* arg, const char* name) {
  // TODO: Support a session with more than one pre-install hook.
  const ApexFile* hook_file = nullptr;
  for (const ApexFile& f : apexes) {
    if (!(f.GetManifest().*fn)().empty()) {
      if (hook_file != nullptr) {
        return Status::Fail(StringLog() << "Missing support for multiple "
                                        << name << " hooks");
      }
      hook_file = &f;
    }
  }
  CHECK(hook_file != nullptr);
  LOG(VERBOSE) << name << " for " << hook_file->GetPath();

  std::vector<const ApexFile*> mounted_apexes;
  std::vector<std::string> activation_dirs;
  auto preinstall_guard = android::base::make_scope_guard([&]() {
    for (const ApexFile* f : mounted_apexes) {
      Status st = apexd_private::UnmountPackage(*f);
      if (!st.Ok()) {
        LOG(ERROR) << "Failed to unmount " << f->GetPath() << " after " << name
                   << ": " << st.ErrorMessage();
      }
    }
    for (const std::string& active_point : activation_dirs) {
      if (0 != rmdir(active_point.c_str())) {
        PLOG(ERROR) << "Could not delete temporary active point "
                    << active_point;
      }
    }
  });

  for (const ApexFile& apex : apexes) {
    // 1) Mount the package, if necessary.
    std::string mount_point =
        apexd_private::GetPackageMountPoint(apex.GetManifest());

    if (!apexd_private::IsMounted(apex.GetManifest().name(), apex.GetPath())) {
      Status mountStatus = apexd_private::MountPackage(apex, mount_point);
      if (!mountStatus.Ok()) {
        return mountStatus;
      }
      mounted_apexes.push_back(&apex);
    }

    // 2) Ensure there is an activation point, and we will clean it up.
    std::string active_point =
        apexd_private::GetActiveMountPoint(apex.GetManifest());
    if (0 == mkdir(active_point.c_str(), kMkdirMode)) {
      activation_dirs.emplace_back(std::move(active_point));
    } else {
      int saved_errno = errno;
      if (saved_errno != EEXIST) {
        return Status::Fail(StringLog()
                            << "Unable to create mount point" << active_point
                            << ": " << strerror(saved_errno));
      }
    }
  }

  // 3) Create invocation args.
  std::vector<std::string> args{
      "/system/bin/apexd", arg,
      hook_file->GetPath(),  // Make the APEX with hook first.
  };
  for (const ApexFile& apex : apexes) {
    if (&apex != hook_file) {
      args.push_back(apex.GetPath());
    }
  }

  std::string error_msg;
  int res = ForkAndRun(args, &error_msg);
  return res == 0 ? Status::Success() : Status::Fail(error_msg);
}

template <typename Fn>
int RunFnInstall(char** in_argv, Fn fn, const char* name) {
  // 1) Unshare.
  if (unshare(CLONE_NEWNS) != 0) {
    PLOG(ERROR) << "Failed to unshare() for apex " << name;
    _exit(200);
  }

  // 2) Make everything private, so that our (and hook's) changes do not
  //    propagate.
  if (mount(nullptr, "/", nullptr, MS_PRIVATE | MS_REC, nullptr) == -1) {
    PLOG(ERROR) << "Failed to mount private.";
    _exit(201);
  }

  std::string hook_path;
  {
    auto bind_fn = [&fn, name](const std::string& apex) {
      std::string hook;
      std::string mount_point;
      std::string active_point;
      {
        StatusOr<ApexFile> apex_file = ApexFile::Open(apex);
        if (!apex_file.Ok()) {
          LOG(ERROR) << "Could not open apex " << apex << " for " << name
                     << ": " << apex_file.ErrorMessage();
          _exit(202);
        }
        const ApexManifest& manifest = apex_file->GetManifest();
        hook = (manifest.*fn)();
        mount_point = apexd_private::GetPackageMountPoint(manifest);
        active_point = apexd_private::GetActiveMountPoint(manifest);
      }

      // 3) Activate the new apex.
      Status bind_status = apexd_private::BindMount(active_point, mount_point);
      if (!bind_status.Ok()) {
        LOG(ERROR) << "Failed to bind-mount " << mount_point << " to "
                   << active_point << ": " << bind_status.ErrorMessage();
        _exit(203);
      }

      return std::make_pair(active_point, hook);
    };

    // First/main APEX.
    auto [active_point, hook] = bind_fn(in_argv[2]);
    hook_path = active_point + "/" + hook;

    for (size_t i = 3;; ++i) {
      if (in_argv[i] == nullptr) {
        break;
      }
      bind_fn(in_argv[i]);  // Ignore result, hook will be empty.
    }
  }

  // 4) Run the hook.

  // For now, just run sh. But this probably needs to run the new linker.
  std::vector<std::string> args{
      hook_path,
  };
  std::vector<const char*> argv;
  argv.resize(args.size() + 1, nullptr);
  std::transform(args.begin(), args.end(), argv.begin(),
                 [](const std::string& in) { return in.c_str(); });

  LOG(ERROR) << "execv of " << android::base::Join(args, " ");

  // Close all file descriptors. They are coming from the caller, we do not
  // want to pass them on across our fork/exec into a different domain.
  CloseSTDDescriptors();

  execv(argv[0], const_cast<char**>(argv.data()));
  PLOG(ERROR) << "execv of " << android::base::Join(args, " ") << " failed";
  _exit(204);
}

}  // namespace

Status StagePreInstall(const std::vector<ApexFile>& apexes) {
  return StageFnInstall(apexes, &ApexManifest::preinstallhook, "--pre-install",
                        "pre-install");
}

int RunPreInstall(char** in_argv) {
  return RunFnInstall(in_argv, &ApexManifest::preinstallhook, "pre-install");
}

Status StagePostInstall(const std::vector<ApexFile>& apexes) {
  return StageFnInstall(apexes, &ApexManifest::postinstallhook,
                        "--post-install", "post-install");
}

int RunPostInstall(char** in_argv) {
  return RunFnInstall(in_argv, &ApexManifest::postinstallhook, "post-install");
}

}  // namespace apex
}  // namespace android
