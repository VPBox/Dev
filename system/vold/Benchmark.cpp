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

#include "Benchmark.h"
#include "BenchmarkGen.h"
#include "VolumeManager.h"

#include <android-base/chrono_utils.h>
#include <android-base/file.h>
#include <android-base/logging.h>

#include <cutils/iosched_policy.h>
#include <hardware_legacy/power.h>
#include <private/android_filesystem_config.h>

#include <thread>

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

using android::base::ReadFileToString;
using android::base::WriteStringToFile;

namespace android {
namespace vold {

// Benchmark currently uses chdir(), which means we can only
// safely run one at a time.
static std::mutex kBenchmarkLock;

static const char* kWakeLock = "Benchmark";

// Reasonable cards are able to complete the create/run stages
// in under 20 seconds.
constexpr auto kTimeout = 20s;

// RAII class for boosting device performance during benchmarks.
class PerformanceBoost {
  private:
    int orig_prio;
    int orig_ioprio;
    IoSchedClass orig_clazz;

  public:
    PerformanceBoost() {
        errno = 0;
        orig_prio = getpriority(PRIO_PROCESS, 0);
        if (errno != 0) {
            PLOG(WARNING) << "Failed to getpriority";
            orig_prio = 0;
        }
        if (setpriority(PRIO_PROCESS, 0, -10) != 0) {
            PLOG(WARNING) << "Failed to setpriority";
        }
        if (android_get_ioprio(0, &orig_clazz, &orig_ioprio)) {
            PLOG(WARNING) << "Failed to android_get_ioprio";
            orig_ioprio = 0;
            orig_clazz = IoSchedClass_NONE;
        }
        if (android_set_ioprio(0, IoSchedClass_RT, 0)) {
            PLOG(WARNING) << "Failed to android_set_ioprio";
        }
    }

    ~PerformanceBoost() {
        if (android_set_ioprio(0, orig_clazz, orig_ioprio)) {
            PLOG(WARNING) << "Failed to android_set_ioprio";
        }
        if (setpriority(PRIO_PROCESS, 0, orig_prio) != 0) {
            PLOG(WARNING) << "Failed to setpriority";
        }
    }
};

static status_t benchmarkInternal(const std::string& rootPath,
                                  const android::sp<android::os::IVoldTaskListener>& listener,
                                  android::os::PersistableBundle* extras) {
    status_t res = 0;

    auto path = rootPath;
    path += "/misc";
    if (android::vold::PrepareDir(path, 01771, AID_SYSTEM, AID_MISC)) {
        return -1;
    }
    path += "/vold";
    if (android::vold::PrepareDir(path, 0700, AID_ROOT, AID_ROOT)) {
        return -1;
    }
    path += "/bench";
    if (android::vold::PrepareDir(path, 0700, AID_ROOT, AID_ROOT)) {
        return -1;
    }

    char orig_cwd[PATH_MAX];
    if (getcwd(orig_cwd, PATH_MAX) == NULL) {
        PLOG(ERROR) << "Failed getcwd";
        return -1;
    }
    if (chdir(path.c_str()) != 0) {
        PLOG(ERROR) << "Failed chdir";
        return -1;
    }

    sync();

    extras->putString(String16("path"), String16(path.c_str()));
    extras->putString(String16("ident"), String16(BenchmarkIdent().c_str()));

    // Always create
    {
        android::base::Timer timer;
        LOG(INFO) << "Creating " << path;
        res |= BenchmarkCreate([&](int progress) -> bool {
            if (listener) {
                listener->onStatus(progress, *extras);
            }
            return (timer.duration() < kTimeout);
        });
        sync();
        if (res == OK) extras->putLong(String16("create"), timer.duration().count());
    }

    // Only drop when we haven't aborted
    if (res == OK) {
        android::base::Timer timer;
        LOG(DEBUG) << "Before drop_caches";
        if (!WriteStringToFile("3", "/proc/sys/vm/drop_caches")) {
            PLOG(ERROR) << "Failed to drop_caches";
            res = -1;
        }
        LOG(DEBUG) << "After drop_caches";
        sync();
        if (res == OK) extras->putLong(String16("drop"), timer.duration().count());
    }

    // Only run when we haven't aborted
    if (res == OK) {
        android::base::Timer timer;
        LOG(INFO) << "Running " << path;
        res |= BenchmarkRun([&](int progress) -> bool {
            if (listener) {
                listener->onStatus(progress, *extras);
            }
            return (timer.duration() < kTimeout);
        });
        sync();
        if (res == OK) extras->putLong(String16("run"), timer.duration().count());
    }

    // Always destroy
    {
        android::base::Timer timer;
        LOG(INFO) << "Destroying " << path;
        res |= BenchmarkDestroy();
        sync();
        if (res == OK) extras->putLong(String16("destroy"), timer.duration().count());
    }

    if (chdir(orig_cwd) != 0) {
        PLOG(ERROR) << "Failed to chdir";
        return -1;
    }

    return res;
}

void Benchmark(const std::string& path,
               const android::sp<android::os::IVoldTaskListener>& listener) {
    std::lock_guard<std::mutex> lock(kBenchmarkLock);
    acquire_wake_lock(PARTIAL_WAKE_LOCK, kWakeLock);

    PerformanceBoost boost;
    android::os::PersistableBundle extras;

    status_t res = benchmarkInternal(path, listener, &extras);
    if (listener) {
        listener->onFinished(res, extras);
    }

    release_wake_lock(kWakeLock);
}

}  // namespace vold
}  // namespace android
