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

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <packagelistparser/packagelistparser.h>
#include <private/android_filesystem_config.h>
#include <scoped_minijail.h>
#include <selinux/android.h>

// simpleperf_app_runner is used to run simpleperf to profile apps with <profileable shell="true">
// on user devices. It works as below:
//   simpleperf cmds in shell -> simpleperf_app_runner -> /system/bin/simpleperf in app's context
//
// 1. User types simpleperf cmds in adb shell. If that is to profile an app, simpleperf calls
//    /system/bin/simpleperf_app_runner with profiling arguments.
// 2. simpleperf_app_runner checks if the app is profileable_from_shell. Then it switches the
//    process to the app's user id / group id, switches secontext to the app's domain, and
//    executes /system/bin/simpleperf with profiling arguments.
// 3. /system/bin/simpleperf records profiling data and writes profiling data to a file descriptor
//    opened by simpleperf cmds in shell.

struct PackageListCallbackArg {
  const char* name;
  pkg_info* info;
};

static bool PackageListParseCallback(pkg_info* info, void* userdata) {
  PackageListCallbackArg* arg = static_cast<PackageListCallbackArg*>(userdata);
  if (strcmp(arg->name, info->name) == 0) {
    arg->info = info;
    return false;
  }
  packagelist_free(info);
  return true;
}

pkg_info* ReadPackageInfo(const char* pkgname) {
  // Switch to package_info gid to read package info.
  gid_t old_egid = getegid();
  if (setegid(AID_PACKAGE_INFO) == -1) {
    error(1, errno, "setegid failed");
  }
  PackageListCallbackArg arg;
  arg.name = pkgname;
  arg.info = nullptr;
  if (!packagelist_parse(PackageListParseCallback, &arg)) {
    error(1, errno, "packagelist_parse failed");
  }
  if (setegid(old_egid) == -1) {
    error(1, errno, "setegid failed");
  }
  return arg.info;
}

std::vector<gid_t> GetSupplementaryGids(uid_t userAppId) {
  std::vector<gid_t> gids;
  int size = getgroups(0, &gids[0]);
  if (size < 0) {
    error(1, errno, "getgroups failed");
  }
  gids.resize(size);
  size = getgroups(size, &gids[0]);
  if (size != static_cast<int>(gids.size())) {
    error(1, errno, "getgroups failed");
  }
  // Profile guide compiled oat files (like /data/app/xxx/oat/arm64/base.odex) are not readable
  // worldwide (DEXOPT_PUBLIC flag isn't set). To support reading them (needed by simpleperf for
  // profiling), add shared app gid to supplementary groups.
  gid_t shared_app_gid = userAppId % AID_USER_OFFSET - AID_APP_START + AID_SHARED_GID_START;
  gids.push_back(shared_app_gid);
  return gids;
}

