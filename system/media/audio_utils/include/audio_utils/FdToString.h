/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_FD_TO_STRING_H
#define ANDROID_AUDIO_FD_TO_STRING_H

#include <fcntl.h>
#include <future>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include <utils/Timers.h>

#include "clock.h"

namespace android {
namespace audio_utils {

/**
 * FdToString
 *
 * Captures string data written to a file descriptor.
 * The class will furnish a writable file descriptor by fd().
 * The string may be read through getStringAndClose().
 */

class FdToString {
public:
    /**
     * \param prefix is the prefix string prepended to each new line.
     * \param timeout is the total timeout to wait for obtaining data.
     */
    explicit FdToString(const std::string &prefix = "- ", int timeoutMs = 200)
            : mPrefix(prefix)
            , mTimeoutTimeNs(systemTime() + timeoutMs * NANOS_PER_MILLISECOND) {
        const int status = pipe2(mPipeFd, O_CLOEXEC);
        if (status == 0) {
            mOutput = std::async(std::launch::async, reader, mPipeFd[0], mTimeoutTimeNs, mPrefix);
        }
        // on initialization failure fd() returns -1.
    }

    ~FdToString() {
        for (auto &fd : mPipeFd) {
            if (fd >= 0) {
                close(fd);
                fd = -1;
            }
        }
    }

    /**
     * Returns the write end of the pipe as a file descriptor or -1 if invalid or already closed.
     *
     * Do not close this fd directly as this class should own the fd. Instead, use
     * getStringAndClose() to close the fd and return the string.
     */
    int fd() const {
        return mPipeFd[1];
    }

    /**
     * Returns the string representation of data written to the fd.
     *
     * An empty string is returned on failure (or timeout).  It is acceptable to call this
     * method multiple times to obtain the final string; the fd is closed after the first call.
     */
    std::string getStringAndClose() {
        if (!mOutput.valid()) return "";
        if (mPipeFd[1] >= 0) {
            close(mPipeFd[1]);
            mPipeFd[1] = -1;
        }
        const int waitMs = toMillisecondTimeoutDelay(systemTime(), mTimeoutTimeNs);
        std::future_status status = mOutput.wait_for(std::chrono::milliseconds(waitMs));
        return status == std::future_status::ready ? mOutput.get() : "";
    }

private:
    static std::string reader(int fd, int64_t timeoutTimeNs, std::string prefix) {
        char buf[4096];
        int red;
        std::stringstream ss;
        bool requiresPrefix = true;

        while (true) {
            struct pollfd pfd = {
                .fd = fd,
                .events = POLLIN | POLLRDHUP,
            };
            const int waitMs = toMillisecondTimeoutDelay(systemTime(), timeoutTimeNs);
            // ALOGD("waitMs: %d", waitMs);
            if (waitMs <= 0) break;
            const int retval = poll(&pfd, 1 /* nfds*/, waitMs);
            if (retval <= 0 || (pfd.revents & POLLIN) != POLLIN) break; // error or timeout
            // data is available
            if ((red = read(fd, buf, sizeof(buf))) <= 0) break;
            char *delim, *bptr = buf;
            while (!prefix.empty() && (delim = (char *)memchr(bptr, '\n', red)) != nullptr) {
                if (requiresPrefix) ss << prefix;
                const size_t line = delim - bptr + 1;
                ss.write(bptr, line);
                bptr += line;
                red -= line;
                requiresPrefix = true;
            }
            if (red > 0) {
                ss << prefix;
                ss.write(bptr, red);
                requiresPrefix = false;
            }
        }
        return ss.str();
    }

    const std::string mPrefix;
    const int64_t mTimeoutTimeNs;
    int mPipeFd[2] = {-1, -1};
    std::future<std::string> mOutput;
};

} // namespace audio_utils
} // namespace android

#endif // !ANDROID_AUDIO_FD_TO_STRING_H
