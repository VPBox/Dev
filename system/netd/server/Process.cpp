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

#include "Process.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <chrono>
#include <sstream>

#include "android-base/stringprintf.h"
#define LOG_TAG "Netd"
#include "log/log.h"

#include "netdutils/Misc.h"
#include "netdutils/Slice.h"
#include "netdutils/Syscalls.h"
#include "netdutils/UniqueFd.h"

namespace android {

using base::StringPrintf;
using netdutils::DumpWriter;
using netdutils::Fd;
using netdutils::isOk;
using netdutils::makeCleanup;
using netdutils::makeSlice;
using netdutils::UniqueFd;

namespace net {
namespace process {
namespace {

const int PID_FILE_FLAGS = O_CREAT | O_TRUNC | O_WRONLY | O_NOFOLLOW | O_CLOEXEC;
const mode_t PID_FILE_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;  // mode 0644, rw-r--r--

// Set up during static initialization.
const std::chrono::steady_clock::time_point sStartTime = std::chrono::steady_clock::now();

std::chrono::milliseconds totalRunningTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - sStartTime);
}

std::string formatDuration(std::chrono::milliseconds duration) {
    auto hrs = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hrs;
    const auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= mins;
    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= secs;

    // No std::chrono::days until C++20.
    const unsigned days = hrs.count() / 24;
    hrs -= std::chrono::hours(days * 24);

    return StringPrintf("%ud%luh%lum%llus%llums",
            days, hrs.count(), mins.count(), secs.count(), duration.count());
}

}  // unnamed namespace

void blockSigPipe() {
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGPIPE);
    if (sigprocmask(SIG_BLOCK, &mask, nullptr) != 0) {
        ALOGW("WARNING: SIGPIPE not blocked\n");
    }
}

void writePidFile(const std::string& pidFile) {
    const std::string pid_buf(StringPrintf("%d\n", (int) getpid()));

    Fd pidFd = open(pidFile.c_str(), PID_FILE_FLAGS, PID_FILE_MODE);
    if (pidFd.get() == -1) {
        ALOGE("Unable to create pid file (%s)", strerror(errno));
        return;
    }

    const UniqueFd autoClosePidFile(pidFd);
    auto rmFile = makeCleanup([pidFile] { removePidFile(pidFile); });

    // File creation is affected by umask, so make sure the right mode bits are set.
    if (fchmod(pidFd.get(), PID_FILE_MODE) == -1) {
        ALOGE("failed to set mode 0%o on %s (%s)", PID_FILE_MODE, pidFile.c_str(), strerror(errno));
        return;
    }

    auto& sys = netdutils::sSyscalls.get();
    const auto rval = sys.write(pidFd, makeSlice(pid_buf));
    if (!isOk(rval.status()) || rval.value() != pid_buf.size()) {
        ALOGE("Unable to write to pid file (%s)", strerror(errno));
        return;
    }

    rmFile.release();  // Don't delete the pid file :-)
}

void removePidFile(const std::string& pidFile) {
    unlink(pidFile.c_str());
}

void dump(DumpWriter& dw) {
    std::stringstream out;
    out << "ppid:" << getppid() << " -> pid:" << getpid() << " -> tid:" << gettid() << '\n';
    const auto runningTime = totalRunningTime();
    out << "total running time: " << formatDuration(runningTime)
        << " (" << totalRunningTime().count() << "ms)" << '\n';

    struct rusage ru{};
    if (getrusage(RUSAGE_SELF, &ru) == 0) {
        out << "user: " << ru.ru_utime.tv_sec << "s" << (ru.ru_utime.tv_usec/1000) << "ms"
            << " sys: " << ru.ru_stime.tv_sec << "s" << (ru.ru_stime.tv_usec/1000) << "ms"
            << '\n';
        out << "maxrss: " << ru.ru_maxrss << "kB" << '\n';
    }

    dw.println(out.str());
}

}  // namespace process
}  // namespace net
}  // namespace android