static void CheckSimpleperfArguments(const char* cmdname, char** args) {
  if (strcmp(cmdname, "stat") != 0 && strcmp(cmdname, "record") != 0 &&
      strcmp(cmdname, "api-collect") != 0) {
    error(1, 0, "cmd isn't allowed: %s", cmdname);
  }
  std::set<std::string> zero_arg_options = {
      "-b", "--csv", "--exit-with-parent", "-g", "--in-app", "--interval-only-values",
      "--no-callchain-joiner", "--no-dump-kernel-symbols", "--no-dump-symbols", "--no-inherit",
      "--post-unwind=no", "--post-unwind=yes", "--trace-offcpu", "--verbose",
  };
  std::set<std::string> one_arg_options = {
      "-c", "--call-graph", "--callchain-joiner-min-matching-nodes", "--clockid", "--cpu",
      "--cpu-percent", "--duration", "-e", "-f", "--group", "--interval", "-j", "--log", "-m",
      "-p", "--size-limit", "-t",
  };
  // options with a file descriptor
  std::set<std::string> fd_options = {
      "--start_profiling_fd", "--stop-signal-fd", "--out-fd",
  };
  // options with path from /data/local/tmp/
  std::set<std::string> path_options = {
      "--symfs", "--tracepoint-events",
  };
  one_arg_options.insert(fd_options.begin(), fd_options.end());
  one_arg_options.insert(path_options.begin(), path_options.end());
  for (int i = 0; args[i] != nullptr; ++i) {
    if (zero_arg_options.count(args[i])) {
      continue;
    } else if (one_arg_options.count(args[i])) {
      if (args[i + 1] == nullptr) {
        error(1, 0, "invalid arg: %s", args[i]);
      }
      if (fd_options.count(args[i])) {
        // Check if the file descriptor is valid.
        int fd;
        if (!android::base::ParseInt(args[i + 1], &fd) || fd < 3 || fcntl(fd, F_GETFD) == -1) {
          error(1, 0, "invalid fd for arg: %s", args[i]);
        }
      } else if (path_options.count(args[i])) {
        std::string path;
        if (!android::base::Realpath(args[i + 1], &path) ||
            !android::base::StartsWith(path, "/data/local/tmp/")) {
          error(1, 0, "invalid path for arg: %s", args[i]);
        }
      }
      ++i;
    } else {
      error(1, 0, "arg isn't allowed: %s", args[i]);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    error(1, 0, "usage: simpleperf_app_runner package_name simpleperf_cmd simpleperf_cmd_args...");
  }
  if (argc < 3) {
    error(1, 0, "no simpleperf command name");
  }
  char* pkgname = argv[1];
  char* simpleperf_cmdname = argv[2];
  int simpleperf_arg_start = 3;
  CheckSimpleperfArguments(simpleperf_cmdname, argv + simpleperf_arg_start);

  if (getuid() != AID_SHELL && getuid() != AID_ROOT) {
    error(1, 0, "program can only run from shell or root");
  }

  pkg_info* info = ReadPackageInfo(pkgname);
  if (info == nullptr) {
    error(1, 0, "failed to find package %s", pkgname);
  }
  if (info->uid < AID_APP_START || info->uid > AID_APP_END) {
    error(1, 0, "package isn't an application: %s", pkgname);
  }
  if (!info->profileable_from_shell) {
    error(1, 0, "package isn't profileable from shell: %s", pkgname);
  }

  // Switch to the app's user id and group id.
  uid_t uid = info->uid;
  gid_t gid = info->uid;
  std::vector<gid_t> supplementary_gids = GetSupplementaryGids(info->uid);
  ScopedMinijail j(minijail_new());
  minijail_change_uid(j.get(), uid);
  minijail_change_gid(j.get(), gid);
  minijail_set_supplementary_gids(j.get(), supplementary_gids.size(), &supplementary_gids[0]);
  minijail_enter(j.get());

  // Switch to the app's selinux context.
  if (selinux_android_setcontext(uid, 0, info->seinfo, pkgname) < 0) {
    error(1, errno, "couldn't set SELinux security context");
  }

  // Switch to the app's data directory.
  if (TEMP_FAILURE_RETRY(chdir(info->data_dir)) == -1) {
    error(1, errno, "couldn't chdir to package's data directory");
  }

  // Run /system/bin/simpleperf.
  std::string simpleperf_in_system_img = "/system/bin/simpleperf";
  int new_argc = 4 + argc - simpleperf_arg_start;
  char* new_argv[new_argc + 1];

  new_argv[0] = &simpleperf_in_system_img[0];
  new_argv[1] = simpleperf_cmdname;
  std::string app_option = "--app";
  new_argv[2] = &app_option[0];
  new_argv[3] = pkgname;
  for (int i = 4, j = simpleperf_arg_start; j < argc;) {
    new_argv[i++] = argv[j++];
  }
  new_argv[new_argc] = nullptr;
  execvp(new_argv[0], new_argv);
  error(1, errno, "exec failed");
}
